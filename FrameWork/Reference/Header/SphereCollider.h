#pragma once

#include "Collider.h"
#include "Transform.h"
#include "DynamicMesh.h"
BEGIN(Engine)
class ENGINE_DLL CSphereCollider : public CCollider
{
private:
	explicit CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphereCollider(const CSphereCollider& rhs);
	virtual ~CSphereCollider();

public:
	void					Get_FrameName(char* szname);
	_float					Get_Radius() { return m_fRadius; }
	_matrix					Get_WorldMatrix() { return m_pTransformCom->m_matWorld; }
	_vec3					Get_Pos();
	Engine::CTransform*		Get_TransformCom() { return m_pTransformCom; }

	void			Set_Radius(_float fScale);

public:
	HRESULT			Ready_Collider(MESHTYPE eMeshType, CTransform* pTransformCom, _float fRadius, const char* pFrameName, CDynamicMesh* pDynamicMeshCom, _vec3* pPos);
	virtual _int	Update_Component(const _double& dTimeDelta) override;
	void			Render_Collider();

private:
	LPD3DXMESH			m_pBoundingSphere;
	char				m_szFrameName[MIN_STR];
	CTransform*			m_pTransformCom;
	_float				m_fRadius;
	_float				m_fOriginRadius;
	CTransform*			m_pParentTransform;

public:
	static CSphereCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, MESHTYPE eMeshType, CTransform* pTransformCom, _float fRadius, const char* pFrameName = "", CDynamicMesh* pDynamicMeshCom = nullptr, _vec3* pPos = &_vec3(0.f, 0.f, 0.f));
	virtual	CComponent*		Clone() override;
	virtual void			Free();
};

END