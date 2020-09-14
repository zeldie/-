#include "stdafx.h"
#include "Ai_HeartState.h"

CAi_HeartState::CAi_HeartState()
	:m_eCollID(COLLISION_END),
	m_bAvoid(false),
	m_bRotationAvoid(true),
	m_bRotation(true),
	m_bKnockBack(true),
	m_bJumpHit(false),
	m_fAngle(0.f),
	m_dbKnockBackTime(0.0)
{
	ZeroMemory(m_vTargetPos, sizeof(_vec3));
	ZeroMemory(m_vPurpose, sizeof(_vec3));
}

CAi_HeartState::~CAi_HeartState()
{
}

void CAi_HeartState::Enter(CAi_Player * pPlayer)
{
	// Closed
}

void CAi_HeartState::Update(CAi_Player * pPlayer, const _double dTimeDelta)
{
	// Closed
}

void CAi_HeartState::ChangeState(CAi_PlayerState * pState)
{
	// Closed
}

void CAi_HeartState::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float * fCrossy, _vec3 * vDir, _float * fDegree)
{
	// Closed
}

void CAi_HeartState::Enter_State(CBasePlayer * pPlayer)
{
	CAi_PlayerState::Enter_State(pPlayer);

	m_eCollID = m_pPlayer->Get_CollisionID();
	
	m_pPlayer->Set_AirComboLevel(0);

	//// SuperArmor - Break
	//if (!m_pPlayer->Get_Break())
	//	m_eCollID = COLLISION_HIT;

	m_pPlayer->Set_BattleType(CAi_Player::TYPE_BATTLE);

	m_vTargetPos = m_pPlayer->Get_TargetPos();
	m_pPlayer->Get_Renderer()->Set_RadialBlur(false);
}

void CAi_HeartState::Update_State(const _double dTimeDelta)
{
	Update_PlayerPattern(dTimeDelta);
}

CAi_Player::STATE_ID CAi_HeartState::LateUpadte_State(const _double dTimeDelta)
{
	if (m_pPlayer->Get_TargetPlayer() == nullptr)
	{
		m_pPlayer->Set_isAttacked(false);
		m_bIsAttacked = false;
	}

	if (m_bIsAttacked)
		return CAi_Player::ID_HEART;
	else
	{
		if (0 >= m_pPlayer->Get_BaseInfo()->iHp)
			m_pPlayer->Set_Dead(true);
		return CAi_Player::ID_COMMON;
	}
}

void CAi_HeartState::Update_PlayerPattern(const _double dTimeDelta)
{
	if (m_pPlayer->Get_TargetPlayer() == nullptr)
		return;

	_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
	_vec3 vDir = *m_pTransform->Get_Info(Engine::INFO_POS) - m_vTargetPos;
	vDir.y = 0.f;

	D3DXVec3Normalize(&vDir, &vDir);

	if (m_bRotation)
	{
		_vec3 vTarget = m_vTargetPos - *m_pTransform->Get_Info(Engine::INFO_POS);
		vTarget.y = 0.f;
		D3DXVec3Normalize(&vTarget, &vTarget);
		_vec3 vLook =*m_pTransform->Get_Info(Engine::INFO_LOOK);
		vLook.y = 0.f;

		D3DXVec3Normalize(&vLook, &vLook);
		
		_float fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vTarget)));
		_vec3 vCross;
		D3DXVec3Cross(&vCross, &vLook, &vTarget);
		if (!isnan(fAngle))
		{
			if (vCross.y > 0.f)
			{
				m_pTransform->Rotation(Engine::ROT_Y, fAngle);
			}
			else
			{
				m_pTransform->Rotation(Engine::ROT_Y, 360.f-fAngle);
			}
		}

		//rotation
		m_bRotation = false;
	}

	switch (m_eCollID)
	{
	case COLLISION_HIT:
	{
		if (m_pPlayer->Get_CurState() == CAi_Player::COMMON_STANDUPATTACK || m_pPlayer->Get_CurState() == CAi_Player::COMMON_STANDUP_ROLLING)
		{
			if (m_bAvoid)
			{
				if (m_bRotationAvoid)
				{
					m_pTransform->Rotation(Engine::ROT_Y, m_fAngle);
					m_bRotationAvoid = false;
				}
				D3DXVec3Normalize(&m_vPurpose, &m_vPurpose);

				if (m_pMeshCom->Get_TrackTime() <= m_pMeshCom->Get_Period()*0.25)
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(*m_pTransform->Get_Info(Engine::INFO_LOOK) * 750.f * (_float)(1.0 - m_dbKnockBackTime)*(_float)dTimeDelta), m_dwNaviIndex)));
			}

			if (m_pMeshCom->Get_TrackTime()>=m_pMeshCom->Get_Period()*0.5)
			{
				m_pPlayer->Reset_SuperAmmor();
				m_bIsAttacked = false;
				m_bJumpHit = false;
				m_pPlayer->Set_Heart();
				break;
			}
			break;
		}

		else if (m_pPlayer->Get_CurState() == CAi_Player::COMMON_RTDOWN_AIR_LANDING_F_F)
		{
			//Key_Check();

			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{

				m_dbKnockBackTime = 0.0;
				if (m_bAvoid)
				{
					m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_STANDUP_ROLLING);
					m_pPlayer->Set_State(CAi_Player::COMMON_STANDUP_ROLLING);
				}
				else
				{
					m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_STANDUPATTACK);
					m_pPlayer->Set_State(CAi_Player::COMMON_STANDUPATTACK);
				}
				return;
			}
			return;
		}

		else if (m_pPlayer->Get_Jump())
		{
			m_bJumpHit = true;
			m_dbKnockBackTime += dTimeDelta;

			m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RTSTAND_AIR_FALL_SMALL_F);
			m_pPlayer->Set_State(CAi_Player::COMMON_RTSTAND_AIR_FALL_SMALL_F);

			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(*m_pTransform->Get_Info(Engine::INFO_LOOK) * 750.f * (_float)(1.0 - m_dbKnockBackTime)*(_float)dTimeDelta), m_dwNaviIndex, true)));
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(_vec3(0.f, -1.f, 0.f) * 250.f * (_float)(1.5 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex, true)));

			if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
			{
				_vec3 vPosY;
				m_pPlayer->Set_Jump_Finish();
				m_pPlayer->Set_Fall_End();
				m_pNaviCom->Find_PosY(&vPos, m_dwNaviIndex, vPosY);
				m_pTransform->Set_Pos(&vPosY);
				m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RTDOWN_AIR_LANDING_F_F);
				m_pPlayer->Set_State(CAi_Player::COMMON_RTDOWN_AIR_LANDING_F_F);
			}
		}

		else if (!m_bJumpHit)
		{
			m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RTSTAND_BIG_F);
			m_pPlayer->Set_State(CAi_Player::COMMON_RTSTAND_BIG_F);
			m_pPlayer->Set_AnimationSpeed(0.75);

			if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
			{
				_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * 250 * (_float)dTimeDelta*0.75), m_dwNaviIndex)));
			}

			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_bIsAttacked = false;
				m_pPlayer->Set_Heart();
				m_pPlayer->Set_AnimationSpeed(1);
			}
		}
		else
		{
			m_bIsAttacked = false;
			m_bJumpHit = false;
			m_pPlayer->Set_Heart();
		}
	}
		break;
	case COLLISION_KNOCKDOWN:
	{
		m_dbKnockBackTime += dTimeDelta;
		
		if (m_pPlayer->Get_CurState() == CAi_Player::COMMON_STANDUPATTACK || m_pPlayer->Get_CurState() == CAi_Player::COMMON_STANDUP_ROLLING)
		{			
			if (m_bAvoid)
			{
				if (m_bRotationAvoid)
				{
					m_pTransform->Rotation(Engine::ROT_Y, m_fAngle);
					m_bRotationAvoid = false;
				}
				D3DXVec3Normalize(&m_vPurpose, &m_vPurpose);
				
				if(m_pMeshCom->Get_TrackTime()<=m_pMeshCom->Get_Period()*0.25)
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(*m_pTransform->Get_Info(Engine::INFO_LOOK) * 750.f * (_float)(1.0-m_dbKnockBackTime)*(_float)dTimeDelta), m_dwNaviIndex)));
			}

			if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
			{
				m_pPlayer->Reset_SuperAmmor();
				m_bIsAttacked = false;
				m_pPlayer->Set_Heart();
				break;
			}
			break;
		}

		else if (m_bKnockBack)
		{
			if (m_pPlayer->Get_Jump())
			{
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * 750.f * (_float)(1.0 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex,true)));
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(_vec3(0.f,-1.f,0.f) * 750.f * (_float)(m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex,true)));
				
				m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RTSTAND_AIR_BIG_F);
				m_pPlayer->Set_State(CAi_Player::COMMON_RTSTAND_AIR_BIG_F);

				if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
				{
					_vec3 vPosY;
					m_pPlayer->Set_Jump_Finish();
					m_pPlayer->Set_Fall_End();
					m_pNaviCom->Find_PosY(&vPos, m_dwNaviIndex, vPosY);
					m_bKnockBack = false;
					m_dbKnockBackTime = 0.0;
				}
				/*if (0.5 <= m_dbKnockBackTime)
				{
					m_bKnockBack = false;
					m_dbKnockBackTime = 0.0;
				}*/
			}
			else
			{
				if (0.5 > m_dbKnockBackTime)
				{
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * 750.f * (_float)(1.0 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex)));
					m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RTSTAND_AIR_BIG_F);
					m_pPlayer->Set_State(CAi_Player::COMMON_RTSTAND_AIR_BIG_F);
				}
				if (0.5 <= m_dbKnockBackTime)
				{
					m_bKnockBack = false;
					m_dbKnockBackTime = 0.0;
				}
			}
		}

		else if(!m_bKnockBack)
		{
			m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RTDOWN_F_F);
			m_pPlayer->Set_State(CAi_Player::COMMON_RTDOWN_F_F);

			//Key_Check();

			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_dbKnockBackTime = 0.0;
				if (m_bAvoid)
				{
					m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_STANDUP_ROLLING);
					m_pPlayer->Set_State(CAi_Player::COMMON_STANDUP_ROLLING);
				}
				else
				{
					m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_STANDUPATTACK);
					m_pPlayer->Set_State(CAi_Player::COMMON_STANDUPATTACK);
				}
			}
		}

		else
		{
			m_bIsAttacked = false;
			m_bJumpHit = false;
			m_pPlayer->Set_Heart();
		}
	}
		break;
	case COLLISION_AIR_UP:
	{
		Airborne(dTimeDelta);

	}
		break;
	case COLLISION_AIR_DOWN:
		m_bIsAttacked = false;
		m_pPlayer->Set_Heart();
		break;
	case COLLISION_AIR_PUSH:
		m_bIsAttacked = false;
		m_pPlayer->Set_Heart();
		break;
	case COLLISION_END:
		m_bIsAttacked = false;
		m_pPlayer->Set_Heart();
		break;
	default:
		m_bIsAttacked = false;
		m_pPlayer->Set_Heart();
		break;
	}
}

void CAi_HeartState::Airborne(const _double dTimeDelta)
{
	m_dbKnockBackTime += dTimeDelta*2.0;

	_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
	_vec3 vDir = *m_pTransform->Get_Info(Engine::INFO_POS) - m_vTargetPos;
	vDir.y = 0.f;

	D3DXVec3Normalize(&vDir, &vDir);

	if (m_pPlayer->Get_CurState() == CAi_Player::COMMON_STANDUPATTACK || m_pPlayer->Get_CurState() == CAi_Player::COMMON_STANDUP_ROLLING)
	{
		if (m_bAvoid)
		{
			if (m_bRotationAvoid)
			{
				m_pTransform->Rotation(Engine::ROT_Y, m_fAngle);
				m_bRotationAvoid = false;
			}
			D3DXVec3Normalize(&m_vPurpose, &m_vPurpose);

			if (m_pMeshCom->Get_TrackTime() <= m_pMeshCom->Get_Period()*0.25)
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(*m_pTransform->Get_Info(Engine::INFO_LOOK) * 750.f * (_float)(1.0 - m_dbKnockBackTime)*(_float)dTimeDelta), m_dwNaviIndex)));
		}

		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
		{
			m_pPlayer->Reset_SuperAmmor();
			m_bIsAttacked = false;
			m_pPlayer->Set_Heart();
			return;
		}
		return;
	}

	if (m_pPlayer->Get_CurState() == CAi_Player::COMMON_RTDOWN_AIR_LANDING_F_F)
	{
		//Key_Check();

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_dbKnockBackTime = 0.0;
			if (m_bAvoid)
			{
				m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_STANDUP_ROLLING);
				m_pPlayer->Set_State(CAi_Player::COMMON_STANDUP_ROLLING);
			}
			else
			{
				m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_STANDUPATTACK);
				m_pPlayer->Set_State(CAi_Player::COMMON_STANDUPATTACK);
			}
			return;
		}
		return;
	}

	if (1.5 > m_dbKnockBackTime)
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RTSTAND_AIR_SMALL_F);
		m_pPlayer->Set_State(CAi_Player::COMMON_RTSTAND_AIR_SMALL_F);

		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * 250.f * (_float)(3.0 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex, true)));
		vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(_vec3(0.f, 1.f, 0.f) * 250.f * (_float)(1.5 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex, true)));
	}
	else
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RTDOWN_AIR_FALL_F_SMALL_F);
		m_pPlayer->Set_State(CAi_Player::COMMON_RTDOWN_AIR_FALL_F_SMALL_F);

		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * 250.f * (_float)(3.0 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex, true)));
		vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(_vec3(0.f, 1.f, 0.f) * 250.f * (_float)(1.5 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex, true)));
		
		if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
		{
			_vec3 vPosY;
			m_pPlayer->Set_Jump_Finish();
			m_pPlayer->Set_Fall_End();
			m_pNaviCom->Find_PosY(&vPos, m_dwNaviIndex, vPosY);
			m_pTransform->Set_Pos(&vPosY);
			m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_RTDOWN_AIR_LANDING_F_F);
			m_pPlayer->Set_State(CAi_Player::COMMON_RTDOWN_AIR_LANDING_F_F);
		}
	}

	//if (3.0 <= m_dbKnockBackTime)
	//{
	//	m_bIsAttacked = false;
	//	m_pPlayer->Set_Heart();
	//}
}
