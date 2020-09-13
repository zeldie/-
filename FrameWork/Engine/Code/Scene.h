#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CComponent*			Get_Component(LAYERTYPE eLayerType, const wstring& wstrObjTag, COMPONENTTYPE eComponentType, COMPONENTID eID, _int iIndex = 0);
	list<CGameObject*>	Get_GameObjectlist(LAYERTYPE eLayerType, const wstring& wstrObjTag);
	Engine::CLayer*		Get_Layer(LAYERTYPE eLayerType) { return m_mapLayer[eLayerType]; }
	CGameObject*		Get_GameObject(LAYERTYPE eLayerType, const wstring& wstrObjTag, _int iIndex = 0);

	virtual HRESULT		Add_GameObject(LAYERTYPE eLayerType, const wstring& wstrObjTag, CGameObject* pInstance);
public:
	virtual HRESULT		Ready_Scene();
	virtual _int		Update_Scene(const _double& dTimeDelta);
	virtual _int		LateUpdate_Scene(const _double& dTimeDelta);
	virtual void		Render_Scene()PURE;

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	map<LAYERTYPE, CLayer*>		m_mapLayer;
	_tchar						m_szFPS[MAX_STR];
	_double						m_dTime;
	_ulong						m_dwRenderCnt;
	_bool						m_bFade;
	_bool						m_bEnterScene;
	_double						m_dEnterSceneTime;
public:
	virtual			void Free();

};


END
#endif // Scene_h__
