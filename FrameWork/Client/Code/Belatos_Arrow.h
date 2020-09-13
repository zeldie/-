#pragma once

#include "StaticMeshObject.h"
#include "BasePlayer.h"
class CBelatos_Arrow : public CStaticMeshObject
{
public:
	enum ARROW_TYPE
	{
		BASIC_ARROW,
		RAIN_ARROW,
		STORM_ARROW,
		ARROW_END
	};
public:
	explicit CBelatos_Arrow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBelatos_Arrow();

public:
	void			Move(const _double& dTimeDelta);
public:
	HRESULT			Ready_GameObject(ARROW_TYPE eArrowType, _vec3* pPos, _vec3* pAngle, _float fStopPos, _vec3 vTargetPos, CONTROLTYPE eControlType);
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual	void	Render_Geometry(const _double& dTimeDelta);
	
private:
	HRESULT			Clone_Component(ARROW_TYPE eArrowType);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect, const _double& dTimeDelta);

private:
	ARROW_TYPE		m_eArrowType;
	_vec3			m_vDir;
	_float			m_fStopPos;
	_uint			m_iPass;
	_bool			m_bIsDissolve;
	_double			m_dDissolveCountDown;
	_bool			m_bCreateEffect;
	CONTROLTYPE		m_eControlType;

	_float			m_fLifeTime;
public:
	static	CBelatos_Arrow*	Create(LPDIRECT3DDEVICE9 pGraphicDev, ARROW_TYPE eArrowType, _vec3* pPos, _vec3* pAngle, _float fStopPos, _vec3 vTargetPos, CONTROLTYPE eControlType);
	virtual void	Free();
};

