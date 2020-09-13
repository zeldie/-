#pragma once

#include "StaticMeshObject.h"

class CDsBullet : public CStaticMeshObject
{
public:
	explicit CDsBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDsBullet();

public:
	HRESULT Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3 vTargetPos, Engine::EFFECTTYPE EffectType, OBJID eID, _bool Right);
	virtual _int Update_GameObject(const _double& dTimeDelta);
	virtual _int LateUpdate_GameObject(const _double& dTimeDelta);
	virtual void Render_Geometry(const _double& dTimeDelta);

private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect, const _double& dTimeDelta);

private:
	_vec3		 m_vDir;
	_uint		 m_iPass;
	_double		 m_dbEffectCreateTime;
	_double		 m_dbKillingTime;
	_bool		 m_bRight;
	_vec3		 m_vAngle;
	Engine::EFFECTTYPE m_eType;

public:
	static	CDsBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3 vTargetPos, Engine::EFFECTTYPE EffectType, OBJID eID, _bool Right = true);
	virtual void	Free();
};