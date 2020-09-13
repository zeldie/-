#ifndef Engine_Math_h__
#define Engine_Math_h__

namespace Engine
{
	//방향 구함
	static _vec3 GetDir(_vec3 vDstPos, _vec3 vSrcPos)
	{
		_vec3 vDir;
		return *D3DXVec3Normalize(&vDir, &(vDstPos - vSrcPos));

	}
	//Y 제외한 방향 구함
	static _vec3 GetDirNoY(_vec3 vDstPos, _vec3 vSrcPos)
	{
		_vec3 vDir = vDstPos - vSrcPos;
		vDir.y = 0.f;
		return *D3DXVec3Normalize(&vDir, &vDir);
	}
	//거리 구함
	static _float GetDist(_vec3  vDstPos, _vec3  vSrcPos)
	{
		_vec3 vDir = vDstPos - vSrcPos;
		return D3DXVec3Length(&vDir);
	}
	//Y제외한 거리 구함
	static _float GetDistNoY(_vec3  vDstPos, _vec3  vSrcPos)
	{
		_vec3 vDir = vDstPos - vSrcPos;
		vDir.y = 0.f;
		return D3DXVec3Length(&vDir);
	}
	//각도 구함
	static _float GetAngleY(_vec3 vSrcDir, _vec3 vDstDir)
	{
		vSrcDir.y = 0.f;
		vDstDir.y = 0.f;
		_float fAngle = acosf(D3DXVec3Dot(&vSrcDir, &vDstDir) / (D3DXVec3Length(&vSrcDir) * D3DXVec3Length(&vDstDir)));
		fAngle = D3DXToDegree(fAngle);
		_vec3 vUpDown;
		D3DXVec3Cross(&vUpDown, &vSrcDir, &vDstDir);
		if (vUpDown.y > 0)
			return fAngle;
		else
			return -fAngle;
		return 0.f;
	}
	//평균 구함
	template <typename T>
	static T Average(T Src, T Dst)
	{
		T tAverage = (Src + Dst) * 0.5;
		return tAverage;
	}
	//랜덤 Int 뽑음
	static _int RandomNumber(_int iRandomNum)
	{
		return (rand() % iRandomNum);
	}
	//최소값과 최대값이 정해져있는 랜덤 뽑기 정수
	static _int RandomIntRange(_int iMinimum, _int iMaximum)
	{

		return (rand() % (iMaximum - iMinimum + 1)) + iMinimum;
	}
	//rand 함수의 최대값 32767
	static _float RandomFloat()
	{
		return ((float)rand() / (float)RAND_MAX);
	}
	//랜덤 Float 뽑음
	static _float RandomFloat(_float fNum)
	{
		return ((float)rand() / (float)RAND_MAX) * fNum;
	}
	//범위 있는 Float 뽑음
	static _float RandomFloatRange(_float fMinimum, _float fMaximum)
	{
		_float fRnd = (_float)rand() / (_float)RAND_MAX;
		return (fRnd * (fMaximum - fMinimum) + fMinimum);
	}

	//파일 이름 바꿈
	static void ChangeFileName(_tchar* pFilePath, _tchar* pSrc, _tchar* pDst)
	{
		_uint iLength = lstrlen(pFilePath);

		for (_uint i = iLength; i >= iLength - 5; --i)
		{
			if (pFilePath[i] == *pSrc)
				pFilePath[i] = *pDst;
		}
	}

	struct Ray
	{
		_vec3 vPos;
		_vec3 vDir;
	};
	//Ray와 구 충돌
	static _bool RayShpereIntersection(Ray* ray, D3DXVECTOR3* Pos, _float fRadius)
	{
		_vec3 v = ray->vPos - *Pos;

		_float fb = 2.f*D3DXVec3Dot(&ray->vDir, &v);
		_float fc = D3DXVec3Dot(&v, &v) - (fRadius * fRadius);

		_float fdicriminant = (fb*fb) - (4.f*fc);

		if (fdicriminant < 0.f)
			return false;

		fdicriminant = sqrtf(fdicriminant);

		_float fs0 = (-fb + fdicriminant) / 2.f;
		_float fs1 = (-fb - fdicriminant) / 2.f;

		if (fs0 >= 0.f || fs1 >= 0.f)
		{
			return true;
		}
		return false;
	}

	//fValue를 fMin과 fMax 사이 값으로 절삭 함
	static _float Clamp(_float fValue, _float fMin, _float fMax)
	{
		if (fValue < fMin)
			return fMin;
		else if (fValue > fMax)
			return fMax;
		return fValue;
	}
	//제곱
	static _float Sqaure(_float fValue)
	{
		return fValue*fValue;
	}
	//스왑
	static void Swap(_float fA, _float fB)
	{
		_float fTemp(fA);
		fA = fB;
		fB = fTemp;
	}
	//FLoat -> DWORD
	static DWORD F2DW(_float fFloat)
	{
		return *((DWORD*)&fFloat);
	}
}

#endif // Engine_Math_h__
