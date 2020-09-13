#pragma once
#ifndef __SkillSlot_h__
#define __SkillSlot_h__

#include "UIObject.h"
class CSkillSlot : public CUIObject
{
public:
	enum SKILLSLOT {BASIC, RECHARGE, OUTLINE, SKILLSLOT_END};
	enum WEAPONTYPE{MAIN, SUB, WEAPONTYPE_END};

public:
	explicit CSkillSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkillSlot();

public:
	//Getter
	Engine::KEYGUIDE&	Get_KeyType() { return m_eKeyType; }
	SKILLSLOT&			Get_SkillSlotType() { return m_eSkillSlotType; }
	_bool				Get_CoolDowning() { return m_bCoolDowning; }
	WEAPONTYPE&			Get_WeaponType() { return m_eWeaponType; }

	//Setter
	void				Set_CoolDown(_bool _bBool = true) { m_bCoolDown = _bBool; }
	void				Set_CoolDowning(_bool _bBool=true) { m_bCoolDowning = _bBool; }
	void				Set_SwitchRenderUI(_bool _bBool = true) { m_bSwitchRenderUI = _bBool; }
	void				Set_Rendering(_bool _bBool = true) { m_bRendering = _bBool; }

public:
	HRESULT				Ready_GameObject(WEAPONTYPE eWeaponType, SKILLSLOT eType, Engine::KEYGUIDE eKeyType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	SKILLSLOT			m_eSkillSlotType;
	WEAPONTYPE			m_eWeaponType;
	Engine::KEYGUIDE	m_eKeyType;

	_float				m_fResultValue;
	_bool				m_bCoolDown; 
	_float				m_fCoolDownSpeed; //��Ÿ�� ���ǵ�
	_double				m_dTime; // �������� ��ų������ �ٽ� �����ְ� ���� �� �ð���°ž� 
	
	_bool				m_bCoolDowning; //��ٿ������� �ƴ��� �Ǻ��Ͽ� ��ٿ�����ϰ��� ����
	_bool				m_bSwitchRenderUI; // ���� �ٲ�鼭 �׸��� ���� �������ִ� ����
	_bool				m_bRendering; //�׷����� �ִ� �͵� �߿��� (�׸��� ���̴� �ȱ׸������̴�)�� �Ǵ�

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ=0.f);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CSkillSlot*	Create(LPDIRECT3DDEVICE9 pGraphicDev, WEAPONTYPE eWeaponType, SKILLSLOT eType, Engine::KEYGUIDE eKeyType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif