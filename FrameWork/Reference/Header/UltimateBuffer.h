#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CUltimateBuffer : public CComponent
{
protected:
	explicit CUltimateBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUltimateBuffer(const CUltimateBuffer& rhs);
	virtual ~CUltimateBuffer();

public:
	virtual HRESULT				Ready_Prototype();
	virtual HRESULT				Ready_Buffer();
	HRESULT						Render_Buffer(_uint iCount);

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;

	_ulong							m_dwVtxSize;
	_ulong 							m_dwVtxCnt;
	_ulong 							m_dwTriCnt;
	_ulong 							m_dwVtxFVF;

public:
	static CUltimateBuffer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*			Clone(void);
	virtual void				Free(void);

};

END
