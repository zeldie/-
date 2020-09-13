#include "stdafx.h"
#include "FrontHair.h"


CFrontHair::CFrontHair(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_iFrontHairNum(4),
	m_iFrontHairIndex(0)
{
	ZeroMemory(&m_vHairColor, sizeof(_vec4));
}


CFrontHair::~CFrontHair()
{
}

HRESULT CFrontHair::Change_FrontHair(_uint iFrontHair, _uint iCurrentTrack, _double dTrackTime, _uint iAnimationNum, _double dAccTime)
{
	//m_pDynamicMeshCom = nullptr;
	//m_mapComponent[Engine::ID_STATIC][Engine::MESH] = nullptr;
	m_pDynamicMeshCom = m_vecFrontHair[iFrontHair];
	m_mapComponent[Engine::ID_STATIC][Engine::MESH] = m_vecFrontHair[iFrontHair];
	//m_pDynamicMeshCom->GetAniCtrl()->Reset_Animation();
	m_pDynamicMeshCom->Set_AnimationSet(iAnimationNum);

	//m_pDynamicMeshCom->GetAniCtrl()->Set_AccTime(dAccTime);
	//m_iAnimationNum = iAnimationNum;
	//m_pDynamicMeshCom->Set_AnimationSet(iAnimationNum);
	//m_pDynamicMeshCom->GetAniCtrl()->Set_TrackTime(iCurrentTrack, dAccTime);

	//m_pDynamicMeshCom->GetAniCtrl()->Reset_Animation();

	m_iFrontHairIndex = iFrontHair;
	return S_OK;
}

HRESULT CFrontHair::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _uint iFronHairNum)
{
	Load_FrontHair();
	if (FAILED(Clone_Component(iFronHairNum)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pDynamicMeshCom->Set_AnimationSet(1);
	m_iFrontHairIndex = iFronHairNum;
	return S_OK;
}

_int CFrontHair::Update_GameObject(const _double & dTimeDelta, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _uint iAnimationNum)
{
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);

	m_pDynamicMeshCom->Set_AnimationSet(iAnimationNum);

	CDynamicMeshObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CFrontHair::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return	Engine::NO_EVENT;
}

void CFrontHair::Render_Geometry(const _double & dTimeDelta)
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
			pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
			pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
			pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
			pEffect->BeginPass(18);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CFrontHair::Render_PostEffect(const _double & dTimeDelta)
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

//void CFrontHair::Render_Shadow(const _double & dTimeDelta)
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

HRESULT CFrontHair::Clone_Component(_uint iFrontHairNum)
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

	pComponent = m_pDynamicMeshCom = m_vecFrontHair[iFrontHairNum];
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

HRESULT CFrontHair::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	pEffect->SetVector("vDyeColor", &_vec4(m_vHairColor));
	return S_OK;
}

HRESULT CFrontHair::Load_FrontHair()
{
	wstring wstrFrontHairOriID = L"Mesh_FrontHair";
	Engine::CDynamicMesh* pDynamicMesh = nullptr;
	for (_uint i = 0; i < m_iFrontHairNum; ++i)
	{
		wstring wstFrontHairID = wstrFrontHairOriID + to_wstring(i);
		pDynamicMesh = static_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstFrontHairID));
		m_vecFrontHair.emplace_back(pDynamicMesh);
	}
	return S_OK;
}

CFrontHair * CFrontHair::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _uint iFrontHairNum)
{
	CFrontHair* pInstance = new CFrontHair(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale, iFrontHairNum)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CFrontHair::Free()
{
	for (_uint i = 0; i < m_iFrontHairNum; ++i)
	{
		if (i == m_iFrontHairIndex)
			continue;
		Engine::Safe_Release(m_vecFrontHair[i]);
	}
	m_vecFrontHair.clear();
	CDynamicMeshObject::Free();

}
