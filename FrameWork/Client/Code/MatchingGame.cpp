#include "stdafx.h"
#include "MatchingGame.h"

#include "Lobby.h"
#include "BossStage.h"
#include "Cartel.h"
#include "ShootingStage.h"
#include "PuzzleGame.h"
#include "RunGame.h"

#include "MiniGameCamera.h"
#include "MatchingPlayer.h"
#include "MatchingAI.h"
#include "SkySphere.h"
#include "BackBar.h"

CMatchingGame::CMatchingGame(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
	, m_pUIMgr(CUIMgr::GetInstance())
	, m_bInit(false)
{
}

CMatchingGame::~CMatchingGame()
{
}

HRESULT CMatchingGame::Load_Architecture(Engine::CLayer* pLayer, WCHAR * wstrPath)
{
	//HANDLE hFile = CreateFile(wstrPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	//if (INVALID_HANDLE_VALUE == hFile)
	//	return E_FAIL;

	//DWORD dwByte = 0;
	//DWORD dwstrLayerLen = 0;
	//DWORD dwstrObjLen = 0;
	//wstring wstrLayerTag = L"";
	//TCHAR* pLayerTag = nullptr;
	//wstring wstrObjTag = L"";
	//TCHAR* pObjTag = nullptr;
	//_vec3	vecPos = _vec3(0.f, 0.f, 0.f);
	//_vec3	vecAngle = _vec3(0.f, 0.f, 0.f);
	//_vec3	vecScale = _vec3(0.f, 0.f, 0.f);

	//while (true)
	//{
	//	//레이어 읽기
	//	ReadFile(hFile, &dwstrLayerLen, sizeof(DWORD), &dwByte, nullptr);
	//	if (0 == dwByte)
	//		break;
	//	pLayerTag = new TCHAR[dwstrLayerLen];
	//	ReadFile(hFile, pLayerTag, dwstrLayerLen, &dwByte, nullptr);
	//	wstrLayerTag = (LPCTSTR)pLayerTag;

	//	//오브젝트이름 읽기
	//	ReadFile(hFile, &dwstrObjLen, sizeof(DWORD), &dwByte, nullptr);
	//	pObjTag = new TCHAR[dwstrObjLen];
	//	ReadFile(hFile, pObjTag, dwstrObjLen, &dwByte, nullptr);
	//	wstrObjTag = (LPCTSTR)pObjTag;

	//	//위치, 각도, 스케일 읽기
	//	ReadFile(hFile, &vecPos, sizeof(_vec3), &dwByte, nullptr);
	//	ReadFile(hFile, &vecAngle, sizeof(_vec3), &dwByte, nullptr);
	//	ReadFile(hFile, &vecScale, sizeof(_vec3), &dwByte, nullptr);

	//	if (L"Mesh_NPC_Crystal_Ani" == wstrObjTag || L"Mesh_H_CubeOfHildard" == wstrObjTag || L"Mesh_GardenSwing01" == wstrObjTag)
	//	{
	//		if (FAILED(pLayer->Add_GameObject(wstrObjTag, CArchitecture_Dynamic::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
	//			return E_FAIL;
	//	}
	//	else if (wstrObjTag == L"Mesh_StainedGlass")
	//	{
	//		if (FAILED(pLayer->Add_GameObject(wstrObjTag, CStainedGlass::Create(m_pGraphicDev, &vecPos, &vecAngle, &vecScale))))
	//			return E_FAIL;
	//	}
	//	else
	//	{
	//		if (FAILED(pLayer->Add_GameObject(wstrObjTag, CArchitecture::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
	//			return E_FAIL;
	//	}

	//	Engine::Safe_Delete_Array(pLayerTag);
	//	Engine::Safe_Delete_Array(pObjTag);
	//}

	//CloseHandle(hFile);
	return S_OK;
}

HRESULT CMatchingGame::Ready_Scene()
{
	if (FAILED(Engine::AddSubject(Engine::MATCHING_GMAE)))
		return E_FAIL;

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

_int CMatchingGame::Update_Scene(const _double & dTimeDelta)
{
	m_dTime += dTimeDelta;

	_int iExit = Engine::CScene::Update_Scene(dTimeDelta);
	LateInit();

	//CCameraMgr::GetInstance()->Update_Camera(dTimeDelta);
	//m_pUIMgr->UI_OnCheck();

	////////////////////////SceneChange

	if (CUIMgr::GetInstance()->CheckClickEXITButton())
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

			pScene = CLobby::Create(m_pGraphicDev);
			if (pScene == nullptr)
				return E_FAIL;

			iExit = Engine::SetUp_Scene(pScene);

			return iExit;
		}
	}

	// =====================================================================================
	// 키 입력이아니라 제대로 씬 전환할때, 매니저에서 데드 함수 호출하는거 추가해야함
	// =====================================================================================
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
	////////////////////////////////

	CMatchingMgr::GetInstance()->Update_MatchingMgr(dTimeDelta);

	// ====================== 임시 발동 코드 ==================================
	if (Engine::KeyDown(DIK_1))
	{
		CMatchingMgr::GetInstance()->Create_Card();
	}
	if (Engine::KeyDown(DIK_2))
	{
		CMatchingMgr::GetInstance()->Shuffle_Card();
	}
	if (Engine::KeyDown(DIK_3))
	{
		CMatchingMgr::GetInstance()->Delete_Card();
	}
	if (Engine::KeyDown(DIK_4))
	{

	}
	// ====================== 임시 발동 코드 ==================================
	/*if (CMatchingMgr::GetInstance()->Get_GameState() == CMatchingMgr::STATE_START)
		//cout << "스타트" << endl;
	else if (CMatchingMgr::GetInstance()->Get_GameState() == CMatchingMgr::STATE_ING)
		//cout << "ING" << endl;
	else
		//cout << "끝" << endl;*/
	return iExit;
}

_int CMatchingGame::LateUpdate_Scene(const _double & dTimeDelta)
{
	//CCameraMgr::GetInstance()->LateUpdate_Camera(dTimeDelta);
	_int iExit = Engine::CScene::LateUpdate_Scene(dTimeDelta);

	return iExit;
}

void CMatchingGame::Render_Scene()
{
	if (CMatchingMgr::STATE_END != CMatchingMgr::GetInstance()->Get_GameState())
	{
		///////////matchingGame Timer UI ///////// - 희정 추가
		CMatchingMgr::GetInstance()->RenderTime();
		//////////////////////////////////////////
	}

	++m_dwRenderCnt;
	if (m_dTime >= 1)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_dTime = 0;
		m_dwRenderCnt = 0;
	}

	Engine::Render_Font(L"Font_Default", m_szFPS, &_vec2(20.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}

void CMatchingGame::LateInit()
{
	if (m_bInit)
		return;
	m_bInit = true;
	CSoundMgr::Get_Instance()->AllStop();
	CSoundMgr::Get_Instance()->HoBGM(43);
	CUIMgr::GetInstance()->CreateMouse(m_pGraphicDev, CMouse::MOUSE_PICKING);

}

HRESULT CMatchingGame::Ready_Environment_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	//pGameObject = CMiniGameCamera::Create(m_pGraphicDev,
	//	&_vec3(800.f, 100.f, 0.f),
	//	&_vec3(0.f, 0.f, 1.f),
	//	&_vec3(0.f, 1.f, 0.f),
	//	D3DXToRadian(45.f),
	//	_float(WINCX) / WINCY,
	//	0.7f,
	//	4000.f);
	//if (FAILED(pLayer->Add_GameObject(L"MiniGameCamera", pGameObject)))
	//	return E_FAIL;

	pGameObject = CSkySphere::Create(m_pGraphicDev, CSkySphere::SKY_SHOOTINGSTAGE);
	if (FAILED(pLayer->Add_GameObject(L"ZSky", pGameObject)))
		return E_FAIL;

	m_mapLayer.emplace(Engine::ENVIRONMENT, pLayer);


	return S_OK;
}

HRESULT CMatchingGame::Ready_GameObject_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;


	pGameObject = CMatchingPlayer::Create(m_pGraphicDev, 130.f, 950.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"MatchingPlayer", pGameObject)))
		return E_FAIL;


	pGameObject = CMatchingAI::Create(m_pGraphicDev, 1135.f, 950.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"MatchingAI", pGameObject)))
		return E_FAIL;


	m_mapLayer.emplace(Engine::GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CMatchingGame::Ready_UI_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CBackBar::Create(m_pGraphicDev, CBackBar::MATCHINGBGTYPE, 0.f, 0.f, WINCX, WINCY, 0.3f);
	pLayer->Add_GameObject(L"BackBar", pGameObject);

	m_pUIMgr->CreateMatchingUI(m_pGraphicDev, pLayer);
	m_mapLayer.emplace(Engine::UI, pLayer);

	return S_OK;
}

HRESULT CMatchingGame::Ready_Light()
{
	//D3DLIGHT9			tLightInfo;
	//ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	//tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	//tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	//tLightInfo.Direction = _vec3(0.f, -1.f, 1.f);
	//if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1)))
	//	return E_FAIL;
	return S_OK;
}

CMatchingGame * CMatchingGame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMatchingGame *	pInstance = new CMatchingGame(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMatchingGame::Free()
{

	Engine::CScene::Free();
}
