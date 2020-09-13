#pragma once

#include "Observer.h"

BEGIN(Engine)
class CTransform;
END

class CAI_Enemy2_Observer : public Engine::CObserver
{
public:
	enum MESSAGE { HP, PREVHP, MAXHP, SUPERARMOR, MAXSUPERARMOR, MAINWEAPON, SUBWEAPON, MESSAGE_END };

private:
	explicit CAI_Enemy2_Observer();
	virtual ~CAI_Enemy2_Observer();

public:
	//Getter
	_int&		Get_Hp() { return m_iHp; }
	_int&		Get_PrevHp() { return m_iPrevHp; }
	_int&		Get_MaxHp() { return m_iMaxHp; }
	_int&		Get_SuperArmor() { return m_iSuperArmor; }
	_int&		Get_MaxSuperArmor() { return m_iMaxSuperArmor; }
	_int&		Get_MainWeapon() { return m_iMainWeapon; }
	_int&		Get_SubWeapon() { return m_iSubWeapon;}

public:
	// CObserver��(��) ���� ��ӵ�
	virtual void Update(int iMessage, void * pData) override;

public:
	static CAI_Enemy2_Observer* Create();

private:
	_int					m_iHp;
	_int					m_iPrevHp;
	_int					m_iMaxHp;
	_int					m_iSuperArmor;
	_int					m_iMaxSuperArmor;
	//���� ���� -> baseplayer���� �̳����� ������������ ���⼭�� int�� �޾ƿͼ� �Ѹ�����
	_int					m_iMainWeapon;
	_int					m_iSubWeapon;

public:
	virtual void Free();

};


