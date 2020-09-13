#ifndef Management_h__
#define Management_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Scene.h"


BEGIN(Engine)

class CSubject;
class CObserver;
class CRenderer;
class ENGINE_DLL CManagement : public CBase 
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit	CManagement();
	virtual		~CManagement();

public:
	CComponent*			Get_Component(LAYERTYPE eLayerType, const wstring& wstrObjTag, COMPONENTTYPE eComponentType, COMPONENTID eID, _int iIndex = 0);
	CScene*				Get_Scene() { return m_pScene; }
	list<CGameObject*>	Get_GameObjectlist(LAYERTYPE eLayerType, const wstring& wstrObjTag);
	CGameObject*		Get_GameObject(LAYERTYPE eLayerType, const wstring& wstrObjTag, _int iIndex = 0);
	HRESULT				Add_GameObject(LAYERTYPE eLayerType, const wstring& wstrObjTag, CGameObject* pInstance);
	HRESULT				SetUp_Scene(CScene* pScene);
	void				ClearRenderer();
	//For Subject & Observer
	HRESULT				AddSubject(SUBJECTTYPE eSubjectType);
	void				Subscribe(SUBJECTTYPE eSubjectType, CObserver* pObserver);
	void				UnSubscribe(SUBJECTTYPE eSubjectType, CObserver* pObserver);
	void				Notify(SUBJECTTYPE eSubjectType, int iMessage, void* pData);
	void				ClearSubject();
public:
	HRESULT				Ready_Shaders(LPDIRECT3DDEVICE9& pGraphicDev);
	HRESULT				Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev);
	HRESULT				Ready_TextureMgr(LPDIRECT3DDEVICE9& pGraphicDev);
	_int				Update_Scene(const _double& dTimeDelta);
	_int				LateUpdate_Scene(const _double& dTimeDelta);
	void				Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev, const _double& dTimeDelta);

private:
	CScene*										m_pScene;
	CRenderer*									m_pRenderer;
	typedef map<SUBJECTTYPE, CSubject*>			MAP_SUBJECT;
	MAP_SUBJECT									m_Subjects;

public:
	virtual void	Free();

};

END
#endif // Management_h__
