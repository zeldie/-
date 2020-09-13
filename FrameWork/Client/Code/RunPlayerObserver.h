#pragma once
#include "Observer.h"

//BEGIN(Engine)
//class CTransform;
//END

class CRunPlayerObserver : public Engine::CObserver
{
public:
	enum MESSAGE { HP, PREVHP, MAXHP, POWER, TIME_BIG, TIME_SPEEDUP, MESSAGE_END };

private:
	explicit CRunPlayerObserver();
	virtual ~CRunPlayerObserver();

public:
	_int&				Get_Hp() { return m_iHp; }
	_int&				Get_PrevHp() { return m_iPrevHp; }
	_int&				Get_MaxHp() { return m_iMaxHp; }
	RUNPLAYER_POWER		Get_Power() { return m_ePower; }
	_double				Get_Time_Big() { return m_dItem_durationTime_Big; }
	_double				Get_Time_SpeedUp() { return m_dItem_durationTime_SpeedUp; }

public:
	// CObserver을(를) 통해 상속됨
	virtual void Update(int iMessage, void * pData) override;

public:
	static CRunPlayerObserver* Create();

private:
	_int					m_iHp;
	_int					m_iPrevHp;
	_int					m_iMaxHp;
	RUNPLAYER_POWER			m_ePower;		// 플레이어 파워상태 (일반, 거대화, 비행)
	_double					m_dItem_durationTime_Big;	// 거대화 아이템 지속시간;
	_double					m_dItem_durationTime_SpeedUp;	// 스피드업 아이템 지속시간;

public:
	virtual void Free();

};


