#include "Calculator.h"
#include "Transform.h"
#include "GameObject.h"
#include "StaticMesh.h"

USING(Engine)

CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev),
	m_pPickTarget(nullptr)
{
	ZeroMemory(&m_vRayPos, sizeof(_vec3));
	ZeroMemory(&m_vRayDir, sizeof(_vec3));
}

CCalculator::CCalculator(const CCalculator & rhs)
	:CComponent(rhs),
	m_pPickTarget(rhs.m_pPickTarget)
{
	ZeroMemory(&m_vRayPos, sizeof(_vec3));
	ZeroMemory(&m_vRayDir, sizeof(_vec3));
}


CCalculator::~CCalculator()
{
}

void CCalculator::TranslateLocalSpace(HWND hWnd, const _matrix& WorldMatrix)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 윈도우 상태의 마우스 좌표를 투영으로 변환

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / (ViewPort.Height * -0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영에서 뷰스페이스
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	m_vRayPos = _vec3(0.f, 0.f, 0.f);
	m_vRayDir = vMousePos - m_vRayPos;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
	// 뷰 스페이스에서 월드 스페이스로 변환

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
	// 월드 스페이스에서 로컬 스페이스로 변환

	_matrix		matWorld;
	D3DXMatrixInverse(&matWorld, nullptr, &WorldMatrix);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorld);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

void CCalculator::TranslateWorldSpace(HWND hWnd)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 윈도우 상태의 마우스 좌표를 투영으로 변환

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / (ViewPort.Height * -0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영에서 뷰스페이스
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3	vRayDir, vRayPos;

	m_vRayPos = _vec3(0.f, 0.f, 0.f);
	m_vRayDir = vMousePos - m_vRayPos;

	// 뷰 스페이스에서 월드 스페이스로 변환

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

void CCalculator::TranslateWorldSpaceDecal(HWND hWnd)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 윈도우 상태의 마우스 좌표를 투영으로 변환

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / (ViewPort.Height * -0.5f) +0.7f;
	vMousePos.z = 0.f;

	// 투영에서 뷰스페이스
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3	vRayDir, vRayPos;

	m_vRayPos = _vec3(0.f, 0.f, 0.f);
	m_vRayDir = vMousePos - m_vRayPos;

	// 뷰 스페이스에서 월드 스페이스로 변환

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

_float CCalculator::Compute_HeightOnTerrain(const _vec3 * pPos, const _vec3 * pTerrainVtxPos, const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwItv)
{
	_ulong	dwIndex = _ulong(pPos->z / dwItv) * dwCntX + _ulong(pPos->x / dwItv);

	_float	fRatioX = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwItv;
	_float	fRatioZ = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwItv;

	_float	fHeight[4] = {
		pTerrainVtxPos[dwIndex + dwCntX].y,
		pTerrainVtxPos[dwIndex + dwCntX + 1].y,
		pTerrainVtxPos[dwIndex + 1].y,
		pTerrainVtxPos[dwIndex].y
	};


	// 오른쪽 위
	if (fRatioX > fRatioZ)
	{
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}
	// 왼쪽 아래
	else
	{
		return fHeight[0] + (fHeight[2] - fHeight[3]) * fRatioX + (fHeight[3] - fHeight[0]) * fRatioZ;
	}
}
_bool CCalculator::PickNaviMesh(HWND hWnd, CGameObject* pNaviMeshObject, LPD3DXBASEMESH Mesh, _vec3 * vecOutPos)
{
	if (Mesh == nullptr)
		return false;
	ZeroMemory(vecOutPos, sizeof(_vec3));
	CTransform* pTransformCom = dynamic_cast<CTransform*>(pNaviMeshObject->Get_Component(COMPONENTTYPE::TRANSFORM, Engine::ID_DYNAMIC));
	TranslateLocalSpace(hWnd, *(pTransformCom->Get_WorldMatrix()));
	_float	fU, fV, fDist;
	BOOL bIsHit = false;
	DWORD dwFace = 0;
	D3DXIntersect(Mesh, &m_vRayPos, &m_vRayDir, &bIsHit, &dwFace, &fU, &fV, &fDist, NULL, NULL);
	if (bIsHit)
	{
		*vecOutPos = (m_vRayPos + fDist*m_vRayDir);
		D3DXVec3TransformCoord(vecOutPos, vecOutPos, pTransformCom->Get_WorldMatrix());
		return true;
	}
	return false;
}

_bool CCalculator::PickMesh(HWND hWnd, CGameObject * pMeshObject, _vec3 * vecOutPos, _float* pOldDist)
{
	if (pMeshObject == nullptr)
		return false;
	//ZeroMemory(vecOutPos, sizeof(_vec3));
	CTransform* pTransformCom = dynamic_cast<CTransform*>(pMeshObject->Get_Component(COMPONENTTYPE::TRANSFORM, Engine::ID_DYNAMIC));
	TranslateLocalSpace(hWnd, *(pTransformCom->Get_WorldMatrix()));

	CStaticMesh* pStaticMeshCom = dynamic_cast<CStaticMesh*>(pMeshObject->Get_Component(COMPONENTTYPE::MESH, Engine::ID_STATIC));
	_float	fU, fV, fDist = 50000.f;
	BOOL bIsHit = false;
	DWORD dwFace = 0;
	D3DXIntersect(pStaticMeshCom->Get_Mesh(), &m_vRayPos, &m_vRayDir, &bIsHit, &dwFace, &fU, &fV, &fDist, NULL, NULL);
	if (*pOldDist < fDist)
		return false;
	else
		*pOldDist = fDist;
	if (bIsHit)
	{
		*vecOutPos = (m_vRayPos + fDist*m_vRayDir);
		D3DXVec3TransformCoord(vecOutPos, vecOutPos, pTransformCom->Get_WorldMatrix());
		*pOldDist = fDist;
		return true;
	}
	return false;
}

_bool CCalculator::PickStaticMesh(HWND hWnd, CGameObject * pMeshObject, _vec3 * vecOutPos, _float * pOldDist)
{
	if (pMeshObject == nullptr)
		return false;
	CTransform* pTransformCom = dynamic_cast<CTransform*>(pMeshObject->Get_Component(COMPONENTTYPE::TRANSFORM, Engine::ID_DYNAMIC));
	TranslateLocalSpace(hWnd, *(pTransformCom->Get_WorldMatrix()));

	CStaticMesh* pStaticMeshCom = dynamic_cast<CStaticMesh*>(pMeshObject->Get_Component(COMPONENTTYPE::MESH, Engine::ID_STATIC));
	_float	fU, fV, fDist = 0.f;
	BOOL bIsHit = false; 
	DWORD dwFace = 0;
	D3DXIntersect(pStaticMeshCom->Get_Mesh(), &m_vRayPos, &m_vRayDir, &bIsHit, &dwFace, &fU, &fV, &fDist, NULL, NULL);
	if (fDist == 0.f)
		return false;
	if (bIsHit)
	{
		if (fDist < *pOldDist)
		{
			*vecOutPos = (m_vRayPos + fDist*m_vRayDir);
			D3DXVec3TransformCoord(vecOutPos, vecOutPos, pTransformCom->Get_WorldMatrix());
			*pOldDist = fDist;
			return true;
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return false;
}

_bool CCalculator::PickDynamicMesh(HWND hWnd, CGameObject * pDynamicMesh, LPD3DXBASEMESH Mesh, _vec3 * vecOutPos, _float* pOldDist)
{
	if (Mesh == nullptr)
		return false;
	ZeroMemory(vecOutPos, sizeof(_vec3));
	CTransform* pTransformCom = dynamic_cast<CTransform*>(pDynamicMesh->Get_Component(COMPONENTTYPE::TRANSFORM, Engine::ID_DYNAMIC));
	TranslateLocalSpace(hWnd, *(pTransformCom->Get_WorldMatrix()));
	_float	fU, fV, fDist = 50000.f;
	BOOL bIsHit = false;
	DWORD dwFace = 0;
	D3DXIntersect(Mesh, &m_vRayPos, &m_vRayDir, &bIsHit, &dwFace, &fU, &fV, &fDist, NULL, NULL);
	if (*pOldDist < fDist)
		return false;
	else
		*pOldDist = fDist;
	if (bIsHit)
	{
		*vecOutPos = (m_vRayPos + fDist*m_vRayDir);
		D3DXVec3TransformCoord(vecOutPos, vecOutPos, pTransformCom->Get_WorldMatrix());
		*pOldDist = fDist;
		return true;
	}
	return false;
}

_bool CCalculator::PickDecalPos(HWND hWnd, CGameObject * pMeshObject, _vec3 * vecOutPos)
{
	if (pMeshObject == nullptr)
		return false;
	//ZeroMemory(vecOutPos, sizeof(_vec3));
	CTransform* pTransformCom = dynamic_cast<CTransform*>(pMeshObject->Get_Component(COMPONENTTYPE::TRANSFORM, Engine::ID_DYNAMIC));
	TranslateWorldSpaceDecal(hWnd);

	CStaticMesh* pStaticMeshCom = dynamic_cast<CStaticMesh*>(pMeshObject->Get_Component(COMPONENTTYPE::MESH, Engine::ID_STATIC));
	_float	fU, fV, fDist = 50000.f;
	BOOL bIsHit = false;
	DWORD dwFace = 0;
	D3DXIntersect(pStaticMeshCom->Get_Mesh(), &m_vRayPos, &m_vRayDir, &bIsHit, &dwFace, &fU, &fV, &fDist, NULL, NULL);
	if (bIsHit)
	{
		*vecOutPos = (m_vRayPos + fDist*m_vRayDir);
		D3DXVec3TransformCoord(vecOutPos, vecOutPos, pTransformCom->Get_WorldMatrix());
		return true;
	}
	return false;
}


CCalculator * CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator* pInstance = new CCalculator(pGraphicDev);
	return pInstance;
}

CComponent * CCalculator::Clone(void)
{
	return new CCalculator(*this);
}

void CCalculator::Free()
{
	CComponent::Free();
}
