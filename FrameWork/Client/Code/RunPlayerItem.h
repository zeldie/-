#pragma once
#ifndef __RunPlayerItem_h__
#define __RunPlayerItem_h__

#include "UIObject.h"
class CRunPlayerItem : public CUIObject
{
public:
	enum ITEMTYPE{BIG, SPEEDUP, ITEMTYPE_END};

public:
	explicit CRunPlayerItem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRunPlayerItem();

public:
	HRESULT				Ready_GameObject(ITEMTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	ITEMTYPE			m_eItemType;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CRunPlayerItem*		Create(LPDIRECT3DDEVICE9 pGraphicDev, ITEMTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif