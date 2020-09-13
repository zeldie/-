#pragma once
#ifndef __CustomizingTopUI_h__
#define __CustomizingTopUI_h__

#include "UIObject.h"
class CCustomizingTopUI : public CUIObject
{
public:
	enum TOPTYPE{TOP0, TOP1, TOP2, TOP3, TOPTYPE_END};

public:
	explicit CCustomizingTopUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingTopUI();

public:
	HRESULT				Ready_GameObject(TOPTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	TOPTYPE				m_eTopType; //CustomizingSelectBox의 인덱스번호와 같을예정
	_bool				m_bChoose;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingTopUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TOPTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void					Free();
};

#endif