#include "stdafx.h"
#include "MatchingGameObserver.h"


CMatchingGameObserver::CMatchingGameObserver()
	: m_iPlayerPoint(0)
	, m_iAIPoint(0)
	, m_eTurn(CMatchingMgr::MATCHINGTURN::TURN_END)
	, m_dbTime(0.)
{
}


CMatchingGameObserver::~CMatchingGameObserver()
{
}

void CMatchingGameObserver::Update(int iMessage, void * pData)
{
	if (pData == nullptr)
		return;

	switch (iMessage)
	{
	case CMatchingGameObserver::PLAYER_POINT:
		m_iPlayerPoint = *reinterpret_cast<_uint*>(pData);
		break;
	case CMatchingGameObserver::AI_POINT:
		m_iAIPoint = *reinterpret_cast<_uint*>(pData);
		break;
	case CMatchingGameObserver::TURN:
		m_eTurn = *reinterpret_cast<CMatchingMgr::MATCHINGTURN*>(pData);
		break;
	case CMatchingGameObserver::TIME:
		m_dbTime = *reinterpret_cast<_double*>(pData);
		break;
	case CMatchingGameObserver::PLAYER_WIN:
		break;
	case CMatchingGameObserver::AI_WIN:
		break;
	case CMatchingGameObserver::DRAW:
		break;
	case CMatchingGameObserver::GAME_START:
		break;
	case CMatchingGameObserver::GAME_END:
		break;
	case CMatchingGameObserver::MESSAGE_END:
		break;
	default:
		break;
	}

}

CMatchingGameObserver * CMatchingGameObserver::Create()
{
	return new CMatchingGameObserver();
}

void CMatchingGameObserver::Free()
{
	Engine::CObserver::Free();
}
