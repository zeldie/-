#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex final : public CVIBuffer
{
private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex();
public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_Buffer();

public:
	static CCubeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

public:
	virtual void Free();
};

END