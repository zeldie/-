#pragma once

#include "Observer.h"

class CMatchingGameObserver : public Engine::CObserver
{
public:
	enum MESSAGE { PLAYER_POINT, AI_POINT, TURN, TIME, PLAYER_WIN, AI_WIN, DRAW, GAME_START, GAME_END, MESSAGE_END };
private:
	explicit CMatchingGameObserver();
	virtual ~CMatchingGameObserver();

public:
	//Getter
	_uint Get_PlayerPoint() { return m_iPlayerPoint; }
	_uint Get_AIPoint() { return m_iAIPoint; }
	CMatchingMgr::MATCHINGTURN& Get_Turn() { return m_eTurn; }

public:
	// CObserver을(를) 통해 상속됨
	virtual void Update(int iMessage, void * pData) override;

private:
	_uint							m_iPlayerPoint;
	_uint							m_iAIPoint;
	CMatchingMgr::MATCHINGTURN		m_eTurn;
	_double							m_dbTime;

public:
	static CMatchingGameObserver* Create();
	virtual void Free();
};

