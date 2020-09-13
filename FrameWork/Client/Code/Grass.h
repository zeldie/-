#pragma once

#include "StaticMeshObject.h"
#define GRASSCNT 3
class CGrass : public CStaticMeshObject
{

private:
	explicit CGrass(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGrass();

public:
	HRESULT			Ready_GameObject();
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual void	Render_Depth(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta)override;
	virtual void	Render_PostEffect(const _double& dTimeDelta)override;
	virtual void	Render_Shadow(const _double& dTimeDelta)override;
private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:

public:
	static	CGrass*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free();
};

