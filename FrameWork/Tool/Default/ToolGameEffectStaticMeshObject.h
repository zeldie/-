#pragma once

#include "ToolGameObject.h"
#include "StaticMesh.h"

class CToolGameEffectStaticMeshObject : public CToolGameObject
{
private:
	explicit CToolGameEffectStaticMeshObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolGameEffectStaticMeshObject();

private:
	// CToolGameObject을(를) 통해 상속됨
	virtual HRESULT		Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component(wstring wstrObjectKey);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);
private:
	Engine::CStaticMesh*		m_pStaticMeshCom;
public:
	static CToolGameEffectStaticMeshObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void	Free();
};

