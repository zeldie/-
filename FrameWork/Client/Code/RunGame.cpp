#include "stdafx.h"
#include "RunGame.h"

#include "Lobby.h"
#include "BossStage.h"
#include "Cartel.h"
#include "MatchingGame.h"
#include "PuzzleGame.h"
#include "ShootingStage.h"

#include "RunPlayer.h"
#include "RunGameCamera.h"

#include "SkySphere.h"
#include "Architecture_Run.h"
#include "Architecture_Dynamic.h"
#include "RunTerrain.h"
#include "RunWeapon_LongBow.h"


#include "RunObj_Obstacle.h"



#include "MiniGameCamera.h"
#include "StaticCamera.h"

#include "LightCamera.h"
CRunGame::CRunGame(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
	, m_pUIMgr(CUIMgr::GetInstance())
	, m_bInit(false)
	, m_szStageSecond(L"")
	, m_dTotalTime(0.f)
	, m_bOneCreateUI(true)
	, m_bRenderStart(false)
	, m_bOne(true)
{
}

CRunGame::~CRunGame()
{
}

HRESULT CRunGame::Load_Architecture(Engine::CLayer* pLayer, WCHAR * wstrPath)
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
	_uint	iSect = 0;
	while (true)
	{
		//���̾� �б�
		ReadFile(hFile, &dwstrLayerLen, sizeof(DWORD), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		pLayerTag = new TCHAR[dwstrLayerLen];
		ReadFile(hFile, pLayerTag, dwstrLayerLen, &dwByte, nullptr);
		wstrLayerTag = (LPCTSTR)pLayerTag;

		//������Ʈ�̸� �б�
		ReadFile(hFile, &dwstrObjLen, sizeof(DWORD), &dwByte, nullptr);
		pObjTag = new TCHAR[dwstrObjLen];
		ReadFile(hFile, pObjTag, dwstrObjLen, &dwByte, nullptr);
		wstrObjTag = (LPCTSTR)pObjTag;

		//��ġ, ����, ������ �б�
		ReadFile(hFile, &vecPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vecAngle, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vecScale, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vSelRotation, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &iSect, sizeof(_uint), &dwByte, nullptr);

		if (FAILED(pLayer->Add_GameObject(wstrObjTag, CArchitecture_Run::Create(m_pGraphicDev, wstrObjTag, &vecPos, &vecAngle, &vecScale, &vSelRotation, (RUN_LINE_SECT)iSect))))
			return E_FAIL;

		Engine::Safe_Delete_Array(pLayerTag);
		Engine::Safe_Delete_Array(pObjTag);
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CRunGame::Ready_Scene()
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
		CCameraMgr::GetInstance()->Change_Camera(CCameraMgr::RUNGAME);
		CCameraMgr::GetInstance()->Ready_Camera(m_pGraphicDev, CCameraMgr::RUNGAME);
	}
	else
		CCameraMgr::GetInstance()->Ready_Camera(m_pGraphicDev, CCameraMgr::RUNGAME);

	CRunGameMgr::GetInstance()->Load_Line(L"../../Data/Mesh/Line_Bot_Left.dat", LINETYPE::LINE_BOTTOM_LEFT, SECT_1);
	CRunGameMgr::GetInstance()->Load_Line(L"../../Data/Mesh/Line_Bot_Mid.dat", LINETYPE::LINE_BOTTOM_MID, SECT_1);
	CRunGameMgr::GetInstance()->Load_Line(L"../../Data/Mesh/Line_Bot_Right.dat", LINETYPE::LINE_BOTTOM_RIGHT, SECT_1);
	CRunGameMgr::GetInstance()->Load_Line(L"../../Data/Mesh/Line_Wall_Left.dat", LINETYPE::LINE_WALL_LEFT_MID, SECT_1);
	CRunGameMgr::GetInstance()->Load_Line(L"../../Data/Mesh/Line_Wall_Right.dat", LINETYPE::LINE_WALL_RIGHT_MID, SECT_1);
	CRunGameMgr::GetInstance()->Load_Line(L"../../Data/Mesh/Line_Top_Mid.dat", LINETYPE::LINE_TOP_MID, SECT_1);

	//CRunGameMgr::GetInstance()->Load_Line(L"../../Data/Mesh/Line_Bot_Left2.dat", LINETYPE::LINE_BOTTOM_LEFT, SECT_2);

	CCameraMgr::GetInstance()->SettingCamView();

	m_pUIMgr->SetZeroForAccumulatedVariables();
	m_pUIMgr->Set_ZeroStageTime();


	return S_OK;
}

_int CRunGame::Update_Scene(const _double & dTimeDelta)
{
	m_dTime += dTimeDelta;
	CCameraMgr::GetInstance()->Update_Camera(dTimeDelta);
	m_pUIMgr->UI_OnCheck();

	_int iExit = Engine::CScene::Update_Scene(dTimeDelta);

	if (CUIMgr::GetInstance()->Get_RunGameStart())
	{
		m_bRenderStart = true;
	}

	LateInit();

	//m_pUIMgr->StageTimeCheck();
	StageTimeCheck(dTimeDelta);

	////////////////////////SceneChange
	// =================== ������ �����ٰ� �ٽ� ���ðŸ� �Ŵ������� Clear_List() �Լ� ȣ���ؾ��� ======================
	if (Engine::KeyPressing(DIK_LCONTROL))
	{
		if (Engine::KeyDown(DIK_F5))
		{
			Engine::ClearSubject();
			Engine::ClearRenderer();
			m_pUIMgr->ClearPointerUI();
			CRunGameMgr::GetInstance()->Clear_List();

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
			CRunGameMgr::GetInstance()->Clear_List();

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
			CRunGameMgr::GetInstance()->Clear_List();

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
			CRunGameMgr::GetInstance()->Clear_List();

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
			CRunGameMgr::GetInstance()->Clear_List();

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
			CRunGameMgr::GetInstance()->Clear_List();

			Engine::CScene*		pScene = nullptr;

			pScene = CPuzzleGame::Create(m_pGraphicDev);
			if (pScene == nullptr)
				return E_FAIL;

			iExit = Engine::SetUp_Scene(pScene);

			return iExit;
		}
	}

	if (Engine::KeyDown(DIK_F8))
	{
		m_pUIMgr->CreateResultUI_Run(m_pGraphicDev);
	}

	//�ƾ������� ���â!
	if (CCameraMgr::GetInstance()->Get_ItemGetCheck())
	{
		if (m_bOneCreateUI)
		{
			//���â
			m_pUIMgr->CreateResultUI_Run(m_pGraphicDev);
			m_bOneCreateUI = false;
		}
	}

	////////////////////////////////
	// ���â -> �κ��
	if (CUIMgr::UITYPE_RESULT_Run == m_pUIMgr->Get_UIType())
	{
		if (CUIMgr::GetInstance()->CheckClickEXITButton())
		{
			if (!m_bFade)
			{
				CLoadingMgr::GetInstance()->Set_StartFade(true); //��Ӱ��Ѵ�
				m_bFade = true;
			}
			if (m_bFade)
			{
				if (CLoadingMgr::GetInstance()->Get_StartFade() == false) //�پ�ο�����
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
	if (!m_bEnterScene && m_dEnterSceneTime > 2)
	{
		CLoadingMgr::GetInstance()->Set_EndFade(true);
		m_bEnterScene = true;
		
		if (m_bOne)
		{
			m_pUIMgr->CreateRunUI(m_pGraphicDev);
			m_bOne = false;
		}

	}
	return iExit;
}

_int CRunGame::LateUpdate_Scene(const _double & dTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(dTimeDelta);
	CCameraMgr::GetInstance()->LateUpdate_Camera(dTimeDelta);
	return iExit;
}

void CRunGame::Render_Scene()
{
	//++m_dwRenderCnt;
	//if (m_dTime >= 1)
	//{
	//	wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
	//	m_dTime = 0;
	//	m_dwRenderCnt = 0;
	//}

	//Engine::Render_Font(L"Font_Default", m_szFPS, &_vec2(20.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	//////���� �߰�
	//�������ӿ����� ���� �ּ��ɰŴϱ�.
	//���â�� ���� �����ϰ� �ð��� ���δ�.
	if (CUIMgr::UITYPE_RESULT_Run != CUIMgr::GetInstance()->Get_UIType())
	{
		if (m_bRenderStart)
		{
			//�ð�
			RenderTime();
			//����
			RenderPoints();
		}
	}	
}

void CRunGame::RenderTime()
{
	//_double dStageTime = m_pUIMgr->Get_StageTime();
	//�ð� ����
	m_pUIMgr->SetAccumulatedTime(m_dTotalTime);

	//�տ� ��
	wsprintf(m_szStageSecond, L"%d", (_uint)m_dTotalTime);

	//�ڸ������ϱ�-> ���� ��ġ
	_uint iQuotient = (_uint)m_dTotalTime / 100;
	_uint iUnitNumber = 0;
	if (0 != iQuotient) //3�ڸ���
	{
		iUnitNumber = 3;
	}
	else
	{
		if (0 != (_uint)m_dTotalTime / 10) // 2�ڸ���
		{
			iUnitNumber = 2;
		}
		else //1�ڸ���
		{
			iUnitNumber = 1;
		}
	}

	//�Ҽ����ڸ� ��
	_double dSemiSecondTime = (m_dTotalTime - _uint(m_dTotalTime)) * 100.f;
	wstring wstrSs = L"." + to_wstring(_uint(dSemiSecondTime)) + L"s";


	//////////////////////0.00s �� ����////////////////////
	if (1 == iUnitNumber)
	{
		Engine::Render_Font(L"Font_GODICHEAVY2", m_szStageSecond, &_vec2(60.f, 20.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Engine::Render_Font(L"Font_GODIC3", wstrSs, &_vec2(92.f, 35.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (2 == iUnitNumber)
	{
		Engine::Render_Font(L"Font_GODICHEAVY2", m_szStageSecond, &_vec2(40.f, 20.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Engine::Render_Font(L"Font_GODIC3", wstrSs, &_vec2(112.f, 35.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (3 == iUnitNumber)
	{
		Engine::Render_Font(L"Font_GODICHEAVY2", m_szStageSecond, &_vec2(20.f, 20.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Engine::Render_Font(L"Font_GODIC3", wstrSs, &_vec2(132.f, 35.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	////////////////////////////////////////////////////////////


	////////////////// 00:00���� ����////////////////////
	//_uint iQuotient = (_ulong)dStageTime / 60;
	//_uint iRemainder = (_ulong)dStageTime % 60;

	//_uint iMinuteTens, iMinuteUnits, iSecondTens, iSecondUnits = 0;
	//iMinuteTens = iQuotient / 10;
	//iMinuteUnits = iQuotient % 10;

	//iSecondTens = iRemainder / 10;
	//iSecondUnits = iRemainder % 10;

	//m_wstrStageTime = to_wstring(iMinuteTens) + to_wstring(iMinuteUnits) + L":" + to_wstring(iSecondTens) + to_wstring(iSecondUnits);
	//Engine::Render_Font(L"Font_GODICHEAVY2", m_wstrStageTime, &_vec2(40.f, 15.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	//////////////////////////////////////////////////////
}

void CRunGame::RenderPoints()
{
	_tchar szPoint[MIN_STR];
	ZeroMemory(szPoint, sizeof(_tchar)*(MIN_STR));
	MakeComma(m_pUIMgr->GetAccumulatedRunGamePoints(), szPoint);

	Engine::Render_Font(L"Font_GODIC3", szPoint, &_vec2(600.f,70.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void CRunGame::StageTimeCheck(const _double& dTimeDelta)
{
	if (!m_pUIMgr->Get_RunGameTimeStop())
	{
		m_dTotalTime += dTimeDelta;
		//�ð��帣�� ������ 1 ���� ������
		m_pUIMgr->SetAccumulatedRunGamePoints(1);
	}
	else
	{
		m_dTotalTime = m_dTotalTime;
	}
}

void CRunGame::MakeComma(_uint iData, _tchar * szResult)
{
	_tchar szBuff[MAX_STR], *szCopy;

	_uint iLen = wsprintf(szBuff, L"%d", iData);
	//���� ���� ����

	szCopy = szBuff;
	//�ڸ����� 3�ڸ� �̻��̾�� ',' ��� -> �ڸ��� üũ
	_uint AddCnt = iLen / 3;
	_uint SkipCnt = iLen % 3;

	if (AddCnt > 0 && !SkipCnt)
		AddCnt--;

	//�ڸ����� 3�ڸ� �̻��̸� ',' ��´�
	if (AddCnt > 0)
	{
		//���ο� �ִ� ','�տ� �߰��� ���ں���
		if (SkipCnt > 0)
		{
			//memcpy(szResult, szCopy, SkipCnt);
			lstrcpy(&szResult[0], &szBuff[0]);

			szResult += SkipCnt;
			*szResult++ = ',';

			szCopy += SkipCnt;
			iLen -= SkipCnt;
		}

		for (_uint i = 0; i < iLen; i++)
		{
			//3�� ������� ',' �߰�
			if (i && !(i % 3))
				*szResult++ = ',';

			*szResult++ = *szCopy++;
		}

		*szResult = 0;
	}
	else
	{
		//�ڸ����� 3�ڸ��� �ȵǸ� ��������
		memcpy(szResult, szCopy, iLen + 1);
	}
}

HRESULT CRunGame::Ready_Environment_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CSkySphere::Create(m_pGraphicDev, CSkySphere::SKY_RUNGAME);
	if (FAILED(pLayer->Add_GameObject(L"RunGame_Sky", pGameObject)))
		return E_FAIL;

	m_mapLayer.emplace(Engine::ENVIRONMENT, pLayer);

	return S_OK;
}

HRESULT CRunGame::Ready_GameObject_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CRunPlayer::Create(m_pGraphicDev, &_vec3(0.f, 506.f, 0.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.1f, 0.1f, 0.1f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RunPlayer", pGameObject)))
		return E_FAIL;
	m_pLightCamera->Set_PlayerTransformCom(static_cast<CBaseObject*>(pGameObject)->Get_TransformCom());

	pGameObject = CRunTerrain::Create(m_pGraphicDev, CRunTerrain::TERRAIN_RUNGAME);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
		return E_FAIL;

	pGameObject = CRunWeapon_LongBow::Create(m_pGraphicDev, &_vec3(0.f, 550.f, 23590.f), &_vec3(20.f, 90.f, 45.f), &_vec3(0.2f, 0.2f, 0.2f));
	//pGameObject = CRunWeapon_LongBow::Create(m_pGraphicDev, &_vec3(0.f, 550.f, -100.f), &_vec3(20.f, 90.f, 45.f), &_vec3(0.2f, 0.2f, 0.2f));
	//pGameObject = CRunWeapon_LongBow::Create(m_pGraphicDev, &_vec3(0.f, 550.f, -100.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.2f, 0.2f, 0.2f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RunWeapon", pGameObject)))
		return E_FAIL;

	Load_Architecture(pLayer, L"../../Data/Mesh/Map_Run001.dat");
	//Load_Architecture(pLayer, L"../../Data/Mesh/Map_Run02.dat");
	//Load_Architecture(pLayer, L"../../Data/Mesh/Map_RunTestBig.dat");

	m_mapLayer.emplace(Engine::GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CRunGame::Ready_UI_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	//Engine::CGameObject*		pGameObject = nullptr;
	m_pUIMgr->Set_UIType(CUIMgr::UITYPE_RUN);
	m_mapLayer.emplace(Engine::UI, pLayer);
	return S_OK;
}

HRESULT CRunGame::Ready_Light()
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

void CRunGame::LateInit()
{
	if (m_bInit)
		return;

	CSoundMgr::Get_Instance()->AllStop();
	CSoundMgr::Get_Instance()->BGMSTART(99);
	m_bInit = true;

	CRunGameMgr::GetInstance()->Create_Obstacle(1);

	
	m_pUIMgr->CreateMouse(m_pGraphicDev, CMouse::MOUSE_INVISIBLE);
}

CRunGame * CRunGame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRunGame *	pInstance = new CRunGame(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunGame::Free()
{
	//m_pUIMgr->EraseMouse();
	Engine::CScene::Free();
}
