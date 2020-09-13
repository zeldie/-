#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }
	_ulong				Get_VtxCntX(void) const { return m_iH.biWidth; }
	_ulong				Get_VtxCntZ(void) const { return m_iH.biHeight; }


	void				Copy_Index(Engine::INDEX32* pIndex, const _ulong& dwTriCnt);

	HRESULT				Ready_Prototype(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;
	_vec3*				m_pPos;
	_ulong				m_dwInterval;
	_ulong				m_dwVtxCntX;
	_ulong				m_dwVtxCntZ;

public:
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const _ulong& dwCntX,
									const _ulong& dwCntZ,
									const _ulong& dwVtxItv);

	virtual CComponent*		Clone();
	virtual void			Free();
};

END
#endif // TerrainTex_h__
