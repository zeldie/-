#include "Export_Engine.h"
#include "Shader.h"

USING(Engine)

Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev),
	m_pVB(nullptr),
	m_pIB(nullptr),
	m_bDrawRenderTarget(false),
	m_fBrightThreshold(1.5f),
	m_fGaussMultiplier(0.4f),
	m_fGaussMean(0.f),
	m_fGaussStdDev(3.f),
	m_fExposure(1.f),
	m_fRadius(1.f),
	m_bFilterMonoChrome(false),
	m_vLightPosition(0.f, 0.f),
	m_bStartFade(false),
	m_bEndFade(false),
	m_vFadeColor(0.f, 0.f, 0.f, 0.f),
	m_iRenderNonAlpha(0),
	m_iRenderAlpha(0),
	m_iRenderUI(0),
	m_bUseRadialBlur(false),
	m_pOriginal_DS_Surface(nullptr),
	m_pShadow_DS_Surface(nullptr),
	m_fGamma(2.2f)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CRenderer::~CRenderer()
{
	Free();
}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if (RENDER_END <= eGroup)
		return;

	m_RenderGroup[eGroup].push_back(pGameObject);
	Safe_AddRef(pGameObject);					// RefCnt 증가
}

void Engine::CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev, const _double& dTimeDelta)
{
	/*m_iRenderNonAlpha = m_RenderGroup[RENDER_NONALPHA].size();
	m_iRenderAlpha = m_RenderGroup[RENDER_ALPHA].size();
	m_iRenderUI = m_RenderGroup[RENDER_UI].size();*/
	Render_Shadow(pGraphicDev, dTimeDelta);	// 툴작업할때 주석걸어야함, 그림자타겟에 그림자 그림 ->FIX
	Render_Depth(pGraphicDev, dTimeDelta);	// 툴작업할때 주석걸어야함, SSD을 위해 지형들에게뎁스를 미리 넣어줌 ->FIX
	Render_Priority(pGraphicDev, dTimeDelta);	// 툴작업할때 주석걸어야함, 딱히 안쓰는듯 -> FIX
	Render_Geometry(pGraphicDev, dTimeDelta);   // 일반적인 렌더 ->FIX
	Render_Cartoon(pGraphicDev, dTimeDelta);	// 툴작업할때 주석걸어야함, //Outline을 그리기위해서 노말이랑, 오클루션서브넣어줌(갓레이) - >FIX
	Render_Edge(pGraphicDev);					// 툴작업할때 주석걸어야함,  OutlineReady로 엣지만들어줌 - >FIX
	Render_Occlusion(pGraphicDev);			// 툴작업할때 주석걸어야함, //Occlusion만듬 - >FIX
	Render_HDR_Alpha(pGraphicDev, dTimeDelta);			//HDR에 들어갈 알파들 그림
	Render_LightAcc(pGraphicDev);			// 빛 그림 - >FIX

	Render_LightShaft(pGraphicDev);			// 툴작업할때 주석걸어야함, //오클루션 서브로 갓레이 만듬 - >FIX

	Render_HDRBase(pGraphicDev);			//빛을 포함한 HDR BASE
	Render_Luminance(pGraphicDev);			// 툴작업할때 주석걸어야함 , 휘도 만듬
	Render_HDR(pGraphicDev);					// 툴작업할때 주석걸어야함 , HDR로직
	Render_RadialBlur(pGraphicDev);
	//Render_DOF(pGraphicDev);					// 툴작업할때 주석걸어야함 , DOF 만듬 ->FIX
	
	Render_Final(pGraphicDev);				// 툴작업할때 주석걸어야함 , 위의 것들을 다 합침
	Render_Alpha(pGraphicDev, dTimeDelta);
	

	Render_UI(pGraphicDev, dTimeDelta);

	Render_Fade(pGraphicDev, dTimeDelta); // 화면 넘어갈때 ->FIX
	if (m_bDrawRenderTarget)
	{
		//Engine::Render_RenderTarget(Engine::PRE_DEPTH);
		Engine::Render_RenderTarget(Engine::GEOMETRY); // List
		Engine::Render_RenderTarget(Engine::SHADE);
		Engine::Render_RenderTarget(Engine::HDR_BASE);
		//Engine::Render_RenderTarget(Engine::LUMINANCE1);
		//Engine::Render_RenderTarget(Engine::LUMINANCE2);
		//Engine::Render_RenderTarget(Engine::LUMINANCE3);
		//Engine::Render_RenderTarget(Engine::LUMINANCE4);
		//Engine::Render_RenderTarget(Engine::LUMINANCE5);
		//Engine::Render_RenderTarget(Engine::LUMINANCE6);
		Engine::Render_RenderTarget(Engine::BRIGHTPASS);
		//Engine::Render_RenderTarget(Engine::BRIGHTPASSDOWNSAMPLE);
		//Engine::Render_RenderTarget(Engine::HORIZONTALBLUR);
		//Engine::Render_RenderTarget(Engine::VERTICALBLUR);
		//Engine::Render_RenderTarget(Engine::CARTOON); //List
		Engine::Render_RenderTarget(Engine::OUTLINE);
		//Engine::Render_RenderTarget(Engine::OCCLUSION);
		//Engine::Render_RenderTarget(Engine::OCCLUSION_HORIZONTALBLUR);

		Engine::Render_RenderTarget(Engine::OCCLUSION_VERTICALBLUR);
		Engine::Render_RenderTarget(Engine::SHADOW_PLAYER); // List
		//DOF
		//Engine::Render_RenderTarget(Engine::HDRFINISH);
		//Engine::Render_RenderTarget(Engine::DOFDOWNSAMPLE);
		//Engine::Render_RenderTarget(Engine::DOFHORIZONTALBLUR);
		Engine::Render_RenderTarget(Engine::DOFVERTICALBLUR);

		//LightShaft
		//Engine::Render_RenderTarget(Engine::OCCLUSIONSUB);
		Engine::Render_RenderTarget(Engine::LIGHTSHAFT);
		//HDR_ALPHA
		Engine::Render_RenderTarget(Engine::BLUR);

		//SHadowFilter
		Engine::Render_RenderTarget(Engine::FINALIMAGE);
		//Engine::Render_RenderTarget(Engine::RADIALBLUR);
	}

	Clear_RenderGroup();
}

void Engine::CRenderer::Clear_RenderGroup()
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Set_DrawRenderTarget()
{
	if (m_bDrawRenderTarget)
		m_bDrawRenderTarget = false;
	else
		m_bDrawRenderTarget = true;
}

HRESULT CRenderer::Ready_Prototype()
{
	return S_OK;
}

HRESULT CRenderer::Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev)
{
	if (FAILED(pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,	// 그리고자 하는 버텍스의 크기
		0,						// 버퍼 사용 방식(0인 정적 버퍼)
		FVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL)))
		return E_FAIL;

	if (FAILED(pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL)))
		return E_FAIL;

	D3DVIEWPORT9		ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);

	pVertex[0].vPos = _vec4(-3.f, -3.f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(_float(ViewPort.Width)+2.f, -3.f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(_float(ViewPort.Width)+2.f, _float(ViewPort.Height)+2.f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(-3.f, _float(ViewPort.Height)+2.f, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();
 
	if (FAILED(m_pGraphicDev->CreateDepthStencilSurface(7680.f, 4320.f, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadow_DS_Surface, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Resize_Buffer(_float fSizeX, _float fSizeY)
{
	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);

	pVertex[0].vPos = _vec4(-3.f, -3.f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(fSizeX+2.f, -3.f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(fSizeX+2.f, fSizeY+2.f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(-3.f, fSizeY+2.f, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();
	return S_OK;
}


void CRenderer::Render_Priority(LPDIRECT3DDEVICE9 & pGraphicDev, const _double& dTimeDelta)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);
	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_Geometry(dTimeDelta);

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9 & pGraphicDev, const _double& dTimeDelta)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_Geometry(dTimeDelta);
}

void CRenderer::Render_NonAlpha_Cartoon(LPDIRECT3DDEVICE9 & pGraphicDev, const _double & dTimeDelta)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_PostEffect(dTimeDelta);
}

_bool		Compare_Z(CGameObject* pDest, CGameObject* pSour)
{
	return pDest->Get_ViewZ() > pSour->Get_ViewZ();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 & pGraphicDev, const _double& dTimeDelta)
{
	m_RenderGroup[RENDER_ALPHA].sort(Compare_Z);

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_Geometry(dTimeDelta);


/*
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_DOF"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Safe_AddRef(pEffect);

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(5);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
	Safe_Release(pShader);*/


}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9 & pGraphicDev, const _double& dTimeDelta)
{
	for (auto& iter : m_RenderGroup[RENDER_UI])
		iter->Render_Geometry(dTimeDelta);
}

void CRenderer::Render_Shadow(LPDIRECT3DDEVICE9 & pGraphicDev, const _double& dTimeDelta)
{
	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	Engine::Begin_RT(Engine::SHADOW_PLAYER);
	m_pGraphicDev->GetDepthStencilSurface(&m_pOriginal_DS_Surface);
	m_pGraphicDev->SetDepthStencilSurface(m_pShadow_DS_Surface);

	m_pGraphicDev->Clear(0, 0, D3DCLEAR_ZBUFFER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 1.f, 0);

	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_Shadow(dTimeDelta);

	Engine::End_RT(Engine::SHADOW_PLAYER);
	m_pGraphicDev->SetDepthStencilSurface(m_pOriginal_DS_Surface);
	Safe_Release(m_pOriginal_DS_Surface);
	/*Engine::Begin_RT(Engine::SHADOW_FILTERING);

	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_HDR"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Safe_AddRef(pEffect);
	_vec2 fTexScale = _vec2(1.f / ViewPort.Width, 1.f / ViewPort.Height);
	pEffect->SetValue("g_texmapscale", &fTexScale, sizeof(_vec2));
	Engine::SetUp_OnShader(pEffect, Engine::SHADOW_PLAYER, "g_ShadowTexture");
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(7);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::SHADOW_FILTERING);
	Safe_Release(pEffect);
	Safe_Release(pShader);*/
}

void CRenderer::Render_Depth(LPDIRECT3DDEVICE9 & pGraphicDev, const _double & dTimeDelta)
{
	Engine::Begin_RT(Engine::PRE_DEPTH);
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_Depth(dTimeDelta);
	Engine::End_RT(Engine::PRE_DEPTH);
}

void CRenderer::Render_Geometry(LPDIRECT3DDEVICE9 & pGraphicDev, const _double& dTimeDelta)
{
	Engine::Begin_MRT(Engine::GEOMETRY);
	Render_NonAlpha(pGraphicDev, dTimeDelta);
	Engine::End_MRT(Engine::GEOMETRY);
	
}

void CRenderer::Render_Cartoon(LPDIRECT3DDEVICE9 & pGraphicDev, const _double & dTimeDelta)
{
	Engine::Begin_MRT(Engine::CARTOON);
	Render_NonAlpha_Cartoon(pGraphicDev, dTimeDelta);
	Engine::End_MRT(Engine::CARTOON);
}

void CRenderer::Render_Edge(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);


	Engine::Begin_RT(Engine::OUTLINE);
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Outline"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Safe_AddRef(pEffect);

	Engine::SetUp_OnShader(pEffect, Engine::OUTLINE_READY, "g_NormalTexture");
	float fX = 1.f / ViewPort.Width;
	float fY = 1.f / ViewPort.Height;
	pEffect->SetFloat("vPixelOffsetX", fX);
	pEffect->SetFloat("vPixelOffsetY", fY);
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::OUTLINE);

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

void CRenderer::Render_Occlusion(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	Engine::Begin_RT(Engine::OCCLUSION);
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Occlusion"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Safe_AddRef(pEffect);


	Engine::SetUp_OnShader(pEffect, Engine::DEPTH, "g_DepthTexture");
	Engine::SetTexture(pEffect, "g_RandomTexture", 0);
	pEffect->SetFloat("fRadius", m_fRadius);
	//cout << m_fRadius << endl;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::OCCLUSION);


	Engine::Begin_RT(Engine::OCCLUSION_HORIZONTALBLUR);

	_float HBlurWeight[9];
	_float HBlurOffset[9];

	for (_int i = 0; i < 9; i++)
	{
		HBlurOffset[i] = (_float(i) - 4.0f) * (1.0f / _float(80));

		_float x = (_float(i) - 4.f) / 4.f;

		HBlurWeight[i] = m_fGaussMultiplier * Compute_Gaussian(x, m_fGaussMean, m_fGaussStdDev);
	}
	pEffect->SetFloatArray("HBlurWeight", HBlurWeight, 9);
	pEffect->SetFloatArray("HBlurOffset", HBlurOffset, 9);
	Engine::SetUp_OnShader(pEffect, Engine::OCCLUSION_HORIZONTALBLUR, "g_OcclusionBaseTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	Resize_Buffer(160.f, 120.f);
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::OCCLUSION_HORIZONTALBLUR);

	Engine::Begin_RT(Engine::OCCLUSION_VERTICALBLUR);

	_float VBlurWeight[9];
	_float VBlurOffset[9];

	for (_int i = 0; i < 9; i++)
	{
		VBlurOffset[i] = (_float(i) - 4.0f) * (1.0f / _float(60));
		_float x = (_float(i) - 4.f) / 4.f;

		VBlurWeight[i] = m_fGaussMultiplier * Compute_Gaussian(x, m_fGaussMean, m_fGaussStdDev);
	}

	pEffect->SetFloatArray("VBlurWeight", VBlurWeight, 9);
	pEffect->SetFloatArray("VBlurOffset", VBlurOffset, 9);

	Engine::SetUp_OnShader(pEffect, Engine::OCCLUSION_HORIZONTALBLUR, "g_OcclusionHorizontalTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	Resize_Buffer(160.f, 120.f);
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::OCCLUSION_VERTICALBLUR);
	Resize_Buffer(_float(ViewPort.Width), _float(ViewPort.Height));
	Safe_Release(pEffect);
	Safe_Release(pShader);



}

void CRenderer::Render_HDR_Alpha(LPDIRECT3DDEVICE9 & pGraphicDev, const _double dTimeDelta)
{
	m_RenderGroup[RENDER_HDR_ALPHA].sort(Compare_Z);
	Engine::Begin_RT(Engine::BLUR);
	for (auto& iter : m_RenderGroup[RENDER_HDR_ALPHA])
		iter->Render_Geometry(dTimeDelta);
	Engine::End_RT(Engine::BLUR);
}

void CRenderer::Render_LightAcc(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Engine::Begin_RT(Engine::SHADE);
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Shade"));
	if (pShader == nullptr)
		return;
	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Safe_AddRef(pEffect);

	Engine::SetUp_OnShader(pEffect, Engine::NORMAL, "g_NormalTexture");
	Engine::SetUp_OnShader(pEffect, Engine::DEPTH, "g_DepthTexture");
	pEffect->SetFloat("g_fView", m_fCamFar);
	pEffect->Begin(NULL, 0);
	Engine::Render_Light(pEffect);
	pEffect->End();
	Engine::End_RT(Engine::SHADE);

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

void CRenderer::Render_HDRBase(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Engine::Begin_RT(Engine::HDR_BASE); // 툴작업할때 주석걸어야함
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_HDRBase"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Safe_AddRef(pEffect);
	pEffect->SetFloat("Gamma", m_fGamma);
	Engine::SetUp_OnShader(pEffect, Engine::ALBEDO, "g_AlbedoTexture");
	Engine::SetUp_OnShader(pEffect, Engine::SHADE, "g_ShadeTexture");
	Engine::SetUp_OnShader(pEffect, Engine::EMMISIVE, "g_EmmisiveTexture");
	Engine::SetUp_OnShader(pEffect, Engine::OUTLINE, "g_OutlineTexture");
	pEffect->SetFloat("fLightAmount", 3.f);
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::HDR_BASE); // 툴작업할때 주석걸어야함

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

void CRenderer::Render_Luminance(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);
	Engine::Begin_RT(Engine::LUMINANCE1);
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_HDR"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	//GreyDownSample

	Safe_AddRef(pEffect);

	_vec2 vTexUV = _vec2(1.f / ViewPort.Width, 1.f / ViewPort.Height);
	_vec4 offsets[4];
	offsets[0] = _vec4(-0.5f * vTexUV.x, 0.5f * vTexUV.y, 0.f, 0.f);
	offsets[1] = _vec4(0.5f * vTexUV.x, 0.5f * vTexUV.y, 0.f, 0.f);
	offsets[2] = _vec4(-0.5f * vTexUV.x, -0.5f * vTexUV.y, 0.f, 0.f);
	offsets[3] = _vec4(0.5f * vTexUV.x, -0.5f * vTexUV.y, 0.f, 0.f);
	pEffect->SetVectorArray("vGreyOffset", offsets, 4);
	Engine::SetUp_OnShader(pEffect, Engine::HDR_BASE, "g_HDRBaseTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);
	Resize_Buffer(243.f, 243.f);
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::LUMINANCE1);

	//DownSmaple 1 - 5

	//1
	Engine::Begin_RT(Engine::LUMINANCE2);

	_vec4 DSOffset[9];
	_int idx = 0;
	for (_int x = -1; x < 2; x++)
	{
		for (_int y = -1; y < 2; y++)
		{
			DSOffset[idx++] = _vec4(
				_float(x) / 243,
				_float(y) / 243,
				0.0f,   //unused
				0.0f    //unused
			);
		}
	}
	pEffect->SetVectorArray("vDSOffset", DSOffset, 9);

	Engine::SetUp_OnShader(pEffect, Engine::LUMINANCE1, "g_LuminanceTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(1);
	Resize_Buffer(81.f, 81.f);
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::LUMINANCE2);

	//2
	Engine::Begin_RT(Engine::LUMINANCE3);

	idx = 0;
	for (_int x = -1; x < 2; x++)
	{
		for (_int y = -1; y < 2; y++)
		{
			DSOffset[idx++] = _vec4(
				_float(x) / 81,
				_float(y) / 81,
				0.0f,   //unused
				0.0f    //unused
			);
		}
	}
	pEffect->SetVectorArray("vDSOffset", DSOffset, 9);


	Engine::SetUp_OnShader(pEffect, Engine::LUMINANCE2, "g_LuminanceTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(1);
	Resize_Buffer(27.f, 27.f);
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::LUMINANCE3);


	//3
	Engine::Begin_RT(Engine::LUMINANCE4);

	idx = 0;
	for (_int x = -1; x < 2; x++)
	{
		for (_int y = -1; y < 2; y++)
		{
			DSOffset[idx++] = _vec4(
				_float(x) / 27,
				_float(y) / 27,
				0.0f,   //unused
				0.0f    //unused
			);
		}
	}
	pEffect->SetVectorArray("vDSOffset", DSOffset, 9);

	Engine::SetUp_OnShader(pEffect, Engine::LUMINANCE3, "g_LuminanceTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(1);
	Resize_Buffer(9.f, 9.f);
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::LUMINANCE4);

	//4
	Engine::Begin_RT(Engine::LUMINANCE5);

	idx = 0;
	for (_int x = -1; x < 2; x++)
	{
		for (_int y = -1; y < 2; y++)
		{
			DSOffset[idx++] = _vec4(
				_float(x) / 9,
				_float(y) / 9,
				0.0f,   //unused
				0.0f    //unused
			);
		}
	}
	pEffect->SetVectorArray("vDSOffset", DSOffset, 9);

	Engine::SetUp_OnShader(pEffect, Engine::LUMINANCE4, "g_LuminanceTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(1);
	Resize_Buffer(3.f, 3.f);
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::LUMINANCE5);

	//5
	Engine::Begin_RT(Engine::LUMINANCE6);

	idx = 0;
	for (_int x = -1; x < 2; x++)
	{
		for (_int y = -1; y < 2; y++)
		{
			DSOffset[idx++] = _vec4(
				_float(x) / 3,
				_float(y) / 3,
				0.0f,   //unused
				0.0f    //unused
			);
		}
	}
	pEffect->SetVectorArray("vDSOffset", DSOffset, 9);

	Engine::SetUp_OnShader(pEffect, Engine::LUMINANCE5, "g_LuminanceTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(1);
	Resize_Buffer(1.f, 1.f);
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::LUMINANCE6);

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

void CRenderer::Render_HDR(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	Engine::Begin_RT(Engine::BRIGHTPASS);
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_HDR"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Safe_AddRef(pEffect);
	pEffect->SetFloat("Gamma", m_fGamma);
	_vec4 vOffsets[4];
	_vec2 vTexUV = _vec2(1.f / ViewPort.Width, 1.f / ViewPort.Height);
	vOffsets[0] = _vec4(-0.5f * vTexUV.x, 0.5f * vTexUV.y, 0.0f, 0.0f);
	vOffsets[1] = _vec4(0.5f * vTexUV.x, 0.5f * vTexUV.y, 0.0f, 0.0f);
	vOffsets[2] = _vec4(-0.5f * vTexUV.x, -0.5f * vTexUV.y, 0.0f, 0.0f);
	vOffsets[3] = _vec4(0.5f * vTexUV.x, -0.5f * vTexUV.y, 0.0f, 0.0f);
	pEffect->SetVectorArray("vDSOffsetBright", vOffsets, 4);
	pEffect->SetFloat("fBrightPassThreshold", 2.f);
	Engine::SetUp_OnShader(pEffect, Engine::HDR_BASE, "g_HDRBaseTexture");
	Engine::SetUp_OnShader(pEffect, Engine::BLUR, "g_BlurTexture");
	pEffect->SetBool("bFilterMonochrome", m_bFilterMonoChrome);
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(2);
	Resize_Buffer(ViewPort.Width, ViewPort.Height);
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::BRIGHTPASS);


	//BrightPass DS
	Engine::Begin_RT(Engine::BRIGHTPASSDOWNSAMPLE);
	_vec4 vDSOffsets[16];
	_int idx = 0;
	for (int i = -2; i < 2; i++)
	{
		for (int j = -2; j < 2; j++)
		{
			vDSOffsets[idx++] = _vec4(
				(_float(i) + 0.5f) * (1.f / ViewPort.Width * 0.5f),
				(_float(j) + 0.5f) * (1.f / ViewPort.Width * 0.5f),
				0.0f, // unused 
				0.0f  // unused
			);
		}
	}
	pEffect->SetVectorArray("vDSOffsetBright", vDSOffsets, 16);

	Engine::SetUp_OnShader(pEffect, Engine::BRIGHTPASS, "g_BrightPassTexture");
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(3);
	Resize_Buffer(ViewPort.Width * 0.5f, ViewPort.Height * 0.5f);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Engine::End_RT(Engine::BRIGHTPASSDOWNSAMPLE);

	//HorizontalBlur
	Engine::Begin_RT(Engine::HORIZONTALBLUR);

	_float HBloomWeights[9];
	_float HBloomOffSets[9];

	for (_int i = 0; i < 9; i++)
	{
		// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
		//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
		//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
		HBloomOffSets[i] = (_float(i) - 4.0f) * (1.0f / _float(ViewPort.Width * 0.5f));

		// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
		_float x = (_float(i) - 4.f) / 4.f;

		// Use a gaussian distribution. Changing the standard-deviation
		// (second parameter) as well as the amplitude (multiplier) gives
		// distinctly different results.
		HBloomWeights[i] = m_fGaussMultiplier * Compute_Gaussian(x, m_fGaussMean, m_fGaussStdDev);
	}
	pEffect->SetFloatArray("HBloomWeights", HBloomWeights, 9);
	pEffect->SetFloatArray("HBloomOffsets", HBloomOffSets, 9);

	Engine::SetUp_OnShader(pEffect, Engine::BRIGHTPASSDOWNSAMPLE, "g_DownSampleTexture");
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(4);
	//Resize_Buffer(80.f, 60.f);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();


	Engine::End_RT(Engine::HORIZONTALBLUR);

	//Vertical BLUR
	Engine::Begin_RT(Engine::VERTICALBLUR);

	_float fVBloomWeights[9];
	_float fVBloomOffSets[9];

	for (_int i = 0; i < 9; i++)
	{
		// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
		//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
		//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
		fVBloomOffSets[i] = (_float(i) - 4.0f) * (1.0f / _float(ViewPort.Height * 0.5f));

		// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
		_float x = (_float(i) - 4.f) / 4.f;

		// Use a gaussian distribution. Changing the standard-deviation
		// (second parameter) as well as the amplitude (multiplier) gives
		// distinctly different results.
		fVBloomWeights[i] = m_fGaussMultiplier * Compute_Gaussian(x, m_fGaussMean, m_fGaussStdDev);
	}

	pEffect->SetFloatArray("VBloomWeights", fVBloomWeights, 9);
	pEffect->SetFloatArray("VBloomOffsets", fVBloomOffSets, 9);

	Engine::SetUp_OnShader(pEffect, Engine::HORIZONTALBLUR, "g_HorizontalBlurTexture");
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(5);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();


	Engine::End_RT(Engine::VERTICALBLUR);

	Engine::Begin_RT(Engine::HDRFINISH);

	Engine::SetUp_OnShader(pEffect, Engine::HDR_BASE, "g_HDRBaseTexture");
	Engine::SetUp_OnShader(pEffect, Engine::LUMINANCE6, "g_LuminanceTexture");
	Engine::SetUp_OnShader(pEffect, Engine::VERTICALBLUR, "g_VerticalBlurTexture");
	Engine::SetUp_OnShader(pEffect, Engine::EMMISIVE, "g_EmmisiveTexture");
	Engine::SetUp_OnShader(pEffect, Engine::BLUR, "g_BlurTexture");
	pEffect->SetFloat("g_rcp_bloom_tex_w", 1.f / (ViewPort.Width * 0.5f));
	pEffect->SetFloat("g_rcp_bloom_tex_h", 1.f / (ViewPort.Height * 0.5f));
	pEffect->SetFloat("fExposure", m_fExposure);
	pEffect->SetFloat("fGaussianScalar", m_fGaussMultiplier);
	pEffect->SetBool("bFilterMonochrome", m_bFilterMonoChrome);
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(6);
	Resize_Buffer(_float(ViewPort.Width), _float(ViewPort.Height));

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Engine::End_RT(Engine::HDRFINISH);

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

void CRenderer::Render_LightShaft(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Engine::Begin_RT(Engine::LIGHTSHAFT);
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_LightShaft"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	Safe_AddRef(pEffect);
	Engine::SetUp_OnShader(pEffect, Engine::OCCLUSIONSUB, "g_OcclusionTexture");
	//X : Density, Y : Decay, Z : Weight, W : Exposure
	//pEffect->SetVector("vLightShaftValue", &_vec4(0.726f, 0.99f, 0.38f, 0.2f));
	pEffect->SetVector("vLightShaftValue", &_vec4(0.926f, 0.96815f, 0.38f, 0.1f));
	pEffect->SetFloat("fLightPositionX", m_vLightPosition.x);
	pEffect->SetFloat("fLightPositionY", m_vLightPosition.y);
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::LIGHTSHAFT);
	Safe_Release(pEffect);
	Safe_Release(pShader);
}

void CRenderer::Render_RadialBlur(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Engine::Begin_RT(Engine::RADIALBLUR);
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_HDR"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	Safe_AddRef(pEffect);

	Engine::SetUp_OnShader(pEffect, Engine::HDR_BASE, "g_RadialBlurTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(7);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::RADIALBLUR);
	Safe_Release(pEffect);
	Safe_Release(pShader);
	//if (m_bUseRadialBlur)
	//	return false;

	//m_bUseRadialBlur = true;
	//m_fRadial_EffectAmount = fEffectAmount;
	//m_fRadial_Radius = fRadius;

	////월드좌표를 UV좌표까지 이동시키자
	//_matrix matView, matProj;
	//_vec3 vPos = { 0.f,0.f,0.f };
	//CEffectMgr::GetInstance()->Get_Current_ViewProj(&matView, &matProj);
	//D3DXVec3TransformCoord(&vPos, &vWorldFocusPos, &matView);
	//D3DXVec3TransformCoord(&vPos, &vPos, &matProj);

	////Z나누기 후 UV좌표계로 변환
	//m_vRadial_Center = { (vPos.x)*0.5f + 0.5f,-(vPos.y)*0.5f + 0.5f };

	//m_vRadial_Center = ENGINE::CFunc::Clamp(m_vRadial_Center, _vec2(0.f, 0.f), _vec2(1.f, 1.f));



	//return true;
}

void CRenderer::Render_DOF(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	//DOF_DOWNSAMPLE
	Engine::Begin_RT(Engine::DOFDOWNSAMPLE);
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_DOF"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	Safe_AddRef(pEffect);

	Engine::SetUp_OnShader(pEffect, Engine::HDRFINISH, "g_HDRFinishTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);
	Resize_Buffer(_float(ViewPort.Width * 0.5f), _float(ViewPort.Height * 0.5f));

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::DOFDOWNSAMPLE);

	//DOF_GAUSSIAN
	Engine::Begin_RT(Engine::DOFHORIZONTALBLUR);
	Engine::SetUp_OnShader(pEffect, Engine::DOFDOWNSAMPLE, "g_DownSampleTexture");
	_float HBlurWeights[9];
	_float HBlurOffSets[9];

	for (_int i = 0; i < 9; i++)
	{
		// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
		//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
		//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
		HBlurOffSets[i] = (_float(i) - 4.0f) * (1.0f / _float(ViewPort.Width * 0.5f));

		// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
		_float x = (_float(i) - 4.f) / 4.f;

		// Use a gaussian distribution. Changing the standard-deviation
		// (second parameter) as well as the amplitude (multiplier) gives
		// distinctly different results.
		HBlurWeights[i] = m_fGaussMultiplier * Compute_Gaussian(x, m_fGaussMean, m_fGaussStdDev);
	}
	pEffect->SetFloatArray("HBlurWeights", HBlurWeights, 9);
	pEffect->SetFloatArray("HBlurOffsets", HBlurOffSets, 9);
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(1);
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::DOFHORIZONTALBLUR);

	Engine::Begin_RT(Engine::DOFVERTICALBLUR);
	Engine::SetUp_OnShader(pEffect, Engine::DOFHORIZONTALBLUR, "g_DOFHorizontalBlurTexture");
	_float VBlurWeights[9];
	_float VBlurOffSets[9];

	for (_int i = 0; i < 9; i++)
	{
		// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
		//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
		//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
		VBlurOffSets[i] = (_float(i) - 4.0f) * (1.0f / _float(ViewPort.Height * 0.5f));

		// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
		_float x = (_float(i) - 4.f) / 4.f;

		// Use a gaussian distribution. Changing the standard-deviation
		// (second parameter) as well as the amplitude (multiplier) gives
		// distinctly different results.
		VBlurWeights[i] = m_fGaussMultiplier * Compute_Gaussian(x, m_fGaussMean, m_fGaussStdDev);
	}
	pEffect->SetFloatArray("VBlurWeights", VBlurWeights, 9);
	pEffect->SetFloatArray("VBlurOffsets", VBlurOffSets, 9);
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(2);
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::DOFVERTICALBLUR);

	Safe_Release(pEffect);
	Safe_Release(pShader);
	Resize_Buffer(_float(ViewPort.Width), _float(ViewPort.Height));
}

void CRenderer::Render_Fade(LPDIRECT3DDEVICE9 & pGraphicDev, const _double& dTimeDelta)
{
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_DOF"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	Safe_AddRef(pEffect);
	if (m_bStartFade)
	{
		m_vFadeColor += _vec4(0.f, 0.f, 0.f, dTimeDelta * 5);
		if (m_vFadeColor.w > 1.f)
			m_bStartFade = false;
	}
	if (m_bEndFade)
	{
		m_vFadeColor -= _vec4(0.f, 0.f, 0.f, dTimeDelta * 5);
		if (m_vFadeColor.w < 0.f)
			m_bEndFade = false;
	}
	pEffect->SetVector("vFadeColor", &m_vFadeColor);
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(4);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
	Safe_Release(pShader);
}


void CRenderer::Render_Final(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	Engine::Begin_RT(Engine::FINALIMAGE);
	CShader*	pShader = dynamic_cast<CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_DOF"));
	if (pShader == nullptr)
		return;

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	Safe_AddRef(pEffect);
	//Engine::SetUp_OnShader(pEffect, Engine::ALBEDO, "g_AlbedoTexture");
	//Engine::SetUp_OnShader(pEffect, Engine::DOFVERTICALBLUR, "g_DOFBlurTexture");
	Engine::SetUp_OnShader(pEffect, Engine::HDRFINISH, "g_HDRTexture");
	Engine::SetUp_OnShader(pEffect, Engine::LIGHTSHAFT, "g_LightShaftTexture");
	//Engine::SetUp_OnShader(pEffect, Engine::RADIALBLUR, "g_RadialBlurTexture");
	//pEffect->SetBool("bFilterMonochrome", m_bFilterMonoChrome);
	//pEffect->SetBool("bRadial", m_bUseRadialBlur);
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(3);
	pEffect->CommitChanges();
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	Engine::End_RT(Engine::FINALIMAGE);

	//////////////////////////////final process///////////////////////////////
	Engine::SetUp_OnShader(pEffect, Engine::FINALIMAGE, "g_FinalImageTexture");
	pEffect->SetBool("bFilterMonochrome", m_bFilterMonoChrome);
	pEffect->SetBool("bRadial", m_bUseRadialBlur);
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(5);
	pEffect->CommitChanges();
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();


	Safe_Release(pEffect);
	Safe_Release(pShader);


}
_float CRenderer::Compute_Gaussian(_float fX, _float fMean, _float std_Deviation)
{
	return (1.0f / sqrt(2.0f * D3DX_PI * std_Deviation * std_Deviation))
		* expf((-((fX - fMean) * (fX - fMean))) / (2.f * std_Deviation * std_Deviation));
}

void CRenderer::Setting_Sun(_vec3 vPos)
{
	_matrix matView, matProj;
	Engine::Get_CamView(&matView);
	Engine::Get_CamProj(&matProj);
	D3DXVec3TransformCoord(&vPos, &vPos, &matView);
	D3DXVec3TransformCoord(&vPos, &vPos, &matProj);
	//cout << " X : " << vPos.x << " Y : " << vPos.y << " Z : " << vPos.z << endl;
	m_vLightPosition.x = vPos.x * 0.5f + 0.5f;
	m_vLightPosition.y = -vPos.y * 0.5f + 0.5f;
}


CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRenderer*	pInstance = new CRenderer(pGraphicDev);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		MSG_BOX("Failed To Creating CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone()
{
	AddRef();

	return this;
}

void Engine::CRenderer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Clear_RenderGroup();
	CComponent::Free();
	Safe_Release(m_pOriginal_DS_Surface);
	Safe_Release(m_pShadow_DS_Surface);
}

