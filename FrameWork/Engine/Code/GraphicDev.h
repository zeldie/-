#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	explicit	CGraphicDev();
	virtual		~CGraphicDev();

public:
	inline LPDIRECT3DDEVICE9		GetDevice() { return m_pGraphicDev;  }
	inline LPD3DXLINE				Get_Line() { return m_pLine; }
	inline DWORD					F2DW(_float f) { return *((DWORD*)&f); }
	inline void						Get_CamView(_matrix* matView);
	inline void						Get_CamProj(_matrix* matProj);
public:
	HRESULT		Ready_GraphicDev(HWND hWnd, 
								WINMODE eMode, 
								const _uint& iSizeX,
								const _uint& iSizeY,
								Engine::CGraphicDev** ppGraphicDev);

	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End();

private:
	LPDIRECT3D9				m_pSDK;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPD3DXLINE				m_pLine;
public:
	inline	virtual void Free();
};

END
#endif // GraphicDev_h__
