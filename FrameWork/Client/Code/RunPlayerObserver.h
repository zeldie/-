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
	// CObserver��(��) ���� ��ӵ�
	virtual void Update(int iMessage, void * pData) override;

public:
	static CRunPlayerObserver* Create();

private:
	_int					m_iHp;
	_int					m_iPrevHp;
	_int					m_iMaxHp;
	RUNPLAYER_POWER			m_ePower;		// �÷��̾� �Ŀ����� (�Ϲ�, �Ŵ�ȭ, ����)
	_double					m_dItem_durationTime_Big;	// �Ŵ�ȭ ������ ���ӽð�;
	_double					m_dItem_durationTime_SpeedUp;	// ���ǵ�� ������ ���ӽð�;

public:
	virtual void Free();

};


