#include "stdafx.h"
#include "NormalObject.h"


CNormalObject::CNormalObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseObject(pGraphicDev),
	m_pBufferCom(nullptr),
	m_pTextureCom(nullptr)
{
}


CNormalObject::~CNormalObject()
{
}

HRESULT CNormalObject::Ready_GameObject()
{
	return S_OK;
}

_int CNormalObject::Update_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::Update_GameObject(dTimeDelta);
	return iExit;
}

_int CNormalObject::LateUpdate_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::LateUpdate_GameObject(dTimeDelta);
	return iExit;
}

void CNormalObject::Render_Geometry(const _double & dTimeDelta)
{
}

void CNormalObject::Free()
{
	CBaseObject::Free();
}
