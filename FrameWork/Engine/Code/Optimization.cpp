#include "Optimization.h"

USING(Engine)

Engine::COptimization::COptimization(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev),
	m_pFrustum(nullptr)
{

}

Engine::COptimization::COptimization(const COptimization& rhs)
	:CComponent(rhs),
	m_pFrustum(rhs.m_pFrustum)
{

}

Engine::COptimization::~COptimization()
{

}

HRESULT Engine::COptimization::Ready_Optimization(USEAGE eType, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	m_pFrustum = CFrustum::Create(m_pGraphicDev);
	if (m_pFrustum == nullptr)
		return E_FAIL;

	if (DISABLE == eType)
		return S_OK;
	return S_OK;
}

_bool Engine::COptimization::IsIn_Frustum_ForObject(const _vec3* pPosWorld, const _float& fRadius)
{
	return m_pFrustum->IsIn_Frustum_ForObject(pPosWorld, fRadius);
}

COptimization* Engine::COptimization::Create(LPDIRECT3DDEVICE9 pGraphicDev, USEAGE eType, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	COptimization*	pInstance = new	COptimization(pGraphicDev);

	if (FAILED(pInstance->Ready_Optimization(eType, dwCntX, dwCntZ)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::COptimization::Clone()
{
	return new COptimization(*this);
}

void Engine::COptimization::Free()
{
	if (false == m_bClone)
		Safe_Release(m_pFrustum);
	CComponent::Free();
}

