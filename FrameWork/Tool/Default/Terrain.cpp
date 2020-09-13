#include "stdafx.h"
#include "Terrain.h"
#include "Texture.h"
#include "StaticMesh.h"
CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CToolGameObject(pGraphicDev),
	m_pStaticMeshCom(nullptr),
	m_pTextureCom(nullptr)
{

}

CTerrain::~CTerrain(void)
{

}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	CToolGameObject::Free();
}

HRESULT CTerrain::Ready_GameObject(void)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	//lobby
	//m_pTransformCom->Set_Scale(10.f, 10.f, 10.f);
	//m_fMipMap = 1.f;
	//
	//boss
	//m_pTransformCom->Set_Scale(0.5f, 1.f, 0.5f);
	//m_pTransformCom->Set_Info(&_vec3(0.f,53.f,0.f),Engine::INFO_POS); 
	//m_fMipMap = 1.f;
	//

	////Cartel
	//m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	//m_pTransformCom->Set_Info(&_vec3(0.f, 53.f, 0.f), Engine::INFO_POS);
	//m_fMipMap = 10.f;

	//Run
	m_pTransformCom->Set_Scale(50.f, 1.f, 50.f);
	m_pTransformCom->Set_Info(&_vec3(0.f, 53.f, 0.f), Engine::INFO_POS);
	m_fMipMap = 10.f;

	return S_OK;
}

_int CTerrain::Update_GameObject(const _double& dTimeDelta)
{
	CToolGameObject::Update_GameObject(dTimeDelta);

	return Engine::NO_EVENT;
}

_int CTerrain::LateUpdate_GameObject(const _double& dTimeDelta)
{
	CToolGameObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CTerrain::Render_Geometry(const _double& dTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();

	_uint iPassMax = 0;

	if (m_bIsWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for (_ulong i = 0; i < dwSubset; ++i)
	{
		if (FAILED(pEffect->SetTexture("g_DiffuseTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_DIFFUSE, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_NormalTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_NORMAL, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_SpecularTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_SPECULAR, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_EmmisiveTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_EMMISIVE, i))))
			return;
		pEffect->CommitChanges();
		pEffect->Begin(&iPassMax, 0);
		//pEffect->BeginPass(0);
		pEffect->BeginPass(16);
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
		pEffect->End();
	}
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Engine::Safe_Release(pEffect);
}

HRESULT CTerrain::Clone_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	//lobby
	//pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_Grass_Plan_01"));  
	//
	//boss
	//pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_SoB_BaseFloor01"));
	//
	//Cartel
	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_WaterGround"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	//Renderer
	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CTerrain::Setup_ShaderProps(LPD3DXEFFECT& pEffect)
{
	CToolGameObject::Set_ShaderMatrix(pEffect);
	pEffect->SetFloat("g_fTerrainMipMap", m_fMipMap);

	return S_OK;
}
