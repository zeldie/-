#include "GraphicDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDev)

Engine::CGraphicDev::CGraphicDev()
	:m_pSDK(nullptr),
	m_pGraphicDev(nullptr),
	m_pLine(nullptr)
{
	
}

Engine::CGraphicDev::~CGraphicDev()
{
	Free();
}

inline void CGraphicDev::Get_CamView(_matrix * matView)
{
	D3DXMatrixIdentity(matView);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, matView);
}

inline void CGraphicDev::Get_CamProj(_matrix * matProj)
{
	D3DXMatrixIdentity(matProj);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, matProj);
}

HRESULT Engine::CGraphicDev::Ready_GraphicDev(HWND hWnd,
											WINMODE eMode, 
											const _uint& iSizeX, 
											const _uint& iSizeY, 
											Engine::CGraphicDev** ppGraphicDev)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	
	if(nullptr == m_pSDK)
		return E_FAIL;

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if(FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
		return E_FAIL;

	/*BOOL    m_bDepthBiasCap;
	_float g_fSlopeScaleDepthBias = 1.0f;
	_float g_fDepthBias = -0.0005f;
	_float g_fDefaultDepthBias = 0.0f;*/

	
	_ulong		uFlag = 0;

	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		uFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		uFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;


	D3DPRESENT_PARAMETERS			Present_Parameter;
	ZeroMemory(&Present_Parameter, sizeof(D3DPRESENT_PARAMETERS));

	Present_Parameter.BackBufferWidth = iSizeX;
	Present_Parameter.BackBufferHeight = iSizeY;
	Present_Parameter.BackBufferFormat = D3DFMT_A8R8G8B8;
	Present_Parameter.BackBufferCount = 1;

	Present_Parameter.MultiSampleType = D3DMULTISAMPLE_NONE;
	Present_Parameter.MultiSampleQuality = 0;

	Present_Parameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Present_Parameter.hDeviceWindow = hWnd;

	Present_Parameter.Windowed = eMode;
	Present_Parameter.EnableAutoDepthStencil = TRUE;
	Present_Parameter.AutoDepthStencilFormat = D3DFMT_D24S8;

	Present_Parameter.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	Present_Parameter.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if(FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, uFlag, &Present_Parameter, &m_pGraphicDev)))
		return E_FAIL;
	if (FAILED(D3DXCreateLine(m_pGraphicDev, &m_pLine)))
		return E_FAIL;

	/*if ((DeviceCaps->RasterCaps) && (DeviceCaps->RasterCaps & D3DPRASTERCAPS_DEPTHBIAS))
	{
		m_bDepthBiasCap = true;
	}

	if (m_bDepthBiasCap)
	{
		m_pGraphicDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, F2DW(g_fSlopeScaleDepthBias));
		m_pGraphicDev->SetRenderState(D3DRS_DEPTHBIAS, F2DW(g_fDepthBias));
	}*/


	*ppGraphicDev = this;

	return S_OK;
}

void Engine::CGraphicDev::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0, 
		nullptr, 
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		Color, 
		1.f, 
		0);

	m_pGraphicDev->BeginScene();
}

void Engine::CGraphicDev::Render_End(void)
{
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);
}

void Engine::CGraphicDev::Free()
{
	_ulong dwRefCnt = 0;

	if (dwRefCnt = Engine::Safe_Release(m_pLine))
		MSG_BOX("m_pLine Release Failed");

	if(dwRefCnt = Engine::Safe_Release(m_pGraphicDev))
		MSG_BOX("m_pGraphicDev Release Failed");

	if(dwRefCnt = Engine::Safe_Release(m_pSDK))
		MSG_BOX("m_pSDK Release Failed");
}

