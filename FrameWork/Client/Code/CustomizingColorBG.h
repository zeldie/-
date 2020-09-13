#pragma once
#ifndef __CustomizingColorBG_h__
#define __CustomizingColorBG_h__

#include "UIObject.h"
class CCustomizingColorBG : public CUIObject
{
public:
	explicit CCustomizingColorBG(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingColorBG();

public:
	HRESULT				Ready_GameObject(wstring wstrType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HDC					m_hDC;
	_bool				m_bChoose;
	_vec4				m_vChangeColor;
	wstring				m_wstrType;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingColorBG*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrType,  _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void					Free();
};

#endif