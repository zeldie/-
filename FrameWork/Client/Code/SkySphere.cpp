#include "stdafx.h"
#include "SkySphere.h"


CSkySphere::CSkySphere(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev)
{
}


CSkySphere::~CSkySphere()
{
}

HRESULT CSkySphere::Ready_GameObject(SKY_TYPE eSkyType)
{
	if (FAILED(Clone_Component(eSkyType)))
		return E_FAIL;
	m_eSkyType = eSkyType;
	return S_OK;
}

_int CSkySphere::Update_GameObject(const _double & dTimeDelta)
{
	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	if(m_eSkyType == SKY_LOBBY)
		m_pTransformCom->Set_Pos(matCamWorld._41, matCamWorld._42 - 5.f, matCamWorld._43);
	else
		m_pTransformCom->Set_Pos(matCamWorld._41, matCamWorld._42, matCamWorld._43);
	CStaticMeshObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CSkySphere::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);
	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CSkySphere::Render_Geometry(const _double & dTimeDelta)
{
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
		if (FAILED(pEffect->SetTexture("g_DiffuseTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_DIFFUSE, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_NormalTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_NORMAL, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_SpecularTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_SPECULAR, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_EmmisiveTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_EMMISIVE, i))))
			return;
		pEffect->BeginPass(0);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

HRESULT CSkySphere::Clone_Component(SKY_TYPE eSkyType)
{
	Engine::CComponent* pComponent = nullptr;

	wstring wstrSkyKey = L"";
	switch (eSkyType)
	{
	case CSkySphere::SKY_LOBBY:
		wstrSkyKey = L"Mesh_Sky_Lobby";
		break;
	case CSkySphere::SKY_BOSSSTAGE:
		wstrSkyKey = L"Mesh_Sky_BossStage";
		break;
	case CSkySphere::SKY_CARTEL:
		wstrSkyKey = L"Mesh_Sky_Cartel";
		break;
	case CSkySphere::SKY_SHOOTINGSTAGE:
		wstrSkyKey = L"Mesh_Sky_ShootingStage";
		break;
	case CSkySphere::SKY_RUNGAME:
		wstrSkyKey = L"Mesh_RunGame_Sky";
		break;
	case CSkySphere::SKY_END:
		break;
	default:
		break;
	}
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrSkyKey));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Sky"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;

}

HRESULT CSkySphere::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	switch (m_eSkyType)
	{
	case CSkySphere::SKY_LOBBY:
		pEffect->SetVector("vChangeColor", &_vec4(0.5f, 0.5f, 0.5f, 1.f));
		break;
	case CSkySphere::SKY_BOSSSTAGE:
		break;
	case CSkySphere::SKY_CARTEL:
		pEffect->SetVector("vChangeColor", &_vec4(0.5f, 0.5f, 0.5f, 1.f));
		break;
	case CSkySphere::SKY_SHOOTINGSTAGE:
		pEffect->SetVector("vChangeColor", &_vec4(0.5f, 0.5f, 0.5f, 1.f));
		break;
	case CSkySphere::SKY_RUNGAME:
		pEffect->SetVector("vChangeColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
	case CSkySphere::SKY_END:
		break;
	default:
		break;
	}
	return S_OK;
}

CSkySphere * CSkySphere::Create(LPDIRECT3DDEVICE9 pGraphicDev, SKY_TYPE eSkyType)
{
	CSkySphere* pInstance = new CSkySphere(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eSkyType)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CSkySphere::Free()
{
	CStaticMeshObject::Free();
}
