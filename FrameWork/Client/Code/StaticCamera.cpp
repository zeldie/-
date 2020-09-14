#include "stdafx.h"
#include "StaticCamera.h"

#include "Management.h"
#include "Transform.h"
#include "Player.h"

_vec3 vCamPos = { 0.f, 0.f, 0.f };
CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CCamera(pGraphicDev),
	m_bClick(false),
	m_bActionZoom(false),
	m_bRecov(true),
	m_bRage(false),
	m_bCutScene(false),
	m_bShaking(false),
	m_bFlag(false),
	m_bTopView(false),
	m_bEndCutScene(false),
	m_fRatio(0.f),
	m_fCamAngle(0.f),
	m_fDist(80.f),
	m_fRotX(0.f),
	m_fRotY(0.f),
	m_fUpDown(0.f),
	m_dTime(0),
	m_dbShakingTime(0.0),
	m_iShakingCnt(0),
	m_bLateInit(false),
	m_pNaviMeshCom(nullptr),
	m_pTargetTransform(nullptr),
	m_dwNaviIndex(0),
	m_eWeapon(CPlayer::TWOHANDSWORD)
{
	ZeroMemory(&m_vNextEye, sizeof(_vec3));
	ZeroMemory(&m_vNextAt, sizeof(_vec3));
	ZeroMemory(&m_vLook, sizeof(_vec3));
	ZeroMemory(&m_vAtUp, sizeof(_vec3));
	ZeroMemory(&vAostlePos, sizeof(_vec3));
	m_eLevel = LEVEL1;
}

CStaticCamera::~CStaticCamera()
{
}

HRESULT CStaticCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = { 20.f, 10.f, 20.f };
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
	m_bCutScene = false;
	return S_OK;
}

_int CStaticCamera::Update_GameObject(const _double & dTimeDelta)
{
	if (!m_bLateInit)
		LateInit();

	Key_Input(dTimeDelta);
	if (m_bMouseFix)
		Mouse_Fix();

	m_dTime += dTimeDelta;
	if (Engine::KeyDown(DIK_H))
	{
		int i = 0;
	}

	if (Engine::KeyDown(DIK_J))
	{
	}


	//if (m_bFlag)
	//{
	//	CutScene_Flag(dTimeDelta);
	//	return S_OK;
	//}


	if (m_bCutScene)
	{
		if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_BOSS)
		{
			CutScene_Belatos(dTimeDelta);
			return S_OK;
		}
		else if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_SHOOTING)
		{

			CutScene_Apostle(dTimeDelta);
			return S_OK;
		}
	}

	// m_bTopView cam
	if (m_bTopView)
	{
		if (CUIMgr::UITYPE_SHOOTING != CUIMgr::GetInstance()->Get_UIType())
			m_bTopView = false;

		Shooting_Camera(dTimeDelta);
		return S_OK;
	}

	if (m_bEndCutScene)
	{
		EndCutScene_APostle(dTimeDelta);
		return S_OK;
	}

	if (m_bRageCam)
	{
		// RageCam
		Rage_Action(dTimeDelta);
		return S_OK;
	}

	if (!m_bActionZoom && !m_bRecov)
	{
		Reset_Camera(dTimeDelta);		
	}

	if (m_bActionZoom)
	{
		if (m_bRecov)
			m_bRecov = false;
		Action_Zoom(dTimeDelta);
		return S_OK;
	}

	if (m_bShaking)
	{
		Shaking_Camera(dTimeDelta);
		return S_OK;
	}

	if (!CUIMgr::GetInstance()->Get_UI_Working())
	{
		if (m_bRecov)
		{
			m_bSetComplet = false;
			if (m_bMoving)
			{
				Normal_Update(dTimeDelta);
			}
		}
	}


	//³ªÁß¿¡ Áö¿ï°ÅÀÓ - ÀÎ±Õ
	vCamPos = m_vEye;
	return S_OK;
}

_int CStaticCamera::LateUpdate_GameObject(const _double & dTimeDelta)
{
	return Engine::NO_EVENT;
}

void CStaticCamera::Shaking_Camera(const _double & dTimeDelta)
{
	m_dbShakingTime += dTimeDelta;

	//{ // High
	//	const _double m_dbDelay = 0.01;
	//	const _float m_fPower = 0.5f;
	//  const _int iEndCnt = 4;
	//}

	//low
		const _double m_dbDelay = 0.01;
		const _float m_fPower = 0.15f;
		const _int iEndCnt = 4;

	switch (m_eLevel)
	{
	case CStaticCamera::LEVEL1:
	{
		if (m_dbDelay <= m_dbShakingTime)
		{
			m_dbShakingTime = 0.0;
			m_fRotY += m_fPower;
			m_fRotX += m_fPower;
			m_eLevel = LEVEL2;
		}
	}
		break;
	case CStaticCamera::LEVEL2:
		if (m_dbDelay <= m_dbShakingTime)
		{
			m_dbShakingTime = 0.0;
			m_fRotY -= m_fPower;
			m_fRotX -= m_fPower;
			m_eLevel = LEVEL3;
		}
		break;
	case CStaticCamera::LEVEL3:
	{
		if (m_dbDelay <= m_dbShakingTime)
		{
			m_dbShakingTime = 0.0;
			m_fRotY += m_fPower;
			m_fRotX -= m_fPower;
			m_eLevel = LEVEL4;
		}
	}
		break;
	case CStaticCamera::LEVEL4:
	{
		if (m_dbDelay <= m_dbShakingTime)
		{
			m_dbShakingTime = 0.0;
			m_fRotY -= m_fPower;
			m_fRotX += m_fPower;
			m_eLevel = LEVEL5;
		}
	}
		break;
	case CStaticCamera::LEVEL5:
	{
		if (m_dbDelay <= m_dbShakingTime)
		{
			m_dbShakingTime = 0.0;
			m_fRotY -= m_fPower;
			m_fRotX += m_fPower;
			m_eLevel = LEVEL6;
		}
	}
	break;
	case CStaticCamera::LEVEL6:
	{
		if (m_dbDelay <= m_dbShakingTime)
		{
			m_dbShakingTime = 0.0;
			m_fRotY += m_fPower;
			m_fRotX -= m_fPower;
			m_eLevel = LEVEL7;
		}
	}
	break;
	case CStaticCamera::LEVEL7:
	{
		if (m_dbDelay <= m_dbShakingTime)
		{
			m_dbShakingTime = 0.0;
			m_fRotY -= m_fPower;
			m_fRotX -= m_fPower;
			m_eLevel = LEVEL8;
		}
	}
	break;
	case CStaticCamera::LEVEL8:
	{
		if (m_dbDelay <= m_dbShakingTime)
		{
			m_dbShakingTime = 0.0;
			m_fRotY += m_fPower;
			m_fRotX += m_fPower;
			m_eLevel = LEVEL9;
		}
	}
	break;
	case CStaticCamera::LEVEL9:
	{
		++m_iShakingCnt;
		m_eLevel = LEVEL1;
		if (iEndCnt <= m_iShakingCnt)
		{
			m_iShakingCnt = 0;
			m_bShaking = false;
		}
	}
	break;

	default:
		break;
	}
	
	LookAtPlayer(dTimeDelta);
	m_vEye = m_vNextEye;
	m_vAt = m_vNextAt;

	Engine::CCamera::Update_GameObject(dTimeDelta);
}

void CStaticCamera::Normal_Update(const _double & dTimeDelta)
{
	LookAtPlayer(dTimeDelta);

	vDir = m_vNextAt - m_vAt;
	m_vAt += vDir * _float(dTimeDelta * 10);
	vDir = m_vNextEye - m_vEye;
	m_vEye += vDir * _float(dTimeDelta * 10);

	//m_pNaviMeshCom->Find_Index(m_dwNaviIndex, &m_vEye);
	//m_vEye = m_pNaviMeshCom->Move_OnCamNavi(&m_vEye, &(vDir * 50.f * (_float)dTimeDelta), m_dwNaviIndex, true);

	Engine::CCamera::Update_GameObject(dTimeDelta);
}

void CStaticCamera::Action_Zoom(const _double & dTimeDelta)
{
	Mouse_Input(dTimeDelta);
	LookAtPlayer(dTimeDelta);

	_vec3 vPlayer = *pPlayerTransform->Get_Info(Engine::INFO_POS);
	_vec3 vLook;// = *pPlayerTransform->Get_Info(Engine::INFO_LOOK);
	_vec3 vPlayerRight = *pPlayerTransform->Get_Info(Engine::INFO_RIGHT);

	pPlayerTransform->Get_Info(Engine::INFO::INFO_LOOK, &vLook);
	m_vLook = vPlayer + vPlayerRight*75.f - vLook*400.f + _vec3(0.f, 75.f, 0.f);

	if (25.f <= D3DXVec3Length(&(m_vEye - m_vLook)) && !m_bSetComplet)
	{
		vDir = m_vLook - m_vEye;
		D3DXVec3Normalize(&vDir, &vDir);
		m_vEye += vDir*(_float)dTimeDelta*550.f;//vPlayer + vPlayerRight*50.f - vLook*150.f;
	}
	else
	{
		m_bSetComplet = true;
		m_vEye = m_vLook;// -m_vAtUp*0.5f;
	}
	//m_vEye.y += 75.f;

	vLook *= 2500.f;
	m_vAt = vPlayer + vLook + m_vAtUp;
	m_fEyeDist = D3DXVec3Length(&(m_vNextEye - m_vEye));
	m_fAtDist = D3DXVec3Length(&(m_vNextAt - m_vAt));

	Engine::CCamera::Update_GameObject(dTimeDelta);
}

void CStaticCamera::Reset_Camera(const _double & dTimeDelta)
{
	LookAtPlayer(dTimeDelta);
	m_dbRecovTime += dTimeDelta;
	_vec3 vPlayer = *pPlayerTransform->Get_Info(Engine::INFO_POS);
	_vec3 vAtDir = m_vNextAt - m_vAt;
	D3DXVec3Normalize(&vAtDir, &vAtDir);
	m_vAt += vAtDir * _float(dTimeDelta*2.0 *m_fAtDist);


	_vec3 vEyeDir = m_vNextEye - m_vEye;
	D3DXVec3Normalize(&vEyeDir, &vEyeDir);
	m_vEye += vEyeDir * _float(dTimeDelta*2.0 * m_fEyeDist);

	Engine::CCamera::Update_GameObject(dTimeDelta);
	if (0.4 <= m_dbRecovTime)
	{
		m_dbRecovTime = 0.0;
		m_bRecov = true;
		m_vAtUp.y = 0.f;
	}
}

void CStaticCamera::Rage_Action(const _double & dTimeDelta)
{
	switch (m_eWeapon)
	{
	case CBasePlayer::COMMON:
		break;
	case CBasePlayer::DUALSWORD:
		DS_Rage(dTimeDelta);
		break;
	case CBasePlayer::TWOHANDSWORD:
		TS_Rage(dTimeDelta);
		break;
	case CBasePlayer::ORB:
		OB_Rage(dTimeDelta);
		break;
	case CBasePlayer::LONGBOW:
		LB_Rage(dTimeDelta);
		break;
	default:
		break;
	}

	Engine::CCamera::Update_GameObject(dTimeDelta);
}

void CStaticCamera::Set_RageCam(CPlayer::PLAYER_WEAPONTYPE eWeapon)
{
	m_eWeapon = eWeapon;
	m_bRageCam = true;
}

void CStaticCamera::TS_Rage(const _double & dTimeDelta)
{
	// Ts-Rage
	_vec3 vPlayerPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);

	_vec3 vPlayerLook = *pPlayerTransform->Get_Info(Engine::INFO_LOOK)*500.f;
	_vec3 vPlayerRight =*pPlayerTransform->Get_Info(Engine::INFO_RIGHT)*500.f;
	_vec3 vPlayerLeft = -vPlayerRight;
	
	m_vAt = vPlayerPos + _vec3(0.f, 40.f, 0.f);
	
	if (!m_bRage)
	{
							// ´«						°î¼±1											½ÃÀÛÁ¡									³¡Á¡												°î¼±2								ÁøÇà·ü
		D3DXVec3CatmullRom(&m_vEye, &(vPlayerRight + vPlayerPos + _vec3(0.f, 30.f, 0.f)), &(vPlayerRight - vPlayerLook + vPlayerPos), &(vPlayerLook + vPlayerPos + _vec3(0.f, 80.f, 0.f)), &(vPlayerRight + vPlayerLook + vPlayerPos + _vec3(0.f, 60.f, 0.f)), m_fRatio);
		m_fRatio += (_float)dTimeDelta*1.3f;
	}
	else
	{
							// ´«						°î¼±1											½ÃÀÛÁ¡											³¡Á¡									°î¼±2									ÁøÇà·ü
		D3DXVec3CatmullRom(&m_vEye , &(vPlayerLeft + vPlayerPos + _vec3(0.f, 110.f, 0.f)), &(vPlayerLook + vPlayerPos + _vec3(0.f, 80.f, 0.f)), &(vPlayerLeft - vPlayerLook + vPlayerPos + _vec3(0.f, 160.f, 0.f)), &(vPlayerLeft + vPlayerLook + vPlayerPos + _vec3(0.f, 160.f, 0.f)), m_fRatio);
		m_fRatio += (_float)dTimeDelta*1.5f;
	}

	if (1.f <= m_fRatio)
	{
		if (m_bRage)
		{
			m_vEye = m_vNextEye;
			m_bRageCam = false;
			m_bRage = false;
			m_fRatio = 0.f;
			return;
		}
		m_fRatio = 0.f;
		m_bRage = true;
	}
}

void CStaticCamera::DS_Rage(const _double & dTimeDelta)
{
	// global
	_vec3 vPlayerPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
	_vec3 vPlayerLook = *pPlayerTransform->Get_Info(Engine::INFO_LOOK);
	_vec3 vPlayerRight = *pPlayerTransform->Get_Info(Engine::INFO_RIGHT);
	_vec3 vPlayerLeft = -vPlayerRight;

	//ds
	switch (m_eStep)
	{
	case CStaticCamera::STEP1:
	{
		D3DXVec3CatmullRom(&m_vEye, &(vPlayerLook*-1500.f + vPlayerPos + _vec3(0.f, 60.f, 0.f)), &(vPlayerRight*500.f + vPlayerPos), &(vPlayerLeft*500.f + vPlayerPos + _vec3(0.f, 80.f, 0.f)), &(vPlayerLook*-1500.f + vPlayerPos + _vec3(0.f, 60.f, 0.f)), m_fRatio);
		m_fRatio += (_float)dTimeDelta;

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP2;
			//m_bRageCam = false;
		}
	}
	break;
	case CStaticCamera::STEP2:
	{
		D3DXVec3CatmullRom(&m_vEye, &(vPlayerLook*-1500.f + vPlayerPos + _vec3(0.f, 160.f, 0.f)), &(vPlayerLeft*500.f + vPlayerPos + _vec3(0.f, 80.f, 0.f)), &(vPlayerRight*500.f + vPlayerPos + _vec3(0.f, 160.f, 0.f)), &(vPlayerLook*-1500.f + vPlayerPos + _vec3(0.f, 160.f, 0.f)), m_fRatio);
		m_fRatio += (_float)dTimeDelta;
		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP1;
			m_bRageCam = false;
		}
	}
	break;
	case CStaticCamera::STEP3:
	{

	}
	break;
	case CStaticCamera::STEP4:
	{

	}
	break;
	case CStaticCamera::STEP5:
	{

	}
	break;
	default:
		break;
	}
}

void CStaticCamera::LB_Rage(const _double & dTimeDelta)
{
	// global
	_vec3 vPlayerPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
	_vec3 vPlayerLook = *pPlayerTransform->Get_Info(Engine::INFO_LOOK);
	_vec3 vPlayerRight = *pPlayerTransform->Get_Info(Engine::INFO_RIGHT);
	_vec3 vPlayerLeft = -vPlayerRight;

	//LB
	_vec3 vSpline = vPlayerPos + vPlayerLook*2500.f;
	_vec3 vSpline2 = vPlayerPos + _vec3(0.f, 85.f, 0.f) + vPlayerLeft;
	_vec3 vSpline3 = vPlayerPos + _vec3(0.f, 140.f, 0.f) + vPlayerLook*1500.f - vPlayerRight*1500.f;

	switch (m_eStep)
	{
	case CStaticCamera::STEP1:
	{
		D3DXVec3CatmullRom(&m_vEye, &(vSpline), &(vPlayerPos + vPlayerLook*500.f + vPlayerRight*150.f + _vec3(0.f, 30.f, 0.f)), &(vPlayerPos + vPlayerLook*500.f + vPlayerLeft*150.f + _vec3(0.f, 30.f, 0.f)), &(vSpline), m_fRatio);
		m_fRatio += (_float)dTimeDelta*0.4f;

		if (0.5f > m_fRatio)
			m_fUpDown += 100.f*(_float)dTimeDelta * 0.4f;
		else
			m_fUpDown -= 100.f*(_float)dTimeDelta * 0.4f;

		m_vAt = vPlayerPos + _vec3(0.f, 40.f + m_fUpDown, 0.f);

		if (1.f <= m_fRatio)
		{
			m_fUpDown = 0.f;
			m_fRatio = 0.f;
			m_eStep = STEP2;
		}	
	}
		break;
	case CStaticCamera::STEP2:
	{
		D3DXVec3CatmullRom(&m_vEye, &(vSpline2), &(vPlayerPos + vPlayerLook*500.f + vPlayerLeft*150.f + _vec3(0.f, 30.f, 0.f)), &(vPlayerPos - vPlayerLook*500.f + _vec3(0.f, 130.f, 0.f) + vPlayerLeft*500.f), &(vSpline2), m_fRatio);
		m_fRatio += (_float)dTimeDelta;

		if (1.f <= m_fRatio)
		{
			m_fUpDown = 0.f;
			m_fRatio = 0.f;
			m_eStep = STEP3;
		}
	}
		break;
	case CStaticCamera::STEP3:
	{
		D3DXVec3CatmullRom(&m_vEye, &(vSpline3), &(vPlayerPos - vPlayerLook*500.f + _vec3(0.f, 130.f, 0.f) + vPlayerLeft*500.f), &(vPlayerPos + _vec3(0.f, 130.f, 0.f) + vPlayerRight*500.f), &(vSpline3), m_fRatio);

		m_vAt = vPlayerPos  + vPlayerLook*m_fRatio*1800.f;

		m_fRatio += (_float)dTimeDelta;
		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP4;
			vTest = vPlayerPos - m_vAt + _vec3(0.f,40.f,0.f);
			vDir = (vPlayerPos + vPlayerLook*250.f + vPlayerRight*250.f) - m_vEye + _vec3(0.f, 140.f, 0.f);
		}
	}
		break;
	case CStaticCamera::STEP4:
	{
		m_fRatio += (_float)dTimeDelta;

		m_vAt += vTest*(_float)dTimeDelta;
		m_vEye += vDir*(_float)dTimeDelta;
		if (1.0 <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP5;
		}
	}
		break;
	case CStaticCamera::STEP5:
	{
		m_vAt = vPlayerPos + _vec3(0.f, 40.f, 0.f);
		//m_vEye = (vPlayerPos + vPlayerLook*250.f + vPlayerLeft*250.f) + _vec3(0.f, 140.f, 0.f);

		D3DXVec3CatmullRom(&m_vEye, &(vPlayerPos + vPlayerLook*500.f + _vec3(0.f, 700.f, 0.f)), &((vPlayerPos + vPlayerLook*250.f + vPlayerLeft*250.f) + _vec3(0.f, 140.f, 0.f)), &(vPlayerPos + vPlayerLook*250.f + vPlayerRight*250.f + _vec3(0.f, 140.f, 0.f)), &(vPlayerPos + vPlayerLook*500.f + _vec3(0.f, 700.f, 0.f)), m_fRatio);

		m_fRatio += (_float)dTimeDelta*0.75f;
		if (1.0 <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP1;
			m_bRageCam = false;
		}
	}
		break;
	default:
		break;
	}
}

void CStaticCamera::OB_Rage(const _double & dTimeDelta)
{
	// global
	_vec3 vPlayerPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
	_vec3 vPlayerLook = *pPlayerTransform->Get_Info(Engine::INFO_LOOK);
	_vec3 vPlayerRight = *pPlayerTransform->Get_Info(Engine::INFO_RIGHT);
	_vec3 vPlayerLeft = -vPlayerRight;

	//LB
	_vec3 vSpline = vPlayerPos + vPlayerLook*2500.f;
	_vec3 vSpline2 = vPlayerPos + vPlayerLeft*500.f;

	switch (m_eStep)
	{
	case CStaticCamera::STEP1:
	{
		D3DXVec3CatmullRom(&m_vEye, &(vSpline), &(vPlayerPos + vPlayerLook*500.f + vPlayerRight*150.f + _vec3(0.f, 30.f, 0.f)), &(vPlayerPos + vPlayerLook*500.f + vPlayerLeft*150.f + _vec3(0.f, 30.f, 0.f)), &(vSpline), m_fRatio);
		m_fRatio += (_float)dTimeDelta;

		if (0.5f > m_fRatio)
			m_fUpDown += 100.f*(_float)dTimeDelta;
		else
			m_fUpDown -= 100.f*(_float)dTimeDelta;

		m_vAt = vPlayerPos + _vec3(0.f, 40.f + m_fUpDown, 0.f);
		//m_vAt = vPlayerPos + _vec3(0.f, 40.f, 0.f);
		if (1.f <= m_fRatio)
		{
			m_fUpDown = 0.f;
			m_fRatio = 0.f;
			m_vAt = vPlayerPos + _vec3(0.f, 40.f, 0.f);
			m_eStep = STEP2;
		}
	}
	break;
	case CStaticCamera::STEP2:
	{
		D3DXVec3CatmullRom(&m_vEye, &(vSpline2), &(vPlayerPos + vPlayerLook*500.f + vPlayerLeft*150.f + _vec3(0.f, 30.f, 0.f)), &(vPlayerPos - vPlayerLook*500.f + _vec3(0.f, 130.f, 0.f) + vPlayerLeft*500.f), &(vSpline2), m_fRatio);
		m_fRatio += (_float)dTimeDelta;
		m_vAt = m_vNextAt;//vPlayerPos + _vec3(0.f, 40.f, 0.f);

		if (1.0 <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP1;
			m_bRageCam = false;
		}
	}
	break;
	case CStaticCamera::STEP3:
	{

	}
	break;
	case CStaticCamera::STEP4:
	{

	}
	break;
	case CStaticCamera::STEP5:
	{

	}
	break;
	default:
		break;
	}
}

void CStaticCamera::CutScene_Belatos(const _double & dTimeDelta)
{
	m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Belatos", Engine::TRANSFORM, Engine::ID_DYNAMIC));

	_vec3 vTargetPos =		*m_pTargetTransform->Get_Info(Engine::INFO_POS);
	_vec3 vTargetLook =		*m_pTargetTransform->Get_Info(Engine::INFO_LOOK);
	_vec3 vTargetRight =	*m_pTargetTransform->Get_Info(Engine::INFO_RIGHT);
	_vec3 vTargetLeft =		-vTargetRight;

	_vec3 vStarting;
	_vec3 vEnd;
	_vec3 vSpline;

	switch (m_eStep)
	{
	case CStaticCamera::STEP1:
	{
		vStarting = vTargetPos + vTargetLook*150.f +_vec3(0.f, 10.f, 0.f);
		vEnd = vTargetPos + vTargetLook * 400.f + _vec3(0.f, 10.f, 0.f);
		vSpline = vTargetPos + vTargetLook * 400.f + _vec3(0.f, 10.f, 0.f);

		D3DXVec3CatmullRom(&m_vEye, &(vSpline), &(vStarting), &(vEnd), &(vSpline), m_fRatio);
		
		m_fRatio += (_float)(dTimeDelta*0.75);
		m_fUpDown += 100.f*(_float)(dTimeDelta*0.25);

		m_vAt = vTargetPos + _vec3(0.f, 10.f + m_fUpDown, 0.f);
		
		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP2;
		}
	}
		break;
	case CStaticCamera::STEP2:
	{
		vStarting = vTargetPos + vTargetLook * 300.f + _vec3(0.f, 10.f, 0.f);
		vEnd = vTargetPos + (vTargetRight + vTargetLook)*700.f + _vec3(0.f,100.f,0.f);
		vSpline = vTargetPos + vTargetLook * -800.f + _vec3(0.f, 50.f, 0.f);

		D3DXVec3CatmullRom(&m_vEye, &(vSpline), &(vStarting), &(vEnd), &(vSpline), m_fRatio);

		m_fRatio += (_float)(dTimeDelta*1.5);
		m_fUpDown += 100.f*(_float)(dTimeDelta*0.75);

		m_vAt = vTargetPos + _vec3(0.f, 10.f + m_fUpDown, 0.f);

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP3;
		}
	}
		break;
	case CStaticCamera::STEP3:
	{
		vStarting = vTargetPos + (vTargetRight + vTargetLook)*700.f + _vec3(0.f, 100.f, 0.f);
		vEnd = vTargetPos +(vTargetLeft + vTargetLook)*700.f + _vec3(0.f, 150.f, 0.f);
		vSpline = vTargetPos + vTargetLook * -1800.f + _vec3(0.f, 125.f, 0.f);

		D3DXVec3CatmullRom(&m_vEye, &(vSpline), &(vStarting), &(vEnd), &(vSpline), m_fRatio);

		m_fRatio += (_float)(dTimeDelta);

		m_vAt = vTargetPos + _vec3(0.f, 10.f + m_fUpDown, 0.f);

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP4;
		}
	}
		break;
	case CStaticCamera::STEP4:
	{
		vStarting = vTargetPos + (vTargetLeft + vTargetLook)*700.f + _vec3(0.f, 150.f, 0.f);
		vEnd = vTargetPos + (vTargetRight + vTargetLook)*300.f + _vec3(0.f, 50.f, 0.f);
		vSpline = vTargetPos + vTargetLook * -1800.f + _vec3(0.f, 100.f, 0.f);

		D3DXVec3CatmullRom(&m_vEye, &(vSpline), &(vStarting), &(vEnd), &(vSpline), m_fRatio);

		m_fRatio += (_float)(dTimeDelta*0.75);

		m_vAt = vTargetPos + _vec3(0.f, 10.f + m_fUpDown, 0.f);

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP5;
		}
	}
		break;
	case CStaticCamera::STEP5:
	{
		vStarting = vTargetPos + _vec3(0.f, 10.f + m_fUpDown, 0.f);
		vEnd = *pPlayerTransform->Get_Info(Engine::INFO_POS) + _vec3(0.f, 80.f, 0.f);
		vSpline = *pPlayerTransform->Get_Info(Engine::INFO_POS) + _vec3(0.f, 80.f, 0.f);

		D3DXVec3CatmullRom(&m_vAt, &(vSpline), &(vStarting), &(vEnd), &(vSpline), m_fRatio);

		m_fRatio += (_float)(dTimeDelta*1.25);


		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP1;
			m_fUpDown = 0.f;
			m_bCutScene = false;
		}
	}
		break;
	default:
		break;
	}
	Engine::CCamera::Update_GameObject(dTimeDelta);
}

void CStaticCamera::CutScene_Apostle(const _double & dTimeDelta)
{
	m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Apostle", Engine::TRANSFORM, Engine::ID_DYNAMIC));

	_vec3 vTargetPos = *m_pTargetTransform->Get_Info(Engine::INFO_POS);
	_vec3 vTargetLook = *m_pTargetTransform->Get_Info(Engine::INFO_LOOK);
	_vec3 vTargetRight = *m_pTargetTransform->Get_Info(Engine::INFO_RIGHT);
	_vec3 vTargetLeft = -vTargetRight;

	_vec3 vStarting;
	_vec3 vEnd;
	_vec3 vSpline;

	_vec3 vAtStart;
	_vec3 vAtEnd;
	_vec3 vAtSpline;

	switch (m_eStep)
	{
	case CStaticCamera::STEP1:
	{
		vStarting = vTargetPos - vTargetLook*250.f + _vec3(0.f, 50.f, 0.f);
		vEnd = vTargetPos + (vTargetLook* 400.f + vTargetLeft* 200.f) + _vec3(0.f, 300.f, 0.f);
		vSpline = vTargetPos + vTargetLeft * 3400.f + +_vec3(0.f, 190.f, 0.f);
		_vec3 vSpline2 = vTargetPos + (vTargetLeft - vTargetLook) * 3400.f +_vec3(0.f, 90.f, 0.f);

		D3DXVec3CatmullRom(&m_vEye, &(vSpline2), &(vStarting), &(vEnd), &(vSpline), m_fRatio);

		vAtStart = vTargetPos - vTargetLook*25.f + _vec3(0.f, 50.f, 0.f);
		vAtEnd = vTargetPos + (vTargetLook + vTargetLeft) * 40.f + _vec3(0.f, 300.f, 0.f);
		vAtSpline = vTargetPos + vTargetLeft * 240.f + +_vec3(0.f, 90.f, 0.f);

		D3DXVec3CatmullRom(&m_vAt, &(vAtSpline), &(vAtStart), &(vAtEnd), &(vAtSpline), m_fRatio);

		if(0.5>m_fRatio)
			m_fRatio += (_float)(dTimeDelta*0.25);
		else
			m_fRatio += (_float)(dTimeDelta*0.5);

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP2;
		}
	}
		break;
	case CStaticCamera::STEP2:
	{
		vStarting = vTargetPos + (vTargetLook* 400.f + vTargetLeft* 200.f) + _vec3(0.f, 300.f, 0.f);
		vEnd = vTargetPos + (vTargetLook* 400.f + vTargetRight* 200.f) + _vec3(0.f, 300.f, 0.f);
		vSpline = vTargetPos + vTargetLook * 500.f + +_vec3(0.f, 300.f, 0.f);

		D3DXVec3CatmullRom(&m_vEye, &(vSpline), &(vStarting), &(vEnd), &(vSpline), m_fRatio);

		m_fRatio += (_float)(dTimeDelta*0.75);
		

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP3;
		}
	}
		break;
	case CStaticCamera::STEP3:
	{
		vStarting = vTargetPos + (vTargetLook + vTargetLeft) * 40.f + _vec3(0.f, 300.f, 0.f);
		vEnd = vTargetPos + (vTargetLook* 100.f + vTargetLeft* 180.f) + _vec3(0.f, 180.f, 0.f);
		vSpline = vTargetPos + (vTargetLook* 100.f + vTargetRight* 80.f) + _vec3(0.f, 150.f, 0.f);

		D3DXVec3CatmullRom(&m_vAt, &(vSpline), &(vStarting), &(vEnd), &(vSpline), m_fRatio);

		m_fRatio += (_float)(dTimeDelta*1.25);

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP4;
		}
	}
		break;
	case CStaticCamera::STEP4:
	{
		vStarting = vTargetPos + (vTargetLook* 100.f + vTargetLeft* 180.f) + _vec3(0.f, 180.f, 0.f);;
		vEnd = *pPlayerTransform->Get_Info(Engine::INFO_POS) + _vec3(0.f, 60.f, 0.f);

		_float fLength = D3DXVec3Length(&((vTargetPos + (vTargetLook* 100.f + vTargetLeft* 180.f)) - *pPlayerTransform->Get_Info(Engine::INFO_POS)));
		_vec3 vCenter = vStarting - vEnd;
		D3DXVec3Normalize(&vCenter, &vCenter);
		
		vSpline = (vCenter*fLength*0.5f) - _vec3(0.f, 5000.f, 0.f);

		D3DXVec3CatmullRom(&m_vAt, &(vSpline), &(vStarting), &(vEnd), &(vSpline), m_fRatio);

		m_fRatio += (_float)(dTimeDelta*1.25);

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP5;
		}
	}
		break;
	case CStaticCamera::STEP5:
	{
		m_fRatio += (_float)(dTimeDelta);

		if (0.5 <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP1;
			m_bCutScene = false;
			m_bTopView = true;
			vAostlePos = *m_pTargetTransform->Get_Info(Engine::INFO_POS);
		}
	}
		break;
	default:
		break;
	}
	Engine::CCamera::Update_GameObject(dTimeDelta);

}

void CStaticCamera::CutScene_Flag(const _double & dTimeDelta)
{
	_vec3 vPlayerPos;
	_vec3 vPlayerLook;
	_vec3 vPlayerRight;
	_vec3 vPlayerLeft;

	_vec3 vTargetPos;
	_vec3 vTargetLook;
	_vec3 vTargetRight;
	_vec3 vTargetLeft;

	_vec3 vStarting;
	_vec3 vEnd;
	_vec3 vSpline;

	_vec3 vAtStart;
	_vec3 vAtEnd;
	_vec3 vAtSpline;
	_vec3 vAtSpline2;

	switch (m_eStep)
	{
	case CStaticCamera::STEP1:
	{
		m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Alliance", Engine::TRANSFORM, Engine::ID_DYNAMIC));

		vTargetPos = *m_pTargetTransform->Get_Info(Engine::INFO_POS);
		vTargetLook = *m_pTargetTransform->Get_Info(Engine::INFO_LOOK);
		vTargetRight = *m_pTargetTransform->Get_Info(Engine::INFO_RIGHT);
		vTargetLeft = -vTargetRight;

		vPlayerPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
		vPlayerLook = *pPlayerTransform->Get_Info(Engine::INFO_LOOK);
		vPlayerRight = *pPlayerTransform->Get_Info(Engine::INFO_RIGHT);
		vPlayerLeft = -vPlayerRight;

		vStarting = vPlayerPos + vPlayerRight*250.f + _vec3(0.f, 10.f, 0.f);
		vEnd = vTargetPos + (vTargetLook* 250.f + vTargetLeft* 250.f) + _vec3(0.f, 40.f, 0.f);
		vSpline = vPlayerPos + vPlayerLook * -1840.f +_vec3(0.f, 20.f, 0.f);
		_vec3 vSpline2 = vTargetPos + vTargetLook * -1840.f + _vec3(0.f, 30.f, 0.f);

		D3DXVec3CatmullRom(&m_vEye, &(vSpline2), &(vStarting), &(vEnd), &(vSpline), m_fRatio);

		vAtStart = vPlayerPos - vPlayerLook*25.f + _vec3(0.f, 10.f, 0.f);
		vAtEnd = vTargetPos + (vTargetLook + vTargetLeft) * 25.f + _vec3(0.f, 40.f, 0.f);
		vAtSpline = vPlayerPos + vPlayerLeft * 240.f +_vec3(0.f, 25.f, 0.f);
		vAtSpline2 = vTargetPos + vTargetLeft * 240.f + _vec3(0.f, 25.f, 0.f);

		D3DXVec3CatmullRom(&m_vAt, &(vAtSpline2), &(vAtStart), &(vAtEnd), &(vAtSpline), m_fRatio);

		if (0.5 > m_fRatio)
			m_fRatio += (_float)(dTimeDelta*0.75);
		else
			m_fRatio += (_float)(dTimeDelta);

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP2;
		}
	}
	break;
	case CStaticCamera::STEP2:
	{
		m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Alliance", Engine::TRANSFORM, Engine::ID_DYNAMIC));

		vTargetPos = *m_pTargetTransform->Get_Info(Engine::INFO_POS);
		vTargetLook = *m_pTargetTransform->Get_Info(Engine::INFO_LOOK);
		vTargetRight = *m_pTargetTransform->Get_Info(Engine::INFO_RIGHT);
		vTargetLeft = -vTargetRight;

		vPlayerPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
		vPlayerLook = *pPlayerTransform->Get_Info(Engine::INFO_LOOK);
		vPlayerRight = *pPlayerTransform->Get_Info(Engine::INFO_RIGHT);
		vPlayerLeft = -vPlayerRight;

		vStarting = vTargetPos + (vTargetLook* 250.f + vTargetLeft* 250.f) + _vec3(0.f, 40.f, 0.f);
		vEnd = vPlayerPos + vPlayerRight*250.f + _vec3(0.f, 80.f, 0.f);
		//vSpline = vTargetPos + vTargetLook * 500.f + +_vec3(0.f, 300.f, 0.f);

		vSpline = vTargetPos + vTargetLook * -1840.f + _vec3(0.f, 70.f, 0.f);
		_vec3 vSpline2 = vPlayerPos + vPlayerLook * -1840.f + _vec3(0.f, 60.f, 0.f);

		D3DXVec3CatmullRom(&m_vEye, &(vSpline), &(vStarting), &(vEnd), &(vSpline2), m_fRatio);

		vAtStart = vTargetPos + (vTargetLook + vTargetLeft) * 25.f + _vec3(0.f, 40.f, 0.f);
		vAtEnd = vPlayerPos - vPlayerLook*25.f + _vec3(0.f, 90.f, 0.f);
		vAtSpline = vTargetPos + vTargetLeft * 240.f +_vec3(0.f, 85.f, 0.f);
		vAtSpline2 = vPlayerPos + vPlayerRight * 240.f + _vec3(0.f, 70.f, 0.f);

		D3DXVec3CatmullRom(&m_vAt, &(vAtSpline2), &(vAtStart), &(vAtEnd), &(vAtSpline), m_fRatio);

		m_fRatio += (_float)(dTimeDelta*0.75);


		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP3;
		}
	}
	break;
	
	case CStaticCamera::STEP3:
	{
		m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Enermy_1", Engine::TRANSFORM, Engine::ID_DYNAMIC));

		vTargetPos =	*m_pTargetTransform->Get_Info(Engine::INFO_POS);
		vTargetLook =	*m_pTargetTransform->Get_Info(Engine::INFO_LOOK);
		vTargetRight =	*m_pTargetTransform->Get_Info(Engine::INFO_RIGHT);
		vTargetLeft =	-vTargetRight;

		m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Enermy_2", Engine::TRANSFORM, Engine::ID_DYNAMIC));

		vPlayerPos = *m_pTargetTransform->Get_Info(Engine::INFO_POS);
		vPlayerLook = *m_pTargetTransform->Get_Info(Engine::INFO_LOOK);
		vPlayerRight = *m_pTargetTransform->Get_Info(Engine::INFO_RIGHT);
		vPlayerLeft = -vTargetRight;

		vStarting = vPlayerPos + vPlayerRight*250.f + _vec3(0.f, 10.f, 0.f);
		vEnd = vTargetPos + (vTargetLook* 250.f + vTargetLeft* 250.f) + _vec3(0.f, 40.f, 0.f);
		vSpline = vPlayerPos + vPlayerLook * -1840.f + _vec3(0.f, 20.f, 0.f);
		_vec3 vSpline2 = vTargetPos + vTargetLook * -1840.f + _vec3(0.f, 30.f, 0.f);

		D3DXVec3CatmullRom(&m_vEye, &(vSpline2), &(vStarting), &(vEnd), &(vSpline), m_fRatio);

		vAtStart = vPlayerPos - vPlayerLook*25.f + _vec3(0.f, 10.f, 0.f);
		vAtEnd = vTargetPos + (vTargetLook + vTargetLeft) * 25.f + _vec3(0.f, 40.f, 0.f);
		vAtSpline = vPlayerPos + vPlayerLeft * 240.f + _vec3(0.f, 25.f, 0.f);
		vAtSpline2 = vTargetPos + vTargetLeft * 240.f + _vec3(0.f, 25.f, 0.f);

		D3DXVec3CatmullRom(&m_vAt, &(vAtSpline2), &(vAtStart), &(vAtEnd), &(vAtSpline), m_fRatio);

		if (0.5 > m_fRatio)
			m_fRatio += (_float)(dTimeDelta*0.75);
		else
			m_fRatio += (_float)(dTimeDelta);

		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP4;
		}
	}
	break;
	case CStaticCamera::STEP4:
	{
		m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Enermy_1", Engine::TRANSFORM, Engine::ID_DYNAMIC));

		vTargetPos = *m_pTargetTransform->Get_Info(Engine::INFO_POS);
		vTargetLook = *m_pTargetTransform->Get_Info(Engine::INFO_LOOK);
		vTargetRight = *m_pTargetTransform->Get_Info(Engine::INFO_RIGHT);
		vTargetLeft = -vTargetRight;

		m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Enermy_2", Engine::TRANSFORM, Engine::ID_DYNAMIC));

		vPlayerPos = *m_pTargetTransform->Get_Info(Engine::INFO_POS);
		vPlayerLook = *m_pTargetTransform->Get_Info(Engine::INFO_LOOK);
		vPlayerRight = *m_pTargetTransform->Get_Info(Engine::INFO_RIGHT);
		vPlayerLeft = -vTargetRight;

		vStarting = vTargetPos + (vTargetLook* 250.f + vTargetLeft* 250.f) + _vec3(0.f, 40.f, 0.f);
		vEnd = vPlayerPos + vPlayerRight*250.f + _vec3(0.f, 80.f, 0.f);
		//vSpline = vTargetPos + vTargetLook * 500.f + +_vec3(0.f, 300.f, 0.f);

		vSpline = vTargetPos + vTargetLook * -1840.f + _vec3(0.f, 70.f, 0.f);
		_vec3 vSpline2 = vPlayerPos + vPlayerLook * -1840.f + _vec3(0.f, 60.f, 0.f);

		D3DXVec3CatmullRom(&m_vEye, &(vSpline), &(vStarting), &(vEnd), &(vSpline2), m_fRatio);

		vAtStart = vTargetPos + (vTargetLook + vTargetLeft) * 25.f + _vec3(0.f, 40.f, 0.f);
		vAtEnd = vPlayerPos - vPlayerLook*25.f + _vec3(0.f, 90.f, 0.f);
		vAtSpline = vTargetPos + vTargetLeft * 240.f + _vec3(0.f, 85.f, 0.f);
		vAtSpline2 = vPlayerPos + vPlayerRight * 240.f + _vec3(0.f, 70.f, 0.f);

		D3DXVec3CatmullRom(&m_vAt, &(vAtSpline2), &(vAtStart), &(vAtEnd), &(vAtSpline), m_fRatio);

		m_fRatio += (_float)(dTimeDelta*0.75);


		if (1.f <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP5;
		}
	}
	break;
	case CStaticCamera::STEP5:
	{
		m_fRatio += (_float)(dTimeDelta);

		if (0.5 <= m_fRatio)
		{
			m_fRatio = 0.f;
			m_eStep = STEP1;
			m_bFlag = false;
		}
	}
	break;
	default:
		break;
	}
	Engine::CCamera::Update_GameObject(dTimeDelta);

}

void CStaticCamera::Shooting_Camera(const _double & dTimeDelta)
{
	_vec3 vPlayerPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);

	_vec3 vDistance = vPlayerPos - vAostlePos;
	_vec3 vTargetDir;
	D3DXVec3Normalize(&vTargetDir, &vDistance);

	_float fDist = D3DXVec3Length(&vDistance);

	//m_vEye = { -3.f,1826.f,1385.f };
	if (700.f <= fDist)
	{
		m_vNextEye = vDistance + vTargetDir*fDist*0.7f + _vec3(0.f, 250.f + fDist*0.75f, 0.f);
		m_vNextAt = vPlayerPos + vTargetDir*-fDist*0.35f;
		m_vNextAt.y += 80.f;
	}

	else
	{
		//m_vNextEye = vDistance + vTargetDir*fDist*0.7f + _vec3(0.f, 250.f + fDist*0.3f, 0.f);
		m_vNextEye = vDistance + vTargetDir*500.f + _vec3(0.f, 250.f + fDist*0.2f, 0.f);
		m_vNextAt =  vPlayerPos + vTargetDir*-fDist*0.1f;
		m_vNextAt.y += 120.f;
	}
	//m_vNextAt = vTargetPos/* + vTargetDir*-fDist*0.35f*/;
	//m_vNextAt.y += 100.f;
	vDir = m_vNextAt - m_vAt;
	m_vAt += vDir * _float(dTimeDelta * 10);
	
	vDir = m_vNextEye - m_vEye;
	m_vEye += vDir * _float(dTimeDelta * 10);

	Engine::CCamera::Update_GameObject(dTimeDelta);
}

void CStaticCamera::EndCutScene_APostle(const _double & dTimeDelta)
{
	// m_pTargetTransform = Apostle
	// vAostlePos = ApostlePos

	//_vec3 vPlayer = *pPlayerTransform->Get_Info(Engine::INFO_POS);

	//switch (m_eLevel)
	//{
	//case CStaticCamera::LEVEL1:
	//	break;
	//case CStaticCamera::LEVEL2:
	//	break;
	//case CStaticCamera::LEVEL3:
	//	break;
	//case CStaticCamera::LEVEL4:
	//	break;
	//case CStaticCamera::LEVEL5:
	//	break;
	//case CStaticCamera::LEVEL6:
	//	break;
	//case CStaticCamera::LEVEL7:
	//	break;
	//case CStaticCamera::LEVEL8:
	//	break;
	//case CStaticCamera::LEVEL9:
	//	break;
	//case CStaticCamera::LEVEL_END:
	//	break;
	//default:
	//	break;
	//}
}

void CStaticCamera::Key_Input(const _double & dTimeDelta)
{
	//_matrix matCamWorld;
	//D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (Engine::KeyDown(DIK_Z))
	{
		//if (true == m_bClick)
		//	return;

		//m_bClick = true;

		if (m_bMouseFix)
			m_bMouseFix = false;
		else
			m_bMouseFix = true;
	}
	/*else
		m_bClick = false;

	if (!m_bMouseFix)
		return;*/
}

void CStaticCamera::Mouse_Input(const _double & dTimeDelta)
{
	_long	dwMouseMove = 0;

	//_matrix		matCamWorld;
	//D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);


	/*if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Y))
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
	}*/

	if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Y))
	{
		m_vAtUp.y -= dwMouseMove * 0.5f;

		if (350.f <= m_vAtUp.y)
			m_vAtUp.y = 350.f;
		if(-150.f>=m_vAtUp.y)
			m_vAtUp.y = -150.f;
		/*_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vEye - m_vAt;

		_matrix		matRot;

		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 5.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
		m_vNextEye = m_vAt + vLook;*/
		
	}


	if (dwMouseMove = Engine::MouseMove(Engine::DIMS_X))
	{
		//m_vLook.x += dwMouseMove / 5.f;
		pPlayerTransform->Rotation(Engine::ROT_Y, dwMouseMove * 0.05f);// *(_float)dTimeDelta * 150.f);
		//_vec3		vUp = _vec3(0.f, 1.f, 0.f);

		//_vec3	vLook = m_vEye - m_vAt;

		//_matrix		matRot;


		//D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 5.f));

		//D3DXVec3TransformCoord(&vLook, &vLook, &matRot);
		//m_vNextEye = m_vAt + vLook;
	}
}

void CStaticCamera::Mouse_Fix()
{
	POINT		ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CStaticCamera::LookAtPlayer(const _double & dTimeDelta)
{
	pPlayerTransform->Get_Info(Engine::INFO::INFO_POS, &m_vNextAt);
	m_vNextAt.y += 80.f;
	_matrix matTrans, matRotY, matRotX, matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matTrans, 0.f, m_fDist, -m_fDist);

	_long	dwMouseMove = 0;
	if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Z))
	{
		m_fDist -= dwMouseMove * 0.1f;
		if (100.f > m_fDist)
			m_fDist = 100.f;
		else if (5000.f < m_fDist)
			m_fDist = 5000.f;
	}
	if (dwMouseMove = Engine::MouseMove(Engine::DIMS_X))
	{
		/*if(TestLock)
			m_fRotY += (dwMouseMove * 0.1f);
		else*/
			m_fRotY += (dwMouseMove * 0.05f);
	}
	if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Y))
	{
		/*if(TestLock)
			m_fRotX += (dwMouseMove * 0.1f);
		else*/
		if (!m_bActionZoom)
			m_fRotX += (dwMouseMove * 0.05f);
		if (40.f < m_fRotX)
		{
			m_fRotX = 40.f;
		}
		else if (-55.f > m_fRotX)
		{
			m_fDist += dwMouseMove * 0.1f;
			if (100.f > m_fDist)
				m_fDist = 100.f;
			m_fRotX = -55.f;
		}
	}
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fRotY));
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_fRotX));
	_matrix playerMatrix;
	D3DXMatrixTranslation(&playerMatrix, m_vNextAt.x, m_vNextAt.y + 80.f, m_vNextAt.z);

	matWorld *= matTrans * matRotX * matRotY  * playerMatrix;
	memcpy(&m_vNextEye, matWorld.m[3], sizeof(_vec3));

}

void CStaticCamera::LateInit()
{
	m_bLateInit = true;
	pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"RealPlayer", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	m_vEye = *(pPlayerTransform->Get_Info(Engine::INFO_POS)) - *(pPlayerTransform->Get_Info(Engine::INFO_LOOK))*500.f;
	m_vNextEye = m_vEye;
	
	if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_BOSS)
	{
		m_fRotX = -45.f;
		m_fRotY = 9.0f;
	}
	
	else if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_SHOOTING)
	{
		m_fRotY = 180.f;
		m_fRotX = -45.f;
	}
	else if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_CARTEL)
	{
		m_fRotY = -137.f;
		m_fRotX = -45.f;
	}
}

CStaticCamera * CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CStaticCamera* pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStaticCamera::Free(void)
{
	Engine::CCamera::Free();
}
