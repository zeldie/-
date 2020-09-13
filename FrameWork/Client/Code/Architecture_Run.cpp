#include "stdafx.h"
#include "Architecture_Run.h"


CArchitecture_Run::CArchitecture_Run(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev)
	, m_eSect(SECT_1)
{
}


CArchitecture_Run::~CArchitecture_Run()
{
}

HRESULT CArchitecture_Run::Ready_GameObject(wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _vec3* vSelfRotate, RUN_LINE_SECT eSect)
{
	if (FAILED(Clone_Component(wstrObjectKey)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_SelfRotAngle(vSelfRotate);
	m_eSect = eSect;
	return S_OK;
}

_int CArchitecture_Run::Update_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CArchitecture_Run::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);

	_matrix matRotateX, matRotateY, matRotateZ, matRotate;
	D3DXMatrixIdentity(&matRotateX);
	D3DXMatrixIdentity(&matRotateY);
	D3DXMatrixIdentity(&matRotateZ);
	D3DXMatrixIdentity(&matRotate);
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(m_pTransformCom->Get_SelfRotAngle()->x));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(m_pTransformCom->Get_SelfRotAngle()->y));
	D3DXMatrixRotationZ(&matRotateZ, D3DXToRadian(m_pTransformCom->Get_SelfRotAngle()->z));
	matRotate = matRotateY * matRotateX * matRotateZ;
	matRotate = matRotate * (*m_pTransformCom->Get_WorldMatrix());
	m_pTransformCom->Set_Matrix(&matRotate);
	if(Culling())
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CArchitecture_Run::Render_Depth(const _double & dTimeDelta)
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

void CArchitecture_Run::Render_Geometry(const _double & dTimeDelta)
{
	
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(0);
	m_pStaticMeshCom->Render_Meshes(pEffect);
	pEffect->EndPass();
	pEffect->End();
	////////////////////////////////////////////////////////////
	/*_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();
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
	pEffect->End();*/



	Engine::Safe_Release(pEffect);
}


void CArchitecture_Run::Render_Shadow(const _double & dTimeDelta)
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

HRESULT CArchitecture_Run::Clone_Component(wstring wstrObjectKey)
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

HRESULT CArchitecture_Run::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	//pEffect->SetFloat("g_fTerrainMipMap", 50.f);

	return S_OK;
}

CArchitecture_Run * CArchitecture_Run::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _vec3* vSelfRotate, RUN_LINE_SECT eSect)
{
	CArchitecture_Run* pInstance = new CArchitecture_Run(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(wstrObjectKey, pPos, pAngle, pScale, vSelfRotate, eSect)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CArchitecture_Run::Free()
{
	CStaticMeshObject::Free();
}
