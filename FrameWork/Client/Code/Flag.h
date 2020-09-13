#pragma once

#include "StaticMeshObject.h"
class CFlag : public CStaticMeshObject
{
private:
	explicit CFlag(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlag();

public:
	void Set_ReCreate(_bool bBool) { m_bReCreate = bBool; }

public:
	HRESULT			Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual	_int	Update_GameObject(const _double& dTimeDelta) override;
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Depth(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta)override;
	virtual void	Render_Shadow(const _double& dTimeDelta)override;

	_bool			Get_isDead() { return m_bIsDead; }

private:
	void			CheckPlayerDist();

private:
	_bool			m_bLateInit;
	_bool			m_bRender;
	_bool			m_bReCreate;
	_bool			m_bOneCheck_Dead;

private:
	HRESULT			Clone_Component(wstring wstrObjectKey);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
public:
	static	CFlag*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void			Free();
};

