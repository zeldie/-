#pragma once
#ifndef __PlayerMp_h__
#define __PlayerMp_h__

#include "UIObject.h"

class CPlayerObserver;

class CPlayerMp : public CUIObject
{
public:
	explicit CPlayerMp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerMp();

public:
	HRESULT				Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	//¿ÉÀú¹ö
	CPlayerObserver*		m_pPlayerObserver;

	_float					m_fMaxMp;
	_float					m_fResultValue;

	_bool					m_bLateInit;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CPlayerMp*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif