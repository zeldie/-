#include "stdafx.h"
#include "DualSword.h"

CDualSword::CDualSword(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_eCurCombat(COMBAT_STATE::STATE_WAIT),
	m_eNextCombat(COMBAT_STATE::STATE_WAIT),
	m_pParentBoneMatrix(nullptr),
	m_pParentWorldMatrix(nullptr),
	m_bHide(false),
	m_bDissolve(false),
	m_dDissolveCountDown(2),
	m_iPass(0)
{
	//ZeroMemory(m_vEditPos, sizeof(_vec3));
}

CDualSword::~CDualSword()
{
}

HRESULT CDualSword::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_vEditPos = { 0.5f,-9.f,2.5f };
	// 1ÀÌ Wait»óÅÂ
	m_pDynamicMeshCom->Set_AnimationSet(1);
	m_pTransformCom->Rotation(Engine::ROT_Y, 90.f);
	//m_pTransformCom->Rotation(Engine::ROT_X, 180.f);

	return S_OK;
}

_int CDualSword::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	if (nullptr == m_pParentBoneMatrix)
	{
		m_eCurCombat = m_eNextCombat;

		Engine::CDynamicMesh* pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::MESH, Engine::ID_STATIC));

		const Engine::D3DXFRAME_DERIVED* pFrame = nullptr;

		if (m_eHand == Engine::HAND_RIGHT)
		{
			//pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_R");
			if (m_eCurCombat == CDualSword::COMBAT_STATE::STATE_COMBAT)
				pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_R");
			else if (m_eCurCombat == CDualSword::COMBAT_STATE::STATE_WAIT)
				pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_Spine_R");
		}
		else if (m_eHand == Engine::HAND_LEFT)
		{
			if (m_eCurCombat == CDualSword::COMBAT_STATE::STATE_COMBAT)
				pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_L");
			else if (m_eCurCombat == CDualSword::COMBAT_STATE::STATE_WAIT)
				pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_Spine_L");
		}

		if (nullptr == pFrame)
			return Engine::ERROR_EXIT;

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;


		Engine::CTransform*			pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::TRANSFORM, Engine::ID_DYNAMIC));
		if (nullptr == pPlayerTransformCom)
			return Engine::ERROR_EXIT;

		m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();
	}

	if (m_eCurCombat != m_eNextCombat)
	{
		m_eCurCombat = m_eNextCombat;

		Engine::CDynamicMesh* pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::MESH, Engine::ID_STATIC));

		const Engine::D3DXFRAME_DERIVED* pFrame = nullptr;

		if (m_eHand == Engine::HAND_RIGHT)
		{
			if (m_eCurCombat == CDualSword::COMBAT_STATE::STATE_COMBAT)
				pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_R");
			else if (m_eCurCombat == CDualSword::COMBAT_STATE::STATE_WAIT)
				pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_Spine_R");
		}
		else if (m_eHand == Engine::HAND_LEFT)
		{
			if (m_eCurCombat == CDualSword::COMBAT_STATE::STATE_COMBAT)
				pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_L");
			else if (m_eCurCombat == CDualSword::COMBAT_STATE::STATE_WAIT)
				pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_Spine_L");
		}

		if (nullptr == pFrame)
			return Engine::ERROR_EXIT;

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;


		Engine::CTransform*			pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::TRANSFORM, Engine::ID_DYNAMIC));
		if (nullptr == pPlayerTransformCom)
			return Engine::ERROR_EXIT;

		m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();
	}

	CDynamicMeshObject::Update_GameObject(dTimeDelta);
	
	

	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * (*m_pParentWorldMatrix)));

	
	//m_pTransformCom->Set_Info(&m_vEditPos, Engine::INFO_POS);
	//m_pTransformCom->Rotation(Engine::ROT_Y, 180.f);

	return Engine::NO_EVENT;
}

_int CDualSword::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);

	if (m_bNoRender)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CDualSword::Render_Geometry(const _double & dTimeDelta)
{
	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta, 0);

	//Shader
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
		if (m_dDissolveCountDown < 0)
			Reset_Dissolve();
	}
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
			pEffect->BeginPass(m_iPass);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CDualSword::Render_PostEffect(const _double & dTimeDelta)
{
	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta, 0);

	if (!m_bDissolve)
	{
		//Shader
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
				pEffect->SetTexture("g_DiffuseTexture", nullptr);
				pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
				pEffect->BeginPass(1);
				pEffect->CommitChanges();
				m_pDynamicMeshCom->Render_Meshes(iter, i);
				pEffect->EndPass();
			}
			m_pDynamicMeshCom->Render_Meshes_End(iter);
		}
		pEffect->End();
		Engine::Safe_Release(pEffect);
	}
}

void CDualSword::Render_Shadow(const _double & dTimeDelta)
{
	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta, 100);
	//Shader
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	//if (FAILED(CBaseObject::Set_ShaderShadow(pEffect)))
	//	return;
	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->BeginPass(6);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CDualSword::Reset_Dissolve()
{
	m_bDissolve = false;
	m_dDissolveCountDown = 2;
	m_iPass = 0;
}

HRESULT CDualSword::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	if (m_eHand == Engine::HAND::HAND_RIGHT)
		pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_DS_R"));
	else if (m_eHand == Engine::HAND::HAND_LEFT)
		pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_DS_L"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CDualSword::Setup_ShaderProps(LPD3DXEFFECT& pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	Engine::SetTexture(pEffect, "g_DissolveEdgeTexture", 30);
	return S_OK;
}

CDualSword * CDualSword::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::HAND eHand, wstring wstrPlayerName, wstring wstrWeaponName)
{
	CDualSword* pInstance = new CDualSword(pGraphicDev);

	pInstance->Set_Hand(eHand);
	pInstance->Set_Name(wstrPlayerName, wstrWeaponName);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDualSword::Free()
{
	CDynamicMeshObject::Free();
}
