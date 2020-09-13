#include "stdafx.h"
#include "Hands.h"


CHands::CHands(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_iHandsNum(1),
	m_iHandsIndex(0)
{
}


CHands::~CHands()
{
}

HRESULT CHands::Change_Hands(_uint iHands, _uint iCurrentTrack, _double dTrackTime, _uint iAnimationNum)
{
	m_pDynamicMeshCom = m_vecHands[iHands];
	m_mapComponent[Engine::ID_STATIC][Engine::MESH] = m_vecHands[iHands];
	//m_pDynamicMeshCom->GetAniCtrl()->Reset_Animation();
	m_pDynamicMeshCom->Set_AnimationSet(iAnimationNum);

	m_iHandsIndex = iHands;
	return S_OK;
}

HRESULT CHands::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _uint iHandsNum)
{
	Load_Hands();
	if (FAILED(Clone_Component(iHandsNum)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pDynamicMeshCom->Set_AnimationSet(1);
	m_iHandsIndex = iHandsNum;
	return S_OK;
}

_int CHands::Update_GameObject(const _double & dTimeDelta, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _uint iAnimationNum)
{
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	
	m_pDynamicMeshCom->Set_AnimationSet(iAnimationNum);

	CDynamicMeshObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CHands::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return	Engine::NO_EVENT;
}

void CHands::Render_Geometry(const _double & dTimeDelta)
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

void CHands::Render_PostEffect(const _double & dTimeDelta)
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

//void CHands::Render_Shadow(const _double & dTimeDelta)
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

HRESULT CHands::Clone_Component(_uint iHandsNum)
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

	pComponent = m_pDynamicMeshCom = m_vecHands[iHandsNum];
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

HRESULT CHands::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

HRESULT CHands::Load_Hands()
{
	wstring wstrHandsOriID = L"Mesh_Hands";
	Engine::CDynamicMesh* pDynamicMesh = nullptr;
	for (_uint i = 0; i < m_iHandsNum; ++i)
	{
		wstring wstrHandsID = wstrHandsOriID + to_wstring(i);
		pDynamicMesh = static_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrHandsID));
		m_vecHands.emplace_back(pDynamicMesh);
	}
	return S_OK;
}

CHands * CHands::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _uint iHandsNum)
{
	CHands* pInstance = new CHands(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale, iHandsNum)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CHands::Free()
{
	for (_uint i = 0; i < m_iHandsNum; ++i)
	{
		if (i == m_iHandsIndex)
			continue;
		Engine::Safe_Release(m_vecHands[i]);
	}
	m_vecHands.clear();
	CDynamicMeshObject::Free();

}
