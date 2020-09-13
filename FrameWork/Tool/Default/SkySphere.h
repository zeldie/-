#pragma once

#include "ToolGameStaticMeshObject.h"

class CSkySphere : public CToolGameStaticMeshObject
{
public:
	enum SKY_TYPE
	{
		SKY_LOBBY,
		SKY_BOSSSTAGE,
		SKY_CARTEL,
		SKY_SHOOTINGSTAGE,
		SKY_END
	};
private:
	explicit CSkySphere(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkySphere();

public:
	HRESULT			Ready_GameObject(SKY_TYPE eSkyType);
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual	void	Render_Geometry(const _double& dTimeDelta);

private:
	HRESULT			Clone_Component(SKY_TYPE eSkyType);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
public:
	static CSkySphere* Create(LPDIRECT3DDEVICE9 pGraphicDev, SKY_TYPE eSkyType);
	virtual void Free();
};

