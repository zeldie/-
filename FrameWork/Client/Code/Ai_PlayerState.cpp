#include "stdafx.h"
#include "Ai_PlayerState.h"
#include "Ai_Cs_State.h"
#include "Transform.h"

#include "BasePlayer.h"
#include "EffectMgr.h"

CAi_PlayerState::CAi_PlayerState()
	: m_pMeshCom(nullptr),
	m_pTransform(nullptr),
	m_bIsReserve(false),
	m_bCreateEffect(false),
	m_bDash(false),
	m_bIsAttacked(false),
	m_bEffect(true),
	m_bDoubleEffect(true),
	m_eStartMotion(Engine::SM_END),
	m_pNaviCom(nullptr),
	m_dwNaviIndex(0),
	m_fSpeed(0.f),
	m_dbAccelTime(0),
	m_eReserveState(CAi_Player::STATE_END),
	m_tBaseInfo(nullptr),
	m_pEffectMgr(CEffectMgr::GetInstance()),
	m_pCalculatorCom(nullptr)
{
	
}

CAi_PlayerState::~CAi_PlayerState()
{
}

void CAi_PlayerState::Enter(CAi_Player * pPlayer)
{
}

void CAi_PlayerState::Update(CAi_Player * pPlayer, const _double dTimeDelta)
{
}

void CAi_PlayerState::ChangeState(CAi_PlayerState * pState)
{
}

void CAi_PlayerState::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _vec3 vMove, _float* fCrossy, _vec3* vDir, _float* fDegree)
{
	_vec3 vDirection = vMove;
	D3DXVec3Normalize(&vDirection, &vDirection);

	_vec3 vCross;
	D3DXVec3Cross(&vCross, &vPlayerLook, &vDirection);
	////cout << vCross.y << endl;
	_float fcos = D3DXVec3Dot(&vDirection, &vPlayerLook);
	*fDegree = D3DXToDegree(acosf(fcos));
	*fCrossy = vCross.y;
	*vDir = vDirection;
}

void CAi_PlayerState::Calculate_FaillingSpeed(const _float & fPower, const _float & fAccel, const _float & fGravity, const _double & dJumpingTime, const _double& dbFall, _float * fSpeed)
{
	*fSpeed = (fPower - fAccel * fGravity * (_float)dJumpingTime - (_float)dbFall*(_float)dJumpingTime);
}

void CAi_PlayerState::Is_Attacked()
{
	m_bIsAttacked = true;
}

void CAi_PlayerState::Attacked_End(CAi_Player* pPlayer)
{
	m_bIsAttacked = false;
	pPlayer->ExChange_State(new CAi_CsState);
}

void CAi_PlayerState::Enter_State(CBasePlayer * pPlayer)
{
	m_pPlayer = dynamic_cast<CAi_Player*>(pPlayer);
	m_pMeshCom = m_pPlayer->Get_PlayerMesh();
	m_pTransform = m_pPlayer->Get_TransformCom();
	m_fSpeed = m_pPlayer->Get_Speed();
	m_dwNaviIndex = m_pPlayer->Get_NaviIndex();
	m_pNaviCom = m_pPlayer->Get_NaviCom();
	m_eWeapon = m_pPlayer->Get_MainWeapon();
	m_pCalculatorCom = m_pPlayer->Get_CalculatorCom();
}

void CAi_PlayerState::Update_State(const _double dTimeDelta)
{
}

CAi_Player::STATE_ID CAi_PlayerState::LateUpadte_State(const _double dTimeDelta)
{
	return CAi_Player::STATEID_END;
}

void CAi_PlayerState::WeaponDissolve()
{
	if (m_pPlayer->Get_MainWeapon() == CBasePlayer::TWOHANDSWORD)
	{
		static_cast<CTwohandSword*>(m_pPlayer->Get_MainWeaponPointer())->Set_Dissolve(true);
	}
	else if (m_pPlayer->Get_MainWeapon() == CBasePlayer::DUALSWORD)
	{
		static_cast<CDualSword*>(m_pPlayer->Get_MainWeaponPointer())->Set_Dissolve(true);
		static_cast<CDualSword*>(m_pPlayer->Get_DualSwordL())->Set_Dissolve(true);
	}
	else if (m_pPlayer->Get_MainWeapon() == CBasePlayer::ORB)
	{
		static_cast<COrb*>(m_pPlayer->Get_MainWeaponPointer())->Set_Dissolve(true);
	}
	else if (m_pPlayer->Get_MainWeapon() == CBasePlayer::LONGBOW)
	{
		static_cast<CLongBow*>(m_pPlayer->Get_MainWeaponPointer())->Set_Dissolve(true);
	}
}

void CAi_PlayerState::Free(void)
{
}
