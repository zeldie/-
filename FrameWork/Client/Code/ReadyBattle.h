#pragma once
#ifndef __ReadyBattle_h__
#define __ReadyBattle_h__

#include "UIObject.h"
class CReadyBattle : public CUIObject
{
public:
	explicit CReadyBattle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CReadyBattle();

public:
	//Setter
	void				Set_Start() { m_bStart = true; }

public:
	HRESULT				Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	_bool				m_bStart;

	_float				m_fTime;
	_float				m_fSpeed;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect, const _double& dTimeDelta);

public:
	static CReadyBattle*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif