#include "stdafx.h"
#include "HeartState.h"

CHeartState::CHeartState()
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

CHeartState::~CHeartState()
{
}

void CHeartState::Enter(CPlayer * pPlayer)
{
	// Closed
}

void CHeartState::Update(CPlayer * pPlayer, const _double dTimeDelta)
{
	// Closed
}

void CHeartState::ChangeState(CPlayerState * pState)
{
	// Closed
}

void CHeartState::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float * fCrossy, _vec3 * vDir, _float * fDegree)
{
	// Closed
}

void CHeartState::Enter_State(CBasePlayer * pPlayer)
{
	CPlayerState::Enter_State(pPlayer);
	m_eCollID = m_pPlayer->Get_CollisionID();
	
	m_pPlayer->Set_AirComboLevel(0);

	// SuperArmor - Break
	if (!m_pPlayer->Get_Break())
		m_eCollID = COLLISION_HIT;

	m_pPlayer->Set_BattleType(CPlayer::TYPE_BATTLE);

	m_vTargetPos = m_pPlayer->Get_TargetPos();
	m_pPlayer->Get_Renderer()->Set_RadialBlur(false);
	CCameraMgr::GetInstance()->ReSet_ActionZoom();
}

void CHeartState::Update_State(const _double dTimeDelta)
{
	Update_PlayerPattern(dTimeDelta);
}

CPlayer::STATE_ID CHeartState::LateUpadte_State(const _double dTimeDelta)
{
	if (m_bIsAttacked)
		return CPlayer::ID_HEART;
	else
	{
		if (0 >= m_pPlayer->Get_BaseInfo()->iHp)
			m_pPlayer->Set_Dead();
		m_pPlayer->ReSet_Incibility();
		return CPlayer::ID_COMMON;
	}
}

void CHeartState::Update_PlayerPattern(const _double dTimeDelta)
{
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
		if (m_pPlayer->Get_CurState() == CPlayer::COMMON_STANDUPATTACK || m_pPlayer->Get_CurState() == CPlayer::COMMON_STANDUP_ROLLING)
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
				m_pPlayer->ReSet_Incibility();
				m_bIsAttacked = false;
				m_bJumpHit = false;
				m_pPlayer->Set_Heart();
				break;
			}
			break;
		}

		else if (m_pPlayer->Get_CurState() == CPlayer::COMMON_RTDOWN_AIR_LANDING_F_F)
		{
			Key_Check();

			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{

				m_dbKnockBackTime = 0.0;
				if (m_bAvoid)
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_STANDUP_ROLLING);
					m_pPlayer->Set_State(CPlayer::COMMON_STANDUP_ROLLING);
					m_pPlayer->Set_Incibility();
				}
				else
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_STANDUPATTACK);
					m_pPlayer->Set_State(CPlayer::COMMON_STANDUPATTACK);
					m_pPlayer->Set_Incibility();
				}
				return;
			}
			return;
		}

		else if (m_pPlayer->Get_Jump())
		{
			m_bJumpHit = true;
			m_dbKnockBackTime += dTimeDelta;

			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RTSTAND_AIR_FALL_SMALL_F);
			m_pPlayer->Set_State(CPlayer::COMMON_RTSTAND_AIR_FALL_SMALL_F);

			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(*m_pTransform->Get_Info(Engine::INFO_LOOK) * 750.f * (_float)(1.0 - m_dbKnockBackTime)*(_float)dTimeDelta), m_dwNaviIndex, true)));
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(_vec3(0.f, -1.f, 0.f) * 250.f * (_float)(1.5 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex, true)));

			if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
			{
				CSoundMgr::Get_Instance()->SoundOn(78);
				_vec3 vPosY;
				m_pPlayer->Set_Jump_Finish();
				m_pPlayer->Set_Fall_End();
				m_pNaviCom->Find_PosY(&vPos, m_dwNaviIndex, vPosY);
				m_pTransform->Set_Pos(&vPosY);
				m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RTDOWN_AIR_LANDING_F_F);
				m_pPlayer->Set_State(CPlayer::COMMON_RTDOWN_AIR_LANDING_F_F);
			}
		}

		else if (!m_bJumpHit)
		{
			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RTSTAND_BIG_F);
			m_pPlayer->Set_State(CPlayer::COMMON_RTSTAND_BIG_F);
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
		
		if (m_pPlayer->Get_CurState() == CPlayer::COMMON_STANDUPATTACK || m_pPlayer->Get_CurState() == CPlayer::COMMON_STANDUP_ROLLING)
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
				m_pPlayer->ReSet_Incibility();
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
				
				m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RTSTAND_AIR_BIG_F);
				m_pPlayer->Set_State(CPlayer::COMMON_RTSTAND_AIR_BIG_F);

				if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
				{
					CSoundMgr::Get_Instance()->SoundOn(78);
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
					m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RTSTAND_AIR_BIG_F);
					m_pPlayer->Set_State(CPlayer::COMMON_RTSTAND_AIR_BIG_F);
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
			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RTDOWN_F_F);
			m_pPlayer->Set_State(CPlayer::COMMON_RTDOWN_F_F);

			Key_Check();

			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_dbKnockBackTime = 0.0;
				if (m_bAvoid)
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_STANDUP_ROLLING);
					m_pPlayer->Set_State(CPlayer::COMMON_STANDUP_ROLLING);
					m_pPlayer->Set_Incibility();
				}
				else
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_STANDUPATTACK);
					m_pPlayer->Set_State(CPlayer::COMMON_STANDUPATTACK);
					m_pPlayer->Set_Incibility();
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

void CHeartState::Key_Check()
{
	_matrix matView = m_pPlayer->Get_ViewMatrix();
	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
	_vec3 vDir = { 0.f,0.f,0.f };

	if (Engine::KeyPressing(DIK_W))
	{
		_vec3 vMove = { matView.m[2][0], 0.f,matView.m[2][2] };
		m_vPurpose += vMove;
		m_bAvoid = true;
	}
	if (Engine::KeyPressing(DIK_A))
	{
		_vec3 vMove = { -matView.m[0][0], 0.f,-matView.m[0][2] };
		m_vPurpose += vMove;
		m_bAvoid = true;
	}
	if (Engine::KeyPressing(DIK_S))
	{
		_vec3 vMove = { -matView.m[2][0], 0.f,-matView.m[2][2] };
		m_vPurpose += vMove;
		m_bAvoid = true;
	}
	if (Engine::KeyPressing(DIK_D))
	{
		_vec3 vMove = { matView.m[0][0], 0.f,matView.m[0][2] };
		m_vPurpose += vMove;
		m_bAvoid = true;
	}

	_float fCrossy, fDegree;

	CPlayerState::Rotation_Direction(matView, vLook, m_vPurpose, &fCrossy, &vDir, &fDegree);

	if (!isnan(fDegree))
	{
		if (fCrossy > 0.f)
			m_fAngle = fDegree;
		else
			m_fAngle = 360.f - fDegree;			
	}

	//if (Engine::KeyDown(DIK_W) || Engine::KeyDown(DIK_A) || Engine::KeyDown(DIK_S) || Engine::KeyDown(DIK_D) || Engine::KeyDown(DIK_LSHIFT))
	//{
	//	m_bAvoid = true;
	//}
}

void CHeartState::Airborne(const _double dTimeDelta)
{
	m_dbKnockBackTime += dTimeDelta*2.0;

	_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
	_vec3 vDir = *m_pTransform->Get_Info(Engine::INFO_POS) - m_vTargetPos;
	vDir.y = 0.f;

	D3DXVec3Normalize(&vDir, &vDir);

	if (m_pPlayer->Get_CurState() == CPlayer::COMMON_STANDUPATTACK || m_pPlayer->Get_CurState() == CPlayer::COMMON_STANDUP_ROLLING)
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
			m_pPlayer->ReSet_Incibility();
			m_bIsAttacked = false;
			m_pPlayer->Set_Heart();
			return;
		}
		return;
	}

	if (m_pPlayer->Get_CurState() == CPlayer::COMMON_RTDOWN_AIR_LANDING_F_F)
	{
		Key_Check();

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_dbKnockBackTime = 0.0;
			if (m_bAvoid)
			{
				m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_STANDUP_ROLLING);
				m_pPlayer->Set_State(CPlayer::COMMON_STANDUP_ROLLING);
				m_pPlayer->Set_Incibility();
			}
			else
			{
				m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_STANDUPATTACK);
				m_pPlayer->Set_State(CPlayer::COMMON_STANDUPATTACK);
				m_pPlayer->Set_Incibility();
			}
			return;
		}
		return;
	}

	if (1.5 > m_dbKnockBackTime)
	{
		m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RTSTAND_AIR_SMALL_F);
		m_pPlayer->Set_State(CPlayer::COMMON_RTSTAND_AIR_SMALL_F);

		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * 250.f * (_float)(3.0 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex, true)));
		vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(_vec3(0.f, 1.f, 0.f) * 250.f * (_float)(1.5 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex, true)));
	}
	else
	{
		m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RTDOWN_AIR_FALL_F_SMALL_F);
		m_pPlayer->Set_State(CPlayer::COMMON_RTDOWN_AIR_FALL_F_SMALL_F);

		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * 250.f * (_float)(3.0 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex, true)));
		vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(_vec3(0.f, 1.f, 0.f) * 250.f * (_float)(1.5 - m_dbKnockBackTime) *(_float)dTimeDelta), m_dwNaviIndex, true)));
		
		if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
		{
			CSoundMgr::Get_Instance()->SoundOn(78);
			_vec3 vPosY;
			m_pPlayer->Set_Jump_Finish();
			m_pPlayer->Set_Fall_End();
			m_pNaviCom->Find_PosY(&vPos, m_dwNaviIndex, vPosY);
			m_pTransform->Set_Pos(&vPosY);
			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_RTDOWN_AIR_LANDING_F_F);
			m_pPlayer->Set_State(CPlayer::COMMON_RTDOWN_AIR_LANDING_F_F);
		}
	}

	//if (3.0 <= m_dbKnockBackTime)
	//{
	//	m_bIsAttacked = false;
	//	m_pPlayer->Set_Heart();
	//}
}
