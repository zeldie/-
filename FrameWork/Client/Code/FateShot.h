#pragma once

#include "StaticMeshObject.h"
class CFateShot : public CStaticMeshObject
{
public:
	enum SKILL_TYPE
	{
		TYPE_MOVE,
		TYPE_HOLD,
		TYPE_END
	};

public:
	explicit CFateShot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFateShot();

public:
	void Move(const _double& dTimeDelta);

public:
	HRESULT			Ready_GameObject(SKILL_TYPE eSkillType, _vec3* pPos, _vec3* pAngle, _float fStopPos, _vec3 vTargetPos, OBJID eID);
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual	void	Render_Geometry(const _double& dTimeDelta);

private:
	HRESULT			Clone_Component(SKILL_TYPE eSkillType);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect, const _double& dTimeDelta);

private:
	SKILL_TYPE		m_eSkillType;
	_vec3			m_vDir;
	_float			m_fStopPos;
	_uint			m_iPass;
	//_bool			m_bIsDissolve;
	//_float		m_fDissolveCountDown;
	_bool			m_bCreateEffect;
	_double			m_dbDeadTime;
	Engine::CStaticMesh*				m_pMeshCom;
public:
	static	CFateShot*	Create(LPDIRECT3DDEVICE9 pGraphicDev, SKILL_TYPE eSkillType, _vec3* pPos, _vec3* pAngle, _float fStopPos, _vec3 vTargetPos, OBJID eID);
	virtual void	Free();
};