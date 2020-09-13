#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
{
	_uint iContainerSize = _uint(rhs.m_vecTexture.size());
	m_vecTexture.reserve(iContainerSize);

	m_vecTexture = rhs.m_vecTexture;

	for (_uint i = 0; i < iContainerSize; ++i)
		m_vecTexture[i]->AddRef();

}

Engine::CTexture::~CTexture(void)
{

}

HRESULT Engine::CTexture::Ready_Prototype(const wstring& wstrPath, TEXTURETYPE eType, const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9*		pTexture = nullptr;
	_tchar		szFileName[MAX_STR] = L"";
	
	for (_uint i = 0; i < iCnt; ++i)
	{
		swprintf_s(szFileName, wstrPath.c_str(), i);

		switch (eType)
		{
		case TEX_NORMAL:
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;

		case TEX_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}

void Engine::CTexture::Render_Texture(const _uint& iIndex)
{
	if (m_vecTexture.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

CComponent* Engine::CTexture::Clone()
{
	return new CTexture(*this);
}

CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath, TEXTURETYPE eType, const _uint& iCnt )
{
	CTexture*		pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Prototype(wstrPath, eType, iCnt)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CTexture::Free(void)
{

	_uint iContainerSize = _uint(m_vecTexture.size());

	for (_uint i = 0; i < iContainerSize; ++i)
		Safe_Release(m_vecTexture[i]);

	m_vecTexture.clear();
	CComponent::Free();
}

void Engine::CTexture::SetTexture(LPD3DXEFFECT& pEffect, const char* pContantName, const _uint& iIndex)
{
	if (m_vecTexture.size() < iIndex)
		return;

	pEffect->SetTexture(pContantName, m_vecTexture[iIndex]);
}

