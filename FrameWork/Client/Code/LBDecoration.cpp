#include "stdafx.h"
#include "LBDecoration.h"


CLBDecoration::CLBDecoration(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev),
	m_pParentBoneMatrix(nullptr),
	m_pParentWorldMatrix(nullptr),
	m_bDissolve(false),
	m_dDissolveCountDown(3),
	m_iPass(0)
{
}


CLBDecoration::~CLBDecoration()
{
}

HRESULT CLBDecoration::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, wstring wstrPlayerName)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_vEditPos = { 0.5f,-9.f,2.5f };
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_wstrPlayerName = wstrPlayerName;
	return S_OK;
}

_int CLBDecoration::Update_GameObject(const _double & dTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh* pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::MESH, Engine::ID_STATIC));

		const Engine::D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_L");
		if (nullptr == pFrame)
			return Engine::ERROR_EXIT;

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;


		Engine::CTransform*			pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::TRANSFORM, Engine::ID_DYNAMIC));
		if (nullptr == pPlayerTransformCom)
			return Engine::ERROR_EXIT;

		m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();
		m_pTransformCom->Set_Info(&m_vEditPos, Engine::INFO_POS);
	}
	
	CStaticMeshObject::Update_GameObject(dTimeDelta);

	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * (*m_pParentWorldMatrix)));
	
	return Engine::NO_EVENT;
}

_int CLBDecoration::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CLBDecoration::Render_Geometry(const _double & dTimeDelta)
{
	if (!m_bDissolve)
		return;
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	if (m_bDissolve)
	{
		Engine::SetTexture(pEffect, "g_DissolveTexture", 6);
		m_dDissolveCountDown -= dTimeDelta * 5;
		pEffect->SetFloat("fTime", m_dDissolveCountDown);
		m_iPass = 14;
	}
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
		pEffect->BeginPass(m_iPass);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	
	Engine::Safe_Release(pEffect);
}


void CLBDecoration::Reset_Dissolve()
{
	m_bDissolve = false;
	m_dDissolveCountDown = 2;
	m_iPass = 0;
}

HRESULT CLBDecoration::Clone_Component()
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

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_LBDecoration"));
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

HRESULT CLBDecoration::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	pEffect->SetVector("vChangeColor", &_vec4(3.f, 3.f, 3.f, 1.f));
	return S_OK;
}

CLBDecoration * CLBDecoration::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, wstring wstrPlayerName)
{
	CLBDecoration* pInstance = new CLBDecoration(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale, wstrPlayerName)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CLBDecoration::Free()
{
	CStaticMeshObject::Free();
}
