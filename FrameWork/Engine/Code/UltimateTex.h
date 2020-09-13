#pragma once

#include "UltimateBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CUltimateTex : public CUltimateBuffer
{
private:
	explicit CUltimateTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUltimateTex(const CUltimateTex& rhs);
	virtual ~CUltimateTex();
public:
	virtual HRESULT				Ready_Prototype();
	virtual HRESULT				Ready_Buffer();
	virtual HRESULT				Render_Buffer();

public:
	static CUltimateTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*			Clone();
	virtual void				Free();
};

END

