#pragma once
#include "Observer.h"

//BEGIN(Engine)
//class CTransform;
//END

class CApostleObserver : public Engine::CObserver
{
public:
	enum MESSAGE { HP, PREVHP, MAXHP, SUPERARMOR, MAXSUPERARMOR, MESSAGE_END };

private:
	explicit CApostleObserver();
	virtual ~CApostleObserver();

public:
	_int&		Get_Hp() { return m_iHp; }
	_int&		Get_PrevHp() { return m_iPrevHp; }
	_int&		Get_MaxHp() { return m_iMaxHp; }
	_int&		Get_SuperArmor() { return m_iSuperArmor; }
	_int&		Get_MaxSuperArmor() { return m_iMaxSuperArmor; }

public:
	// CObserver을(를) 통해 상속됨
	virtual void Update(int iMessage, void * pData) override;

public:
	static CApostleObserver* Create();

private:
	_int					m_iHp;
	_int					m_iPrevHp;
	_int					m_iMaxHp;
	_int					m_iSuperArmor;
	_int					m_iMaxSuperArmor;

public:
	virtual void Free();

};


