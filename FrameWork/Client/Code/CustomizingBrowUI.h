#pragma once
#ifndef __CustomizingBrowUI_h__
#define __CustomizingBrowUI_h__

#include "UIObject.h"
class CCustomizingBrowUI : public CUIObject
{
public:
	enum BROWTYPE{BROW0, BROW1, BROW2, BROW3, BROWTYPE_END};

public:
	explicit CCustomizingBrowUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingBrowUI();

public:
	HRESULT				Ready_GameObject(BROWTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	BROWTYPE			m_eBrowType; //CustomizingSelectBox의 인덱스번호와 같을예정
	_bool				m_bChoose;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingBrowUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, BROWTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void					Free();
};

#endif