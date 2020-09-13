#pragma once

#include "Scene.h"

BEGIN(Engine)
END
class CTerrain;
class CMapScene : public Engine::CScene
{
private:
	explicit CMapScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapScene();


public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _double& dTimeDelta) override;
	virtual _int LateUpdate_Scene(const _double& dTimeDelta) override;
	virtual void Render_Scene() override;


public:
	void SceneClear();	
	void LayerClear(Engine::LAYERTYPE eLayerType);
	
private:
	HRESULT Ready_Component();
	HRESULT	Ready_Priority_Layer();
	HRESULT	Ready_GameObjct_Layer();
	HRESULT	Ready_Environment_Layer();
	HRESULT	Ready_StaticMesh_Layer();
	HRESULT	Ready_DynamicMesh_Layer();
	HRESULT	Ready_NaviMesh_Layer();
	HRESULT	Ready_Light();
public:
	CTerrain*			m_pTerrain;
	list<IMAGEPATH*>	m_listImagePath;

public:
	static CMapScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;

};

