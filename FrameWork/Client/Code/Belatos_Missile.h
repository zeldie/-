#pragma once

#include "StaticMeshObject.h"
class CBelatos_Missile : public CStaticMeshObject
{
public:
	explicit CBelatos_Missile(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBelatos_Missile();

public:
	void			Move(const _double& dTimeDelta);
public:
	HRESULT			Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3 vTargetPos);
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual	void	Render_Geometry(const _double& dTimeDelta);

private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect, const _double& dTimeDelta);

private:
	_vec3			m_vDir;
	_float			m_fStopPos;
	_uint			m_iPass;
	_bool			m_bIsDissolve;
	_float			m_fDissolveCountDown;

	const _matrix*		m_pParentBoneMatrix;
	const _matrix*		m_pParentWorldMatrix;
	_float			m_fLifeTime;
public:
	static	CBelatos_Missile*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3 vTargetPos);
	virtual void	Free();
};

