#include "stdafx.h"
#include "RunPlayer.h"

#include "RunPlayerWait.h"
#include "RunPlayerRun.h"
#include "RunPlayerDown_End.h"
#include "RunPlayerIdle1.h"

#include "RunObj_Obstacle.h"
#include "RunObj_Item.h"
#include "RunPlayerObserver.h"

CRunPlayer::CRunPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev)
	, m_pState(nullptr)
	, m_eMoveState(MOVESTATE_END)
	, m_dbAnimationSpeed(0.)
	, m_dbAniCtrlTime(0.)
	, m_pUIMgr(CUIMgr::GetInstance())
	, m_dProgress(0.)
	, m_eLineType(LINE_END)
	, m_fAccel(0.f)
	, m_bJump(false)
	, m_eSect(SECT_1)
	, m_dSavePonint(0.0)
	, m_bOneCreateUI(true)
	, m_iDeathCnt(0)
	, m_bGameStart(true)
	, m_dPlayerSpeed(2.0)
	, m_dLineAccel(1.0)
	, m_ePower(RUN_POWER_NORMAL)
	, m_dItem_durationTime_Big(0.0)
	, m_dItem_durationTime_SpeedUp(0.0)
	, m_dCollSlow(1.0)
	, m_dCollSlowTime(0.0)
	, m_bImmortal(false)
	, m_bShooting(false)
	, m_iPass(0)
	, m_bIsHit(false)
	, m_dHitTime(0)
{
	ZeroMemory(&m_vJump, sizeof(_vec3));
}

CRunPlayer::~CRunPlayer()
{
}

HRESULT CRunPlayer::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_Speed(1000.f);

	m_tBaseInfo.eObjectID = OBJECT_PLAYER;
	m_tBaseInfo.iMaxHp = 10000;
	m_tBaseInfo.iHp = 10000;
	m_tBaseInfo.iPrevHp = 10000;
	m_tBaseInfo.iSuperAmmor = 1000;
	m_tBaseInfo.iMaxSuperAmmor = 1000;
	Change_State(STATE_WAIT);
	m_eLineType = LINE_BOTTOM_MID;
	m_eMoveState = MOVESTATE_WAIT;

	LoadOuterCollider(L"../../Data/Collider/ApostleCollOuter.dat");
	LoadHitCollider(L"../../Data/Collider/ApostleCollHit.dat");

	ZeroMemory(&m_vJump, sizeof(_vec3));

	if (FAILED(Engine::AddSubject(Engine::RUNPLAYER)))
		return E_FAIL;

	return S_OK;
}

_int CRunPlayer::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CDynamicMeshObject::Update_GameObject(dTimeDelta);
	//CCollisionMgr::GetInstance()->Add_Collision(COLL_PLAYER, this);

	Update_State();	// ���� �ִϸ��̼� ���������� �������� üũ�ϴ� �Լ�
	m_pState->Update_State(dTimeDelta);	// ���� �������� ������Ʈ


	// ============== ����׿� COUT ======================
	cout << m_dProgress << endl;
	cout << m_tBaseInfo.iHp << endl;

	if (m_ePower == RUN_POWER_BIG)
	{
		cout << "���ӽð�: " << m_dItem_durationTime_Big - g_dAccumulatedTime << endl;
	}
	if (m_ePower == RUN_POWER_SPEEDUP)
	{
		cout << "���ӽð�: " << m_dItem_durationTime_SpeedUp - g_dAccumulatedTime << endl;
	}
	// ============== ����׿� COUT ======================


	// �浹�� �������� ��� ����
	if (m_dCollSlowTime < g_dAccumulatedTime)
		m_dCollSlow = 1.0;

	if (!m_bShooting)
	{
		ObserverNotify();	//������ ������Ʈ
		RunningFinish();	// �����ߴ��� Ȯ��
		Running(dTimeDelta);	// �޸���, �¿� ����
		Power_Big(dTimeDelta);	// �Ŵ�ȭ ������ �Ծ����� ũ�� ���ϰ� �ϴ� �Լ�
		Power_SpeedUp(dTimeDelta);	// ���ǵ�� ������ �Ծ����� �ӵ� �������� �Լ�
		CollisionCheck();	// �浹üũ�Լ�, �����̰��� �ϸ�ȴ�.
		CollisionItem();	// �������̶� �浹�ߴ��� Ȯ���ϴ� �Լ�
	}





	// =================================================================  �׽�Ʈ�� �ڵ� ============================================================

	// ================ �޸��� �غ� �Լ� ================== 
	if (Engine::KeyDown(DIK_1))
	{
		RunReady();
	}
	// ================ �޸��� ���� �Լ� ================== 
	if (Engine::KeyDown(DIK_2))
	{
		RunStart();
	}

	////���ӽ��۽� START ���� ���� �Ŀ� ���ӽ��۵ǰ� �ҷ��� ���� �߰�
	if (CUIMgr::GetInstance()->Get_RunGameStart())
	{
		RunStart();
		//�ð� �ٽ� ����
		m_pUIMgr->Set_RunGameTimeStop(false);
		
		m_pUIMgr->Set_RunGameStart(false);
	}

	// ġƮ�� ����/����
	if (Engine::KeyPressing(DIK_S))
	{
		if (CRunGameMgr::GetInstance()->Follow_Line(m_eLineType, &m_eMoveState, m_pTransformCom->Get_Info(Engine::INFO_POS), m_pTransformCom->Get_Info(Engine::INFO_UP), &m_dProgress, &m_dLineAccel, dTimeDelta))
		{
			m_dProgress -= (m_dPlayerSpeed / CRunGameMgr::GetInstance()->Get_LineLength(m_eSect, m_eLineType)) * m_dLineAccel * dTimeDelta * 300.f;

			Gravity(dTimeDelta);
		}
	}

	if (Engine::KeyDown(DIK_3))
	{
		Change_State(STATE_WAIT);
	}
	if (Engine::KeyDown(DIK_4))
	{
		CRunGameMgr::GetInstance()->Change_Line(&m_eLineType, &m_eMoveState, CRunGameMgr::KEY_LEFT, m_pTransformCom->Get_Info(Engine::INFO_POS), m_pTransformCom->Get_Info(Engine::INFO_UP), m_pTransformCom->Get_Angle(), &m_dProgress, &m_dLineAccel, dTimeDelta);
	}
	if (Engine::KeyDown(DIK_5))
	{
		CRunGameMgr::GetInstance()->Change_Line(&m_eLineType, &m_eMoveState, CRunGameMgr::KEY_RIGHT, m_pTransformCom->Get_Info(Engine::INFO_POS), m_pTransformCom->Get_Info(Engine::INFO_UP), m_pTransformCom->Get_Angle(), &m_dProgress, &m_dLineAccel, dTimeDelta);
	}
	if (Engine::KeyDown(DIK_6))
	{
		m_bImmortal = !m_bImmortal;
	}
	if (Engine::KeyDown(DIK_7))
	{
		--m_dPlayerSpeed;
	}
	if (Engine::KeyDown(DIK_8))
	{
		m_ePower = RUN_POWER_NORMAL;
	}
	if (Engine::KeyDown(DIK_9))
	{
		m_ePower = RUN_POWER_BIG;
	}
	if (Engine::KeyDown(DIK_0))
	{
		_vec3 vUp = m_pTransformCom->m_vInfo[Engine::INFO_UP];
		_vec3 vLook = m_pTransformCom->m_vInfo[Engine::INFO_LOOK];
		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Normalize(&vLook, &vLook);
		vUp *= 30.f;
		vLook *= 20.f;
		m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_RUNGAME_TELEPORT, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + vUp + vLook), &m_pTransformCom->m_vAngle);
	}



	if (Engine::KeyDown(DIK_B))
	{
		CCameraMgr::GetInstance()->Change_Camera(CCameraMgr::DYNAMIC);
	}
	if (Engine::KeyDown(DIK_V))
	{
		CCameraMgr::GetInstance()->Change_Camera(CCameraMgr::RUNGAME);
	}
	// =================================================================  �׽�Ʈ�� �ڵ� ============================================================

	return Engine::NO_EVENT;
}

_int CRunPlayer::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);
	Check_HitTime(dTimeDelta);
	if (MOVESTATE_DOWN == m_eMoveState)
	{
		if (m_bOneCreateUI)
		{
			////�ε�����

			//1. ��Ȱī��Ʈ �ٿ����
			m_pUIMgr->CreateRunPlayer_RevivalUI(m_pGraphicDev);
			//2. �ð�����
			m_pUIMgr->Set_RunGameTimeStop(true);
			//3.�״°� ī��Ʈüũ
			m_pUIMgr->SetAccumulatedDeath(1);

			m_bOneCreateUI = false;
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CRunPlayer::Render_Geometry(const _double & dTimeDelta)
{
	if (!m_bJump && m_eMoveState != MOVESTATE_FALL)
		m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta, m_dbAnimationSpeed * m_dPlayerSpeed * m_dLineAccel * m_dCollSlow); // �ִϸ��̼Ǽӵ� * �÷��̾�ӵ� * ���ΰ��ӵ� * �浹���

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
			pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
			pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
			pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
			pEffect->BeginPass(m_iPass);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}

	pEffect->End();
	Engine::Safe_Release(pEffect);

	CDynamicMeshObject::Render_Geometry(dTimeDelta);
}

void CRunPlayer::Render_PostEffect(const _double & dTimeDelta)
{
	//Shader
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->SetTexture("g_DiffuseTexture", nullptr);
			pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
			pEffect->BeginPass(1);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CRunPlayer::Render_Shadow(const _double & dTimeDelta)
{
	//Shader
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->BeginPass(6);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CRunPlayer::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_ApostleOfGreed"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CRunPlayer::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	if (m_bIsHit)
	{
		m_iPass = 19;
		_vec3 vCamPos = CCameraMgr::GetInstance()->Get_Pos();
		pEffect->SetValue("g_vCamPos", &vCamPos, sizeof(_vec3));
		_vec3 vPos = m_pTransformCom->Get_Info_byWorld(Engine::INFO_POS);
		pEffect->SetValue("g_vPosition", &vPos, sizeof(_vec3));
		pEffect->SetValue("g_vRimColor", &_vec3(1.f, 1.f, 0.f), sizeof(_vec3));
		pEffect->SetValue("g_vRimMinMax", &_vec2(0.01f, 0.1f), sizeof(_vec2));
	}
	else
		m_iPass = 0;
	return S_OK;
}

void CRunPlayer::Update_State()
{
	if (m_pDynamicMeshCom->Is_AnimationSetFinish(m_dbAniCtrlTime))
	{
		switch (m_eRunPlayerState)
		{
		case CRunPlayer::STATE_DYING:
		{
			m_bIsDead = true;
			break;
		}
		case CRunPlayer::STATE_DOWN_END:
		{
			break;
		}
		case CRunPlayer::STATE_DOWN_LOOP:
		{
			Change_State(STATE_DOWN_END);

			break;
		}
		case CRunPlayer::STATE_DOWN_START:
		{
			Change_State(STATE_DOWN_LOOP);
			break;
		}
		case CRunPlayer::STATE_IDLE_1:
		{
			Change_State(STATE_IDLE_1);
			break;
		}
		//case CRunPlayer::STATE_GROGGY_END:
		//{
		//	Change_State(STATE_WAIT);
		//	break;
		//}
		//case CRunPlayer::STATE_GROGGY_LOOP:
		//{
		//	Change_State(STATE_GROGGY_END);
		//	break;
		//}
		//case CRunPlayer::STATE_GROGGY_START:
		//{
		//	Change_State(STATE_GROGGY_LOOP);
		//	break;
		//}
		default:
			break;
		}
	}
}

void CRunPlayer::Change_State(RunPlayerStateEnum eState)
{
	if (m_eRunPlayerState != eState)
	{
		m_eRunPlayerState = eState;

		Engine::Safe_Release(m_pState);

		switch (m_eRunPlayerState)
		{
		case CRunPlayer::STATE_IDLE_1:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(12);
			m_pState = CRunPlayerIdle1::Create(m_pDynamicMeshCom, m_pTransformCom, &m_tBaseInfo);
			break;
		}
		case CRunPlayer::STATE_WAIT:
		{
			m_pState = CRunPlayerWait::Create(m_pDynamicMeshCom, m_pTransformCom, &m_tBaseInfo);
			break;
		}
		case CRunPlayer::STATE_RUN:
		{
			m_pState = CRunPlayerRun::Create(m_pDynamicMeshCom, m_pTransformCom, &m_tBaseInfo);
			break;
		}
		case CRunPlayer::STATE_DOWN_END: // END�� �ϸ� �̻��ؼ� ���� �ִϸ��̼��� LOOP�� �ٲ�, Ŭ�����̸��� �ٸ�!!!!!
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(9);
			m_pState = CRunPlayerDown_End::Create(m_pDynamicMeshCom, m_pTransformCom, &m_tBaseInfo);
			break;
		}
		default:
			break;
		}

		m_dbAnimationSpeed = m_pState->Get_AniSpeed();
		m_dbAniCtrlTime = m_pState->Get_AniCtrlTime();
	}
}

void CRunPlayer::Check_HitTime(const _double & dTimeDelta)
{
	if (m_dHitTime > 0)
	{
		m_dHitTime -= dTimeDelta;
		m_bIsHit = true;
	}

	if (m_dHitTime < 0)
	{
		m_dHitTime = 0;
		m_bIsHit = false;
	}
}

void CRunPlayer::ObserverNotify()
{
	Engine::Notify(Engine::RUNPLAYER, CRunPlayerObserver::HP, (void*)(&m_tBaseInfo.iHp));
	Engine::Notify(Engine::RUNPLAYER, CRunPlayerObserver::PREVHP, (void*)(&m_tBaseInfo.iPrevHp));
	Engine::Notify(Engine::RUNPLAYER, CRunPlayerObserver::MAXHP, (void*)(&m_tBaseInfo.iMaxHp));
	Engine::Notify(Engine::RUNPLAYER, CRunPlayerObserver::POWER, (void*)(&m_ePower));
	Engine::Notify(Engine::RUNPLAYER, CRunPlayerObserver::TIME_BIG, (void*)(&m_dItem_durationTime_Big));
	Engine::Notify(Engine::RUNPLAYER, CRunPlayerObserver::TIME_SPEEDUP, (void*)(&m_dItem_durationTime_SpeedUp));
}

void CRunPlayer::Running(const _double & dTimeDelta)
{
	// ================ �޸��� ================
	if (m_eRunPlayerState == STATE_RUN)
	{
		// ================ ������ ���� (�ڵ�) ================
		if (CRunGameMgr::GetInstance()->Follow_Line(m_eLineType, &m_eMoveState, m_pTransformCom->Get_Info(Engine::INFO_POS), m_pTransformCom->Get_Info(Engine::INFO_UP), &m_dProgress, &m_dLineAccel, dTimeDelta))
		{
			// FALL �����϶� ���� �ٷ�Ÿ������, ���系�� ���κ��� �Ʒ����ִٸ� ��Ÿ�� �ؾ��Ѵ�.
			m_dProgress += (m_dPlayerSpeed / CRunGameMgr::GetInstance()->Get_LineLength(m_eSect, m_eLineType)) * m_dLineAccel * m_dCollSlow * dTimeDelta * 300.f;
			Gravity(dTimeDelta);
		}
		// ================ ���� ================
		if (Engine::KeyDown(DIK_W))
		{
			if (m_bJump == false)
			{
				CSoundMgr::Get_Instance()->SiwonSoundOn(8);

				m_bJump = true;
				m_fAccel = 400.f;
			}
		}
		// ================ ���� �̵� ================
		if (Engine::KeyDown(DIK_D))
		{
			if (m_bJump == false && m_eMoveState != MOVESTATE_FALL)
			{
				// ����
				CSoundMgr::Get_Instance()->SiwonSoundOn(14);

				// ����Ʈ
				_vec3 vUp = m_pTransformCom->m_vInfo[Engine::INFO_UP];
				_vec3 vLook = m_pTransformCom->m_vInfo[Engine::INFO_LOOK];
				D3DXVec3Normalize(&vUp, &vUp);
				D3DXVec3Normalize(&vLook, &vLook);
				vUp *= 30.f;
				vLook *= 10.f;
				m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_RUNGAME_TELEPORT, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + vUp + vLook), &m_pTransformCom->m_vAngle);

				// �̵�
				CRunGameMgr::GetInstance()->Change_Line(&m_eLineType, &m_eMoveState, CRunGameMgr::KEY_RIGHT, m_pTransformCom->Get_Info(Engine::INFO_POS), m_pTransformCom->Get_Info(Engine::INFO_UP), m_pTransformCom->Get_Angle(), &m_dProgress, &m_dLineAccel, dTimeDelta);
			}
		}
		// ================ ������ �̵� ================
		if (Engine::KeyDown(DIK_A))
		{
			if (m_bJump == false && m_eMoveState != MOVESTATE_FALL)
			{
				// ����
				CSoundMgr::Get_Instance()->SiwonSoundOn(14);

				// ����Ʈ
				_vec3 vUp = m_pTransformCom->m_vInfo[Engine::INFO_UP];
				_vec3 vLook = m_pTransformCom->m_vInfo[Engine::INFO_LOOK];
				D3DXVec3Normalize(&vUp, &vUp);
				D3DXVec3Normalize(&vLook, &vLook);
				vUp *= 30.f;
				vLook *= 10.f;
				m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_RUNGAME_TELEPORT, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + vUp + vLook), &m_pTransformCom->m_vAngle);

				// �̵�
				CRunGameMgr::GetInstance()->Change_Line(&m_eLineType, &m_eMoveState, CRunGameMgr::KEY_LEFT, m_pTransformCom->Get_Info(Engine::INFO_POS), m_pTransformCom->Get_Info(Engine::INFO_UP), m_pTransformCom->Get_Angle(), &m_dProgress, &m_dLineAccel, dTimeDelta);
			}
		}
	}
}

void CRunPlayer::Gravity(const _double& dTimeDelta)
{
	// �������� ���� - �� * ���μ�Ÿ * �ð� - �߷� * �ð� * �ð� * 0.5f 
	// y = m_fPower * m_fAccel *1.f - GRAVITY * m_fAccel * m_fAccel * 0.5f;

	if (m_bJump)
	{
		_vec3 vUp;
		m_pTransformCom->Get_Info(Engine::INFO_UP, &vUp);
		D3DXVec3Normalize(&vUp, &vUp);

		m_vJump += vUp * m_fAccel * dTimeDelta;

		if (m_fAccel >= -400.f)
			m_fAccel -= 25.f;

		if (m_eMoveState == MOVESTATE_FALL)
		{
			_vec3 vDir = (m_pTransformCom->m_vInfo[Engine::INFO_POS] + m_vJump) - m_pTransformCom->m_vInfo[Engine::INFO_POS];

			_float fLength = D3DXVec3Length(&vDir);

			m_pTransformCom->Move_Pos(&m_vJump, 1.f, 1.f);

			if (fLength >= 500.f)
			{
				// �������� ������
				Change_State(STATE_DOWN_END);
				m_eMoveState = MOVESTATE_DOWN;
				m_vJump = { 0.f,0.f,0.f };
				m_bJump = false;
			}
		}
		else if (m_eMoveState == MOVESTATE_RUN || m_eMoveState == MOVESTATE_WAIT)
		{
			// ���ڸ������� �̷����ϸ� �ȵȴ�, �ۿ��� �ݵ�� ������ �¿�⶧���� ������ ����
			_vec3 vDir = (m_pTransformCom->m_vInfo[Engine::INFO_POS] + m_vJump) - m_pTransformCom->m_vInfo[Engine::INFO_POS];
			D3DXVec3Normalize(&vDir, &vDir);

			_float fDot = D3DXVec3Dot(&vDir, &vUp);

			if (fDot <= 0.f) // ��� -1�� �ؾ���. ���� Ÿ�� ������ �Ʒ��� �ִٴ� ��,	// �̷��� �ȵ�, �������±����� ������Ž
			{
				m_bJump = false;
				m_vJump = { 0.f,0.f,0.f };
				m_fAccel = 0.f;
				m_pTransformCom->Move_Pos(&m_vJump, 1.f, 1.f);
			}
			else
			{
				m_pTransformCom->Move_Pos(&m_vJump, 1.f, 1.f);
			}
		}

	}
	else if (m_eMoveState == MOVESTATE_FALL)
	{
		_vec3 vUp;
		m_pTransformCom->Get_Info(Engine::INFO_UP, &vUp);
		D3DXVec3Normalize(&vUp, &vUp);

		m_vJump += vUp * m_fAccel * dTimeDelta;

		if (m_fAccel >= -400.f)
			m_fAccel -= 25.f;

		m_pTransformCom->Move_Pos(&m_vJump, 1.f, 1.f);

		_vec3 vDir = (m_pTransformCom->m_vInfo[Engine::INFO_POS] + m_vJump) - m_pTransformCom->m_vInfo[Engine::INFO_POS];

		_float fLength = D3DXVec3Length(&vDir);
		if (fLength >= 500.f)
		{
			// �������� ������

			Change_State(STATE_DOWN_END);
			m_eMoveState = MOVESTATE_DOWN;
		}
	}
	if (m_bJump == false && m_eMoveState != MOVESTATE_FALL)
	{
		m_fAccel = 0.f;
		m_vJump = { 0.f,0.f,0.f };
	}
}

void CRunPlayer::RunReady()
{
	// ���̺� ����Ʈ�� �̵�
	SavePoint_Set();
	CRunGameMgr::GetInstance()->Follow_Line(m_eLineType, &m_eMoveState, m_pTransformCom->Get_Info(Engine::INFO_POS), m_pTransformCom->Get_Info(Engine::INFO_UP), &m_dProgress, &m_dLineAccel, 0.0);

	// ����,���� �ʱ�ȭ
	m_bJump = false;
	m_vJump = { 0.f,0.f,0.f };

	// ü��ȸ��
	m_tBaseInfo.iHp = m_tBaseInfo.iMaxHp;

	m_bOneCreateUI = true;
	Change_State(STATE_WAIT);
}

void CRunPlayer::RunStart()
{
	Change_State(STATE_RUN);	
}

void CRunPlayer::Power_Big(const _double & dTimeDelta)
{
	if (m_ePower == RUN_POWER_BIG  && (m_dItem_durationTime_Big < g_dAccumulatedTime))
	{
		m_ePower = RUN_POWER_NORMAL;
	}
	if (m_ePower == RUN_POWER_BIG)
	{
		if (m_pTransformCom->m_vScale.x <= 0.3f)
		{
			_vec3 vScale = m_pTransformCom->m_vScale;
			vScale.x += 0.3f * dTimeDelta;
			vScale.y += 0.3f * dTimeDelta;
			vScale.z += 0.3f * dTimeDelta;
			m_pTransformCom->Set_Scale(&vScale);
		}
	}
	else if (m_ePower != RUN_POWER_BIG)
	{
		if (m_pTransformCom->m_vScale.x > 0.1f)
		{
			_vec3 vScale = m_pTransformCom->m_vScale;
			vScale.x -= 0.3f * dTimeDelta;
			vScale.y -= 0.3f * dTimeDelta;
			vScale.z -= 0.3f * dTimeDelta;
			m_pTransformCom->Set_Scale(&vScale);
		}
		else
		{
			m_pTransformCom->Set_Scale(&_vec3(0.1f, 0.1f, 0.1f));
		}
	}
}

void CRunPlayer::Power_SpeedUp(const _double & dTimeDelta)
{
	if (m_ePower == RUN_POWER_SPEEDUP && (m_dItem_durationTime_SpeedUp < g_dAccumulatedTime))
	{
		m_ePower = RUN_POWER_NORMAL;
	}

	if (m_ePower == RUN_POWER_SPEEDUP)
	{
		m_dPlayerSpeed = 4.0;
	}
	else if (m_ePower != RUN_POWER_SPEEDUP)
	{
		m_dPlayerSpeed = 2.0;
	}
}

void CRunPlayer::Item_Looting(RUNGAME_ITEMTYPE eItemType)
{
	CSoundMgr::Get_Instance()->SiwonSoundOn(13);

	switch (eItemType)
	{
	case RUNITEM_MONEY:	// ���� ȹ��
	{
		// ���� 1,000��
		break;
	}
	case RUNITEM_JEWELRY_BLUE:	// ���� ȹ��
	{
		// ���� 10,000��
		break;
	}
	case RUNITEM_POTION:	// �÷��̾� ü�� ȸ��
	{
		// ���� 5,000��
		m_tBaseInfo.iHp = m_tBaseInfo.iMaxHp;

		break;
	}
	case RUNITEM_POWER:	// �÷��̾� ũ�� Ŀ���� ������
	{
		// ���� 5,000��
		m_ePower = RUN_POWER_BIG;
		m_dItem_durationTime_Big = g_dAccumulatedTime + 5.0;

		CSoundMgr::Get_Instance()->SiwonSoundOn(12);

		break;
	}
	case RUNITEM_SPEED:	// �÷��̾� �ӵ� �������� ������
	{
		// ���� 5,000��
		m_ePower = RUN_POWER_SPEEDUP;
		m_dItem_durationTime_SpeedUp = g_dAccumulatedTime + 3.0;

		break;
	}
	case RUNITEM_END:
		break;
	default:
		break;
	}
}

void CRunPlayer::SavePoint_Set()
{
	if (m_dProgress >= 1.0)
	{

	}
	else if (m_dProgress > 0.521)
	{
		m_dProgress = 0.521;
		m_eLineType = LINE_WALL_RIGHT_MID;
		m_pTransformCom->m_vAngle.z = 90.f;
	}
	else if (m_dProgress > 0.365)
	{
		m_dProgress = 0.365;
		m_eLineType = LINE_TOP_MID;
		m_pTransformCom->m_vAngle.z = 180.f;
	}
	else
	{
		m_dProgress = 0.0;
		m_eLineType = LINE_BOTTOM_MID;
		m_pTransformCom->m_vAngle.z = 0.f;
	}

	return;

}

void CRunPlayer::RunningFinish()
{
	if (m_dProgress >= 1.0 && m_eMoveState != MOVESTATE_FINISH)
	{
		Change_State(STATE_IDLE_1);
		m_eMoveState = MOVESTATE_FINISH;
		// call cam cutscene
		CCameraMgr::GetInstance()->Set_ItemGet();
	}
}

void CRunPlayer::CollisionCheck()
{
	if (m_eRunPlayerState != STATE_RUN)
		return;

	list<CGameObject*> listObj = CRunGameMgr::GetInstance()->Get_ObstacleList();
	
	for (auto& iter : listObj)
	{
		CRunObj_Obstacle* pObj = dynamic_cast<CRunObj_Obstacle*>(iter);
		if ((pObj->Get_LineSect() == m_eSect) && (pObj->Get_LineType() == m_eLineType))
		{
			_double dObjProgress = pObj->Get_Progress();
			_double dObjRadius = pObj->Get_Radius();

			if ((!m_bJump) && ((pObj->Get_CollType() == OBJ_COLL_RUN) || (pObj->Get_CollType() == OBJ_COLL_All)))
			{
				// �浹üũ
				if ((m_dProgress >= dObjProgress - dObjRadius) && (m_dProgress <= dObjProgress + dObjRadius))
				{
					if (m_ePower == RUN_POWER_NORMAL || m_ePower == RUN_POWER_SPEEDUP)
					{
						pObj->Set_Collision(RUN_POWER_NORMAL);
						CollisionResult();
						CRunGameMgr::GetInstance()->NullCheck_ObstacleList_Dead();
					}
					else if (m_ePower == RUN_POWER_BIG)
					{
						pObj->Set_Collision(RUN_POWER_BIG);
						CRunGameMgr::GetInstance()->NullCheck_ObstacleList();	// ���ư� �ִ� ���̻� �浹�� �ʿ� ���⶧���� �̸� ���� ��.
					}
				}
			}
			else if ((m_bJump) && ((pObj->Get_CollType() == OBJ_COLL_JUMP) || (pObj->Get_CollType() == OBJ_COLL_All)))
			{
				// �浹üũ
				if ((m_dProgress >= dObjProgress - dObjRadius) && (m_dProgress <= dObjProgress + dObjRadius))
				{
					if (m_ePower == RUN_POWER_NORMAL || m_ePower == RUN_POWER_SPEEDUP)
					{
						pObj->Set_Collision(RUN_POWER_NORMAL);
						CollisionResult();
						CRunGameMgr::GetInstance()->NullCheck_ObstacleList_Dead();
					}
					else if (m_ePower == RUN_POWER_BIG)
					{
						pObj->Set_Collision(RUN_POWER_BIG);
						CRunGameMgr::GetInstance()->NullCheck_ObstacleList();	// ���ư� �ִ� ���̻� �浹�� �ʿ� ���⶧���� �̸� ���� ��.
					}
				}
			}
		}
	}
}

void CRunPlayer::CollisionItem()
{
	if (m_eRunPlayerState != STATE_RUN)
		return;

	list<CGameObject*> listObj = CRunGameMgr::GetInstance()->Get_ItemList();

	for (auto& iter : listObj)
	{
		CRunObj_Item* pObj = dynamic_cast<CRunObj_Item*>(iter);
		if ((pObj->Get_LineSect() == m_eSect) && (pObj->Get_LineType() == m_eLineType))
		{
			if (m_ePower == RUN_POWER_NORMAL || m_ePower == RUN_POWER_SPEEDUP)
			{
				_double dObjProgress = pObj->Get_Progress();
				_double dObjRadius = pObj->Get_Radius();

				if ((!m_bJump) && ((pObj->Get_CollType() == OBJ_COLL_RUN) || (pObj->Get_CollType() == OBJ_COLL_All)))
				{
					// �浹üũ
					if ((m_dProgress >= dObjProgress - dObjRadius) && (m_dProgress <= dObjProgress + dObjRadius))
					{
						pObj->Set_Collision(RUN_POWER_NORMAL);
						Item_Looting(pObj->Get_ItemType());
					}
				}
				else if ((m_bJump) && ((pObj->Get_CollType() == OBJ_COLL_JUMP) || (pObj->Get_CollType() == OBJ_COLL_All)))
				{
					// �浹üũ
					if ((m_dProgress >= dObjProgress - dObjRadius) && (m_dProgress <= dObjProgress + dObjRadius))
					{
						pObj->Set_Collision(RUN_POWER_NORMAL);
						Item_Looting(pObj->Get_ItemType());
					}
				}
			}
			else if (m_ePower == RUN_POWER_BIG)
			{
				_double dObjProgress = pObj->Get_Progress();
				_double dObjRadius = pObj->Get_Radius();

				// �浹üũ
				if ((m_dProgress >= dObjProgress - dObjRadius) && (m_dProgress <= dObjProgress + dObjRadius))
				{
					pObj->Set_Collision(RUN_POWER_BIG);
					Item_Looting(pObj->Get_ItemType());
				}
			}
		}
	}
	CRunGameMgr::GetInstance()->NullCheck_ItemList();
}

void CRunPlayer::CollisionResult()
{
	if (!m_bImmortal)
		m_tBaseInfo.iHp -= 10;

	m_dCollSlow = 0.4;
	m_dCollSlowTime = g_dAccumulatedTime + 1.0;
	m_dHitTime = 0.1;
	if (m_tBaseInfo.iHp <= 0)
	{
		Change_State(STATE_DOWN_END);
		m_eMoveState = MOVESTATE_DOWN;
		CSoundMgr::Get_Instance()->SiwonSoundOn(9);
	}
	else
	{
		CSoundMgr::Get_Instance()->SiwonSoundOn(11);
	}
}

CRunPlayer * CRunPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CRunPlayer* pInstance = new CRunPlayer(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunPlayer::Free()
{
	Engine::Safe_Release(m_pState);
	CDynamicMeshObject::Free();
}
