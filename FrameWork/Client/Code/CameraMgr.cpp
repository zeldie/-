#include "stdafx.h"
#include "CameraMgr.h"
#include "StaticCamera.h"
#include "RunGameCamera.h"
#include "DynamicCam.h"
#include "CustomizeCamera.h"

IMPLEMENT_SINGLETON(CCameraMgr);

CCameraMgr::CCameraMgr()
	:m_bCreate(false),
	m_bBelatosComplete(false),
	m_bApostleComplete(false),
	m_bFlagComplete(false),
	m_eMode(MODE_END),
	m_eWeapon(CPlayer::TWOHANDSWORD),
	m_pRendererCom(nullptr),
	m_pCam(nullptr)
{
}

CCameraMgr::~CCameraMgr()
{
	Free();
}

void CCameraMgr::Ready_Camera(LPDIRECT3DDEVICE9 pGraphicDev, MODE eMode)
{
	
	// 최초 1회 카메라 생성
	if (!m_bCreate)
	{
		m_eMode = eMode;
		m_bCreate = true;
		Engine::CCamera* pGameObject = CStaticCamera::Create(pGraphicDev,
			&_vec3(800.f, 100.f, -800.f),
			&_vec3(0.f, 0.f, 1.f),
			&_vec3(0.f, 1.f, 0.f),
			D3DXToRadian(45.f),
			_float(WINCX) / WINCY,
			0.7f,
			4000.f);

		//m_vecCam->push_back(pGameObject);
		m_vecCam[STATIC].push_back(pGameObject);

		pGameObject = CRunGameCamera::Create(pGraphicDev,
			&_vec3(800.f, 100.f, -800.f),
			&_vec3(0.f, 80.f, 1.f),
			&_vec3(0.f, 1.f, 0.f),
			D3DXToRadian(45.f),
			_float(WINCX) / WINCY,
			0.7f,
			7000.f);
		m_vecCam[RUNGAME].push_back(pGameObject);

		
		pGameObject = CDynamicCam::Create(pGraphicDev,
			&_vec3(800.f, 100.f, -800.f),
			&_vec3(0.f, 0.f, 1.f),
			&_vec3(0.f, 1.f, 0.f),
			D3DXToRadian(45.f),
			_float(WINCX) / WINCY,
			0.7f,
			7000.f);

		m_vecCam[DYNAMIC].push_back(pGameObject);
		//m_vecCam[DYNAMIC].push_back(pGameObject);
		
		pGameObject = CCustomizeCamera::Create(pGraphicDev,
			&_vec3(800.f, 100.f, -800.f),
			&_vec3(0.f, 0.f, 1.f),
			&_vec3(0.f, 1.f, 0.f),
			D3DXToRadian(45.f),
			_float(WINCX) / WINCY,
			0.7f,
			4000.f);

		m_vecCam[CUSTOM].push_back(pGameObject);

		switch (eMode)
		{
		case CCameraMgr::STATIC:
			m_pCam = m_vecCam[STATIC].back();
			break;
		case CCameraMgr::DYNAMIC:
			m_pCam = m_vecCam[DYNAMIC].back();
			break;
		case CCameraMgr::RUNGAME:
			m_pCam = m_vecCam[RUNGAME].back();
			break;
		case CCameraMgr::CUSTOM:
			m_pCam = m_vecCam[CUSTOM].back();
			break;
		case CCameraMgr::ACTION:
			break;
		case CCameraMgr::MODE_END:
			break;
		default:
			break;
		}
	}
	else // 스테이지 변경 시 카메라 설정 리셋?
	{
		if (m_eMode != eMode)
			Change_Camera(eMode);

		switch (m_eMode)
		{
		case CCameraMgr::STATIC:
			(dynamic_cast<CStaticCamera*>(m_pCam))->Ready_Object(&_vec3(800.f, 100.f, -800.f),
				&_vec3(0.f, 0.f, 1.f),
				&_vec3(0.f, 1.f, 0.f),
				D3DXToRadian(45.f),
				_float(WINCX) / WINCY,
				0.7f,
				4000.f);
			break;
		case CCameraMgr::DYNAMIC:
			(dynamic_cast<CDynamicCam*>(m_pCam))->Ready_Object(&_vec3(800.f, 100.f, -800.f),
				&_vec3(0.f, 0.f, 1.f),
				&_vec3(0.f, 1.f, 0.f),
				D3DXToRadian(45.f),
				_float(WINCX) / WINCY,
				0.7f,
				7000.f);
			break;
		case CCameraMgr::RUNGAME:
		{
			(dynamic_cast<CRunGameCamera*>(m_pCam))->Ready_Object(&_vec3(800.f, 100.f, -800.f),
				&_vec3(0.f, 0.f, 1.f),
				&_vec3(0.f, 1.f, 0.f),
				D3DXToRadian(45.f),
				_float(WINCX) / WINCY,
				0.7f,
				7000.f);
		}
			break;
		case CCameraMgr::CUSTOM:
		{
			(dynamic_cast<CCustomizeCamera*>(m_pCam))->Ready_Object(&_vec3(800.f, 100.f, -800.f),
				&_vec3(0.f, 0.f, 1.f),
				&_vec3(0.f, 1.f, 0.f),
				D3DXToRadian(45.f),
				_float(WINCX) / WINCY,
				0.7f,
				4000.f);
		}
			break;
		case CCameraMgr::ACTION:
			break;
		case CCameraMgr::MODE_END:
			break;
		default:
			break;
		}
		//m_pCam->Ready_GameObject();
	}
}

_vec3 & CCameraMgr::Get_Eye()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pCam->Get_Eye();
}

_vec3 & CCameraMgr::Get_At()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pCam->Get_At();

}

void CCameraMgr::Update_Camera(const _double & dTimeDelta)
{
	m_pCam->Update_GameObject(dTimeDelta);
}

void CCameraMgr::LateUpdate_Camera(const _double & dTimeDelta)
{
	m_pCam->LateUpdate_GameObject(dTimeDelta);
}

void CCameraMgr::Change_Camera(MODE eMode)
{
	m_eMode = eMode;
	m_pCam = m_vecCam[eMode].back();
	
}

void CCameraMgr::Set_MouseFix(_bool bBool)
{
	(m_pCam)->Set_MouseFix(bBool);
}

void CCameraMgr::Set_CamMove(_bool bBool)
{
	(m_pCam)->Set_CamMove(bBool);
}

void CCameraMgr::Set_RageCam(CPlayer::PLAYER_WEAPONTYPE eWeapon)
{
	if (m_eMode != STATIC)
		return;
	
	dynamic_cast<CStaticCamera*>(m_pCam)->Set_RageCam(eWeapon);
}

void CCameraMgr::Set_BelatosCutscene()
{
	if (m_eMode != STATIC)
		return;
	if (!m_bBelatosComplete)
	{
		dynamic_cast<CStaticCamera*>(m_pCam)->Set_BelatosCutscene();
		m_bBelatosComplete = true;
	}
}

void CCameraMgr::Set_ApostleCutscene()
{
	if (m_eMode != STATIC)
		return;
	if (!m_bApostleComplete)
	{
		dynamic_cast<CStaticCamera*>(m_pCam)->Set_ApostleCutscene();
		m_bApostleComplete = true;
	}
}

void CCameraMgr::Set_ShakingCamera()
{
	if (m_eMode != STATIC)
		return;

	dynamic_cast<CStaticCamera*>(m_pCam)->Set_Shaking();
}

void CCameraMgr::Set_FlagCutscene()
{
	if (m_eMode != STATIC)
		return;
	if (!m_bFlagComplete)
	{
		dynamic_cast<CStaticCamera*>(m_pCam)->Set_FlagCutscene();
		m_bFlagComplete = true;
	}
}


void CCameraMgr::LateInit()
{
	if (m_eMode != STATIC)
		return;
	dynamic_cast<CStaticCamera*>(m_pCam)->LateInit();
}

void CCameraMgr::Set_RendererCom()
{
	m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (m_pRendererCom == nullptr)
		return;
}

void CCameraMgr::SettingCamView()
{
	if (m_pRendererCom == nullptr)
		return;
	if (m_pCam == nullptr)
		return;
	m_pRendererCom->Set_CamFar(m_pCam->Get_Far());
}

_float CCameraMgr::Get_CamFar()
{
	if (m_pCam == nullptr)
		return 0.f;
	return m_pCam->Get_Far();
}

void CCameraMgr::Set_ActionZoom()
{
	if (m_eMode != STATIC)
		return;
	dynamic_cast<CStaticCamera*>(m_pCam)->Set_ActionZoom();
}

void CCameraMgr::ReSet_ActionZoom()
{
	if (m_eMode != STATIC)
		return;
	dynamic_cast<CStaticCamera*>(m_pCam)->ReSet_ActionZoom();
}

void CCameraMgr::Set_ItemGet()
{
	if (m_eMode != RUNGAME)
		return;
	dynamic_cast<CRunGameCamera*>(m_pCam)->Set_ItemGet(true);
}

_vec3 CCameraMgr::Get_Pos()
{
	if (m_eMode != STATIC)
		return _vec3(0.f, 0.f, 0.f);
	return dynamic_cast<CStaticCamera*>(m_pCam)->Get_Pos();
}

_vec3 CCameraMgr::Get_PosRun()
{
	return dynamic_cast<CRunGameCamera*>(m_pCam)->Get_Eye();
}

void CCameraMgr::Free(void)
{
	for (_uint i = 0; i < MODE_END; ++i)
	{
		for_each(m_vecCam[i].begin(), m_vecCam[i].end(), Engine::CDeleteObj());
		m_vecCam[i].clear();
	}
	Engine::Safe_Release(m_pRendererCom);
}
