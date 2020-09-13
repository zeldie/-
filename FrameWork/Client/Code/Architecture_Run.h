#pragma once

#include "StaticMeshObject.h"
class CArchitecture_Run : public CStaticMeshObject
{
private:
	explicit CArchitecture_Run(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CArchitecture_Run();

public:
	HRESULT			Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _vec3* vSelfRotate, RUN_LINE_SECT eSect);
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual void	Render_Depth(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta)override;
	virtual void	Render_Shadow(const _double& dTimeDelta)override;
private:
	HRESULT			Clone_Component(wstring wstrObjectKey);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	RUN_LINE_SECT			m_eSect;

public:
	static	CArchitecture_Run*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _vec3* vSelfRotate = &_vec3(0.f, 0.f, 0.f), RUN_LINE_SECT eSect = SECT_1);
	virtual void			Free();
};

