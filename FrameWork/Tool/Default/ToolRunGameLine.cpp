#include "stdafx.h"
#include "ToolRunGameLine.h"
#include "RunTool.h"

CToolRunGameLine::CToolRunGameLine(LPDIRECT3DDEVICE9 pGraphicDev)
	: CToolGameObject(pGraphicDev)
	, m_iOption(0)
	, m_dProgressValueStart(0.)
	, m_dProgressValueEbd(0.)
{
}

CToolRunGameLine::~CToolRunGameLine()
{
}

HRESULT CToolRunGameLine::Ready_GameObject(_uint iOption)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_iOption = iOption;

	return S_OK;
}

_int CToolRunGameLine::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CToolRunGameLine::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::LateUpdate_GameObject(dTimeDelta);
	if (m_vecVertex.size() == 2)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	}
	return Engine::NO_EVENT;
}

void CToolRunGameLine::Render_Geometry(const _double & dTimeDelta)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	_vec3		vPoint[2];

	vPoint[0] = *m_vecVertex[0]->Get_Transform()->Get_Info(Engine::INFO_POS);
	vPoint[1] = *m_vecVertex[1]->Get_Transform()->Get_Info(Engine::INFO_POS);

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 2; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
		if (vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}
	Engine::Get_Line()->SetWidth(10.f);	//라인의 굵기 결정

	m_pGraphicDev->EndScene();
	m_pGraphicDev->BeginScene();

	Engine::Get_Line()->Begin();

	_matrix		matTemp;
	Engine::Get_Line()->DrawTransform(vPoint, 2, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(0.f, 1.f, 1.f, 1.f));

	Engine::Get_Line()->End();
}

HRESULT CToolRunGameLine::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	//Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CTriCol*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_TriCol"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

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

void CToolRunGameLine::Add_Vertex(CToolNaviMeshObjectVertex * MeshObjectVertex)
{
	if (MeshObjectVertex == nullptr)
		return;
	if (m_vecVertex.size() >= 2)
		return;
	m_vecVertex.emplace_back(MeshObjectVertex);
}

_uint CToolRunGameLine::Get_VertexNum()
{
	if (m_vecVertex.empty())
		return 0;
	return _uint(m_vecVertex.size());
}

CToolRunGameLine * CToolRunGameLine::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iOption)
{
	CToolRunGameLine* pInstance = new CToolRunGameLine(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(iOption)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CToolRunGameLine::Free(void)
{
	CToolGameObject::Free();
	for (auto& vecVertex : m_vecVertex)
		vecVertex->Set_Dead();
}
