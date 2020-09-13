#pragma once

#include "DynamicMeshObject.h"
class CPhoenix : public CDynamicMeshObject
{
	enum ANIMATION
	{
		PHOENIX_UP,
		PHOENIX_MOVE
	};
private:
	explicit CPhoenix(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPhoenix();

public:
	virtual HRESULT Ready_GameObject(_vec3* pPos, _vec3* pDir, CONTROLTYPE eControlType);
	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_Shadow(const _double& dTimeDelta) override;
private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	ANIMATION		m_eAnimation;
	_vec3			m_vDir;
	CONTROLTYPE		m_eControlType;
	_double			m_dCountDown;
public:
	static CPhoenix* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pDir, CONTROLTYPE eControlType);
	virtual void Free() override;
};

