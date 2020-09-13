#include "stdafx.h"
#include "Belatos.h"
#include "PlayerObserver.h"
#include "DamageBox.h"
#include "Belatos_Missile.h"

#include "Arrow.h"
#include "Belatos_Arrow.h"
#include "Orb.h"

CBelatos::CBelatos(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev)
	, m_bLateInit(false),
	m_pPlayerObserver(nullptr),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_bCritical(false),
	m_dDissolveCountDown(0),
	m_iPass(0),
	m_bDissolveStart(false),
	m_bIsHit(false),
	m_dHitTime(0),
	m_bUseSpearRim(false),
	m_bUseShieldRim(false)
{
	m_fAnimationSpeed = 1.f;
}


CBelatos::~CBelatos()
{
}

HRESULT CBelatos::Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_Speed(1000.f);
	m_eState = STATE::STATE_IDLE;
	m_iPlayingAniNum = 7;

	//옵저버
	m_pPlayerObserver = CPlayerObserver::Create();
	if (m_pPlayerObserver == nullptr)
		return E_FAIL;
	Engine::Subscribe(Engine::PLAYER, m_pPlayerObserver);

	//
	m_tBaseInfo.eObjectID = OBJECT_MONSTER;
	//m_tBaseInfo.iMaxHp = 120000;
	//m_tBaseInfo.iHp = 120000;
	m_tBaseInfo.iMaxHp = 20000;
	m_tBaseInfo.iHp = 20000;
	m_tBaseInfo.iPrevHp = 20000;
	m_tBaseInfo.iSuperAmmor = 5000;
	m_tBaseInfo.iMaxSuperAmmor = 5000;
	m_tBaseInfo.iCheatATK = 0;
	//m_tBaseInfo.iMaxHp = 3000;
	//m_tBaseInfo.iHp = 3000;
	//m_tBaseInfo.iPrevHp = 3000;
	//m_tBaseInfo.iSuperAmmor = 2000;
	//m_tBaseInfo.iMaxSuperAmmor = 2000;

	LoadOuterCollider(L"../../Data/Collider/ColliderOuterBelatos.dat");
	LoadHitCollider(L"../../Data/Collider/ColliderHitBelatos.dat");

	// yh test code
	m_tBaseInfo.vObjectPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	m_tBaseInfo.iCritical = 3;
	m_tBaseInfo.eEffectID = EFFECT_END;
	// yh test end

	return S_OK;
}

_int CBelatos::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{
		m_pUIMgr->Set_CreateBossResultUI(true);
		m_pUIMgr->Set_StageTimeStop();
		_vec3 CreatePos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
		CreatePos.y += 50.f;
		COrb::Create(m_pGraphicDev, CreatePos);
		return Engine::OBJ_DEAD;
	}

	if (m_tBaseInfo.iHp <= 0)
		m_eState = STATE_DEAD;

	// yh test code
	m_tBaseInfo.vObjectPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	// yh test end

	LateInit();
	
	CDynamicMeshObject::Link_NaviMesh();
	CDynamicMeshObject::Update_GameObject(dTimeDelta);
	m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);

	CCollisionMgr::GetInstance()->Add_Collision(COLL_MONSTER, this);
	// ㄴ 충돌 체크를 위해(곂침 방지, 공격&방어 체크) 호출
	Check_State();
	Run_State(dTimeDelta);

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
	return Engine::NO_EVENT;
}

_int CBelatos::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{
		m_pUIMgr->Set_CreateBossResultUI(true);
		m_pUIMgr->Set_StageTimeStop();
		_vec3 CreatePos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
		CreatePos.y += 50.f;
		COrb::Create(m_pGraphicDev, CreatePos);
		return Engine::OBJ_DEAD;
	}

	if (m_tBaseInfo.iHp <= 0)
		m_eState = STATE_DEAD;

	// yh test code
	m_tBaseInfo.vObjectPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	// yh test end

	if (!m_bisStart)
	{
		//_vec3 vPlayerStartPos = { 2.5f, 53.f, -1053.25f };

		float fDist = Engine::GetDistNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		////cout << fDist << endl;

		// m_bisReady는 Start가 시작되면 true가 된다. 한번 Start되면 다시 거리체크해서 Ready로 못돌아가게 막음
		if (!m_bisReady)
		{
			if (fDist < 10)
				m_eState = STATE_START;
			else
				m_eState = STATE_READY;
		}

		if(m_bisReady)
		{
			CCameraMgr::GetInstance()->Set_BelatosCutscene();
		}
	}

	Check_HitTime(dTimeDelta);
	Check_PlayerDead(dTimeDelta);
	m_fHitSoundTime += dTimeDelta;
	//View_State();

	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CBelatos::Render_Geometry(const _double & dTimeDelta)
{
	if (m_eState == STATE_READY)
	{
		m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta, 0);
	}
	else
	{
		if(!m_bDissolveStart)
			m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta, m_fAnimationSpeed);
	}

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect, dTimeDelta)))
		return;
	_uint iPassMax = 0;

	if (m_bDissolveStart)
	{
		Engine::SetTexture(pEffect, "g_DissolveTexture", 6);
		Engine::SetTexture(pEffect, "g_DissolveEdgeTexture", 2);
		m_dDissolveCountDown += dTimeDelta * 0.5;
		pEffect->SetFloat("fTime", m_dDissolveCountDown);
		m_iPass = 14;
		if (m_dDissolveCountDown > 1)
			m_bIsDead = true;
	}

	pEffect->Begin(&iPassMax, 0);
	
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			if (i == 0) // 관절 흰색
			{
				pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
				pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
				pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
				pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
				pEffect->BeginPass(m_iPass);
			}
			else if (i == 1) //눈 치골
			{
				pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
				pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
				pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
				pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
				pEffect->BeginPass(m_iPass);
			}
			else if (i == 2) //몸 기본 색
			{
				pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
				pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
				pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
				pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
				pEffect->BeginPass(m_iPass);
			}
			else if (i == 3) //방패
			{
				if (m_bUseShieldRim)
				{
					_vec3 vCamPos = CCameraMgr::GetInstance()->Get_Pos();
					pEffect->SetValue("g_vCamPos", &vCamPos, sizeof(_vec3));
					_vec3 vPos = m_pTransformCom->Get_Info_byWorld(Engine::INFO_POS);
					pEffect->SetValue("g_vPosition", &vPos, sizeof(_vec3));
					pEffect->SetValue("g_vRimColor", &_vec3(1.f, 0.7f, 0.f), sizeof(_vec3));
					pEffect->SetValue("g_vRimMinMax", &_vec2(0.2f, 1.f), sizeof(_vec2));
					m_iPass = 19;
				}
				
				pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
				pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
				pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
				pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
				pEffect->BeginPass(m_iPass);
				if (m_iPass == 19 && !m_bIsHit)
					m_iPass = 0;
			}
			else if (i == 4) // 스피어
			{
				if (m_bUseSpearRim)
				{
					_vec3 vCamPos = CCameraMgr::GetInstance()->Get_Pos();
					pEffect->SetValue("g_vCamPos", &vCamPos, sizeof(_vec3));
					_vec3 vPos = m_pTransformCom->Get_Info_byWorld(Engine::INFO_POS);
					pEffect->SetValue("g_vPosition", &vPos, sizeof(_vec3));
					pEffect->SetValue("g_vRimColor", &_vec3(1.f, 0.7f, 0.f), sizeof(_vec3));
					pEffect->SetValue("g_vRimMinMax", &_vec2(0.2f, 1.f), sizeof(_vec2));
					m_iPass = 19;
				}
				pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
				pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
				pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
				pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
				pEffect->BeginPass(m_iPass);
				if (m_iPass == 19 && !m_bIsHit)
					m_iPass = 0;
			}
			else if (i == 5) //망토
			{
				pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
				pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
				pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
				pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
				pEffect->BeginPass(m_iPass);
			}

			
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

void CBelatos::Render_PostEffect(const _double & dTimeDelta)
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

void CBelatos::Render_Shadow(const _double & dTimeDelta)
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


void CBelatos::LateInit()
{
	if (!m_bLateInit)
	{

		m_bLateInit = true;
	}
}

void CBelatos::Check_State()
{
	 // Auto변수로 자동모드와 수동모드로 나눈다.
	if (Engine::KeyDown(DIK_NUMPAD7))
		m_bAuto = !m_bAuto;

	if (m_bisStart)
	{
		if (m_bAuto)
			// 자동모드
			Auto_Check_State();
		else
			// 수동모드
			NonAuto_Check_State();
	}
}

void CBelatos::Auto_Check_State()
{
	if (IsReady_Timer(L"Idle"))
	{
		if (Get_Timer(L"Idle") > /*0.15f*//*0.3f*/0.6f)
		{
			// 움직이기 전에 먼저 공격타입을 정함
			
			if (m_pPlayerObserver->Get_Hp() <= 0)
				return;

			Select_AttackState();
			Check_Dist();

			// 여기있는 공격타입이면 이동해서 접근한다.
			//if (m_eNextState == STATE_ATTACK || m_eNextState == STATE_SWEEP || m_eNextState == STATE_SHIELDRUSH || m_eNextState == STATE_THRUST)
			//{
				//if (Engine::GetDistNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS)) > 400.f)
				//	Change_State(STATE::STATE_RUN);
				//else if (Engine::GetDistNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS)) < 400.f)
				//	Change_State(STATE::STATE_MOVE);
			//}
		}
	}
}

void CBelatos::NonAuto_Check_State()
{
	//if (Engine::KeyDown(DIK_J))
	//{
	//	m_eNextState = STATE_BACKJUMP;
	//	Change_State(m_eNextState);
	//}
//	if (Engine::KeyDown(DIK_K))
//	{
	//	m_eNextState = STATE_HIT_FRONT;
	//	Change_State(m_eNextState);
	//}
	//if (Engine::KeyDown(DIK_L))
	//{
	//	m_eNextState = STATE_HIT_BACK;
	//	Change_State(m_eNextState);
	//}
}

void CBelatos::Run_State(const _double & dTimeDelta)
{
	// 시작
	Start(dTimeDelta);
	Ready(dTimeDelta);

	// 상태 및 이동
	Idle(dTimeDelta);
	Move(dTimeDelta);
	Run(dTimeDelta);
	BackJump(dTimeDelta);

	// 공격
	ShieldUp(dTimeDelta);
	Attack(dTimeDelta);
	Sweep(dTimeDelta);
	ShieldRush(dTimeDelta);
	Missile(dTimeDelta);
	Thrust(dTimeDelta);

	// 점프관련
	JumpAttack(dTimeDelta);
	JumpShieldAttack(dTimeDelta);

	// 피격
	Hit_Front(dTimeDelta);
	Hit_Back(dTimeDelta);

	Front_Hit_Up(dTimeDelta);
	Front_Hit_Down(dTimeDelta);
	Front_Hit_Ground(dTimeDelta);
	Front_Hit_AirUp(dTimeDelta);
	Front_Hit_AirDown(dTimeDelta);

	Back_Hit_Up(dTimeDelta);
	Back_Hit_Down(dTimeDelta);
	Back_Hit_Ground(dTimeDelta);
	Back_Hit_AirUp(dTimeDelta);
	Back_Hit_AirDown(dTimeDelta);

	// 피격후 기상
	Front_StandUp(dTimeDelta);
	Back_StandUp(dTimeDelta);

	// 기절상태
	Break(dTimeDelta);
	Break_1(dTimeDelta);
	Break_2(dTimeDelta);
	Break_Jump(dTimeDelta);
	AirUp(dTimeDelta);

	// 죽음
	Dead(dTimeDelta);

	// BreakTime 체크
	Check_SuperAmor(dTimeDelta);
}

void CBelatos::Select_AttackState()
{
	_float fDist = Engine::GetDistNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));

	//m_iAttackState = 3;

	//switch (m_iAttackState)
	//{
	//case 0:
	//	m_eNextState = STATE_ATTACK;
	//	break;
	//case 1:
	//	m_eNextState = STATE_SWEEP;
	//	break;
	//case 2:
	//	m_eNextState = STATE_THRUST;
	//	break;
	//case 3:
	//	m_eNextState = STATE_SHIELDRUSH;
	//	break;
	//case 4:
	//	m_eNextState = STATE_MISSILE;
	//	break;
	//case 5:
	//	m_eNextState = STATE_JUMPATTACK;
	//	break;
	//case 6:
	//	m_eNextState = STATE_SHIELDUP;
	//	break;
	//}

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	m_eNextState = STATE_END;

	_bool	bShortDist_Attack = false;
	_bool	bLongDist_Attack = false;

	if (fDist < 500)
	{
		bShortDist_Attack = true;
		bLongDist_Attack = false;
	}
	else
	{
		bLongDist_Attack = true;
		bShortDist_Attack = false;
	}


	if (m_ePreviousAttackState == STATE_MISSILE || m_ePreviousAttackState == STATE_SHIELDUP)
	{
		m_eNextState = STATE_SHIELDRUSH;
		m_ePreviousAttackState = STATE_SHIELDRUSH;
	}

	if (m_ePreviousAttackState == STATE_JUMPATTACK)
	{
		m_eNextState = STATE_SHIELDUP;
		m_ePreviousAttackState = STATE_SHIELDUP;
	}


	if (m_eNextState == STATE_END)
	{
		while (true)
		{
			if (bShortDist_Attack == true)
				m_iAttackState = Engine::RandomNumber(6);
			else if (bLongDist_Attack == true)
				m_iAttackState = Engine::RandomNumber(4);

			if (bShortDist_Attack)	// 근거리
			{
				switch (m_iAttackState)
				{
				case 0:
					m_eNextState = STATE_ATTACK;
					break;
				case 1:
					m_eNextState = STATE_SWEEP;
					break;
				case 2:
					m_eNextState = STATE_THRUST;
					break;
				case 3:
					m_eNextState = STATE_SHIELDRUSH;
					break;
				case 4:
					m_eNextState = STATE_SHIELDRUSH;
					break;
				case 5:
					m_eNextState = STATE_SHIELDRUSH;
					break;
				}
			}
			else if (bLongDist_Attack)	// 원거리
			{
				switch (m_iAttackState)
				{
				case 0:
					m_eNextState = STATE_MISSILE;
					break;
				case 1:
					m_eNextState = STATE_JUMPATTACK;
					break;
				case 2:
					m_eNextState = STATE_SHIELDUP;
					break;
				case 3:
					m_eNextState = STATE_SHIELDRUSH;
					break;
				}
			}

			// 이전 공격과 같으면 while문 계속 돔
			if (m_ePreviousAttackState == m_eNextState)
				continue;
			// 다음 공격이 정해졌으면 while문 나감
			else
				break;
		}
	}

	m_ePreviousAttackState = m_eNextState;
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


	//m_iAttackState = Engine::RandomNumber(10);

	//m_iAttackState++;
	//if (m_iAttackState > 6)
	//	m_iAttackState = 0;


}

void CBelatos::Check_Dist()
{
	_float fDist = Engine::GetDistNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));

	switch (m_eNextState)
	{
	case CBelatos::STATE_IDLE:
		break;
	case CBelatos::STATE_MOVE:
		break;
	case CBelatos::STATE_RUN:
		break;
	case CBelatos::STATE_BACKJUMP:
		break;
	case CBelatos::STATE_HIT_FRONT:
		break;
	case CBelatos::STATE_HIT_BACK:
		break;
	case CBelatos::STATE_FRONT_HIT_UP:
		break;
	case CBelatos::STATE_FRONT_HIT_DOWN:
		break;
	case CBelatos::STATE_FRONT_HIT_GROUND:
		break;
	case CBelatos::STATE_BACK_HIT_UP:
		break;
	case CBelatos::STATE_BACK_HIT_DOWN:
		break;
	case CBelatos::STATE_BACK_HIT_GROUND:
		break;
	case CBelatos::STATE_FRONT_STANDUP:
		break;
	case CBelatos::STATE_BACK_STANDUP:
		break;
	case CBelatos::STATE_SHIELDUP:
		//if (fDist < 500)
		//{
		//	Change_State(STATE::STATE_BACKJUMP);
		//}
		//else
		//{
			Change_State(STATE::STATE_SHIELDUP);
		//}
		break;
	case CBelatos::STATE_ATTACK:
		Change_State(STATE::STATE_RUN);
		break;
	case CBelatos::STATE_SWEEP:
		Change_State(STATE::STATE_RUN);
		break;
	case CBelatos::STATE_JUMPATTACK:
		if (fDist < 700)
		{
			Change_State(STATE::STATE_BACKJUMP);
		}
		else if (700 <= fDist && fDist <= 720)
		{
			Change_State(STATE::STATE_JUMPATTACK);
			m_vJump_TargetPos = m_pPlayerObserver->Get_Pos();
		}
		else
		{
			Change_State(STATE::STATE_RUN);
		}
		break;
	case CBelatos::STATE_JUMPSHIELDATTACK:
		break;
	case CBelatos::STATE_SHIELDRUSH:
		if (fDist < 400)
		{
			Change_State(STATE::STATE_SHIELDRUSH);
			m_vJump_TargetPos = m_pPlayerObserver->Get_Pos();
		}
		else
		{
			Change_State(STATE::STATE_RUN);
		}
		break;
	case CBelatos::STATE_MISSILE:
		if (fDist < 700)
		{
			//Change_State(STATE::STATE_BACKJUMP);
			Change_State(STATE::STATE_MISSILE);
		}
		else
		{
			Change_State(STATE::STATE_MISSILE);
		}
		break;
	case CBelatos::STATE_THRUST:
		Change_State(STATE::STATE_RUN);
		break;
	case CBelatos::STATE_BREAK_1:
		break;
	case CBelatos::STATE_BREAK_2:
		break;
	case CBelatos::STATE_END:
		break;
	default:
		break;
	}

}

void CBelatos::Start(const _double & dTimeDelta)
{
	if (m_eState == STATE_START)
	{
		m_bisReady = true;

		if (!IsReady_Timer(L"Start"))
		{
			Set_Timer(L"Start");
			Set_Timer(L"Sound_Start");	// 목소리
			Set_Timer(L"Sound_Start_EffectSound_1");	// 맨처음 쿵쿵거리는 사운드
			Set_Timer(L"Sound_Start_EffectSound_2");	// 맨처음 쿵쿵거리는 사운드
		}

		Start_Timer(L"Start");

		m_iPlayingAniNum = 81;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();


		// ㅡㅡㅡㅡㅡㅡㅡㅡ 사운드 ㅡㅡㅡㅡㅡㅡㅡ
		if (IsReady_Timer(L"Sound_Start_EffectSound_1"))
		{
			if (Get_Timer(L"Start") > 0.2f)
			{
				CSoundMgr::Get_Instance()->SoundOn(135);
				Delete_Timer(L"Sound_Start_EffectSound_1");
			}
		}
		if (IsReady_Timer(L"Sound_Start_EffectSound_2"))
		{
			if (Get_Timer(L"Start") > 1.3f)
			{
				CSoundMgr::Get_Instance()->SoundOn(135);
				Delete_Timer(L"Sound_Start_EffectSound_2");
			}
		}
		if (IsReady_Timer(L"Sound_Start"))
		{
			if (Get_Timer(L"Start") > 1.5f)
			{
				CSoundMgr::Get_Instance()->SoundOn(133);
				Delete_Timer(L"Sound_Start");
			}
		}
		// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

		if (Get_Timer(L"Start") > m_dAnimationPeriod * 0.9f)
		{
			Delete_Timer(L"Start");
			Change_State(STATE_IDLE);
			m_bisStart = true;

			//희정 추가
			m_pUIMgr->CreateBattleUI(m_pGraphicDev, CUIMgr::UITYPE_BOSS);
			m_pUIMgr->CreatePlayerInfoUI(m_pGraphicDev, nullptr);
			m_pUIMgr->Set_StageTimeStart();

		}
	}
}

void CBelatos::Ready(const _double & dTimeDelta)
{
	if (m_eState == STATE_READY)
	{
		m_iPlayingAniNum = 81;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
	}
}

void CBelatos::Idle(const _double & dTimeDelta)
{
	if (m_eState == STATE_IDLE)
	{
		m_bIsHit = false;
		m_iPlayingAniNum = 7;

		//Set_Timer(L"Idle");
		//Start_Timer(L"Idle");

		m_pTransformCom->Look_Target(m_pPlayerObserver->Get_Pos());
		////cout << Get_Timer(L"Idle") << endl;

		m_bUseShieldRim = false;
		m_bUseSpearRim = false;
	}
}

void CBelatos::Move(const _double & dTimeDelta)
{
	if (m_eState == STATE_MOVE)
	{
		Set_Timer(L"Move");
		Start_Timer(L"Move");

		m_iPlayingAniNum = 86;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		_vec3 vDir = Engine::GetDir(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		_vec3 * pPos = m_pTransformCom->Get_Info(Engine::INFO_POS);
		m_pTransformCom->Set_Pos(&(m_pNaviMeshCom->Move_OnNaviMesh(pPos, &(vDir * 250.f * (_float)dTimeDelta), m_dwNaviIndex)));
		//m_pTransformCom->Move_Pos(&vDir, 250.f, dTimeDelta);
		m_pTransformCom->Look_Target(m_pPlayerObserver->Get_Pos());

		if (Engine::GetDistNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS)) < 150.f)
		{
			Delete_Timer(L"Move");
			Change_State(m_eNextState);
		}
	}
}

void CBelatos::Run(const _double & dTimeDelta)
{
	if (m_eState == STATE_RUN)
	{
		
		Set_Timer(L"Run");
		Start_Timer(L"Run");
		
		Set_Timer(L"Sound_Run");
		Start_Timer(L"Sound_Run");

		if (IsReady_Timer(L"Sound_Run"))
		{
			if (Get_Timer(L"Sound_Run") > 0.4)
			{
				//switch (Engine::RandomNumber(4))
				//{
				//case 0:
				//	CSoundMgr::Get_Instance()->SoundOn(151);
				//	break;
				//case 1:
				//	CSoundMgr::Get_Instance()->SoundOn(152);
				//	break;
				//case 2:
				//	CSoundMgr::Get_Instance()->SoundOn(153);
				//	break;
				//case 3:
				//	CSoundMgr::Get_Instance()->SoundOn(154);
				//	break;
				//}
				CSoundMgr::Get_Instance()->SoundOn(154);
				Delete_Timer(L"Sound_Run");
			}
		}

		_float fDist = Engine::GetDistNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));

		m_iPlayingAniNum = 83;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		_vec3 vDir = Engine::GetDir(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		_vec3 * pPos = m_pTransformCom->Get_Info(Engine::INFO_POS);
		m_pTransformCom->Set_Pos(&(m_pNaviMeshCom->Move_OnNaviMesh(pPos, &(vDir * 500.f * (_float)dTimeDelta), m_dwNaviIndex)));
		//m_pTransformCom->Move_Pos(&vDir, 500.f, dTimeDelta);
		m_pTransformCom->Look_Target(m_pPlayerObserver->Get_Pos());

		if (m_eNextState == STATE::STATE_JUMPATTACK)
		{
			if (700 <= fDist && fDist <= 720)
			{
				Change_State(m_eNextState);
				m_vJump_TargetPos = m_pPlayerObserver->Get_Pos();
				Delete_Timer(L"Run");
				Delete_Timer(L"Sound_Run");
			}
			else if (fDist < 700)
			{
				m_eNextState = STATE::STATE_SHIELDRUSH;
			}
		}
		else if (m_eNextState == STATE::STATE_SHIELDRUSH)
		{
			if (fDist < 400)
			{
				Change_State(m_eNextState);
				m_vJump_TargetPos = m_pPlayerObserver->Get_Pos();
				Delete_Timer(L"Run");
				Delete_Timer(L"Sound_Run");
			}
		}

		else if (fDist < 150.f)
		{
			Delete_Timer(L"Run");
			Delete_Timer(L"Sound_Run");
			Change_State(m_eNextState);
		}
	}
}

void CBelatos::BackJump(const _double & dTimeDelta)
{
	if (m_eState == STATE_BACKJUMP)
	{

		if (!IsReady_Timer(L"Initialize_Jump"))
		{
			Set_Timer(L"Initialize_Jump");
			Set_Timer(L"BackJump");
		}

		Start_Timer(L"BackJump");

		m_iPlayingAniNum = 11;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		_vec3 vDir = Engine::GetDirNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));

		if (Get_Timer(L"BackJump") > m_dAnimationPeriod * 0.15)
		{
			if (Jump(STATE_JUMP::JUMP_BACK, 1500.f, 0.f, 0.f, 0.f, 100.f, vDir, dTimeDelta))
			{
				Delete_Timer(L"Initialize_Jump");
				Delete_Timer(L"BackJump");
				// %%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(154);
				// %%%%%%%% 사운드

				if (m_eNextState == STATE::STATE_SHIELDUP)
				{
					Change_State(STATE::STATE_SHIELDUP);
					return;
				}
				else if (m_eNextState == STATE::STATE_MISSILE)
				{
					Change_State(STATE::STATE_MISSILE);
					return;
				}
				else if (m_eNextState == STATE::STATE_JUMPATTACK)
				{
					float fDist = Engine::GetDistNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));

					if (fDist < 700)
					{
						Change_State(STATE::STATE_SHIELDRUSH);
					}
					else
					{
						Change_State(STATE::STATE_RUN);
					}

					return;
				}

				Change_State(STATE::STATE_IDLE);
			}
		}


		//if (Get_Timer(L"BackJump") > m_dAnimationPeriod - 0.3f)
		//{
		//	Delete_Timer(L"Initialize_Jump");
		//	Delete_Timer(L"BackJump");
		//}
	}
}

void CBelatos::ShieldUp(const _double & dTimeDelta)
{
	if (m_eState == STATE_SHIELDUP)
	{
		m_bUseShieldRim = true;
		m_bUseSpearRim = true;
		if (!IsReady_Timer(L"ShieldUp"))
		{
			Set_Timer(L"ShieldUp");
			
			Set_Timer(L"Effect_ShieldUp");
			Set_Timer(L"Effect_ShieldUp2");
			
			Set_Timer(L"Sound_ShieldUp_1");
			Set_Timer(L"Sound_ShieldUp_2");

			if (0 == Engine::RandomNumber(2))
				CSoundMgr::Get_Instance()->SoundOn(104);
			else
				CSoundMgr::Get_Instance()->SoundOn(108);
		}

		Start_Timer(L"ShieldUp");

		m_iPlayingAniNum = 6;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (IsReady_Timer(L"Effect_ShieldUp"))
		{
			if (Get_Timer(L"ShieldUp") > 1.f)
			{
				_matrix newPos = m_pTransformCom->m_matWorld;
				newPos._42 -= 50;
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_HEMISPHERE, &newPos, &m_pTransformCom->m_vAngle, m_tBaseInfo);
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_HEMISPHERE3, &newPos, &m_pTransformCom->m_vAngle, m_tBaseInfo);
				Delete_Timer(L"Effect_ShieldUp");
				// %%%%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(140);
				CSoundMgr::Get_Instance()->SoundOn(141);
				// %%%%%%%%%% 사운드
			}
		}

		if (IsReady_Timer(L"Effect_ShieldUp2"))
		{
			if (Get_Timer(L"ShieldUp") > 2.2f)
			{
				_matrix newPos = m_pTransformCom->m_matWorld;
				newPos._42 -= 50;
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_HEMISPHERE2, &newPos, &m_pTransformCom->m_vAngle, m_tBaseInfo);
				Delete_Timer(L"Effect_ShieldUp2");
				// %%%%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(141);
				// %%%%%%%%%% 사운드
			}
		}

		if (IsReady_Timer(L"Sound_ShieldUp_1"))
		{
			if (Get_Timer(L"ShieldUp") > 2.4f)
			{
				// %%%%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(142);
				// %%%%%%%%%% 사운드
				Delete_Timer(L"Sound_ShieldUp_1");
			}
		}
		
		if (IsReady_Timer(L"Sound_ShieldUp_2"))
		{
			if (Get_Timer(L"ShieldUp") > 2.7f)
			{
				// %%%%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(143);
				// %%%%%%%%%% 사운드
				Delete_Timer(L"Sound_ShieldUp_2");
			}
		}

		if (Get_Timer(L"ShieldUp") > m_dAnimationPeriod - 0.3f)
		{
			Delete_Timer(L"Sound_ShieldUp_1");
			Change_State(STATE::STATE_IDLE);
		}
	}
}

void CBelatos::Attack(const _double & dTimeDelta)
{
	if (m_eState == STATE_ATTACK)
	{
		m_bUseSpearRim = true;
		if (!IsReady_Timer(L"Attack"))
		{
			Set_Timer(L"Attack");
			Set_Timer(L"Effect_Attack");
			// %%%%%%%%%% 사운드
			CSoundMgr::Get_Instance()->SoundOn(114);
			// %%%%%%%%%% 사운드
		}

		Start_Timer(L"Attack");

		m_iPlayingAniNum = 32;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (IsReady_Timer(L"Effect_Attack"))
		{
			if (Get_Timer(L"Attack") > 0.1f)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_ATTACK, &m_pTransformCom->m_matWorld, &m_pTransformCom->m_vAngle, m_tBaseInfo);
				Delete_Timer(L"Effect_Attack");
				// %%%%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(150);
				// %%%%%%%%%% 사운드
			}
		}

		if (Get_Timer(L"Attack") > m_dAnimationPeriod - 0.8f)
		{
			Delete_Timer(L"Attack");
			Change_State(STATE::STATE_IDLE);
		}
	}
}

void CBelatos::Sweep(const _double & dTimeDelta)
{
	if (m_eState == STATE_SWEEP)
	{
		m_bUseSpearRim = true;
		if (!IsReady_Timer(L"Sweep"))
		{
			Set_Timer(L"Sweep");
			Set_Timer(L"Effect_Sweep");
			// %%%%%%%% 사운드
			CSoundMgr::Get_Instance()->SoundOn(105);
			// %%%%%%%% 사운드
		}

		Start_Timer(L"Sweep");

		m_iPlayingAniNum = 0;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (IsReady_Timer(L"Effect_Sweep"))
		{
			if (Get_Timer(L"Sweep") > 0.7f)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_SWEEP, &m_pTransformCom->m_matWorld, &m_pTransformCom->m_vAngle, m_tBaseInfo);
				Delete_Timer(L"Effect_Sweep");
				// %%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(148);
				// %%%%%%%% 사운드
			}
		}

		if (Get_Timer(L"Sweep") > m_dAnimationPeriod - 0.3f)
		{
			Delete_Timer(L"Sweep");
			Change_State(STATE::STATE_IDLE);
		}
	}
}

void CBelatos::ShieldRush(const _double & dTimeDelta)
{
	if (m_eState == STATE_SHIELDRUSH)
	{
		m_bUseShieldRim = true;
		if (!IsReady_Timer(L"Rush"))
		{
			Set_Timer(L"Rush");
			Set_Timer(L"Effect_Rush");
			m_vJumpDir = Engine::GetDirNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
			// %%%%%%% 사운드
			CSoundMgr::Get_Instance()->SoundOn(115);
			CSoundMgr::Get_Instance()->SoundOn(146);
			// %%%%%%% 사운드
		}

		Start_Timer(L"Rush");

		m_pTransformCom->Get_Angle();

		m_iPlayingAniNum = 37;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		// 이펙트 생성 타이밍
		if (IsReady_Timer(L"Effect_Rush"))
		{
			if (Get_Timer(L"Rush") > 0.1)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_SHIELDFRONT, &m_pTransformCom->m_matWorld, &m_pTransformCom->m_vAngle, m_tBaseInfo);
				Delete_Timer(L"Effect_Rush");

				// %%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(147);
				// %%%%%%% 사운드
			}
		}

		_vec3 vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
		D3DXVec3Normalize(&vLook, &vLook);

		if (Jump(STATE_JUMP::JUMP_ATTACK, 1000.f, 0.f, 0.f, 0.f, 50.f, vLook, dTimeDelta))
		{
			Delete_Timer(L"Rush");
			Change_State(STATE::STATE_IDLE);
		}
	}
}

void CBelatos::Missile(const _double & dTimeDelta)
{
	if (m_eState == STATE_MISSILE)
	{
		m_bUseShieldRim = true;
		m_bUseSpearRim = true;
		if (!IsReady_Timer(L"Initialize_Missile"))
		{
			Set_Timer(L"Initialize_Missile");
			Set_Timer(L"Missile_1");
			Set_Timer(L"Effect_Missile");
		}

		if (IsReady_Timer(L"Missile_1"))
		{
			Start_Timer(L"Missile_1");

			m_iPlayingAniNum = 10;
			m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
			m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

			m_pTransformCom->Look_Target(m_pPlayerObserver->Get_Pos());

			if (Get_Timer(L"Missile_1") > m_dAnimationPeriod - 0.3f)
			{
				Delete_Timer(L"Missile_1");
				Set_Timer(L"Missile_2");		// 미사일 발사중
			}
		}

		if (IsReady_Timer(L"Missile_2"))
		{
			Start_Timer(L"Missile_2");
			Start_Timer(L"Effect_Missile");

			m_iPlayingAniNum = 27;
			m_pTransformCom->Look_Target(m_pPlayerObserver->Get_Pos());

			if (Get_Timer(L"Effect_Missile") > 0.3)
			{
				//CGameObject* pGameObject = CBelatos_Missile::Create(m_pGraphicDev, m_pTransformCom->Get_Info(Engine::INFO::INFO_POS), m_pTransformCom->Get_Angle()/*m_pTransformCom->Get_Angle()*/, m_pPlayerObserver->Get_Pos());
				//Engine::CManagement::GetInstance()->Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"Belatos_Missile", pGameObject);

				_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
				_vec3 vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
				_vec3 vTargetPos = m_pPlayerObserver->Get_Pos();
				D3DXVec3Normalize(&vLook, &vLook);
				vPos.y += 100.f;
				vTargetPos.y = vPos.y;
				CGameObject* pGameObject = CBelatos_Arrow::Create(m_pGraphicDev, CBelatos_Arrow::BASIC_ARROW, &vPos, &_vec3(0.f, 0.f, 0.f), 100.f, vTargetPos, CTRL_AI_ENERMY_1);
				Engine::Add_GameObject(Engine::GAMEOBJECT, L"Belatos_Arrow", pGameObject);

				// %%%%%%%% 사운드 %%%%%%%%%%
				CSoundMgr::Get_Instance()->SoundOn(137);
				// %%%%%%%% 사운드 %%%%%%%%%%

				Delete_Timer(L"Effect_Missile");
				Set_Timer(L"Effect_Missile");
			}

			if (Get_Timer(L"Missile_2") > 3.f)
			{
				Delete_Timer(L"Missile_2");
				Delete_Timer(L"Effect_Missile");
				Set_Timer(L"Missile_3");		// 미사일 발사 완료
			}
		}

		if (IsReady_Timer(L"Missile_3"))
		{
			Start_Timer(L"Missile_3");

			m_iPlayingAniNum = 9;
			m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
			m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

			if (Get_Timer(L"Missile_3") > m_dAnimationPeriod - 0.3f)
			{
				Delete_Timer(L"Missile_3");
				Delete_Timer(L"Initialize_Missile");
				Change_State(STATE::STATE_IDLE);
				//Set_Timer(L"Missile_3");
			}
		}
	}
}

void CBelatos::Thrust(const _double & dTimeDelta)
{
	if (m_eState == STATE_THRUST)
	{
		m_bUseSpearRim = true;
		if (!IsReady_Timer(L"Thrust"))
		{
			Set_Timer(L"Thrust");
			Set_Timer(L"Effect_Thrust");
		}

		Start_Timer(L"Thrust");

		m_iPlayingAniNum = 31;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();
		m_fAnimationSpeed = 2.f;
		m_dAnimationPeriod *= 0.5;

		// 이펙트 생성
		if (IsReady_Timer(L"Effect_Thrust"))
		{
			if (Get_Timer(L"Thrust") > m_dAnimationPeriod * 0.42f)
			{
				CSoundMgr::Get_Instance()->SoundOn(110);
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_THRUST2, &m_pTransformCom->m_matWorld, m_pTransformCom->Get_Angle(), m_tBaseInfo);
				////cout << "이펙트 생성" << endl;
				Delete_Timer(L"Effect_Thrust");
				// %%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(149);
				// %%%%%%%% 사운드
			}
		}

		if (Get_Timer(L"Thrust") > m_dAnimationPeriod * 0.8f)
		{
			////cout << "end" << endl;
			Delete_Timer(L"Thrust");
			m_fAnimationSpeed = 1.f;
			Change_State(STATE::STATE_IDLE);
		}
	}
}

void CBelatos::JumpAttack(const _double & dTimeDelta)
{
	if (m_eState == STATE_JUMPATTACK)
	{
		m_bUseShieldRim = true;
		m_bUseSpearRim = true;
		if (!IsReady_Timer(L"JumpAttack"))
		{
			Set_Timer(L"JumpAttack");
			Set_Timer(L"Effect_JumpCloud");
			Set_Timer(L"Effect_Lay");
			Set_Timer(L"Effect_JumpLight");
			Set_Timer(L"Sound_Voice");	// 뛸때 목소리
			m_vJumpDir = Engine::GetDirNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		}

		_matrix matSaveWorld;

		Start_Timer(L"JumpAttack");

		m_iPlayingAniNum = 12;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();	// 5.6초

		// 사운드
		if (IsReady_Timer(L"Sound_Voice"))
		{
			if (Get_Timer(L"JumpAttack") > 1.1f)
			{
				CSoundMgr::Get_Instance()->SoundOn(103);
				Delete_Timer(L"Sound_Voice");
			}
		}

		// 이펙트
		if (IsReady_Timer(L"Effect_JumpLight"))
		{
			if (Get_Timer(L"JumpAttack") > 1.25f)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_JUMPLIGHT, &m_pTransformCom->m_matWorld, &m_pTransformCom->m_vAngle, m_tBaseInfo);
				// %%%%%%%% 사운드 %%%%%%%%%%
				CSoundMgr::Get_Instance()->SoundOn(135);
				// %%%%%%%% 사운드 %%%%%%%%%%
				Delete_Timer(L"Effect_JumpLight");
			}
		}

		if (IsReady_Timer(L"Effect_Lay"))
		{
			if (Get_Timer(L"JumpAttack") > m_dAnimationPeriod - 3.6f)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_SHOCKWAVE_LAY, &m_pTransformCom->m_matWorld, &m_pTransformCom->m_vAngle, m_tBaseInfo);
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_SHOCKWAVE2, &m_pTransformCom->m_matWorld, &m_pTransformCom->m_vAngle, m_tBaseInfo);
				_vec3 vAngle = m_pTransformCom->m_vAngle;
				vAngle.y += 50;
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_SHOCKWAVE2, &m_pTransformCom->m_matWorld, &vAngle, m_tBaseInfo);
				// %%%%%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(138);
				CSoundMgr::Get_Instance()->SoundOn(139);
				// %%%%%%%%%%% 사운드
				Delete_Timer(L"Effect_Lay");
			}
		}

		if (IsReady_Timer(L"Effect_JumpCloud"))
		{
			if (Get_Timer(L"JumpAttack") > m_dAnimationPeriod - 3.3f)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BELATOS_SHOCKWAVE, &m_pTransformCom->m_matWorld, &m_pTransformCom->m_vAngle, m_tBaseInfo);
				Delete_Timer(L"Effect_JumpCloud");
			}
		}


		if (Get_Timer(L"JumpAttack") > m_dAnimationPeriod * 0.2f)
		{
			if (Jump(STATE_JUMP::JUMP_ATTACK, 1000.f, 0.f, 0.f, 0.f, /*28*/40.f, m_vJumpDir, dTimeDelta))
			{
				if (Get_Timer(L"JumpAttack") > m_dAnimationPeriod * 0.5f)
				{
					Delete_Timer(L"JumpAttack");
					Change_State(STATE::STATE_IDLE);
				}
			}
		}
	}
}

void CBelatos::JumpShieldAttack(const _double & dTimeDelta)
{
	if (m_eState == STATE_JUMPSHIELDATTACK)
	{
		if (!IsReady_Timer(L"Initialize_JumpShieldAttack"))
		{
			Set_Timer(L"Initialize_JumpShieldAttack");
			Set_Timer(L"JumpShieldAttack_1");
		}

		if (IsReady_Timer(L"JumpShieldAttack_1"))
		{
			Start_Timer(L"JumpShieldAttack_1");

			m_iPlayingAniNum = 36;
			m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
			m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

			if (Get_Timer(L"JumpShieldAttack_1") > m_dAnimationPeriod - 0.3f)
			{
				Delete_Timer(L"JumpShieldAttack_1");
				Set_Timer(L"JumpShieldAttack_2");
			}
		}

		if (IsReady_Timer(L"JumpShieldAttack_2"))
		{
			Start_Timer(L"JumpShieldAttack_2");

			m_iPlayingAniNum = 35;
			m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
			m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

			if (Get_Timer(L"JumpShieldAttack_2") > m_dAnimationPeriod - 0.5f)
			{
				Delete_Timer(L"JumpShieldAttack_2");
				Set_Timer(L"JumpShieldAttack_3");
			}
		}

		if (IsReady_Timer(L"JumpShieldAttack_3"))
		{
			Start_Timer(L"JumpShieldAttack_3");

			m_iPlayingAniNum = 34;
			m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
			m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

			if (Get_Timer(L"JumpShieldAttack_3") > m_dAnimationPeriod - 0.3f)
			{
				Delete_Timer(L"JumpShieldAttack_3");
				Delete_Timer(L"Initialize_JumpShieldAttack");
				Change_State(STATE::STATE_IDLE);
			}
		}
	}
}

void CBelatos::Hit_Front(const _double & dTimeDelta)
{
	if (m_eState == STATE_HIT_FRONT)
	{
		if (!IsReady_Timer(L"Initialize_Hit_Front"))
		{
			Set_Timer(L"Initialize_Hit_Front");
			Set_Timer(L"Hit_Front");
		}

		Start_Timer(L"Hit_Front");

		m_iPlayingAniNum = 43;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Get_Timer(L"Hit_Front") > m_dAnimationPeriod * 0.6)
		{
			Delete_Timer(L"Hit_Front");
			Delete_Timer(L"Initialize_Hit_Front");
			//Change_State(STATE::STATE_IDLE);
			
			if (m_pNaviMeshCom->Check_Landing(m_pTransformCom->Get_Info(Engine::INFO_POS), m_dwNaviIndex))
			{
				Change_State(STATE_BREAK);
			}
			else
			{
				if (m_bFrontHit == true)
				{
					Change_State(STATE_FRONT_HIT_AIRDOWN);
				}
				else if (m_bBackHit == true)
				{
					Change_State(STATE_BACK_HIT_AIRDOWN);
				}
			}
		}
	}
}

void CBelatos::Hit_Back(const _double & dTimeDelta)
{
	if (m_eState == STATE_HIT_BACK)
	{
		if (!IsReady_Timer(L"Initialize_Hit_Back"))
		{
			Set_Timer(L"Initialize_Hit_Back");
			Set_Timer(L"Hit_Back");
		}

		Start_Timer(L"Hit_Back");

		m_iPlayingAniNum = 44;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Get_Timer(L"Hit_Back") > m_dAnimationPeriod * 0.6)
		{
			Delete_Timer(L"Hit_Back");
			Delete_Timer(L"Initialize_Hit_Back");
			//Change_State(STATE::STATE_IDLE);
			
			if (m_pNaviMeshCom->Check_Landing(m_pTransformCom->Get_Info(Engine::INFO_POS), m_dwNaviIndex))
			{
				Change_State(STATE_BREAK);
			}
			else
			{
				if (m_bFrontHit == true)
				{
					Change_State(STATE_FRONT_HIT_AIRDOWN);
				}
				else if (m_bBackHit == true)
				{
					Change_State(STATE_BACK_HIT_AIRDOWN);
				}
			}
		}
	}
}

void CBelatos::Front_Hit_Up(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_FRONT_HIT_UP)
	{
		if (!IsReady_Timer(L"Init_Front_Hit_Up"))
		{
			Set_Timer(L"Init_Front_Hit_Up");
			Set_Timer(L"Front_Hit_Up");
			m_vJumpDir = m_pPlayerObserver->Get_Look();
			D3DXVec3Normalize(&m_vJumpDir, &m_vJumpDir);
			m_vJumpDir.y = 0.f;
			//m_vJumpDir = Engine::GetDirNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		}
		
		Start_Timer(L"Front_Hit_Up");
		
		m_iPlayingAniNum = 46;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Jump(STATE_JUMP::JUMP_UP, 400.f, 400.f, 10.f, 10.f, 70.f, m_vJumpDir, dTimeDelta))
		{
			Delete_Timer(L"Init_Front_Hit_Up");
			Delete_Timer(L"Front_Hit_Up");
			Change_State(STATE::STATE_FRONT_HIT_DOWN);
		}
	}
}

void CBelatos::Front_Hit_Down(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_FRONT_HIT_DOWN)
	{
		if (!IsReady_Timer(L"Init_Front_Hit_Down"))
		{
			Set_Timer(L"Init_Front_Hit_Down");
			Set_Timer(L"Front_Hit_Down");
			//m_vJumpDir = Engine::GetDirNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		}

 		Start_Timer(L"Front_Hit_Down");

		m_iPlayingAniNum = 61;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (m_fAirStunTime < 0)
		{
			if (Jump(STATE_JUMP::JUMP_DOWN, 200.f, 300.f, 0.f, 10.f, 700.f, m_vJumpDir, dTimeDelta))
			{
				Delete_Timer(L"Init_Front_Hit_Down");
				Delete_Timer(L"Front_Hit_Down");
				Change_State(STATE::STATE_FRONT_HIT_GROUND);
				// %%%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(155);
				// %%%%%%%%% 사운드
			}
		}
		else
			m_fAirStunTime -= dTimeDelta;
	}
}

void CBelatos::Front_Hit_Ground(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_FRONT_HIT_GROUND)
	{
		Set_Timer(L"Front_Hit_Gronud");
		Start_Timer(L"Front_Hit_Gronud");

		m_iPlayingAniNum = 57;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Get_Timer(L"Front_Hit_Gronud") > m_dAnimationPeriod - 0.2f)
		{
			Delete_Timer(L"Front_Hit_Gronud");
			Change_State(STATE_FRONT_STANDUP);
		}
	}
}

void CBelatos::Front_Hit_AirUp(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_FRONT_HIT_AIRUP)
	{
		if (!IsReady_Timer(L"Init_Front_Hit_AirUp"))
		{
			Set_Timer(L"Init_Front_Hit_AirUp");
			Set_Timer(L"Front_Hit_AirUp");
			m_vJumpDir = m_pPlayerObserver->Get_Look();
			D3DXVec3Normalize(&m_vJumpDir, &m_vJumpDir);
			m_vJumpDir.y = 0.f;
			//m_vJumpDir = Engine::GetDirNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		}

		Start_Timer(L"Front_Hit_AirUp");

		m_iPlayingAniNum = 46;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Jump(STATE_JUMP::JUMP_UP, 600.f, 0.f, 10.f, 10.f, 70.f, m_vJumpDir, dTimeDelta))
		{
			Delete_Timer(L"Init_Front_Hit_AirUp");
			Delete_Timer(L"Front_Hit_AirUp");
			Change_State(STATE::STATE_FRONT_HIT_DOWN);
		}
	}
}

void CBelatos::Front_Hit_AirDown(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_FRONT_HIT_AIRDOWN)
	{
		if (!IsReady_Timer(L"Init_Front_Hit_AirDown"))
		{
			Set_Timer(L"Init_Front_Hit_AirDown");
			Set_Timer(L"Front_Hit_AirDown");
			//m_vJumpDir = Engine::GetDirNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		}

		Start_Timer(L"Front_Hit_AirDown");

		m_iPlayingAniNum = 61;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (m_fAirStunTime < 0)
		{
			if (Jump(STATE_JUMP::JUMP_DOWN, 200.f, 0.f, 0.f, 10.f, 700.f, m_vJumpDir, dTimeDelta))
			{
				Delete_Timer(L"Init_Front_Hit_AirDown");
				Delete_Timer(L"Front_Hit_AirDown");
				Change_State(STATE::STATE_FRONT_HIT_GROUND);
				// %%%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(155);
				// %%%%%%%%% 사운드
			}
		}
		else
			m_fAirStunTime -= dTimeDelta;
	}
}

void CBelatos::Back_Hit_Up(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_BACK_HIT_UP)
	{
		if (!IsReady_Timer(L"Init_Back_Hit_Up"))
		{
			Set_Timer(L"Init_Back_Hit_Up");
			Set_Timer(L"Back_Hit_Up");
			m_vJumpDir = m_pPlayerObserver->Get_Look();
			D3DXVec3Normalize(&m_vJumpDir, &m_vJumpDir);
			m_vJumpDir.y = 0.f;
			//m_vJumpDir = Engine::GetDirNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		}

		Start_Timer(L"Back_Hit_Up");

		m_iPlayingAniNum = 47;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Jump(STATE_JUMP::JUMP_UP, 400.f, 400.f, 10.f, 10.f, 70.f, m_vJumpDir, dTimeDelta))
		{
			Delete_Timer(L"Init_Back_Hit_Up");
			Delete_Timer(L"Back_Hit_Up");
			Change_State(STATE::STATE_BACK_HIT_DOWN);
		}
	}
}

void CBelatos::Back_Hit_Down(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_BACK_HIT_DOWN)
	{
		if (!IsReady_Timer(L"Init_Back_Hit_Down"))
		{
			Set_Timer(L"Init_Back_Hit_Down");
			Set_Timer(L"Back_Hit_Down");
			//m_vJumpDir = Engine::GetDirNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		}

		Start_Timer(L"Back_Hit_Down");

		m_iPlayingAniNum = 62;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (m_fAirStunTime < 0)
		{
			if (Jump(STATE_JUMP::JUMP_DOWN, 200.f, 300.f, 0.f, 10.f, 700.f, m_vJumpDir, dTimeDelta))
			{
				Delete_Timer(L"Init_Back_Hit_Down");
				Delete_Timer(L"Back_Hit_Down");
				Change_State(STATE::STATE_BACK_HIT_GROUND);
				// %%%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(155);
				// %%%%%%%%% 사운드
			}
		}
		else
			m_fAirStunTime -= dTimeDelta;
	}
}

void CBelatos::Back_Hit_Ground(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_BACK_HIT_GROUND)
	{
		Set_Timer(L"Back_Hit_Ground");
		Start_Timer(L"Back_Hit_Ground");

		m_iPlayingAniNum = 60;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Get_Timer(L"Back_Hit_Ground") > m_dAnimationPeriod)
		{
			Delete_Timer(L"Back_Hit_Ground");
			Change_State(STATE::STATE_BACK_STANDUP);
		}
	}
}

void CBelatos::Back_Hit_AirUp(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_BACK_HIT_AIRUP)
	{
		if (!IsReady_Timer(L"Init_Back_Hit_AirUp"))
		{
			Set_Timer(L"Init_Back_Hit_AirUp");
			Set_Timer(L"Back_Hit_AirUp");
			m_vJumpDir = m_pPlayerObserver->Get_Look();
			D3DXVec3Normalize(&m_vJumpDir, &m_vJumpDir);
			m_vJumpDir.y = 0.f;
			//m_vJumpDir = Engine::GetDirNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		}

		Start_Timer(L"Back_Hit_AirUp");

		m_iPlayingAniNum = 47;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Jump(STATE_JUMP::JUMP_UP, 600.f, 0.f, 10.f, 10.f, 70.f, m_vJumpDir, dTimeDelta))
		{
			Delete_Timer(L"Init_Back_Hit_AirUp");
			Delete_Timer(L"Back_Hit_AirUp");
			Change_State(STATE::STATE_BACK_HIT_DOWN);
		}
	}
}

void CBelatos::Back_Hit_AirDown(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_BACK_HIT_AIRDOWN)
	{
		if (!IsReady_Timer(L"Init_Back_Hit_AirDown"))
		{
			Set_Timer(L"Init_Back_Hit_AirDown");
			Set_Timer(L"Back_Hit_AirDown");
			//m_vJumpDir = Engine::GetDirNoY(m_pPlayerObserver->Get_Pos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));
		}

		Start_Timer(L"Back_Hit_AirDown");

		m_iPlayingAniNum = 62;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (m_fAirStunTime < 0)
		{
			if (Jump(STATE_JUMP::JUMP_DOWN, 200.f, 0.f, 0.f, 10.f, 700.f, m_vJumpDir, dTimeDelta))
			{
				Delete_Timer(L"Init_Back_Hit_AirDown");
				Delete_Timer(L"Back_Hit_AirDown");
				Change_State(STATE::STATE_BACK_HIT_GROUND);
				// %%%%%%%%% 사운드
				CSoundMgr::Get_Instance()->SoundOn(155);
				// %%%%%%%%% 사운드
			}
		}
		else
			m_fAirStunTime -= dTimeDelta;
	}
}

void CBelatos::Front_StandUp(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_FRONT_STANDUP)
	{
		Set_Timer(L"Front_StandUp");
		Start_Timer(L"Front_StandUp");

		m_iPlayingAniNum = 42;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Get_Timer(L"Front_StandUp") > m_dAnimationPeriod - 0.3f)
		{
			Delete_Timer(L"Front_StandUp");
			if (m_bBreak)
				Change_State(STATE_BREAK);
			else
				Change_State(STATE_IDLE);
		}
	}
}

void CBelatos::Back_StandUp(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_BACK_STANDUP)
	{
		Set_Timer(L"Back_StandUp");
		Start_Timer(L"Back_StandUp");

		m_iPlayingAniNum = 41;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Get_Timer(L"Back_StandUp") > m_dAnimationPeriod - 0.3f)
		{
			Delete_Timer(L"Back_StandUp");
			if (m_bBreak)
				Change_State(STATE_BREAK);
			else
				Change_State(STATE_IDLE);
		}
	}
}

void CBelatos::Break(const _double & dTimeDelta)
{
	if (m_eState == STATE_BREAK)
	{
		m_bBreak = true;
		m_fBreakTime += dTimeDelta;

		m_iPlayingAniNum = 40;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (m_bBreak == true)
		{
			if (m_fBreakTime > 2.f)
			{
				m_tBaseInfo.iSuperAmmor = m_tBaseInfo.iMaxSuperAmmor;
				m_bFrontHit = false;
				m_bBackHit = false;
				m_bBreak = false;
				m_fBreakTime = 0.f;
				Change_State(STATE::STATE_IDLE);
			}
		}
	}
}

void CBelatos::Break_1(const _double & dTimeDelta)
{
	if (m_eState == STATE_BREAK_1)
	{
		Set_Timer(L"Break_1");
		Start_Timer(L"Break_1");

		m_iPlayingAniNum = 40;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Get_Timer(L"Break_1") > m_dAnimationPeriod)
		{
			Delete_Timer(L"Break_1");
			Change_State(STATE::STATE_IDLE);
		}
	}
}

void CBelatos::Break_2(const _double & dTimeDelta)
{
	if (m_eState == STATE_BREAK_2)
	{
		Set_Timer(L"Break_2");
		Start_Timer(L"Break_2");

		m_iPlayingAniNum = 80;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();

		if (Get_Timer(L"Break_2") > m_dAnimationPeriod)
		{
			Delete_Timer(L"Break_2");
			Change_State(STATE::STATE_IDLE);
		}
	}
}

void CBelatos::Break_Jump(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_BREAK_JUMP)
	{
		_vec3	vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);	// 몬스터의 Look
		D3DXVec3Normalize(&vLook, &vLook);

		_vec3 vMonsterLook = m_pPlayerObserver->Get_Look();
		D3DXVec3Normalize(&vMonsterLook, &vMonsterLook);

		float fAngle = Engine::GetAngleY(vLook, vMonsterLook);
		////cout << fAngle << endl;

		if (-90 < fAngle && fAngle < 90)
		{
			m_eState = STATE::STATE_BACK_HIT_UP;
			//Change_State(STATE::STATE_BACK_HIT_UP);
		}
		else
		{
			m_eState = STATE::STATE_FRONT_HIT_UP;
			//Change_State(STATE::STATE_FRONT_HIT_UP);
		}
	}
}

void CBelatos::AirUp(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_AIRUP)
	{
		_vec3	vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);	// 몬스터의 Look
		D3DXVec3Normalize(&vLook, &vLook);

		_vec3 vMonsterLook = m_pPlayerObserver->Get_Look();
		D3DXVec3Normalize(&vMonsterLook, &vMonsterLook);

		float fAngle = Engine::GetAngleY(vLook, vMonsterLook);
		////cout << fAngle << endl;

		if (-90 < fAngle && fAngle < 90)
		{
			m_eState = STATE::STATE_BACK_HIT_AIRUP;
			m_bBackHit = true;
		}
		else
		{
			m_eState = STATE::STATE_FRONT_HIT_AIRUP;
			m_bFrontHit = true;
		}
	}
}

void CBelatos::Dead(const _double & dTimeDelta)
{
	if (m_eState == STATE::STATE_DEAD)
	{
		m_fDeadTime += dTimeDelta;

		
		if (!m_bSound_Dead)
		{
			if (m_fDeadTime > 0.5f)
			{
				CSoundMgr::Get_Instance()->SoundOn(116);
				m_bSound_Dead = true;
			}
		}


		m_iPlayingAniNum = 71;
		m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
		m_dAnimationPeriod = m_pDynamicMeshCom->Get_Period();
		m_fAnimationSpeed = 1.f;

		if (m_fDeadTime > m_dAnimationPeriod)
			m_bDissolveStart = true;
	}
}

_bool CBelatos::Jump(STATE_JUMP eState, float fPower, float ColPower, float fGravity, float fAccel, float fDecel, _vec3 _vDir, const _double & dTimeDelta)
{
	if (eState == STATE_JUMP::JUMP_ATTACK)
	{
		if (!IsReady_Timer(L"Jump_Attack"))
		{
			size_t i = Get_TimerSize();

			Set_Timer(L"Jump_Attack");
			m_fPower = fPower;
			m_fAccel = fAccel;
			m_fDecel = 0.f;
			m_vJump_TargetPos = m_pPlayerObserver->Get_Pos();
			m_fJump_FirstDist = Engine::GetDistNoY(m_vJump_TargetPos, *m_pTransformCom->Get_Info(Engine::INFO_POS));
		}

		Start_Timer(L"Jump_Attack");

		_float fDist = Engine::GetDistNoY(m_vJump_TargetPos, *m_pTransformCom->Get_Info(Engine::INFO_POS));
		_vec3 * pPos = m_pTransformCom->Get_Info(Engine::INFO_POS);
		_vec3 vDir = _vDir;
		D3DXVec3Normalize(&vDir, &vDir);

		m_fDecel += (_float)(fDecel * dTimeDelta) + (_float)Get_Timer(L"Jump_Attack");
		m_fPower -= m_fDecel;

		if (m_fPower < 0)
			return true;

		//m_pNaviMeshCom->Find_Index(m_dwNaviIndex, pPos);
		m_pTransformCom->Set_Pos(&(m_pNaviMeshCom->Move_OnNaviMesh(pPos, &(vDir * m_fPower * (_float)dTimeDelta), m_dwNaviIndex)));

		return false;
	}
	else if (eState == STATE_JUMP::JUMP_BACK)
	{
		if (!IsReady_Timer(L"Jump_Back"))
		{
			Set_Timer(L"Jump_Back");
			m_fPower = fPower;
			m_fAccel = fAccel;
			m_fDecel = 0.f;
			m_vJump_TargetPos = m_pPlayerObserver->Get_Pos();
			m_fJump_FirstDist = Engine::GetDistNoY(m_vJump_TargetPos, *m_pTransformCom->Get_Info(Engine::INFO_POS));
		}

		Start_Timer(L"Jump_Back");

		_float fDist = Engine::GetDistNoY(m_vJump_TargetPos, *m_pTransformCom->Get_Info(Engine::INFO_POS));
		_vec3 * pPos = m_pTransformCom->Get_Info(Engine::INFO_POS);
		_vec3 vDir = -_vDir;
		D3DXVec3Normalize(&vDir, &vDir);

		m_fDecel += (_float)(fDecel * dTimeDelta) + (_float)Get_Timer(L"Jump_Back");
		m_fPower -= m_fDecel;

		if (m_fPower < 0)
			return true;

		//m_pNaviMeshCom->Find_Index(m_dwNaviIndex, pPos);
		m_pTransformCom->Set_Pos(&(m_pNaviMeshCom->Move_OnNaviMesh(pPos, &(vDir * m_fPower * (_float)dTimeDelta), m_dwNaviIndex)));

		return false;
	}
	else if (eState == STATE_JUMP::JUMP_UP)
	{
		if (!IsReady_Timer(L"Jump_Up"))
		{
			size_t i = Get_TimerSize();

			Set_Timer(L"Jump_Up");
			m_fPower = fPower;
		}

		// ㅡ ㅡ ㅡ 현재 최대파워에서 0까지의 퍼센트의 몇퍼인지를 구하고, 파워의 퍼센트만큼 (최대각도(fDegree) * 파워퍼센트)의 값을 m_fDegree에 넣음
		//float PerOfPower = (m_fPower / fPower) * 100.f;
		//m_fDegree = fDegree * PerOfPower;
		////cout << m_fDegree << endl;
		// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

		m_fPower -= (_float)(fDecel * dTimeDelta) + fGravity;

		//_matrix matWorld;
		//_vec3	vRight;
		//m_pTransformCom->Get_NRotWorldMatrix(&matWorld);
		//memcpy(&matWorld.m[0], &vRight, sizeof(_vec3));
		//D3DXMatrixRotationAxis(&matWorld, &vRight, -D3DXToRadian(90));
		//
		//// Degree로 Rotation이 된 matWorld의 Look벡터를 받아온다.
		//memcpy(&matWorld.m[2], &m_vJumpDir, sizeof(_vec3));
		//D3DXVec3Normalize(&m_vJumpDir, &m_vJumpDir);

		_vec3 * pPos = m_pTransformCom->Get_Info(Engine::INFO_POS);
		_vec3 vUp = { 0.f, 1.f, 0.f };
		_vec3 vDir = _vDir;
		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Normalize(&vDir, &vDir);

		//m_pNaviMeshCom->Find_Index(m_dwNaviIndex, pPos);
		m_pTransformCom->Set_Pos(&(m_pNaviMeshCom->Move_OnNaviMesh(pPos, &(vUp * m_fPower * (_float)dTimeDelta), m_dwNaviIndex, true)));
		m_pTransformCom->Set_Pos(&(m_pNaviMeshCom->Move_OnNaviMesh(pPos, &(vDir * ColPower * (_float)dTimeDelta), m_dwNaviIndex, true)));

		if (m_fPower < 0)
		{
			m_fPower = 0.f;
			m_fAccel = 0.f;
			m_fDecel = 0.f;
			Delete_Timer(L"Jump_Up");
			return true;
		}
		return false;
	}
	else if (eState == STATE_JUMP::JUMP_DOWN)
	{
		if (!IsReady_Timer(L"Jump_Down"))
		{
			Set_Timer(L"Jump_Down");
			m_fPower = fPower;
		}

		m_fPower += (_float)(fDecel * dTimeDelta);

		_vec3 * pPos = m_pTransformCom->Get_Info(Engine::INFO_POS);
		_vec3 vUp = { 0.f, -1.f, 0.f };
		_vec3 vDir = _vDir;
		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Normalize(&vDir, &vDir);

		m_pNaviMeshCom->Find_Index(m_dwNaviIndex, pPos);

		m_pTransformCom->Set_Pos(&(m_pNaviMeshCom->Move_OnNaviMesh(pPos, &(vUp * m_fPower * (_float)dTimeDelta), m_dwNaviIndex, true)));
		m_pTransformCom->Set_Pos(&(m_pNaviMeshCom->Move_OnNaviMesh(pPos, &(vDir * ColPower * (_float)dTimeDelta), m_dwNaviIndex, true)));

		if (m_pNaviMeshCom->Check_Landing(pPos, m_dwNaviIndex))
		{
			m_fPower = 0.f;
			m_fAccel = 0.f;
			m_fDecel = 0.f;
			Delete_Timer(L"Jump_Down");
			return true;
		}

		return false;
	}
	return false;
}

void CBelatos::View_State()
{
	if (m_eState != m_ePreviousState)
	{
		m_ePreviousState = m_eState;

		switch (m_ePreviousState)
		{
		case CBelatos::STATE_START:
			//cout << "STATE_START" << endl;
			break;
		case CBelatos::STATE_READY:
			//cout << "STATE_READY" << endl;
			break;
		case CBelatos::STATE_IDLE:
			//cout << "STATE_IDLE" << endl;
			break;
		case CBelatos::STATE_MOVE:
			//cout << "STATE_MOVE" << endl;
			break;
		case CBelatos::STATE_RUN:
			//cout << "STATE_RUN" << endl;
			break;
		case CBelatos::STATE_BACKJUMP:
			//cout << "STATE_BACKJUMP" << endl;
			break;
		case CBelatos::STATE_HIT_FRONT:
			//cout << "STATE_HIT_FRONT" << endl;
			break;
		case CBelatos::STATE_HIT_BACK:
			//cout << "STATE_HIT_BACK" << endl;
			break;
		case CBelatos::STATE_FRONT_HIT_UP:
			//cout << "STATE_FRONT_HIT_UP" << endl;
			break;
		case CBelatos::STATE_FRONT_HIT_DOWN:
			//cout << "STATE_FRONT_HIT_DOWN" << endl;
			break;
		case CBelatos::STATE_FRONT_HIT_GROUND:
			//cout << "STATE_FRONT_HIT_GROUND" << endl;
			break;
		case CBelatos::STATE_FRONT_HIT_AIRUP:
			//cout << "STATE_FRONT_HIT_AIRUP" << endl;
			break;
		case CBelatos::STATE_FRONT_HIT_AIRDOWN:
			//cout << "STATE_FRONT_HIT_AIRDOWN" << endl;
			break;
		case CBelatos::STATE_BACK_HIT_UP:
			//cout << "STATE_BACK_HIT_UP" << endl;
			break;
		case CBelatos::STATE_BACK_HIT_DOWN:
			//cout << "STATE_BACK_HIT_DOWN" << endl;
			break;
		case CBelatos::STATE_BACK_HIT_GROUND:
			//cout << "STATE_BACK_HIT_GROUND" << endl;
			break;
		case CBelatos::STATE_BACK_HIT_AIRUP:
			//cout << "STATE_BACK_HIT_AIRUP" << endl;
			break;
		case CBelatos::STATE_BACK_HIT_AIRDOWN:
			//cout << "STATE_BACK_HIT_AIRDOWN" << endl;
			break;
		case CBelatos::STATE_FRONT_STANDUP:
			//cout << "STATE_FRONT_STANDUP" << endl;
			break;
		case CBelatos::STATE_BACK_STANDUP:
			//cout << "STATE_BACK_STANDUP" << endl;
			break;
		case CBelatos::STATE_SHIELDUP:
			//cout << "STATE_SHIELDUP" << endl;
			break;
		case CBelatos::STATE_ATTACK:
			//cout << "STATE_ATTACK" << endl;
			break;
		case CBelatos::STATE_SWEEP:
			//cout << "STATE_SWEEP" << endl;
			break;
		case CBelatos::STATE_JUMPATTACK:
			//cout << "STATE_JUMPATTACK" << endl;
			break;
		case CBelatos::STATE_JUMPSHIELDATTACK:
			//cout << "STATE_JUMPSHIELDATTACK" << endl;
			break;
		case CBelatos::STATE_SHIELDRUSH:
			//cout << "STATE_SHIELDRUSH" << endl;
			break;
		case CBelatos::STATE_MISSILE:
			//cout << "STATE_MISSILE" << endl;
			break;
		case CBelatos::STATE_THRUST:
			//cout << "STATE_THRUST" << endl;
			break;
		case CBelatos::STATE_BREAK:
			//cout << "STATE_BREAK" << endl;
			break;
		case CBelatos::STATE_BREAK_1:
			//cout << "STATE_BREAK_1" << endl;
			break;
		case CBelatos::STATE_BREAK_2:
			//cout << "STATE_BREAK_2" << endl;
			break;
		case CBelatos::STATE_BREAK_JUMP:
			//cout << "STATE_BREAK_JUMP" << endl;
			break;
		case CBelatos::STATE_AIRUP:
			//cout << "STATE_AIRUP" << endl;
			break;
		case CBelatos::STATE_DEAD:
			//cout << "STATE_DEAD" << endl;
			break;
		case CBelatos::STATE_END:
			//cout << "STATE_END" << endl;
			break;
		default:
			//cout << "???" << endl;
			break;
		}
	}
	////cout << m_pTransformCom->Calculate_Distance(m_pPlayer->GetPlayerPos()) << endl;
	////cout << m_pTransformCom->Get_Info(Engine::INFO_POS)->x << " / " << m_pTransformCom->Get_Info(Engine::INFO_POS)->y << " / " << m_pTransformCom->Get_Info(Engine::INFO_POS)->z << endl;
	////cout << m_pTransformCom->m_vAngle.x << " " << m_pTransformCom->m_vAngle.y << " " << m_pTransformCom->m_vAngle.z << endl;
	_matrix matWorld = m_pTransformCom->m_matWorld;
	_vec3 vAngle = *m_pTransformCom->Get_Angle();
	_vec3 fScale = *m_pTransformCom->Get_Scale();
}

void CBelatos::Set_Collision(CBaseObject * pObj, COLLIDER eColliderType)
{
	// 여기서부터 충돌 처리
	if (pObj == nullptr)
		return;

	const BASE_INFO*	pBaseInfo = pObj->Get_BaseInfo();


	// 공격에 맞았다
	if (eColliderType == COLLIDER::COLLIDER_ATT_HIT)
	{
		//if (pBaseInfo->eObjectID == OBJID::OBJECT_EFFECT)
		{
			Set_Collision_Effect(pObj);
		}
		////cout << "벨라토스 어택에맞음" << endl;
	}
	// 몸통끼리 부딪혔다
	else if (eColliderType == COLLIDER::COLLIDER_DEF_HIT)
	{
		////cout << "벨라토스 몸통에맞음" << endl;
	}
}

void CBelatos::Set_Collision_Effect(CBaseObject * pObj)
{
	// 충돌 구현
	// 맵 만들어서 거기에 충돌한 이펙트들 넣어주고, 시간값 돌리면 된다.
	// 스테이지가 구성되면 스테이지 매니저를 통해 누적데미지,킬/데스 등 정보를 알려주면된다.
	// map(pobj, float)

	if (0 >= m_tBaseInfo.iHp)
		return;

	if (m_mapinvincibility.end() == m_mapinvincibility.find(pObj->Get_BaseInfo()->eEffectID))
	{
		////cout << "Belatos Hit" << endl;
		m_mapinvincibility.emplace(pObj->Get_BaseInfo()->eEffectID, pObj->Get_BaseInfo()->fEffectHitTime);

		// KNOCKDOWN은 BREAK에서 살짝 띄워졌다가 쓰러지는거 UP은 공중콤보용으로 높이 띄우는거
		if (pObj->Get_BaseInfo()->eCollisionID == COLLISIONID::COLLISION_KNOCKDOWN)
		{
			Change_State(STATE_BREAK_JUMP);
		}
		else if (pObj->Get_BaseInfo()->eCollisionID == COLLISIONID::COLLISION_AIR_UP)
		{
			Change_State(STATE_AIRUP);
		}
		else if (pObj->Get_BaseInfo()->eCollisionID == COLLISIONID::COLLISION_HIT)
		{
			// 브레이크 0상태에서만
			if (m_eState == STATE::STATE_BREAK)
			{
				if (0 == Engine::RandomNumber(2))
				{
					Change_State(STATE_HIT_FRONT);
				}
				else
				{
					Change_State(STATE_HIT_BACK);
				}
			}

			if (m_eState == STATE::STATE_BACK_HIT_AIRDOWN || m_eState == STATE::STATE_FRONT_HIT_AIRDOWN || m_eState == STATE::STATE_FRONT_HIT_DOWN || m_eState == STATE::STATE_BACK_HIT_DOWN)
			{
				m_fAirStunTime = 1.f;
			}
		}

		// 크리티컬 체크
		_int iCritical = rand() % 10;

		if (pObj->Get_BaseInfo()->iCritical >= iCritical)
			m_bCritical = true;
		else
			m_bCritical = false;

		_int test = pObj->Get_BaseInfo()->iAtt >> 2;
		_int iDif = rand() % test + 1;

		_int iBasicDmg = (pObj->Get_BaseInfo()->iAtt + iDif);// +pObj->Get_BaseInfo()->iCheatATK;

		if (m_bCritical)
		{
			// 카메라 쉐이킹
			CCameraMgr::GetInstance()->Set_ShakingCamera();
			// Critical Effect
			_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
			m_pEffectMgr->Create_Effect_Critical(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 125.f, 0.f) - vDir*70.f));

			m_tBaseInfo.iHp -= iBasicDmg * 1.5;
			m_pUIMgr->CreateDamageBox(m_pGraphicDev, m_pTransformCom, CDamageBox::DAMAGEBOXTYPE::CRITICAL, iBasicDmg *1.5);

		}
		else
		{
			m_tBaseInfo.iHp -= iBasicDmg;
			m_pUIMgr->CreateDamageBox(m_pGraphicDev, m_pTransformCom, CDamageBox::DAMAGEBOXTYPE::BASIC, iBasicDmg);
		}


		if (0 >= m_tBaseInfo.iHp)
		{
			m_tBaseInfo.iHp = 0;
		}
		else
		{
			if (m_fHitSoundTime > 0.5)
			{
				// 사운드
				switch (Engine::RandomNumber(6))
				{
				case 0:
					CSoundMgr::Get_Instance()->SoundOn(123);
					break;
				case 1:
					CSoundMgr::Get_Instance()->SoundOn(124);
					break;
				case 2:
					CSoundMgr::Get_Instance()->SoundOn(125);
					break;
				case 3:
					CSoundMgr::Get_Instance()->SoundOn(126);
					break;
				case 4:
					CSoundMgr::Get_Instance()->SoundOn(127);
					break;
				case 5:
					CSoundMgr::Get_Instance()->SoundOn(128);
					break;
				}
				m_fHitSoundTime = 0.f;
			}
		}

		//데미지 누적
		m_pUIMgr->SetAccumulatedDamage(pObj->Get_BaseInfo()->iAtt);

		// 슈아 깎임
		_int iSuperArmorDamage = (_int)((pObj->Get_BaseInfo()->iAtt + iDif)*0.3);
		m_tBaseInfo.iSuperAmmor -= iSuperArmorDamage;

		if (0 >= m_tBaseInfo.iSuperAmmor)
		{
			m_tBaseInfo.iSuperAmmor = 0;
			if (!m_bBreak)
			{
				// 카메라 쉐이킹
				CCameraMgr::GetInstance()->Set_ShakingCamera();

				_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
				m_pEffectMgr->Create_Effect_Break(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 125.f, 0.f) - vDir*70.f));
			}
		}
		else
		{
			m_pUIMgr->SetAccumulatedBreak(iSuperArmorDamage);
		}

		m_dHitTime = 0.1;
		CCameraMgr::GetInstance()->Set_ShakingCamera();
		
		m_fAnimationSpeed = 1.f;
	}
}

void CBelatos::Check_SuperAmor(const _double & dTimeDelta)
{
	if (!m_bBreak)
	{
		if (m_tBaseInfo.iSuperAmmor == 0)
			Change_State(STATE::STATE_BREAK);
	}
}

void CBelatos::Check_HitTime(const _double & dTimeDelta)
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

void CBelatos::Check_PlayerDead(const _double & dTimeDelta)
{
	if (m_bSound_PlayerDead == false)
	{
		if (m_pPlayerObserver->Get_Hp() == 0)
		{
			m_fPlayerDeadTime += dTimeDelta;

			if (m_fPlayerDeadTime > 5)
			{
				CSoundMgr::Get_Instance()->SoundOn(131);
				m_bSound_PlayerDead = true;
				m_fPlayerDeadTime = 0;
			}
		}
	}


	if (m_pPlayerObserver->Get_Hp() > 0)
	{
		m_bSound_PlayerDead = false;
		m_fPlayerDeadTime = 0;
	}
}

HRESULT CBelatos::Clone_Component()
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

	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_Belatos"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CBelatos::Setup_ShaderProps(LPD3DXEFFECT & pEffect, const _double& dTimeDelta)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	if (m_bIsHit)
	{
		m_iPass = 19;
		_vec3 vCamPos = CCameraMgr::GetInstance()->Get_Pos();
		pEffect->SetValue("g_vCamPos", &vCamPos, sizeof(_vec3));
		_vec3 vPos = m_pTransformCom->Get_Info_byWorld(Engine::INFO_POS);
		pEffect->SetValue("g_vPosition", &vPos, sizeof(_vec3));
		pEffect->SetValue("g_vRimColor", &_vec3(1.f, 0.f, 0.f), sizeof(_vec3));
		pEffect->SetValue("g_vRimMinMax", &_vec2(0.01f, 0.1f), sizeof(_vec2));
		m_bUseShieldRim = false;
		m_bUseSpearRim = false;
	}
	else
		m_iPass = 0;
	return S_OK;
}

void CBelatos::SaveSoundNum()
{
	//// 101 / Belatos Long_Attack_1 / Long_Attack_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Long_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 102 / Belatos Long_Attack_2 / Long_Attack_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Long_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 103 / Belatos Long_Attack_2 / Long_Attack_3
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Long_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 104 / Belatos Long_Attack_2 / Long_Attack_4
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Long_04_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 105 / Belatos Middle_Attack_1 / Middle_Attack_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Middle_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 106 / Belatos Middle_Attack_2 / Middle_Attack_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Middle_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 107 / Belatos Middle_Attack_3 / Middle_Attack_3
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Middle_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 108 / Belatos Middle_Attack_4 / Middle_Attack_4
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Middle_04_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 109 / Belatos Middle_Attack_5 / Middle_Attack_5
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Middle_05_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 110 / Belatos Short_Attack_1 / Short_Attack_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 111 / Belatos Short_Attack_2 / Short_Attack_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 112 / Belatos Short_Attack_3 / Short_Attack_3
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 113 / Belatos Short_Attack_4 / Short_Attack_4
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_04_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 114 / Belatos Short_Attack_5 / Short_Attack_5
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_05_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 115 / Belatos Short_Attack_6 / Short_Attack_6
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_06_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 116 / Belatos Dying_Dialogue_1 / Dying_Dialogue_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Dying_Dialogue_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 117 / Belatos Dying_Dialogue_2 / Dying_Dialogue_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Dying_Dialogue_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 118 / Belatos Dying_Middle_1 / Dying_Middle_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Dying_Middle_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 119 / Belatos Dying_Shrot_1 / Dying_Shrot_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Dying_Short_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 120 / Belatos Dying_Shrot_2 / Dying_Shrot_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Dying_Short_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 121 / Belatos Hurt_Long_1 / Hurt_Long_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Long_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 122 / Belatos Hurt_Long_2 / Hurt_Long_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Long_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 123 / Belatos Hurt_Middle_1 / Hurt_Middle_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Middle_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 124 / Belatos Hurt_Middle_2 / Hurt_Middle_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Middle_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 125 / Belatos Hurt_Short_1 / Hurt_Short_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Short_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 126 / Belatos Hurt_Short_2 / Hurt_Short_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Short_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 127 / Belatos Hurt_Short_3 / Hurt_Short_3
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Short_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 128 / Belatos Hurt_Short_4 / Hurt_Short_4
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Short_04_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 129 / Belatos Idle_1 / Idle_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Idle_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 130 / Belatos Idle_2 / Idle_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Idle_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 131 / Belatos Idle_3 / Idle_3
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Idle_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 132 / Belatos Player_Dead_1 / Player_Dead_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_PlayerDead_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 133 / Belatos Start_1 / Start_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Start_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 134 / Belatos UltimateSkill_1 / UltimateSkill_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_UltimateSkill_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 135 / Belatos Start_EffectSound / Start_EffectSound
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Start_EffectSound.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 136 / Belatos Start_2 / Start_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_StartVoice_2.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 137 / Belatos Arrow / Arrow
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Arrow.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 138 / Belatos JumpAttack_Sword / JumpAttack_Sword
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_JumpAttack_SwordSound.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 139 / Belatos JumpAttack_ShockSound / JumpAttack_ShockSound
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_JumpAttack_ShockSound.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 140 / Belatos Shield_Up_1 / Shield_Up_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_1.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 141 / Belatos Shield_Up_2 / Shield_Up_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_2.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 142 / Belatos Shield_Up_3 / Shield_Up_3
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_3.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 143 / Belatos Shield_Up_4 / Shield_Up_4
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_4.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 144 / Belatos Shield_Up_5 / Shield_Up_5
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_5.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 145 / Belatos Shield_Up_6 / Shield_Up_6
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_6.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 146 / Belatos ShieldRush_1 / ShieldRush_1
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldRush_1.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);

	//// 147 / Belatos ShieldRush_2 / ShieldRush_2
	//eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldRush_2.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);
}

CBelatos * CBelatos::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CBelatos* pInstance = new CBelatos(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBelatos::Free()
{
	Engine::UnSubscribe(Engine::PLAYER, m_pPlayerObserver);
	Engine::Safe_Release(m_pPlayerObserver);
	m_mapinvincibility.clear();
	CDynamicMeshObject::Free();
}
