#include "stdafx.h"
#include "PlayerObserver.h"


CPlayerObserver::CPlayerObserver()
	:CObserver()
{
	ZeroMemory(&m_vPos, sizeof(_vec3));
	
	m_iHp = 0;
	m_iPrevHp = 0;
	m_iMaxHp = 0;
	m_iSuperArmor = 0;
	m_iMaxSuperArmor = 0;
	m_iStamina = 0;
	m_iMaxStamina = 0;
	m_iMp = 0;
	m_iMaxMp = 0;
	m_iMainWeapon=0;
	m_iSubWeapon=0;
	m_dbRage = 0;
	m_dbMaxRage = 0;
}


CPlayerObserver::~CPlayerObserver()
{
}

void CPlayerObserver::Update(int iMessage, void * pData)
{
	if (pData == nullptr)
		return;

	switch (iMessage)
	{
	case POS:
		m_vPos = *reinterpret_cast<_vec3*>(pData);
		break;
	case LOOK:
		m_vLook = *reinterpret_cast<_vec3*>(pData);
		break;
	case HP:
		m_iHp = *reinterpret_cast<_int*>(pData);
		break;
	case PREVHP:
		m_iPrevHp = *reinterpret_cast<_int*>(pData);
		break;
	case MAXHP:
		m_iMaxHp = *reinterpret_cast<_int*>(pData);
		break;
	case SUPERARMOR:
		m_iSuperArmor = *reinterpret_cast<_int*>(pData);
		break;
	case MAXSUPERARMOR:
		m_iMaxSuperArmor = *reinterpret_cast<_int*>(pData);
		break;
	case STAMINA:
		m_iStamina = *reinterpret_cast<_int*>(pData);
		break;
	case MAXSTAMINA:
		m_iMaxStamina = *reinterpret_cast<_int*>(pData);
		break;
	case MP:
		m_iMp = *reinterpret_cast<_float*>(pData);
		break;
	case MAXMP:
		m_iMaxMp = *reinterpret_cast<_float*>(pData);
		break;
	case MAINWEAPON:
		m_iMainWeapon = *reinterpret_cast<_int*>(pData);
		break;
	case SUBWEAPON:
		m_iSubWeapon = *reinterpret_cast<_int*>(pData);
		break;
	case RAGE:
		m_dbRage= *reinterpret_cast<_double*>(pData);
		break;
	case MAXRAGE:
		m_dbMaxRage = *reinterpret_cast<_double*>(pData);
		break;
	default:
		break;
	}
}

CPlayerObserver * CPlayerObserver::Create()
{
	return new CPlayerObserver();
}

void CPlayerObserver::Free()
{
	Engine::CObserver::Free();
}
