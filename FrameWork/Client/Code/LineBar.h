#pragma once
#ifndef __LineBar_h__
#define __LineBar_h__

#include "UIObject.h"
class CLineBar : public CUIObject
{
public:
	enum LINEBARTYPE {TENLINES, FIVELINES, LINEBARTYPE_END};

public:
	explicit CLineBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLineBar();

public:
	HRESULT				Ready_GameObject(LINEBARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	LINEBARTYPE			m_eLineBarType;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CLineBar*	Create(LPDIRECT3DDEVICE9 pGraphicDev, LINEBARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif