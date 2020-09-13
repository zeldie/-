#ifndef MainApp_h__
#define MainApp_h__

BEGIN(Engine)

class CGraphicDev;
class CManagement;
END
class CEffectMgr;
class CUIMgr;
class CCollisionMgr;
class CLoadingMgr;
class CAiMgr;
class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT		Ready_MainApp();
	_int		Update_MainApp(const _double& dTimeDelta);
	_int		LateUpdate_MainApp(const _double& dTimeDelta);
	void		Render_MainApp(const _double& dTimeDelta);

private:
	HRESULT		SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT		Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, Engine::CManagement** ppManagementClass);
	HRESULT		Ready_BasicComponent();
	HRESULT		Ready_BasicLight();
private:
	Engine::CGraphicDev*	m_pDeviceClass;
	Engine::CManagement*	m_pManagement;
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	// Mgr
	Engine::CTimerMgr*		m_pTimeMgr;
	CEffectMgr*				m_pEffectMgr;
	CUIMgr*					m_pUIMgr;
	CCollisionMgr*			m_pCollisionMgr;
	CCameraMgr*				m_pCamMgr;
	CMatchingMgr*			m_pMatchingCardMgr;
	CPuzzleMgr*				m_pPuzzleMgr;
	CRunGameMgr*			m_pRunGameMgr;
	CLoadingMgr*			m_pLoadingMgr;
	CAiMgr*					m_pAiMgr;
	CQuestMgr*				m_pQuestMgr;
	CSoundMgr*				m_pSoundMgr;

public:
	static CMainApp*		Create();
private:
	virtual void			Free();
};

#endif // MainApp_h__
