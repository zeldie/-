#ifndef Loading_h__
#define Loading_h__

class CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	LOADINGID			Get_LoadingID() const { return m_eLoading; }
	CRITICAL_SECTION*	Get_Crt() { return &m_Crt; }
	_bool				Get_Finish() const { return m_bFinish; }
	const _tchar*		Get_String() const { return m_szLoading; }
	_bool				Get_Loading() { return m_bLoading; }
	_bool				Get_Complete() { return m_bComplete; }

public:
	static unsigned int CALLBACK	Thread_Main(void* pArg);

public:
	HRESULT					Ready_Loading(LOADINGID eLoadingID);
	_uint					Loading_First();
public:
	void					Load_Texture();
	void					Load_Mesh();
	void					Load_EffectMesh();
private:
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_Crt;
	LOADINGID				m_eLoading;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bFinish;
	_tchar					m_szLoading[256];
	CEffectMgr*				m_pEffectMgr;
	CLoadingMgr*			m_pLoadingMgr;

	_bool					m_bLoading;
	_bool					m_bComplete;

public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading);
private:
	virtual void			Free();

};

#endif // Loading_h__
