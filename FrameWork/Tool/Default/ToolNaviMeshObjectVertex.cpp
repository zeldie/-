#include "stdafx.h"
#include "ToolNaviMeshObjectVertex.h"


CToolNaviMeshObjectVertex::CToolNaviMeshObjectVertex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CToolGameObject(pGraphicDev),
	m_pSphere(nullptr),
	m_bIsPick(false)
{
	ZeroMemory(&m_tColor, sizeof(D3DXCOLOR));
}


CToolNaviMeshObjectVertex::~CToolNaviMeshObjectVertex()
{
}

HRESULT CToolNaviMeshObjectVertex::Ready_GameObject(_vec3 vPos)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Info(&vPos, Engine::INFO_POS);
	if (FAILED(D3DXCreateSphere(m_pGraphicDev, 20.f, 10, 10, &m_pSphere, NULL)))
		return E_FAIL;
	return S_OK;
}

_int CToolNaviMeshObjectVertex::Update_GameObject(const _double& dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CToolNaviMeshObjectVertex::LateUpdate_GameObject(const _double& dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CToolNaviMeshObjectVertex::Render_Geometry(const _double& dTimeDelta)
{
	D3DMATERIAL9 mtrl;
	if (m_bIsPick)
		m_tColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	else
		m_tColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	mtrl.Emissive = m_tColor;
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetMaterial(&mtrl);
	m_pSphere->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetTexture(0, nullptr);
}

HRESULT CToolNaviMeshObjectVertex::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	// RendererCom
	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);
	return S_OK;
}

CToolNaviMeshObjectVertex * CToolNaviMeshObjectVertex::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CToolNaviMeshObjectVertex* pInstance = new CToolNaviMeshObjectVertex(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CToolNaviMeshObjectVertex::Free(void)
{
	CToolGameObject::Free();
	Engine::Safe_Release(m_pSphere);
}
