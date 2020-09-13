#include "stdafx.h"
#include "MapScene.h"

#include "Calculator.h"

#include "DynamicCamera.h"
#include "Terrain.h"

#include "FileInfo.h"

#include "SkySphere.h"
//용호 테스트 할떄 사용
//#define CAMTOOL
/////
CMapScene*	g_pMapScene;
CMapScene::CMapScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev),
	m_pTerrain(nullptr)
{
}


CMapScene::~CMapScene()
{
	for_each(m_listImagePath.begin(), m_listImagePath.end(), Engine::Safe_Delete<IMAGEPATH*>);
	m_listImagePath.clear();
}

HRESULT CMapScene::Ready_Scene(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	if (FAILED(Ready_Priority_Layer()))
		return E_FAIL;
	if (FAILED(Ready_GameObjct_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_StaticMesh_Layer()))
		return E_FAIL;
	if (FAILED(Ready_DynamicMesh_Layer()))
		return E_FAIL;
	if (FAILED(Ready_NaviMesh_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Light()))
		return E_FAIL;
	g_pMapScene = this;
	return S_OK;
}


_int CMapScene::Update_Scene(const _double& dTimeDelta)
{
	Engine::CScene::Update_Scene(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CMapScene::LateUpdate_Scene(const _double& dTimeDelta)
{
	Engine::CScene::LateUpdate_Scene(dTimeDelta);
	return Engine::NO_EVENT;
}

void CMapScene::Render_Scene(void)
{

}

void CMapScene::SceneClear()
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), Engine::CDeleteMap());
	m_mapLayer.clear();

	if (FAILED(Ready_Priority_Layer()))
		return;
	if (FAILED(Ready_Environment_Layer()))
		return;
	if (FAILED(Ready_GameObjct_Layer()))
		return;
	if (FAILED(Ready_StaticMesh_Layer()))
		return;
	if (FAILED(Ready_DynamicMesh_Layer()))
		return;
	if (FAILED(Ready_NaviMesh_Layer()))
		return;
	if (FAILED(Ready_Light()))
		return;
}

void CMapScene::LayerClear(Engine::LAYERTYPE eLayerType)
{
	/*auto iter = m_mapLayer.find(eLayerType);
	if (iter == m_mapLayer.end())
		return;
	iter->second->LayerClear(eLayerType);*/
}


HRESULT CMapScene::Ready_Component()
{
#ifndef CAMTOOL
	CFileInfo::DirInfoExtractionForMeshTool(L"../../Resource/Mesh", m_listImagePath);
	CFileInfo::DirInfoExtractionForEffectTool(L"../../Resource/Texture", m_listImagePath);

	list<IMAGEPATH*>::iterator iter_begin = m_listImagePath.begin();
	list<IMAGEPATH*>::iterator iter_end = m_listImagePath.end();

	for (; iter_begin != iter_end; ++iter_begin)
	{
		if (!lstrcmp(L"StaticMesh", (*iter_begin)->wstrMeshType.c_str()))
		{
			if (FAILED((Engine::Ready_Meshes(m_pGraphicDev,
				Engine::RESOURCE_STATIC,
				L"Mesh_" + (*iter_begin)->wstrFileName,
				Engine::TYPE_STATIC,
				L"../../Resource/Mesh/StaticMesh/" + (*iter_begin)->wstrTextureName + L"/" + (*iter_begin)->wstrFileName + L"/",////*texturename -> 스테이지별 폴더구분용*/
				(*iter_begin)->wstrFileName + L".X"))))
				AfxMessageBox(L"MeshTool - StaticMesh Load Failed");

		}
		else if (!lstrcmp(L"DynamicMesh", (*iter_begin)->wstrMeshType.c_str()))
		{
			if (FAILED((Engine::Ready_Meshes(m_pGraphicDev,
				Engine::RESOURCE_STATIC,
				L"Mesh_" + (*iter_begin)->wstrFileName,
				Engine::TYPE_DYNAMIC,
				L"../../Resource/Mesh/DynamicMesh/" + (*iter_begin)->wstrTextureName + L"/" + (*iter_begin)->wstrFileName + L"/",////*texturename -> 스테이지별 폴더구분용*/
				(*iter_begin)->wstrFileName + L".X"))))
				AfxMessageBox(L"MeshTool - DynamicMesh Load Failed");
		}
		else if (!lstrcmp(L"EffectALL", (*iter_begin)->wstrTextureName.c_str()))
		{
			if (FAILED((Engine::Ready_Texture(m_pGraphicDev,
				Engine::RESOURCE_STATIC,
				L"Texture_" + (*iter_begin)->wstrTextureName,
				Engine::TEX_NORMAL,
				L"../../Resource/Texture/Effect/EffectALL/" + (*iter_begin)->wstrTextureName + L"%d.tga",
				(*iter_begin)->iCount))))
				AfxMessageBox(L"MeshTool -Texture - Effect  Load Failed"); //수정해야하는데 귀찮다 -> 노디가고쳐서 씀 워닝떠서!!!!
		}
	}
#endif // CAMTOOL


#ifdef CAMTOOL
	////////////////////////용호///////////////////////
	if (FAILED((Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		L"Mesh_SoB_BaseFloor01",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Boss/SoB_BaseFloor01/",// //*texturename -> 스테이지별 폴더구분용*/
		L"SoB_BaseFloor01.X"))))
		AfxMessageBox(L"mapscene-ready failed");
	 ///////////////////////////////////////////////////

#endif // CAMTOOL
	return S_OK;
}

HRESULT CMapScene::Ready_Priority_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	m_mapLayer.emplace(Engine::PRIORITY, pLayer);

	return S_OK;
}

HRESULT CMapScene::Ready_GameObjct_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	m_mapLayer.emplace(Engine::GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CMapScene::Ready_Environment_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 10.f, -10.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(45.f),
		_float(WINCX) / WINCY,
		0.1f,
		10000.f);
	if (FAILED(pLayer->Add_GameObject(L"DynamicCamera", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrain::Create(m_pGraphicDev);
	if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
		return E_FAIL;
	m_pTerrain = dynamic_cast<CTerrain*>(pGameObject);

	pGameObject = CSkySphere::Create(m_pGraphicDev, CSkySphere::SKY_SHOOTINGSTAGE);
	if (FAILED(pLayer->Add_GameObject(L"ZSky", pGameObject)))
		return E_FAIL;
	m_mapLayer.emplace(Engine::ENVIRONMENT, pLayer);

	return S_OK;
}

HRESULT CMapScene::Ready_StaticMesh_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	

	m_mapLayer.emplace(Engine::STATICMESH, pLayer);

	return S_OK;
}

HRESULT CMapScene::Ready_DynamicMesh_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	m_mapLayer.emplace(Engine::DYNAMICMESH, pLayer);

	return S_OK;
}

HRESULT CMapScene::Ready_NaviMesh_Layer()
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	if (pLayer == nullptr)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	m_mapLayer.emplace(Engine::NAVIMESH, pLayer);

	return S_OK;
}

HRESULT CMapScene::Ready_Light()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	tLightInfo.Direction = _vec3(0.f, -1.f, 1.f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
		return E_FAIL;
	return S_OK;
}


CMapScene * CMapScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMapScene *pInstance = new CMapScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMapScene::Free(void)
{
	Engine::CScene::Free();
}
