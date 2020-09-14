#include "stdafx.h"
#include "Portal.h"


CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev),
	m_dDistortionAcctime(0.)
{
}


CPortal::~CPortal()
{
}

HRESULT CPortal::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	return S_OK;
}

_int CPortal::Update_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::Update_GameObject(dTimeDelta);
	m_dDistortionAcctime += dTimeDelta;
	return Engine::NO_EVENT;
}

_int CPortal::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	return Engine::NO_EVENT;
}

void CPortal::Render_Geometry(const _double & dTimeDelta)
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
		if (FAILED(pEffect->SetTexture("g_DiffuseTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_DIFFUSE, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_NormalTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_NORMAL, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_SpecularTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_SPECULAR, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_EmmisiveTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_EMMISIVE, i))))
			return;
		pEffect->BeginPass(11);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CPortal::Render_PostEffect(const _double & dTimeDelta)
{
}

HRESULT CPortal::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_Portal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);


	return S_OK;
}

HRESULT CPortal::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect); 
	Engine::SetTexture(pEffect, "g_DistortionTexture", 6);
	pEffect->SetFloat("fDistortionWeight", 0.2f);
	pEffect->SetFloat("fDistortionUV", _float(m_dDistortionAcctime * 0.2));
	Engine::SetTexture(pEffect, "g_DistortionColorTexture", 15);
	//pEffect->SetVector("vDistortionColor", &_vec4(0.58f, 0.97f, 0.99f, 1.f));
	return S_OK;
}

CPortal * CPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CPortal* pInstance = new CPortal(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPortal::Free()
{
	CStaticMeshObject::Free();
}
