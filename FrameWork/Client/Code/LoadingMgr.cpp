#include "stdafx.h"
#include "LoadingMgr.h"

IMPLEMENT_SINGLETON(CLoadingMgr)
CLoadingMgr::CLoadingMgr()
	:m_pGraphicDev(nullptr),
	m_pRendererCom(nullptr)
{
}


CLoadingMgr::~CLoadingMgr()
{
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pRendererCom);
}

HRESULT CLoadingMgr::Ready_LoadingMgr(LPDIRECT3DDEVICE9& pGraphicDev)
{
	m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	m_pGraphicDev = pGraphicDev;
	Engine::Safe_AddRef(m_pGraphicDev);
	return S_OK;
}

_bool CLoadingMgr::Get_StartFade()
{
	return m_pRendererCom->Get_StartFade();
}

_bool CLoadingMgr::Get_EndFade()
{
	return m_pRendererCom->Get_EndFade();
}

void CLoadingMgr::Set_StartFade(_bool bStartFade)
{
	m_pRendererCom->Set_StartFade(bStartFade);
}

void CLoadingMgr::Set_EndFade(_bool bEndFade)
{
	m_pRendererCom->Set_EndFade(bEndFade);
}
