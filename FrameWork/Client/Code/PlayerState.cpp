#include "stdafx.h"
#include "PlayerState.h"
#include "Cs_State.h"
#include "Transform.h"

#include "BasePlayer.h"
#include "EffectMgr.h"

CPlayerState::CPlayerState()
	: m_pMeshCom(nullptr),
	m_pTransform(nullptr),
	m_bIsReserve(false),
	m_bCreateEffect(false),
	m_bDash(false),
	m_bIsAttacked(false),
	m_bEffect(true),
	m_bDoubleEffect(true),
	m_bActionZoom(true),
	m_bRotation(true),
	m_eStartMotion(Engine::SM_END),
	m_pNaviCom(nullptr),
	m_dwNaviIndex(0),
	m_fSpeed(0.f),
	m_dbAccelTime(0),
	m_eReserveState(CPlayer::STATE_END),
	m_tBaseInfo(nullptr),
	m_pEffectMgr(CEffectMgr::GetInstance()),
	m_pCalculatorCom(nullptr),
	m_bFindTarget(false)
{
	ZeroMemory(&m_vTargetPos, sizeof(_vec3));
}

CPlayerState::~CPlayerState()
{
}

void CPlayerState::Enter(CPlayer * pPlayer)
{
}

void CPlayerState::Update(CPlayer * pPlayer, const _double dTimeDelta)
{
}

void CPlayerState::ChangeState(CPlayerState * pState)
{
}

void CPlayerState::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _vec3 vMove, _float* fCrossy, _vec3* vDir, _float* fDegree)
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

void CPlayerState::Calculate_FaillingSpeed(const _float & fPower, const _float & fAccel, const _float & fGravity, const _double & dJumpingTime, const _double& dbFall, _float * fSpeed)
{
	*fSpeed = (fPower - fAccel * fGravity * (_float)dJumpingTime - (_float)dbFall*(_float)dJumpingTime);
}

void CPlayerState::Is_Attacked()
{
	m_bIsAttacked = true;
}

void CPlayerState::Attacked_End(CPlayer* pPlayer)
{
	m_bIsAttacked = false;
	//pPlayer->ExChange_State(new CCsState);
}

void CPlayerState::Enter_State(CBasePlayer * pPlayer)
{
	m_pPlayer = dynamic_cast<CPlayer*>(pPlayer);
	m_pMeshCom = m_pPlayer->Get_PlayerMesh();
	m_pTransform = m_pPlayer->Get_TransformCom();
	m_fSpeed = m_pPlayer->Get_Speed();
	m_dwNaviIndex = m_pPlayer->Get_NaviIndex();
	m_pNaviCom = m_pPlayer->Get_NaviCom();
	m_eWeapon = m_pPlayer->Get_MainWeapon();
	m_pCalculatorCom = m_pPlayer->Get_CalculatorCom();
}

void CPlayerState::Update_State(const _double dTimeDelta)
{
}

CPlayer::STATE_ID CPlayerState::LateUpadte_State(const _double dTimeDelta)
{
	return CPlayer::STATEID_END;
}

void CPlayerState::WeaponDissolve()
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

void CPlayerState::RotationTarget()
{
	if (m_pPlayer->Get_TargetLock())
	{
		_vec3 vTarget = m_pPlayer->Get_Focus() - *m_pTransform->Get_Info(Engine::INFO_POS);
		vTarget.y = 0.f;
		D3DXVec3Normalize(&vTarget, &vTarget);

		_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
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
				m_pTransform->Rotation(Engine::ROT_Y, 360.f - fAngle);
			}
		}
	}
	else
		int i = 0;
}

void CPlayerState::RotationCam()
{
	_matrix matView = m_pPlayer->Get_ViewMatrix();
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3 vTarget = { matView.m[2][0], 0.f,matView.m[2][2] };	
	vTarget.y = 0.f;
	D3DXVec3Normalize(&vTarget, &vTarget);

	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
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
			////cout << "Rot Angle :" <<fAngle << endl;
		}
		else
		{
			m_pTransform->Rotation(Engine::ROT_Y, 360.f - fAngle);
			////cout << "Rot Angle :" << 360.f - fAngle << endl;
		}
	}
}

void CPlayerState::RotationTarget_Range(_int iChooseTarget)
{
	if (iChooseTarget == 0)
	{
		m_vTargetPos = _vec3(0.f, 0.f, 0.f);
		m_bFindTarget = false;
		return;
	}
	else if (iChooseTarget == 1)
	{
		_vec3 vPlayerToBelatos = Engine::GetDirNoY(*(CUIMgr::GetInstance()->Get_BelatosTransformCom()->Get_Info(Engine::INFO_POS)), m_pPlayer->GetPlayerPos());
		_vec3 vLook = m_pPlayer->GetDir(Engine::INFO_LOOK);
		_float fAngle = Engine::GetAngleY(vLook, vPlayerToBelatos);
		if (!isnan(fAngle))
		{
			m_pTransform->Rotation(Engine::ROT_Y, fAngle);
			m_vTargetPos = *(CUIMgr::GetInstance()->Get_BelatosTransformCom()->Get_Info(Engine::INFO_POS)) + _vec3(0.f, 50.f, 0.f);
			m_bFindTarget = true;
		}
	}
	else if (iChooseTarget == 2)
	{
		_vec3 vPlayerToAI1 = Engine::GetDirNoY(*(CUIMgr::GetInstance()->Get_AI1TransformCom()->Get_Info(Engine::INFO_POS)), m_pPlayer->GetPlayerPos());
		_vec3 vLook = m_pPlayer->GetDir(Engine::INFO_LOOK);
		_float fAngle = Engine::GetAngleY(vLook, vPlayerToAI1);
		if (!isnan(fAngle))
		{
			m_pTransform->Rotation(Engine::ROT_Y, fAngle);
			m_vTargetPos = *(CUIMgr::GetInstance()->Get_AI1TransformCom()->Get_Info(Engine::INFO_POS)) + _vec3(0.f, 50.f, 0.f);
			m_bFindTarget = true;
		}
	}
	else if (iChooseTarget == 3)
	{
		_vec3 vPlayerToAI2 = Engine::GetDirNoY(*(CUIMgr::GetInstance()->Get_AI2TransformCom()->Get_Info(Engine::INFO_POS)), m_pPlayer->GetPlayerPos());
		_vec3 vLook = m_pPlayer->GetDir(Engine::INFO_LOOK);
		_float fAngle = Engine::GetAngleY(vLook, vPlayerToAI2);
		if (!isnan(fAngle))
		{
			m_pTransform->Rotation(Engine::ROT_Y, fAngle);
			m_vTargetPos = *(CUIMgr::GetInstance()->Get_AI2TransformCom()->Get_Info(Engine::INFO_POS)) + _vec3(0.f, 50.f, 0.f);
			m_bFindTarget = true;
		}
	}
}

void CPlayerState::Free(void)
{
}
