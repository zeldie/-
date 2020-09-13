#pragma once
#ifndef __CustomizingButton_h__
#define __CustomizingButton_h__

#include "UIObject.h"
class CCustomizingButton : public CUIObject
{
public:
	enum BUTTONTYPE{CREATE1, CREATE2, BUTTONTYPE_END}; //CREATE2 ->´­·¶À»¶§.

public:
	explicit CCustomizingButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingButton();

public:
	HRESULT				Ready_GameObject(BUTTONTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	BUTTONTYPE			m_eButtonType;
	_bool				m_bChoose;


private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingButton*		Create(LPDIRECT3DDEVICE9 pGraphicDev, BUTTONTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void					Free();
};

#endif