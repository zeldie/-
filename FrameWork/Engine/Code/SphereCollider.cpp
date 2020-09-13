#include "SphereCollider.h"
#include "ComponentMgr.h"

USING(Engine)

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCollider(pGraphicDev),
	m_pBoundingSphere(nullptr),
	m_pTransformCom(nullptr),
	m_fRadius(0.f),
	m_fOriginRadius(0.f),
	m_pParentTransform(nullptr)
{
	ZeroMemory(&m_szFrameName, sizeof(_char)*MIN_STR);
}

CSphereCollider::CSphereCollider(const CSphereCollider & rhs)
	:CCollider(rhs),
	m_pBoundingSphere(rhs.m_pBoundingSphere),
	m_pTransformCom(rhs.m_pTransformCom),
	m_fRadius(rhs.m_fRadius),
	m_fOriginRadius(rhs.m_fOriginRadius),
	m_pParentTransform(rhs.m_pParentTransform)
{
	ZeroMemory(&m_szFrameName, sizeof(_char)*MIN_STR);
}


CSphereCollider::~CSphereCollider()
{	
}

void CSphereCollider::Get_FrameName(char * szname)
{
	strcpy_s(szname, MIN_STR, m_szFrameName);
}

_vec3 CSphereCollider::Get_Pos()
{
	_vec3 vPos;
	memcpy(&vPos, &m_pTransformCom->m_matWorld.m[3][0], sizeof(_vec3));

	//m_pTransformCom->m_matWorld
	return vPos;
}

void CSphereCollider::Set_Radius(_float fScale)
{
	m_fRadius += m_fOriginRadius * fScale * 0.5f;
}


HRESULT CSphereCollider::Ready_Collider(MESHTYPE eMeshType, CTransform * pTransformCom, _float fRadius, const char * pFrameName, CDynamicMesh * pDynamicMeshCom, _vec3 * pPos)
{
	m_pTransformCom = dynamic_cast<Engine::CTransform*>(CComponentMgr::GetInstance()->Clone(RESOURCE_STATIC, L"TransformCom"));
 	m_eParentMeshType = eMeshType;
	m_fRadius = fRadius * pTransformCom->m_vScale.x;
	m_fOriginRadius = fRadius * pTransformCom->m_vScale.x;
	m_pParentTransform = pTransformCom;
	strcpy_s(m_szFrameName, MIN_STR, pFrameName);
	switch (m_eParentMeshType)
	{
	case Engine::TYPE_STATIC:
		m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
		m_pParentTransform->Get_WorldMatrix(&m_matParent);
		//pTransformCom->Get_WorldMatrix(&m_matParent);
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
		m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	}
		
		break;
	case Engine::MESHTYPE_END:
		break;
	default:
		break;
	}

#ifdef _DEBUG
	if (FAILED(D3DXCreateSphere(m_pGraphicDev, fRadius, 10, 10, &m_pBoundingSphere, NULL)))
		return E_FAIL;

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

_int CSphereCollider::Update_Component(const _double & dTimeDelta)
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

void CSphereCollider::Render_Collider()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
#ifdef _DEBUG
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTexture(0, m_pTexture[m_eCollType]);
	m_pBoundingSphere->DrawSubset(0);

	m_pGraphicDev->SetTexture(0, nullptr);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
#endif
}

CSphereCollider * CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, MESHTYPE eMeshType, CTransform * pTransformCom, _float fRadius, const char * pFrameName, CDynamicMesh * pDynamicMeshCom, _vec3 * pPos)
{
	CSphereCollider*	pInstance = new CSphereCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(eMeshType, pTransformCom, fRadius, pFrameName, pDynamicMeshCom, pPos)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CSphereCollider::Clone()
{
	return new CSphereCollider(*this);
}

void CSphereCollider::Free()
{
	CCollider::Free();
	Safe_Release(m_pTransformCom);

#ifdef _DEBUG
	Safe_Release(m_pBoundingSphere);
#endif
}
