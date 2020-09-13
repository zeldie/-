#include "stdafx.h"
#include "Ai_Cs_State.h"
//#include "Ds_State.h"
//#include "TS_State.h"

CAi_CsState::CAi_CsState()
	: m_fAccelerate(0.f),
	m_fMaxSpeed(0.f),
	m_fJumpMoveSpeed(0.f),
	m_vRecordY(0.f, 0.f, 0.f),
	m_bIsCurve(false),
	m_bIsPeak(false),
	m_bIsStop(false),
	m_bIsRun(false),
	m_bJumpMove(false),
	m_dbJumpTime(0),
	m_dbFall(0),
	m_dbDash(0),
	m_dbCaptureTime(0.0),
	m_dDustTime(0.)
{
}

CAi_CsState::~CAi_CsState()
{

}

void CAi_CsState::Enter(CAi_Player * pPlayer)
{
	return;
}

void CAi_CsState::Update(CAi_Player * pPlayer, const _double dTimeDelta)
{
	return;
}

void CAi_CsState::ChangeState(CAi_PlayerState * pState)
{
}

void CAi_CsState::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float * fCrossy, _vec3 * vDir, _float * fDegree)
{
}

void CAi_CsState::Enter_State(CBasePlayer * pPlayer)
{
	CAi_PlayerState::Enter_State(pPlayer);

	m_fAccelerate = m_pPlayer->Get_Accelerate();
	m_fMaxSpeed = m_pPlayer->Get_MaxSpeed();

	m_dDustTime = g_dAccumulatedTime;
	if (m_pPlayer->Get_Fall())
	{
		m_dbFall = 1000;
		m_bIsPeak = true;
		m_pPlayer->Set_State(CAi_Player::COMMON_AIR_JUMPDOWN);
	}

	if (m_pPlayer->Get_Dash())
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_DASH);
		m_pPlayer->Set_State(CAi_Player::COMMON_DASH);
		m_pPlayer->Set_AnimationSpeed(1.5);
		m_bDash = true;
		m_dbDash = m_pMeshCom->Get_Period() * 0.75;

		m_pPlayer->ReSet_DashReserve();
	}

	else if (m_ePress_Key == KEY_W || m_ePress_Key == KEY_A || m_ePress_Key == KEY_S || m_ePress_Key == KEY_D)
	{
		m_bIsRun = true;		
	}

	m_pPlayer->Set_BattleType(CAi_Player::TYPE_IDLE);

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	CAiMgr::GetInstance()->Set_AttackType(pPlayer, CAiMgr::ATTACKTYPE::ATTACK_END);
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$%%%%
}

void CAi_CsState::Update_State(const _double dTimeDelta)
{
	if (m_bIsAttacked)
		return;

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//m_pAi_Player->Set_StateState(STATETYPE::STATE_COMMON);
	m_pPlayer->Set_isCheckLookDir(false);
	m_pPlayer->Set_isSetLookDir(false);
	m_pPlayer->Set_ActionEnd(false);
	m_bDecalCreateOnce = false;
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


	m_pNaviCom = m_pPlayer->Get_NaviCom();
	m_pNaviCom->Find_Index(m_dwNaviIndex, &m_pPlayer->GetPlayerPos());


	if (false == GraoundAction(m_pPlayer, dTimeDelta))
		return;


	if (false == Finish_Animation(m_pPlayer))
		return;
}

CAi_Player::STATE_ID CAi_CsState::LateUpadte_State(const _double dTimeDelta)
{
	if (m_eDown_Key == KEY_LB)
	{
		m_pPlayer->Set_AnimationSpeed(1);
		m_fSpeed = m_pPlayer->Get_Speed();
		m_pPlayer->Set_JumpTime(m_dbJumpTime);
		m_pPlayer->Set_ReserveAction(Engine::SM_LB);

		if (m_pPlayer->Get_Jump())
		{
			if (m_pPlayer->Get_MainWeapon() == CAi_Player::TWOHANDSWORD && m_pPlayer->Get_AirComboLevel() >= 4)
			{
				return CAi_Player::ID_COMMON;
			}

			else if(m_pPlayer->Get_MainWeapon() == CAi_Player::ORB && m_pPlayer->Get_AirComboLevel() >= 3)
				return CAi_Player::ID_COMMON;

			else if (m_pPlayer->Get_MainWeapon() == CAi_Player::LONGBOW && m_pPlayer->Get_AirComboLevel() >= 3)
				return CAi_Player::ID_COMMON;

			return CAi_Player::ID_AIRCOMBO;
		}

		else
			return CAi_Player::ID_BASICCOMBO;
	}

	if (m_eDown_Key == KEY_RB)
	{
		m_pPlayer->Set_AnimationSpeed(1);
		m_fSpeed = m_pPlayer->Get_Speed();
		m_pPlayer->Set_JumpTime(m_dbJumpTime);
		m_pPlayer->Set_ReserveAction(Engine::SM_RB);

		if (m_pPlayer->Get_Jump() && m_pPlayer->Get_MainWeapon() == CAi_Player::ORB && m_pPlayer->Get_AirComboLevel() >= 3)
		{
			m_pPlayer->Set_AirComboLevel(4);
			return CAi_Player::ID_AIRCOMBO;
		}

		if (m_pPlayer->Get_MainWeapon() == CAi_Player::TWOHANDSWORD && !m_pPlayer->Get_Jump())
		{
			m_pPlayer->Set_AtkLevel(CBasePlayer::LEVEL4);
			return CAi_Player::ID_BASICCOMBOEX;
		}

		if (m_pPlayer->Get_MainWeapon() == CAi_Player::DUALSWORD && !m_pPlayer->Get_Jump())
		{
			m_pPlayer->Set_AtkLevel(CBasePlayer::LEVEL4);
			return CAi_Player::ID_BASICCOMBOEX;
		}

		if (m_pPlayer->Get_MainWeapon() == CAi_Player::LONGBOW && !m_pPlayer->Get_Jump())
		{
			m_pPlayer->Set_AtkLevel(CBasePlayer::LEVEL4);
			return CAi_Player::ID_BASICCOMBOEX;
		}

		if (m_pPlayer->Get_MainWeapon() == CAi_Player::ORB && !m_pPlayer->Get_Jump())
		{
			m_pPlayer->Set_AtkLevel(CBasePlayer::LEVEL2);
			return CAi_Player::ID_BASICCOMBOEX;
		}
	}

	// Skill
	if (m_pPlayer->Get_Jump())
		return CAi_Player::ID_COMMON;

	if (m_eDown_Key == KEY_E)
	{
		//if (0 > m_pPlayer->Get_Mp() - 200)
		//{
		//	int i = 0;
		//	// 마나 부족 ui 출력
		//	return CAi_Player::ID_COMMON;
		//}
		//else if (m_pUIMgr->CheckSkillCoolDowning(Engine::KEYGUIDE_E))
		//{
		//	m_pUIMgr->OnCoolDown();
		//	return CAi_Player::ID_COMMON;
		//}
		//m_pPlayer->Set_Mp(200);
		m_pPlayer->Set_ReserveAction(Engine::SM_E);
		//m_pPlayer->Set_AnimationSpeed(2);
	
		return CAi_Player::ID_SKILL;
	}

	if (m_eDown_Key == KEY_Q)
	{
		//// 분노로 바꿔야 함
		//if (100 > m_pPlayer->Get_Rage())
		//{
		//	int i = 0;
		//	// 분노 부족 ui 출력
		//	return CAi_Player::ID_COMMON;
		//}
		//else if (m_pUIMgr->CheckSkillCoolDowning(Engine::KEYGUIDE_Q))
		//{
		//	m_pUIMgr->OnCoolDown();
		//	return CAi_Player::ID_COMMON;
		//}
		//m_pPlayer->Set_Rage(100);
		m_pPlayer->Set_ReserveAction(Engine::SM_Q);
		m_pPlayer->Set_AnimationSpeed(1);

		return CAi_Player::ID_SKILL;
	}

	if (m_eDown_Key == KEY_R)
	{
		//if (0 > m_pPlayer->Get_Mp() - 200)
		//{
		//	int i = 0;
		//	// 마나 부족 ui 출력
		//	return CAi_Player::ID_COMMON;
		//}
		//else if (m_pUIMgr->CheckSkillCoolDowning(Engine::KEYGUIDE_R))
		//{
		//	m_pUIMgr->OnCoolDown();
		//	return CAi_Player::ID_COMMON;
		//}
		//m_pPlayer->Set_Mp(200);
		m_pPlayer->Set_ReserveAction(Engine::SM_R);
		m_pPlayer->Set_AnimationSpeed(1);

		return CAi_Player::ID_SKILL;
	}

	if (m_eDown_Key == KEY_F)
	{
		//if (0 > m_pPlayer->Get_Mp() - 200)
		//{
		//	int i = 0;
		//	// 마나 부족 ui 출력
		//	return CAi_Player::ID_COMMON;
		//}
		//else if (m_pUIMgr->CheckSkillCoolDowning(Engine::KEYGUIDE_F))
		//{
		//	m_pUIMgr->OnCoolDown();
		//	return CAi_Player::ID_COMMON;
		//}
		//m_pPlayer->Set_Mp(200);
		m_pPlayer->Set_ReserveAction(Engine::SM_F);
		m_pPlayer->Set_AnimationSpeed(1);

		return CAi_Player::ID_SKILL;
	}
	
	return CAi_Player::ID_COMMON;
}

void CAi_CsState::JumpAction(const _double dTimeDelta)
{
	m_dbJumpTime += dTimeDelta;
	_vec3 *vUp = m_pTransform->Get_Info(Engine::INFO_UP);
	_vec3 vPos = m_pPlayer->GetPlayerPos();
	_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
	_matrix matView = m_pPlayer->Get_ViewMatrix();
	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
	if (m_eDown_Key == KEY_SHIFT && !m_bDash)
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_DASH);
		m_pPlayer->Set_State(CAi_Player::COMMON_DASH);
		m_pPlayer->Set_AnimationSpeed(1.5);
		m_bDash = true;
		m_dbDash = m_pMeshCom->Get_Period() * 0.75;
	}

	if (m_bDash)
	{
		if (m_bIsRun)
			m_bIsRun = false;
		m_dbDash -= dTimeDelta * 2.5;
		if (0 > m_dbDash)
			m_dbDash = 0.f;
		////cout << m_dbDash << endl;
		if (m_pMeshCom->Is_AnimationSetFinish(m_pMeshCom->Get_Period()*0.5))
		{
			m_bDash = false;
			//if (Engine::KeyPressing(DIK_W) || Engine::KeyPressing(DIK_A) || Engine::KeyPressing(DIK_S) || Engine::KeyPressing(DIK_D))
			//{
			//	m_bIsRun = true;
			//}
			//else
			{
				//if (m_pPlayer->Get_Tired())
				//{
				//	m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_TIRED);
				//	m_bIsReserve = false;
				//}
				//else
				{
					m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_AIR_JUMPDOWN);
					m_pPlayer->Set_State(CAi_Player::COMMON_AIR_JUMPDOWN);
				}
			}
		}
		if (!m_pMeshCom->Is_AnimationSetFinish(m_pMeshCom->Get_Period()*0.5))
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 1500.f * (_float)(m_dbDash*dTimeDelta)), m_dwNaviIndex, true)));
	}

	if (m_bIsRun)
	{
		m_bJumpMove = true;
		m_bIsRun = false;
	}

	if (m_eDown_Key == KEY_W || m_eDown_Key == KEY_A || m_eDown_Key == KEY_S || m_eDown_Key == KEY_D)
		m_bIsJumpMove = true;

	if (m_bIsJumpMove && !m_pNaviCom->Check_Landing(&m_pPlayer->GetPlayerPos(),m_pPlayer->Get_NaviIndex()))
	{
		_matrix matView = m_pPlayer->Get_ViewMatrix();
		_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
		_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
		_vec3 vPos = m_pPlayer->GetPlayerPos();

		D3DXMatrixInverse(&matView, 0, &matView);
		D3DXVec3Normalize(&vLook, &vLook);

		if (m_ePress_Key == KEY_W)
		{
			_vec3 vMove = { matView.m[2][0], 0.f,matView.m[2][2] };
			vPurpose += vMove;
		}
		if (m_ePress_Key == KEY_A)
		{
			_vec3 vMove = { -matView.m[0][0], 0.f,-matView.m[0][2] };
			vPurpose += vMove;
		}
		if (m_ePress_Key == KEY_S)
		{
			_vec3 vMove = { -matView.m[2][0], 0.f,-matView.m[2][2] };
			vPurpose += vMove;
		}
		if (m_ePress_Key == KEY_D)
		{
			_vec3 vMove = { matView.m[0][0], 0.f,matView.m[0][2] };
			vPurpose += vMove;
		}

		_float fCrossy, fDegree;

		CAi_PlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

		if (!isnan(fDegree))
		{
			if (fCrossy > 0.f)
				m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
			else
				m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
		}
		m_pNaviCom->Find_Index(m_dwNaviIndex, &m_pPlayer->GetPlayerPos());
		m_pPlayer->Set_Index(m_dwNaviIndex);
		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * m_fJumpMoveSpeed * (_float)dTimeDelta), m_dwNaviIndex, true)));

		if (0.f <= m_fJumpMoveSpeed)
			m_fJumpMoveSpeed -= m_fAccelerate*(_float)dTimeDelta*2.f;

		if (!m_ePress_Key == KEY_W && !m_ePress_Key == KEY_A && !m_ePress_Key == KEY_S && !m_ePress_Key == KEY_D)
		{
			m_bIsJumpMove = false;
		}
	}

	vPos = m_pPlayer->GetPlayerPos();

	if (!m_bIsStop && !m_bDash)
	{
		if (!m_bIsPeak)
		{
			CAi_PlayerState::Calculate_FaillingSpeed(1000.f, m_fAccelerate, 25.f, m_dbJumpTime, m_dbFall, &m_fSpeed);
			m_pNaviCom->Find_Index(m_dwNaviIndex, &m_pPlayer->GetPlayerPos());
			m_pPlayer->Set_Index(m_dwNaviIndex);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(*vUp * m_fSpeed*(_float)dTimeDelta*1.5), m_dwNaviIndex, true)));
			
			//m_pNaviCom->Find_Index(m_dwNaviIndex, &m_pPlayer->GetPlayerPos());
		}
		else
		{
			CAi_PlayerState::Calculate_FaillingSpeed(0.f, m_fAccelerate, 15.f, m_dbJumpTime, m_dbFall, &m_fSpeed);
			m_pNaviCom->Find_Index(m_dwNaviIndex, &m_pPlayer->GetPlayerPos());
			m_pPlayer->Set_Index(m_dwNaviIndex);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(*vUp * m_fSpeed*(_float)dTimeDelta*2.5), m_dwNaviIndex, true)));
		}
		////cout << m_fSpeed << endl;
	}

	if (CAi_Player::COMMON_AIR_JUMPUP == m_pPlayer->Get_CurState())
	{
		m_bIsReserve = true;

		m_eReserveState = CAi_Player::COMMON_AIR_JUMPUP;

		if (m_fSpeed < 500.f)
			m_eReserveState = CAi_Player::COMMON_AIR_JUMP;
		else
			m_eReserveState = CAi_Player::COMMON_AIR_JUMPUP;
	}

	else if (CAi_Player::COMMON_AIR_JUMP == m_pPlayer->Get_CurState())
	{
		m_bIsReserve = true;

		if (!m_bIsPeak)
		{
			if (m_fSpeed <= 0.f)
			{
				m_bIsPeak = true;
				m_dbJumpTime = 0;
			}

			m_eReserveState = CAi_Player::COMMON_AIR_JUMP;
		}
		else
		{
			if (-250.f >= m_fSpeed)
			{
				m_eReserveState = CAi_Player::COMMON_AIR_JUMPDOWN;
			}
			else
				m_eReserveState = CAi_Player::COMMON_AIR_JUMP;
		}
	}

	else if (CAi_Player::COMMON_AIR_JUMPLANDING == m_pPlayer->Get_CurState())
	{
		m_bIsReserve = true;
		_double test = m_pMeshCom->Get_Period();
		////cout << test << endl;
		if (m_pMeshCom->Is_AnimationSetFinish(0.65) && (m_ePress_Key == KEY_W || m_ePress_Key == KEY_A || m_ePress_Key == KEY_S || m_ePress_Key == KEY_D))
		{
			m_bIsRun = true;
			m_dbFall = 0;
			m_bIsPeak = false;
			m_eReserveState = CAi_Player::COMMON_RUN;
			m_dbJumpTime = 0;
			m_fSpeed = m_pPlayer->Get_Speed();
			m_pPlayer->Set_Jump_Finish();
			m_pPlayer->Set_AirComboLevel(0);
			m_pPlayer->Set_Fall_End();
		}

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			//m_bIsReserve = false;
			m_dbFall = 0;
			m_bIsPeak = false;
			m_eReserveState = CAi_Player::COMMON_COMBATWAIT;
			m_dbJumpTime = 0;
			m_fSpeed = m_pPlayer->Get_Speed();
			m_pPlayer->Set_Jump_Finish();
  			m_pPlayer->Set_AirComboLevel(0);
			m_pPlayer->Set_Fall_End();
		}
	}

	else if (CAi_Player::COMMON_AIR_JUMPDOWN == m_pPlayer->Get_CurState())
	{
		m_pNaviCom->Find_Index(m_dwNaviIndex, &m_pPlayer->GetPlayerPos());
		m_pPlayer->Set_Index(m_dwNaviIndex);
		if (m_pNaviCom->Check_Landing(&m_pPlayer->GetPlayerPos(),m_dwNaviIndex))
		{
			m_bIsReserve = true;
			_vec3 vPlayerPos = *m_pTransform->Get_Info(Engine::INFO_POS);
			m_pNaviCom->Find_PosY(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex, vPos);
			m_pTransform->Set_Pos(&vPos);

			m_eReserveState = CAi_Player::COMMON_AIR_JUMPLANDING;
			m_fSpeed = m_pPlayer->Get_Speed();
			m_dbJumpTime = 0;
			m_bIsStop = true;
		}
		else if (!m_pNaviCom->Check_Landing(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex))
		{
			m_bIsReserve = true;
			m_eReserveState = CAi_Player::COMMON_AIR_JUMPDOWN;
		}
	}

	else
	{
		m_bIsReserve = true;
		m_eReserveState = CAi_Player::COMMON_AIR_JUMP;
	}

}

_bool CAi_CsState::GraoundAction(CAi_Player * pPlayer, const _double dTimeDelta)
{
	_vec3 vPos = pPlayer->GetPlayerPos();
	_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
	_matrix matView = pPlayer->Get_ViewMatrix();
	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);

	//if (m_pPlayer->Get_CurState() == CAi_Player::COMMON_CAPTUREFLAG_LOOP)
	//{
	//	// 점령 시간 증가
	//	m_dbCaptureTime += dTimeDelta;

	//	if (5 <= m_dbCaptureTime)
	//	{
	//		m_pPlayer->Set_State(CAi_Player::COMMON_CAPTUREFLAG_END);
	//		m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_CAPTUREFLAG_END);
	//		m_dbCaptureTime = 0.0;
	//	}
	//	if (m_pMeshCom->Is_AnimationSetFinish(0.2))
	//	{
	//		m_bIsReserve = true;
	//		m_eReserveState = CAi_Player::COMMON_CAPTUREFLAG_LOOP;
	//	}
	//}

	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3Normalize(&vLook, &vLook);

	if (m_eDown_Key == KEY_Capture_Flag && !m_bDash)
	{
		if (pPlayer->GetDist_ToFlag() < 180)
		{
			if (!(pPlayer->Get_CurState() == CAi_Player::COMMON_CAPTUREFLAG_START
				|| pPlayer->Get_CurState() == CAi_Player::COMMON_CAPTUREFLAG_LOOP
				|| pPlayer->Get_CurState() == CAi_Player::COMMON_CAPTUREFLAG_END))
			{
				_vec3 vLook = *pPlayer->Get_TransformCom()->Get_Info(Engine::INFO_LOOK);
				_vec3 vFlag = pPlayer->Get_FlagPos() - pPlayer->GetPlayerPos();
				float fAngle = Engine::GetAngleY(vLook, vFlag);
				if (!isnan(fAngle))
					pPlayer->Get_TransformCom()->Rotation(Engine::ROTATION::ROT_Y, fAngle);

				pPlayer->Set_State(CAi_Player::COMMON_CAPTUREFLAG_START);
				m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_CAPTUREFLAG_START);
				m_bIsReserve = true;
				m_eReserveState = CAi_Player::COMMON_CAPTUREFLAG_LOOP;
				m_bIsRun = false;
			}
		}
	}

	if (pPlayer->Get_CurState() == CAi_Player::COMMON_CAPTUREFLAG_LOOP)
	{
		if (pPlayer->Get_CaptureFlag() == true)
		{
			pPlayer->Set_State(CAi_Player::COMMON_CAPTUREFLAG_END);
			m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_CAPTUREFLAG_END);
		}
	}


	INPUT_KEY A = m_eDown_Key;
	INPUT_KEY B = m_ePress_Key;
	INPUT_KEY C = m_eUp_Key;


	if (m_eDown_Key == KEY_SHIFT && !m_bDash)
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_DASH);
		pPlayer->Set_State(CAi_Player::COMMON_DASH);
		pPlayer->Set_AnimationSpeed(1.5);
		m_bDash = true;
		m_dbDash = m_pMeshCom->Get_Period() * 0.75;
	}

	if (m_bDash)
	{
		if (m_bIsRun)
			m_bIsRun = false;
		m_dbDash -= dTimeDelta*2.5;
		if (0 > m_dbDash)
			m_dbDash = 0.f;
		////cout << m_dbDash << endl;
		if (m_pMeshCom->Is_AnimationSetFinish(m_pMeshCom->Get_Period()*0.5))
		{
			m_bDash = false;
			
			if (pPlayer->Get_Tired())
			{
				//m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_TIRED);
				m_bIsReserve = false;
				m_bIsRun = false;
			}

			if (m_ePress_Key == KEY_W || m_ePress_Key == KEY_A || m_ePress_Key == KEY_S || m_ePress_Key == KEY_D)
			{     
				m_bIsRun = true;
			}

			else
			{
				m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_COMBATWAIT);
				pPlayer->Set_State(CAi_Player::COMMON_COMBATWAIT);				
			}
		}
		if (!m_pMeshCom->Is_AnimationSetFinish(m_pMeshCom->Get_Period()*0.5))
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 1000.f * (_float)(m_dbDash*dTimeDelta)), m_dwNaviIndex)));
	}

	if (m_eDown_Key == KEY_SPACE)
	{
		if (pPlayer->Get_Tired())
		{
			m_bIsReserve = false;
			return false;
		}
		
		m_fJumpMoveSpeed = m_fSpeed;
		pPlayer->Set_Jump_Start();
		m_fSpeed = pPlayer->Get_Speed();
		m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_AIR_JUMPUP);
		pPlayer->Set_State(CAi_Player::COMMON_AIR_JUMPUP);
		m_pNaviCom->Find_PosY(&vPos, m_dwNaviIndex, m_vRecordY);
		m_dbJumpTime = 0;
		m_bIsStop = false;
		m_bDash = false;
		return false;
	}

	if (m_eDown_Key == KEY_W || m_eDown_Key == KEY_A || m_eDown_Key == KEY_S)
	{
		m_bIsRun = true;
		m_pPlayer->Set_MoveKey(true);
	}
	
	if (m_bIsRun && !m_bDash)
	{
		 //====== 달릴때 먼지 ======
		if (m_dDustTime <= g_dAccumulatedTime)
		{
			m_dDustTime = g_dAccumulatedTime + 0.2f;
			m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_RUN_DUST, &(m_pTransform->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 12.f, 0.f)));
		}
		// =========================

		if (!(m_ePress_Key == KEY_W) && !(m_ePress_Key == KEY_A) && !(m_ePress_Key == KEY_S))
		{
			m_bIsRun = false;

			//if (pPlayer->Get_Tired())
			//{
			//	pPlayer->Set_AnimationSpeed(1);
			//	//m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_TIRED);
			//	//pPlayer->Set_State(CAi_Player::COMMON_TIRED);
			//	m_bIsReserve = false;
			//	return false;
			//}
			if (450.f <= m_fSpeed)
			{
				pPlayer->Set_AnimationSpeed(1);
				m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RUNEND_FAST);
				pPlayer->Set_State(CAi_Player::COMMON_RUNEND_FAST);
			}
			else
			{
				pPlayer->Set_AnimationSpeed(1);
				m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RUNEND);
				pPlayer->Set_State(CAi_Player::COMMON_RUNEND);
				m_fSpeed = pPlayer->Get_Speed();
			}
			m_eReserveState = CAi_Player::COMMON_COMBATWAIT;
			return false;
		}

		m_bIsReserve = true;
		_float fCrossy = 0.f, fDegree = 0.f;

		if (m_ePress_Key == KEY_W)
		{
			if (pPlayer->Get_TargetPlayer() == nullptr)
				return false;

			if (m_iTestW == 0)
			{
				m_vTestPosW[m_iTestW] = pPlayer->GetPlayerPos();
				m_iTestW++;
			}
			else if (m_iTestW == 1)
			{
				m_vTestPosW[m_iTestW] = pPlayer->GetPlayerPos();
				m_iTestW--;
			}

			if (m_vTestPosW[0] == m_vTestPosW[1])
			{
				CONTROLTYPE eType = pPlayer->Get_ControlType();
				CBasePlayer* pTarget = pPlayer->Get_TargetPlayer();
				STATETYPE eStateType = pPlayer->Get_StateState();
				CBasePlayer::STATE_ID eID = pPlayer->Get_CurStateID();
				CBasePlayer::PLAYER_STATE ePlayerState = pPlayer->Get_CurState();
				
				_vec3 vTempDir = Engine::GetDirNoY(pPlayer->Get_TargetPos(), pPlayer->GetPlayerPos());
				pPlayer->Get_TransformCom()->Move_Pos(&vTempDir, m_fSpeed, dTimeDelta);
				
				cout << "%%% W MovePos발동 %%%" << endl;
				int i = 0;
			}

			_vec3 vTargetDir = Engine::GetDirNoY(pPlayer->Get_TargetPos(), pPlayer->GetPlayerPos());
			_vec3 vMove = { vTargetDir.x, 0.f, vTargetDir.z };
			vPurpose += vMove;

			CAi_PlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);
		}
		if (m_ePress_Key == KEY_A)
		{
			if (m_iTestA == 0)
			{
				m_vTestPosA[m_iTestA] = pPlayer->GetPlayerPos();
				m_iTestA++;
			}
			else if (m_iTestA == 1)
			{
				m_vTestPosA[m_iTestA] = pPlayer->GetPlayerPos();
				m_iTestA--;
			}

			if (m_vTestPosA[0] == m_vTestPosA[1])
			{
				CONTROLTYPE eType = pPlayer->Get_ControlType();
				CBasePlayer* pTarget = pPlayer->Get_TargetPlayer();
				STATETYPE eStateType = pPlayer->Get_StateState();
				CBasePlayer::STATE_ID eID = pPlayer->Get_CurStateID();
				CBasePlayer::PLAYER_STATE ePlayerState = pPlayer->Get_CurState();

				_vec3 vTempDir = Engine::GetDirNoY(pPlayer->Get_FlagPos(), pPlayer->GetPlayerPos());
				pPlayer->Get_TransformCom()->Move_Pos(&vTempDir, m_fSpeed, dTimeDelta);

				cout << "%%% A MovePos발동 %%%" << endl;
				int i = 0;
			}

			_vec3 vFlagDir = Engine::GetDirNoY(pPlayer->Get_FlagPos(), pPlayer->GetPlayerPos());
			D3DXVec3Normalize(&vFlagDir, &vFlagDir);

			_vec3 vMove = { vFlagDir.x, 0.f, vFlagDir.z };
			vPurpose += vMove;

			CAi_PlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);
		}
		if (m_ePress_Key == KEY_S)
		{
			if (pPlayer->Get_TargetPlayer() == nullptr)
				return false;

			CBasePlayer* pObj = pPlayer->Get_TargetPlayer();
			_vec3 vTargetDir = Engine::GetDirNoY(pPlayer->Get_TargetPos(), pPlayer->GetPlayerPos());

			_vec3 vMove = { -vTargetDir.x, 0.f, -vTargetDir.z };
			vPurpose += vMove;
			CAi_PlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);
		}
		if (m_ePress_Key == KEY_D)
		{
			_vec3 vMove = { matView.m[0][0], 0.f,matView.m[0][2] };
			vPurpose += vMove;
			CAi_PlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);
		}


		if (!isnan(fDegree))
		{
			if (fCrossy > 0.f)
				m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 10.f);
			else
				m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 10.f);
		}
		else
			m_pTransform->Rotation(Engine::ROT_Y, (_float)dTimeDelta);


		if (m_fMaxSpeed > m_fSpeed)
			m_fSpeed += m_fAccelerate * (_float)dTimeDelta*2.f;
		else
			m_fSpeed = m_fMaxSpeed;

		//if (450.f <= m_fSpeed)
		//{
		//	//m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RUN_FAST);
		//	//pPlayer->Set_State(CAi_Player::COMMON_RUN_FAST);
		//}		


		if (250.f <= m_fSpeed && 450.f > m_fSpeed)
		{
			m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RUN);
			pPlayer->Set_State(CAi_Player::COMMON_RUN);
			m_eReserveState = CAi_Player::COMMON_RUN;
		}
		else if (450.f <= m_fSpeed)
		{
			m_pPlayer->Set_AnimationSpeed(2);
			//m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RUN_FAST);
			//pPlayer->Set_State(CAi_Player::COMMON_RUN_FAST);
			//m_eReserveState = CAi_Player::COMMON_RUN_FAST;
		}

		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * m_fSpeed * (_float)dTimeDelta), m_dwNaviIndex)));
		//m_pTransform->Move_Pos(&vDir, m_fSpeed, dTimeDelta);
	}
	if (pPlayer->Get_CurState() == CAi_Player::COMMON_RUNEND_FAST)
	{
		vDir = vLook;
		D3DXVec3Normalize(&vDir, &vDir);
		pPlayer->Set_AnimationSpeed(1);
		m_fSpeed -= m_fAccelerate * (_float)dTimeDelta * 7;
		if (m_fSpeed < 0.f)
			m_fSpeed = 0.f;

		m_pNaviCom->Find_Index(m_dwNaviIndex, &vPos);
		pPlayer->Set_Index(m_dwNaviIndex);
		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * m_fSpeed * (_float)dTimeDelta), m_dwNaviIndex)));
		//m_pTransform->Move_Pos(&vDir, m_fSpeed, dTimeDelta);


		if (m_pMeshCom->Is_AnimationSetFinish(0.3))
			m_fSpeed = pPlayer->Get_Speed();
	}
	return true;
}

_bool CAi_CsState::Finish_Animation(CAi_Player* pPlayer)
{
	if (m_bIsReserve && m_pMeshCom->Is_AnimationSetFinish(0.2))
	{
		m_pMeshCom->Set_AnimationSet(m_eReserveState);
		pPlayer->Set_State(m_eReserveState);
		m_bIsReserve = false;
		return false;
	}

	if (!m_bIsReserve && !pPlayer->Get_Jump() && m_pMeshCom->Is_AnimationSetFinish(0.2))
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_COMBATWAIT);
		pPlayer->Set_State(CAi_Player::COMMON_COMBATWAIT);
		m_pPlayer->Set_AnimationSpeed(1);
		m_fSpeed = pPlayer->Get_Speed();
		return false;
	}

	if (m_pMeshCom->Is_AnimationSetFinish(0.2))
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_COMBATWAIT);
		pPlayer->Set_State(CAi_Player::COMMON_COMBATWAIT);
		m_pPlayer->Set_AnimationSpeed(1);
		pPlayer->Set_AirComboLevel(0);
		m_fSpeed = pPlayer->Get_Speed();
		return false;
	}
	return true;
}
