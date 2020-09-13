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
	_float				m_fCoolDownSpeed; //쿨타임 스피드
	_double				m_dTime; // 쓸수없는 스킬슬롯을 다시 쓸수있게 돌릴 때 시간재는거야 
	
	_bool				m_bCoolDowning; //쿨다운중인지 아닌지 판별하여 쿨다운시작하게할 변수
	_bool				m_bSwitchRenderUI; // 무기 바뀌면서 그릴지 말지 변경해주는 변수
	_bool				m_bRendering; //그려지고 있는 것들 중에서 (그리는 중이다 안그리는중이다)를 판단

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ=0.f);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CSkillSlot*	Create(LPDIRECT3DDEVICE9 pGraphicDev, WEAPONTYPE eWeaponType, SKILLSLOT eType, Engine::KEYGUIDE eKeyType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif