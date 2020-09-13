#include "stdafx.h"
#include "DynamicCam.h"

CDynamicCam::CDynamicCam(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CCamera(pGraphicDev), 
	m_bClick(false)
{
}

CDynamicCam::~CDynamicCam()
{
}

HRESULT CDynamicCam::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = { 20.f, 10.f, 20.f };
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	Engine::CCamera::Ready_GameObject();
	return S_OK;
}

_int CDynamicCam::Update_GameObject(const _double & dTimeDelta)
{
	Key_Input(dTimeDelta);
	
	if (true == m_bMouseFix)
	{
		Mouse_Move(dTimeDelta);
		Fix_Mouse();
	}
	Engine::CCamera::Update_GameObject(dTimeDelta);
	return S_OK;
}

_int CDynamicCam::LateUpdate_GameObject(const _double & dTimeDelta)
{
	return Engine::NO_EVENT;
}

void CDynamicCam::Key_Input(const _float & fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);


	if (Engine::KeyPressing(DIK_W))
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * 500.f * (_float)fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}
	if (Engine::KeyPressing(DIK_S))
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * 500.f * (_float)fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}
	if (Engine::KeyPressing(DIK_A))
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * 500.f * (_float)fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}
	if (Engine::KeyPressing(DIK_D))
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * 500.f * (_float)fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::KeyDown(DIK_Z))
	{
		if (true == m_bClick)
			return;

		m_bClick = true;

		if (m_bMouseFix)
			m_bMouseFix = false;
		else
			m_bMouseFix = true;
	}
	else
		m_bClick = false;

	if (!m_bMouseFix)
		return;
}

void CDynamicCam::Mouse_Move(const _float & fTimeDelta)
{
	_long	dwMouseMove = 0;
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);


	//----- Rotation Y
	POINT pt = {};
	POINT ptMouseMove = { (WINCX >> 1) , WINCY >> 1 };

	::GetCursorPos(&pt);
	::ScreenToClient(g_hWnd, &pt);

	_vec3 vRight;
	memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));
	_vec3	vLook = m_vAt - m_vEye;

	_long dwMouseY = pt.y - ptMouseMove.y;

	_matrix		matRotY;
	D3DXMatrixRotationAxis(&matRotY, &vRight, D3DXToRadian(dwMouseY / 10.f));
	D3DXVec3TransformNormal(&vLook, &vLook, &matRotY);

	m_vAt = m_vEye + vLook;

	//----- Rotation X

	_vec3		vUp = _vec3(0.f, 1.f, 0.f);

	_long dwMouseX = pt.x - ptMouseMove.x;

	_matrix		matRotX;

	vLook = m_vAt - m_vEye;
	D3DXMatrixRotationAxis(&matRotX, &vUp, D3DXToRadian(dwMouseX / 10.f));
	D3DXVec3TransformNormal(&vLook, &vLook, &matRotX);

	m_vAt = m_vEye + vLook;
}

void CDynamicCam::Fix_Mouse(void)
{
	POINT		ptMouse{ (WINCX >> 1), WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

CDynamicCam * CDynamicCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CDynamicCam*	pInstance = new CDynamicCam(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDynamicCam::Free(void)
{
	Engine::CCamera::Free();
}
