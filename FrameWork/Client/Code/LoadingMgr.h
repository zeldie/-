#pragma once

#include "Layer.h"
#include "Renderer.h"
class CLoadingMgr
{
	DECLARE_SINGLETON(CLoadingMgr)
private:
	explicit CLoadingMgr();
	~CLoadingMgr();

public:
	HRESULT			Ready_LoadingMgr(LPDIRECT3DDEVICE9& pGraphicDev);
	_bool			Get_StartFade();
	_bool			Get_EndFade();
	void			Set_StartFade(_bool bStartFade);
	void			Set_EndFade(_bool bEndFade);
private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_uint				m_iCurLayer;
	Engine::CRenderer*	m_pRendererCom;
};

