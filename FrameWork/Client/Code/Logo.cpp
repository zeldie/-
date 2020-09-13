#include "stdafx.h"
#include "Logo.h"
#include "Lobby.h"
#include "BackGround.h"
#include "BossStage.h"
#include "Cartel.h"
#include "ShootingStage.h"
#include "MatchingGame.h"
#include "PuzzleGame.h"
#include "RunGame.h"
#include "Customize.h"
CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev),
	m_pLoading(nullptr)
{
}


CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_FIRST);
	if (m_pLoading == nullptr)
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_GameObject_Layer()))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;

	return S_OK;
}

_int CLogo::Update_Scene(const _double & dTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(dTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		// 로딩완료 체크용 사운드
		
		////////////////////////스토리상의  씬체인지///////////////////
		if (Engine::KeyPressing(DIK_RETURN))
		{
			if (!m_bFade)
			{
				CLoadingMgr::GetInstance()->Set_StartFade(true); //어둡게
				m_bFade = true;
			}	
		}
		if (m_bFade)
		{
			if (CLoadingMgr::GetInstance()->Get_StartFade() == false) // 다 어두워지면
			{
				Engine::ClearRenderer();
				Engine::CScene*		pScene = nullptr;

				pScene = CLobby::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return E_FAIL;

				iExit = Engine::SetUp_Scene(pScene);
				return iExit;
			}
		}
		///////////////////////////////////////////////////
		if (Engine::KeyPressing(DIK_LCONTROL))
		{
			if (Engine::KeyDown(DIK_F4))
			{
				Engine::ClearSubject();

				Engine::CScene*		pScene = nullptr;

				pScene = CCustomize::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return E_FAIL;

				iExit = Engine::SetUp_Scene(pScene);

				return iExit;
			}
			if (Engine::KeyDown(DIK_F5))
			{
				Engine::ClearSubject();

				Engine::CScene*		pScene = nullptr;

				pScene = CLobby::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return E_FAIL;

				iExit = Engine::SetUp_Scene(pScene);

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
				Engine::ClearRenderer();
				CUIMgr::GetInstance()->ClearPointerUI();

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
			if (Engine::KeyDown(DIK_F10))
			{
				Engine::ClearSubject();

				Engine::CScene*		pScene = nullptr;

				pScene = CPuzzleGame::Create(m_pGraphicDev);
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

		/////////////////////////////////
	}

	return iExit;
}

_int CLogo::LateUpdate_Scene(const _double & dTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(dTimeDelta);
	return iExit;
}

void CLogo::Render_Scene()
{
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_String(), &_vec2(20.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}


HRESULT CLogo::Ready_Environment_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	m_mapLayer.emplace(Engine::ENVIRONMENT, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_GameObject_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	m_mapLayer.emplace(Engine::GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_UI_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CBackGround::Create(m_pGraphicDev);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"BackGround", pGameObject)))
		return E_FAIL;

	m_mapLayer.emplace(Engine::UI, pLayer);

	return S_OK;
}


CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo *	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}


void CLogo::Free()
{
	//test
	//CSoundMgr::Get_Instance()->AllStop();

	Engine::Safe_Release(m_pLoading);
	Engine::CScene::Free();
}
