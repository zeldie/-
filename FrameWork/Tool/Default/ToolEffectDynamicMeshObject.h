#pragma once

#include "ToolGameObject.h"
#include "DynamicMesh.h"
#include "SphereCollider.h"
class CToolEffectDynamicMeshObject : public CToolGameObject
{
public:
	explicit CToolEffectDynamicMeshObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolEffectDynamicMeshObject();

private:
	// CToolGameObject을(를) 통해 상속됨
	virtual HRESULT						Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual _int						Update_GameObject(const _double& dTimeDelta) override;
	virtual _int						LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void						Render_Geometry(const _double& dTimeDelta) override;

public:
	Engine::CDynamicMesh*				Get_DynamicMeshCom() { return m_pDynamicMeshCom; }
	_uint								Get_ColliderNum() { return _uint(m_vecCollider.size()); }
	vector<Engine::CSphereCollider*>&	Get_vecCollider() { return m_vecCollider; }
	LPD3DXMESH							Get_PickingSphere() { return m_pPickingSphere; }

	void								Set_PlayingAniNum(_uint iNum) { m_iPlayingAniNum = iNum; }
	void								Set_PlayAnimation(_bool bPlayAnimation) { m_bPlayAnimation = bPlayAnimation; }
	void								Set_CollTrue(_uint iColliderNum);

	HRESULT								Add_Collider(Engine::CTransform* pTransformCom, _float fRadius, const char* pFrameName, Engine::CDynamicMesh* pDynamicMeshCom);
	void								DeleteCollider(_int iColliderNum);
private:
	HRESULT								Clone_Component(wstring wstrObjectKey);
	HRESULT								Setup_ShaderProps(LPD3DXEFFECT& pEffect);
private:
	Engine::CDynamicMesh*				m_pDynamicMeshCom;
	_bool								m_bPlayAnimation;
	_uint								m_iPlayingAniNum; // 현재 틀어지고 있는 에니메이션 번호

	LPD3DXMESH							m_pPickingSphere;
	vector<Engine::CSphereCollider*>	m_vecCollider;
public:
	static CToolEffectDynamicMeshObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos = &_vec3(0.f,0.f, 0.f), _vec3* pAngle = &_vec3(0.f, 0.f, 0.f), _vec3* pScale = &_vec3(1.f, 1.f, 1.f));
protected:
	virtual void	Free(void);
};

