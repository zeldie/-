#include "stdafx.h"
#include "ToolDynamicMeshObject.h"

CToolDynamicMeshObject::CToolDynamicMeshObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CToolGameObject(pGraphicDev),
	m_pDynamicMeshCom(nullptr),
	m_bPlayAnimation(false),
	m_iPlayingAniNum(0),
	m_pPickingSphere(nullptr)
{
}


CToolDynamicMeshObject::~CToolDynamicMeshObject()
{
}

HRESULT CToolDynamicMeshObject::Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale)
{
	if (FAILED(Clone_Component(wstrObjectKey)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	if (FAILED(D3DXCreateSphere(m_pGraphicDev, 20.f, 10, 10, &m_pPickingSphere, NULL)))
		return E_FAIL;
	return S_OK;
}

_int CToolDynamicMeshObject::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::Update_GameObject(dTimeDelta);
	for (auto& vecCollider : m_vecCollider)
		vecCollider->Update_Component(dTimeDelta);

	m_pDynamicMeshCom->Set_AnimationSet(m_iPlayingAniNum);
	

	return Engine::NO_EVENT;
}

_int CToolDynamicMeshObject::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CToolDynamicMeshObject::Render_Geometry(const _double & dTimeDelta)
{
	if (m_bPlayAnimation)
		m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta, 1);
	for (auto& vecCollider : m_vecCollider)
		vecCollider->Render_Collider();
	m_pPickingSphere->DrawSubset(0);
	//Shader
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;
	if (m_bIsWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(0);
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
			pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
			pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
			pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->EndPass();
	pEffect->End();
	Engine::Safe_Release(pEffect);

	
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetTexture(0, nullptr);
}

void CToolDynamicMeshObject::Set_CollTrue(_uint iColliderNum)
{
	for (auto iter = m_vecCollider.begin(); iter != m_vecCollider.end(); ++iter)
	{
		if(iter != m_vecCollider.begin()+iColliderNum)
			(*iter)->Set_ColType(Engine::COL_FALSE);
		else
			(*iter)->Set_ColType(Engine::COL_TRUE);
	}
}

HRESULT CToolDynamicMeshObject::Add_Collider(Engine::CTransform* pTransformCom, _float fRadius, const char* pFrameName, Engine::CDynamicMesh* pDynamicMeshCom)
{
	Engine::CComponent*		pComponent = nullptr;
	pComponent = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_DYNAMIC, m_pTransformCom, fRadius, pFrameName, pDynamicMeshCom);
	if (pComponent == nullptr)
		return E_FAIL;
	m_vecCollider.emplace_back(dynamic_cast<Engine::CSphereCollider*>(pComponent));
	return S_OK;
}

void CToolDynamicMeshObject::DeleteCollider(_int iColliderNum)
{
	if (iColliderNum < 0)
		return;
	Engine::Safe_Release(m_vecCollider[iColliderNum]);
	m_vecCollider.erase(m_vecCollider.begin() + iColliderNum);
}

HRESULT CToolDynamicMeshObject::Clone_Component(wstring wstrObjectKey)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrObjectKey));
	if (pComponent == nullptr)
		return E_FAIL;

	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	// Transform
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

HRESULT CToolDynamicMeshObject::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CToolGameObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

CToolDynamicMeshObject * CToolDynamicMeshObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale)
{
	CToolDynamicMeshObject* pInstance = new CToolDynamicMeshObject(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(wstrObjectKey, pPos, pAngle, pScale)))
		Engine::Safe_Release((pInstance));
	return pInstance;
}

void CToolDynamicMeshObject::Free()
{
	for (auto& vecCollider : m_vecCollider)
		Engine::Safe_Release(vecCollider);
	m_vecCollider.clear();
	Engine::Safe_Release(m_pPickingSphere);
	CToolGameObject::Free();
}
