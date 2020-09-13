#ifndef FontMgr_h__
#define FontMgr_h__

#include "Font.h"

BEGIN(Engine)

class ENGINE_DLL CFontMgr : public CBase
{
	DECLARE_SINGLETON(CFontMgr)

private:
	explicit CFontMgr();
	virtual ~CFontMgr();

public:
	HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
						const wstring& wstrFontTag,
						const wstring& pFontType,
						const _uint& iWidth,
						const _uint& iHeight,
						const _uint& iWeight);

	void	Render_Font(const wstring& wstrFontTag,
						const wstring& pString,
						const _vec2* pPos, 
						D3DXCOLOR Color);

private:
	CFont*		Find_Font(const wstring& wstrFontTag);

private:
	map<wstring, CFont*>		m_mapFont;

public:
	virtual void Free(void);
};

END
#endif // FontMgr_h__
