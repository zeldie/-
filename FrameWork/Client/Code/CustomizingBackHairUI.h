#pragma once
#ifndef __CustomizingBackHairUI_h__
#define __CustomizingBackHairUI_h__

#include "UIObject.h"
class CCustomizingBackHairUI : public CUIObject
{
public:
	enum BACKHAIRTYPE { BACKHAIR0, BACKHAIR1, BACKHAIR2, BACKHAIR3, BACKHAIRTYPE_END };

public:
	explicit CCustomizingBackHairUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingBackHairUI();

public:
	HRESULT				Ready_GameObject(BACKHAIRTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	BACKHAIRTYPE			m_eBackHairType; //CustomizingSelectBox의 인덱스번호와 같을예정
	_bool					m_bChoose;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingBackHairUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, BACKHAIRTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ = 0.f);
	virtual void						Free();
};

#endif