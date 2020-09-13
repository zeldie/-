#include "stdafx.h"
#include "CustomizeCamera.h"

CCustomizeCamera::CCustomizeCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CCamera(pGraphicDev),
	m_pTargetTransform(nullptr),
	m_bLateInit(false),
	m_fUpDown(0.f),
	m_fEyeY(0.f),
	m_fZoom(0.f)
{
}

CCustomizeCamera::~CCustomizeCamera()
{
}

HRESULT CCustomizeCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = { 20.f, 10.f, 20.f };
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	m_bTestHead = false;
	m_bTestBody = false;

	Engine::CCamera::Ready_GameObject();
	return S_OK;
}

_int CCustomizeCamera::Update_GameObject(const _double & dTimeDelta)
{
	if (!m_bLateInit)
		LateInit();

	//if (m_bTestHead)
	//{

	//}
	//else if (m_bTestBody)
	//{

	//}
	//else

	MouseInput(dTimeDelta);


	/*if (Engine::KeyDown(DIK_Q))
	{
		m_bTestHead = !m_bTestHead;

		_vec3 vDir = m_vAt - m_vEye;
		D3DXVec3Normalize(&vDir, &vDir);

		m_fZoom = 120.f;

		m_vEye = vDir*m_fZoom;

		m_vEye.y = 200.f;
		m_vAt.y = 150.f;
	}

	if (Engine::KeyDown(DIK_W))
	{
		m_bTestBody = !m_bTestBody;
	}*/

	if (Engine::KeyPressing(DIK_Z)) // Cam_Reset
	{
		m_vAt = { -40.f,100.f,0.f };
		m_vEye = { 65.f,135.f,175.f };
		m_vEye.y = 150.f + m_fUpDown + m_fEyeY;
		m_vAt.y = 100.f + m_fUpDown;
	}

	Engine::CCamera::Update_GameObject(dTimeDelta);
	return S_OK;
}

_int CCustomizeCamera::LateUpdate_GameObject(const _double & dTimeDelta)
{
	return Engine::NO_EVENT;
}

void CCustomizeCamera::LateInit()
{
	m_bLateInit = true;

	m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Body", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	m_vAt = { -40.f,100.f,0.f };
	m_vEye = { 65.f,135.f,175.f };
	m_vEye.y = 150.f + m_fUpDown + m_fEyeY;
	m_vAt.y = 100.f + m_fUpDown;
	//m_vNextEye = m_vEye;
}

void CCustomizeCamera::MouseInput(const _double & dTimeDelta)
{
	POINT pt = {};
	_long	dwMouseMove = 0;

	::GetCursorPos(&pt);
	::ScreenToClient(g_hWnd, &pt);

	if (WINCX - 400.f <= pt.x) // 여기에 ui 클릭 함수 배치
		return; // 얼굴/헤어, 몸 버튼 클릭했을 때 바디 센터, 머리 센터로 이동하는 코드 구현해야 함



	if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Z))
	{
		_vec3 vPlayer = { 0.f, 50.f, 20.f };

		_vec3 vDir = m_vAt - m_vEye;
		_float fDist = D3DXVec3Length(&(m_vAt - m_vEye)) ;

		m_fZoom += dwMouseMove*0.1f;


		D3DXVec3Normalize(&vDir, &vDir);

		_float fNDist = D3DXVec3Length(&(m_vAt - (m_vEye + vDir*m_fZoom)));
		if (200 <= fNDist)
			m_fZoom = 0.f;
		else if (120 >= fNDist)
			m_fZoom = 0.f;
		m_vEye += vDir*m_fZoom;
	}

	if (Engine::MousePressing(Engine::DIM_RB))
	{
		if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Y))
		{
			m_fUpDown -= (_float)(dwMouseMove*0.1f);

			if (50.f <= (m_fUpDown - (_float)dwMouseMove*0.1f))
				m_fUpDown = 50.f;

			if (-50.f >= (m_fUpDown - (_float)dwMouseMove*0.1f))
				m_fUpDown = -50.f;
		}

		if (dwMouseMove = Engine::MouseMove(Engine::DIMS_X))
		{
			m_pTargetTransform->Rotation(Engine::ROT_Y, -dwMouseMove*0.2f);

		}
	}
	if (Engine::MousePressing(Engine::DIM_LB))
	{
		if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Y))
		{
			m_fEyeY -= (_float)dwMouseMove*0.2f;

			if (50.f <= m_fEyeY)
				m_fEyeY = 50.f;

			if (-75.f >= m_fEyeY)
				m_fEyeY = -75.f;
		}
	}

	m_vEye.y = 150.f + m_fUpDown + m_fEyeY;
	m_vAt.y = 100.f + m_fUpDown;
}

CCustomizeCamera * CCustomizeCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CCustomizeCamera*	pInstance = new CCustomizeCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCustomizeCamera::Free(void)
{
	Engine::CCamera::Free();
}
