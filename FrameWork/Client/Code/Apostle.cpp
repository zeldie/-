#include "stdafx.h"
#include "Apostle.h"
#include "PlayerObserver.h"
#include "ApostleObserver.h"

// 상태패턴
#include "ApostleState.h"
#include "ApostleStateDying.h"
#include "ApostleStateWait.h"
#include "ApostleStateGroggyStart.h"
#include "ApostleStateGroggyLoop.h"
#include "ApostleStateGroggyEnd.h"
#include "ApostleStateDownStart.h"
#include "ApostleStateDownLoop.h"
#include "ApostleStateDownEnd.h"
#include "ApostleStatePullSwing.h"
#include "ApostleStateJumpTurnSpike.h"
#include "ApostleStateOverSwing.h"
#include "ApostleStateSpike.h"
#include "ApostleStateJumpAttack.h"
#include "ApostleStatePowerSpike.h"
#include "ApostleStatePray.h"
#include "ApostleStateSpinStart.h"
#include "ApostleStateSpinLoop.h"
#include "ApostleStateSpinEnd.h"
#include "ApostleStateJumpHooray.h"
#include "ApostleStatePowerDown.h"
#include "ApostleStateIdle1.h"
#include "ApostleStateIdle2.h"
#include "ApostleStatePoint.h"

CApostle::CApostle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev)
	, m_pPlayerObserver(nullptr)
	, m_pState(nullptr)
	, m_eApostleState(STATE_END)
	, m_dbAnimationSpeed(0.)
	, m_dbAniCtrlTime(0.)
	, m_iPhase(0)
	, m_dbIdleTime(0.)
	, m_dbAddDelay(0.)
	, m_iPatternLoop(0)
	, m_pUIMgr(CUIMgr::GetInstance())
	, m_iPattern(0)
	, m_bIsStart(false)
	, m_bCutscene(false)
	, m_dbCutscene(0.0)
	, m_iPass(19)
	, m_bDying(false)
	, m_bIsHit(false)
	, m_dHitTime(0)
	, m_bCreateUI(false)
	, m_bIsRender(false)
	, m_dDissolveCountDown(0)
	, m_bDissolveStart(false)
{

}

CApostle::~CApostle()
{
}

HRESULT CApostle::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);

	m_pPlayerObserver = CPlayerObserver::Create();
	if (m_pPlayerObserver == nullptr)
		return E_FAIL;
	Engine::Subscribe(Engine::PLAYER, m_pPlayerObserver);

	m_tBaseInfo.eObjectID = OBJECT_MONSTER;
	m_tBaseInfo.iMaxHp = 400;
	m_tBaseInfo.iHp = 400;
	m_tBaseInfo.iPrevHp = 400;
	m_tBaseInfo.iSuperAmmor = 1000;
	m_tBaseInfo.iMaxSuperAmmor = 1000;
	m_tBaseInfo.iCheatATK = 0;
	m_iPhase = 1;
	m_dbAddDelay = 0.5;
	Change_State(STATE_WAIT);

	LoadOuterCollider(L"../../Data/Collider/ApostleCollOuter.dat");
	LoadHitCollider(L"../../Data/Collider/ApostleCollHit.dat");

	if (FAILED(Engine::AddSubject(Engine::APOSTLE)))
		return E_FAIL;

	return S_OK;
}

_int CApostle::Update_GameObject(const _double & dTimeDelta)
{
	// yh test code
	m_tBaseInfo.vObjectPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	// yh test end
	if (m_bDying)
	{
		if (!m_bCreateUI)
		{
			m_pUIMgr->Set_CreateShootingResultUI(true);
			m_pUIMgr->Set_StageTimeStop();
			m_bCreateUI = true;
		}
	}

	//CDynamicMeshObject::Link_NaviMesh();
	CDynamicMeshObject::Update_GameObject(dTimeDelta);
	CCollisionMgr::GetInstance()->Add_Collision(COLL_MONSTER, this);

	//Change_State(STATE_IDLE_1);
	Update_State();
	m_pState->Update_State(dTimeDelta);
	Update_Pattern();
	ObserverNotify();

	/*//cout << "슈퍼아머 : "<<m_tBaseInfo.iSuperAmmor << endl;
	//cout << "체력 : "<<m_tBaseInfo.iHp << endl;*/

	for (auto iter = m_mapinvincibility.begin(); iter != m_mapinvincibility.end();)
	{
		iter->second -= dTimeDelta;
		if (iter->second <= 0)
		{
			m_mapinvincibility.erase(iter++);
		}
		else
		{
			iter++;
		}
	}

	////cout << D3DXVec3Length(&(m_pTransformCom->m_vInfo[Engine::INFO_UP] - m_pPlayerObserver->Get_Pos())) << endl;
	m_dbCutscene += dTimeDelta;
	if (1 <= m_dbCutscene)
	{
		if (D3DXVec3Length(&(m_pTransformCom->m_vInfo[Engine::INFO_UP] - m_pPlayerObserver->Get_Pos())) <= 1000.f)
		{
			if (!m_bCutscene)
			{
				Change_State(STATE_IDLE_2);
				// cutscene cam 
				CCameraMgr::GetInstance()->Set_ApostleCutscene();
				m_bCutscene = true;
			}
		}
	}
	// ==  테스트용 코드 ============================================================
	if (Engine::KeyDown(DIK_1))
	{
		Change_State(STATE_WAIT);
		m_dbIdleTime = g_dAccumulatedTime + m_dbAddDelay;
	}
	if (Engine::KeyDown(DIK_2))
	{
		Change_State(STATE_IDLE_2);
	}
	if (Engine::KeyDown(DIK_3))
	{
		Change_State(STATE_IDLE_1);
	}
	if (Engine::KeyDown(DIK_4))
	{
		Change_State(STATE_SKILL_4_POINT);

	}
	if (Engine::KeyDown(DIK_5))
	{
		m_bIsStart = false;
	}
	if (Engine::KeyDown(DIK_6))
	{
		Change_State(STATE_SKILL_7_JUMP_ATTACK);
	}
	if (Engine::KeyDown(DIK_7))
	{
		Change_State(STATE_SKILL_8_POWER_SPIKE);
	}
	if (Engine::KeyDown(DIK_8))
	{
		Change_State(STATE_SKILL_11_JUMP_HOORAY);
	}
	if (Engine::KeyDown(DIK_9))
	{
		Change_State(STATE_SKILL_12_POWER_DOWN);
	}
	// ==  테스트용 코드 ============================================================

	return Engine::NO_EVENT;
}

_int CApostle::LateUpdate_GameObject(const _double & dTimeDelta)
{
	// yh test code
	m_tBaseInfo.vObjectPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	// yh test end
	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);
	Check_HitTime(dTimeDelta);
	if (!m_bIsRender)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CApostle::Render_Geometry(const _double & dTimeDelta)
{
	if(!m_bDissolveStart)
		m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta, m_dbAnimationSpeed);

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect, dTimeDelta)))
		return;
	if (m_bDissolveStart)
	{
		Engine::SetTexture(pEffect, "g_DissolveTexture", 6);
		Engine::SetTexture(pEffect, "g_DissolveEdgeTexture", 2);
		m_dDissolveCountDown += dTimeDelta * 0.5;
		pEffect->SetFloat("fTime", m_dDissolveCountDown);
		m_iPass = 14;
		if (m_dDissolveCountDown > 1)
			m_bDying = true;
	}
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

void CApostle::Render_PostEffect(const _double & dTimeDelta)
{
	if (!m_bDissolveStart)
	{
		//Shader
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		if (pEffect == nullptr)
			return;
		Engine::Safe_AddRef(pEffect);
		if (FAILED(Setup_ShaderProps(pEffect, dTimeDelta)))
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
}

void CApostle::Render_Shadow(const _double & dTimeDelta)
{
	if (!m_bDissolveStart)
	{
		//Shader
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		if (pEffect == nullptr)
			return;
		Engine::Safe_AddRef(pEffect);
		if (FAILED(Setup_ShaderProps(pEffect, dTimeDelta)))
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
}

HRESULT CApostle::Clone_Component()
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

HRESULT CApostle::Setup_ShaderProps(LPD3DXEFFECT & pEffect, const _double& dTimeDelta)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	if (m_bIsHit)
	{
		m_iPass = 19;
		_vec3 vCamPos = CCameraMgr::GetInstance()->Get_Pos();
		pEffect->SetValue("g_vCamPos", &vCamPos, sizeof(_vec3));
		_vec3 vPos = m_pTransformCom->Get_Info_byWorld(Engine::INFO_POS);
		pEffect->SetValue("g_vPosition", &vPos, sizeof(_vec3));
		pEffect->SetValue("g_vRimColor", &_vec3(0.1f, 0.1f, 0.f), sizeof(_vec3));
		pEffect->SetValue("g_vRimMinMax", &_vec2(0.01f, 0.1f), sizeof(_vec2));
	}
	else
		m_iPass = 0;
	return S_OK;
}

void CApostle::Update_State()
{
	if (m_pDynamicMeshCom->Is_AnimationSetFinish(m_dbAniCtrlTime))
	{
		switch (m_eApostleState)
		{
		case CApostle::STATE_DYING:
		{
			m_bDissolveStart = true;
			break;
		}
		case CApostle::STATE_IDLE_1:
		{
			Change_State(STATE_SKILL_4_POINT);
			break;
		}
		case CApostle::STATE_IDLE_2:
		{
			Change_State(STATE_IDLE_1);
			break;
		}
		case CApostle::STATE_RUN:
			break;
		case CApostle::STATE_START:
			break;
		case CApostle::STATE_TURN:
			break;
		case CApostle::STATE_WAIT:
			break;
		case CApostle::STATE_WALK:
			break;
		case CApostle::STATE_DOWN_END:
		{
			m_tBaseInfo.iSuperAmmor = m_tBaseInfo.iMaxSuperAmmor;
			Change_State(STATE_WAIT);
			break;
		}
		case CApostle::STATE_DOWN_LOOP:
		{
			if (m_dbIdleTime < g_dAccumulatedTime)
			{
				Change_State(STATE_DOWN_END);
			}
			break;
		}
		case CApostle::STATE_DOWN_START:
		{
			Change_State(STATE_DOWN_LOOP);
			m_dbIdleTime = g_dAccumulatedTime + 5.;
			break;
		}
		case CApostle::STATE_GROGGY_END:
		{
			Change_State(STATE_WAIT);
			break;
		}
		case CApostle::STATE_GROGGY_LOOP:
		{
			if (m_dbIdleTime < g_dAccumulatedTime)
			{
				Change_State(STATE_GROGGY_END);
			}
			break;
		}
		case CApostle::STATE_GROGGY_START:
		{
			Change_State(STATE_GROGGY_LOOP);
			m_dbIdleTime = g_dAccumulatedTime + 4.;
			break;
		}
		case CApostle::STATE_SKILL_0_PULLSWING:
		{
			Change_State(STATE_WAIT);
			m_dbIdleTime = g_dAccumulatedTime + m_dbAddDelay;
			break;
		}
		case CApostle::STATE_SKILL_1_JUMP_TURN_SPIKE:
		{
			Change_State(STATE_WAIT);
			m_dbIdleTime = g_dAccumulatedTime + m_dbAddDelay;
			break;
		}
		case CApostle::STATE_SKILL_2_TRUN_SWING:
			break;
		case CApostle::STATE_SKILL_3_OVERSWING:
		{
			Change_State(STATE_WAIT);
			m_dbIdleTime = g_dAccumulatedTime + m_dbAddDelay;
			break;
		}
		case CApostle::STATE_SKILL_4_POINT:
		{
			Change_State(STATE_WAIT);
			m_bIsStart = true;

			////////희정 추가 
			m_pUIMgr->CreateBattleUI(m_pGraphicDev, CUIMgr::UITYPE_SHOOTING);
			m_pUIMgr->CreatePlayerInfoUI(m_pGraphicDev, nullptr);
			m_pUIMgr->Set_StageTimeStart();

			break;
		}
		case CApostle::STATE_SKILL_5_SPIKE:
		{
			Change_State(STATE_WAIT);
			m_dbIdleTime = g_dAccumulatedTime + m_dbAddDelay;
			break;
		}
		case CApostle::STATE_SKILL_6_END:
			break;
		case CApostle::STATE_SKILL_6_LOOP:
			break;
		case CApostle::STATE_SKILL_6_START:
			break;
		case CApostle::STATE_SKILL_6_STOP:
			break;
		case CApostle::STATE_SKILL_7_JUMP_ATTACK:
		{
			Change_State(STATE_WAIT);
			m_dbIdleTime = g_dAccumulatedTime + m_dbAddDelay;
			break;
		}
		case CApostle::STATE_SKILL_8_POWER_SPIKE:
		{
			Change_State(STATE_WAIT);
			m_dbIdleTime = g_dAccumulatedTime + m_dbAddDelay;
			break;
		}
		case CApostle::STATE_SKILL_9_PRAY:
		{
			Change_State(STATE_WAIT);
			m_dbIdleTime = g_dAccumulatedTime + m_dbAddDelay;
			break;
		}
		case CApostle::STATE_SKILL_10_SPIN_END:
		{
			Change_State(STATE_WAIT);
			m_dbIdleTime = g_dAccumulatedTime + m_dbAddDelay;
			break;
		}
		case CApostle::STATE_SKILL_10_SPIN_LOOP:
		{
			if (m_dbIdleTime < g_dAccumulatedTime)
			{
				Change_State(STATE_SKILL_10_SPIN_END);
			}
			break;
		}
		case CApostle::STATE_SKILL_10_SPIN_START:
		{
			Change_State(STATE_SKILL_10_SPIN_LOOP);
			m_dbIdleTime = g_dAccumulatedTime + 5.;
			break;
		}
		case CApostle::STATE_SKILL_11_JUMP_HOORAY:
		{
			Change_State(STATE_WAIT);
			m_dbIdleTime = g_dAccumulatedTime + m_dbAddDelay;
			break;
		}
		case CApostle::STATE_SKILL_12_POWER_DOWN:
		{
			Change_State(STATE_WAIT);
			m_dbIdleTime = g_dAccumulatedTime + m_dbAddDelay;
			break;
		}
		case CApostle::STATE_END:
			break;
		default:
			break;
		}
	}
}

void CApostle::Change_State(ApostleStateEnum eState)
{
	if (m_eApostleState != eState)
	{
		m_eApostleState = eState;

		Engine::Safe_Release(m_pState);

		switch (m_eApostleState)
		{
		case CApostle::STATE_DYING:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(9);
			m_pState = CApostleStateDying::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver,&m_tBaseInfo);
			break;
		}
		case CApostle::STATE_IDLE_1:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(12);
			m_pState = CApostleStateIdle1::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_IDLE_2:
		{
			m_pState = CApostleStateIdle2::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_RUN:
			break;
		case CApostle::STATE_START:
			break;
		case CApostle::STATE_TURN:
			break;
		case CApostle::STATE_WAIT:
		{
			m_pState = CApostleStateWait::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_WALK:
			break;
		case CApostle::STATE_DOWN_END:
		{
			m_pState = CApostleStateDownEnd::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_DOWN_LOOP:
		{
			m_pState = CApostleStateDownLoop::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_DOWN_START:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(10);
			m_pState = CApostleStateDownStart::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_GROGGY_END:
		{
			m_pState = CApostleStateGroggyEnd::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_GROGGY_LOOP:
		{
			m_pState = CApostleStateGroggyLoop::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_GROGGY_START:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(11);
			m_pState = CApostleStateGroggyStart::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_0_PULLSWING:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(0);
			m_pState = CApostleStatePullSwing::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_1_JUMP_TURN_SPIKE:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(1);
			m_pState = CApostleStateJumpTurnSpike::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_2_TRUN_SWING:
			break;
		case CApostle::STATE_SKILL_3_OVERSWING:
		{
			m_pState = CApostleStateOverSwing::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_4_POINT:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(3);
			m_pState = CApostleStatePoint::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_5_SPIKE:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(4);
			m_pState = CApostleStateSpike::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_6_END:
			break;
		case CApostle::STATE_SKILL_6_LOOP:
			break;
		case CApostle::STATE_SKILL_6_START:
			break;
		case CApostle::STATE_SKILL_6_STOP:
			break;
		case CApostle::STATE_SKILL_7_JUMP_ATTACK:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(5);
			m_pState = CApostleStateJumpAttack::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_8_POWER_SPIKE:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(6);
			m_pState = CApostleStatePowerSpike::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_9_PRAY:
		{
			m_pState = CApostleStatePray::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_10_SPIN_END:
		{
			m_pState = CApostleStateSpinEnd::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_10_SPIN_LOOP:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(7);
			m_pState = CApostleStateSpinLoop::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_10_SPIN_START:
		{
			m_pState = CApostleStateSpinStart::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_11_JUMP_HOORAY:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(12);
			m_pState = CApostleStateJumpHooray::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_SKILL_12_POWER_DOWN:
		{
			CSoundMgr::Get_Instance()->SiwonSoundOn(8);
			m_pState = CApostleStatePowerDown::Create(m_pDynamicMeshCom, m_pTransformCom, m_pPlayerObserver, &m_tBaseInfo);
			break;
		}
		case CApostle::STATE_END:
			break;
		default:
			break;
		}

		m_dbAnimationSpeed = m_pState->Get_AniSpeed();
		m_dbAniCtrlTime = m_pState->Get_AniCtrlTime();
	}
}

void CApostle::Update_Pattern()
{
	// 죽음
	if (m_tBaseInfo.iHp <= 0)
	{
		Change_State(STATE_DYING);
		return;
	}

	if (!m_bIsStart)
		return;

	//  페이즈
	if (m_iPhase == 1 && m_tBaseInfo.iHp <= m_tBaseInfo.iMaxHp *0.5f)
	{
		m_iPhase = 2;
	}

	// 다운, 기절
	if ((m_tBaseInfo.iSuperAmmor <= 0) && (m_eApostleState != STATE_GROGGY_START) && (m_eApostleState != STATE_GROGGY_LOOP) && (m_eApostleState != STATE_GROGGY_END)
		&& (m_eApostleState != STATE_DOWN_START) && (m_eApostleState != STATE_DOWN_LOOP) && (m_eApostleState != STATE_DOWN_END))
	{
		m_tBaseInfo.iSuperAmmor = m_tBaseInfo.iMaxSuperAmmor;
		Change_State(STATE_GROGGY_START);
	}
	else if ((m_tBaseInfo.iSuperAmmor <= 0) &&(m_eApostleState == STATE_GROGGY_LOOP))
	{
		Change_State(STATE_DOWN_START);
	}

	// 패턴 쿨타임
	if (m_dbIdleTime > g_dAccumulatedTime)
		return;

	// 공격패턴 분기
	if (m_eApostleState == STATE_WAIT)
	{
		if (m_iPattern == 0) // 부채꼴 45도
		{
			Change_State(STATE_SKILL_0_PULLSWING);
			while (true)
			{
				m_iPattern = rand() % 9;
				if (m_iPattern != 0)
					break;
			}
		}
		else if (m_iPattern == 1) //부채꼴 180도
		{
			Change_State(STATE_SKILL_1_JUMP_TURN_SPIKE);
			while (true)
			{
				m_iPattern = rand() % 9;
				if (m_iPattern == 4 || m_iPattern == 6 || m_iPattern == 8)
					break;
			}
		}
		else if (m_iPattern == 2) // 부채꼴 270도
		{
			Change_State(STATE_SKILL_3_OVERSWING);
			while (true)
			{
				m_iPattern = rand() % 9;
				if (m_iPattern == 3 || m_iPattern == 4 || m_iPattern == 5)
					break;
			}
		}
		else if (m_iPattern == 3) // 360도
		{
			Change_State(STATE_SKILL_5_SPIKE);
			m_iPattern = 5;
		}
		else if (m_iPattern == 4) // 랜덤한위치에 빛기둥
		{
			Change_State(STATE_SKILL_7_JUMP_ATTACK);
			while (true)
			{
				m_iPattern = rand() % 9;
				if (m_iPattern != 4)
					break;
			}
		}
		else if (m_iPattern == 5) // 레이저
		{
			Change_State(STATE_SKILL_8_POWER_SPIKE);
			while (true)
			{
				m_iPattern = rand() % 9;
				if (m_iPattern != 5)
					break;
			}
		}
		else if (m_iPattern == 6) // 빙글빙글
		{
			Change_State(STATE_SKILL_10_SPIN_START);
			while (true)
			{
				m_iPattern = rand() % 9;
				if (m_iPattern != 6)
					break;
			}
		}
		else if (m_iPattern == 7) // 플레이어 위치에 빛기둥
		{
			Change_State(STATE_SKILL_11_JUMP_HOORAY);
			while (true)
			{
				m_iPattern = rand() % 9;
				if (m_iPattern != 7)
					break;
			}
		}
		else if (m_iPattern == 8) // 6개씩 전방향 지속기
		{
			Change_State(STATE_SKILL_12_POWER_DOWN);
			while (true)
			{
				m_iPattern = rand() % 9;
				if (m_iPattern == 0 || m_iPattern == 3 || m_iPattern == 6 || m_iPattern == 7)
					break;
			}
		}

		//if (m_iPhase == 1)
		//{
		//	int	iPattern = 0;
		//	iPattern = rand() % 5;

		//	if (iPattern == 0)
		//	{
		//		Change_State(STATE_SKILL_0_PULLSWING);
		//	}
		//	else if (iPattern == 1)
		//	{
		//		Change_State(STATE_SKILL_1_JUMP_TURN_SPIKE);
		//	}
		//	else if (iPattern == 2)
		//	{
		//		Change_State(STATE_SKILL_5_SPIKE);
		//	}
		//	else if (iPattern == 3)
		//	{
		//		Change_State(STATE_SKILL_7_JUMP_ATTACK);
		//	}
		//	else if (iPattern == 4)
		//	{
		//		Change_State(STATE_SKILL_10_SPIN_START);
		//	}
		//}
		//else if (m_iPhase == 2)
		//{
		//	// 랜덤 패턴
		//	int	iPattern = 0;
		//	iPattern = rand() % 9;

		//	if (iPattern == 0)
		//	{
		//		Change_State(STATE_SKILL_0_PULLSWING);
		//	}
		//	else if (iPattern == 1)
		//	{
		//		Change_State(STATE_SKILL_1_JUMP_TURN_SPIKE);
		//	}
		//	else if (iPattern == 2)
		//	{
		//		Change_State(STATE_SKILL_3_OVERSWING);
		//	}
		//	else if (iPattern == 3)
		//	{
		//		Change_State(STATE_SKILL_5_SPIKE);
		//	}
		//	else if (iPattern == 4)
		//	{
		//		Change_State(STATE_SKILL_7_JUMP_ATTACK);
		//	}
		//	else if (iPattern == 5)
		//	{
		//		Change_State(STATE_SKILL_8_POWER_SPIKE);
		//	}
		//	else if (iPattern == 6)
		//	{
		//		Change_State(STATE_SKILL_10_SPIN_START);
		//	}
		//	else if (iPattern == 7)
		//	{
		//		Change_State(STATE_SKILL_11_JUMP_HOORAY);
		//	}
		//	else if (iPattern == 8)
		//	{
		//		Change_State(STATE_SKILL_12_POWER_DOWN);
		//	}
		//	//// 테스트용 순차패턴
		//	//++iPattern;
		//	//if (iPattern >= 10)
		//	//	iPattern = 0;
		//}

		// 랜덤아닌거
		//else if (m_iPhase == 2)
		//{

		//	if (m_iPattern == 0) // 부채꼴 45도
		//	{
		//		Change_State(STATE_SKILL_0_PULLSWING);
		//		while (true)
		//		{
		//			m_iPattern = rand() % 9;
		//			if (m_iPattern != 0)
		//				break;
		//		}
		//	}
		//	else if (m_iPattern == 1) //부채꼴 180도
		//	{
		//		Change_State(STATE_SKILL_1_JUMP_TURN_SPIKE);
		//		while (true)
		//		{
		//			m_iPattern = rand() % 9;
		//			if (m_iPattern == 4 || m_iPattern == 6 || m_iPattern == 8)
		//				break;
		//		}

		//	}
		//	else if (m_iPattern == 2) // 부채꼴 270도
		//	{
		//		Change_State(STATE_SKILL_3_OVERSWING);
		//		while (true)
		//		{
		//			m_iPattern = rand() % 9;
		//			if (m_iPattern == 4 || m_iPattern == 5)
		//				break;
		//		}
		//	}
		//	else if (m_iPattern == 3) // 360도
		//	{
		//		Change_State(STATE_SKILL_5_SPIKE);
		//		m_iPattern = 5;
		//	}
		//	else if (m_iPattern == 4) // 랜덤한위치에 빛기둥
		//	{
		//		Change_State(STATE_SKILL_7_JUMP_ATTACK);
		//		while (true)
		//		{
		//			m_iPattern = rand() % 9;
		//			if (m_iPattern != 4)
		//				break;
		//		}
		//	}
		//	else if (m_iPattern == 5) // 레이저
		//	{
		//		Change_State(STATE_SKILL_8_POWER_SPIKE);
		//		while (true)
		//		{
		//			m_iPattern = rand() % 9;
		//			if (m_iPattern != 5)
		//				break;
		//		}
		//	}
		//	else if (m_iPattern == 6) // 빙글빙글
		//	{
		//		Change_State(STATE_SKILL_10_SPIN_START);
		//		while (true)
		//		{
		//			m_iPattern = rand() % 9;
		//			if (m_iPattern != 6)
		//				break;
		//		}
		//	}
		//	else if (m_iPattern == 7) // 플레이어 위치에 빛기둥
		//	{
		//		Change_State(STATE_SKILL_11_JUMP_HOORAY);
		//		while (true)
		//		{
		//			m_iPattern = rand() % 9;
		//			if (m_iPattern != 7)
		//				break;
		//		}
		//	}
		//	else if (m_iPattern == 8) // 6개씩 전방향 지속기
		//	{
		//		Change_State(STATE_SKILL_12_POWER_DOWN);
		//		while (true)
		//		{
		//			m_iPattern = rand() % 9;
		//			if (m_iPattern == 0 || m_iPattern == 6 || m_iPattern == 7)
		//				break;
		//		}
		//	}
		//	//// 테스트용 순차패턴
		//	//++iPattern;
		//	//if (iPattern >= 10)
		//	//	iPattern = 0;
		//}
	}
}

void CApostle::ObserverNotify()
{
	Engine::Notify(Engine::APOSTLE, CApostleObserver::HP, (void*)(&m_tBaseInfo.iHp));
	Engine::Notify(Engine::APOSTLE, CApostleObserver::PREVHP, (void*)(&m_tBaseInfo.iPrevHp));
	Engine::Notify(Engine::APOSTLE, CApostleObserver::MAXHP, (void*)(&m_tBaseInfo.iMaxHp));
	Engine::Notify(Engine::APOSTLE, CApostleObserver::SUPERARMOR, (void*)(&m_tBaseInfo.iSuperAmmor));
	Engine::Notify(Engine::APOSTLE, CApostleObserver::MAXSUPERARMOR, (void*)(&m_tBaseInfo.iMaxSuperAmmor));
}

void CApostle::Check_HitTime(const _double & dTimeDelta)
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

void CApostle::Set_Collision(CBaseObject * pObj, COLLIDER eColliderType)
{
	if (pObj == nullptr)
		return;

	const BASE_INFO*	pBaseInfo = pObj->Get_BaseInfo();

	// 공격에 맞았다
	if (eColliderType == COLLIDER::COLLIDER_ATT_HIT)
	{
		Set_Collision_Effect(pObj);
	}
	// 몸통끼리 부딪혔다
	else if (eColliderType == COLLIDER::COLLIDER_DEF_HIT)
	{
		// 플레이어가 이펙트 지금 안떠서 테스트용
		//Set_Collision_Effect(pObj);
	}
}

void CApostle::Set_Collision_Effect(CBaseObject * pObj)
{
	if (0 >= m_tBaseInfo.iHp)
		return;

	if (m_mapinvincibility.end() == m_mapinvincibility.find(pObj->Get_BaseInfo()->eEffectID))
	{
		m_mapinvincibility.emplace(pObj->Get_BaseInfo()->eEffectID, pObj->Get_BaseInfo()->fEffectHitTime);
		
		_uint i = pObj->Get_BaseInfo()->iMaxHp;

		// HP
		_int test = pObj->Get_BaseInfo()->iAtt >> 2;
		_int iDif = rand() % test + 1;

		_int iDamage = pObj->Get_BaseInfo()->iAtt + iDif;// +pObj->Get_BaseInfo()->iCheatATK; //희정 수정& 추가
		m_tBaseInfo.iHp -= iDamage;
		m_pUIMgr->SetAccumulatedDamage(iDamage); //희정 추가

		if (0 >= m_tBaseInfo.iHp)
			m_tBaseInfo.iHp = 0;
		/*m_tBaseInfo.iHp -= pObj->Get_BaseInfo()->iAtt;
		if (0 >= m_tBaseInfo.iHp)
			m_tBaseInfo.iHp = 0;*/

		m_pUIMgr->CreateDamageBox(m_pGraphicDev, m_pTransformCom, CDamageBox::DAMAGEBOXTYPE::BASIC, pObj->Get_BaseInfo()->iAtt + iDif);

		// 슈퍼아머

		_uint iSuperArmorDamage = pObj->Get_BaseInfo()->iAtt * 0.1f; //희정 수정& 추가
		m_tBaseInfo.iSuperAmmor -= iSuperArmorDamage;
		m_pUIMgr->SetAccumulatedBreak(iSuperArmorDamage); //희정 추가


		if (0 >= m_tBaseInfo.iSuperAmmor)
			m_tBaseInfo.iSuperAmmor = 0;
		m_dHitTime = 0.1;
	}
}

CApostle * CApostle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CApostle* pInstance = new CApostle(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostle::Free()
{
	Engine::Safe_Release(m_pState);
	Engine::UnSubscribe(Engine::PLAYER, m_pPlayerObserver);
	Engine::Safe_Release(m_pPlayerObserver);
	m_mapinvincibility.clear();
	CDynamicMeshObject::Free();
}
