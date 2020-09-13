#include "stdafx.h"
#include "AI_Enemy2_Observer.h"


CAI_Enemy2_Observer::CAI_Enemy2_Observer()
	:CObserver()
{	
	m_iHp = 0;
	m_iPrevHp = 0;
	m_iMaxHp = 0;
	m_iSuperArmor = 0;
	m_iMaxSuperArmor = 0;
	m_iMainWeapon=0;
	m_iSubWeapon=0;
}


CAI_Enemy2_Observer::~CAI_Enemy2_Observer()
{
}

void CAI_Enemy2_Observer::Update(int iMessage, void * pData)
{
	if (pData == nullptr)
		return;

	switch (iMessage)
	{
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
	case MAINWEAPON:
		m_iMainWeapon = *reinterpret_cast<_int*>(pData);
		break;
	case SUBWEAPON:
		m_iSubWeapon = *reinterpret_cast<_int*>(pData);
		break;
	default:
		break;
	}
}

CAI_Enemy2_Observer * CAI_Enemy2_Observer::Create()
{
	return new CAI_Enemy2_Observer();
}

void CAI_Enemy2_Observer::Free()
{
	Engine::CObserver::Free();
}
