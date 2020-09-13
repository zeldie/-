#ifndef Engine_Math_h__
#define Engine_Math_h__

namespace Engine
{
	//���� ����
	static _vec3 GetDir(_vec3 vDstPos, _vec3 vSrcPos)
	{
		_vec3 vDir;
		return *D3DXVec3Normalize(&vDir, &(vDstPos - vSrcPos));

	}
	//Y ������ ���� ����
	static _vec3 GetDirNoY(_vec3 vDstPos, _vec3 vSrcPos)
	{
		_vec3 vDir = vDstPos - vSrcPos;
		vDir.y = 0.f;
		return *D3DXVec3Normalize(&vDir, &vDir);
	}
	//�Ÿ� ����
	static _float GetDist(_vec3  vDstPos, _vec3  vSrcPos)
	{
		_vec3 vDir = vDstPos - vSrcPos;
		return D3DXVec3Length(&vDir);
	}
	//Y������ �Ÿ� ����
	static _float GetDistNoY(_vec3  vDstPos, _vec3  vSrcPos)
	{
		_vec3 vDir = vDstPos - vSrcPos;
		vDir.y = 0.f;
		return D3DXVec3Length(&vDir);
	}
	//���� ����
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
	//��� ����
	template <typename T>
	static T Average(T Src, T Dst)
	{
		T tAverage = (Src + Dst) * 0.5;
		return tAverage;
	}
	//���� Int ����
	static _int RandomNumber(_int iRandomNum)
	{
		return (rand() % iRandomNum);
	}
	//�ּҰ��� �ִ밪�� �������ִ� ���� �̱� ����
	static _int RandomIntRange(_int iMinimum, _int iMaximum)
	{

		return (rand() % (iMaximum - iMinimum + 1)) + iMinimum;
	}
	//rand �Լ��� �ִ밪 32767
	static _float RandomFloat()
	{
		return ((float)rand() / (float)RAND_MAX);
	}
	//���� Float ����
	static _float RandomFloat(_float fNum)
	{
		return ((float)rand() / (float)RAND_MAX) * fNum;
	}
	//���� �ִ� Float ����
	static _float RandomFloatRange(_float fMinimum, _float fMaximum)
	{
		_float fRnd = (_float)rand() / (_float)RAND_MAX;
		return (fRnd * (fMaximum - fMinimum) + fMinimum);
	}

	//���� �̸� �ٲ�
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
	//Ray�� �� �浹
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

	//fValue�� fMin�� fMax ���� ������ ���� ��
	static _float Clamp(_float fValue, _float fMin, _float fMax)
	{
		if (fValue < fMin)
			return fMin;
		else if (fValue > fMax)
			return fMax;
		return fValue;
	}
	//����
	static _float Sqaure(_float fValue)
	{
		return fValue*fValue;
	}
	//����
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
