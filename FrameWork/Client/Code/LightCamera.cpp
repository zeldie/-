#include "stdafx.h"
#include "LightCamera.h"


CLightCamera::CLightCamera(LPDIRECT3DDEVICE9 pGarphicDev)
	:Engine::CCamera(pGarphicDev),
	m_pPlayerTransformCom(nullptr)
{
}


CLightCamera::~CLightCamera()
{
}

HRESULT CLightCamera::Ready_GameObject(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;
	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	return S_OK;
}

_int CLightCamera::Update_GameObject(const _double & dTimeDelta)
{
	m_vAt = *m_pPlayerTransformCom->Get_Info(Engine::INFO_POS);
	m_vEye = m_vAt + _vec3(0.f, 2500.f, 1000.f);
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	return Engine::NO_EVENT;
}

void CLightCamera::Set_PlayerTransformCom(Engine::CTransform * pTransformCom)
{
	m_pPlayerTransformCom = pTransformCom;

}

CLightCamera * CLightCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CLightCamera* pInatance = new CLightCamera(pGraphicDev);
	if (FAILED(pInatance->Ready_GameObject(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Engine::Safe_Release(pInatance);
	return pInatance;
}

void CLightCamera::Free()
{
	CCamera::Free();
}
