#include "stdafx.h"
#include "Terrain.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CStaticMeshObject(pGraphicDev),
	m_pNaviMeshCom(nullptr),
	m_fMipMap(1.f),
	m_eTerrainType(TERRAIN_END)
{

}

CTerrain::~CTerrain()
{

}


HRESULT CTerrain::Ready_GameObject(TERRAIN_TYPE eTerrainType)
{
	if (FAILED(Clone_Component(eTerrainType)))
		return E_FAIL;
	m_eTerrainType = eTerrainType;
	return S_OK;
}

_int CTerrain::Update_GameObject(const _double& dTimeDelta)
{
	CStaticMeshObject::Update_GameObject(dTimeDelta);

	return Engine::NO_EVENT;
}

_int CTerrain::LateUpdate_GameObject(const _double& dTimeDelta)
{
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CTerrain::Render_Depth(const _double & dTimeDelta)
{
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();
	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	for (_ulong i = 0; i < dwSubset; ++i)
	{
		pEffect->CommitChanges();

		pEffect->BeginPass(9);
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();

	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CTerrain::Render_Geometry(const _double& dTimeDelta)
{
#ifdef _DEBUG
	if(g_RenderNaviMesh)
		m_pNaviMeshCom->Render_NaviMesh();
#endif // !_DEBUG
	if (g_RenderNaviMesh)
		m_pNaviMeshCom->Render_NaviMesh();
	
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();
	Engine::SetUp_OnShader(pEffect, Engine::SHADOW_PLAYER, "g_ShadowTexture");
	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
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
		Engine::SetUp_OnShader(pEffect, Engine::SHADOW_PLAYER, "g_ShadowTexture");
		pEffect->BeginPass(16);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
	
}

HRESULT CTerrain::Clone_Component(TERRAIN_TYPE eTerrainType)
{
	Engine::CComponent*		pComponent = nullptr;

	//Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	wstring wstrTerrainName = L"";
	wstring wstrNaviMeshName = L"";
	switch (eTerrainType)
	{
	case CTerrain::TERRAIN_LOBBY:
		m_pTransformCom->Set_Scale(15.f, 10.f, 15.f);
		wstrTerrainName = L"Mesh_Grass_Plan_01";
		wstrNaviMeshName = L"Mesh_Navi_Lobby";
		break;
	case CTerrain::TERRAIN_CARTEL:
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformCom->Set_Info(&_vec3(0.f, 53.f, 0.f), Engine::INFO_POS);
		wstrTerrainName = L"Mesh_WaterGround";
		wstrNaviMeshName = L"Mesh_Navi_Cartel"; 
		m_fMipMap = 10.f;
		break;
	case CTerrain::TERRAIN_BOSSSTAGE:
		m_pTransformCom->Set_Scale(0.5f, 1.f, 0.5f);
		m_pTransformCom->Set_Info(&_vec3(0.f, 53.f, 0.f), Engine::INFO_POS);
		wstrTerrainName = L"Mesh_SoB_BaseFloor01";
		wstrNaviMeshName = L"Mesh_Navi_Boss";
		break;
	case CTerrain::TERRAIN_SHOOTINGSTAGE:
		//m_pTransformCom->Set_Scale(2.f, 0.1f, 2.f);
		m_pTransformCom->Set_Scale(0.1f, 0.1f, 0.1f);
		m_pTransformCom->Set_Info(&_vec3(0.f, 53.f, 0.f), Engine::INFO_POS);
		wstrTerrainName = L"Mesh_Ground_Tile_2";
		wstrNaviMeshName = L"Mesh_Navi_Apostle";
		break;
	default:
		break;
	}

	// Mesh
	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrTerrainName));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Renderer
	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	//NaviMesh
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrNaviMeshName));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::NAVI, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);
	return S_OK;
}

HRESULT CTerrain::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	pEffect->SetFloat("g_fTerrainMipMap", m_fMipMap);
	switch (m_eTerrainType)
	{
	case CTerrain::TERRAIN_LOBBY:
		pEffect->SetVector("vChangeColor", &_vec4(0.8f, 0.8f, 0.8f, 1.f));
		break;
	case CTerrain::TERRAIN_CARTEL:
		pEffect->SetVector("vChangeColor", &_vec4(0.5f, 0.5f, 0.5f, 1.f));
		break;
	case CTerrain::TERRAIN_BOSSSTAGE:
		pEffect->SetVector("vChangeColor", &_vec4(0.5f, 0.5f, 0.5f, 1.f));
		break;
	case CTerrain::TERRAIN_SHOOTINGSTAGE:
		pEffect->SetVector("vChangeColor", &_vec4(0.5f, 0.5f, 0.5f, 1.f));
		break;
	case CTerrain::TERRAIN_END:
		break;
	default:
		break;
	}
	
	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, TERRAIN_TYPE eTerrainType)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eTerrainType)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	CStaticMeshObject::Free();
}