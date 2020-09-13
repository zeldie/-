#include "stdafx.h"
#include "Customize.h"

#include "Lobby.h"

#include "SkySphere.h"
#include "Body.h"
#include "Player.h"
CCustomize::CCustomize(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_bLateInit(true),
	m_dSceneChange(0.1f)
{
}


CCustomize::~CCustomize()
{
}

HRESULT CCustomize::Ready_Scene()
{
	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_GameObject_Layer()))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Light()))
		return E_FAIL;
	
	CCameraMgr::GetInstance()->Ready_Camera(m_pGraphicDev, CCameraMgr::CUSTOM);
	//CLoadingMgr::GetInstance()->Set_EndFade(true);
	CCameraMgr::GetInstance()->SettingCamView();

	return S_OK;
}

_int CCustomize::Update_Scene(const _double & dTimeDelta)
{
	CCameraMgr::GetInstance()->Update_Camera(dTimeDelta);
	_int iExit = Engine::CScene::Update_Scene(dTimeDelta);

	if (m_bLateInit)
	{
		m_pUIMgr->CreateMouse(m_pGraphicDev, CMouse::MOUSE_PICKING);
		CSoundMgr::Get_Instance()->AllStop();
		CSoundMgr::Get_Instance()->BGMSTART(100);
		m_bLateInit = false;
	}

	//커마 -> 로비
	if (CUIMgr::GetInstance()->Get_FinishCustomize())
	{
		//버튼 누르고 애니매이션 한번돌고 멈추면 시간재기 시작
		m_dSceneChange -= dTimeDelta;
		if (0.f >= m_dSceneChange)
		{
			m_dSceneChange = 0.f;

			if (!m_bFade)
			{
				CLoadingMgr::GetInstance()->Set_StartFade(true); //어둡게
				m_bFade = true;
			}

			if (CLoadingMgr::GetInstance()->Get_StartFade() == false) // 다 어두워지면
			{
				Engine::ClearSubject();
				Engine::ClearRenderer();
				m_pUIMgr->ClearAllCustomizingUI();
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




	//if (Engine::KeyDown(DIK_F5))
	//{
	//	Engine::ClearSubject();
	//	Engine::ClearRenderer();
	//	m_pUIMgr->ClearPointerUI();

	//	Engine::CScene*		pScene = nullptr;

	//	pScene = CLobby::Create(m_pGraphicDev);
	//	if (pScene == nullptr)
	//		return E_FAIL;

	//	iExit = Engine::SetUp_Scene(pScene);

	//	return iExit;
	//}

	return iExit;
}

_int CCustomize::LateUpdate_Scene(const _double & dTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(dTimeDelta);
	CCameraMgr::GetInstance()->LateUpdate_Camera(dTimeDelta);

	return iExit;
}

void CCustomize::Render_Scene()
{
}

HRESULT CCustomize::Ready_Environment_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CSkySphere::Create(m_pGraphicDev, CSkySphere::SKY_BOSSSTAGE);
	if (FAILED(pLayer->Add_GameObject(L"ZSky", pGameObject)))
		return E_FAIL;

	m_mapLayer.emplace(Engine::ENVIRONMENT, pLayer);
	return S_OK;
}

HRESULT CCustomize::Ready_GameObject_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;


	pGameObject = CBody::Create(m_pGraphicDev, CBody::FEMALE, &_vec3(0.f, 50.f, 20.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.5f, 0.5f, 0.5f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Body", pGameObject)))
		return E_FAIL;
	m_mapLayer.emplace(Engine::GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CCustomize::Ready_UI_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;
	
	m_pUIMgr->CreateCustomizeUI(m_pGraphicDev, pLayer);
	m_mapLayer.emplace(Engine::UI, pLayer);
	return S_OK;
}

HRESULT CCustomize::Ready_Light()
{
	return S_OK;
}

CCustomize * CCustomize::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCustomize *	pInstance = new CCustomize(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCustomize::Free()
{	
	//m_pUIMgr->EraseMouse();
	Engine::CScene::Free();
	Engine::Delete_Light(1);
}
