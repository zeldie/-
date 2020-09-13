#include "NaviMesh.h"

USING(Engine)

Engine::CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_dwIndex(1000)
	, m_bIsLanding(false)
	, m_fHight(0.f)
{

}

Engine::CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	: CMesh(rhs)
	, m_vecCell(rhs.m_vecCell)
	, m_dwIndex(rhs.m_dwIndex)
{
	for (auto& iter : m_vecCell)
		iter->AddRef();
}

Engine::CNaviMesh::~CNaviMesh()
{

}

HRESULT Engine::CNaviMesh::Ready_NaviMesh(wstring wstrNaviMeshName)
{
	m_vecCell.reserve(100);

	Load_NaviMesh(wstrNaviMeshName);

	if (FAILED(Link_Cell()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNaviMesh::Load_NaviMesh(wstring wstrNaviMeshName)
{
	WCHAR* StrPath = L"";
	if (wstrNaviMeshName == L"Mesh_Navi_Lobby")
		StrPath = L"../../Data/Mesh/Mesh_Navi_Lobby.dat";
	else if (wstrNaviMeshName == L"Mesh_Navi_Cartel")
		StrPath = L"../../Data/Mesh/Mesh_Navi_Cartel.dat";
	else if (wstrNaviMeshName == L"Mesh_Navi_Boss")
		StrPath = L"../../Data/Mesh/Mesh_Navi_Boss.dat";
	else if (wstrNaviMeshName == L"Mesh_Navi_Apostle")
		StrPath = L"../../Data/Mesh/Mesh_Navi_Apostle.dat";

	HANDLE hFile = CreateFile(StrPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	_vec3 vecPos[3];
	_uint iOption = 0;
	_bool bSorting = false;
	Engine::NAVIMESHTYPE eNaviMeshType;
	CCell*		pCell = nullptr;
	ZeroMemory(&vecPos, sizeof(_vec3) * 3);

	while (true)
	{
		int iVertexNum = 0;
		ReadFile(hFile, &eNaviMeshType, sizeof(Engine::NAVIMESHTYPE), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		ReadFile(hFile, &bSorting, sizeof(_bool), &dwByte, nullptr);
		ReadFile(hFile, &vecPos[0], sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vecPos[1], sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vecPos[2], sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(_uint), &dwByte, nullptr);
		if (bSorting == true)
		{
			pCell = CCell::Create(m_pGraphicDev,
				_ulong(m_vecCell.size()),
				&vecPos[0],
				&vecPos[2],
				&vecPos[1]);
			if (pCell == nullptr)
				return E_FAIL;
			m_vecCell.push_back(pCell);
		}
		else
		{
			pCell = CCell::Create(m_pGraphicDev,
				_ulong(m_vecCell.size()),
				&vecPos[0],
				&vecPos[1],
				&vecPos[2]);
			if (pCell == nullptr)
				return E_FAIL;
			m_vecCell.push_back(pCell);
		}
		pCell->Set_Option(iOption);
	}
	CloseHandle(hFile);
	return S_OK;
}

_bool CNaviMesh::Check_Landing(_vec3 * pRayPos, _ulong dwIndex)
{
	_vec3 vVtx1, vVtx2, vVtx3;
	_vec3 vTargetPos = *pRayPos;
	vVtx1 = *m_vecCell[dwIndex]->Get_Point(CCell::POINT_A);
	vVtx2 = *m_vecCell[dwIndex]->Get_Point(CCell::POINT_B);
	vVtx3 = *m_vecCell[dwIndex]->Get_Point(CCell::POINT_C);

	D3DXPLANE Plane;
	D3DXPlaneFromPoints(&Plane, &vVtx1, &vVtx2, &vVtx3);
	_float Hight = (-Plane.a* vTargetPos.x - Plane.c*vTargetPos.z - Plane.d) / Plane.b;

	if (Hight + 15.f >= vTargetPos.y)
	{
		return true; // ÂøÁö
	}
	else
		return false; // Ã¼°ø
}

_bool CNaviMesh::Check_Index(_vec3 Target, _ulong dwIndex)
{
	_float fNewDist = 0.f, fPrevDist = 10000.f;
	_ulong Index = 0;
	_vec3 vVtx1, vVtx2, vVtx3;
	_float fU = 0, fV = 0, fDist = 0;
	_vec3 vRayDir = _vec3(0.f, -1.f, 0.f);
	for (auto & iter : m_vecCell)
	{
		vVtx1 = *(iter->Get_Point(CCell::POINT_A));
		vVtx2 = *(iter->Get_Point(CCell::POINT_B));
		vVtx3 = *(iter->Get_Point(CCell::POINT_C));
		if (D3DXIntersectTri(&vVtx1, &vVtx2, &vVtx3, &Target, &vRayDir, &fU, &fV, &fNewDist))
		{
			//if (fPrevDist > fNewDist)
			{
				//fPrevDist = fNewDist;
				dwIndex = Index;
				return true;
			}
		}
		dwIndex++;
	}
	return false;
	//_vec3 vPos = { Target.x, Target.z,0.f };
	//_uint i = 0;
	//_uint j = 0;
	//for (auto& iter : m_vecCell)
	//{
	//	_vec3 vVtx[3];
	//	vVtx[0] = *(iter->Get_Point(CCell::POINT_A));
	//	vVtx[0] = { (vVtx[0].x), (vVtx[0].z),0.f };

	//	vVtx[1] = *(iter->Get_Point(CCell::POINT_B));
	//	vVtx[1] = { (vVtx[1].x), (vVtx[1].z),0.f };

	//	vVtx[2] = *(iter->Get_Point(CCell::POINT_C));
	//	vVtx[2] = { (vVtx[2].x), (vVtx[2].z),0.f };

	//	_vec3 vIn[3];
	//	vIn[0] = vPos - vVtx[0];
	//	vIn[1] = vPos - vVtx[1];
	//	vIn[2] = vPos - vVtx[2];

	//	_vec3 vDir[3];
	//	vDir[0] = vVtx[1] - vVtx[0];
	//	vDir[1]	= vVtx[2] - vVtx[1];
	//	vDir[2]	= vVtx[0] - vVtx[2];

	//	_vec3 vNormal[3];
	//	vNormal[0] = {-(vDir[0].y), (vDir[0].x), 0.f};
	//	vNormal[1] = {-(vDir[1].y), (vDir[1].x), 0.f};
	//	vNormal[2] = {-(vDir[2].y), (vDir[2].x), 0.f};
	//	///*for (_uint ix = 0; ix < 3; ++ix)
	//	//{
	//	//	vNormal[ix] = { -vDir[ix].y, vDir[ix].x, 0.f };
	//	//}*/

	//	j = 0;
	//	for (_uint iz = 0; iz < 3; ++iz)
	//	{
	//		D3DXVec3Normalize(&vNormal[iz], &vNormal[iz]);
	//		D3DXVec3Normalize(&vIn[iz], &vIn[iz]);

	//		_float t = D3DXVec3Dot(&vNormal[iz], &vIn[iz]);
	//		_float angle = D3DXToDegree(acosf(t));
	//		if (!isnan(angle))
	//		{
	//			if (90.f > angle)
	//				break;
	//			++j;

	//			if (j == 3)
	//			{
	//				dwIndex = i;
	//				return true;
	//			}
	//		}
	//	}

	//	++i;

	//	//if (i == 147)
	//	//	int kk = 3;
	//}	
	//
	//_uint test = i;
	//return false;
}

_vec3 CNaviMesh::CheckY(_vec3 Target,_ulong dwIndex)
{
	_float fNewDist = 0.f, fPrevDist = 10000.f;
	_vec3 vVtx1, vVtx2, vVtx3;
	_float fU = 0, fV = 0;
	_vec3 vRayDir = { 0.f, -1.f, 0.f };
	_vec3 vRayPos = Target;
	vRayPos.y = 0.f;
	vRayPos += _vec3(0.f, 130.f, 0.f);
	vVtx1 = *m_vecCell[dwIndex]->Get_Point(CCell::POINT_A);
	vVtx2 = *m_vecCell[dwIndex]->Get_Point(CCell::POINT_B);
	vVtx3 = *m_vecCell[dwIndex]->Get_Point(CCell::POINT_C);

	_vec3 Res;
	Res.x = (vVtx1.x + vVtx2.x + vVtx3.x) / 3.f;
	Res.y = (vVtx1.y + vVtx2.y + vVtx3.y) / 3.f;
	Res.z = (vVtx1.z + vVtx2.z + vVtx3.z) / 3.f;

	return Res;
	//if (D3DXIntersectTri(&vVtx1, &vVtx2, &vVtx3, &vRayPos, &vRayDir, &fU, &fV, &fNewDist))
	//{
	//	if (fPrevDist > fNewDist)
	//	{
	//		fPrevDist = fNewDist;
	//		return _vec3(vVtx1 + fU*(vVtx2 - vVtx1) + fV*(vVtx3 - vVtx1));
	//	}
	//}
	//else _vec3(0.f, 0.f, 0.f);
	//_float fNewDist = 0.f, fPrevDist = 10000.f;
	//_ulong Index = 0;
	//_vec3 vVtx1, vVtx2, vVtx3;
	//_float fU = 0, fV = 0, fDist = 0;
	//_vec3 vRayDir = _vec3(0.f, -1.f, 0.f);
	//for (auto & iter : m_vecCell)
	//{
	//	vVtx1 = *(iter->Get_Point(CCell::POINT_A));
	//	vVtx2 = *(iter->Get_Point(CCell::POINT_B));
	//	vVtx3 = *(iter->Get_Point(CCell::POINT_C));
	//	if (D3DXIntersectTri(&vVtx1, &vVtx2, &vVtx3, &Target, &vRayDir, &fU, &fV, &fNewDist))
	//	{
	//		//if (fPrevDist > fNewDist)
	//		{
	//			//fPrevDist = fNewDist;
	//			dwIndex = Index;
	//			return true;
	//		}
	//	}
	//	dwIndex++;
	//}
	//return false;

	//return _vec3();
}

HRESULT Engine::CNaviMesh::Link_Cell(void)
{
	_ulong dwCnt = _ulong(m_vecCell.size());

	for (_ulong i = 0; i < dwCnt; ++i)
	{
		for (_ulong j = 0; j < dwCnt; ++j)
		{
			if (i == j)
				continue;

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A),
					m_vecCell[i]->Get_Point(CCell::POINT_B),
					m_vecCell[i]))

			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
					m_vecCell[i]->Get_Point(CCell::POINT_C),
					m_vecCell[i]))

			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
					m_vecCell[i]->Get_Point(CCell::POINT_A),
					m_vecCell[i]))

			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}

	return S_OK;
}

CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrNaviMeshName)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_NaviMesh(wstrNaviMeshName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CNaviMesh::Clone()
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free()
{
	CMesh::Free();

	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();
}

_vec3 Engine::CNaviMesh::Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, _ulong& pIndex, _bool bIsJump)
{
	_vec3		vEndPos = *pTargetPos + *pTargetDir;

	_vec3		vRayPos = *pTargetPos, vRayDir = { 0.f,-1.f, 0.f };
	_vec3		vTargetPos = *pTargetPos;

	_vec3 vVtx1, vVtx2, vVtx3;
	//_float fU = 0, fV = 0, fDist = 0;
	vVtx1 = *m_vecCell[pIndex]->Get_Point(CCell::POINT_A);
	vVtx2 = *m_vecCell[pIndex]->Get_Point(CCell::POINT_B);
	vVtx3 = *m_vecCell[pIndex]->Get_Point(CCell::POINT_C);

	_float fU = 0, fV = 0, fDist = 0;
	vRayPos.y += 100.f;
	if (D3DXIntersectTri(&vVtx1, &vVtx2, &vVtx3, &vRayPos, &vRayDir, &fU, &fV, &fDist))
		vRayPos = vVtx1 + fU*(vVtx2 - vVtx1) + fV*(vVtx3 - vVtx1);
	else
		vRayPos = *pTargetPos;

	if (CCell::COMPARE_MOVE == m_vecCell[pIndex]->Compare(&vEndPos, &pIndex))
	{
		if(!bIsJump)
			vEndPos.y = vRayPos.y;
		return  vEndPos;

	}
	else if (CCell::COMPARE_SLIDING == m_vecCell[pIndex]->Compare(&vEndPos, &pIndex))
	{
		_vec3 vDir = *pTargetDir;
		CCell::LINE eSlidingLine = m_vecCell[pIndex]->Get_SlidingLine();
		_vec3 vNormal = m_vecCell[pIndex]->Get_Line(eSlidingLine)->Get_Normal();
		_vec3 vSliding = vDir - D3DXVec3Dot(&vDir, &vNormal) * vNormal;
		//D3DXVec3Normalize(&vSliding, &vSliding);
		vEndPos = *pTargetPos + vSliding;

		_ulong dwIndex = pIndex;
		Find_Index(dwIndex, &vEndPos);

		if (Check_Index(vEndPos, pIndex))
		{
			//vSliding = vDir - D3DXVec3Dot(&vDir, &vNormal) * vNormal;
			//vEndPos = *pTargetPos + vSliding;
			if(!bIsJump)
				vEndPos.y = vRayPos.y;
			return vEndPos;
		}
		else
		{
			if (!bIsJump)
				vEndPos.y = vRayPos.y;
			return vEndPos = *pTargetPos;
		}
	}
	if (!bIsJump)
		vEndPos.y = vRayPos.y;
	return vEndPos;


	//_vec3		vEndPos = *pTargetPos + *pTargetDir;

	//_vec3		vRayPos = *pTargetPos, vRayDir = { 0.f,-1.f, 0.f };
	//_vec3		vTargetPos = *pTargetPos;

	//_vec3 vVtx1, vVtx2, vVtx3;
	////_float fU = 0, fV = 0, fDist = 0;
	//vVtx1 = *m_vecCell[pIndex]->Get_Point(CCell::POINT_A);
	//vVtx2 = *m_vecCell[pIndex]->Get_Point(CCell::POINT_B);
	//vVtx3 = *m_vecCell[pIndex]->Get_Point(CCell::POINT_C);

	//D3DXPLANE Plane;
	//D3DXPlaneFromPoints(&Plane, &vVtx1, &vVtx2, &vVtx3);
	//_float Hight = (-Plane.a* vTargetPos.x - Plane.c*vTargetPos.z - Plane.d) / Plane.b;

	//if (Hight - 3.f > vEndPos.y)
	//{
	//	m_bIsLanding = true;
	//	m_fHight = Hight;
	//}
	//else
	//	m_bIsLanding = false;

	//if (CCell::COMPARE_MOVE == m_vecCell[pIndex]->Compare(&vEndPos, &pIndex))
	//{
	//	if (!bIsJump)
	//	{
	//		vEndPos.y = Hight;
	//		return  vEndPos;
	//	}
	//	else
	//	{
	//		return  vEndPos;
	//	}
	//}
	//else if (CCell::COMPARE_SLIDING == m_vecCell[pIndex]->Compare(&vEndPos, &pIndex))
	//{
	//	_vec3 vDir = *pTargetDir;
	//	CCell::LINE eSlidingLine = m_vecCell[pIndex]->Get_SlidingLine();
	//	_vec3 vNormal = m_vecCell[pIndex]->Get_Line(eSlidingLine)->Get_Normal();
	//	_vec3 vSliding = vDir - D3DXVec3Dot(&vDir, &vNormal) * vNormal;
	//	vEndPos = *pTargetPos + vSliding;
	//	if (!bIsJump)
	//	{
	//		vEndPos.y = vRayPos.y;
	//		return  vEndPos;
	//	}
	//	else
	//	{
	//		return  vEndPos;
	//	}
	//}
	//return vEndPos;
}

void CNaviMesh::Find_Index(_ulong& pIndex, _vec3* pRayPos)
{
	_float fNewDist = 0.f, fPrevDist = 10000.f;
	_ulong dwIndex = 0;
	_vec3 vVtx1, vVtx2, vVtx3;
	_float fU = 0, fV = 0, fDist = 0;
	_vec3 vRayDir = _vec3(0.f, -1.f, 0.f);
	for (auto & iter : m_vecCell)
	{
		vVtx1 = *(iter->Get_Point(CCell::POINT_A));
		vVtx2 = *(iter->Get_Point(CCell::POINT_B));
		vVtx3 = *(iter->Get_Point(CCell::POINT_C));
		if (D3DXIntersectTri(&vVtx1, &vVtx2, &vVtx3, pRayPos, &vRayDir, &fU, &fV, &fNewDist))
		{
			if (fPrevDist > fNewDist)
			{
				fPrevDist = fNewDist;
				pIndex = dwIndex;
			}
		}

		dwIndex++;
	}
}

_vec3 CNaviMesh::Move_OnCamNavi(const _vec3* pTargetPos, const _vec3* pTargetDir, _ulong& pIndex, _bool bIsJump)
{
	_vec3		vEndPos = *pTargetPos + *pTargetDir;

	_vec3		vRayPos = *pTargetPos, vRayDir = { 0.f,-1.f, 0.f };
	_vec3		vTargetPos = *pTargetPos;

	if (CCell::COMPARE_MOVE == m_vecCell[pIndex]->Compare(&vEndPos, &pIndex))
	{
		return  vEndPos;

	}
	else if (CCell::COMPARE_SLIDING == m_vecCell[pIndex]->Compare(&vEndPos, &pIndex))
	{
		_vec3 vDir = *pTargetDir;
		CCell::LINE eSlidingLine = m_vecCell[pIndex]->Get_SlidingLine();
		_vec3 vNormal = m_vecCell[pIndex]->Get_Line(eSlidingLine)->Get_Normal();
		_vec3 vSliding = vDir - D3DXVec3Dot(&vDir, &vNormal) * vNormal;
		//D3DXVec3Normalize(&vSliding, &vSliding);
		vEndPos = *pTargetPos + vSliding;

		_ulong dwIndex = pIndex;
		Find_Index(dwIndex, &vEndPos);
		if(Check_Index(vEndPos, pIndex))
			return vEndPos = *pTargetPos + vSliding;
		else
			return vEndPos = *pTargetPos;
	}
	return vEndPos;

}

void Engine::CNaviMesh::Render_NaviMesh()
{
#ifdef _DEBUG
	for (auto& iter : m_vecCell)
		iter->Render_Cell();
#endif
}

void CNaviMesh::Find_PosY(_vec3* pRayPos, _ulong dwIndex, _vec3& vPosY)
{
	_float fNewDist = 0.f, fPrevDist = 10000.f;
	_vec3 vVtx1, vVtx2, vVtx3;
	_float fU = 0, fV = 0;
	_vec3 vRayDir = { 0.f, -1.f, 0.f };
	_vec3 vRayPos = *pRayPos;
	vRayPos += _vec3(0.f, 100.f, 0.f);
	//vVtx1 = *m_vecCell[dwIndex]->Get_Point(CCell::POINT_A);
	//vVtx2 = *m_vecCell[dwIndex]->Get_Point(CCell::POINT_B);
	//vVtx3 = *m_vecCell[dwIndex]->Get_Point(CCell::POINT_C);
	for (auto & iter : m_vecCell)
	{
		vVtx1 = *(iter->Get_Point(CCell::POINT_A));
		vVtx2 = *(iter->Get_Point(CCell::POINT_B));
		vVtx3 = *(iter->Get_Point(CCell::POINT_C));
		if (D3DXIntersectTri(&vVtx1, &vVtx2, &vVtx3, &vRayPos, &vRayDir, &fU, &fV, &fNewDist))
		{
			if (fPrevDist > fNewDist)
			{
				fPrevDist = fNewDist;
				vPosY = vVtx1 + fU*(vVtx2 - vVtx1) + fV*(vVtx3 - vVtx1);
			}
		}
	}
}

