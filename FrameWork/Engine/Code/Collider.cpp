#include "Collider.h"

USING(Engine)

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev),
	m_pmatParentBone(nullptr),
	m_pmatParentWorld(nullptr),
	m_eCollType(COL_FALSE),
	m_bCollision(false),
	m_eParentMeshType(MESHTYPE_END),
	m_bIsDead(false)
{
}

CCollider::CCollider(const CCollider & rhs)
	:CComponent(rhs),
	m_pmatParentBone(rhs.m_pmatParentBone),
	m_pmatParentWorld(rhs.m_pmatParentWorld),
	m_eCollType(rhs.m_eCollType),
	m_bCollision(rhs.m_bCollision),
	m_eParentMeshType(rhs.m_eParentMeshType),
	m_bIsDead(rhs.m_bIsDead)
{
}


CCollider::~CCollider()
{
}

_int CCollider::Update_Component(const _double & dTimeDelta)
{
	return NO_EVENT;
}

CComponent * CCollider::Clone()
{
	return new CCollider(*this);
}

void CCollider::Free()
{
	CComponent::Free();
#ifdef _DEBUG
	for (_uint i = 0; i < COL_END; ++i)
		Safe_Release(m_pTexture[i]);
#endif
}
