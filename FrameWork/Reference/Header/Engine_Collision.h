#ifndef Engine_Collision_h__
#define Engine_Collision_h__

namespace Engine
{
	typedef	struct tagOBB
	{
		_vec3		vPoint[8];
		_vec3		vCenter;
		_vec3		vProjAxis[3];	// 객체 당 세 개의 면을 향해 뻗어나가는 벡터
		_vec3		vAxis[3];		// 박스와 평행한 임의의 축

	}OBB;

	static _bool ColSphere_Sphere(_vec3& vSrcPos, _vec3& vDstPos, _float fSrcRadius, _float fDstRadius)
	{
		_float fRadiusSum = (fSrcRadius + fDstRadius) * 0.5f;

		D3DXVECTOR3 vDist = vSrcPos - vDstPos;
		float fDist = D3DXVec3Length(&vDist);

		if (fDist < fRadiusSum)
		{
			return true;
		}
		return false;
	}

	static _bool ColBox_Box(_vec3& vSrcPos, _vec3& vDstPos, _vec3& vSrcEdges, _vec3& vDstEdges)
	{
		_vec3 vRadiusSum = _vec3(vSrcEdges.x + vDstEdges.x, vSrcEdges.y + vDstEdges.y, vSrcEdges.z + vDstEdges.z) * 0.5f;
		_vec3 vDist = _vec3(fabsf(vSrcPos.x - vDstPos.x), fabsf(vSrcPos.y - vDstPos.y), fabsf(vSrcPos.z - vDstPos.z));

		if (vDist.x < vRadiusSum.x && vDist.y < vRadiusSum.y && vDist.z < vRadiusSum.z)
		{
			return true;
		}
		return false;
	}

	static _bool ColBox_Box_Push(_vec3& vSrcPos, _vec3& vDstPos, _vec3& vSrcEdges, _vec3& vDstEdges, _vec3* pPush)
	{
		_vec3 vRadiusSum = _vec3(vSrcEdges.x + vDstEdges.x, vSrcEdges.y + vDstEdges.y, vSrcEdges.z + vDstEdges.z) * 0.5f;
		_vec3 vDist = _vec3(fabsf(vSrcPos.x - vDstPos.x), fabsf(vSrcPos.y - vDstPos.y), fabsf(vSrcPos.z - vDstPos.z));

		if (vDist.x < vRadiusSum.x && vDist.y < vRadiusSum.y && vDist.z < vRadiusSum.z)
		{
			(*pPush).x = vRadiusSum.x - vDist.x;
			(*pPush).y = vRadiusSum.y - vDist.y;
			(*pPush).z = vRadiusSum.z - vDist.z;
			return true;
		}
		return false;
	}

	static _bool ColSphere_Box(_vec3& vSrcPos, _vec3& vDstPos, _vec3& vSrcEdges, _float fDstRadius)
	{
		_vec3 vClosestPos = {};
		//X에 대한 검사
		if (vDstPos.x < vSrcPos.x - fDstRadius * 0.5f)
			vClosestPos.x = vSrcPos.x - fDstRadius * 0.5f;
		else if (vDstPos.x > vSrcPos.x + fDstRadius * 0.5f)
			vClosestPos.x = vSrcPos.x + fDstRadius * 0.5f;
		else
			vClosestPos.x = vDstPos.x;

		//Y에 대한 검사
		if (vDstPos.y < vSrcPos.y - fDstRadius * 0.5f)
			vClosestPos.y = vSrcPos.y - fDstRadius * 0.5f;
		else if (vDstPos.y > vSrcPos.y + fDstRadius * 0.5f)
			vClosestPos.y = vSrcPos.y + fDstRadius * 0.5f;
		else
			vClosestPos.y = vDstPos.y;

		//Z에 대한 검사
		if (vDstPos.z < vSrcPos.z - fDstRadius * 0.5f)
			vClosestPos.z = vSrcPos.z - fDstRadius * 0.5f;
		else if (vDstPos.z > vSrcPos.z + fDstRadius * 0.5f)
			vClosestPos.z = vSrcPos.z + fDstRadius * 0.5f;
		else
			vClosestPos.z = vDstPos.z;

		D3DXVECTOR3 vDist = vDstPos - vClosestPos;
		float fDist = D3DXVec3Length(&vDist);

		if (fDist > fDstRadius * 0.5f)
			return false;
		else
			return true;
	}


	static void Set_Point(OBB * pObb, const _vec3 * pMin, const _vec3 * pMax)
	{

		pObb->vPoint[0] = _vec3(pMin->x, pMax->y, pMin->z);
		pObb->vPoint[1] = _vec3(pMax->x, pMax->y, pMin->z);
		pObb->vPoint[2] = _vec3(pMax->x, pMin->y, pMin->z);
		pObb->vPoint[3] = _vec3(pMin->x, pMin->y, pMin->z);

		pObb->vPoint[4] = _vec3(pMin->x, pMax->y, pMax->z);
		pObb->vPoint[5] = _vec3(pMax->x, pMax->y, pMax->z);
		pObb->vPoint[6] = _vec3(pMax->x, pMin->y, pMax->z);
		pObb->vPoint[7] = _vec3(pMin->x, pMin->y, pMax->z);

		pObb->vCenter = (*pMin + *pMax) * 0.5f;

	}

	static void Set_Axis(OBB * pObb)
	{
		pObb->vProjAxis[0] = (pObb->vPoint[2] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
		pObb->vProjAxis[1] = (pObb->vPoint[0] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
		pObb->vProjAxis[2] = (pObb->vPoint[7] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;

		pObb->vAxis[0] = pObb->vPoint[2] - pObb->vPoint[3];
		pObb->vAxis[1] = pObb->vPoint[0] - pObb->vPoint[3];
		pObb->vAxis[2] = pObb->vPoint[7] - pObb->vPoint[3];

		for (_uint i = 0; i < 3; ++i)
			D3DXVec3Normalize(&pObb->vAxis[i], &pObb->vAxis[i]);
	}

	static _bool ColSphere_OBB(const _vec3* pDestMin, const _vec3* pDestMax, _matrix* pDestWorld, _vec3& vSrcPos, _float fDstRadius)
	{
		OBB		tObb;
		ZeroMemory(&tObb, sizeof(OBB));

		Set_Point(&tObb, pDestMin, pDestMax);

		for (_uint i = 0; i < 8; ++i)
		{
			D3DXVec3TransformCoord(&tObb.vPoint[i], &tObb.vPoint[i], pDestWorld);
		}
		D3DXVec3TransformCoord(&tObb.vCenter, &tObb.vCenter, pDestWorld);

		Set_Axis(&tObb);

		_vec3	vBoxPos;

		// x
		if (vSrcPos.x < tObb.vCenter.x - D3DXVec3Length(&tObb.vAxis[0]) / 2.f)
		{
			vBoxPos.x = tObb.vCenter.x - D3DXVec3Length(&tObb.vAxis[0]) / 2.f;
		}
		else if (vSrcPos.x > tObb.vCenter.x + D3DXVec3Length(&tObb.vAxis[0]) / 2.f)
		{
			vBoxPos.x = tObb.vCenter.x + D3DXVec3Length(&tObb.vAxis[0]) / 2.f;
		}
		else
		{
			vBoxPos.x = tObb.vCenter.x;
		}

		// y
		if (vSrcPos.y < tObb.vCenter.y - D3DXVec3Length(&tObb.vAxis[1]) / 2.f)
		{
			vBoxPos.y = tObb.vCenter.y - D3DXVec3Length(&tObb.vAxis[1]) / 2.f;
		}
		else if (vSrcPos.y > tObb.vCenter.y + D3DXVec3Length(&tObb.vAxis[1]) / 2.f)
		{
			vBoxPos.y = tObb.vCenter.y + D3DXVec3Length(&tObb.vAxis[1]) / 2.f;
		}
		else
		{
			vBoxPos.y = tObb.vCenter.y;
		}

		// z
		if (vSrcPos.z < tObb.vCenter.z - D3DXVec3Length(&tObb.vAxis[2]) / 2.f)
		{
			vBoxPos.z = tObb.vCenter.z - D3DXVec3Length(&tObb.vAxis[2]) / 2.f;
		}
		else if (vSrcPos.z > tObb.vCenter.z + D3DXVec3Length(&tObb.vAxis[2]) / 2.f)
		{
			vBoxPos.z = tObb.vCenter.z + D3DXVec3Length(&tObb.vAxis[2]) / 2.f;
		}
		else
		{
			vBoxPos.z = tObb.vCenter.z;
		}

		_vec3 vDist = vSrcPos - vBoxPos;

		if (fDstRadius > D3DXVec3Length(&vDist))
			return true;

		return false;
	}
}

#endif // Engine_Collision_h__
