#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_fFovY(0.f),
	m_fAspect(0.f),
	m_fNear(0.f),
	m_fFar(0.f),
	m_bMouseFix(true),
	m_bMoving(true)
{
	ZeroMemory(&m_vEye, sizeof(_vec3));
	ZeroMemory(&m_vAt, sizeof(_vec3));
	ZeroMemory(&m_vUp, sizeof(_vec3));
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matOrthoView);
}

Engine::CCamera::~CCamera()
{

}

HRESULT Engine::CCamera::Ready_GameObject()
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int Engine::CCamera::Update_GameObject(const _double& dTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	D3DXMatrixOrthoLH(&m_matOrthoView, 1280, 720, 0.f, 1.f);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	return NO_EVENT;
}

void Engine::CCamera::Free()
{
	CGameObject::Free();
}

