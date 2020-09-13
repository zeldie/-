#include "stdafx.h"
#include "Cartel.h"
#include "Lobby.h"
#include "BossStage.h"
#include "ShootingStage.h"
#include "MatchingGame.h"
#include "PuzzleGame.h"
#include "RunGame.h"

#include "Player.h"
#include "TwohandSword.h"
#include "StaticCamera.h"
#include "Terrain.h"
#include "Architecture.h"
#include "SkySphere.h"
#include "Sun.h"
#include "StainedGlass.h"
#include "Mouse.h"
#include "Belatos.h"
#include "Tree.h"
#include "Flag.h"

#include "LightCamera.h"
CCartel::CCartel(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_pAiMgr(CAiMgr::GetInstance()),
	m_bLateInit(true),
	m_dCreateCartelResultUITime(2.f),
	m_bCreateUI(true),
	m_bCreateResultUI(true)
{
	m_pAiMgr->Set_GraphicDev(pGraphicDev);

	//test
	//ftest = 0.f;
}

CCartel::~CCartel()
{

}

HRESULT CCartel::Load_Architecture(Engine::CLayer* pLayer, WCHAR * wstrPath)
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

		if (L"Mesh_Middle_Tree_02" == wstrObjTag)
		{
			if (FAILED(pLayer->Add_GameObject(wstrObjTag, CTree::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
				return E_FAIL;
		}
		else if (L"Mesh_Flag" == wstrObjTag)
		{
			if (FAILED(pLayer->Add_GameObject(wstrObjTag, CFlag::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
				return E_FAIL;

			CAiMgr::GetInstance()->Set_FlagPos(vecPos);
		}
		else if (FAILED(pLayer->Add_GameObject(wstrObjTag, CArchitecture::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
			return E_FAIL;


		Engine::Safe_Delete_Array(pLayerTag);
		Engine::Safe_Delete_Array(pObjTag);
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CCartel::Ready_Scene()
{
	m_pUIMgr->Set_PlayerMainWeapon(2); //대검
	m_pUIMgr->Set_PlayerSubWeapon(3); //오브


	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_GameObject_Layer()))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Light()))
		return E_FAIL;

	CCameraMgr::GetInstance()->Ready_Camera(m_pGraphicDev, CCameraMgr::STATIC);
	CCameraMgr::GetInstance()->SettingCamView();

	m_pUIMgr->SetZeroForAccumulatedVariables();
	m_pUIMgr->Set_ZeroStageTime();

	if (CAiMgr::GetInstance()->Get_CartelReady() == false)
		CAiMgr::GetInstance()->Set_CartelReady(true);


	return S_OK;
}

_int CCartel::Update_Scene(const _double & dTimeDelta)
{
	m_dTime += dTimeDelta;
	m_pUIMgr->UI_OnCheck();

	if (m_bCreateUI)
	{
		if (CAiMgr::GetInstance()->Get_CartelStart())
		{
			m_pUIMgr->CreateCartelUI(m_pGraphicDev,m_mapLayer[Engine::UI]);
			m_pUIMgr->CreatePlayerInfoUI(m_pGraphicDev,nullptr);
			m_pUIMgr->Set_StageTimeStart();

			m_bCreateUI = false;
		}

	}

	CCameraMgr::GetInstance()->Set_FlagCutscene();
	CCameraMgr::GetInstance()->Update_Camera(dTimeDelta);


	_int iExit = Engine::CScene::Update_Scene(dTimeDelta);

	//ftest += dTimeDelta;

	if (m_bLateInit)
	{
		m_pUIMgr->CreateMouse(m_pGraphicDev, CMouse::MOUSE_CROSSHAIR);

		//플레이어&AI 아이디 설정 -->보스전 아이디는 UI-BACKBAR에서 직접 적어줌
		_tchar szID[MIN_STR];
		wsprintf(szID, L"JUSIN103");
		m_pUIMgr->Set_FlagPlayersID(OBJID::OBJECT_PLAYER, szID);

		wsprintf(szID, L"동지");
		m_pUIMgr->Set_FlagPlayersID(OBJID::OBJECT_ALLIANCE, szID);

		wsprintf(szID, L"적1");
		m_pUIMgr->Set_FlagPlayersID(OBJID::OBJECT_ENEMY_1, szID);

		wsprintf(szID, L"적2");
		m_pUIMgr->Set_FlagPlayersID(OBJID::OBJECT_ENEMY_2, szID);

		m_bLateInit = false;
		CSoundMgr::Get_Instance()->AllStop();
		CSoundMgr::Get_Instance()->BGMSTART(98);
	}

	m_pUIMgr->StageTimeCheck();

	m_pAiMgr->Update(dTimeDelta);

	////////////////////////SceneChange
	if (Engine::KeyPressing(DIK_LCONTROL))
	{
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
	if (Engine::KeyDown(DIK_F8))
	{
		m_pUIMgr->CreateResultUI_Cartel(m_pGraphicDev);
	}

	//게임 시간초과로 끝나거나 점수 500점 먼저 땄을때, 결과창 뜸
	if (m_pUIMgr->Get_CartelFinish())
	{
		m_dCreateCartelResultUITime -= dTimeDelta;

		if (0.f >= m_dCreateCartelResultUITime)
		{
			if (m_bCreateResultUI)
			{
				m_pUIMgr->CreateResultUI_Cartel(m_pGraphicDev);
				m_pUIMgr->Set_CartelFinish(false);

				m_bCreateResultUI = false;
			}

			m_dCreateCartelResultUITime = 2.f;
		}
	}


	// 일단 다시 로비...나중에 엔딩크레딧으로!
	if (CUIMgr::UITYPE_RESULT_Cartel == m_pUIMgr->Get_UIType())
	{
		if (CUIMgr::GetInstance()->CheckClickEXITButton())
		{
			if (!m_bFade)
			{
				CLoadingMgr::GetInstance()->Set_StartFade(true); //어둡게한다
				m_bFade = true;
			}
			if (m_bFade)
			{
				if (CLoadingMgr::GetInstance()->Get_StartFade() == false) //다어두워지면
				{
					Engine::ClearSubject();
					Engine::ClearRenderer();
					m_pUIMgr->ClearPointerUI();
					Engine::CScene*		pScene = nullptr;

					pScene = CLobby::Create(m_pGraphicDev);
					if (pScene == nullptr)
						return E_FAIL;

					iExit = Engine::SetUp_Scene(pScene);
					return iExit;
				}
			}
		}
	}
	m_pLightCamera->Update_GameObject(dTimeDelta);
	////////////////////////////////
	m_dEnterSceneTime += dTimeDelta;
	if (!m_bEnterScene && m_dEnterSceneTime > 5)
	{
		CLoadingMgr::GetInstance()->Set_EndFade(true);
		m_bEnterScene = true;
	}
	return iExit;
}

_int CCartel::LateUpdate_Scene(const _double & dTimeDelta)
{
	m_pAiMgr->LateUpdate(dTimeDelta);

	_int iExit = Engine::CScene::LateUpdate_Scene(dTimeDelta);

	CCameraMgr::GetInstance()->LateUpdate_Camera(dTimeDelta);

	return iExit;
}

void CCartel::Render_Scene()
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

HRESULT CCartel::Ready_Environment_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CSkySphere::Create(m_pGraphicDev, CSkySphere::SKY_CARTEL);
	if (FAILED(pLayer->Add_GameObject(L"ZSky", pGameObject)))
		return E_FAIL;
	m_mapLayer.emplace(Engine::ENVIRONMENT, pLayer);


	return S_OK;
}

HRESULT CCartel::Ready_GameObject_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CPlayer::Create(m_pGraphicDev, &_vec3(650.f, 56.f, 1260.f), &_vec3(0.f, -150.f, 0.f), &_vec3(0.5f, 0.5f, 0.5f), CBasePlayer::PLAYER_ID::ID_PLAYER);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RealPlayer", pGameObject)))
		return E_FAIL;
	m_pLightCamera->Set_PlayerTransformCom(static_cast<CBaseObject*>(pGameObject)->Get_TransformCom());
	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ AiMgr ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
	m_pAiMgr->Set_Player(dynamic_cast<CPlayer*>(pGameObject));
	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ AiMgr ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ Ai 생성 ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	pGameObject = m_pAiMgr->Create_Alliance();
	if (FAILED(pLayer->Add_GameObject(m_pAiMgr->Get_NameAlliance(), pGameObject)))
		return E_FAIL;

	pGameObject = m_pAiMgr->Create_Enermy_1();
	if (FAILED(pLayer->Add_GameObject(m_pAiMgr->Get_NameEnermy_1(), pGameObject)))
		return E_FAIL;

	m_pUIMgr->Set_AITransformCom1(static_cast<CBasePlayer*>(pGameObject)->Get_TransformCom());

	pGameObject = m_pAiMgr->Create_Enermy_2();
	if (FAILED(pLayer->Add_GameObject(m_pAiMgr->Get_NameEnermy_2(), pGameObject)))
		return E_FAIL;

	m_pUIMgr->Set_AITransformCom2(static_cast<CBasePlayer*>(pGameObject)->Get_TransformCom());

	m_pAiMgr->Team_Setting();
	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


	//pGameObject = CTwohandSword::Create(m_pGraphicDev);
	//if (pGameObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"TS", pGameObject)))
	//	return E_FAIL;
	/*pGameObject = CBelatos::Create(m_pGraphicDev, &_vec3(200.f, 50.f, 300.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.4f, 0.4f, 0.4f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Belatos", pGameObject)))
		return E_FAIL;*/


	pGameObject = CTerrain::Create(m_pGraphicDev, CTerrain::TERRAIN_CARTEL);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
		return E_FAIL;

	//pGameObject = CMouse::Create(m_pGraphicDev, CMouse::MOUSE_ZOOM); ////////////////////// 수정예정
	//if (pGameObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"Mouse", pGameObject)))
	//	return E_FAIL;

	Load_Architecture(pLayer, L"../../Data/Mesh/Map_Cartel.dat");
	m_mapLayer.emplace(Engine::GAMEOBJECT, pLayer);


	return S_OK;
}

HRESULT CCartel::Ready_UI_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	m_pUIMgr->Set_UIType(CUIMgr::UITYPE_CARTEL);
	//m_pUIMgr->CreateCartelUI(m_pGraphicDev, pLayer);
	m_mapLayer.emplace(Engine::UI, pLayer);

	return S_OK;
}

HRESULT CCartel::Ready_Light()
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

CCartel * CCartel::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCartel *	pInstance = new CCartel(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCartel::Free()
{
	//m_pUIMgr->EraseMouse();
	Engine::Safe_Single_Destory(m_pAiMgr);
	Engine::CScene::Free();
}
