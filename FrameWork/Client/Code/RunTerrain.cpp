#include "stdafx.h"
#include "RunTerrain.h"

CRunTerrain::CRunTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CStaticMeshObject(pGraphicDev),
	m_pNaviMeshCom(nullptr),
	m_fMipMap(1.f),
	m_dMoveTime(0.29)
{

}

CRunTerrain::~CRunTerrain()
{

}


HRESULT CRunTerrain::Ready_GameObject(TERRAIN_TYPE eTerrainType)
{
	if (FAILED(Clone_Component(eTerrainType)))
		return E_FAIL;
	
	return S_OK;
}

_int CRunTerrain::Update_GameObject(const _double& dTimeDelta)
{
	CStaticMeshObject::Update_GameObject(dTimeDelta);

	return Engine::NO_EVENT;
}

_int CRunTerrain::LateUpdate_GameObject(const _double& dTimeDelta)
{
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CRunTerrain::Render_Depth(const _double & dTimeDelta)
{
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect, dTimeDelta)))
		return;
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();
	_uint iPassMax = 0;

	for (_ulong i = 0; i < dwSubset; ++i)
	{
		pEffect->CommitChanges();
		pEffect->Begin(&iPassMax, 0);
		pEffect->BeginPass(9);
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
		pEffect->End();
	}
	Engine::Safe_Release(pEffect);
}

void CRunTerrain::Render_Geometry(const _double& dTimeDelta)
{
#ifdef _DEBUG
	if(g_RenderNaviMesh)
		m_pNaviMeshCom->Render_NaviMesh();
#endif // !_DEBUG

	
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect, dTimeDelta)))
		return;
	Engine::SetUp_OnShader(pEffect, Engine::SHADOW_PLAYER, "g_ShadowTexture");
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();

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
		pEffect->BeginPass(16);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
	
}

HRESULT CRunTerrain::Clone_Component(TERRAIN_TYPE eTerrainType)
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
	case CRunTerrain::TERRAIN_LOBBY:
		m_pTransformCom->Set_Scale(10.f, 10.f, 10.f);
		wstrTerrainName = L"Mesh_Grass_Plan_01";
		wstrNaviMeshName = L"Mesh_Navi_Lobby";
		break;
	case CRunTerrain::TERRAIN_CARTEL:
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformCom->Set_Info(&_vec3(0.f, 53.f, 0.f), Engine::INFO_POS);
		wstrTerrainName = L"Mesh_WaterGround";
		wstrNaviMeshName = L"Mesh_Navi_Cartel"; 
		m_fMipMap = 10.f;
		break;
	case CRunTerrain::TERRAIN_BOSSSTAGE:
		m_pTransformCom->Set_Scale(0.5f, 1.f, 0.5f);
		m_pTransformCom->Set_Info(&_vec3(0.f, 53.f, 0.f), Engine::INFO_POS);
		wstrTerrainName = L"Mesh_SoB_BaseFloor01";
		wstrNaviMeshName = L"Mesh_Navi_Boss";
		break;
	case CRunTerrain::TERRAIN_SHOOTINGSTAGE:
		//m_pTransformCom->Set_Scale(2.f, 0.1f, 2.f);
		m_pTransformCom->Set_Scale(0.1f, 0.1f, 0.1f);
		m_pTransformCom->Set_Info(&_vec3(0.f, 53.f, 0.f), Engine::INFO_POS);
		wstrTerrainName = L"Mesh_Ground_Tile_2";
		wstrNaviMeshName = L"Mesh_Navi_Apostle";
		break;
	case CRunTerrain::TERRAIN_RUNGAME:
		//m_pTransformCom->Set_Scale(2.f, 0.1f, 2.f);
		m_pTransformCom->Set_Scale(50.f, 0.1f, 50.f);
		m_pTransformCom->Set_Info(&_vec3(0.f, 53.f, 11500.f), Engine::INFO_POS);
		wstrTerrainName = L"Mesh_RunGame_Terrain";
		wstrNaviMeshName = L"Mesh_Navi_Apostle1";
		m_fMipMap = 10.f;
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
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMeshRun*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrNaviMeshName));
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

HRESULT CRunTerrain::Setup_ShaderProps(LPD3DXEFFECT & pEffect, const _double& dTimeDelta)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	pEffect->SetFloat("g_fTerrainMipMap", m_fMipMap);

	pEffect->SetVector("vChangeColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
	//if (m_dMoveTime > 2.5)
	//	m_dMoveTime -= dTimeDelta * 0.05;
	//else if (m_dMoveTime < 0.3)
	//	m_dMoveTime += dTimeDelta * 0.05;
	////cout << m_dMoveTime << endl;
	//if (m_dMoveTime > 2.5)
	//	m_dMoveTime = 0.3;
	//pEffect->SetFloat("g_Time", m_dMoveTime);
	//pEffect->SetFloat("g_WaveHeight", 500.f);
	//pEffect->SetFloat("g_Speed", 1.f);
	//pEffect->SetFloat("g_WaveFrequency", 3.f);
	//pEffect->SetFloat("g_UVSpeed", 0.1f);

	return S_OK;
}

CRunTerrain* CRunTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, TERRAIN_TYPE eTerrainType)
{
	CRunTerrain*	pInstance = new CRunTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eTerrainType)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunTerrain::Free(void)
{
	CStaticMeshObject::Free();
}