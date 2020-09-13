#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex();
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_Buffer();

public:
	//UI 텍스처 UV조절 함수 -> 희정
	void VertexXControl_LeftDir(_float fSizeX); //가로

public:
	static CRcTex*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone();
	virtual void			Free();
};

END

