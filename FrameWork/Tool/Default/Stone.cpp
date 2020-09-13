#include "stdafx.h"
#include "stone.h"

#include "Picking.h"
#include "StaticMesh.h"
#include "CubeCollider.h"
#include "Transform.h"

CStone::CStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: CToolGameObject(pGraphicDev)
	, m_vDirection(0.f, 0.f, 0.f)
{

}

CStone::~CStone(void)
{

}


void CStone::Free(void)
{
	ENGINE::CGameObject::Free();
}

HRESULT CStone::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

_int CStone::Update_GameObject(const _float& fDeltaTime)
{
	//SetUp_OnTerrain();

	ENGINE::CGameObject::Update_GameObject(fDeltaTime);

	

	return 0;
}

_int CStone::LateUpdate_GameObject(const _float & fDeltaTime)
{
	ENGINE::CGameObject::LateUpdate_GameObject(fDeltaTime);


	m_pRendererCom->Add_RenderGroup(ENGINE::RENDER_PRIORITY, this);
	return 0;
}

void CStone::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	//m_pMeshCom->Render_Meshes();

	//_matrix		matWorld;
	//m_pTransformCom->Get_WorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(ENGINE::COL_FALSE, &matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
}

HRESULT CStone::Add_Component(void)
{
	ENGINE::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<ENGINE::CStaticMesh*>(ENGINE::Clone(RESOURCE_STATIC, L"Mesh_Stone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ENGINE::ID_STATIC].emplace(L"MeshCom", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<ENGINE::CTransform*>(ENGINE::Clone_Prototype(L"TransformCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ENGINE::ID_DYNAMIC].emplace(L"TransformCom", pComponent);

	// RendererCom
	pComponent = m_pRendererCom = ENGINE::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ENGINE::ID_STATIC].emplace(L"RendererCom", pComponent);

	// Calculator
	pComponent = m_pPikcingCom= dynamic_cast<ENGINE::CPicking*>(ENGINE::Clone_Prototype(L"PickingCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ENGINE::ID_DYNAMIC].emplace(L"PickingCom", pComponent);

	// Collider
	/*pComponent = m_pColliderCom = ENGINE::CCubeCollider::Create(m_pGraphicDev,
		m_pMeshCom->Get_VtxPos(),
		m_pMeshCom->Get_VtxCnt(),
		m_pMeshCom->Get_Stride());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ENGINE::ID_STATIC].emplace(L"ColliderCom", pComponent);*/

	return S_OK;
}

void CStone::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(ENGINE::INFO_POS, &vPos);

	ENGINE::CTerrainTex*		pTerrainBufferCom = dynamic_cast<ENGINE::CTerrainTex*>(ENGINE::Get_Component(L"GameObject", L"Terrain", L"BufferCom", ENGINE::ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	_float fHeight = m_pPikcingCom->Compute_HeightOnTerrain(&vPos,
															pTerrainBufferCom->Get_VtxPos(),
															pTerrainBufferCom->Get_CntX(),
															pTerrainBufferCom->Get_CntZ(),
															pTerrainBufferCom->Get_VtxItv());

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);

}

_vec3 CStone::PickUp_OnTerrain(void)
{
	//ENGINE::CTerrainTex*		pTerrainBufferCom    = dynamic_cast<ENGINE::CTerrainTex*>(ENGINE::Get_Component(L"GameObject", L"Terrain", L"BufferCom", ENGINE::ID_STATIC));
	//ENGINE::CTransform*			pTerrainTransformCom = dynamic_cast<ENGINE::CTransform*>(ENGINE::Get_Component(L"GameObject", L"Terrain", L"TransformCom", ENGINE::ID_DYNAMIC));

	//return m_pCalculator->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
	return _vec3(0.f, 0.f, 0.f);
}


CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vecPos)
{
	CStone*	pInstance = new CStone(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		ENGINE::Safe_Release(pInstance);
	pInstance->m_pTransformCom->Set_Pos(vecPos);
	return pInstance;
}