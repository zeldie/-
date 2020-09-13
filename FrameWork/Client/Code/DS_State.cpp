#include "stdafx.h"
#include "DS_State.h"

CDS_State::CDS_State()
	://m_bIsReserve(false),
	m_bSecondRocBreak(false),
	m_bIsMinimumTime(false),
	m_bAfterEffect(false),
	//	m_eReserveState(CPlayer::STATE_END),
	//m_dbMotionCancel(0),
	m_dbBasicLoopTime(0),
	m_dbIsChargingTime(0),
	m_dbWaitingStartMotion(0)
{

}

CDS_State::~CDS_State()
{

}

void CDS_State::Enter(CPlayer * pPlayer)
{
	m_pMeshCom = pPlayer->Get_PlayerMesh();
	m_pTransform = pPlayer->Get_TransformCom();
	m_pNaviCom = pPlayer->Get_NaviCom();
	m_tBaseInfo = pPlayer->Get_BaseInfo();
	m_bCreateEffect = true;
	pPlayer->Set_BattleType(CPlayer::TYPE_BATTLE);
}

void CDS_State::Update(CPlayer * pPlayer, const _double dTimeDelta)
{
	if (m_bIsAttacked)
		return;
	Check_Key(pPlayer, dTimeDelta);
	Reserve_State(pPlayer, dTimeDelta);
	Run_State(pPlayer, dTimeDelta);
	End_DS_State(pPlayer);
}

void CDS_State::ChangeState(CPlayerState * pState)
{
}

void CDS_State::Check_Key(CPlayer * pPlayer, const _double dTimeDelta)
{
	Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_R", Engine::MESH, Engine::ID_STATIC));
	pWeaponCom->Set_AnimationSet(0);

	pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_L", Engine::MESH, Engine::ID_STATIC));
	pWeaponCom->Set_AnimationSet(0);

	CDualSword* pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_L"));
	pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

	switch (m_eStartMotion)	// 마우스 버튼
	{
		case Engine::SM_LB:
		{

			if (pPlayer->Get_Jump())
			{
				switch (pPlayer->Get_AirComboLevel())
				{
				case 0:
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::DS_AIRCOMBO_01);
					pPlayer->Set_State(CPlayer::DS_AIRCOMBO_01);
					pPlayer->Set_AirComboLevel(1);
					m_eStartMotion = Engine::SM_END;
				}
				break;
				case 1:
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::DS_AIRCOMBO_02);
					pPlayer->Set_State(CPlayer::DS_AIRCOMBO_02);
					pPlayer->Set_AirComboLevel(2);
					m_eStartMotion = Engine::SM_END;
				}
				break;
				case 2:
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::DS_AIRCOMBO_03);
					pPlayer->Set_State(CPlayer::DS_AIRCOMBO_03);
					pPlayer->Set_AirComboLevel(3);
					m_eStartMotion = Engine::SM_END;
				}
				break;
				case 3:
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::DS_AIRCOMBO_04);
					pPlayer->Set_State(CPlayer::DS_AIRCOMBO_04);
					pPlayer->Set_AirComboLevel(4);
					m_eStartMotion = Engine::SM_END;
					break;
				}
				default:
				{
					pPlayer->Set_Fall_Start();
					pPlayer->ExChange_State(new CCsState);
				}
					break;
				}
			}
			
			else
			{
				pPlayer->Set_State(CPlayer::DS_BASICCOMBO_01);
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL1, m_pTransform->Get_WorldMatrix(), &(m_pTransform->m_vAngle), *m_tBaseInfo);
			}
			
			m_eStartMotion = Engine::SM_END;

			break;
		}
		case Engine::SM_RB:
		{
			pPlayer->Set_State(CPlayer::DS_SPECIALCOMBO_CHARGE);
			m_eStartMotion = Engine::SM_END;

			break;
		}
		case Engine::SM_E:
		{
			pPlayer->Set_State(CPlayer::DS_SKILL_FATEDCIRCLE_START);
			m_eStartMotion = Engine::SM_END;

			break;
		}
		case Engine::SM_R:
		{
			pPlayer->Set_State(CPlayer::DS_SKILL_SCART);
			m_eStartMotion = Engine::SM_END;

			break;
		}
		case Engine::SM_F:
		{
			pPlayer->Set_State(CPlayer::DS_SKILL_CRITICALX);
			m_eStartMotion = Engine::SM_END;

			break;
		}
		case Engine::SM_Q:
		{
			pPlayer->Set_State(CPlayer::DS_RAGESKILL_GENOCIDE);
			m_eStartMotion = Engine::SM_END;

			break;
		}
	}
}

void CDS_State::Reserve_State(CPlayer * pPlayer, const _double dTimeDelta)
{
	if (CPlayer::DS_BASICCOMBO_01 == pPlayer->Get_CurState())
	{
		if (Engine::MouseDown(Engine::DIM_LB))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::DS_BASICCOMBO_02;
		}
		if (m_pMeshCom->Get_TrackTime()<m_pMeshCom->Get_Period()*0.3)
		{
			_vec3 vPos = pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
		}
	}
	else if (CPlayer::DS_BASICCOMBO_02 == pPlayer->Get_CurState())
	{
		if (m_bCreateEffect)
		{
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL2, m_pTransform->Get_WorldMatrix(), &(m_pTransform->m_vAngle), *m_tBaseInfo);
			////cout << "test" << endl;
			m_bCreateEffect = false;
		}
		if (Engine::MouseDown(Engine::DIM_LB))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::DS_BASICCOMBO_03;
		}
		if (m_pMeshCom->Get_TrackTime()<m_pMeshCom->Get_Period()*0.3)
		{
			_vec3 vPos = pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
		}
	}
	else if (CPlayer::DS_BASICCOMBO_03 == pPlayer->Get_CurState())
	{
		if (m_bCreateEffect)
		{
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL3, m_pTransform->Get_WorldMatrix(), &(m_pTransform->m_vAngle), *m_tBaseInfo);
			m_bCreateEffect = false;
		}

		if (Engine::MouseDown(Engine::DIM_LB))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::DS_BASICCOMBO_04;
		}
		if (m_pMeshCom->Get_TrackTime()<m_pMeshCom->Get_Period()*0.3)
		{
			_vec3 vPos = pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
		}
	}
	else if (CPlayer::DS_BASICCOMBO_04 == pPlayer->Get_CurState())
	{
		if (!IsReady_Timer(L"Init_Basic_Combo4"))
		{
			// Init_Basic_Combo4는 밑에 실제 애니메이션 돌아가는 함수에서 애니메이션이 끝날때 삭제해준다.
			Set_Timer(L"Init_Basic_Combo4");
			Set_Timer(L"Basic_Combo4");
		} 
		
		Start_Timer(L"Init_Basic_Combo4");

		
		if (IsReady_Timer(L"Basic_Combo4"))
		{
			if (m_bCreateEffect)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL2, m_pTransform->Get_WorldMatrix(), &(m_pTransform->m_vAngle), *m_tBaseInfo);
				m_bCreateEffect = false;
			}


			if (!m_bAfterEffect && Get_Timer(L"Init_Basic_Combo4") > m_pMeshCom->Get_Period() * 0.35)
			{
				m_bAfterEffect = true;
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL1, m_pTransform->Get_WorldMatrix(), &(m_pTransform->m_vAngle), *m_tBaseInfo);
				Delete_Timer(L"Basic_Combo4");
			}
		}

		if (Engine::MouseDown(Engine::DIM_LB))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::DS_BASICCOMBOEX2_START;
		}

		if (Engine::MouseDown(Engine::DIM_RB))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::DS_DASHCOMBO;
		}
		if (m_pMeshCom->Get_TrackTime()<m_pMeshCom->Get_Period()*0.3)
		{
			_vec3 vPos = pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
		}
	}
	else if (CPlayer::DS_BASICCOMBOEX2_START == pPlayer->Get_CurState() || CPlayer::DS_BASICCOMBOEX2_END == pPlayer->Get_CurState())
	{
		if (Engine::MousePressing(Engine::DIM_LB))
		{
			if (!IsReady_Timer(L"Loop_End"))
			{
				// END상태에서 일정시간 이내에서만 LOOP로 넘어갈수있고, END상태가 어느정도 진행된 상태에선 LOOP로 넘어갈수 없다.
				if (CPlayer::DS_BASICCOMBOEX2_START == pPlayer->Get_CurState() || Get_Timer(L"DS_BASICCOMBO_EX2_END") < m_pMeshCom->Get_Period() * 0.3)
				{
					Set_Timer(L"MousePress");
					Start_Timer(L"MousePress");
					
					if (Get_Timer(L"MousePress") > 0.2)
					{
						// 한번 LOOP를 사용했으면 다시 사용하지 않게 막으려고 타이머로 계산. LOOP -> END로 상태변환후 END가 종료될때 이 타이머를 삭제해준다.		
						Set_Timer(L"Loop_End");

						pPlayer->Set_State(CPlayer::DS_BASICCOMBOEX2_LOOP);
						Delete_Timer(L"MousePress");
						Delete_Timer(L"DS_BASICCOMBO_EX2_START");
						Delete_Timer(L"DS_BASICCOMBO_EX2_END");
					}
				}
			}
		}
		else
			Delete_Timer(L"MousePress");
	}
	else if (CPlayer::DS_AIRCOMBO_01 == pPlayer->Get_CurState())
	{
		if (Engine::MouseDown(Engine::DIM_LB))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::DS_AIRCOMBO_02;
			pPlayer->Set_AirComboLevel(1);
		}
	}  
	else if (CPlayer::DS_AIRCOMBO_02 == pPlayer->Get_CurState())
	{
		if (Engine::MouseDown(Engine::DIM_LB))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::DS_AIRCOMBO_03;
			pPlayer->Set_AirComboLevel(2);

		}
	}
	else if (CPlayer::DS_AIRCOMBO_03 == pPlayer->Get_CurState())
	{
		if (Engine::MouseDown(Engine::DIM_LB))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::DS_AIRCOMBO_04;
			pPlayer->Set_AirComboLevel(3);
		}
	}
	else if (CPlayer::DS_AIRCOMBO_04 == pPlayer->Get_CurState())
	{
		pPlayer->Set_AirComboLevel(4);
	}
}

void CDS_State::Run_State(CPlayer * pPlayer, const _double dTimeDelta)
{
	// 평타 5번 , 평타 4 + 꾹누르기1 , 평타4 + 적 공중에 띄우기
	Attack_Normal_5(pPlayer, dTimeDelta);
	// 우클릭
	Attack_Charge(pPlayer, dTimeDelta);
	// 공중평타
	Air_Combo(pPlayer, dTimeDelta);
	// E 스킬
	E_Skill(pPlayer, dTimeDelta);
	// R 스킬
	R_Skill(pPlayer, dTimeDelta);
	// F 스킬
	F_Skill(pPlayer, dTimeDelta);
	// Q 스킬
	Q_Skill(pPlayer, dTimeDelta);
}

void CDS_State::End_DS_State(CPlayer * pPlayer)
{
	size_t iSize = m_mapTimer.size();

	if ((Get_TimerSize() == 0) && (m_bIsReserve == false))
	{
		CDualSword* pWeapon = static_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_L"));
		pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_WAIT);

		m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_COMBATWAIT);
		pPlayer->ExChange_State(new CCsState);
	}
}

void CDS_State::Attack_Normal_5(CPlayer * pPlayer, const _double dTimeDelta)
{
	if (CPlayer::DS_BASICCOMBO_01 == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_BASICCOMBO_01");
		Start_Timer(L"DS_BASICCOMBO_01");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_BASICCOMBO_01);

		// 다음 동작이 예약되있으면 최소한의 동작 후 다음동작으로 넘어가게 만듬
		if (m_bIsReserve)
		{
			if (Get_Timer(L"DS_BASICCOMBO_01") > m_pMeshCom->Get_Period() * 0.25)
			{
				pPlayer->Set_State(m_eReserveState);
				m_bIsReserve = false;
				Delete_Timer(L"DS_BASICCOMBO_01");
			}
		}
		// 다음 동작이 없고, 현재 동작이 끝나면 행동을 끝낸다.
		else if (Get_Timer(L"DS_BASICCOMBO_01") > m_pMeshCom->Get_Period() * 0.6)
			Delete_Timer(L"DS_BASICCOMBO_01");

	}

	if (CPlayer::DS_BASICCOMBO_02 == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_BASICCOMBO_02");
		Start_Timer(L"DS_BASICCOMBO_02");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_BASICCOMBO_02);

		// 다음 동작이 예약되있으면 최소한의 동작 후 다음동작으로 넘어가게 만듬
		if (m_bIsReserve)
		{
			if (Get_Timer(L"DS_BASICCOMBO_02") > m_pMeshCom->Get_Period() * 0.25)
			{
				pPlayer->Set_State(m_eReserveState);
				m_bIsReserve = false;
				m_bCreateEffect = true;
				Delete_Timer(L"DS_BASICCOMBO_02");
			}
		}
		// 다음 동작이 없고, 현재 동작이 끝나면 행동을 끝낸다.
		else if (Get_Timer(L"DS_BASICCOMBO_02") > m_pMeshCom->Get_Period() * 0.6)
		{
			Delete_Timer(L"DS_BASICCOMBO_02");
			m_bCreateEffect = true;
		}
	}

	if (CPlayer::DS_BASICCOMBO_03 == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_BASICCOMBO_03");
		Start_Timer(L"DS_BASICCOMBO_03");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_BASICCOMBO_03);

		// 다음 동작이 예약되있으면 최소한의 동작 후 다음동작으로 넘어가게 만듬
		if (m_bIsReserve)
		{
			if (Get_Timer(L"DS_BASICCOMBO_03") > m_pMeshCom->Get_Period() * 0.3)
			{
				pPlayer->Set_State(m_eReserveState);
				m_bIsReserve = false;
				m_bCreateEffect = true;
				Delete_Timer(L"DS_BASICCOMBO_03");
			}
		}
		// 다음 동작이 없고, 현재 동작이 끝나면 행동을 끝낸다.
		else if (Get_Timer(L"DS_BASICCOMBO_03") >= m_pMeshCom->Get_Period() * 0.6)
		{
			Delete_Timer(L"DS_BASICCOMBO_03");
			m_bCreateEffect = true;
		}
	}

	if (CPlayer::DS_BASICCOMBO_04 == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_BASICCOMBO_04");
		Start_Timer(L"DS_BASICCOMBO_04");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_BASICCOMBO_04);

		// 다음 동작이 예약되있으면 최소한의 동작 후 다음동작으로 넘어가게 만듬
		if (m_bIsReserve)
		{
			if (Get_Timer(L"DS_BASICCOMBO_04") > m_pMeshCom->Get_Period() * 0.5)
			{
				pPlayer->Set_State(m_eReserveState);
				m_bIsReserve = false;
				m_bCreateEffect = true;
				Delete_Timer(L"DS_BASICCOMBO_04");
				if (IsReady_Timer(L"Init_Basic_Combo4"))
					Delete_Timer(L"Init_Basic_Combo4");
				if (IsReady_Timer(L"Basic_Combo4"))
					Delete_Timer(L"Basic_Combo4");
			}
		}
		// 다음 동작이 없고, 현재 동작이 끝나면 행동을 끝낸다.
		else if (Get_Timer(L"DS_BASICCOMBO_04") >= m_pMeshCom->Get_Period() * 0.7)
		{
			Delete_Timer(L"DS_BASICCOMBO_04");
			if (IsReady_Timer(L"Init_Basic_Combo4"))
				Delete_Timer(L"Init_Basic_Combo4");
			if (IsReady_Timer(L"Basic_Combo4"))
				Delete_Timer(L"Basic_Combo4");
			m_bCreateEffect = true;
		}
	}

	if (CPlayer::DS_DASHCOMBO == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_DASHCOMBO");
		Start_Timer(L"DS_DASHCOMBO");

		//if (m_bCreateEffect)
		//{
		//	m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL1, m_pTransform->Get_WorldMatrix(), &(m_pTransform->m_vAngle), *m_tBaseInfo);
		//	m_bCreateEffect = false;
		//}

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_DASHCOMBO);

		if (Get_Timer(L"DS_DASHCOMBO") >= m_pMeshCom->Get_Period() * 0.7)
		{
			Delete_Timer(L"DS_DASHCOMBO");
			m_bCreateEffect = true;
		}
	}

	if (CPlayer::DS_BASICCOMBOEX2_START == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_BASICCOMBO_EX2_START");
		Start_Timer(L"DS_BASICCOMBO_EX2_START");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_BASICCOMBOEX2_START);

		// 다음 동작이 없고, 현재 동작이 끝나면 행동을 끝낸다.
		if (Get_Timer(L"DS_BASICCOMBO_EX2_START") > m_pMeshCom->Get_Period() * 0.2)
		{
			Delete_Timer(L"DS_BASICCOMBO_EX2_START");

			// 예약된 상태가 없으면 자동으로 START -> END로 넘어간다.
			pPlayer->Set_State(CPlayer::DS_BASICCOMBOEX2_END);
			Set_Timer(L"NextState_DS_BASICCOMBO_EX2_END");
			//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL5, m_pTransform->Get_WorldMatrix(), &(m_pTransform->m_vAngle), *m_tBaseInfo);
		}
	}

	if (CPlayer::DS_BASICCOMBOEX2_LOOP == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_BASICCOMBO_EX2_LOOP");
		Start_Timer(L"DS_BASICCOMBO_EX2_LOOP");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_BASICCOMBOEX2_LOOP);

		// 현재 동작이 끝나면 행동을 끝낸다.
		if (Get_Timer(L"DS_BASICCOMBO_EX2_LOOP") > m_pMeshCom->Get_Period() * 3)
		{
			pPlayer->Set_State(CPlayer::DS_BASICCOMBOEX2_END);
			// 타이머 사이즈가 0이면 공격상태가 종료되므로 타이머에 하나 넣어준다.
			Delete_Timer(L"DS_BASICCOMBO_EX2_LOOP");
			Set_Timer(L"NextState_DS_BASICCOMBO_EX2_END");
		}
	}

	if (CPlayer::DS_BASICCOMBOEX2_END == pPlayer->Get_CurState())
	{
		// LOOP -> END or START -> END로 이어주는 타이머가 아직 존재하면 타이머를 싹 비운다.
		if (IsReady_Timer(L"NextState_DS_BASICCOMBO_EX2_END"))
		{
			Set_Timer(L"DS_BASICCOMBO_EX2_END");
			Delete_Timer(L"NextState_DS_BASICCOMBO_EX2_END");
		}

		Start_Timer(L"DS_BASICCOMBO_EX2_END");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_BASICCOMBOEX2_END);

		// 현재 동작이 끝나면 행동을 끝낸다.
		if (Get_Timer(L"DS_BASICCOMBO_EX2_END") > m_pMeshCom->Get_Period() * 0.6)
		{
			// END동작에서 LOOP로 들어가면 Reserve가 true상태로 바뀌는데 해결해줘야한다.
			m_bIsReserve = false;
			Delete_Timer(L"Loop_End");
			Delete_Timer(L"DS_BASICCOMBO_EX2_END");
			//Clear_Timer();
			
			//size_t a = m_mapTimer.size();
			//int b = 0;
		}
	}
}

void CDS_State::Attack_Charge(CPlayer * pPlayer, const _double dTimeDelta)
{
	if (CPlayer::DS_SPECIALCOMBO_CHARGE == pPlayer->Get_CurState())
	{
		if (Engine::MousePressing(Engine::DIM_RB))
		{
			Set_Timer(L"DS_SPECIALCOMBO_CHARGE");
			Start_Timer(L"DS_SPECIALCOMBO_CHARGE");

			m_pMeshCom->Set_AnimationSet(CPlayer::DS_SPECIALCOMBO_CHARGE);
			pPlayer->Set_AnimationSpeed(2);

			if (Get_Timer(L"DS_SPECIALCOMBO_CHARGE") > m_pMeshCom->Get_Period() * 0.25)
			{
				pPlayer->Set_State(CPlayer::DS_SPECIALCOMBO_CHARGE_LOOP);
				Delete_Timer(L"DS_SPECIALCOMBO_CHARGE");
			}
		}
		else
			Delete_Timer(L"DS_SPECIALCOMBO_CHARGE");
	}

	if (CPlayer::DS_SPECIALCOMBO_CHARGE_LOOP == pPlayer->Get_CurState())
	{
		if (Engine::MouseUp(Engine::DIM_RB))
		{
			if (Get_Timer(L"DS_SPECIALCOMBO_CHARGE_LOOP") < m_pMeshCom->Get_Period() * 3)
			{
				pPlayer->Set_State(CPlayer::DS_SPECIALCOMBO_END);
				Delete_Timer(L"DS_SPECIALCOMBO_CHARGE_LOOP");
			}
		}

		if (Engine::MousePressing(Engine::DIM_RB))
		{
			Set_Timer(L"DS_SPECIALCOMBO_CHARGE_LOOP");
			Start_Timer(L"DS_SPECIALCOMBO_CHARGE_LOOP");

			m_pMeshCom->Set_AnimationSet(CPlayer::DS_SPECIALCOMBO_CHARGE_LOOP);
			pPlayer->Set_AnimationSpeed(1);

			if (Get_Timer(L"DS_SPECIALCOMBO_CHARGE_LOOP") > m_pMeshCom->Get_Period() * 3)
			{
				pPlayer->Set_State(CPlayer::DS_SPECIALCOMBO_END);
				Delete_Timer(L"DS_SPECIALCOMBO_CHARGE_LOOP");
			}
		}
		else
			Delete_Timer(L"DS_SPECIALCOMBO_CHARGE_LOOP");
	}

	if (CPlayer::DS_SPECIALCOMBO_END == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_SPECIALCOMBO_END");
		Start_Timer(L"DS_SPECIALCOMBO_END");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_SPECIALCOMBO_END);
		pPlayer->Set_AnimationSpeed(2);

		if (Get_Timer(L"DS_SPECIALCOMBO_END") > m_pMeshCom->Get_Period() * 0.5)
			Delete_Timer(L"DS_SPECIALCOMBO_END");
	}
}

void CDS_State::Air_Combo(CPlayer * pPlayer, const _double dTimeDelta)
{
	if (CPlayer::DS_AIRCOMBO_01 == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_AIRCOMBO_01");
		Start_Timer(L"DS_AIRCOMBO_01");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_AIRCOMBO_01);
		pPlayer->Set_AnimationSpeed(2);

		if (m_bIsReserve)
		{
			if (Get_Timer(L"DS_AIRCOMBO_01") > m_pMeshCom->Get_Period() * 0.2)
			{
				Delete_Timer(L"DS_AIRCOMBO_01");
				pPlayer->Set_State(m_eReserveState);
				m_bIsReserve = false;
			}

		}
		else if (Get_Timer(L"DS_AIRCOMBO_01") > m_pMeshCom->Get_Period() * 0.3)
		{
			//pPlayer->Set_State(CPlayer::DS_AIRCOMBO_02);
			Delete_Timer(L"DS_AIRCOMBO_01");
			pPlayer->Set_Fall_Start();
			pPlayer->ExChange_State(new CCsState);
		}
	}

	if (CPlayer::DS_AIRCOMBO_02 == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_AIRCOMBO_02");
		Start_Timer(L"DS_AIRCOMBO_02");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_AIRCOMBO_02);
		pPlayer->Set_AnimationSpeed(2);

		if (m_bIsReserve)
		{
			if (Get_Timer(L"DS_AIRCOMBO_02") > m_pMeshCom->Get_Period() * 0.2)
			{
				Delete_Timer(L"DS_AIRCOMBO_02");
				pPlayer->Set_State(m_eReserveState);
				m_bIsReserve = false;
			}

		}
		else if (Get_Timer(L"DS_AIRCOMBO_02") > m_pMeshCom->Get_Period() * 0.3)
		{
			//pPlayer->Set_State(CPlayer::DS_AIRCOMBO_03);
			Delete_Timer(L"DS_AIRCOMBO_02");
			pPlayer->Set_Fall_Start();
			pPlayer->ExChange_State(new CCsState);
		}
	}

	if (CPlayer::DS_AIRCOMBO_03 == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_AIRCOMBO_03");
		Start_Timer(L"DS_AIRCOMBO_03");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_AIRCOMBO_03);
		pPlayer->Set_AnimationSpeed(2);

		if (m_bIsReserve)
		{
			if (Get_Timer(L"DS_AIRCOMBO_03") > m_pMeshCom->Get_Period() * 0.2)
			{
				Delete_Timer(L"DS_AIRCOMBO_03");
				pPlayer->Set_State(m_eReserveState);
				m_bIsReserve = false;
			}
		}
		else if (Get_Timer(L"DS_AIRCOMBO_03") > m_pMeshCom->Get_Period() * 0.3)
		{
			//pPlayer->Set_State(CPlayer::DS_AIRCOMBO_04);
			Delete_Timer(L"DS_AIRCOMBO_03");
			pPlayer->Set_Fall_Start();
			pPlayer->ExChange_State(new CCsState);
		}
	}

	if (CPlayer::DS_AIRCOMBO_04 == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_AIRCOMBO_04");
		Start_Timer(L"DS_AIRCOMBO_04");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_AIRCOMBO_04);
		pPlayer->Set_AnimationSpeed(2);

		if (Get_Timer(L"DS_AIRCOMBO_04") > m_pMeshCom->Get_Period() * 0.3)
		{
			Delete_Timer(L"DS_AIRCOMBO_04");
			//pPlayer->Set_AirComboLevel(0);
			pPlayer->Set_Fall_Start();
			pPlayer->ExChange_State(new CCsState);
		}
	}
}

void CDS_State::E_Skill(CPlayer * pPlayer, const _double dTimeDelta)
{
	if (CPlayer::DS_SKILL_FATEDCIRCLE_START == pPlayer->Get_CurState())
	{
		if (Engine::KeyDown(DIK_E))
		{
			CUIMgr::GetInstance()->OnCoolDown();
		}

		Set_Timer(L"DS_SKILL_FATEDCIRCLE_START");
		Start_Timer(L"DS_SKILL_FATEDCIRCLE_START");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_SKILL_FATEDCIRCLE_START);
		pPlayer->Set_AnimationSpeed(2.3);

		// Effect Start
		
		if (Get_Timer(L"DS_SKILL_FATEDCIRCLE_START") < m_pMeshCom->Get_Period()* 0.16 * 0.41&& Get_Timer(L"DS_SKILL_FATEDCIRCLE_START") > m_pMeshCom->Get_Period()* 0.15* 0.41)
		{
			//if (m_bCreateEffect)
			{
				//m_bCreateEffect = false;
				//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_DS_SKILL_E01, m_pTransform->Get_WorldMatrix(), &(m_pTransform->m_vAngle), *m_tBaseInfo);
			}
		}

		if (Get_Timer(L"DS_SKILL_FATEDCIRCLE_START") < m_pMeshCom->Get_Period()* 0.36* 0.41&& Get_Timer(L"DS_SKILL_FATEDCIRCLE_START") > m_pMeshCom->Get_Period()* 0.35* 0.41)
		{
			//if (!m_bCreateEffect)
			{
				//m_bCreateEffect = true;
//				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_DS_SKILL_E02, m_pTransform->Get_WorldMatrix(), &(m_pTransform->m_vAngle), *m_tBaseInfo);
			}
		}


		if (Get_Timer(L"DS_SKILL_FATEDCIRCLE_START") < m_pMeshCom->Get_Period()* 0.61* 0.41&& Get_Timer(L"DS_SKILL_FATEDCIRCLE_START") > m_pMeshCom->Get_Period()* 0.60* 0.41)
		{
			//if (m_bCreateEffect)
			{
				//m_bCreateEffect = false;
//				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_DS_SKILL_E03, m_pTransform->Get_WorldMatrix(), &(m_pTransform->m_vAngle), *m_tBaseInfo);
			}
		}

		// Effect End
		if (Get_Timer(L"DS_SKILL_FATEDCIRCLE_START") > m_pMeshCom->Get_Period()* 0.41)
		{
			m_bCreateEffect = true;
			Delete_Timer(L"DS_SKILL_FATEDCIRCLE_START");
			pPlayer->Set_State(CPlayer::DS_SKILL_FATEDCIRCLE_END);
			Set_Timer(L"NextState_DS_SKILL_FATEDCIRCLE_END");
		}
	}

	if (CPlayer::DS_SKILL_FATEDCIRCLE_END == pPlayer->Get_CurState())
	{
		if (Engine::KeyDown(DIK_E))
		{
			CUIMgr::GetInstance()->OnCoolDown();
		}

		if (m_bCreateEffect)
		{
			m_bCreateEffect = false;
			//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_DS_SKILL_E04, m_pTransform->Get_WorldMatrix(), &(m_pTransform->m_vAngle), *m_tBaseInfo);
		}
		if (IsReady_Timer(L"NextState_DS_SKILL_FATEDCIRCLE_END"))
		{
			Delete_Timer(L"NextState_DS_SKILL_FATEDCIRCLE_END");
			m_bCreateEffect = true;
		}

		Set_Timer(L"DS_SKILL_FATEDCIRCLE_END");
		Start_Timer(L"DS_SKILL_FATEDCIRCLE_END");
		

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_SKILL_FATEDCIRCLE_END);
		pPlayer->Set_AnimationSpeed(2.3);
		// 무기의 Hide상태가 false(보이는 상태)일때 true(안보이는 상태)로 잠시 바꿔줌
		if (!pPlayer->Get_isDualSword_Hide())
			pPlayer->Set_DualSowrd_Hide(true);
		

		if (Get_Timer(L"DS_SKILL_FATEDCIRCLE_END") > m_pMeshCom->Get_Period() * 0.4)
		{
			Delete_Timer(L"DS_SKILL_FATEDCIRCLE_END");
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_E);
			CUIMgr::GetInstance()->UseableSkillCheck_All(true);
			pPlayer->Set_DualSowrd_Hide(false);
		}
	}
}

void CDS_State::R_Skill(CPlayer * pPlayer, const _double dTimeDelta)
{
	if (CPlayer::DS_SKILL_SCART == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_SKILL_SCART");
		Start_Timer(L"DS_SKILL_SCART");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_SKILL_SCART);
		pPlayer->Set_AnimationSpeed(1.2);

		if (Get_Timer(L"DS_SKILL_SCART") > m_pMeshCom->Get_Period()* 0.7)
		{
			Delete_Timer(L"DS_SKILL_SCART");
		}
	}
}

void CDS_State::F_Skill(CPlayer * pPlayer, const _double dTimeDelta)
{
	if (CPlayer::DS_SKILL_CRITICALX == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_SKILL_CRITICALX");
		Start_Timer(L"DS_SKILL_CRITICALX");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_SKILL_CRITICALX);
		pPlayer->Set_AnimationSpeed(1);

		if (Get_Timer(L"DS_SKILL_CRITICALX") > m_pMeshCom->Get_Period()* 0.7)
			Delete_Timer(L"DS_SKILL_CRITICALX");
		
	}
}

void CDS_State::Q_Skill(CPlayer * pPlayer, const _double dTimeDelta)
{
	if (CPlayer::DS_RAGESKILL_GENOCIDE == pPlayer->Get_CurState())
	{
		Set_Timer(L"DS_RAGESKILL_GENOCIDE");
		Start_Timer(L"DS_RAGESKILL_GENOCIDE");

		m_pMeshCom->Set_AnimationSet(CPlayer::DS_RAGESKILL_GENOCIDE);
		pPlayer->Set_AnimationSpeed(1);

		if (Get_Timer(L"DS_RAGESKILL_GENOCIDE") > m_pMeshCom->Get_Period()* 0.8)
			Delete_Timer(L"DS_RAGESKILL_GENOCIDE");
		
	}
}
