#include "stdafx.h"
#include "Ending.h"
#include "BackGround_Ending.h"
#include "Ending_Credit.h"
CEnding::CEnding(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev)
{
}


CEnding::~CEnding()
{
}

HRESULT CEnding::Ready_Scene()
{
	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_GameObject_Layer()))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;

	

	return S_OK;
}

_int CEnding::Update_Scene(const _double & dTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(dTimeDelta);


	return iExit;
}

_int CEnding::LateUpdate_Scene(const _double & dTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(dTimeDelta);
	return iExit;
}

void CEnding::Render_Scene()
{
	// Engine::Render_Font(L"Font_Default", m_pLoading->Get_String(), &_vec2(20.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}


HRESULT CEnding::Ready_Environment_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	m_mapLayer.emplace(Engine::ENVIRONMENT, pLayer);

	return S_OK;
}

HRESULT CEnding::Ready_GameObject_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CEnding_Credit::Create(m_pGraphicDev, WINCX >> 1, (WINCY >> 1) + 1500);
	if (FAILED(pLayer->Add_GameObject(L"Ending_Credit", pGameObject)))
		return E_FAIL;

	m_mapLayer.emplace(Engine::GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CEnding::Ready_UI_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CBackGround_Ending::Create(m_pGraphicDev);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"BackGround_Ending", pGameObject)))
		return E_FAIL;

	m_mapLayer.emplace(Engine::UI, pLayer);

	return S_OK;
}


CEnding * CEnding::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEnding *	pInstance = new CEnding(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}


void CEnding::Free()
{


	Engine::CScene::Free();
}
