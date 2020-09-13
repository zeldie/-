#pragma once

#include "DynamicMeshObject.h"

class CLBDecoration;
class CRunWeapon_LongBow final: public CDynamicMeshObject
{
private:
	explicit CRunWeapon_LongBow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRunWeapon_LongBow();

public:
	virtual HRESULT Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_Shadow(const _double& dTimeDelta) override;

private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);


private:


public:
	static CRunWeapon_LongBow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void Free() override;
};