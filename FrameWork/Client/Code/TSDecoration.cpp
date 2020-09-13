#include "stdafx.h"
#include "TSDecoration.h"


CTSDecoration::CTSDecoration(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev),
	m_pParentBoneMatrix(nullptr),
	m_pParentWorldMatrix(nullptr),
	m_bUltimateRender(false),
	m_dUltimateTime(0.f),
	m_bNoDead(false)
{
}


CTSDecoration::~CTSDecoration()
{
}

HRESULT CTSDecoration::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, wstring wstrPlayerName, wstring wstrWeaponName)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_vEditPos = { 0.5f,-9.f,2.5f };
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_wstrPlayerName = wstrPlayerName;
	m_wstrWeaponName = wstrWeaponName;
	m_eCombatState = STATE_WAIT;
	return S_OK;
}

_int CTSDecoration::Update_GameObject(const _double & dTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh* pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::MESH, Engine::ID_STATIC));

		const Engine::D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName("Weapon_R");
		if (nullptr == pFrame)
			return Engine::ERROR_EXIT;

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		Engine::CTransform*			pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrPlayerName, Engine::TRANSFORM, Engine::ID_DYNAMIC));
		if (nullptr == pPlayerTransformCom)
			return Engine::ERROR_EXIT;

		m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();
	}

	Change_WeaponState(m_eCombatState);
	
	CStaticMeshObject::Update_GameObject(dTimeDelta);

	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * (*m_pParentWorldMatrix)));
	
	m_pTransformCom->Set_Info(&m_vEditPos, Engine::INFO_POS);

	if (m_bUltimateRender)
	{
		if(!m_bNoDead)
			m_dUltimateTime += dTimeDelta;

	}
	else
	{
		if (m_bNoDead) //¹«Àû °è¼Ó±Ã±Ø±â»óÅÂ
		{
			m_bUltimateRender = true;
			m_dUltimateTime = 0.f;
		}
		else
		{
			m_bUltimateRender = false;
			m_dUltimateTime = 0.f;
		}
	}

	return Engine::NO_EVENT;
}

_int CTSDecoration::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);

	if (m_bUltimateRender)
	{
		//20ÃÊ µ¿¾È ±Ã±Ø±â »óÅÂ À¯Áö -> ±×ÈÄ ´Ù½Ã ±Ã±Ø±â »óÅÂ off
		if (20.f <= m_dUltimateTime)
		{
			m_dUltimateTime = 0.f;
			m_bUltimateRender = false;
			
			dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, m_wstrWeaponName))->Set_UltimateState(CTwohandSword::ULTIMATE_OFF);
		}
	}
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CTSDecoration::Render_Geometry(const _double & dTimeDelta)
{
	if (m_bUltimateRender)
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
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
			pEffect->BeginPass(0);
			pEffect->CommitChanges();

			m_pStaticMeshCom->Render_Meshes(i);
			pEffect->EndPass();
		}
		
		pEffect->End();
		Engine::Safe_Release(pEffect);

	}
}


void CTSDecoration::Change_WeaponState(COMBAT_STATE eState)
{
	if (m_eCombatState == STATE_WAIT || STATE_END == m_eCombatState)
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

		m_pTransformCom->Set_Angle(15.f,0.f,200.f);		
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
		m_pTransformCom->Set_Angle(0.f, 0.f, 0.f);		
	}

}

HRESULT CTSDecoration::Clone_Component()
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

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_TSDecoration"));
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

HRESULT CTSDecoration::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

CTSDecoration * CTSDecoration::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, wstring wstrPlayerName, wstring wstrWeaponName)
{
	CTSDecoration* pInstance = new CTSDecoration(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale, wstrPlayerName, wstrWeaponName)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTSDecoration::Free()
{
	CStaticMeshObject::Free();
}
