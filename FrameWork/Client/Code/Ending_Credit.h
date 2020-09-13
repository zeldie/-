#pragma once

#include "NormalObject.h"

class CEnding_Credit : public CNormalObject
{
private:
	explicit CEnding_Credit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEnding_Credit();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	_matrix				m_matProj; //직교투영행렬

public:
	static CEnding_Credit* Create(LPDIRECT3DDEVICE9 pGraphicDev, float X, float Y);
	virtual void Free() override;
};

