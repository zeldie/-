#pragma once
#ifndef __CustomizingFootUI_h__
#define __CustomizingFootUI_h__

#include "UIObject.h"
class CCustomizingFootUI : public CUIObject
{
public:
	enum FOOTTYPE{FOOT0, FOOT1, FOOT2, FOOT3, FOOTTYPE_END};

public:
	explicit CCustomizingFootUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingFootUI();

public:
	HRESULT				Ready_GameObject(FOOTTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	FOOTTYPE			m_eFootType; //CustomizingSelectBox의 인덱스번호와 같을예정
	_bool				m_bChoose;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingFootUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, FOOTTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void					Free();
};

#endif