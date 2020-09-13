#include "stdafx.h"
#include "Orb.h"

COrb::COrb(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_bIsCombat(false),
	m_pParentBoneMatrix(nullptr),
	m_pParentWorldMatrix(nullptr),
	m_eState(WAIT),
	m_bDissolve(false),
	m_dDissolveCountDown(6),
	m_iPass(0)
{
	//ZeroMemory(m_vEditPos, sizeof(_vec3));
}

COrb::~COrb()
{
}

HRESULT COrb::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_vEditPos = { 25.5f,-100.f,-0.5f };
	m_pDynamicMeshCom->Set_AnimationSet(1);
	m_pTransformCom->Rotation(Engine::ROT_Y, 180.f);
	if (m_bItem)
	{
		//m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	}

	return S_OK;
}

_int COrb::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	
	if (m_bItem)
	{
		CGameObject* pPlayer = Engine::Get_GameObject(Engine::GAMEOBJECT, L"RealPlayer");
		_vec3 vPlayerPos = dynamic_cast<CPlayer*>(pPlayer)->GetPlayerPos();
		float fDist = Engine::GetDistNoY(vPlayerPos, *m_pTransformCom->Get_Info(Engine::INFO_POS));
		
		if (fDist < 50)
		{
			CUIMgr::GetInstance()->Set_StageTimeStop();
			CUIMgr::GetInstance()->Set_CreateBossResultUI(true);
			m_bIsDead = true;
		}

		m_pTransformCom->Set_Info(&m_vItemPos, Engine::INFO_POS);
	}

	else if (!m_bItem)
	{
		if (nullptr == m_pParentBoneMatrix)
		{
			Engine::CDynamicMesh* pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::MESH, Engine::ID_STATIC));

			const Engine::D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_Pelvis_R");
			if (nullptr == pFrame)
				return Engine::ERROR_EXIT;

			m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;


			Engine::CTransform*	pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::TRANSFORM, Engine::ID_DYNAMIC));
			if (nullptr == pPlayerTransformCom)
				return Engine::ERROR_EXIT;

			m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();
		}
	}

	CDynamicMeshObject::Update_GameObject(dTimeDelta);

	if (!m_bItem)
		m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * (*m_pParentWorldMatrix)));

	//m_pTransformCom->Rotation(Engine::ROT_Y, 180.f);


	if (m_pDynamicMeshCom->Is_AnimationSetFinish())
	{
		m_pDynamicMeshCom->Set_AnimationSet(WAIT);
	}
	
	if (m_bItem)
		m_pTransformCom->Set_Info(&m_vItemPos, Engine::INFO_POS);
	else if (!m_bItem)
		m_pTransformCom->Set_Info(&m_vEditPos, Engine::INFO_POS);

	return Engine::NO_EVENT;
}

_int COrb::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);

	if (m_bNoRender)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void COrb::Render_Geometry(const _double & dTimeDelta)
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

	CDynamicMeshObject::Render_Geometry(dTimeDelta);
}

void COrb::Render_PostEffect(const _double & dTimeDelta)
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

void COrb::Render_Shadow(const _double & dTimeDelta)
{
	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta);
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

HRESULT COrb::Clone_Component()
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

	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_OB"));
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

HRESULT COrb::Setup_ShaderProps(LPD3DXEFFECT& pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	Engine::SetTexture(pEffect, "g_DissolveEdgeTexture", 23);
	return S_OK;
}

void COrb::Set_OB_Animation(ORB_STATE eState)
{
	m_pDynamicMeshCom->Set_AnimationSet(eState);
}

void COrb::Reset_Dissolve()
{
	m_bDissolve = false;
	m_dDissolveCountDown = 2;
	m_iPass = 0;
}

COrb * COrb::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPlayerName, wstring wstrWeaponName)
{
	COrb* pInstance = new COrb(pGraphicDev);

	pInstance->Set_Name(wstrPlayerName, wstrWeaponName);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

COrb * COrb::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	COrb* pInstance = new COrb(pGraphicDev);

	pInstance->Set_Item(true);
	pInstance->Set_ItemPos(vPos);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void COrb::Free()
{
	CDynamicMeshObject::Free();
}
