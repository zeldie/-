#include "stdafx.h"
#include "PuzzleGame.h"

#include "Lobby.h"
#include "BossStage.h"
#include "Cartel.h"
#include "ShootingStage.h"
#include "MatchingGame.h"
#include "RunGame.h"

#include "MiniGameCamera.h"
#include "LightCamera.h"

CPuzzleGame::CPuzzleGame(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_pLoading(nullptr),
	m_bInit(false)
{
}

CPuzzleGame::~CPuzzleGame()
{
}

HRESULT CPuzzleGame::Ready_Scene()
{
	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_GameObject_Layer()))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Light()))
		return E_FAIL;
	//CCameraMgr::GetInstance()->Ready_Camera(m_pGraphicDev);
	CLoadingMgr::GetInstance()->Set_EndFade(true); //밝게한다
	return S_OK;
}

_int CPuzzleGame::Update_Scene(const _double & dTimeDelta)
{
	m_dTime += dTimeDelta;
	_int iExit = Engine::CScene::Update_Scene(dTimeDelta);

	LateInit();

	//CCameraMgr::GetInstance()->Update_Camera(dTimeDelta);
	//m_pUIMgr->UI_OnCheck();

	////////////////////////SceneChange
	// 키 입력이아니라 제대로 씬 전환할때, 매니저에서 데드 함수 호출하는거 추가하면됨.
	if (Engine::KeyPressing(DIK_LCONTROL))
	{
		if (Engine::KeyDown(DIK_F5))
		{
			Engine::ClearSubject();
			Engine::ClearRenderer();
			m_pUIMgr->ClearPointerUI();

			Engine::CScene*		pScene = nullptr;

			pScene = CLobby::Create(m_pGraphicDev);
			if (pScene == nullptr)
				return E_FAIL;

			iExit = Engine::SetUp_Scene(pScene);
			CPuzzleMgr::GetInstance()->Delete_Puzzle();
			return iExit;
		}
		if (Engine::KeyDown(DIK_F6))
		{
			Engine::ClearSubject();

			Engine::CScene*		pScene = nullptr;

			pScene = CBossStage::Create(m_pGraphicDev);
			if (pScene == nullptr)
				return E_FAIL;

			iExit = Engine::SetUp_Scene(pScene);

			return iExit;
		}
		if (Engine::KeyDown(DIK_F7))
		{
			Engine::ClearSubject();

			Engine::CScene*		pScene = nullptr;

			pScene = CCartel::Create(m_pGraphicDev);
			if (pScene == nullptr)
				return E_FAIL;

			iExit = Engine::SetUp_Scene(pScene);

			return iExit;
		}
		if (Engine::KeyDown(DIK_F8))
		{
			Engine::ClearSubject();

			Engine::CScene*		pScene = nullptr;

			pScene = CShootingStage::Create(m_pGraphicDev);
			if (pScene == nullptr)
				return E_FAIL;

			iExit = Engine::SetUp_Scene(pScene);

			return iExit;
		}
		if (Engine::KeyDown(DIK_F9))
		{
			Engine::ClearSubject();

			Engine::CScene*		pScene = nullptr;

			pScene = CMatchingGame::Create(m_pGraphicDev);
			if (pScene == nullptr)
				return E_FAIL;

			iExit = Engine::SetUp_Scene(pScene);

			return iExit;
		}
		if (Engine::KeyDown(DIK_F11))
		{
			Engine::ClearSubject();

			Engine::CScene*		pScene = nullptr;

			pScene = CRunGame::Create(m_pGraphicDev);
			if (pScene == nullptr)
				return E_FAIL;

			iExit = Engine::SetUp_Scene(pScene);

			return iExit;
		}
	}
	if (CPuzzleMgr::GetInstance()->Get_ChangeScene() == true)
	{
		if (CPuzzleMgr::GetInstance()->Get_PuzzleType() == CPuzzleMgr::PUZZLE_BELATOS)
		{
			if (!m_bFade)
			{
				CLoadingMgr::GetInstance()->Set_StartFade(true); //어둡게한다
				m_bFade = true;
			}
			if (CLoadingMgr::GetInstance()->Get_StartFade() == false) //다어두워지면
			{
				Engine::ClearSubject();
				Engine::ClearRenderer();
				m_pUIMgr->ClearPointerUI();
				Engine::CScene*		pScene = nullptr;

				pScene = CBossStage::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return E_FAIL;

				iExit = Engine::SetUp_Scene(pScene);
				CPuzzleMgr::GetInstance()->Delete_Puzzle();
				CPuzzleMgr::GetInstance()->Set_SceneChange(false);
				return iExit;
			}
		}
		else if (CPuzzleMgr::GetInstance()->Get_PuzzleType() == CPuzzleMgr::PUZZLE_APOSTLE)
		{
			if (!m_bFade)
			{
				CLoadingMgr::GetInstance()->Set_StartFade(true); //어둡게한다
				m_bFade = true;
			}
			if (CLoadingMgr::GetInstance()->Get_StartFade() == false) //다어두워지면
			{
				Engine::ClearSubject();
				Engine::ClearRenderer();
				m_pUIMgr->ClearPointerUI();
				Engine::CScene*		pScene = nullptr;

				pScene = CShootingStage::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return E_FAIL;

				iExit = Engine::SetUp_Scene(pScene);
				CPuzzleMgr::GetInstance()->Delete_Puzzle();
				CPuzzleMgr::GetInstance()->Set_SceneChange(false);
				return iExit;
			}
		}

	}
	////////////////////////////////

	CPuzzleMgr::GetInstance()->Update_PuzzleMgr(dTimeDelta);

	if (Engine::KeyDown(DIK_1))
	{
		CPuzzleMgr::GetInstance()->Create_Puzzle_Apostle();
	}
	if (Engine::KeyDown(DIK_2))
	{
		CPuzzleMgr::GetInstance()->Reset_Puzzle_Apostle();
	}
	if (Engine::KeyDown(DIK_3))
	{
		CPuzzleMgr::GetInstance()->Delete_Puzzle();
	}
	if (Engine::KeyDown(DIK_4))
	{
		CPuzzleMgr::GetInstance()->Create_Puzzle_Belatos();

	}
	// ======================테스트 코드 ==================================

	return iExit;
}

_int CPuzzleGame::LateUpdate_Scene(const _double & dTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(dTimeDelta);
	//CCameraMgr::GetInstance()->LateUpdate_Camera(dTimeDelta);
	return iExit;
}

void CPuzzleGame::Render_Scene()
{

	++m_dwRenderCnt;
	if (m_dTime >= 1)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_dTime = 0;
		m_dwRenderCnt = 0;
	}

	Engine::Render_Font(L"Font_Default", m_szFPS, &_vec2(20.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}

void CPuzzleGame::LateInit()
{
	if (m_bInit)
		return;
	m_bInit = true;

	CUIMgr::GetInstance()->CreateMouse(m_pGraphicDev, CMouse::MOUSE_PICKING);

}

HRESULT CPuzzleGame::Ready_Environment_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMiniGameCamera::Create(m_pGraphicDev,
		&_vec3(800.f, 100.f, -800.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(45.f),
		_float(WINCX) / WINCY,
		0.7f,
		4000.f);
	if (FAILED(pLayer->Add_GameObject(L"MiniGameCamera", pGameObject)))
		return E_FAIL;

	//pGameObject = CSkySphere::Create(m_pGraphicDev, CSkySphere::SKY_SHOOTINGSTAGE);
	//if (FAILED(pLayer->Add_GameObject(L"ZSky", pGameObject)))
	//	return E_FAIL;
	m_mapLayer.emplace(Engine::ENVIRONMENT, pLayer);


	return S_OK;
}

HRESULT CPuzzleGame::Ready_GameObject_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	m_mapLayer.emplace(Engine::GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CPuzzleGame::Ready_UI_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	//Matching Button - START
	pGameObject = CMatchingButton::Create(m_pGraphicDev, CMatchingButton::RESET1, 884.4f, 13.3f, 314.f, 78.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"MatchingButton", pGameObject);
	m_pUIMgr->AddCurUI(pGameObject);

	m_mapLayer.emplace(Engine::UI, pLayer);

	return S_OK;
}

HRESULT CPuzzleGame::Ready_Light()
{
	//D3DLIGHT9			tLightInfo;
	//ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	//tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	//tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	//tLightInfo.Direction = _vec3(0.f, -1.f, 1.f);
	//if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
	//	return E_FAIL;
	return S_OK;
}

CPuzzleGame * CPuzzleGame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPuzzleGame *	pInstance = new CPuzzleGame(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPuzzleGame::Free()
{
	if (m_pLightCamera != nullptr)
		m_pLightCamera->Release_TransformCom();
	Engine::CScene::Free();
}
