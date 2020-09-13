#pragma once

#include "StaticMeshObject.h"
class CArchitecture : public CStaticMeshObject
{
private:
	explicit CArchitecture(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CArchitecture();

public:
	HRESULT			Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual void	Render_Depth(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta)override;
	virtual void	Render_Shadow(const _double& dTimeDelta)override;
private:
	HRESULT			Clone_Component(wstring wstrObjectKey);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	wstring			m_wstrObjectKey;
public:
	static	CArchitecture*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void			Free();
};

