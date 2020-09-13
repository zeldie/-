#include "FontMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFontMgr)

Engine::CFontMgr::CFontMgr()
{

}

Engine::CFontMgr::~CFontMgr()
{
	Free();
}

HRESULT Engine::CFontMgr::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFontTag, const wstring& pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CFont*		pFont = nullptr;

	pFont = Find_Font(wstrFontTag);
	if (nullptr != pFont)
		return E_FAIL;

	pFont = CFont::Create(pGraphicDev, pFontType, iWidth, iHeight, iWeight);
	if (pFont == nullptr)
		return E_FAIL;
	m_mapFont.emplace(wstrFontTag, pFont);

	return S_OK;
}

void Engine::CFontMgr::Render_Font(const wstring& wstrFontTag, const wstring& pString, const _vec2* pPos, D3DXCOLOR Color)
{
	CFont*	pFont = Find_Font(wstrFontTag);
	if (pFont == nullptr)
		return;
	pFont->Render_Font(pString, pPos, Color);

}

CFont* Engine::CFontMgr::Find_Font(const wstring& wstrFontTag)
{
	auto	iter = m_mapFont.find(wstrFontTag);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void Engine::CFontMgr::Free()
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}

