#include "stdafx.h"
#include "MainApp.h"
#include "Logo.h"

#include "Transform.h"
#include "Calculator.h"
#include "Optimization.h"
#include "LightCamera.h"
_bool g_RenderCollider = false;
_bool g_RenderNaviMesh = false;
//Test
//얘는 카메라 매니저 나오면 거기에다가 넣을거임
CLightCamera*			m_pLightCamera = nullptr;
//아래 이 코드는 클라에서 실시간으로 값을 확인할려고 씀, 값을 늘리는방법은 아래 Update에 KeyPressing 으로 만듬
_vec3 vTest = _vec3(1.f, 1.f, 1.f);
_double temp = 0;
float temp2 = 300.f;
CMainApp::CMainApp()
	:m_pDeviceClass(nullptr),
	m_pManagement(nullptr),
	m_pGraphicDev(nullptr),
	m_pTimeMgr(Engine::CTimerMgr::GetInstance()),
	m_pEffectMgr(CEffectMgr::GetInstance()),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_pCollisionMgr(CCollisionMgr::GetInstance()),
	m_pCamMgr(CCameraMgr::GetInstance()),
	m_pMatchingCardMgr(CMatchingMgr::GetInstance()),
	m_pPuzzleMgr(CPuzzleMgr::GetInstance()),
	m_pRunGameMgr(CRunGameMgr::GetInstance()),
	m_pLoadingMgr(CLoadingMgr::GetInstance()),
	m_pQuestMgr(CQuestMgr::GetInstance()),
	m_pSoundMgr(CSoundMgr::Get_Instance())
{

}

CMainApp::~CMainApp()
{

}

HRESULT CMainApp::Ready_MainApp()
{
	m_pSoundMgr->Initialize();

	if (FAILED(SetUp_DefaultSetting(&m_pGraphicDev)))
		return E_FAIL;
	if (FAILED(Ready_BasicComponent()))
		return E_FAIL;
	if (FAILED(Ready_Scene(m_pGraphicDev, &m_pManagement)))
		return E_FAIL;

	if (FAILED(Engine::Ready_InputDev(g_hInst, g_hWnd)))
		return E_FAIL;
	
	// Font
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"맑은 고딕", 15, 20, FW_HEAVY)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_GODO", L"고도B", 10, 10, FW_MEDIUM)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_GODIC", L"맑은 고딕", 5, 15, FW_BOLD)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_GODIC2", L"맑은 고딕", 7, 20, FW_BOLD)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_GODIC3", L"맑은 고딕", 15, 25, FW_BOLD)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_GODICHEAVY", L"맑은 고딕", 10, 30, FW_HEAVY)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_GODICHEAVY2", L"맑은 고딕", 30, 50, FW_HEAVY)))
		return E_FAIL;


	srand((_uint)time(NULL));	

	// Mgr
	if (FAILED(m_pEffectMgr->Set_Device(m_pGraphicDev)))
		return E_FAIL;
	if (FAILED(m_pMatchingCardMgr->Set_Device(m_pGraphicDev)))
		return E_FAIL;
	if (FAILED(m_pPuzzleMgr->Set_Device(m_pGraphicDev)))
		return E_FAIL;
	if (FAILED(m_pRunGameMgr->Set_Device(m_pGraphicDev)))
		return E_FAIL;
	if (FAILED(m_pLoadingMgr->Ready_LoadingMgr(m_pGraphicDev)))
		return E_FAIL;
	
	m_pCamMgr->Set_RendererCom();

	//m_pEffectMgr->Ready_ObjPool();

	//라이트 카메라, 추후 카메라 매니저로 이동
	m_pLightCamera = CLightCamera::Create(m_pGraphicDev, &_vec3(0.f, 5000.f, 3000.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(45.f), _float(WINCX) / WINCY, 0.7f, 7000.f);
	return S_OK;
}

_int CMainApp::Update_MainApp(const _double& dTimeDelta)
{
	if (m_pManagement == nullptr)
		return Engine::ERROR_EXIT;
	ShowCursor(FALSE);
	g_dAccumulatedTime += dTimeDelta;

	Engine::Set_InputDev();

	////희정 UI TEST ->위치 조절 
	//if(Engine::KeyPressing(DIK_RCONTROL))
	//	m_pUIMgr->KeyInput(dTimeDelta);

	//////////////////TestCode/////////////////////////////////
	if (Engine::KeyDown(DIK_F2))
	{
		if (g_RenderCollider)
			g_RenderCollider = false;
		else
			g_RenderCollider = true;
	}

	if (Engine::KeyDown(DIK_F3))
	{
		if (g_RenderNaviMesh)
			g_RenderNaviMesh = false;
		else
			g_RenderNaviMesh = true;
	}
	if (Engine::KeyPressing(DIK_X))
	{
		if (Engine::KeyDown(DIK_5))
		{
			temp += 0.00000005;
			//cout << temp << endl;
		}
		if (Engine::KeyDown(DIK_6))
		{
			temp -= 0.00000005;
			//cout << temp << endl;
		}
		if (Engine::KeyPressing(DIK_7))
		{
			vTest.y += dTimeDelta;
		}
		if (Engine::KeyPressing(DIK_8))
		{
			vTest.y -= dTimeDelta;
		}
		if (Engine::KeyPressing(DIK_9))
		{
			vTest.z += dTimeDelta;
		}
		if (Engine::KeyPressing(DIK_0))
		{
			vTest.z -= dTimeDelta;
		}
	}
	
//	if (Engine::KeyPressing(DIK_F7))
//	{
//	temp += 50.f*dTimeDelta;
//	}
//	if (Engine::KeyPressing(DIK_F8))
//	{
//	temp -= 50.f*dTimeDelta;
//	}
//
//if (Engine::KeyPressing(DIK_F9))
//{
//	temp2 += 50.f*dTimeDelta;
//}
//if (Engine::KeyPressing(DIK_F10))
//{
//	temp2 -= 50.f*dTimeDelta;
//}
	////cout << " X : " << vTest.x << " Y : " << vTest.y << " Z : " << vTest.z << endl;
////cout << " temp : " << temp << " temp2 : " << temp2  << endl;
	
	/////////////////TestCode/////////////////////////////////
	_int iMsg = m_pManagement->Update_Scene(dTimeDelta);

	//m_pCamMgr->Update_Camera(dTimeDelta);


	return iMsg;
}

_int CMainApp::LateUpdate_MainApp(const _double & dTimeDelta)
{
	if (m_pManagement == nullptr)
		return Engine::ERROR_EXIT;

	
	_int iMsg = m_pManagement->LateUpdate_Scene(dTimeDelta);
	m_pCollisionMgr->Update_Collision();
	//m_pCamMgr->LateUpdate_Camera(dTimeDelta);

	return iMsg;
}

void CMainApp::Render_MainApp(const _double& dTimeDelta)
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	if (m_pManagement == nullptr)
		return;

	m_pManagement->Render_Scene(m_pGraphicDev, dTimeDelta);

	Engine::Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	if (FAILED(Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
		return E_FAIL;

	*ppGraphicDev = m_pDeviceClass->GetDevice();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 & pGraphicDev, Engine::CManagement ** ppManagementClass)
{
	Engine::CScene*		pScene = nullptr;
	if (FAILED(Engine::Create_Management(pGraphicDev, ppManagementClass)))
		return E_FAIL;
	(*ppManagementClass)->AddRef();
	
	pScene = CLogo::Create(pGraphicDev);
	if (pScene == nullptr)
		return E_FAIL;

	if (FAILED((*ppManagementClass)->SetUp_Scene(pScene)))
		return E_FAIL;

	if (FAILED(Ready_BasicLight()))
		return E_FAIL;
	return S_OK;
}

HRESULT CMainApp::Ready_BasicComponent()
{

	Engine::Reserve_ContainerSize(Engine::RESOURCE_END);
	//Buffer
	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_TriCol", Engine::BUFFER_TRICOL)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_CubeCol", Engine::BUFFER_CUBECOL)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_ParticleTex", Engine::BUFFER_PARTICLETEX)))
		return E_FAIL;

	//if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_Dynamic", Engine::BUFFER_DYNAMIC, 50)))
	//	return E_FAIL;

	//Background Texture
	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_BackGround", Engine::TEX_NORMAL, L"../../Resource/BasicTexture/BackGround/BackGround.tga", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Decal_SkillTarget", Engine::TEX_CUBE, L"../../Resource/Cube/SkillTarget.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Decal_Decal", Engine::TEX_CUBE, L"../../Resource/Cube/Decal.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Decal_OrbF", Engine::TEX_CUBE, L"../../Resource/Cube/Orb_F.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Decal_OrbR", Engine::TEX_CUBE, L"../../Resource/Cube/Orb_R.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Decal_TS_F", Engine::TEX_CUBE, L"../../Resource/Cube/TS_F.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Decal_Bow_E_Fall", Engine::TEX_CUBE, L"../../Resource/Cube/Bow_E_Fall.dds", 1)))
		return E_FAIL;

	////Component
	Engine::CComponent* pComponent = nullptr;

	pComponent = Engine::CTransform::Create(m_pGraphicDev);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Engine::Add_Component(pComponent, L"TransformCom", Engine::RESOURCE_STATIC)))
		return E_FAIL;

	pComponent = Engine::CCalculator::Create(m_pGraphicDev);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Engine::Add_Component(pComponent, L"CalculatorCom", Engine::RESOURCE_STATIC)))
		return E_FAIL;

	pComponent = Engine::COptimization::Create(m_pGraphicDev, Engine::ENABLE, 129, 129);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Engine::Add_Component(pComponent, L"OptimizationCom", Engine::RESOURCE_STATIC)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_BasicLight()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tLightInfo.Direction = _vec3(0.f, -1.f, 1.f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
		return E_FAIL;

	/*tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	tLightInfo.Direction = _vec3(0.f, -1.f, -1.f);
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1)))
		return E_FAIL;*/

	/*tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tLightInfo.Position = _vec3(0.f, 400, 0.f);
	tLightInfo.Range = 3000.f;
	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
		return E_FAIL; */
	return S_OK;
}


CMainApp* CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMainApp::Free()
{	
	m_pQuestMgr->DestroyInstance();
	m_pRunGameMgr->DestroyInstance();
	m_pMatchingCardMgr->DestroyInstance();
	m_pPuzzleMgr->DestroyInstance();
	m_pEffectMgr->DestroyInstance();
	m_pUIMgr->DestroyInstance();
	m_pCollisionMgr->DestroyInstance();
	m_pCamMgr->DestroyInstance();
	m_pSoundMgr->Destroy_Instance();

	m_pLoadingMgr->DestroyInstance();
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pLightCamera); 	//카메라 매니저에가면 지울거임
	Engine::Release_Engine();
	
}

