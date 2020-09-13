#include "stdafx.h"
#include "Top.h"


CTop::CTop(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_iTopNum(4),
	m_iTopIndex(0)
{
}


CTop::~CTop()
{
}

HRESULT CTop::Change_Top(_uint iTop, _uint iCurrentTrack, _double dTrackTime, _uint iAnimationNum, _double dAccTime)
{
	m_pDynamicMeshCom = m_vecTop[iTop];
	m_mapComponent[Engine::ID_STATIC][Engine::MESH] = m_vecTop[iTop];
	//m_pDynamicMeshCom->GetAniCtrl()->Reset_Animation();
	m_pDynamicMeshCom->Set_AnimationSet(iAnimationNum);
	
	//m_pDynamicMeshCom->GetAniCtrl()->Set_AnimationSet_Change(iAnimationNum, dTrackTime, dAccTime);
	//m_pDynamicMeshCom->Set_AnimationSet(0);
	//m_pDynamicMeshCom->GetAniCtrl()->Set_AnimationSet_Change(iAnimationNum, dTrackTime, dAccTime);
	//m_pDynamicMeshCom->Play_AnimationSet(0.000016);
	//m_pDynamicMeshCom->GetAniCtrl()->Set_AccTime(dAccTime);
	//m_iAnimationNum = iAnimationNum;
	//m_pDynamicMeshCom->Set_AnimationSet(iAnimationNum);
	//m_pDynamicMeshCom->GetAniCtrl()->Set_TrackTime(iCurrentTrack, dAccTime);
	
	m_iTopIndex = iTop;
	return S_OK;
}

HRESULT CTop::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _uint iTopNum)
{
	Load_Top();
	if (FAILED(Clone_Component(iTopNum)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pDynamicMeshCom->Set_AnimationSet(1);
	m_iTopIndex = iTopNum;
	return S_OK;
}

_int CTop::Update_GameObject(const _double & dTimeDelta, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iAnimationNum)
{
	
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pDynamicMeshCom->Set_AnimationSet(iAnimationNum);

	CDynamicMeshObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CTop::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return	Engine::NO_EVENT;
}

void CTop::Render_Geometry(const _double & dTimeDelta)
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

void CTop::Render_PostEffect(const _double & dTimeDelta)
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


HRESULT CTop::Clone_Component(_uint iTopNum)
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

	pComponent = m_pDynamicMeshCom = m_vecTop[iTopNum];
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

HRESULT CTop::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

HRESULT CTop::Load_Top()
{
	wstring wstrTopOriID = L"Mesh_Top";
	Engine::CDynamicMesh* pDynamicMesh = nullptr;
	for (_uint i = 0; i < m_iTopNum; ++i)
	{
		wstring wstrTopID = wstrTopOriID + to_wstring(i);
		pDynamicMesh = static_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrTopID));
		m_vecTop.emplace_back(pDynamicMesh);
	}
	return S_OK;
}

CTop * CTop::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _uint iTopNum)
{
	CTop* pInstance = new CTop(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale, iTopNum)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CTop::Free()
{
	for (_uint i = 0; i < m_iTopNum; ++i)
	{
		if (i == m_iTopIndex)
			continue;
		Engine::Safe_Release(m_vecTop[i]);
	}
	m_vecTop.clear();
	CDynamicMeshObject::Free();

}
