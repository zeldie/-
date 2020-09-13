#pragma once
#ifndef __CustomizingSideHairUI_h__
#define __CustomizingSideHairUI_h__

#include "UIObject.h"
class CCustomizingSideHairUI : public CUIObject
{
public:
	enum SIDEHAIRTYPE { SIDEHAIR0, SIDEHAIR1, SIDEHAIR2, SIDEHAIR3, SIDEHAIRTYPE_END };

public:
	explicit CCustomizingSideHairUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingSideHairUI();

public:
	HRESULT				Ready_GameObject(SIDEHAIRTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	SIDEHAIRTYPE			m_eSideHairType; //CustomizingSelectBox의 인덱스번호와 같을예정
	_bool					m_bChoose;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingSideHairUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, SIDEHAIRTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ = 0.f);
	virtual void						Free();
};

#endif