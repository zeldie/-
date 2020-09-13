#include "stdafx.h"
#include "BossStage.h"
#include "Lobby.h"
#include "Cartel.h"
#include "ShootingStage.h"
#include "MatchingGame.h"
#include "PuzzleGame.h"
#include "RunGame.h"

#include "Belatos.h"
#include "Player.h"
#include "TwohandSword.h"
#include "StaticCamera.h"
#include "Terrain.h"
#include "Architecture.h"
#include "SkySphere.h"
#include "Sun.h"
#include "Cartel.h"
#include "StainedGlass.h"

#include "LightCamera.h"
CBossStage::CBossStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_bLateInit(true),
	m_iStainedGlassNum(0),
	m_dCreateBossResultUITime(2.f)	
{

}

CBossStage::~CBossStage()
{
}

HRESULT CBossStage::Load_Architecture(Engine::CLayer* pLayer, WCHAR * wstrPath)
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
		if (wstrObjTag == L"Mesh_StainedGlass")
		{
			if (m_iStainedGlassNum < 4)
			{
				if (FAILED(pLayer->Add_GameObject(wstrObjTag, CStainedGlass::Create(m_pGraphicDev, &vecPos, &vecAngle, &vecScale, true, m_iStainedGlassNum))))
					return E_FAIL;
				m_iStainedGlassNum++;
			}
			else
				if (FAILED(pLayer->Add_GameObject(wstrObjTag, CStainedGlass::Create(m_pGraphicDev, &vecPos, &vecAngle, &vecScale, false, 0))))
					return E_FAIL;

		}
		else if (FAILED(pLayer->Add_GameObject(wstrObjTag, CArchitecture::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
			return E_FAIL;


		Engine::Safe_Delete_Array(pLayerTag);
		Engine::Safe_Delete_Array(pObjTag);
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CBossStage::Ready_Scene()
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
		CCameraMgr::GetInstance()->Ready_Camera(m_pGraphicDev, CCameraMgr::STATIC);

	CCameraMgr::GetInstance()->SettingCamView();

	m_pUIMgr->SetZeroForAccumulatedVariables();
	m_pUIMgr->Set_ZeroStageTime();


	return S_OK;
}

_int CBossStage::Update_Scene(const _double & dTimeDelta)
{
	m_dTime += dTimeDelta;
	m_pUIMgr->UI_OnCheck();
	CCameraMgr::GetInstance()->Update_Camera(dTimeDelta);
	_int iExit = Engine::CScene::Update_Scene(dTimeDelta);

	if (m_bLateInit)
	{
		//벨라토스 안에서 생성 중
		//m_pUIMgr->CreateBattleUI(m_pGraphicDev, CUIMgr::UITYPE_BOSS);
		//m_pUIMgr->CreatePlayerInfoUI(m_pGraphicDev, nullptr);
		m_pUIMgr->CreateMouse(m_pGraphicDev, CMouse::MOUSE_CROSSHAIR);

		m_bLateInit = false;

		CSoundMgr::Get_Instance()->AllStop();
		CSoundMgr::Get_Instance()->BGMSTART(97);
	}

	m_pUIMgr->StageTimeCheck();


	////////////////////////SceneChange

	if (Engine::KeyDown(DIK_F8))
	{
		m_pUIMgr->Set_StageTimeStop();
		m_pUIMgr->CreateResultUI_Boss(m_pGraphicDev);
	}
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
	// 보스 죽으면 결과창이 떠야함(보스죽을때 변수true로 만듬)
	if (m_pUIMgr->Get_CreateBossResultUI())
	{
		m_dCreateBossResultUITime -= dTimeDelta;

		if (0.f >= m_dCreateBossResultUITime)
		{
			m_pUIMgr->CreateResultUI_Boss(m_pGraphicDev);
			m_pUIMgr->Set_CreateBossResultUI(false);

			m_dCreateBossResultUITime = 2.f;
		}
	}


	// 로비로
	if (CUIMgr::UITYPE_RESULT_Boss == m_pUIMgr->Get_UIType())
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

_int CBossStage::LateUpdate_Scene(const _double & dTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(dTimeDelta);
	CCameraMgr::GetInstance()->LateUpdate_Camera(dTimeDelta);
	return iExit;
}

void CBossStage::Render_Scene()
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

HRESULT CBossStage::Ready_Environment_Layer()
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
	//	0.1f,
	//	4000.f);
	//if (FAILED(pLayer->Add_GameObject(L"StaticCamera", pGameObject)))
	//	return E_FAIL;

	pGameObject = CSkySphere::Create(m_pGraphicDev, CSkySphere::SKY_BOSSSTAGE);
	if (FAILED(pLayer->Add_GameObject(L"ZSky", pGameObject)))
		return E_FAIL;
	m_mapLayer.emplace(Engine::ENVIRONMENT, pLayer);


	return S_OK;
}

HRESULT CBossStage::Ready_GameObject_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CPlayer::Create(m_pGraphicDev, &_vec3(2.f, 53.f, -877.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.5f, 0.5f, 0.5f), CBasePlayer::PLAYER_ID::ID_PLAYER); /////////////내맘대로수정
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RealPlayer", pGameObject)))
		return E_FAIL;
	m_pLightCamera->Set_PlayerTransformCom(static_cast<CBaseObject*>(pGameObject)->Get_TransformCom());
	pGameObject = CBelatos::Create(m_pGraphicDev, &_vec3(10.f, 53.f, 381.f), &_vec3(0.f, -165.f, 0.f), &_vec3(0.4f, 0.4f, 0.4f));
	if (pGameObject == nullptr)
		return E_FAIL;
	m_pUIMgr->Set_BelatosTransformCom(static_cast<CBelatos*>(pGameObject)->Get_TransformCom());
	if (FAILED(pLayer->Add_GameObject(L"Belatos", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrain::Create(m_pGraphicDev, CTerrain::TERRAIN_BOSSSTAGE);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
		return E_FAIL;

	//pGameObject = CSun::Create(m_pGraphicDev, &_vec3(-1558.f, 1510.f, -1578.f), &_vec3(0.f, 0.f, 0.f), &_vec3(5.f, 5.f, 5.f));
	//if (pGameObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"Sun", pGameObject)))
	//	return E_FAIL;

	pGameObject = CSun::Create(m_pGraphicDev, &_vec3(0.f, -100.f, 20.f), &_vec3(0.f, 0.f, 0.f), &_vec3(5.f, 5.f, 5.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Sun", pGameObject)))
		return E_FAIL;

	Load_Architecture(pLayer, L"../../Data/Mesh/Map_Boss.dat");
	m_mapLayer.emplace(Engine::GAMEOBJECT, pLayer);


	return S_OK;
}

HRESULT CBossStage::Ready_UI_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	//Engine::CGameObject*		pGameObject = nullptr;
	m_pUIMgr->Set_UIType(CUIMgr::UITYPE_BOSS);
	//m_pUIMgr->CreateBattleUI(m_pGraphicDev, pLayer, CUIMgr::UITYPE_BOSS);

	m_mapLayer.emplace(Engine::UI, pLayer);

	return S_OK;
}

HRESULT CBossStage::Ready_Light()
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

CBossStage * CBossStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossStage *	pInstance = new CBossStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBossStage::Free()
{
	Engine::CScene::Free();
	//m_pUIMgr->EraseMouse();

}
