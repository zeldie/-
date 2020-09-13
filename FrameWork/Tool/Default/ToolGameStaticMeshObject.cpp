#include "stdafx.h"
#include "ToolGameStaticMeshObject.h"


CToolGameStaticMeshObject::CToolGameStaticMeshObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CToolGameObject(pGraphicDev),
	m_pStaticMeshCom(nullptr),
	m_iSect(0)
{
	
}

CToolGameStaticMeshObject::~CToolGameStaticMeshObject()
{
}

HRESULT CToolGameStaticMeshObject::Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _vec3* vSelfRotate, _uint iSect)
{
	if (FAILED(Clone_Component(wstrObjectKey)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_SelfRotAngle(vSelfRotate);
	m_iSect = iSect;
	return S_OK;
}

_int CToolGameStaticMeshObject::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::Update_GameObject(dTimeDelta);

	return Engine::NO_EVENT;
}

_int CToolGameStaticMeshObject::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::LateUpdate_GameObject(dTimeDelta);
	Self_Rotation();
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CToolGameStaticMeshObject::Render_Geometry(const _double& dTimeDelta)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();

	_uint iPassMax = 0;

	if (m_bIsWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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
		pEffect->CommitChanges();
		pEffect->Begin(&iPassMax, 0);
		pEffect->BeginPass(0);
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
		pEffect->End();
	}
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Engine::Safe_Release(pEffect);
}

HRESULT CToolGameStaticMeshObject::Clone_Component(wstring wstrObjectKey)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrObjectKey));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	//Renderer
	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CToolGameStaticMeshObject::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CToolGameObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

HRESULT CToolGameStaticMeshObject::Self_Rotation()
{
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
	return S_OK;
}

CToolGameStaticMeshObject * CToolGameStaticMeshObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _vec3* vSelfRotate, _uint iSect)
{
	CToolGameStaticMeshObject* pInstance = new CToolGameStaticMeshObject(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(wstrObjectKey, pPos, pAngle, pScale, vSelfRotate, iSect)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CToolGameStaticMeshObject::Free(void)
{
	CToolGameObject::Free();
}
