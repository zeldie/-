#include "Font.h"

USING(Engine)

Engine::CFont::CFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pFont(nullptr)
	, m_pSprite(nullptr)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CFont::~CFont(void)
{

}

HRESULT Engine::CFont::Ready_Font(const wstring& pFontType,
									const _uint& iWidth,
									const _uint& iHeight,
									const _uint& iWeight)
{
	D3DXFONT_DESC	Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.CharSet = HANGUL_CHARSET;
	//lstrcpy(Font_Desc.FaceName, pFontType); //���ڵ�-> _tchar*�϶�
	lstrcpy(Font_Desc.FaceName, pFontType.c_str());

	Font_Desc.Width  = iWidth;
	Font_Desc.Height = iHeight;
	Font_Desc.Weight = iWeight;

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &Font_Desc, &m_pFont)))
	{
		MSG_BOX("Font Create Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		MSG_BOX("Sprite Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

void Engine::CFont::Render_Font(const wstring& pString, const _vec2* pPos, D3DXCOLOR Color)
{
	RECT	rc{ _long(pPos->x), _long(pPos->y) };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString.c_str(), lstrlen(pString.c_str()), &rc, DT_NOCLIP, Color);

	m_pSprite->End();
}

CFont* Engine::CFont::Create(LPDIRECT3DDEVICE9 pGraphicDev,
							const wstring& pFontType,
							const _uint& iWidth,
							const _uint& iHeight,
							const _uint& iWeight)
{
	CFont*	pInstance = new CFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
	{
		MSG_BOX("Font Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CFont::Free()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pGraphicDev);
}

