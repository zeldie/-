#include "Scene.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev),
	m_dTime(0),
	m_dwRenderCnt(0),
	m_bFade(false),
	m_bEnterScene(false),
	m_dEnterSceneTime(0)
{
	Safe_AddRef(m_pGraphicDev);
	ZeroMemory(m_szFPS, sizeof(_tchar) * 256);
}

Engine::CScene::~CScene()
{

}

CComponent* Engine::CScene::Get_Component(LAYERTYPE eLayerType, const wstring& wstrObjTag, COMPONENTTYPE eComponentType, COMPONENTID eID, _int iIndex)
{
	auto	iter = m_mapLayer.find(eLayerType);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(wstrObjTag, eComponentType, eID, iIndex);
}

list<CGameObject*> CScene::Get_GameObjectlist(LAYERTYPE eLayerType, const wstring & wstrObjTag)
{
	auto iter = m_mapLayer.find(eLayerType);
	return iter->second->Get_GameObjectlist(wstrObjTag);
}

CGameObject * CScene::Get_GameObject(LAYERTYPE eLayerType, const wstring & wstrObjTag, _int iIndex)
{
	auto	iter = m_mapLayer.find(eLayerType);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(wstrObjTag, iIndex);
}

HRESULT CScene::Add_GameObject(LAYERTYPE eLayerType, const wstring & wstrObjTag, CGameObject * pInstance)
{
	if (pInstance == nullptr)
		return E_FAIL;

	m_mapLayer[eLayerType]->Add_GameObject(wstrObjTag, pInstance);
	return S_OK;
}

HRESULT Engine::CScene::Ready_Scene()
{
	return S_OK;
}


_int Engine::CScene::Update_Scene(const _double& dTimeDelta)
{
	_int iExit = 0;

	for (auto& iter : m_mapLayer)
	{
		iExit = iter.second->Update_Layer(dTimeDelta);

		if (iExit & 0x80000000)
			return iExit;
	}

	return iExit;
}

_int CScene::LateUpdate_Scene(const _double & dTimeDelta)
{
	_int iExit = 0;

	for (auto& iter : m_mapLayer)
	{
		iExit = iter.second->LateUpdate_Layer(dTimeDelta);

		if (iExit & 0x80000000)
			return iExit;
	}

	return iExit;
}


void Engine::CScene::Free()
{
	for (auto& MyPair : m_mapLayer)
		Safe_Release(MyPair.second);
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}



