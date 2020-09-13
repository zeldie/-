#pragma once
#ifndef __Stamina_h__
#define __Stamina_h__

#include "UIObject.h"

class CPlayerObserver;

class CStamina : public CUIObject
{
public:
	explicit CStamina(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStamina();

public:
	HRESULT				Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	//¿ÉÀú¹ö
	CPlayerObserver*		m_pPlayerObserver;
	
	_int					m_iMaxStamina;
	_float					m_fResultValue;

	_bool					m_bLateInit;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CStamina*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif