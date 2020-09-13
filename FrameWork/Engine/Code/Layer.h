#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLayer();

public:
	CComponent*			Get_Component(const wstring& wstrObjTag, COMPONENTTYPE eComponentType, COMPONENTID eID, _int iIndex = 0);
	list<CGameObject*>	Get_GameObjectlist(const wstring& wstrObjTag);
	_int				Get_GameObjectlistSize(const wstring& wstrObjTag);
	CGameObject*		Get_GameObject(const wstring& wstrObjTag, _int iIndex = 0);

	HRESULT				Add_GameObject(const wstring& wstrObjTag, CGameObject* pInstance);

public:
	HRESULT				Ready_Layer();
	_int				Update_Layer(const _double& dTimeDelta);
	_int				LateUpdate_Layer(const _double& dTimeDelta);

private:
	LPDIRECT3DDEVICE9										m_pGraphicDev;

	typedef list<CGameObject*>								GAMEOBJECT_LIST;
	typedef map<wstring, GAMEOBJECT_LIST>					MAP_OBJECT;

	MAP_OBJECT												m_mapGameObjects;

public:
	static CLayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free();

public:
	MAP_OBJECT&			Get_mapGameObject() { return m_mapGameObjects; }
};

END
#endif // Layer_h__
