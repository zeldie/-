#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeCol : public CVIBuffer
{
private:
	explicit CCubeCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeCol(const CCubeCol& rhs);
	virtual ~CCubeCol();

public:
	virtual HRESULT		Ready_Prototype();
	virtual HRESULT		Ready_Buffer();

public:
	static CCubeCol*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();
	virtual void		Free();
};

END
#endif // CubeTex_h__
