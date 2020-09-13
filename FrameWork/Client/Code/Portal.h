#pragma once

#include "StaticMeshObject.h"
class CPortal : public CStaticMeshObject
{
private:
	explicit CPortal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPortal();

public:
	HRESULT			Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual	void	Render_Geometry(const _double& dTimeDelta);
	virtual void	Render_PostEffect(const _double& dTimeDelta);
private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
private:
	_double			m_dDistortionAcctime;
public:
	static	CPortal*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void			Free();
};

