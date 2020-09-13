#include "stdafx.h"
#include "TwohandSword.h"
#include "TSDecoration.h"

CTwohandSword::CTwohandSword(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_pParentBoneMatrix(nullptr),
	m_pParentWorldMatrix(nullptr),
	m_eCombatState(STATE_END),
	m_pTSDecoration(nullptr),
	m_bDissolve(false),
	m_dDissolveCountDown(2),
	m_iPass(0)
{
	//ZeroMemory(m_vEditPos, sizeof(_vec3));
}

CTwohandSword::~CTwohandSword()
{
}

HRESULT CTwohandSword::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_vEditPos = { 0.5f,-9.f,2.5f };
	m_pDynamicMeshCom->Set_AnimationSet(1);
	m_pTransformCom->Rotation(Engine::ROT_Y, 90.f);

	m_eUltimateState = ULTIMATE_OFF;

	return S_OK;
}

_int CTwohandSword::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh* pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::MESH, Engine::ID_STATIC));

		const Engine::D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_Spine_R");
		if (nullptr == pFrame)
			return Engine::ERROR_EXIT;

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		Engine::CTransform*			pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::TRANSFORM, Engine::ID_DYNAMIC));
		if (nullptr == pPlayerTransformCom)
			return Engine::ERROR_EXIT;

		m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();

		m_pTSDecoration = CTSDecoration::Create(m_pGraphicDev, &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f), &_vec3(1.f, 1.f, 1.f), m_wstrPlayerName, m_wstrWeaponName);
	}

	CDynamicMeshObject::Update_GameObject(dTimeDelta);

	for (_uint i = SPHERE_FIRST; i < SPHERE_END; ++i)
	{
		for (auto& iter : m_vecCollSphere[i])
			iter->Update_Component(dTimeDelta);
	}

	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * (*m_pParentWorldMatrix)));
	
	m_pTransformCom->Set_Info(&m_vEditPos, Engine::INFO_POS);
	//m_pTransformCom->Rotation(Engine::ROT_Y, 180.f);

	m_pTSDecoration->Set_CombatState(CTSDecoration::COMBAT_STATE(m_eCombatState));
	m_pTSDecoration->Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CTwohandSword::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);

	m_pTSDecoration->LateUpdate_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CTwohandSword::Render_Geometry(const _double & dTimeDelta)
{
	//Change_WeaponState(m_eCombatState);

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

void CTwohandSword::Render_PostEffect(const _double & dTimeDelta)
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

void CTwohandSword::Render_Shadow(const _double & dTimeDelta)
{
	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta, 100);
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

HRESULT CTwohandSword::Clone_Component()
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

	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_TS"));
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

HRESULT CTwohandSword::Setup_ShaderProps(LPD3DXEFFECT& pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	pEffect->SetVector("vChangeColor", &_vec4(0.7f, 0.7f, 0.7f, 1.f));
	Engine::SetTexture(pEffect, "g_DissolveEdgeTexture", 2);
	return S_OK;
}

void CTwohandSword::Change_WeaponState(COMBAT_STATE eState)
{
	if (m_eCombatState == STATE_WAIT)
	{
		Engine::CDynamicMesh* pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::MESH, Engine::ID_STATIC));

		const Engine::D3DXFRAME_DERIVED* pFrame = nullptr;

		pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_Spine_R");
		if (nullptr == pFrame)
			return;

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;


		Engine::CTransform*			pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::TRANSFORM, Engine::ID_DYNAMIC));
		if (nullptr == pPlayerTransformCom)
			return;

		m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();

		m_pDynamicMeshCom->Set_AnimationSet(1);

		if(nullptr==m_pTSDecoration)
			m_pTSDecoration = CTSDecoration::Create(m_pGraphicDev, &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f), &_vec3(1.f, 1.f, 1.f), m_wstrPlayerName, m_wstrWeaponName);
	}

	else if (m_eCombatState == STATE_COMBAT)
	{
		Engine::CDynamicMesh* pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::MESH, Engine::ID_STATIC));

		const Engine::D3DXFRAME_DERIVED* pFrame = nullptr;

		pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_R");
		if (nullptr == pFrame)
			return;

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;


		Engine::CTransform*			pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::TRANSFORM, Engine::ID_DYNAMIC));
		if (nullptr == pPlayerTransformCom)
			return;

		m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();

		m_pDynamicMeshCom->Set_AnimationSet(0);
		if (nullptr == m_pTSDecoration)
			m_pTSDecoration = CTSDecoration::Create(m_pGraphicDev, &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f), &_vec3(1.f, 1.f, 1.f), m_wstrPlayerName, m_wstrWeaponName);
	}
}

void CTwohandSword::Set_TSDecoRender(_bool bBool)
{
	m_pTSDecoration->Set_UltimateRender(bBool);
	m_eUltimateState = ULTIMATE_ON;
}

void CTwohandSword::Set_UltimateNoDead()
{
	if (m_pTSDecoration->Get_NoDead()) //무적상태이면
	{
		m_pTSDecoration->Set_NoDead(false);
		m_pTSDecoration->Set_UltimateRender(false);
		m_eUltimateState = ULTIMATE_OFF;
	}
	else //무적상태 아니면
	{
		m_pTSDecoration->Set_NoDead(true);
		m_eUltimateState = ULTIMATE_ON;
	}
}

void CTwohandSword::Reset_Dissolve()
{
	m_bDissolve = false;
	m_dDissolveCountDown = 2;
	m_iPass = 0;
}

CTwohandSword * CTwohandSword::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPlayerName, wstring wstrWeaponName)
{
	CTwohandSword* pInstance = new CTwohandSword(pGraphicDev);

	pInstance->Set_Name(wstrPlayerName, wstrWeaponName);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTwohandSword::Free()
{
	Engine::Safe_Release(m_pTSDecoration);
	CDynamicMeshObject::Free();
}
