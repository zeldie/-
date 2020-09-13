#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CTextureMgr : public CBase
{
	DECLARE_SINGLETON(CTextureMgr)
private:
	explicit CTextureMgr();
	virtual ~CTextureMgr();

public:
	HRESULT Ready_TextureMgr(const wstring& wstrPath, const _uint& iIndex, LPDIRECT3DDEVICE9& pGrahicDev, TEXTURETYPE eTextureType = TEX_NORMAL);
	void	SetTexture(LPD3DXEFFECT& pEffect, const char* pContantName, const _uint& iIndex);
private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	LPDIRECT3DDEVICE9					m_pGraphicDev;

public:
	virtual void Free();
};

END