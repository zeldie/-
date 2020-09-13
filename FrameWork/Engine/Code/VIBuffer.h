#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	_ulong						Get_TriCnt() { return m_dwTriCnt; }
	LPDIRECT3DVERTEXBUFFER9		Get_VB() { return m_pVB; }
	LPDIRECT3DINDEXBUFFER9		Get_IB() { return m_pIB; }
public:
	virtual HRESULT				Ready_Prototype();
	virtual HRESULT				Ready_Buffer();
	virtual HRESULT				Render_Buffer();

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;

	_ulong							m_dwVtxSize;
	_ulong 							m_dwVtxCnt;
	_ulong 							m_dwTriCnt;
	_ulong 							m_dwVtxFVF;

	_ulong 							m_dwIdxSize;
	D3DFORMAT						m_IdxFmt;

public:	
	virtual CComponent*	Clone(void)PURE;
	virtual void Free(void);

};

END
