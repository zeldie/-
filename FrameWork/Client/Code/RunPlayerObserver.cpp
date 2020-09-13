#include "stdafx.h"
#include "RunPlayerObserver.h"


CRunPlayerObserver::CRunPlayerObserver()
	:CObserver()
	, m_iHp(0)
	, m_iPrevHp(0)
	, m_iMaxHp(0)
	, m_ePower(RUN_POWER_END)
	, m_dItem_durationTime_Big(0.0)
	, m_dItem_durationTime_SpeedUp(0.0)
{
}

CRunPlayerObserver::~CRunPlayerObserver()
{
}

void CRunPlayerObserver::Update(int iMessage, void * pData)
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
	case POWER:
		m_ePower = *reinterpret_cast<RUNPLAYER_POWER*>(pData);
		break;
	case TIME_BIG:
		m_dItem_durationTime_Big = *reinterpret_cast<_double*>(pData);
		break;
	case TIME_SPEEDUP:
		m_dItem_durationTime_SpeedUp = *reinterpret_cast<_double*>(pData);
		break;
	default:
		break;
	}
}

CRunPlayerObserver * CRunPlayerObserver::Create()
{
	return new CRunPlayerObserver();
}

void CRunPlayerObserver::Free()
{
	Engine::CObserver::Free();
}
