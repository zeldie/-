#pragma once
#ifndef __WeaponUI_h__
#define __WeaponUI_h__

#include "UIObject.h"
class CWeaponUI : public CUIObject
{
public:
	enum WEAPONUITYPE{WEAPONUI_LB, WEAPONUI_ORB, WEAPONUI_FX, WEAPONUI_END}; //WEAPONUI_FX->¿Ã∆Â∆Æ

public:
	explicit CWeaponUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWeaponUI();

public:
	HRESULT				Ready_GameObject(WEAPONUITYPE eType,_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	WEAPONUITYPE		m_eWeaponUIType;
	_float				m_fFrame;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CWeaponUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, WEAPONUITYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void			Free();
};

#endif