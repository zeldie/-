#include "stdafx.h"
#include "ApostleObserver.h"


CApostleObserver::CApostleObserver()
	:CObserver()
	, m_iHp(0)
	, m_iPrevHp(0)
	, m_iMaxHp(0)
	, m_iSuperArmor(0)
	, m_iMaxSuperArmor(0)
{
}

CApostleObserver::~CApostleObserver()
{
}

void CApostleObserver::Update(int iMessage, void * pData)
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
	default:
		break;
	}
}

CApostleObserver * CApostleObserver::Create()
{
	return new CApostleObserver();
}

void CApostleObserver::Free()
{
	Engine::CObserver::Free();
}
