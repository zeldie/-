#include "stdafx.h"
#include "UIObject.h"
#include "Calculator.h"

CUIObject::CUIObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseObject(pGraphicDev),
	m_pScreenTexBufferCom(nullptr),
	m_pTextureCom(nullptr),
	m_bRenderUI(false)
{
}


CUIObject::~CUIObject()
{
}

HRESULT CUIObject::Ready_GameObject()
{
	return S_OK;
}

_int CUIObject::Update_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::Update_GameObject(dTimeDelta);
	return iExit;
}

_int CUIObject::LateUpdate_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::LateUpdate_GameObject(dTimeDelta);
	return iExit;
}

void CUIObject::Render_Geometry(const _double & dTimeDelta)
{
}

void CUIObject::Free()
{
	CBaseObject::Free();
}
