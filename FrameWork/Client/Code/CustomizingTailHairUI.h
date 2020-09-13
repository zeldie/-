#pragma once
#ifndef __CustomizingTailHairUI_h__
#define __CustomizingTailHairUI_h__

#include "UIObject.h"
class CCustomizingTailHairUI : public CUIObject
{
public:
	enum TAILHAIRTYPE { TAILHAIR0, TAILHAIR1, TAILHAIR2, TAILHAIR3, TAILHAIRTYPE_END };

public:
	explicit CCustomizingTailHairUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingTailHairUI();

public:
	HRESULT				Ready_GameObject(TAILHAIRTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	TAILHAIRTYPE			m_eTailHairType; //CustomizingSelectBox의 인덱스번호와 같을예정
	_bool					m_bChoose;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingTailHairUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TAILHAIRTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ = 0.f);
	virtual void						Free();
};

#endif