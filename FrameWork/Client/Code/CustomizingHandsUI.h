#pragma once
#ifndef __CustomizingHandsUI_h__
#define __CustomizingHandsUI_h__

#include "UIObject.h"
class CCustomizingHandsUI : public CUIObject
{
public:
	enum HANDSTYPE{HANDS0, HANDSTYPE_END};

public:
	explicit CCustomizingHandsUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingHandsUI();

public:
	HRESULT				Ready_GameObject(HANDSTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HANDSTYPE			m_eHandsType; //CustomizingSelectBox의 인덱스번호와 같을예정
	_bool				m_bChoose;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingHandsUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, HANDSTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void					Free();
};

#endif