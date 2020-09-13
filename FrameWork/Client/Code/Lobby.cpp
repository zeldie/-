#include "stdafx.h"
#include "Lobby.h"

#include "BossStage.h"
#include "Cartel.h"
#include "ShootingStage.h"
#include "MatchingGame.h"
#include "PuzzleGame.h"
#include "RunGame.h"
#include "Customize.h"

#include "Belatos.h"
#include "Player.h"
#include "Apostle.h"
#include "TwohandSword.h"
#include "StaticCamera.h"
#include "Terrain.h"
#include "Architecture.h"
#include "Architecture_Dynamic.h"
#include "SkySphere.h"
#include "Decal.h"
#include "Portal.h"
#include "Sun.h"
#include "Body.h"
#include "Top.h"
#include "Flower.h"
#include "Tree.h"
#include "NPC_Red.h"
#include "NPC_Yellow.h"

#include "LightCamera.h"
CLobby::CLobby(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_bLateInit(true)
{
}

CLobby::~CLobby()
{
}

HRESULT CLobby::Load_Architecture(Engine::CLayer* pLayer, WCHAR * wstrPath)
{
	HANDLE hFile = CreateFile(wstrPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwstrLayerLen = 0;
	DWORD dwstrObjLen = 0;
	wstring wstrLayerTag = L"";
	TCHAR* pLayerTag = nullptr;
	wstring wstrObjTag = L"";
	TCHAR* pObjTag = nullptr;
	_vec3	vecPos = _vec3(0.f, 0.f, 0.f);
	_vec3	vecAngle = _vec3(0.f, 0.f, 0.f);
	_vec3	vecScale = _vec3(0.f, 0.f, 0.f);

	while (true)
	{
		//레이어 읽기
		ReadFile(hFile, &dwstrLayerLen, sizeof(DWORD), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		pLayerTag = new TCHAR[dwstrLayerLen];
		ReadFile(hFile, pLayerTag, dwstrLayerLen, &dwByte, nullptr);
		wstrLayerTag = (LPCTSTR)pLayerTag;

		//오브젝트이름 읽기
		ReadFile(hFile, &dwstrObjLen, sizeof(DWORD), &dwByte, nullptr);
		pObjTag = new TCHAR[dwstrObjLen];
		ReadFile(hFile, pObjTag, dwstrObjLen, &dwByte, nullptr);
		wstrObjTag = (LPCTSTR)pObjTag;

		//위치, 각도, 스케일 읽기
		ReadFile(hFile, &vecPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vecAngle, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vecScale, sizeof(_vec3), &dwByte, nullptr);

		if (L"Mesh_NPC_Crystal_Ani" == wstrObjTag || L"Mesh_H_CubeOfHildard" == wstrObjTag
			|| L"Mesh_GardenSwing01" == wstrObjTag)
		{
			if (FAILED(pLayer->Add_GameObject(wstrObjTag, CArchitecture_Dynamic::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
				return E_FAIL;
		}
		else if (L"Mesh_MatchingAI" == wstrObjTag)
		{
			if (FAILED(pLayer->Add_GameObject(wstrObjTag, CNPC_Red::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
				return E_FAIL;
		}
		else if (L"Mesh_HFPlayer" == wstrObjTag)
		{
			if (FAILED(pLayer->Add_GameObject(wstrObjTag, CNPC_Yellow::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
				return E_FAIL;
		}
		else if (L"Mesh_Flower_Poinsettia_01" == wstrObjTag
			|| L"Mesh_Ivy07" == wstrObjTag || L"Mesh_Ivy09" == wstrObjTag
			|| L"Mesh_Tree_Hedge_1" == wstrObjTag || L"Mesh_Tree_Ivy_001" == wstrObjTag)
		{
			if (FAILED(pLayer->Add_GameObject(wstrObjTag, CFlower::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
				return E_FAIL;
		}
		else if (/*L"Mesh_Middle_Tree_01" == wstrObjTag ||*/ L"Mesh_Middle_Tree_02" == wstrObjTag)
		{
			if (FAILED(pLayer->Add_GameObject(wstrObjTag, CTree::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
				return E_FAIL;
		}
		else
		{
			if (FAILED(pLayer->Add_GameObject(wstrObjTag, CArchitecture::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
				return E_FAIL;
		}

		Engine::Safe_Delete_Array(pLayerTag);
		Engine::Safe_Delete_Array(pObjTag);
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CLobby::Ready_Scene()
{
	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_GameObject_Layer()))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (CCameraMgr::GetInstance()->Get_Cerate())
	{
		CCameraMgr::GetInstance()->Change_Camera(CCameraMgr::STATIC);
		CCameraMgr::GetInstance()->Ready_Camera(m_pGraphicDev, CCameraMgr::STATIC);
	}
	else
	{

		CCameraMgr::GetInstance()->Ready_Camera(m_pGraphicDev, CCameraMgr::STATIC);
	}


	
	CCameraMgr::GetInstance()->SettingCamView();

	CQuestMgr::GetInstance()->AddQuestNarration();

	CSoundMgr::Get_Instance()->AllStop();
	

	return S_OK;
}

_int CLobby::Update_Scene(const _double & dTimeDelta)
{
	m_dTime += dTimeDelta;
	m_pUIMgr->UI_OnCheck();
	CCameraMgr::GetInstance()->Update_Camera(dTimeDelta);
	_int iExit = Engine::CScene::Update_Scene(dTimeDelta);

	if (m_bLateInit)
	{
		m_pUIMgr->CreateMouse(m_pGraphicDev, CMouse::MOUSE_INVISIBLE);
		CQuestMgr::GetInstance()->MakeFirstQuest();
		CSoundMgr::Get_Instance()->BGMSTART(95);
		m_bLateInit = false;
	}

	if (CUIMgr::UITYPE_READY == m_pUIMgr->Get_UIType())
	{
		m_pUIMgr->CheckEraseSwapPossible();
	}

	////////////////////////SceneChange
	if (Engine::KeyPressing(DIK_LCONTROL))
	{
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
			m_pUIMgr->ClearPointerUI();

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
			Engine::ClearRenderer();
			m_pUIMgr->ClearPointerUI();

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
			Engine::ClearRenderer();
			m_pUIMgr->ClearPointerUI();

			Engine::CScene*		pScene = nullptr;

			pScene = CRunGame::Create(m_pGraphicDev);
			if (pScene == nullptr)
				return E_FAIL;

			iExit = Engine::SetUp_Scene(pScene);

			return iExit;
		}
	}

	if (CUIMgr::UITYPE_READY == m_pUIMgr->Get_UIType())
	{
		if (m_pUIMgr->Get_SceneChange()) //로비 -> 벨라토스 퍼즐
		{
			if (!m_bFade)
			{
				CLoadingMgr::GetInstance()->Set_StartFade(true); //어둡게
				m_bFade = true;
			}

			if (CLoadingMgr::GetInstance()->Get_StartFade() == false) // 다 어두워지면
			{
				Engine::ClearSubject();
				Engine::ClearRenderer();
				m_pUIMgr->ClearPointerUI();
				Engine::CScene*		pScene = nullptr;

				pScene = CPuzzleGame::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return E_FAIL;

				iExit = Engine::SetUp_Scene(pScene);
				CPuzzleMgr::GetInstance()->Create_Puzzle_Belatos();

				return iExit;
			}
		}
	}

	if (CUIMgr::UITYPE_MISSION == m_pUIMgr->Get_UIType())
	{
		if (m_pUIMgr->SceneChangeToCTF()) //로비 ->깃발전
		{
			if (!m_bFade)
			{
				CLoadingMgr::GetInstance()->Set_StartFade(true); //어둡게
				m_bFade = true;
			}

			if (CLoadingMgr::GetInstance()->Get_StartFade() == false) // 다 어두워지면
			{
				Engine::ClearSubject();
				Engine::ClearRenderer();
				m_pUIMgr->ClearPointerUI();
				Engine::CScene*		pScene = nullptr;
				_uint i = rand() % 6 + 18;
				CSoundMgr::Get_Instance()->SoundOn(i);
				pScene = CCartel::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return E_FAIL;

				iExit = Engine::SetUp_Scene(pScene);

				return iExit;
			}
		}

		if (m_pUIMgr->SceneChangeToApostle()) //로비 -> 돼지천사 퍼즐
		{
			if (!m_bFade)
			{
				CLoadingMgr::GetInstance()->Set_StartFade(true); //어둡게
				m_bFade = true;
			}

			if (CLoadingMgr::GetInstance()->Get_StartFade() == false) // 다 어두워지면
			{
				Engine::ClearSubject();
				Engine::ClearRenderer();
				m_pUIMgr->ClearPointerUI();
				Engine::CScene*		pScene = nullptr;

				pScene = CPuzzleGame::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return E_FAIL;

				iExit = Engine::SetUp_Scene(pScene);
				CPuzzleMgr::GetInstance()->Create_Puzzle_Apostle();

				return iExit;
			}
		}
	}

	// 로비 -> 짝맞추기
	if (CUIMgr::UITYPE_NPC == m_pUIMgr->Get_UIType())
	{
		if (m_pUIMgr->SceneChageToMatchingGame()) 
		{
			if (!m_bFade)
			{
				CLoadingMgr::GetInstance()->Set_StartFade(true); //어둡게
				m_bFade = true;
			}

			if (CLoadingMgr::GetInstance()->Get_StartFade() == false) // 다 어두워지면
			{
				Engine::ClearSubject();
				Engine::ClearRenderer();
				m_pUIMgr->ClearPointerUI();
				Engine::CScene*		pScene = nullptr;

				pScene = CMatchingGame::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return E_FAIL;

				iExit = Engine::SetUp_Scene(pScene);

				return iExit;
			}
		}

	}
	m_pLightCamera->Update_GameObject(dTimeDelta);
	/////////////////////////////////
	m_dEnterSceneTime += dTimeDelta;
	if (!m_bEnterScene && m_dEnterSceneTime > 5)
	{
		CLoadingMgr::GetInstance()->Set_EndFade(true);
		m_bEnterScene = true;
	}
	return iExit;
}

_int CLobby::LateUpdate_Scene(const _double & dTimeDelta)
{
	CCameraMgr::GetInstance()->LateUpdate_Camera(dTimeDelta);
	_int iExit = Engine::CScene::LateUpdate_Scene(dTimeDelta);
	return iExit;
}

void CLobby::Render_Scene()
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

HRESULT CLobby::Ready_Environment_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	//pGameObject = CStaticCamera::Create(m_pGraphicDev,
	//	&_vec3(800.f, 100.f, -800.f),
	//	&_vec3(0.f, 0.f, 1.f),
	//	&_vec3(0.f, 1.f, 0.f),
	//	D3DXToRadian(45.f),
	//	_float(WINCX) / WINCY,
	//	0.7f,
	//	4000.f);
	//if (FAILED(pLayer->Add_GameObject(L"StaticCamera", pGameObject)))
	//	return E_FAIL;

	pGameObject = CSkySphere::Create(m_pGraphicDev, CSkySphere::SKY_LOBBY);
	if (FAILED(pLayer->Add_GameObject(L"ZSky", pGameObject)))
		return E_FAIL;
	m_mapLayer.emplace(Engine::ENVIRONMENT, pLayer);


	return S_OK;
}

HRESULT CLobby::Ready_GameObject_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CPlayer::Create(m_pGraphicDev, &_vec3(0.f, 50.f, 20.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.5f, 0.5f, 0.5f), CBasePlayer::PLAYER_ID::ID_PLAYER); /////////////내맘대로수정
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RealPlayer", pGameObject)))
		return E_FAIL;
	m_pLightCamera->Set_PlayerTransformCom(static_cast<CBaseObject*>(pGameObject)->Get_TransformCom());
	pGameObject = CTerrain::Create(m_pGraphicDev, CTerrain::TERRAIN_LOBBY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
		return E_FAIL;

	pGameObject = CPortal::Create(m_pGraphicDev, &_vec3(-658.f, 310.f, -878.f), &_vec3(0.f, 35.f, 0.f), &_vec3(0.3f, 0.6f, 0.1f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Portal", pGameObject)))
		return E_FAIL;

	pGameObject = CSun::Create(m_pGraphicDev, &_vec3(-658.f, 610.f, -878.f), &_vec3(0.f, 0.f, 0.f), &_vec3(2.f, 2.f, 2.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Sun", pGameObject)))
		return E_FAIL;

	Load_Architecture(pLayer, L"../../Data/Mesh/Map_Lobby.dat");
	m_mapLayer.emplace(Engine::GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CLobby::Ready_UI_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	m_pUIMgr->CreateMainUI(m_pGraphicDev, pLayer);
	m_mapLayer.emplace(Engine::UI, pLayer);

	return S_OK;
}

HRESULT CLobby::Ready_Light()
{
	//D3DLIGHT9			tLightInfo;
	//ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	//tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	//tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f);
	//tLightInfo.Direction = _vec3(0.f, -1.f, 1.f);
	//if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
	//	return E_FAIL;
	return S_OK;
}

CLobby * CLobby::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLobby *	pInstance = new CLobby(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLobby::Free()
{
	if (m_pLightCamera != nullptr)
		m_pLightCamera->Release_TransformCom();
	//m_pUIMgr->EraseMouse();
	Engine::CScene::Free();
	
}
