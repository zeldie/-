#include "stdafx.h"
#include "Loading.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
	, m_hThread(nullptr)
	, m_pEffectMgr(CEffectMgr::GetInstance())
	, m_pLoadingMgr(CLoadingMgr::GetInstance())
{
	Engine::Safe_AddRef(m_pGraphicDev);
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
}

CLoading::~CLoading(void)
{

}

unsigned int CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_FIRST:
		iFlag = pLoading->Loading_First();
		break;
	case LOADING_LOBBY:
		break;
	case LOADING_BOSSSTAGE:
		break;
	case LOADING_2to2:
		break;
	case LOADING_SHOOTINGSTAGE:
		break;
	case LOADING_RUNGAME:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoading = eLoadingID;

	return S_OK;
}

_uint CLoading::Loading_First()
{
	lstrcpy(m_szLoading, L"-------Texture Loading-------");

	// 터짐방지
	Load_Texture();

	lstrcpy(m_szLoading, L"-------Mesh Loading-------");

	Load_Mesh();

	// Test Effect Load
	Load_EffectMesh();

	lstrcpy(m_szLoading, L"-------Loading Complete-------");
	
	// 로딩끝 확인용
	CSoundMgr::Get_Instance()->SoundOn(0);

	m_pEffectMgr->Ready_ObjPool();
	m_bFinish = true;

	return Engine::NO_EVENT;
}

void CLoading::Load_Texture()
{
	if (FAILED(Engine::LoadTexture(m_pGraphicDev, L"../../Data/Texture/Texture.txt")))
		return;
	return;
}

void CLoading::Load_Mesh()
{
	if (FAILED(Engine::LoadMesh(m_pGraphicDev, L"../../Data/Mesh/Mesh_Dynamic.txt")))
		return;

	if (FAILED(Engine::LoadMesh(m_pGraphicDev, L"../../Data/Mesh/Mesh_Lobby.txt")))
		return;
	if (FAILED(Engine::LoadMesh(m_pGraphicDev, L"../../Data/Mesh/Mesh_Cartel.txt")))
		return;
	if (FAILED(Engine::LoadMesh(m_pGraphicDev, L"../../Data/Mesh/Mesh_Boss.txt")))
		return;
	if (FAILED(Engine::LoadMesh(m_pGraphicDev, L"../../Data/Mesh/Mesh_Run.txt")))
		return;

	if (FAILED(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Mesh_Navi_Lobby", Engine::TYPE_NAVI, L"", L"Mesh_Navi_Lobby")))
		return;
	if (FAILED(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Mesh_Navi_Boss", Engine::TYPE_NAVI, L"", L"Mesh_Navi_Boss")))
		return;
	if (FAILED(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Mesh_Navi_Cartel", Engine::TYPE_NAVI, L"", L"Mesh_Navi_Cartel")))
		return;
	if (FAILED(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Mesh_Navi_Apostle", Engine::TYPE_NAVI, L"", L"Mesh_Navi_Apostle")))
		return;
	if (FAILED(Engine::Ready_Meshes(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Mesh_Navi_Apostle1", Engine::TYPE_NAVI_RUN, L"", L"Mesh_Navi_Apostle1")))
		return;

	return;
}

void CLoading::Load_EffectMesh()
{
	if (FAILED(Engine::LoadMesh(m_pGraphicDev, L"../../Data/Mesh/Mesh_Effect_Dynamic.txt")))
		return;
	if (FAILED(Engine::LoadMesh(m_pGraphicDev, L"../../Data/Mesh/Mesh_Effect_Static.txt")))
		return;

}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoading)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Engine::Safe_Release(m_pGraphicDev);
}

