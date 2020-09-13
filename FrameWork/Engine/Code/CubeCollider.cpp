#include "CubeCollider.h"
#include "ComponentMgr.h"

USING(Engine)

CCubeCollider::CCubeCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCollider(pGraphicDev),
	//m_pBoundingSphere(nullptr),
	m_pTransformCom(nullptr)
{
	ZeroMemory(&m_szFrameName, sizeof(_char)*MIN_STR);
}

CCubeCollider::CCubeCollider(const CCubeCollider & rhs)
	:CCollider(rhs),
	//m_pBoundingSphere(rhs.m_pBoundingSphere),
	m_pTransformCom(rhs.m_pTransformCom)
{
	ZeroMemory(&m_szFrameName, sizeof(_char)*MIN_STR);
}


CCubeCollider::~CCubeCollider()
{	
}

void CCubeCollider::Get_FrameName(char * szname)
{
	strcpy_s(szname, MIN_STR, m_szFrameName);
}

_vec3 CCubeCollider::Get_Pos()
{
	_vec3 vPos;
	memcpy(&vPos, &m_pTransformCom->m_matWorld.m[3][0], sizeof(_vec3));

	return vPos;
}


HRESULT CCubeCollider::Ready_Collider(MESHTYPE eMeshType, CTransform* pTransformCom, const char* pFrameName, CDynamicMesh* pDynamicMeshCom, const _vec3* pPos, _ulong dwNumVtx)
{
	D3DXComputeBoundingBox(pPos, dwNumVtx, sizeof(_vec3), &m_vMin, &m_vMax);

	m_pTransformCom = dynamic_cast<Engine::CTransform*>(CComponentMgr::GetInstance()->Clone(RESOURCE_STATIC, L"TransformCom"));
	m_eParentMeshType = eMeshType;
	m_pParentTransform = pTransformCom;
	strcpy_s(m_szFrameName, MIN_STR, pFrameName);
	switch (m_eParentMeshType)
	{
	case Engine::TYPE_STATIC:
		m_pTransformCom->Set_Info(pPos->x, pPos->y, pPos->z, Engine::INFO_POS);
		m_pParentTransform->Get_WorldMatrix(&m_matParent);
		break;
	case Engine::TYPE_DYNAMIC:
	{
		if (pDynamicMeshCom == nullptr)
			return E_FAIL;
		const D3DXFRAME_DERIVED* pFrame = pDynamicMeshCom->Get_FrameByName(pFrameName);
		if (pFrame == nullptr)
			return E_FAIL;
		m_pmatParentBone = &pFrame->CombinedTransformationMatrix;
		if (pTransformCom == nullptr)
			return E_FAIL;
		m_pmatParentWorld = pTransformCom->Get_WorldMatrix();
		m_pTransformCom->Set_Info(pPos->x, pPos->y, pPos->z, Engine::INFO_POS);
	}

	break;
	case Engine::MESHTYPE_END:
		break;
	default:
		break;
	}




#ifdef _DEBUG
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * 8,	// 그리고자 하는 버텍스의 크기
		0,						// 버퍼 사용 방식(0인 정적 버퍼)
		FVF_CUBE,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX32) * 12,
		0,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL)))
		return E_FAIL;

	VTXCUBE*		pVtxCube = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	pVtxCube[0].vPos = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVtxCube[0].vTex = pVtxCube[0].vPos;

	pVtxCube[1].vPos = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVtxCube[1].vTex = pVtxCube[1].vPos;

	pVtxCube[2].vPos = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVtxCube[2].vTex = pVtxCube[2].vPos;

	pVtxCube[3].vPos = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVtxCube[3].vTex = pVtxCube[3].vPos;


	pVtxCube[4].vPos = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVtxCube[4].vTex = pVtxCube[4].vPos;

	pVtxCube[5].vPos = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVtxCube[5].vTex = pVtxCube[5].vPos;

	pVtxCube[6].vPos = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVtxCube[6].vTex = pVtxCube[6].vPos;

	pVtxCube[7].vPos = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVtxCube[7].vTex = pVtxCube[7].vPos;

	m_pVB->Unlock();

	INDEX32*	pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// -x
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;


	// +y
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// -y	 
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// +z
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// -z
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	for (_uint i = 0; i < COL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT			LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1.f - i), 0.f, 1.f);

		m_pTexture[i]->UnlockRect(0);
	}

#endif	

	return S_OK;
}

_int CCubeCollider::Update_Component(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return OBJ_DEAD;
	if (m_eParentMeshType == Engine::TYPE_DYNAMIC)
	{

		m_pTransformCom->Update_Component(dTimeDelta);
		D3DXMatrixIdentity(&m_matParent);
		m_matParent = *m_pmatParentBone * (*m_pmatParentWorld);
		m_pTransformCom->Set_ParentMatrix(&m_matParent);

		_vec3 vRight;
		memcpy(vRight, m_pTransformCom->m_matWorld.m[0], sizeof(_vec3));
		_float fScale = D3DXVec3Length(&vRight);

	}
	else
	{
		m_pTransformCom->Update_Component(dTimeDelta);
		m_pTransformCom->Set_ParentMatrix(&m_pParentTransform->m_matWorld);
	}
	return NO_EVENT;
}

void CCubeCollider::Render_Collider()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
#ifdef _DEBUG
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTexture(0, m_pTexture[m_eCollType]);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));
	m_pGraphicDev->SetFVF(FVF_CUBE);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	m_pGraphicDev->SetTexture(0, nullptr);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
#endif
}

CCubeCollider * CCubeCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, MESHTYPE eMeshType, CTransform* pTransformCom, _ulong dwNumVtx, const _vec3* pPos, const char* pFrameName, CDynamicMesh* pDynamicMeshCom)
{
	CCubeCollider*	pInstance = new CCubeCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(eMeshType, pTransformCom, pFrameName, pDynamicMeshCom, pPos, dwNumVtx)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CCubeCollider::Clone()
{
	return new CCubeCollider(*this);
}

void CCubeCollider::Free()
{
	CCollider::Free();
	Safe_Release(m_pTransformCom);

#ifdef _DEBUG
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	//Safe_Release(m_pBoundingSphere);
#endif
}
