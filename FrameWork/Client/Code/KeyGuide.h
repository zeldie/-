#pragma once
#ifndef __KeyGuide_h__
#define __KeyGuide_h__

#include "UIObject.h"
class CKeyGuide : public CUIObject
{
public:
	explicit CKeyGuide(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKeyGuide();

public:
	HRESULT				Ready_GameObject(Engine::KEYGUIDE eKeyType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	Engine::KEYGUIDE    m_eKeyType;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CKeyGuide*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::KEYGUIDE eKeyType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void			Free();
};

#endif