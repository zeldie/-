#include "stdafx.h"
#include "RunGameCamera.h"

#include "Management.h"
#include "Transform.h"
#include "RunPlayer.h"

//_vec3 vCamPos = { 0.f, 0.f, 0.f };
CRunGameCamera::CRunGameCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CCamera(pGraphicDev),
	m_bClick(false),
	m_bMouseFix(true),
	m_fCamAngle(0.f),
	m_fDist(80.f),
	m_fRotX(0.f),
	m_fRotY(0.f),
	m_dTime(0),
	m_bLateInit(false),
	m_eStep(STEP1),
	m_pItemTransfrom(nullptr),
	m_fRatio(0.f),
	m_bGetItem(false)
{
	ZeroMemory(&m_vNextEye, sizeof(_vec3));
	ZeroMemory(&m_vNextAt, sizeof(_vec3));
	ZeroMemory(&m_vLook, sizeof(_vec3));
	ZeroMemory(&vLastEye, sizeof(_vec3));
}

CRunGameCamera::~CRunGameCamera()
{
}

HRESULT CRunGameCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	m_vNextEye = m_vEye;
	m_vAt = *pAt;
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

_int CRunGameCamera::Update_GameObject(const _double & dTimeDelta)
{
	if (!m_bLateInit)
		LateInit();

	Key_Input(dTimeDelta);
	
	if(m_bMouseFix)
		Mouse_Fix();

	if (m_bGetItem)
	{
		Item_Get(dTimeDelta);
	}
	else
	{
		LookAtPlayer(dTimeDelta);

		_vec3 vDir = m_vNextAt - m_vAt;
		//D3DXVec3Normalize(&vDir, &vDir);
		m_vAt += vDir * _float(dTimeDelta * 7.5f);

		vDir = m_vNextEye - m_vEye;
		//D3DXVec3Normalize(&vDir, &vDir);
		m_vEye += vDir * _float(dTimeDelta *7.5f);
		vLastEye = m_vEye;
	}
	Engine::CCamera::Update_GameObject(dTimeDelta);

	return S_OK;
}

_int CRunGameCamera::LateUpdate_GameObject(const _double & dTimeDelta)
{
	return Engine::NO_EVENT;
}

void CRunGameCamera::Key_Input(const _double & dTimeDelta)
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

	//if (!m_bMouseFix)
		//return;
}

void CRunGameCamera::Mouse_Input(const _double & dTimeDelta)
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

void CRunGameCamera::Mouse_Fix()
{
	POINT		ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CRunGameCamera::LookAtPlayer(const _double & dTimeDelta)
{
	if (Engine::KeyDown(DIK_L))
		int i = 0;

	m_vNextAt = *pPlayerTransform->Get_Info(Engine::INFO::INFO_POS) + *pPlayerTransform->Get_Info(Engine::INFO::INFO_LOOK)*1800.f + *pPlayerTransform->Get_Info(Engine::INFO::INFO_UP)*500.f;
	m_vNextEye = *pPlayerTransform->Get_Info(Engine::INFO::INFO_POS) + -*pPlayerTransform->Get_Info(Engine::INFO::INFO_LOOK)* 2000.f + *pPlayerTransform->Get_Info(Engine::INFO::INFO_UP)*1000.f;

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
	///*if (dwMouseMove = Engine::MouseMove(Engine::DIMS_X))
	//	m_fRotY += (dwMouseMove * 0.05f);
	//if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Y))
	//{
	//	m_fRotX += (dwMouseMove * 0.05f);
	//	if (40.f < m_fRotX)
	//		m_fRotX = 40.f;
	//	else if (-55.f > m_fRotX)
	//		m_fRotX = -55.f;
	//}*/
	//D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fRotY));
	//D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_fRotX));
	//_matrix playerMatrix;
	//D3DXMatrixTranslation(&playerMatrix, m_vAt.x, m_vAt.y + 50.f, m_vAt.z);

	//matWorld *= matTrans * matRotX * matRotY * playerMatrix;
	//memcpy(&m_vNextEye, matWorld.m[3], sizeof(_vec3));
}

void CRunGameCamera::Item_Get(const _double & dTimeDelta)
{

	switch (m_eStep)
	{
	case CRunGameCamera::STEP1:
	{
		_vec3 vTargetPos = *m_pItemTransfrom->Get_Info(Engine::INFO_POS);
		_vec3 vEndEye = vTargetPos - *pPlayerTransform->Get_Info(Engine::INFO_LOOK)* 250.f;

		// Eye Move
		D3DXVec3CatmullRom(&m_vEye, &(vEndEye), &(vLastEye), &(vEndEye), &(vEndEye), m_fRatio);
		// At Move
		D3DXVec3CatmullRom(&m_vAt, &(m_vNextAt), &(m_vNextAt), &(vTargetPos), &(vTargetPos), m_fRatio);

		m_fRatio += (_float)dTimeDelta*0.5f;

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP2;
		}
	}
		break;
	case CRunGameCamera::STEP2:
	{
		_vec3 vTargetPos = *m_pItemTransfrom->Get_Info(Engine::INFO_POS);

		_vec3 vStartEye = vTargetPos - *pPlayerTransform->Get_Info(Engine::INFO_LOOK)* 500.f;
		_vec3 vSpline = vTargetPos + *pPlayerTransform->Get_Info(Engine::INFO_RIGHT)*1800.f;
		_vec3 vEndEye = vTargetPos + *pPlayerTransform->Get_Info(Engine::INFO_LOOK)* 1000.f;
		// Eye Move
		D3DXVec3CatmullRom(&m_vEye, &(vSpline), &(vStartEye), &(vEndEye), &(vSpline), m_fRatio);

		m_fRatio += (_float)dTimeDelta*0.5f;

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP3;
		}
	}
		break;
	case CRunGameCamera::STEP3:
	{
		_vec3 vTargetPos = *m_pItemTransfrom->Get_Info(Engine::INFO_POS);
		// 카메라 -> 아이템
		//_vec3 viTemToCam = vTargetPos - m_vEye;
		// 아이템->플레이어 방향 백터
		//_vec3 vItemToPlayer = *pPlayerTransform->Get_Info(Engine::INFO_POS) - vTargetPos;
		// iTem + Right
		//_vec3 vRight = vTargetPos + *pPlayerTransform->Get_Info(Engine::INFO_RIGHT)*3800.f;
		//// item backword
		//_vec3 vBack = vTargetPos + *pPlayerTransform->Get_Info(Engine::INFO_LOOK);
		//// At-> item
		//_vec3 vLook = vTargetPos + *pPlayerTransform->Get_Info(Engine::INFO_LOOK)* -500.f;

		// spline1
		_vec3 vSpline1 = *pPlayerTransform->Get_Info(Engine::INFO_POS) + (*pPlayerTransform->Get_Info(Engine::INFO_LOOK) + *pPlayerTransform->Get_Info(Engine::INFO_RIGHT)*-1.f)*800.f;
		

		// Eye Move
		D3DXVec3CatmullRom(&m_vEye, &(vSpline1 + _vec3(0.f, 80.f, 0.f)), &(vTargetPos + *pPlayerTransform->Get_Info(Engine::INFO_LOOK)* 1000.f), &(vSpline1 + _vec3(0.f, 80.f, 0.f)), &(vSpline1 + _vec3(0.f, 80.f, 0.f)), m_fRatio);

		m_fRatio += (_float)dTimeDelta*0.5f;

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP4;
		}
	}
		break;
	case CRunGameCamera::STEP4:
	{
		_vec3 vTargetPos = *m_pItemTransfrom->Get_Info(Engine::INFO_POS);
		
		_vec3 vRight = vTargetPos + *pPlayerTransform->Get_Info(Engine::INFO_RIGHT)*3800.f;
		// item backword
		_vec3 vBack = vTargetPos + *pPlayerTransform->Get_Info(Engine::INFO_LOOK);
		// At-> item
		_vec3 vLook = vTargetPos + *pPlayerTransform->Get_Info(Engine::INFO_LOOK)* -500.f;

		// spline1
		_vec3 vSpline1 = *pPlayerTransform->Get_Info(Engine::INFO_POS) + (*pPlayerTransform->Get_Info(Engine::INFO_LOOK) + *pPlayerTransform->Get_Info(Engine::INFO_RIGHT)*-1.f)*800.f;
		_vec3 vSpline2 = *pPlayerTransform->Get_Info(Engine::INFO_POS) + (*pPlayerTransform->Get_Info(Engine::INFO_RIGHT)*-1.f)*3400.f;
		_vec3 vEndPos = *pPlayerTransform->Get_Info(Engine::INFO_POS) + (*pPlayerTransform->Get_Info(Engine::INFO_LOOK)*-1.f)*1250.f;

		// Eye Move
		D3DXVec3CatmullRom(&m_vEye, &(vSpline2 + _vec3(0.f, 80.f, 0.f)), &(vSpline1 + _vec3(0.f, 80.f, 0.f)), &(vEndPos + _vec3(0.f, 80.f, 0.f)), &(vSpline2 + _vec3(0.f, 80.f, 0.f)), m_fRatio);

		m_fRatio += (_float)dTimeDelta*0.5f;

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP5;
		}
	}
		break;
	case CRunGameCamera::STEP5:
	{
		m_fRatio += (_float)dTimeDelta;

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP1;
		}
	}
		break;
	default:
		break;
	}
}

void CRunGameCamera::LateInit()
{
	m_bLateInit = true;
	m_fRotX = -47.f;
	m_fRotY = 0.55f;
	pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"RunPlayer", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	m_vAt = *(pPlayerTransform->Get_Info(Engine::INFO_POS));
	m_vNextEye = m_vEye;

	pPlayerTransform->Get_Info(Engine::INFO::INFO_POS, &m_vNextAt);
	m_vNextAt.y += 40.f;
	_matrix matTrans, matRotY, matRotX, matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matTrans, 0.f, m_fDist, -m_fDist);

	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fRotY));
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_fRotX));
	_matrix playerMatrix;
	D3DXMatrixTranslation(&playerMatrix, m_vAt.x, m_vAt.y + 50.f, m_vAt.z);

	matWorld *= matTrans * matRotX * matRotY * playerMatrix;
	memcpy(&m_vNextEye, matWorld.m[3], sizeof(_vec3));
	
	m_pItemTransfrom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"RunWeapon", Engine::TRANSFORM, Engine::ID_DYNAMIC));
}

CRunGameCamera * CRunGameCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CRunGameCamera* pInstance = new CRunGameCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunGameCamera::Free(void)
{
	Engine::CCamera::Free();
}
