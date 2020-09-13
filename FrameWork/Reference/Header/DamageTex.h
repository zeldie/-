#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CDamageTex : public CVIBuffer
{
private:
	explicit CDamageTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDamageTex(const CDamageTex& rhs);
	virtual ~CDamageTex();
public:
	virtual HRESULT				Ready_Prototype();
	virtual HRESULT				Ready_Buffer();
public:
	static CDamageTex*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*			Clone();
	virtual void				Free();
};

END

