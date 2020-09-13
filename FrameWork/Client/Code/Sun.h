#pragma once

#include "StaticMeshObject.h"
class CSun : public CStaticMeshObject
{
private:
	explicit CSun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSun();

public:
	virtual HRESULT		Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;
	virtual void		Render_PostEffect(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CSun*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void			Free();
};

