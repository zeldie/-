#include "stdafx.h"
#include "StaticMeshObject.h"


CStaticMeshObject::CStaticMeshObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseObject(pGraphicDev),
	m_pStaticMeshCom(nullptr)
{
}


CStaticMeshObject::~CStaticMeshObject()
{
}

HRESULT CStaticMeshObject::Ready_GameObject()
{
	return S_OK;
}

_int CStaticMeshObject::Update_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::Update_GameObject(dTimeDelta);
	return iExit;
}

_int CStaticMeshObject::LateUpdate_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::LateUpdate_GameObject(dTimeDelta);
	return iExit;
}

void CStaticMeshObject::Render_Depth(const _double & dTimeDelta)
{
}

void CStaticMeshObject::Render_Geometry(const _double & dTimeDelta)
{
	CBaseObject::Render_Geometry(dTimeDelta);
}

void CStaticMeshObject::Render_PostEffect(const _double & dTimeDelta)
{
}

void CStaticMeshObject::Free()
{
	CBaseObject::Free();
}
