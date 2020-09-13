#include "stdafx.h"
#include "Flag.h"


CFlag::CFlag(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev),
	m_bLateInit(true),
	m_bRender(true),
	m_bReCreate(false),
	m_bOneCheck_Dead(true)
{
}


CFlag::~CFlag()
{
}

HRESULT CFlag::Ready_GameObject(wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	if (FAILED(Clone_Component(wstrObjectKey)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	return S_OK;
}

_int CFlag::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{
		if (m_bOneCheck_Dead)
		{
			//죽으면 렌더하는게 false가 되서 안보이는 상태.
			m_bRender = false;

			//깃발 획득 성공 문구 생성
			CUIMgr::GetInstance()->CreateGetFlagSuccessPopUp(m_pGraphicDev);
			m_bOneCheck_Dead = false;
		}

		m_bIsDead = false;
	}

	CStaticMeshObject::Update_GameObject(dTimeDelta);

	//처음 생성될때
	if (m_bLateInit)
	{
		if (CAiMgr::GetInstance()->Get_CartelStart())
		{
			//깃발위치 나타내는 표시 
			CUIMgr::GetInstance()->CheckFlagPos(m_pGraphicDev, m_pTransformCom);
			m_bLateInit = false;
		}
	}

	//재생성될때
	if (CUIMgr::GetInstance()->Get_ReCreateFlag()) //flagnofuncobj안에서 셋해줌
	{
		//다시 생성될때 필요한문구->"생성완료"문구
		CUIMgr::GetInstance()->CreateReCreateFlagPopUp(m_pGraphicDev);

		//깃발위치 나타내는 표시 
		CUIMgr::GetInstance()->CheckFlagPos(m_pGraphicDev, m_pTransformCom);

		//깃발 점령 취소상태로 셋
		CUIMgr::GetInstance()->Set_GetFlag(false);

		//깃발 재생성상태 취소상태로 셋
		CUIMgr::GetInstance()->Set_ReCreateFlag(false);

		//렌더해서 보이게
		m_bRender = true;
		m_bOneCheck_Dead = true;


		m_bReCreate = false;
	}


	return Engine::NO_EVENT;
}

_int CFlag::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);

	if (CUIMgr::GetInstance()->Get_GetFlag())
	{
		//깃발이 점령된 상태이다?
		//스페이스바 ui 렌더 ->false
		CUIMgr::GetInstance()->SetRender_SpaceBar(false);
		m_bIsDead = true;
	}
	else
	{
		if (CUIMgr::UITYPE_CARTEL == CUIMgr::GetInstance()->Get_UIType())
			CheckPlayerDist();

		if (CUIMgr::UITYPE_RESULT_Cartel == CUIMgr::GetInstance()->Get_UIType())
			CUIMgr::GetInstance()->SetRender_SpaceBar(false);
	}

	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return Engine::NO_EVENT;
}

void CFlag::Render_Depth(const _double & dTimeDelta)
{
	//SHADER
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
		pEffect->BeginPass(9);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CFlag::Render_Geometry(const _double & dTimeDelta)
{
	//SHADER
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
		pEffect->BeginPass(17);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}


void CFlag::Render_Shadow(const _double & dTimeDelta)
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

void CFlag::CheckPlayerDist()
{
	_vec3 vPlayerPos;
	vPlayerPos = *dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"RealPlayer", Engine::TRANSFORM, Engine::ID_DYNAMIC))->Get_Info(Engine::INFO_POS);

	_float fDist = Engine::GetDistNoY(vPlayerPos, m_pTransformCom->m_vInfo[Engine::INFO_POS]);

	if (200.f >= fDist)
	{
		CUIMgr::GetInstance()->SetRender_SpaceBar(true);
	}
	else
	{
		CUIMgr::GetInstance()->SetRender_SpaceBar(false);
	}
}

HRESULT CFlag::Clone_Component(wstring wstrObjectKey)
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
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	//Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone(Engine::RESOURCE_STATIC, L"OptimizationCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::OPTIMIZATION, pComponent);

	return S_OK;
}

HRESULT CFlag::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

CFlag * CFlag::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CFlag* pInstance = new CFlag(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(wstrObjectKey, pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);

	// %%%%%%%% AI %%%%%%%%%
	CAiMgr::GetInstance()->Set_Flag(pInstance);
	// %%%%%%%% AI %%%%%%%%%

	return pInstance;
}

void CFlag::Free()
{
	CStaticMeshObject::Free();
}
