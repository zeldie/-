#include "TerrainTex.h"

USING(Engine)

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pPos(nullptr)
{

}

Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	:CVIBuffer(rhs)
	, m_pPos(rhs.m_pPos)
	, m_dwInterval(rhs.m_dwInterval)
	, m_dwVtxCntX(rhs.m_dwVtxCntX)
	, m_dwVtxCntZ(rhs.m_dwVtxCntZ)
{

}

CTerrainTex::~CTerrainTex()
{
}

void CTerrainTex::Copy_Index(Engine::INDEX32 * pIndex, const _ulong & dwTriCnt)
{
	Engine::INDEX32*			pOriIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pOriIndex, 0);

	memcpy(pOriIndex, pIndex, sizeof(Engine::INDEX32) * dwTriCnt);

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;
}

HRESULT CTerrainTex::Ready_Prototype(const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv)
{
	///////////////////////////////////////////////////////////////////////
	/*LPDIRECT3DTEXTURE9			pTexture = nullptr;

	D3DXCreateTexture(m_pGraphicDev, 129, 129, 1, 0,
	D3DFMT_A8R8G8B8,
	D3DPOOL_MANAGED,
	&pTexture);

	D3DLOCKED_RECT LockRect;
	pTexture->LockRect(0, &LockRect, NULL, 0);

	for (_int i = 0; i < 129; ++i)
	{
	for (_int j = 0; j < 129; ++j)
	{
	_int	iIndex = i * 129 + j;

	*(((_ulong*)LockRect.pBits) + iIndex) = D3DCOLOR_ARGB(255, 255, 0, 0);
	}
	}

	pTexture->UnlockRect(0);

	D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/TestHeight.bmp", D3DXIFF_BMP, pTexture, NULL);*/

	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;

	m_pPos = new _vec3[m_dwVtxCnt];

	m_dwVtxFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXNORTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Prototype()))
		return E_FAIL;

	_ulong		dwByte = 0;

	m_hFile = CreateFile(L"../Bin/Resource/Texture/Terrain/Height.bmp",
		GENERIC_READ,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	_ulong*		pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biHeight * m_iH.biWidth, &dwByte, NULL);

	CloseHandle(m_hFile);


	VTXNORTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	_ulong		dwIndex = 0;

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVtxTex[dwIndex].vPos = _vec3(_float(j) * dwVtxItv,
				(pPixel[dwIndex] & 0x000000ff) / 20.f,
				_float(i) * dwVtxItv);

			m_pPos[dwIndex] = pVtxTex[dwIndex].vPos;

			pVtxTex[dwIndex].vNormal = _vec3(0.f, 0.f, 0.f);

			pVtxTex[dwIndex].vTexUV = _vec2(_float(j) / (dwCntX - 1),
				_float(i) / (dwCntZ - 1));
		}
	}



	Safe_Delete_Array(pPixel);

	INDEX32*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong	dwTriIdx = 0;

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위
			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriIdx]._2 = dwIndex + 1;

			_vec3		vDest, vSour, vNormal;

			vDest = pVtxTex[pIndex[dwTriIdx]._1].vPos - pVtxTex[pIndex[dwTriIdx]._0].vPos;
			vSour = pVtxTex[pIndex[dwTriIdx]._2].vPos - pVtxTex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._2].vNormal += vNormal;
			++dwTriIdx;

			// 왼쪽 아래
			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;

			vDest = pVtxTex[pIndex[dwTriIdx]._1].vPos - pVtxTex[pIndex[dwTriIdx]._0].vPos;
			vSour = pVtxTex[pIndex[dwTriIdx]._2].vPos - pVtxTex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._2].vNormal += vNormal;
			++dwTriIdx;

		}
	}

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVtxTex[i].vNormal, &pVtxTex[i].vNormal);


	m_pIB->Unlock();
	m_pVB->Unlock();

	return S_OK;
}

CTerrainTex * CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv)
{
	CTerrainTex*	pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Prototype(dwCntX, dwCntZ, dwVtxItv)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTerrainTex::Clone()
{
	return new CTerrainTex(*this);
}

void CTerrainTex::Free()
{
	CVIBuffer::Free();

	if (false == m_bClone)
		Safe_Delete_Array(m_pPos);
}
