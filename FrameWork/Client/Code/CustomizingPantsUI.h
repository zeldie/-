#pragma once
#ifndef __CustomizingPantsUI_h__
#define __CustomizingPantsUI_h__

#include "UIObject.h"
class CCustomizingPantsUI : public CUIObject
{
public:
	enum PANTSTYPE{PANTS0, PANTS1, PANTS2, PANTS3, PANTSTYPE_END};

public:
	explicit CCustomizingPantsUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingPantsUI();

public:
	HRESULT				Ready_GameObject(PANTSTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	PANTSTYPE			m_ePantsType; //CustomizingSelectBox의 인덱스번호와 같을예정
	_bool				m_bChoose;
	_bool				m_bCheckDouble;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingPantsUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, PANTSTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void					Free();
};

#endif