#pragma once

#include "ToolGameObject.h"


class CToolNaviMeshObjectVertex : public CToolGameObject
{
private:
	explicit CToolNaviMeshObjectVertex(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolNaviMeshObjectVertex();

public:
	LPD3DXMESH				Get_Sphere() { return m_pSphere; }
	_bool					Get_Pick() { return m_bIsPick; }

	void					Set_Pick(_bool bIspIck) { m_bIsPick = bIspIck; }

public:
	// CToolGameObject을(를) 통해 상속됨
	virtual HRESULT		Ready_GameObject(_vec3 vPos);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component();

private:
	LPD3DXMESH				m_pSphere;
	D3DXCOLOR				m_tColor;
	_bool					m_bIsPick;
public:
	static CToolNaviMeshObjectVertex* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void	Free();
};

