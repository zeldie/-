#pragma once
#ifndef __BattleImg_h__
#define __BattleImg_h__

#include "UIObject.h"
class CBattleImg : public CUIObject
{
public:
	enum BATTLEIMGTYPE{ BATTLEIMGTYPE_CIRCLE, BATTLEIMGTYPE_BATTLE, BATTLEIMGTYPE_END};

public:
	explicit CBattleImg(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBattleImg();
	
public:
	HRESULT				Ready_GameObject(BATTLEIMGTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	BATTLEIMGTYPE		m_eBattleImgType;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CBattleImg*	Create(LPDIRECT3DDEVICE9 pGraphicDev, BATTLEIMGTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif