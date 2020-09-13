#include "stdafx.h"
#include "CS_State.h"
#include "Ds_State.h"
#include "TS_State.h"
#include "Renderer.h"
CCsState::CCsState()
	: m_fAccelerate(0.f),
	m_fMaxSpeed(0.f),
	m_fJumpMoveSpeed(0.f),
	m_vRecordY(0.f, 0.f, 0.f),
	m_bIsCurve(false),
	m_bIsPeak(false),
	m_bIsStop(false),
	m_bIsRun(false),
	m_bJumpMove(false),
	m_bJumpSound(false),
	m_dbJumpTime(0),
	m_dbFall(0),
	m_dbDash(0),
	m_dbCaptureTime(0.0),
	m_dbFlagCapture(0.0),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_dDustTime(0.)
{
}

CCsState::~CCsState()
{

}

void CCsState::Enter(CPlayer * pPlayer)
{
	return;
}

void CCsState::Update(CPlayer * pPlayer, const _double dTimeDelta)
{
	return;
}

void CCsState::ChangeState(CPlayerState * pState)
{
}

void CCsState::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float * fCrossy, _vec3 * vDir, _float * fDegree)
{
}

void CCsState::Enter_State(CBasePlayer * pPlayer)
{
	CPlayerState::Enter_State(pPlayer);

	m_fAccelerate = m_pPlayer->Get_Accelerate();
	m_fMaxSpeed = m_pPlayer->Get_MaxSpeed();
	
	m_dDustTime = g_dAccumulatedTime;
	if (m_pPlayer->Get_Fall())
	{
		m_dbFall = 1000;
		m_bIsPeak = true;
		m_pPlayer->Set_State(CPlayer::COMMON_AIR_JUMPDOWN);
	}

	if (m_pPlayer->Get_Dash())
	{
		if (m_pUIMgr->CheckSkillCoolDowning(Engine::KEYGUIDE_LSHIFT) && !m_pPlayer->Get_Tired())
		{
			m_pUIMgr->OnCoolDown();
		}
		else
		{
			if (0 < m_pPlayer->Get_Stamina() && !m_pPlayer->Get_Tired())
			{
				m_pPlayer->Set_Stamina(400);
				m_pPlayer->Set_Incibility();
				m_pUIMgr->SkillCoolDownCheck(Engine::KEYGUIDE_LSHIFT);
				m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_DASH);
				m_pPlayer->Set_State(CPlayer::COMMON_DASH);
				CSoundMgr::Get_Instance()->SoundOn(83);
				m_pPlayer->Set_AnimationSpeed(1.5);
				m_bDash = true;
				m_pPlayer->Set_Incibility();
				m_pPlayer->Get_Renderer()->Set_RadialBlur(true);
				m_dbDash = m_pMeshCom->Get_Period() * 0.75;
				m_pPlayer->ReSet_DashReserve();
			}
		}
	}

	else if (Engine::KeyPressing(DIK_W) || Engine::KeyPressing(DIK_A) || Engine::KeyPressing(DIK_S) || Engine::KeyPressing(DIK_D))
	{
		m_bIsRun = true;		
	}

	// 무기 설정 리셋 씨발
	switch (m_pPlayer->Get_MainWeapon())
	{
	case CPlayer::TWOHANDSWORD:
	{
		CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));
		pWeapon->Set_CombatState(CTwohandSword::STATE_WAIT);
		break;
	}
	case CPlayer::DUALSWORD:
	{
		CDualSword* pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_L"));
		pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_WAIT);

		pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_R"));
		pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_WAIT);
		break;
	}
	case CPlayer::LONGBOW:
	{
		CLongBow* pWeapon = dynamic_cast<CLongBow*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"LB"));
		pWeapon->Set_CombatState(CLongBow::COMBAT_STATE::STATE_WAIT);
		

		break;
	}
	default:
		break;
	}
	
	//m_pPlayer->Release_Decal();
	m_pPlayer->Set_BattleType(CPlayer::TYPE_IDLE);
}

void CCsState::Update_State(const _double dTimeDelta)
{
	if (m_bIsAttacked)
		return;

	m_pNaviCom = m_pPlayer->Get_NaviCom();
	m_pNaviCom->Find_Index(m_dwNaviIndex, &m_pPlayer->GetPlayerPos());

	//// %%%%%%%%%%%%%%%%%%% Cartel 시작할때 컷씬 %%%%%%%%%%%%%%%%%%%%%%%%%%%
	//if (m_pPlayer->Get_StateID() == CBasePlayer::PLAYER_STATE::COMMON_WAIT_HABIT)
	//{
	//	if (m_pPlayer->Get_DyanmicMesh()->Get_TrackTime() > m_pPlayer->Get_DyanmicMesh()->Get_Period() * 0.9)
	//		m_pPlayer->Set_CartelBlueStart(true);
	//}
	//// %%%%%%%%%%%%%%%%%%% Cartel 시작할때 컷씬 %%%%%%%%%%%%%%%%%%%%%%%%%%%

	if (!m_pPlayer->Get_Jump()&&m_pPlayer->Get_Tired() && m_pPlayer->Get_CurState() != CPlayer::COMMON_DASH && m_pPlayer->Get_CurState() != CPlayer::COMMON_TIRED_WALK)
	{
		m_pPlayer->Set_AnimationSpeed(0.75);
		m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_TIRED);
		m_pPlayer->Set_State(CPlayer::COMMON_TIRED);
	}

	//if (m_pPlayer->Get_Tired())
	//{
	//	if (Engine::KeyPressing(DIK_W) || Engine::KeyPressing(DIK_A) || Engine::KeyPressing(DIK_S) || Engine::KeyPressing(DIK_D))
	//	{
	//		m_pPlayer->Set_AnimationSpeed(1);
	//		m_pPlayer->Set_State(CPlayer::COMMON_TIRED_WALK);
	//		m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_TIRED_WALK);
	//	}
	//	else
	//	{
	//		m_pPlayer->Set_AnimationSpeed(0.75);
	//		m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_TIRED);
	//		m_pPlayer->Set_State(CPlayer::COMMON_TIRED);
	//	}
	//}

	//if (CPlayer::COMMON_TIRED_WALK == m_pPlayer->Get_CurState() && m_pPlayer->Get_Tired())
	//{
	//	_matrix matView = m_pPlayer->Get_ViewMatrix();
	//	_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
	//	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
	//	_vec3 vPos = m_pPlayer->GetPlayerPos();


	//	if (Engine::KeyPressing(DIK_W))
	//	{
	//		_vec3 vMove = { matView.m[2][0], 0.f,matView.m[2][2] };
	//		vPurpose += vMove;
	//	}
	//	if (Engine::KeyPressing(DIK_A))
	//	{
	//		_vec3 vMove = { -matView.m[0][0], 0.f,-matView.m[0][2] };
	//		vPurpose += vMove;
	//	}
	//	if (Engine::KeyPressing(DIK_S))
	//	{
	//		_vec3 vMove = { -matView.m[2][0], 0.f,-matView.m[2][2] };
	//		vPurpose += vMove;
	//	}
	//	if (Engine::KeyPressing(DIK_D))
	//	{
	//		_vec3 vMove = { matView.m[0][0], 0.f,matView.m[0][2] };
	//		vPurpose += vMove;
	//	}

	//	_float fCrossy, fDegree;

	//	CPlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);
	//	//cout << fDegree << endl;
	//	if (!isnan(fDegree))
	//	{
	//		if (fCrossy > 0.f)
	//			m_pTransform->Rotation(Engine::ROT_Y, -fDegree * (_float)dTimeDelta * 5.f);
	//		else
	//			m_pTransform->Rotation(Engine::ROT_Y, (fDegree)* (_float)dTimeDelta * 5.f);
	//	}
	//	vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
	//	m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 100.f * (_float)(dTimeDelta)), m_dwNaviIndex)));

	//	return;
	//}



	if (m_pPlayer->Get_Jump())
	{
		///////////////////////////////////////////희정
		m_pUIMgr->UseableSkillCheck(Engine::KEYGUIDE_TAB);
		////////////////////////////////////////////

		JumpAction(dTimeDelta);
	}
	// 점프가 아닐 때 이동 패턴
	else
	{
		m_pUIMgr->UseableSkillCheck_All(true);
		if (false == GraoundAction(m_pPlayer, dTimeDelta))
			return;
	}

	//if (false == ChangeState_ATK(m_pPlayer))
	//	return;

	if (false == Finish_Animation(m_pPlayer))
		return;
}

CPlayer::STATE_ID CCsState::LateUpadte_State(const _double dTimeDelta)
{
	if (CUIMgr::UITYPE_MAIN == CUIMgr::GetInstance()->Get_UIType())
		return CPlayer::ID_COMMON;

	if (Engine::MouseDown(Engine::DIM_LB))
	{
		m_pPlayer->Set_AnimationSpeed(1);
		m_fSpeed = m_pPlayer->Get_Speed();
		m_pPlayer->Set_JumpTime(m_dbJumpTime);
		m_pPlayer->Set_ReserveAction(Engine::SM_LB);

		if (m_pPlayer->Get_Jump())
		{
			if (m_pPlayer->Get_MainWeapon() == CPlayer::TWOHANDSWORD && m_pPlayer->Get_AirComboLevel() >= 4)
			{
				return CPlayer::ID_COMMON;
			}

			else if(m_pPlayer->Get_MainWeapon() == CPlayer::ORB && m_pPlayer->Get_AirComboLevel() >= 3)
				return CPlayer::ID_COMMON;

			else if (m_pPlayer->Get_MainWeapon() == CPlayer::LONGBOW && m_pPlayer->Get_AirComboLevel() >= 3)
				return CPlayer::ID_COMMON;

			return CPlayer::ID_AIRCOMBO;
		}

		else
			return CPlayer::ID_BASICCOMBO;
	}

	if (Engine::MouseDown(Engine::DIM_RB))
	{
		m_pPlayer->Set_AnimationSpeed(1);
		m_fSpeed = m_pPlayer->Get_Speed();
		m_pPlayer->Set_JumpTime(m_dbJumpTime);
		m_pPlayer->Set_ReserveAction(Engine::SM_RB);

		if (m_pPlayer->Get_Jump() && m_pPlayer->Get_MainWeapon() == CPlayer::ORB && m_pPlayer->Get_AirComboLevel() >= 3)
		{
			m_pPlayer->Set_AirComboLevel(4);
			return CPlayer::ID_AIRCOMBO;
		}

		if (m_pPlayer->Get_MainWeapon() == CPlayer::TWOHANDSWORD && !m_pPlayer->Get_Jump())
		{
			m_pPlayer->Set_AtkLevel(CBasePlayer::LEVEL4);
			return CPlayer::ID_BASICCOMBOEX;
		}

		if (m_pPlayer->Get_MainWeapon() == CPlayer::DUALSWORD && !m_pPlayer->Get_Jump())
		{
			m_pPlayer->Set_AtkLevel(CBasePlayer::LEVEL4);
			return CPlayer::ID_BASICCOMBOEX;
		}

		if (m_pPlayer->Get_MainWeapon() == CPlayer::LONGBOW && !m_pPlayer->Get_Jump())
		{
			m_pPlayer->Set_AtkLevel(CBasePlayer::LEVEL4);
			return CPlayer::ID_BASICCOMBOEX;
		}

		if (m_pPlayer->Get_MainWeapon() == CPlayer::ORB && !m_pPlayer->Get_Jump())
		{
			m_pPlayer->Set_AtkLevel(CBasePlayer::LEVEL2);
			return CPlayer::ID_BASICCOMBOEX;
		}
	}

	// Skill
	if (m_pPlayer->Get_Jump())
		return CPlayer::ID_COMMON;

	//if (Engine::KeyDown(DIK_C))
	//{
	//	m_dbCaptureTime = 0.0;
	//	m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_CAPTUREFLAG_START);
	//	m_pPlayer->Set_State(CPlayer::COMMON_CAPTUREFLAG_START);
	//	m_bIsReserve = true;
	//	m_eReserveState = CPlayer::COMMON_CAPTUREFLAG_LOOP;
	//}

	if (Engine::KeyDown(DIK_E))
	{
		if (0 > m_pPlayer->Get_Mp() - 200)
		{
			// 마나 부족 ui 출력
			m_pUIMgr->CreateDeficientMP();
			return CPlayer::ID_COMMON;
		}
		else if (m_pUIMgr->CheckSkillCoolDowning(Engine::KEYGUIDE_E))
		{
			m_pUIMgr->OnCoolDown();
			return CPlayer::ID_COMMON;
		}
		m_pPlayer->Set_Mp(200);
		m_pPlayer->Set_ReserveAction(Engine::SM_E);
		//m_pPlayer->Set_AnimationSpeed(2);
	
		return CPlayer::ID_SKILL;
	}

	if (Engine::KeyDown(DIK_Q))
	{
		// 분노로 바꿔야 함
		if (100 > m_pPlayer->Get_Rage())
		{
			int i = 0;
			// 분노 부족 ui 출력
			return CPlayer::ID_COMMON;
		}
		else if (m_pUIMgr->CheckSkillCoolDowning(Engine::KEYGUIDE_Q))
		{
			m_pUIMgr->OnCoolDown();
			return CPlayer::ID_COMMON;
		}
		m_pPlayer->Set_Rage(100);
		m_pPlayer->Set_ReserveAction(Engine::SM_Q);
		m_pPlayer->Set_AnimationSpeed(1);

		return CPlayer::ID_SKILL;
	}

	if (Engine::KeyDown(DIK_R))
	{
		if (0 > m_pPlayer->Get_Mp() - 200)
		{
			m_pUIMgr->CreateDeficientMP();
			// 마나 부족 ui 출력
			return CPlayer::ID_COMMON;
		}
		else if (m_pUIMgr->CheckSkillCoolDowning(Engine::KEYGUIDE_R))
		{
			m_pUIMgr->OnCoolDown();
			return CPlayer::ID_COMMON;
		}
		m_pPlayer->Set_Mp(200);
		m_pPlayer->Set_ReserveAction(Engine::SM_R);
		m_pPlayer->Set_AnimationSpeed(1);
		return CPlayer::ID_SKILL;
	}

	if (Engine::KeyDown(DIK_F))
	{
		if (0 > m_pPlayer->Get_Mp() - 200)
		{
			m_pUIMgr->CreateDeficientMP();
			// 마나 부족 ui 출력
			return CPlayer::ID_COMMON;
		}
		else if (m_pUIMgr->CheckSkillCoolDowning(Engine::KEYGUIDE_F))
		{
			m_pUIMgr->OnCoolDown();
			return CPlayer::ID_COMMON;
		}
		m_pPlayer->Set_Mp(200);
		m_pPlayer->Set_ReserveAction(Engine::SM_F);
		m_pPlayer->Set_AnimationSpeed(1);

		return CPlayer::ID_SKILL;
	}
	
	return CPlayer::ID_COMMON;
}

void CCsState::JumpAction(const _double dTimeDelta)
{
	m_dbJumpTime += dTimeDelta;
	_vec3 *vUp = m_pTransform->Get_Info(Engine::INFO_UP);
	_vec3 vPos = m_pPlayer->GetPlayerPos();
	_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
	_matrix matView = m_pPlayer->Get_ViewMatrix();
	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
	if (Engine::KeyDown(DIK_LSHIFT) && !m_bDash)
	{
		if (m_pUIMgr->CheckSkillCoolDowning(Engine::KEYGUIDE_LSHIFT) && !m_pPlayer->Get_Tired())
		{
			m_pUIMgr->OnCoolDown();
		}
		else
		{
			if (0 < m_pPlayer->Get_Stamina() && !m_pPlayer->Get_Tired())
			{
				m_pPlayer->Set_Stamina(400);
				m_pPlayer->Set_Incibility();
				m_pUIMgr->SkillCoolDownCheck(Engine::KEYGUIDE_LSHIFT);
				m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_DASH);
				m_pPlayer->Set_State(CPlayer::COMMON_DASH);
				CSoundMgr::Get_Instance()->SoundOn(83);
				m_pPlayer->Set_AnimationSpeed(1.5);
				m_bDash = true;
				m_pPlayer->Set_Incibility();
				m_pPlayer->Get_Renderer()->Set_RadialBlur(true);
				m_dbDash = m_pMeshCom->Get_Period() * 0.75;
			}
		}
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
			m_pPlayer->Get_Renderer()->Set_RadialBlur(false);
			m_pPlayer->ReSet_Incibility();
			//if (Engine::KeyPressing(DIK_W) || Engine::KeyPressing(DIK_A) || Engine::KeyPressing(DIK_S) || Engine::KeyPressing(DIK_D))
			//{
			//	m_bIsRun = true;
			//}
			//else
			{
				//if (m_pPlayer->Get_Tired())
				//{
				//	m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_TIRED);
				//	m_bIsReserve = false;
				//}
				//else
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_AIR_JUMPDOWN);
					m_pPlayer->Set_State(CPlayer::COMMON_AIR_JUMPDOWN);
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

	if (Engine::KeyDown(DIK_W) || Engine::KeyDown(DIK_A) || Engine::KeyDown(DIK_S) || Engine::KeyDown(DIK_D))
		m_bIsJumpMove = true;

	if (m_bIsJumpMove && !m_pNaviCom->Check_Landing(&m_pPlayer->GetPlayerPos(),m_pPlayer->Get_NaviIndex()))
	{
		_matrix matView = m_pPlayer->Get_ViewMatrix();
		_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
		_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
		_vec3 vPos = m_pPlayer->GetPlayerPos();

		D3DXMatrixInverse(&matView, 0, &matView);
		D3DXVec3Normalize(&vLook, &vLook);

		if (Engine::KeyPressing(DIK_W))
		{
			_vec3 vMove = { matView.m[2][0], 0.f,matView.m[2][2] };
			vPurpose += vMove;
		}
		if (Engine::KeyPressing(DIK_A))
		{
			_vec3 vMove = { -matView.m[0][0], 0.f,-matView.m[0][2] };
			vPurpose += vMove;
		}
		if (Engine::KeyPressing(DIK_S))
		{
			_vec3 vMove = { -matView.m[2][0], 0.f,-matView.m[2][2] };
			vPurpose += vMove;
		}
		if (Engine::KeyPressing(DIK_D))
		{
			_vec3 vMove = { matView.m[0][0], 0.f,matView.m[0][2] };
			vPurpose += vMove;
		}

		_float fCrossy, fDegree;

		CPlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

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

		if (!Engine::KeyPressing(DIK_W) && !Engine::KeyPressing(DIK_A) && !Engine::KeyPressing(DIK_S) && !Engine::KeyPressing(DIK_D))
		{
			m_bIsJumpMove = false;
		}
	}

	vPos = m_pPlayer->GetPlayerPos();

	if (!m_bIsStop && !m_bDash)
	{
		if (!m_bIsPeak)
		{
			CPlayerState::Calculate_FaillingSpeed(1000.f, m_fAccelerate, 25.f, m_dbJumpTime, m_dbFall, &m_fSpeed);
			m_pNaviCom->Find_Index(m_dwNaviIndex, &m_pPlayer->GetPlayerPos());
			m_pPlayer->Set_Index(m_dwNaviIndex);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(*vUp * m_fSpeed*(_float)dTimeDelta*1.5), m_dwNaviIndex, true)));
			
			//m_pNaviCom->Find_Index(m_dwNaviIndex, &m_pPlayer->GetPlayerPos());
		}
		else
		{
			CPlayerState::Calculate_FaillingSpeed(0.f, m_fAccelerate, 15.f, m_dbJumpTime, m_dbFall, &m_fSpeed);
			m_pNaviCom->Find_Index(m_dwNaviIndex, &m_pPlayer->GetPlayerPos());
			m_pPlayer->Set_Index(m_dwNaviIndex);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(*vUp * m_fSpeed*(_float)dTimeDelta*2.5), m_dwNaviIndex, true)));
		}
		////cout << m_fSpeed << endl;
	}

	if (CPlayer::COMMON_AIR_JUMPUP == m_pPlayer->Get_CurState())
	{
		m_bIsReserve = true;

		m_eReserveState = CPlayer::COMMON_AIR_JUMPUP;

		if (m_fSpeed < 500.f)
			m_eReserveState = CPlayer::COMMON_AIR_JUMP;
		else
			m_eReserveState = CPlayer::COMMON_AIR_JUMPUP;
	}

	else if (CPlayer::COMMON_AIR_JUMP == m_pPlayer->Get_CurState())
	{
		m_bIsReserve = true;

		if (!m_bIsPeak)
		{
			if (m_fSpeed <= 0.f)
			{
				m_bIsPeak = true;
				m_dbJumpTime = 0;
			}

			m_eReserveState = CPlayer::COMMON_AIR_JUMP;
		}
		else
		{
			if (-250.f >= m_fSpeed)
			{
				m_eReserveState = CPlayer::COMMON_AIR_JUMPDOWN;
			}
			else
				m_eReserveState = CPlayer::COMMON_AIR_JUMP;
		}
	}

	else if (CPlayer::COMMON_AIR_JUMPLANDING == m_pPlayer->Get_CurState())
	{
		if (m_bJumpSound)
		{
			CSoundMgr::Get_Instance()->SoundOn(1);
			m_bJumpSound = false;
		}

		m_bIsReserve = true;
		_double test = m_pMeshCom->Get_Period();
		////cout << test << endl;
		if (m_pMeshCom->Is_AnimationSetFinish(0.65) && (Engine::KeyPressing(DIK_W) || Engine::KeyPressing(DIK_A) || Engine::KeyPressing(DIK_S) || Engine::KeyPressing(DIK_D)))
		{
			m_bIsRun = true;
			m_dbFall = 0;
			m_bIsPeak = false;
			m_eReserveState = CPlayer::COMMON_RUN;
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
			m_eReserveState = CPlayer::COMMON_COMBATWAIT;
			m_dbJumpTime = 0;
			m_fSpeed = m_pPlayer->Get_Speed();
			m_pPlayer->Set_Jump_Finish();
  			m_pPlayer->Set_AirComboLevel(0);
			m_pPlayer->Set_Fall_End();
		}
	}

	else if (CPlayer::COMMON_AIR_JUMPDOWN == m_pPlayer->Get_CurState())
	{
		m_pNaviCom->Find_Index(m_dwNaviIndex, &m_pPlayer->GetPlayerPos());
		m_pPlayer->Set_Index(m_dwNaviIndex);
		if (m_pNaviCom->Check_Landing(&m_pPlayer->GetPlayerPos(),m_dwNaviIndex))
		{
			m_bIsReserve = true;
			_vec3 vPlayerPos = *m_pTransform->Get_Info(Engine::INFO_POS);
			m_pNaviCom->Find_PosY(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex, vPos);
			m_pTransform->Set_Pos(&vPos);

			m_eReserveState = CPlayer::COMMON_AIR_JUMPLANDING;
			m_bJumpSound = true;
			m_fSpeed = m_pPlayer->Get_Speed();
			m_dbJumpTime = 0;
			m_bIsStop = true;
		}
		else if (!m_pNaviCom->Check_Landing(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex))
		{
			m_bIsReserve = true;
			m_eReserveState = CPlayer::COMMON_AIR_JUMPDOWN;
		}
	}

	else
	{
		m_bIsReserve = true;
		m_eReserveState = CPlayer::COMMON_AIR_JUMP;
	}

}

_bool CCsState::GraoundAction(CPlayer * pPlayer, const _double dTimeDelta)
{
	_vec3 vPos = pPlayer->GetPlayerPos();
	_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
	_matrix matView = pPlayer->Get_ViewMatrix();
	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);

	//if (m_pPlayer->Get_CurState() == CPlayer::COMMON_CAPTUREFLAG_LOOP)
	//{
	//	// 점령 시간 증가
	//	m_dbCaptureTime += dTimeDelta;

	//	if (5 <= m_dbCaptureTime)
	//	{
	//		m_pPlayer->Set_State(CPlayer::COMMON_CAPTUREFLAG_END);
	//		m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_CAPTUREFLAG_END);
	//		m_dbCaptureTime = 0.0;
	//	}
	//	if (m_pMeshCom->Is_AnimationSetFinish(0.2))
	//	{
	//		m_bIsReserve = true;
	//		m_eReserveState = CPlayer::COMMON_CAPTUREFLAG_LOOP;
	//	}
	//}


	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3Normalize(&vLook, &vLook);

	if (Engine::KeyDown(DIK_C) && !m_bDash)
	{
		if (pPlayer->GetDist_ToFlag() < 200)
		{
			_vec3 vLook = *pPlayer->Get_TransformCom()->Get_Info(Engine::INFO_LOOK);
			_vec3 vFlag = pPlayer->Get_FlagPos() - pPlayer->GetPlayerPos();
			float fAngle = Engine::GetAngleY(vLook, vFlag);
			if (!isnan(fAngle))
				pPlayer->Get_TransformCom()->Rotation(Engine::ROTATION::ROT_Y, fAngle);

			pPlayer->Set_State(CPlayer::COMMON_CAPTUREFLAG_START);
			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_CAPTUREFLAG_START);
			m_bIsReserve = true;
			m_eReserveState = CPlayer::COMMON_CAPTUREFLAG_LOOP;
		}
	}

	if (pPlayer->Get_CurState() == CPlayer::COMMON_CAPTUREFLAG_LOOP)
	{
		m_dbFlagCapture += dTimeDelta;

		if (0.75 <= m_dbFlagCapture)
		{
			m_dbFlagCapture = 0;
			CSoundMgr::Get_Instance()->SoundOn(76);
		}

		if (pPlayer->Get_CaptureFlag() == true)
		{
			pPlayer->Set_State(CPlayer::COMMON_CAPTUREFLAG_END);
			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_CAPTUREFLAG_END);
			CSoundMgr::Get_Instance()->SoundOn(77);
		}
	}

	if (Engine::KeyDown(DIK_LSHIFT) && !m_bDash)
	{
		if (m_pUIMgr->CheckSkillCoolDowning(Engine::KEYGUIDE_LSHIFT) && !pPlayer->Get_Tired())
		{
			m_pUIMgr->OnCoolDown();
		}
		else
		{
			if (0 < pPlayer->Get_Stamina() && !pPlayer->Get_Tired())
			{
				pPlayer->Set_Stamina(400);
				m_pPlayer->Set_Incibility();
				m_pUIMgr->SkillCoolDownCheck(Engine::KEYGUIDE_LSHIFT);
				m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_DASH);
				pPlayer->Set_State(CPlayer::COMMON_DASH);
				CSoundMgr::Get_Instance()->SoundOn(83);
				pPlayer->Set_AnimationSpeed(1.5);
				m_bDash = true;
				m_pPlayer->Set_Incibility();
				m_pPlayer->Get_Renderer()->Set_RadialBlur(true);
				m_dbDash = m_pMeshCom->Get_Period() * 0.75;
			}
		}
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
			m_pPlayer->Get_Renderer()->Set_RadialBlur(false);
			m_pPlayer->ReSet_Incibility();
			if (pPlayer->Get_Tired())
			{
				//m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_TIRED);
				m_bIsReserve = false;
				m_bIsRun = false;
			}

			if (Engine::KeyPressing(DIK_W) || Engine::KeyPressing(DIK_A) || Engine::KeyPressing(DIK_S) || Engine::KeyPressing(DIK_D))
			{     
				m_bIsRun = true;
			}

			else
			{
				m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_COMBATWAIT);
				pPlayer->Set_State(CPlayer::COMMON_COMBATWAIT);				
			}
		}
		if (!m_pMeshCom->Is_AnimationSetFinish(m_pMeshCom->Get_Period()*0.5))
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 1000.f * (_float)(m_dbDash*dTimeDelta)), m_dwNaviIndex)));
	}

	if (Engine::KeyDown(DIK_SPACE))
	{
		if (pPlayer->Get_Tired())
		{
			m_bIsReserve = false;
			return false;
		}
		
		m_fJumpMoveSpeed = m_fSpeed;
		pPlayer->Set_Jump_Start();
		m_pPlayer->ReSet_Incibility();
		m_fSpeed = pPlayer->Get_Speed();
		m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_AIR_JUMPUP);
		CSoundMgr::Get_Instance()->SoundOn(4);
		pPlayer->Set_State(CPlayer::COMMON_AIR_JUMPUP);
		m_pNaviCom->Find_PosY(&vPos, m_dwNaviIndex, m_vRecordY);
		m_dbJumpTime = 0;
		m_bIsStop = false;
		m_bDash = false;
		m_pPlayer->Get_Renderer()->Set_RadialBlur(false);
		return false;
	}

	if (Engine::KeyDown(DIK_W) || Engine::KeyDown(DIK_A) || Engine::KeyDown(DIK_S) || Engine::KeyDown(DIK_D))
		m_bIsRun = true;

	if (m_bIsRun && !m_bDash)
	{
		// ====== 달릴때 먼지 ======
		//if (m_dDustTime <= g_dAccumulatedTime)
		//{
		//	m_dDustTime = g_dAccumulatedTime + 0.2f;
		//	m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_RUN_DUST, &(m_pTransform->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 12.f, 0.f)));
		//}
		// =========================

		if (pPlayer->Get_Tired())
		{
			//m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_TIRED);
			m_bIsReserve = false;
			m_bIsRun = false;
			return true;
		}

		if (!Engine::KeyPressing(DIK_W) && !Engine::KeyPressing(DIK_A) && !Engine::KeyPressing(DIK_S) && !Engine::KeyPressing(DIK_D))
		{
			m_bIsRun = false;

			if (pPlayer->Get_Tired())
			{
				pPlayer->Set_AnimationSpeed(1);
				//m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_TIRED);
				//pPlayer->Set_State(CPlayer::COMMON_TIRED);
				m_bIsReserve = false;
				return false;
			}
			if (450.f <= m_fSpeed)
			{
				pPlayer->Set_AnimationSpeed(1);
				m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RUNEND_FAST);
				pPlayer->Set_State(CPlayer::COMMON_RUNEND_FAST);
				CSoundMgr::Get_Instance()->SoundOn(3);
			}
			else
			{
				pPlayer->Set_AnimationSpeed(1);
				m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RUNEND);
				pPlayer->Set_State(CPlayer::COMMON_RUNEND);
				//CSoundMgr::Get_Instance()->SoundOn(3);
				m_fSpeed = pPlayer->Get_Speed();
			}
			m_eReserveState = CPlayer::COMMON_COMBATWAIT;
			return false;
		}

		m_bIsReserve = true;

		if (Engine::KeyPressing(DIK_W))
		{
			_vec3 vMove = { matView.m[2][0], 0.f,matView.m[2][2] };
			vPurpose += vMove;
		}
		if (Engine::KeyPressing(DIK_A))
		{
			_vec3 vMove = { -matView.m[0][0], 0.f,-matView.m[0][2] };
			vPurpose += vMove;
		}
		if (Engine::KeyPressing(DIK_S))
		{
			_vec3 vMove = { -matView.m[2][0], 0.f,-matView.m[2][2] };
			vPurpose += vMove;
		}
		if (Engine::KeyPressing(DIK_D))
		{
			_vec3 vMove = { matView.m[0][0], 0.f,matView.m[0][2] };
			vPurpose += vMove;
		}

		_float fCrossy, fDegree;

		CPlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

		if (!isnan(fDegree))
		{
			if (fCrossy > 0.f)
				m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
			else
				m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
		}

		if (!m_pPlayer->Get_Tired())
		{
			if (m_fMaxSpeed > m_fSpeed)
				m_fSpeed += m_fAccelerate * (_float)dTimeDelta*2.f;
			else
				m_fSpeed = m_fMaxSpeed;
		}
		//if (450.f <= m_fSpeed)
		//{
		//	//m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RUN_FAST);
		//	//pPlayer->Set_State(CPlayer::COMMON_RUN_FAST);
		//}		

		if (!pPlayer->Get_Tired())
		{
			if (250.f <= m_fSpeed && 450.f > m_fSpeed)
			{
				if (15.f <= fDegree)
				{
					m_bIsCurve = true;
					if (fCrossy > 0.f)
					{
						m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RUN_R);
						m_pPlayer->Set_State(CPlayer::COMMON_RUN_R);
						m_eReserveState = CPlayer::COMMON_RUN_R;

						m_dbRun += dTimeDelta;
						if (0.35 <= m_dbRun)
						{
							CSoundMgr::Get_Instance()->SoundOn(2);
							m_dbRun = 0.0;
						}
					}
					else
					{
						m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RUN_L);
						m_pPlayer->Set_State(CPlayer::COMMON_RUN_L);
						m_eReserveState = CPlayer::COMMON_RUN_L;

						m_dbRun += dTimeDelta;
						if (0.35 <= m_dbRun)
						{
							CSoundMgr::Get_Instance()->SoundOn(2);
							m_dbRun = 0.0;
						}
					}
				}
				else
					m_bIsCurve = false;

				if (!m_bIsCurve)
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RUN);
					pPlayer->Set_State(CPlayer::COMMON_RUN);
					m_eReserveState = CPlayer::COMMON_RUN;

					m_dbRun += dTimeDelta;
					if (0.35 <= m_dbRun)
					{
						CSoundMgr::Get_Instance()->SoundOn(2);
						m_dbRun = 0.0;
					}
				}
			}

			else if (450.f <= m_fSpeed)
			{
				m_pPlayer->Set_AnimationSpeed(2);
				if (15.f <= fDegree)
				{
					m_bIsCurve = true;
					if (fCrossy > 0.f)
					{
						m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RUN_R);
						m_pPlayer->Set_State(CPlayer::COMMON_RUN_R);
						m_eReserveState = CPlayer::COMMON_RUN_R;

						m_dbRun += dTimeDelta;
						if (0.25 <= m_dbRun)
						{
							CSoundMgr::Get_Instance()->SoundOn(2);
							m_dbRun = 0.0;
						}
					}
					else
					{
						m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RUN_L);
						m_pPlayer->Set_State(CPlayer::COMMON_RUN_L);
						m_eReserveState = CPlayer::COMMON_RUN_L;

						m_dbRun += dTimeDelta;
						if (0.25 <= m_dbRun)
						{
							CSoundMgr::Get_Instance()->SoundOn(2);
							m_dbRun = 0.0;
						}
					}
				}
				else
					m_bIsCurve = false;

				if (!m_bIsCurve)
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RUN);
					pPlayer->Set_State(CPlayer::COMMON_RUN);
					m_eReserveState = CPlayer::COMMON_RUN;

					m_dbRun += dTimeDelta;
					if (0.25 <= m_dbRun)
					{
						CSoundMgr::Get_Instance()->SoundOn(2);
						m_dbRun = 0.0;
					}
				}
			}

			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * m_fSpeed * (_float)dTimeDelta), m_dwNaviIndex)));
		}
		
	}
	if (pPlayer->Get_CurState() == CPlayer::COMMON_RUNEND_FAST)
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


		if (m_pMeshCom->Is_AnimationSetFinish(0.3))
			m_fSpeed = pPlayer->Get_Speed();
	}


	return true;
}

_bool CCsState::ChangeState_ATK(CPlayer* pPlayer)
{
	return true;
}

_bool CCsState::Finish_Animation(CPlayer* pPlayer)
{
	if (m_bIsReserve && m_pMeshCom->Is_AnimationSetFinish(0.2))
	{
		if (pPlayer->Get_Tired() && m_eReserveState == CPlayer::COMMON_COMBATWAIT)
		{
			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_TIRED);
			pPlayer->Set_State(CPlayer::COMMON_TIRED);		
			m_bIsReserve = false;
			return false;
		}

		m_pMeshCom->Set_AnimationSet(m_eReserveState);
		pPlayer->Set_State(m_eReserveState);
		m_bIsReserve = false;
		return false;
	}

	if (!m_bIsReserve && !pPlayer->Get_Jump() && m_pMeshCom->Is_AnimationSetFinish(0.2))
	{
		if (pPlayer->Get_Tired())
		{
			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_TIRED);

			m_bIsReserve = false;
			return false;
		}

		m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_COMBATWAIT);
		pPlayer->Set_State(CPlayer::COMMON_COMBATWAIT);
		m_pPlayer->Set_AnimationSpeed(1);
		m_fSpeed = pPlayer->Get_Speed();
		return false;
	}

	if (m_pMeshCom->Is_AnimationSetFinish(0.2))
	{
		if (pPlayer->Get_Tired())
		{
			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_TIRED);
			pPlayer->Set_State(CPlayer::COMMON_TIRED);

			pPlayer->Set_AirComboLevel(0);
			m_bIsReserve = false;
			return false;
		}
		
		else
		{
			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_COMBATWAIT);
			pPlayer->Set_State(CPlayer::COMMON_COMBATWAIT);
			m_pPlayer->Set_AnimationSpeed(1);
			pPlayer->Set_AirComboLevel(0);
		}
		
		m_fSpeed = pPlayer->Get_Speed();
		return false;
	}
	return true;
}
