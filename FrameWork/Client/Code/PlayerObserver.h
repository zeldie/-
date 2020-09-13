#pragma once

#include "Observer.h"

BEGIN(Engine)
class CTransform;
END

class CPlayerObserver : public Engine::CObserver
{
public:
	enum MESSAGE { POS, LOOK, HP, PREVHP, MAXHP, SUPERARMOR, MAXSUPERARMOR, STAMINA, MAXSTAMINA, MP, MAXMP, MAINWEAPON, SUBWEAPON, RAGE , MAXRAGE, MESSAGE_END };

	//희정>UI에 필요한거: HP, 이전HP, 최대 HP, MP, Stamina, SuperAromor,때린횟수, 콤보횟수? , 
private:
	explicit CPlayerObserver();
	virtual ~CPlayerObserver();

public:
	//Getter
	_vec3&		Get_Pos() { return m_vPos; }
	_vec3&		Get_Look() { return m_vLook; }
	_int&		Get_Hp() { return m_iHp; }
	_int&		Get_PrevHp() { return m_iPrevHp; }
	_int&		Get_MaxHp() { return m_iMaxHp; }
	_int&		Get_SuperArmor() { return m_iSuperArmor; }
	_int&		Get_MaxSuperArmor() { return m_iMaxSuperArmor; }
	_int&		Get_Stamina() { return m_iStamina; }
	_int&		Get_MaxStamina() { return m_iMaxStamina; }
	_float&		Get_Mp() { return m_iMp; }
	_float&		Get_MaxMp() { return m_iMaxMp; }
	_int&		Get_MainWeapon() { return m_iMainWeapon; }
	_int&		Get_SubWeapon() { return m_iSubWeapon;}
	_double&	Get_Rage() { return m_dbRage; }
	_double&	Get_MaxRage() { return m_dbMaxRage; }

public:
	// CObserver을(를) 통해 상속됨
	virtual void Update(int iMessage, void * pData) override;

public:
	static CPlayerObserver* Create();

private:
	_vec3					m_vPos;	
	_vec3					m_vLook;
	_int					m_iHp;
	_int					m_iPrevHp;
	_int					m_iMaxHp;
	_int					m_iSuperArmor;
	_int					m_iMaxSuperArmor;
	//스태미나, mp -> player 클래스 내부에 존재
	_int					m_iStamina;
	_int					m_iMaxStamina;
	_float					m_iMp;
	_float					m_iMaxMp;
	_double					m_dbRage;
	_double					m_dbMaxRage;
	//무기 종류 -> baseplayer에서 이넘으로 구분중이지만 여기서는 int로 받아와서 뿌릴예정
	_int					m_iMainWeapon;
	_int					m_iSubWeapon;

public:
	virtual void Free();

};


