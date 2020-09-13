#pragma once

#include "Collider.h"
#include "Transform.h"
#include "DynamicMesh.h"
BEGIN(Engine)
class ENGINE_DLL CCubeCollider : public CCollider
{
private:
	explicit CCubeCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeCollider(const CCubeCollider& rhs);
	virtual ~CCubeCollider();

public:
	void			Get_FrameName(char* szname);
	_matrix			Get_WorldMatrix() { return m_pTransformCom->m_matWorld; }
	_vec3			Get_Pos();
	_vec3*			Get_Min(void) { return &m_vMin; }
	_vec3*			Get_Max(void) { return &m_vMax; }
public:
	HRESULT			Ready_Collider(MESHTYPE eMeshType, CTransform* pTransformCom, const char* pFrameName, CDynamicMesh* pDynamicMeshCom, const _vec3* pPos, _ulong dwNumVtx);
	virtual _int	Update_Component(const _double& dTimeDelta) override;
	void			Render_Collider();

private:
	//LPD3DXMESH			m_pBoundingSphere;
	char				m_szFrameName[MIN_STR];
	CTransform*			m_pTransformCom;
	CTransform*			m_pParentTransform;
	_vec3				m_vMin;
	_vec3				m_vMax;

#ifdef _DEBUG
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
#endif

public:
	static CCubeCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, MESHTYPE eMeshType, CTransform* pTransformCom, _ulong dwNumVtx, const _vec3* pPos, const char* pFrameNamee = "", CDynamicMesh* pDynamicMeshCom = nullptr);
	virtual	CComponent*		Clone() override;
	virtual void			Free();
};

END