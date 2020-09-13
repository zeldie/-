#pragma once

#include "StaticMeshObject.h"

class CStainedGlass : public CStaticMeshObject
{
private:
	explicit CStainedGlass(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStainedGlass();

public:
	HRESULT			Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, _bool bUseLightShaft, _uint iLightShaftNum);
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual void	Render_Depth(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta)override;
	virtual void	Render_PostEffect(const _double& dTimeDelta)override;
	virtual void	Render_Shadow(const _double& dTimeDelta)override;
private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect, _uint iSubset);

private:
	_bool			m_bUseLightShaft;
	_uint			m_iLightShaftNum;
public:
	static	CStainedGlass*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _bool bUseLightShaft, _uint iLightShaftNum);
	virtual void			Free();
};

