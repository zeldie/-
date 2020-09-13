#pragma once

#include "Observer.h"

BEGIN(Engine)
class CTransform;
END

class CPlayerObserver : public Engine::CObserver
{
public:
	enum MESSAGE { POS, LOOK, HP, PREVHP, MAXHP, SUPERARMOR, MAXSUPERARMOR, STAMINA, MAXSTAMINA, MP, MAXMP, MAINWEAPON, SUBWEAPON, RAGE , MAXRAGE, MESSAGE_END };

	//����>UI�� �ʿ��Ѱ�: HP, ����HP, �ִ� HP, MP, Stamina, SuperAromor,����Ƚ��, �޺�Ƚ��? , 
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
	// CObserver��(��) ���� ��ӵ�
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
	//���¹̳�, mp -> player Ŭ���� ���ο� ����
	_int					m_iStamina;
	_int					m_iMaxStamina;
	_float					m_iMp;
	_float					m_iMaxMp;
	_double					m_dbRage;
	_double					m_dbMaxRage;
	//���� ���� -> baseplayer���� �̳����� ������������ ���⼭�� int�� �޾ƿͼ� �Ѹ�����
	_int					m_iMainWeapon;
	_int					m_iSubWeapon;

public:
	virtual void Free();

};


