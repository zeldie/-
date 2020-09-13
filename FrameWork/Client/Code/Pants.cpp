#include "stdafx.h"
#include "Pants.h"


CPants::CPants(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_iPantsNum(4),
	m_iPantsIndex(0)
{
}


CPants::~CPants()
{
}

HRESULT CPants::Change_Pants(_uint iPants, _uint iCurrentTrack, _double dTrackTime, _uint iAnimationNum)
{
	m_pDynamicMeshCom = m_vecPants[iPants];
	m_mapComponent[Engine::ID_STATIC][Engine::MESH] = m_vecPants[iPants];
	m_pDynamicMeshCom->GetAniCtrl()->Set_TrackTime(iCurrentTrack, dTrackTime);
	m_iPantsIndex = iPants;
	return S_OK;
}

HRESULT CPants::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _uint iPantsNum)
{
	Load_Pants();
	if (FAILED(Clone_Component(iPantsNum)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pDynamicMeshCom->Set_AnimationSet(1);
	m_iPantsIndex = iPantsNum;
	return S_OK;
}

_int CPants::Update_GameObject(const _double& dTimeDelta, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iAnimationNum)
{
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);

	m_pDynamicMeshCom->Set_AnimationSet(iAnimationNum);

	CDynamicMeshObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CPants::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return	Engine::NO_EVENT;
}

void CPants::Render_Geometry(const _double & dTimeDelta)
{
	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta);
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
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
			pEffect->BeginPass(0);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CPants::Render_PostEffect(const _double & dTimeDelta)
{
	////Shader
	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//if (pEffect == nullptr)
	//	return;
	//Engine::Safe_AddRef(pEffect);
	//if (FAILED(Setup_ShaderProps(pEffect)))
	//	return;
	//_uint iPassMax = 0;

	//pEffect->Begin(&iPassMax, 0);

	//list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	//for (auto& iter : *plistMeshContainer)
	//{
	//	_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
	//	m_pDynamicMeshCom->Render_Meshes_Begin(iter);
	//	for (_ulong i = 0; i < dwSubsetNum; ++i)
	//	{
	//		pEffect->SetTexture("g_DiffuseTexture", nullptr);
	//		pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
	//		pEffect->BeginPass(1);
	//		pEffect->CommitChanges();
	//		m_pDynamicMeshCom->Render_Meshes(iter, i);
	//		pEffect->EndPass();
	//	}
	//	m_pDynamicMeshCom->Render_Meshes_End(iter);
	//}
	//pEffect->End();
	//Engine::Safe_Release(pEffect);
}

//void CPants::Render_Shadow(const _double & dTimeDelta)
//{
//	////Shader
//	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
//	//if (pEffect == nullptr)
//	//	return;
//	//Engine::Safe_AddRef(pEffect);
//	//if (FAILED(Setup_ShaderProps(pEffect)))
//	//	return;
//	//_uint iPassMax = 0;
//	//pEffect->Begin(&iPassMax, 0);
//	//list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
//	//for (auto& iter : *plistMeshContainer)
//	//{
//	//	_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
//	//	m_pDynamicMeshCom->Render_Meshes_Begin(iter);
//	//	for (_ulong i = 0; i < dwSubsetNum; ++i)
//	//	{
//	//		pEffect->BeginPass(6);
//	//		pEffect->CommitChanges();
//	//		m_pDynamicMeshCom->Render_Meshes(iter, i);
//	//		pEffect->EndPass();
//	//	}
//	//	m_pDynamicMeshCom->Render_Meshes_End(iter);
//	//}
//	//pEffect->End();
//
//	//Engine::Safe_Release(pEffect);
//}

HRESULT CPants::Clone_Component(_uint iPantsNum)
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

	pComponent = m_pDynamicMeshCom = m_vecPants[iPantsNum];
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

HRESULT CPants::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

HRESULT CPants::Load_Pants()
{
	wstring wstrPantsOriID = L"Mesh_Pants";
	Engine::CDynamicMesh* pDynamicMesh = nullptr;
	for (_uint i = 0; i < m_iPantsNum; ++i)
	{
		wstring wstrPantsID = wstrPantsOriID + to_wstring(i);
		pDynamicMesh = static_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrPantsID));
		m_vecPants.emplace_back(pDynamicMesh);
	}
	return S_OK;
}

CPants * CPants::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _uint iPantsNum)
{
	CPants* pInstance = new CPants(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale, iPantsNum)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CPants::Free()
{
	for (_uint i = 0; i < m_iPantsNum; ++i)
	{
		if (i == m_iPantsIndex)
			continue;
		Engine::Safe_Release(m_vecPants[i]);
	}
	m_vecPants.clear();
	CDynamicMeshObject::Free();

}
