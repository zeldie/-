#include "stdafx.h"
#include "BasePlayer.h"

#include "PlayerState.h"
#include "Ai_PlayerState.h"
#include "PlayerObserver.h"

CBasePlayer::CBasePlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_pColldier(nullptr),
	m_eCurState(COMMON_WAIT),
	m_ePlayerID(ID_END),
	m_ePlayingType(PLAYINGCHARACTER),
	m_pState(nullptr),
	m_eIdCurrent(STATEID_END),
	m_eIdNext(STATEID_END),
	m_eAtkLevel(LEVEL_END),
	m_dbRage(0),
	m_dbMaxRage(0),
	m_pTargetPlayer(nullptr),
	m_pCalculatorCom(nullptr),
	m_pDecal(nullptr),
	m_pOrb(nullptr),
	m_pTwoHandSword(nullptr),
	m_pLongBow(nullptr),
	m_pDualSword_R(nullptr),
	m_pDualSword_L(nullptr)
{
	ZeroMemory(&m_vDecalPos, sizeof(_vec3));
}

CBasePlayer::~CBasePlayer()
{
}

_vec3 CBasePlayer::GetPlayerPos()
{
	return *m_pTransformCom->Get_Info(Engine::INFO_POS);
}

HRESULT CBasePlayer::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	return S_OK;
}

_int CBasePlayer::Update_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::Update_GameObject(dTimeDelta);
	return iExit;
}

_int CBasePlayer::LateUpdate_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::LateUpdate_GameObject(dTimeDelta);
	return iExit;
}

void CBasePlayer::Render_Geometry(const _double & dTimeDelta)
{
}

void CBasePlayer::Set_DownKey(INPUT_KEY eKey)
{
	m_pAiState->Set_DownKey(eKey);
}

void CBasePlayer::Set_UpKey(INPUT_KEY eKey)
{
	m_pAiState->Set_UpKey(eKey);
}

void CBasePlayer::Set_PressKey(INPUT_KEY eKey)
{
	m_pAiState->Set_PressKey(eKey);
}

void CBasePlayer::Set_RageZero()
{
	m_dbRage = 0.f;
	Engine::Notify(Engine::PLAYER, CPlayerObserver::RAGE,&m_dbRage);
}

_bool CBasePlayer::CheckLookDir(const _double& dTimeDelta)
{
	if (m_bisCheckLookDir == false)
	{
		_vec3 vLook = m_pTransformCom->Get_Info_byWorld(Engine::INFO::INFO_LOOK);
		_vec3 vDirToEnermy = m_pTargetPlayer->GetPlayerPos() - m_pTransformCom->Get_Info_byWorld(Engine::INFO::INFO_POS);
		vLook.y = 0.f;
		vDirToEnermy.y = 0.f;
		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Normalize(&vDirToEnermy, &vDirToEnermy);

		float fAngle = Engine::GetAngleY(vLook, vDirToEnermy);
		float fAngle2 = fAngle;
		////cout << fAngle << endl;

		if (fabsf(fAngle2) > 3)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, (fAngle * dTimeDelta) * 10);
			return false;
		}
		else
		{
			////cout << "Angle " << fAngle << endl;
			m_bisCheckLookDir = true;
			return true;
		}
	}
}

void CBasePlayer::Set_LookDir_ToTarget(const _double & dTimeDelta)
{
	if (m_bisSetLookDir == false)
	{
		_vec3 vLook = m_pTransformCom->Get_Info_byWorld(Engine::INFO::INFO_LOOK);
		_vec3 vDir = Engine::GetDirNoY(m_pTargetPlayer->GetPlayerPos(), m_pTransformCom->Get_Info_byWorld(Engine::INFO::INFO_POS));
		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Normalize(&vDir, &vDir);

		float fAngle = Engine::GetAngleY(vLook, vDir);

		if (!isnan(fAngle))
			m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		
		m_bisSetLookDir = true;
	}
}

_float CBasePlayer::GetDist_ToEnermy()
{
	if (m_pTargetPlayer == nullptr)
		return 0.f;

	_float fDist = Engine::GetDistNoY(m_pTargetPlayer->GetPlayerPos(), *m_pTransformCom->Get_Info(Engine::INFO_POS));

	return fDist;
}

_float CBasePlayer::GetDist_ToFlag()
{

	_float fDist = Engine::GetDistNoY(m_vFlagPos, *m_pTransformCom->Get_Info(Engine::INFO_POS));

	return fDist;
}

CDynamicMeshObject * CBasePlayer::Get_MainWeaponPointer()
{
	switch (m_eMainWeapon)
	{
	case CBasePlayer::COMMON:
		break;
	case CBasePlayer::DUALSWORD:
		return m_pDualSword_R;
		break;
	case CBasePlayer::TWOHANDSWORD:
		return m_pTwoHandSword;
		break;
	case CBasePlayer::ORB:
		return m_pOrb;
		break;
	case CBasePlayer::LONGBOW:
		return m_pLongBow;
		break;
	default:
		break;
	}
	return nullptr;
}

void CBasePlayer::Release_Decal()
{
	if (m_pDecal != nullptr)
	{
		m_pDecal->Set_Dead();
		m_pDecal = nullptr;
	}
}

HRESULT CBasePlayer::Create_Decal(CDecal::DECAL_TYPE eDecalType, _double dCountDown, _vec3 vPlayerAngle)
{
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = m_pDecal = CDecal::Create(Engine::Get_Device(), eDecalType, Get_DecalPos(), dCountDown, vPlayerAngle, m_eCtrlType);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::GAMEOBJECT, L"Decal", pGameObject);
	return S_OK;
}

void CBasePlayer::RotationToDecal()
{
	if (m_pDecal == nullptr)
		return;
	_vec3 vPlayerLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	_vec3 vToDecal = m_vDecalPos - GetPlayerPos();
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	D3DXVec3Normalize(&vToDecal, &vToDecal);
	_float fAngle = Engine::GetAngleY(vPlayerLook, vToDecal);
	_float fabsfAngle = fabsf(fAngle);
	if(fabsfAngle  > 3.f)
		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
}

HRESULT CBasePlayer::Clone_Component()
{
	return S_OK;
}

void CBasePlayer::Change_State()
{
}

void CBasePlayer::ExChange_State(CPlayerState* pState, _bool bAtk)
{
}

CBasePlayer * CBasePlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CBasePlayer::Free()
{
	CBaseObject::Free();
}
