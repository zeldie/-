#ifndef TriCol_h__
#define TriCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol : public CVIBuffer
{
private:
	explicit CTriCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTriCol(const CTriCol& rhs);
	virtual ~CTriCol();

public:
	virtual HRESULT		Ready_Prototype();
	virtual HRESULT		Ready_Buffer();

public:
	static CTriCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();
	virtual void		Free();
};


END
#endif // RcCol_h__
