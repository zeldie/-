#pragma once
#ifndef __CustomizingFrontHairUI_h__
#define __CustomizingFrontHairUI_h__

#include "UIObject.h"
class CCustomizingFrontHairUI : public CUIObject
{
public:
	enum FRONTHAIRTYPE{FRONTHAIR0, FRONTHAIR1, FRONTHAIR2, FRONTHAIR3, FRONTHAIRTYPE_END};

public:
	explicit CCustomizingFrontHairUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingFrontHairUI();

public:
	HRESULT				Ready_GameObject(FRONTHAIRTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	FRONTHAIRTYPE			m_eFrontHairType; //CustomizingSelectBox의 인덱스번호와 같을예정
	_bool					m_bChoose;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingFrontHairUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, FRONTHAIRTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void						Free();
};

#endif