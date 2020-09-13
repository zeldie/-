#include "stdafx.h"
#include "ShootingStage.h"

#include "Lobby.h"
#include "BossStage.h"
#include "Cartel.h"
#include "MatchingGame.h"
#include "PuzzleGame.h"
#include "RunGame.h"

#include "Player.h"
#include "Apostle.h"
#include "TwohandSword.h"
#include "StaticCamera.h"
#include "Terrain.h"
#include "Architecture.h"
#include "Architecture_Dynamic.h"
#include "SkySphere.h"
#include "StainedGlass.h"

#include "Architecture_Run.h"
#include "LightCamera.h"
#include "RunPlayer.h"

CShootingStage::CShootingStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_bLateInit(true),
	m_dCreateShootingResultUITime(2.f),
	m_pPlayer(nullptr),
	m_pApostle(nullptr),
	m_bIsTrans(false),
	m_dTransDelay(0.0),
	m_bOneClearUI(true),
	m_dChangeScene(3.f)
{
}

CShootingStage::~CShootingStage()
{
}

HRESULT CShootingStage::Load_Architecture(Engine::CLayer* pLayer, WCHAR * wstrPath)
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
	_vec3	vSelRotation = _vec3(0.f, 0.f, 0.f);

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

		if (FAILED(pLayer->Add_GameObject(wstrObjTag, CArchitecture::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale))))
			return E_FAIL;

		Engine::Safe_Delete_Array(pLayerTag);
		Engine::Safe_Delete_Array(pObjTag);
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CShootingStage::Ready_Scene()
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

_int CShootingStage::Update_Scene(const _double & dTimeDelta)
{
	m_dTime += dTimeDelta;
	m_pUIMgr->UI_OnCheck();
	CCameraMgr::GetInstance()->Update_Camera(dTimeDelta);
	_int iExit = Engine::CScene::Update_Scene(dTimeDelta);

	if (m_bLateInit)
	{
		//  //Apostle 상태에서 생성중
		//	//m_pUIMgr->CreateBattleUI(m_pGraphicDev, CUIMgr::UITYPE_SHOOTING);
		//	//m_pUIMgr->CreatePlayerInfoUI(m_pGraphicDev, nullptr);
		m_pUIMgr->CreateMouse(m_pGraphicDev, CMouse::MOUSE_INVISIBLE);
		m_bLateInit = false;

		CSoundMgr::Get_Instance()->AllStop();
		CSoundMgr::Get_Instance()->BGMSTART(96);
		CSoundMgr::Get_Instance()->SetVolumeDown(0.5f);
	}

	m_pUIMgr->StageTimeCheck();

	//변신 후 런게임 이동
	if (!m_bOneClearUI)
	{
		m_dChangeScene -= dTimeDelta;
		if (0.f >= m_dChangeScene)
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

					pScene = CRunGame::Create(m_pGraphicDev);
					if (pScene == nullptr)
						return E_FAIL;

					iExit = Engine::SetUp_Scene(pScene);
					return iExit;
				}
			}
		}

	}

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
	// Apostle 죽으면 결과창이 떠야함(Apostle죽을때 변수true로 만듬)
	if (m_pUIMgr->Get_CreateShootingResultUI())
	{
		m_dCreateShootingResultUITime -= dTimeDelta;

		if (0.f >= m_dCreateShootingResultUITime)
		{
			m_pUIMgr->CreateResultUI_ShootingStage(m_pGraphicDev);
			m_pUIMgr->Set_CreateShootingResultUI(false);

			m_dCreateShootingResultUITime = 2.f;
		}
	}

	// 변신해야되니까 유아이만 없애주기
	if (CUIMgr::UITYPE_RESULT_Shooting == m_pUIMgr->Get_UIType())
	{
		if (CUIMgr::GetInstance()->CheckClickEXITButton())
		{
			if (m_bOneClearUI)
			{
				m_pUIMgr->ClearUI();

				//// Apostle end cutscene set
				//int i = 0;
				//
				//// Player Cutscene et


				m_bIsTrans = true;
				m_dTransDelay = g_dAccumulatedTime +2.0;
			}
		}
	}
	////////////////////////////////
	m_dEnterSceneTime += dTimeDelta;
	if (!m_bEnterScene && m_dEnterSceneTime > 2)
	{
		CLoadingMgr::GetInstance()->Set_EndFade(true);
		m_bEnterScene = true;
	}

	if (m_bIsTrans && (m_dTransDelay < g_dAccumulatedTime))
	{
		m_bIsTrans = false;
		// 변신
		dynamic_cast<CPlayer*>(m_pPlayer)->Set_TransApostle(true);
		dynamic_cast<CApostle*>(m_pApostle)->Set_Render(true);

		_vec3	vAngle = { 90.f,0.f,0.f };
		_vec3 vPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_TransformCom()->m_vInfo[Engine::INFO_POS];
		BASE_INFO	tBaseInfo;
		memcpy(&tBaseInfo, dynamic_cast<CPlayer*>(m_pPlayer)->Get_BaseInfo(), sizeof(BASE_INFO));
		CEffectMgr::GetInstance()->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_SPHERE_GOLD2, &vPos, &vAngle, &tBaseInfo);

		_vec3 vDir = Engine::GetDirNoY(vPos, CCameraMgr::GetInstance()->Get_Pos());
		vAngle.x = 0.f;
		vPos += +_vec3(0.f, 125.f, 0.f) - vDir*80.f;
		CEffectMgr::GetInstance()->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_TRANS_APOSTLE, &vPos, &vAngle, &tBaseInfo);

		Engine::CGameObject* pRunPlayer = CRunPlayer::Create(m_pGraphicDev, &dynamic_cast<CPlayer*>(m_pPlayer)->Get_TransformCom()->m_vInfo[Engine::INFO_POS], &_vec3(0.f, 0.f, 0.f), &_vec3(0.5f, 0.5f, 0.5f));
		Engine::Add_GameObject(Engine::GAMEOBJECT, L"RunPlayer", pRunPlayer);
		dynamic_cast<CRunPlayer*>(pRunPlayer)->Set_Shooting(true);

		m_bOneClearUI = false;
	}
	return iExit;
}

_int CShootingStage::LateUpdate_Scene(const _double & dTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(dTimeDelta);
	CCameraMgr::GetInstance()->LateUpdate_Camera(dTimeDelta);
	return iExit;
}

void CShootingStage::Render_Scene()
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

HRESULT CShootingStage::Ready_Environment_Layer()
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

	pGameObject = CSkySphere::Create(m_pGraphicDev, CSkySphere::SKY_SHOOTINGSTAGE);
	if (FAILED(pLayer->Add_GameObject(L"ZSky", pGameObject)))
		return E_FAIL;
	m_mapLayer.emplace(Engine::ENVIRONMENT, pLayer);

	return S_OK;
}

HRESULT CShootingStage::Ready_GameObject_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;
	//Player Starting Point
	m_pPlayer = pGameObject = CPlayer::Create(m_pGraphicDev, &_vec3(4.f, 75.f, 1300.f), &_vec3(0.f, 180.f, 0.f), &_vec3(0.5f, 0.5f, 0.5f), CBasePlayer::PLAYER_ID::ID_PLAYER);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RealPlayer", pGameObject)))
		return E_FAIL;
	m_pLightCamera->Set_PlayerTransformCom(static_cast<CBaseObject*>(pGameObject)->Get_TransformCom());

	pGameObject = CTerrain::Create(m_pGraphicDev, CTerrain::TERRAIN_SHOOTINGSTAGE);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
		return E_FAIL;

	Load_Architecture(pLayer, L"../../Data/Mesh/Map_Apostle.dat");
	m_mapLayer.emplace(Engine::GAMEOBJECT, pLayer);

	m_pApostle = pGameObject = CApostle::Create(m_pGraphicDev, &_vec3(0.f, 76.f, 20.f), &_vec3(0.f, 0.f, 0.f), &_vec3(1.f, 1.f, 1.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Apostle", pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShootingStage::Ready_UI_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	//Engine::CGameObject*		pGameObject = nullptr;

	m_pUIMgr->Set_UIType(CUIMgr::UITYPE_SHOOTING);
	//m_pUIMgr->CreateBattleUI(m_pGraphicDev, pLayer, CUIMgr::UITYPE_SHOOTING);
	m_mapLayer.emplace(Engine::UI, pLayer);

	return S_OK;
}

HRESULT CShootingStage::Ready_Light()
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

CShootingStage * CShootingStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShootingStage *	pInstance = new CShootingStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CShootingStage::Free()
{
	if (m_pLightCamera != nullptr)
		m_pLightCamera->Release_TransformCom();
	Engine::CScene::Free();
}
