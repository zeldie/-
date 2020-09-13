#include "stdafx.h"
#include "ToolNaviMeshObject.h"

CToolNaviMeshObject::CToolNaviMeshObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CToolGameObject(pGraphicDev),
	m_pBufferCom(nullptr),
	m_eNaviMeshType(Engine::NAVIMESHTYPE_END),
	m_iNum(0),
	m_dwColor(0),
	m_bSorting(false),
	m_bIsPick(false),
	m_iOption(0)
{
}


CToolNaviMeshObject::~CToolNaviMeshObject()
{
}

HRESULT CToolNaviMeshObject::Ready_GameObject(Engine::NAVIMESHTYPE eNaviMeshType)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_eNaviMeshType = eNaviMeshType;
	return S_OK;
}

_int CToolNaviMeshObject::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CToolNaviMeshObject::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::LateUpdate_GameObject(dTimeDelta);
	if (m_vecVertex.size() == 3)
	{
		Sorting();
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	}
	return Engine::NO_EVENT;
}

void CToolNaviMeshObject::Render_Geometry(const _double & dTimeDelta)
{
	Set_BufferPos();
	Set_Color();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	_vec3		vPoint[4];

	vPoint[0] = *m_vecVertex[0]->Get_Transform()->Get_Info(Engine::INFO_POS);
	vPoint[1] = *m_vecVertex[1]->Get_Transform()->Get_Info(Engine::INFO_POS);
	vPoint[2] = *m_vecVertex[2]->Get_Transform()->Get_Info(Engine::INFO_POS);
	vPoint[3] = *m_vecVertex[0]->Get_Transform()->Get_Info(Engine::INFO_POS);

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 4; ++i)
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
	Engine::Get_Line()->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(0.f, 1.f, 1.f, 1.f));

	Engine::Get_Line()->End();
	

}

void CToolNaviMeshObject::Set_BufferPos()
{
	Engine::VTXCOL*		pVertex = nullptr;

	m_pBufferCom->Get_VB()->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = *m_vecVertex[0]->Get_Transform()->Get_Info(Engine::INFO_POS);

	pVertex[1].vPos = *m_vecVertex[1]->Get_Transform()->Get_Info(Engine::INFO_POS);

	pVertex[2].vPos = *m_vecVertex[2]->Get_Transform()->Get_Info(Engine::INFO_POS);

	m_pBufferCom->Get_VB()->Unlock();
}

void CToolNaviMeshObject::Set_Color()
{
	Engine::VTXCOL*		pVertex = nullptr;

	m_pBufferCom->Get_VB()->Lock(0, 0, (void**)&pVertex, 0);

	switch (m_eNaviMeshType)
	{
	case Engine::NONE:
		m_dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		break;
	case Engine::TRIGGER:
		m_dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		break;
	case Engine::EVENT:
		m_dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
		break;
	case Engine::NAVIMESHTYPE_END:
		break;
	default:
		break;
	}
	if (m_bIsPick)
	{
		pVertex[0].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
		pVertex[1].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
		pVertex[2].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	}
	else
	{
		pVertex[0].dwColor = m_dwColor;
		pVertex[1].dwColor = m_dwColor;
		pVertex[2].dwColor = m_dwColor;

	}
	
	m_pBufferCom->Get_VB()->Unlock();
}

void CToolNaviMeshObject::Sorting()
{
	_vec3 vPos0 = *(m_vecVertex[0]->Get_Transform()->Get_Info(Engine::INFO_POS));
	_vec3 vPos1 = *(m_vecVertex[1]->Get_Transform()->Get_Info(Engine::INFO_POS));
	_vec3 vPos2 = *(m_vecVertex[2]->Get_Transform()->Get_Info(Engine::INFO_POS));

	_vec3 vDir0 = Engine::GetDir(vPos1, vPos0);
	vDir0.y = 0.f;
	_vec3 vDir1 = Engine::GetDir(vPos2, vPos1);
	vDir1.y = 0.f;
	_vec3 vDir2 = Engine::GetDir(vPos0, vPos2);
	vDir2.y = 0.f;
	_vec3 vCross;
	D3DXVec3Cross(&vCross,&vDir0, &vDir1);
	if (vCross.y < 0)
		m_bSorting = true;
}

_uint CToolNaviMeshObject::Get_VertexNum()
{
	if (m_vecVertex.empty())
		return 0;
	return _uint(m_vecVertex.size());
}

void CToolNaviMeshObject::Add_Vertex(CToolNaviMeshObjectVertex * MeshObjectVertex)
{
	if (MeshObjectVertex == nullptr)
		return;
	if (m_vecVertex.size() >= 3)
		return;
	m_vecVertex.emplace_back(MeshObjectVertex);
}

HRESULT CToolNaviMeshObject::Clone_Component()
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


CToolNaviMeshObject * CToolNaviMeshObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::NAVIMESHTYPE eNaviMeshType)
{
	CToolNaviMeshObject* pInstance = new CToolNaviMeshObject(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eNaviMeshType)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CToolNaviMeshObject::Free(void)
{
	CToolGameObject::Free();
	for (auto& vecVertex : m_vecVertex)
		vecVertex->Set_Dead();
}
