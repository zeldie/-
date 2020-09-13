#include "stdafx.h"
#include "MiniGameCamera.h"

#include "Management.h"
#include "Transform.h"
#include "Player.h"

CMiniGameCamera::CMiniGameCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CCamera(pGraphicDev),
	m_bClick(false),
	m_bMouseFix(true),
	m_fCamAngle(0.f),
	m_fDist(80.f),
	m_fRotX(0.f),
	m_fRotY(0.f),
	m_dTime(0),
	m_bLateInit(false),
	m_bMoving(true),
	m_pNaviMeshCom(nullptr),
	m_dwNaviIndex(0)
{
	ZeroMemory(&m_vNextEye, sizeof(_vec3));
	ZeroMemory(&m_vNextAt, sizeof(_vec3));
	ZeroMemory(&m_vLook, sizeof(_vec3));
}

CMiniGameCamera::~CMiniGameCamera()
{
}

HRESULT CMiniGameCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = { 0.f, 1500.f, -500.f };
	m_vNextEye = m_vEye;
	//m_vAt = *pAt;
	m_vAt = { 0.f, 100.f, 2000.f };
	m_vNextAt = m_vAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	m_fCamAngle = 35.f;

	m_fDist = 200.f;
	Engine::CCamera::Ready_GameObject();
	m_bLateInit = false;
	return S_OK;
}

_int CMiniGameCamera::Update_GameObject(const _double & dTimeDelta)
{
	if (!m_bLateInit)
		LateInit();

	m_dTime += dTimeDelta;

	return S_OK;
}

_int CMiniGameCamera::LateUpdate_GameObject(const _double & dTimeDelta)
{
	return Engine::NO_EVENT;
}

void CMiniGameCamera::Key_Input(const _double & dTimeDelta)
{
	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

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

void CMiniGameCamera::Mouse_Input(const _double & dTimeDelta)
{
	_long	dwMouseMove = 0;

	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);


	if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Y))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vEye - m_vAt;

		_matrix		matRot;

		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 5.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
		m_vNextEye = m_vAt + vLook;
	}


	if (dwMouseMove = Engine::MouseMove(Engine::DIMS_X))
	{
		_vec3		vUp = _vec3(0.f, 1.f, 0.f);

		_vec3	vLook = m_vEye - m_vAt;

		_matrix		matRot;

		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 5.f));

		D3DXVec3TransformCoord(&vLook, &vLook, &matRot);
		m_vNextEye = m_vAt + vLook;
	}
}

void CMiniGameCamera::Mouse_Fix()
{
	POINT		ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CMiniGameCamera::LookAtPlayer(const _double & dTimeDelta)
{
	//pPlayerTransform->Get_Info(Engine::INFO::INFO_POS, &m_vNextAt);
	//m_vNextAt.y += 40.f;
	//_matrix matTrans, matRotY, matRotX, matWorld;
	//D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixTranslation(&matTrans, 0.f, m_fDist, -m_fDist);

	//_long	dwMouseMove = 0;
	//if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Z))
	//{
	//	m_fDist -= dwMouseMove * 0.1f;
	//	if (100.f > m_fDist)
	//		m_fDist = 100.f;
	//	else if (5000.f < m_fDist)
	//		m_fDist = 5000.f;
	//}
	//if (dwMouseMove = Engine::MouseMove(Engine::DIMS_X))
	//	m_fRotY += (dwMouseMove * 0.05f);
	//if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Y))
	//{
	//	m_fRotX += (dwMouseMove * 0.05f);
	//	if (40.f < m_fRotX)
	//		m_fRotX = 40.f;
	//	else if (-55.f > m_fRotX)
	//		m_fRotX = -55.f;
	//}
	//D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fRotY));
	//D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_fRotX));
	//_matrix playerMatrix;
	//D3DXMatrixTranslation(&playerMatrix, m_vAt.x, m_vAt.y + 50.f, m_vAt.z);

	//matWorld *= matTrans * matRotX * matRotY * playerMatrix;
	//memcpy(&m_vNextEye, matWorld.m[3], sizeof(_vec3));

}

void CMiniGameCamera::LateInit()
{
	m_bLateInit = true;
	//pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"HFPlayer", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	//m_vEye = *(pPlayerTransform->Get_Info(Engine::INFO_POS));
	//m_vNextEye = m_vEye;

	//if (m_pNaviMeshCom == nullptr)
	//{
	//	m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Terrain", Engine::NAVI, Engine::ID_STATIC));
	//	m_pNaviMeshCom->Find_Index(m_dwNaviIndex, &(m_vEye));
	//	if (m_dwNaviIndex == 1000)
	//		return;
	//	m_pNaviMeshCom->Find_PosY(&m_vEye, m_dwNaviIndex, m_vEye);
	//	return;
	//}
}

CMiniGameCamera * CMiniGameCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CMiniGameCamera* pInstance = new CMiniGameCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMiniGameCamera::Free(void)
{
	Engine::CCamera::Free();
}
