#ifndef Texture_h__
#define Texture_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
protected:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();


public:
	HRESULT		Ready_Prototype(const wstring& wstrPath,
								TEXTURETYPE eType,
								const _uint& iCnt);

	void		Render_Texture(const _uint& iIndex = 0);

	size_t		Get_TextureCnt() { return m_vecTexture.size(); }
	void		SetTexture(LPD3DXEFFECT& pEffect, const char* pContantName, const _uint& iIndex = 0);

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;

public:

	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const wstring& wstrPath,
									TEXTURETYPE eType,
									const _uint& iCnt = 1);
	virtual CComponent*		Clone();
	virtual void			Free();

};

END

#endif // Texture_h__
