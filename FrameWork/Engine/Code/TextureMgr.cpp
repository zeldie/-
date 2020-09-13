#include "TextureMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTextureMgr)
CTextureMgr::CTextureMgr()
	:m_pGraphicDev(nullptr)
{
}


CTextureMgr::~CTextureMgr()
{
	Free();
}

HRESULT CTextureMgr::Ready_TextureMgr(const wstring& wstrPath, const _uint& iIndex, LPDIRECT3DDEVICE9& pGraphicDev, TEXTURETYPE eTextureType)
{
	if (iIndex <= 0)
		return E_FAIL;
	m_vecTexture.reserve(iIndex);
	m_pGraphicDev = pGraphicDev;
	Safe_AddRef(m_pGraphicDev);
	IDirect3DBaseTexture9* pTexture = nullptr;
	_tchar szFileName[MAX_STR] = L"";

	for (_uint i = 0; i < iIndex; ++i)
	{
		swprintf_s(szFileName, wstrPath.c_str(), i);
		switch (eTextureType)
		{
		case Engine::TEX_NORMAL:
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		case Engine::TEX_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		case Engine::TEX_END:
			break;
		default:
			break;
		}
		m_vecTexture.push_back(pTexture);
	}
	return S_OK;
}


void CTextureMgr::SetTexture(LPD3DXEFFECT & pEffect, const char* pContantName,  const _uint & iIndex)
{
	if (m_vecTexture.size() < iIndex)
		return;
	pEffect->SetTexture(pContantName, m_vecTexture[iIndex]);
}

void CTextureMgr::Free()
{
	Safe_Release(m_pGraphicDev);
	for (auto& rTextures : m_vecTexture)
		Safe_Release(rTextures);
	m_vecTexture.clear();
}
