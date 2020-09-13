#pragma once

#include "ToolGameObject.h"
#include "StaticMesh.h"

class CToolGameStaticMeshObject : public CToolGameObject
{
protected:
	explicit CToolGameStaticMeshObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolGameStaticMeshObject();

protected:
	// CToolGameObject을(를) 통해 상속됨
	virtual HRESULT		Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _vec3* vSelfRotate, _uint iSect);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

protected:
	HRESULT				Clone_Component(wstring wstrObjectKey);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	HRESULT				Self_Rotation();

public:
	_uint				Get_Sect() { return m_iSect; }

	void				Set_iSect(_uint iSect) { m_iSect = iSect; }

protected:
	Engine::CStaticMesh*		m_pStaticMeshCom;
	_uint						m_iSect;
public:
	static CToolGameStaticMeshObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _vec3* vSelfRotate = &_vec3(0.f, 0.f, 0.f), _uint iSect = 0);
	virtual void	Free();
};

