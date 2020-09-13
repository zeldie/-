#include "stdafx.h"
#include "Tree.h"

CTree::CTree(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev),
	m_bRender(true)
{
}


CTree::~CTree()
{
}

HRESULT CTree::Ready_GameObject(wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	if (FAILED(Clone_Component(wstrObjectKey)))
		return E_FAIL;

	m_wstrObjectKey = wstrObjectKey;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);	
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	return S_OK;

}

_int CTree::Update_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CTree::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);
	//if(Culling())
	Compute_ViewZ(m_pTransformCom->Get_Info(Engine::INFO_POS));
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return Engine::NO_EVENT;
}


void CTree::Render_Geometry(const _double & dTimeDelta)
{
	//SHADER
	LPD3DXEFFECT	pEffect = nullptr;
	if (m_bRender) //논알파 -> 나무기둥
		pEffect = 	m_pMeshShaderCom->Get_EffectHandle();
	else //알파 -> 나뭇잎 , 나뭇가지
		pEffect = m_pShaderCom->Get_EffectHandle();


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

		
		if (0 == i)//나뭇가지 -> 알파
		{
			if (!m_bRender)
			{
				pEffect->BeginPass(21);
				pEffect->CommitChanges();
				m_pStaticMeshCom->Render_Meshes(i);
			}
		}
		else if (1 == i) //나무기둥 -> 논알파
		{
			if (m_bRender)
			{
				pEffect->BeginPass(0);
				pEffect->CommitChanges();
				m_pStaticMeshCom->Render_Meshes(i);
			}
		}
		else if (2 == i) //나뭇잎 -> 알파
		{
			if (!m_bRender)
			{
				pEffect->BeginPass(21);
				pEffect->CommitChanges();
				m_pStaticMeshCom->Render_Meshes(i);
				
			}
		}
		pEffect->EndPass();

	}
	pEffect->End();
	Engine::Safe_Release(pEffect);

	// 논알파 그린 후 알파 그리니까 
	// 불값을 바꿔준당
	if (m_bRender)
		m_bRender = false; 
	else
		m_bRender = true;

}


void CTree::Render_Shadow(const _double & dTimeDelta)
{
	/*LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
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
	pEffect->BeginPass(6);
	pEffect->CommitChanges();
	m_pStaticMeshCom->Render_Meshes(i);
	pEffect->EndPass();
	}

	pEffect->End();
	Engine::Safe_Release(pEffect);*/
}

HRESULT CTree::Clone_Component(wstring wstrObjectKey)
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

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrObjectKey));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	//Shader
	pComponent = m_pMeshShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER2, pComponent);

	//Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone(Engine::RESOURCE_STATIC, L"OptimizationCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::OPTIMIZATION, pComponent);

	return S_OK;

}

HRESULT CTree::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

CTree * CTree::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CTree* pInstance = new CTree(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(wstrObjectKey, pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTree::Free()
{
	CStaticMeshObject::Free();
}

