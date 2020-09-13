#include "stdafx.h"
#include "TS_State.h"

CTsState::CTsState()
	:m_bSecondRocBreak(false),
	m_bIsMinimumTime(false),
	m_dbMotionCancel(0),
	m_dbBasicLoopTime(0),
	m_dbIsChargingTime(0),
	m_dbWaitingStartMotion(0),
	m_dbPressingTime(0)
{

}

CTsState::~CTsState()
{

}

void CTsState::Enter(CPlayer * pPlayer)
{
	m_pMeshCom = pPlayer->Get_PlayerMesh();
	m_pTransform = pPlayer->Get_TransformCom();
	m_fSpeed = pPlayer->Get_Speed();
	m_dbJumpTime = 0;
	pPlayer->Set_BattleType(CPlayer::TYPE_BATTLE);
	
}

void CTsState::Update(CPlayer * pPlayer, const _double dTimeDelta)
{
	//_vec3 vPos = 

	// Branch_StartMotion
	if (m_bIsAttacked)
		return;
	m_pNaviCom = pPlayer->Get_NaviCom();

	switch (m_eStartMotion)
	{
	case Engine::SM_LB:
	{
		Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"TS", Engine::MESH, Engine::ID_STATIC));
		pWeaponCom->Set_AnimationSet(0);
		if (pPlayer->Get_Jump())
		{
			switch (pPlayer->Get_AirComboLevel())
			{
			case 0:
			{
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_AIR_COMBO_01);
				pPlayer->Set_State(CPlayer::TS_AIR_COMBO_01);
				pPlayer->Set_AirComboLevel(1);
				m_eStartMotion = Engine::SM_END;
			}
			break;
			case 1:
			{
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_AIR_COMBO_02);
				pPlayer->Set_State(CPlayer::TS_AIR_COMBO_02);
				pPlayer->Set_AirComboLevel(2);
				m_eStartMotion = Engine::SM_END;
			}
			break;
			case 2:
			{
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_AIR_COMBO_03);
				pPlayer->Set_State(CPlayer::TS_AIR_COMBO_03);
				pPlayer->Set_AirComboLevel(3);
				m_eStartMotion = Engine::SM_END;
			}
			break;
			case 3:
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_AIR_COMBO_04);
				pPlayer->Set_State(CPlayer::TS_AIR_COMBO_04);
				pPlayer->Set_AnimationSpeed(0.5);
				pPlayer->Set_AirComboLevel(4);
				m_eStartMotion = Engine::SM_END;
				break;
			default:
				break;
			}

		}
		else
		{
			m_pMeshCom->Set_AnimationSet(CPlayer::TS_BASICCOMBO_01);
			pPlayer->Set_State(CPlayer::TS_BASICCOMBO_01);
			m_eStartMotion = Engine::SM_END;
		}
		break;
	}
	case Engine::SM_E:
	{
		Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"TS", Engine::MESH, Engine::ID_STATIC));
		pWeaponCom->Set_AnimationSet(0);
		pPlayer->Set_State(CPlayer::TS_OUTRAGE_START);
		m_pMeshCom->Set_AnimationSet(CPlayer::TS_OUTRAGE_START);
		m_eStartMotion = Engine::SM_END;
		break;
	}
	case Engine::SM_Q:
	{
		Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"TS", Engine::MESH, Engine::ID_STATIC));
		pWeaponCom->Set_AnimationSet(0);
		pPlayer->Set_State(CPlayer::TS_RAGESKILL_ARMAGEDDONBLADE);
		m_pMeshCom->Set_AnimationSet(CPlayer::TS_RAGESKILL_ARMAGEDDONBLADE);
		m_eStartMotion = Engine::SM_END;
		break;
	}

	case Engine::SM_F:
	{
		Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"TS", Engine::MESH, Engine::ID_STATIC));
		pWeaponCom->Set_AnimationSet(0);
		pPlayer->Set_State(CPlayer::TS_ROCKSHOT);
		m_pMeshCom->Set_AnimationSet(CPlayer::TS_ROCKSHOT);
		m_eStartMotion = Engine::SM_END;
		break;
	}


	case Engine::SM_R:
	{
		Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"TS", Engine::MESH, Engine::ID_STATIC));
		pWeaponCom->Set_AnimationSet(0);
		pPlayer->Set_State(CPlayer::TS_LEAP_ATTACK_READY);
		m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_READY);
		m_eStartMotion = Engine::SM_END;
		break;
	}

	case Engine::SM_RB:
	{
		m_dbWaitingStartMotion += dTimeDelta;
		if (m_dbWaitingStartMotion >= 0.5)
		{
			Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"TS", Engine::MESH, Engine::ID_STATIC));

			if (Engine::MousePressing(Engine::DIM_RB))
			{
				pWeaponCom->Set_AnimationSet(0);
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_SPECIALCOMBO_READY);
				pPlayer->Set_State(CPlayer::TS_SPECIALCOMBO_READY);
				m_eStartMotion = Engine::SM_END;

			}
			else //if (Engine::MouseUp(Engine::DIM_RB))
			{
				pWeaponCom->Set_AnimationSet(0);
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_ROCKBREAK);
				pPlayer->Set_State(CPlayer::TS_ROCKBREAK);
				m_bSecondRocBreak = false;
				m_eStartMotion = Engine::SM_END;
			}


		}
		return;
	}
	break;

	case Engine::SM_END:
		break;
	default:
		break;
	}
	if (pPlayer->Get_Jump())
	{
		m_pNaviCom->Find_Index(m_dwNaviIndex, &pPlayer->GetPlayerPos());

		if (CPlayer::TS_AIR_COMBO_01 == pPlayer->Get_CurState())
		{
			if (Engine::MouseDown(Engine::DIM_LB))
			{
				m_bIsReserve = true;
				m_eReserveState = CPlayer::TS_AIR_COMBO_02;
				m_dbAccelTime = 0;
				pPlayer->Set_AirComboLevel(1);
			}
		}

		if (CPlayer::TS_AIR_COMBO_02 == pPlayer->Get_CurState())
		{

			if (Engine::MouseDown(Engine::DIM_LB))
			{
				m_bIsReserve = true;
				m_eReserveState = CPlayer::TS_AIR_COMBO_03;
				m_dbAccelTime = 0;
				pPlayer->Set_AirComboLevel(2);
			}
		}

		if (CPlayer::TS_AIR_COMBO_03 == pPlayer->Get_CurState())
		{
			if (Engine::MouseDown(Engine::DIM_LB))
			{
				//pPlayer->Set_AnimationSpeed(0.5);
				m_bIsReserve = true;
				m_eReserveState = CPlayer::TS_AIR_COMBO_04;
				m_dbAccelTime = 0;
				pPlayer->Set_AirComboLevel(3);
			}
		}

		if (CPlayer::TS_AIR_COMBO_04_LANDING == pPlayer->Get_CurState())
		{
			_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
			_vec3 vUp;
			m_pTransform->Get_Info(Engine::INFO_UP, &vUp);
			_ulong m_dwNaviIndex = pPlayer->Get_NaviIndex();

			m_dbJumpTime += dTimeDelta + pPlayer->Get_JumpTime();
			_float fSpeed;
			CPlayerState::Calculate_FaillingSpeed(0.f, pPlayer->Get_Accelerate(), 15.f, m_dbJumpTime, 500, &fSpeed);

			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vUp * fSpeed*(_float)dTimeDelta*0.05f), m_dwNaviIndex, true)));

			m_pNaviCom->Find_Index(m_dwNaviIndex, &pPlayer->GetPlayerPos());
			pPlayer->Set_Index(m_dwNaviIndex);
			if (m_pNaviCom->Check_Landing(&pPlayer->GetPlayerPos(), m_dwNaviIndex))
			{
				m_pNaviCom->Find_PosY(&pPlayer->GetPlayerPos(), m_dwNaviIndex, vPos);
				m_pTransform->Set_Pos(&vPos);

				m_bIsReserve = false;
				//pPlayer->Set_AnimationSpeed(0.5);
				//pPlayer->Set_State(CPlayer::TS_AIR_COMBO_04_LANDING);
				//m_pMeshCom->Set_AnimationSet(CPlayer::TS_AIR_COMBO_04_LANDING);
			}


			//pPlayer->Set_AnimationSpeed(1);
			//m_bIsReserve = false;
			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				pPlayer->Set_AirComboLevel(0);
				pPlayer->Set_AnimationSpeed(1);
				pPlayer->Set_Jump_Finish();
				pPlayer->Set_Fall_End();
				pPlayer->ExChange_State(new CCsState);
				return;
			}
		}

		if (CPlayer::TS_AIR_COMBO_04 == pPlayer->Get_CurState())
		{
			_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
			_vec3 vUp;
			m_pTransform->Get_Info(Engine::INFO_UP, &vUp);
			_ulong m_dwNaviIndex = pPlayer->Get_NaviIndex();

			m_dbJumpTime += dTimeDelta + pPlayer->Get_JumpTime();
			_float fSpeed;
			CPlayerState::Calculate_FaillingSpeed(0.f, pPlayer->Get_Accelerate(), 15.f, m_dbJumpTime, 500, &fSpeed);
			pPlayer->Set_AnimationSpeed(0.5);
			if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.5)
			{
				pPlayer->Set_AnimationSpeed(0.5);
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vUp * fSpeed*(_float)dTimeDelta*0.05f), m_dwNaviIndex, true)));
			}

			else
			{
				pPlayer->Set_AnimationSpeed(0.5);
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vUp * fSpeed*(_float)dTimeDelta*0.5f), m_dwNaviIndex, true)));
				pPlayer->Set_State(CPlayer::TS_AIR_COMBO_04_LANDING);
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_AIR_COMBO_04_LANDING);
			}

			////cout << fSpeed*(_float)dTimeDelta*0.05f << endl;
			m_pNaviCom->Find_Index(m_dwNaviIndex, &pPlayer->GetPlayerPos());
			pPlayer->Set_Index(m_dwNaviIndex);
			if (m_pNaviCom->Check_Landing(&pPlayer->GetPlayerPos(), m_dwNaviIndex))
			{
				m_pNaviCom->Find_PosY(&pPlayer->GetPlayerPos(), m_dwNaviIndex, vPos);
				m_pTransform->Set_Pos(&vPos);

				m_bIsReserve = false;
				//pPlayer->Set_AnimationSpeed(0.5);
				//pPlayer->Set_State(CPlayer::TS_AIR_COMBO_04_LANDING);
				//m_pMeshCom->Set_AnimationSet(CPlayer::TS_AIR_COMBO_04_LANDING);
			}
		}


	}
	//if (CPlayer::TS_OUTRAGE_END == pPlayer->Get_CurState() && m_pMeshCom->Is_AnimationSetFinish(0.2))
	//{
	//	CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_E);
	//	CUIMgr::GetInstance()->UseableSkillCheck_All(true);
	//	pPlayer->Reset_SuperAmmor();
	//}
	if (CPlayer::TS_OUTRAGE_START == pPlayer->Get_CurState())
	{
		pPlayer->Set_AnimationSpeed(2);

		m_bIsReserve = true;
		m_eReserveState = CPlayer::TS_BASICCOMBO_01;
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			pPlayer->Reset_SuperAmmor();
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_E);
			CUIMgr::GetInstance()->UseableSkillCheck_All(true);
		}

		if (Engine::KeyDown(DIK_E))
		{
			CUIMgr::GetInstance()->OnCoolDown();
		}

	}

	if (CPlayer::TS_LEAP_ATTACK_AIRDOWN == pPlayer->Get_CurState())
	{
		m_bIsReserve = true;
		m_eReserveState = CPlayer::TS_LEAP_ATTACK_LANDING;
	}


	if (CPlayer::TS_LEAP_ATTACK_AIRUP == pPlayer->Get_CurState())
	{
		m_bIsReserve = true;
		m_eReserveState = CPlayer::TS_LEAP_ATTACK_AIRDOWN;
	}

	if (CPlayer::TS_LEAP_ATTACK_AIR == pPlayer->Get_CurState())
	{
		m_bIsReserve = true;
		m_eReserveState = CPlayer::TS_LEAP_ATTACK_AIRUP;
	}

	if (CPlayer::TS_LEAP_ATTACK_START == pPlayer->Get_CurState())
	{
		m_bIsReserve = true;
		m_eReserveState = CPlayer::TS_LEAP_ATTACK_AIR;
	}

	if (CPlayer::TS_LEAP_ATTACK_READY == pPlayer->Get_CurState())
	{
		m_bIsReserve = true;
		m_eReserveState = CPlayer::TS_LEAP_ATTACK_START;
	}


	if (CPlayer::TS_BASICCOMBO_01 == pPlayer->Get_CurState())
	{
		if (Engine::MouseDown(Engine::DIM_LB))
		{
			m_bIsReserve = true;
			m_dbPressingTime = 0;
			m_eReserveState = CPlayer::TS_BASICCOMBO_02;
		}

		if (m_pMeshCom->Get_TrackTime()<m_pMeshCom->Get_Period()*0.3)
		{
			_vec3 vPos = pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
		}
	}

	if (CPlayer::TS_SPECIALCOMBO_READY == pPlayer->Get_CurState())
	{
		m_dbIsChargingTime += dTimeDelta;
		if (Engine::MouseUp(Engine::DIM_RB))
		{
			if (m_dbIsChargingTime >= m_pMeshCom->Get_Period()*0.1)
			{
				// 즉시 교체
				m_eReserveState = CPlayer::TS_SPECIALCOMBO_CRASH;
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_SPECIALCOMBO_CRASH);
			}
		}
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_eReserveState = CPlayer::TS_SPECIALCOMBO_CRASH;
			m_pMeshCom->Set_AnimationSet(CPlayer::TS_SPECIALCOMBO_CRASH);
		}

		//// Charge ATK - normal / fullcharge 이펙트에 따라 구분해야 하므로 추후에 구현
		//if (m_dbIsChargingTime >= m_pMeshCom->Get_Period()*0.5)
		//{
		//	// FullCharge
		//}
	}

	if (CPlayer::TS_BASICCOMBO_02 == pPlayer->Get_CurState())
	{
		if (Engine::MouseUp(Engine::DIM_LB))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::TS_BASICCOMBO_03;
		}

		if (Engine::MousePressing(Engine::DIM_LB))
		{
			m_dbPressingTime += dTimeDelta;
			
			if (0.75 >= m_dbPressingTime)
			{
				m_bIsReserve = true;
				m_dbPressingTime = 0;
				m_eReserveState = CPlayer::TS_BASICCOMBOEX02_START;
			}
		}

		if (Engine::MouseUp(Engine::DIM_RB))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::TS_BASICCOMBOEX01;
		}

		if (m_pMeshCom->Get_TrackTime()<m_pMeshCom->Get_Period()*0.3)
		{
			_vec3 vPos = pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
		}
	}
	if (CPlayer::TS_BASICCOMBO_03 == pPlayer->Get_CurState())
	{
		if (m_pMeshCom->Get_TrackTime()<m_pMeshCom->Get_Period()*0.3)
		{
			_vec3 vPos = pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
		}
	}
	if (CPlayer::TS_BASICCOMBOEX02_LOOP == pPlayer->Get_CurState())
	{
		m_dbBasicLoopTime += dTimeDelta;
		pPlayer->Set_AnimationSpeed(1.25);
		if (!m_bIsMinimumTime)
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::TS_BASICCOMBOEX02_LOOP;

			if (Engine::MousePressing(Engine::DIM_LB))
			{
				m_bIsReserve = true;
				m_eReserveState = CPlayer::TS_BASICCOMBOEX02_LOOP;
				//return;
			}

			if (/*m_dbBasicLoopTime >= 0.25 && */Engine::MouseUp(Engine::DIM_LB))
			{
				m_bIsMinimumTime = true;
			}

			if (m_dbIsBasicCombo2FinishTime <= m_dbBasicLoopTime)// || Engine::MouseUp(Engine::DIM_LB))
			{
				m_bIsReserve = true;
				m_eReserveState = CPlayer::TS_BASICCOMBOEX02_END;
				pPlayer->Set_AnimationSpeed(1);
			}
		}
		if (m_bIsMinimumTime)
		{
			if (2 <= m_dbBasicLoopTime)
			{
				m_bIsReserve = true;
				m_eReserveState = CPlayer::TS_BASICCOMBOEX02_END;
				pPlayer->Set_AnimationSpeed(1);
			}
			else
			{
				m_bIsReserve = true;
				m_eReserveState = CPlayer::TS_BASICCOMBOEX02_LOOP;
			}
		}
	}

	if (CPlayer::TS_BASICCOMBOEX02_START == pPlayer->Get_CurState())
	{
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::TS_BASICCOMBOEX02_LOOP;
			m_pMeshCom->Set_AnimationSet(CPlayer::TS_BASICCOMBOEX02_LOOP);
		}
	}

	if (CPlayer::TS_ROCKBREAK == pPlayer->Get_CurState())
	{
		// motion cancel time 0.25 sec
		if (Engine::MouseDown(Engine::DIM_RB))
		{
			if (!m_bSecondRocBreak)
			{
				m_bIsReserve = true;
				m_bSecondRocBreak = true;
				m_eReserveState = CPlayer::TS_ROCKBREAK;
			}
		}
		if (m_pMeshCom->Get_TrackTime()<m_pMeshCom->Get_Period()*0.3)
		{
			_vec3 vPos = pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
		}
	}

	if (CPlayer::TS_AIR_COMBO_01 == pPlayer->Get_CurState()
		|| CPlayer::TS_AIR_COMBO_02 == pPlayer->Get_CurState()
		|| CPlayer::TS_AIR_COMBO_03 == pPlayer->Get_CurState())
	{
		m_dbAccelTime += dTimeDelta;

		_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
		_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
		_vec3 vUp = *m_pTransform->Get_Info(Engine::INFO_UP);

		_float fSpeed = 500.f;

		m_dwNaviIndex = pPlayer->Get_NaviIndex();


		if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.21)
		{
			m_pMeshCom->Set_AnimationSet(m_eReserveState);
			pPlayer->Set_State(m_eReserveState);
			pPlayer->Set_AnimationSpeed(1);
			m_bIsReserve = false;
			m_dbMotionCancel = 0;
			return;
		}
		else
		{
			m_pTransform->Get_Info(Engine::INFO_LOOK, &vLook);

			if (0 < 1 - m_dbAccelTime)
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * fSpeed*(_float)((1 - m_dbAccelTime)*dTimeDelta)), m_dwNaviIndex, true)));
		}

		//_double JumpTime = pPlayer->Get_JumpTime() + dTimeDelta;
		
		if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.21)
		{
			m_dbJumpTime += dTimeDelta + pPlayer->Get_JumpTime();

			CPlayerState::Calculate_FaillingSpeed(0.f, pPlayer->Get_Accelerate(), 15.f, m_dbJumpTime, 500, &fSpeed);

			if (!m_pNaviCom->Check_Landing(&pPlayer->GetPlayerPos(), m_dwNaviIndex))
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vUp * fSpeed*(_float)dTimeDelta*0.01f), m_dwNaviIndex, true)));

			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				if (pPlayer->Get_Jump())
					pPlayer->Set_Fall_Start();

				pPlayer->ExChange_State(new CCsState);
				return;
			}
		}
	}

	if (CPlayer::TS_AIR_COMBO_04 == pPlayer->Get_CurState())
	{
		if (m_pMeshCom->Is_AnimationSetFinish())
		{
			m_pMeshCom->Set_AnimationSet(m_eReserveState);
			pPlayer->Set_State(m_eReserveState);
			//m_bIsFirstMotionCheck = true;
			m_bIsReserve = false;
			m_dbMotionCancel = 0;
			return;
		}
		else
			return;
	}


	if (m_bIsReserve)
	{
		m_dbMotionCancel += dTimeDelta;
		if (CPlayer::TS_BASICCOMBO_01 == pPlayer->Get_CurState()
			|| CPlayer::TS_BASICCOMBO_02 == pPlayer->Get_CurState())
		{
			if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.21)
			{
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				pPlayer->Set_State(m_eReserveState);
				//m_bIsFirstMotionCheck = true;
				m_bIsReserve = false;
				m_dbMotionCancel = 0;
				return;
			}
		}

		if (CPlayer::TS_ROCKBREAK == pPlayer->Get_CurState())
		{
			if (m_dbMotionCancel >= 0.25)
			{
				// 리셋 용으로 의미 없는 애니메이션 삽입
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_PIERCINGRUSH_END);

				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				pPlayer->Set_State(m_eReserveState);
				m_bIsReserve = false;
				m_dbMotionCancel = 0;
				return;
			}
		}

		if (CPlayer::TS_AIR_COMBO_04 == pPlayer->Get_CurState())
		{
			if (m_pMeshCom->Is_AnimationSetFinish())
			{
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				pPlayer->Set_State(m_eReserveState);
				pPlayer->Set_AnimationSpeed(1);
				m_bIsReserve = false;
				m_dbMotionCancel = 0;
				return;
			}
		}

		else if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_pMeshCom->Set_AnimationSet(m_eReserveState);
			pPlayer->Set_State(m_eReserveState);
			pPlayer->Set_AnimationSpeed(1);
			m_bIsReserve = false;
			m_dbMotionCancel = 0;
			return;
		}

		return;
	}

	if (m_pMeshCom->Is_AnimationSetFinish(0.2))
	{

		if (pPlayer->Get_Jump())
			pPlayer->Set_Fall_Start();

		pPlayer->Set_AnimationSpeed(1);

		pPlayer->ExChange_State(new CCsState);
	}
}

void CTsState::ChangeState(CPlayerState * pState)
{
}