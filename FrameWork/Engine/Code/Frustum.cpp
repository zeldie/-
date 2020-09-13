#include "Frustum.h"

USING(Engine)

Engine::CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CFrustum::~CFrustum(void)
{

}

HRESULT Engine::CFrustum::Ready_Frustum(void)
{
	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return S_OK;
}

_bool Engine::CFrustum::IsIn_Frustum(const _vec3* pPos, const _float fRadius)
{
	_float		fDistance = 0.f;

	for (_uint i = 0; i < 6; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPos);

		if (fDistance > fRadius)
			return false;
	}

	return true;
}

_bool Engine::CFrustum::IsIn_Frustum_ForObject(const _vec3* pPosWorld, const _float& fRadius)
{
	Ready_Frustum();

	_matrix		matProj, matView;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_uint i = 0; i < 8; ++i)
	{
		
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
		
	}
	
	// +x
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// -x
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// +y
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// -y
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// +z
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// -z
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	return IsIn_Frustum(pPosWorld, fRadius);

}

_bool CFrustum::IsIn_Rectangle(_vec3 * pMinimun, _vec3 * pMaximum)
{
	_float fDist;

	for (int i = 0; i < 6; ++i)
	{
		fDist = D3DXPlaneDotCoord(&m_Plane[i], pMinimun);
		if (fDist > 1000.f)
			return false;

		fDist = D3DXPlaneDotCoord(&m_Plane[i], pMaximum);
		if (fDist > 1000.f)
			return false;
	}

	return true;
}

CFrustum* Engine::CFrustum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFrustum*		pInstance = new CFrustum(pGraphicDev);

	if (FAILED(pInstance->Ready_Frustum()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CFrustum::Free(void)
{
	Safe_Release(m_pGraphicDev);
}

