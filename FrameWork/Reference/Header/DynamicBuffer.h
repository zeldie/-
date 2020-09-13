#ifndef DynamicBuffer_h__
#define DynamicBuffer_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicBuffer final : public CVIBuffer
{
private:
	explicit CDynamicBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicBuffer(const CDynamicBuffer& rhs);
	virtual ~CDynamicBuffer();

public:
	HRESULT				Ready_ProtoType(const _ulong& dwCnt);
	virtual HRESULT		Ready_Buffer();
public:
	DWORD				Get_VtxCnt() const { return m_dwCnt; }
	_int				Update_Buffer(list<_vec3> ListPos[]);

private:
	DWORD				m_dwCnt;

public:
	static CDynamicBuffer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCnt);
	virtual CComponent*			Clone();
	virtual void				Free();
};


END
#endif // DynamicBuffer_h__
