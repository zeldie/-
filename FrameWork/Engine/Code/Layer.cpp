#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CLayer::~CLayer()
{

}

CComponent* Engine::CLayer::Get_Component(const wstring& wstrObjTag, COMPONENTTYPE eComponentType, COMPONENTID eID, _int iIndex)
{
	auto iter_find = m_mapGameObjects.find(wstrObjTag);

	if (m_mapGameObjects.end() == iter_find)
		return nullptr;

	auto iter_begin = iter_find->second.begin();

	if (iter_find->second.end() == iter_begin)
		return nullptr;

	for (int i = 0; i < iIndex; ++i, ++iter_begin);

	return (*iter_begin)->Get_Component(eComponentType, eID);
}

list<CGameObject*> CLayer::Get_GameObjectlist(const wstring & wstrObjTag)
{
	auto iter_find = m_mapGameObjects.find(wstrObjTag);

	return iter_find->second;
}

_int CLayer::Get_GameObjectlistSize(const wstring & wstrObjTag)
{
	auto iter = m_mapGameObjects.find(wstrObjTag);

	if (iter == m_mapGameObjects.end())
		return 0;

	return _int(iter->second.size());
}

CGameObject * CLayer::Get_GameObject(const wstring & wstrObjTag, _int iIndex)
{
	auto iter_find = m_mapGameObjects.find(wstrObjTag);

	if (m_mapGameObjects.end() == iter_find)
		return nullptr;

	auto iter_begin = iter_find->second.begin();

	if (iter_find->second.end() == iter_begin)
		return nullptr;

	for (int i = 0; i < iIndex; ++i, ++iter_begin);

	return (*iter_begin);
}

HRESULT Engine::CLayer::Add_GameObject(const wstring& wstrObjTag, CGameObject* pInstance)
{
	if (nullptr == pInstance)
		return E_FAIL;

	m_mapGameObjects[wstrObjTag].emplace_back(pInstance);

	return  S_OK;
}

HRESULT Engine::CLayer::Ready_Layer()
{
	return S_OK;
}


_int Engine::CLayer::Update_Layer(const _double& dTimeDelta)
{
	_int iExit = 0;

	MAP_OBJECT::iterator iter_begin_map = m_mapGameObjects.begin();
	MAP_OBJECT::iterator iter_end_map = m_mapGameObjects.end();

	GAMEOBJECT_LIST::iterator iter_begin_list;
	GAMEOBJECT_LIST::iterator iter_end_list;

	for (; iter_begin_map != iter_end_map; ++iter_begin_map)
	{
		iter_begin_list = iter_begin_map->second.begin();
		iter_end_list = iter_begin_map->second.end();

		for (auto& iter = iter_begin_list; iter != iter_end_list;)
		{
			iExit = (*iter)->Update_GameObject(dTimeDelta);
			if (iExit == ERROR_EXIT)
				return iExit;
			else if (iExit == OBJ_DEAD)
			{
				Safe_Release(*iter);
				iter = iter_begin_map->second.erase(iter);
			}
			else if (iExit == OBJ_POOL)
			{
				iter = iter_begin_map->second.erase(iter);
			}
			else
				++iter;
		}
	}

	return iExit;
}

_int CLayer::LateUpdate_Layer(const _double & dTimeDelta)
{
	_int iExit = 0;

	MAP_OBJECT::iterator iter_begin_map = m_mapGameObjects.begin();
	MAP_OBJECT::iterator iter_end_map = m_mapGameObjects.end();

	GAMEOBJECT_LIST::iterator iter_begin_list;
	GAMEOBJECT_LIST::iterator iter_end_list;

	for (; iter_begin_map != iter_end_map; ++iter_begin_map)
	{
		iter_begin_list = iter_begin_map->second.begin();
		iter_end_list = iter_begin_map->second.end();

		for (auto& iter = iter_begin_list; iter != iter_end_list;)
		{
			iExit = (*iter)->LateUpdate_GameObject(dTimeDelta);
			if (iExit == ERROR_EXIT)
				return iExit;
			else if (iExit == OBJ_DEAD)
			{
				Safe_Release(*iter);
				iter = iter_begin_map->second.erase(iter);
			}
			else if (iExit == OBJ_POOL)
			{
				iter = iter_begin_map->second.erase(iter);
			}
			else
				++iter;
		}
	}

	return iExit;
}


CLayer* Engine::CLayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLayer*	pInstance = new CLayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for (auto& MyPair : m_mapGameObjects)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), CDeleteObj());
		MyPair.second.clear();
	}

	Safe_Release(m_pGraphicDev);
}



