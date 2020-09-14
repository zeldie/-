#include "Export_Engine.h"
#include "Management.h"
#include "Subject.h"
#include "Observer.h"
#include "Renderer.h"
#include "Component.h"
#include "TextureMgr.h"
USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement()
	:m_pScene(nullptr),
	m_pRenderer(nullptr)
{

}

Engine::CManagement::~CManagement()
{
	Free();
}

CComponent * CManagement::Get_Component(LAYERTYPE eLayerType, const wstring & wstrObjTag, COMPONENTTYPE eComponentType, COMPONENTID eID, _int iIndex)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(eLayerType, wstrObjTag, eComponentType, eID, iIndex);
}

list<CGameObject*> CManagement::Get_GameObjectlist(LAYERTYPE eLayerType, const wstring & wstrObjTag)
{
	return m_pScene->Get_GameObjectlist(eLayerType, wstrObjTag);
}

CGameObject * CManagement::Get_GameObject(LAYERTYPE eLayerType, const wstring & wstrObjTag, _int iIndex)
{
	if (nullptr == m_pScene)
		return nullptr;
	return m_pScene->Get_GameObject(eLayerType, wstrObjTag, iIndex);
}

HRESULT CManagement::Add_GameObject(LAYERTYPE eLayerType, const wstring & wstrObjTag, CGameObject * pInstance)
{
	if (m_pScene == nullptr)
		return E_FAIL;
	m_pScene->Add_GameObject(eLayerType, wstrObjTag, pInstance);
	return S_OK;
}

HRESULT CManagement::AddSubject(SUBJECTTYPE eSubjectType)
{
	auto iter_find = m_Subjects.find(eSubjectType);

	if (m_Subjects.end() != iter_find)
		Safe_Release(iter_find->second);

	m_Subjects.insert(make_pair(eSubjectType, CSubject::Create()));

	return S_OK;
}

void CManagement::Subscribe(SUBJECTTYPE eSubjectType, CObserver * pObserver)
{
	auto iter_find = m_Subjects.find(eSubjectType);

	if (m_Subjects.end() == iter_find)
		return;

	iter_find->second->Subscribe(pObserver);
}

void CManagement::UnSubscribe(SUBJECTTYPE eSubjectType, CObserver * pObserver)
{
	auto iter_find = m_Subjects.find(eSubjectType);

	if (m_Subjects.end() == iter_find)
		return;

	iter_find->second->UnSubscribe(pObserver);
}

void CManagement::Notify(SUBJECTTYPE eSubjectType, int iMessage, void * pData)
{
	auto iter_find = m_Subjects.find(eSubjectType);

	if (m_Subjects.end() == iter_find)
		return;

	iter_find->second->Notify(iMessage, pData);
}

void CManagement::ClearSubject()
{
	for (auto& MyPair : m_Subjects)
		Safe_Release(MyPair.second);

	m_Subjects.clear();
}

HRESULT CManagement::SetUp_Scene(CScene * pScene)
{
	if (nullptr != m_pScene)
		Safe_Release(m_pScene);

	m_pScene = pScene;

	return CHANGE_SCENE;
}

void CManagement::ClearRenderer()
{
	m_pRenderer->Clear_RenderGroup();
}

HRESULT CManagement::Ready_Shaders(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	CShader*		pShader = nullptr;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC, L"Shader_Sky", L"../../Reference/Header/Shader_Sky.hpp")))
		return E_FAIL;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC , L"Shader_Mesh", L"../../Reference/Header/Shader_Mesh.hpp")))
		return E_FAIL;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC, L"Shader_Normal", L"../../Reference/Header/Shader_Normal.hpp")))
		return E_FAIL;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC, L"Shader_Shade", L"../../Reference/Header/Shader_Shade.hpp")))
		return E_FAIL;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC, L"Shader_HDRBase", L"../../Reference/Header/Shader_HDRBase.hpp")))
		return E_FAIL;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC, L"Shader_HDR", L"../../Reference/Header/Shader_HDR.hpp")))
		return E_FAIL;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC, L"Shader_Outline", L"../../Reference/Header/Shader_Outline.hpp")))
		return E_FAIL;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC, L"Shader_Occlusion", L"../../Reference/Header/Shader_Occlusion.hpp")))
		return E_FAIL;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC, L"Shader_DOF", L"../../Reference/Header/Shader_DOF.hpp")))
		return E_FAIL;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC, L"Shader_LightShaft", L"../../Reference/Header/Shader_LightShaft.hpp")))
		return E_FAIL;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC, L"Shader_Water", L"../../Reference/Header/Shader_Water.hpp")))
		return E_FAIL;

	if (FAILED(Engine::Ready_Shader(pGraphicDev, RESOURCE_STATIC, L"Shader_Effect", L"../../Reference/Header/Shader_Effect.hpp")))
		return E_FAIL;

	//렌더 타겟

	//프리뎁스
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::PRE_DEPTH, ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	//if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::PRE_DEPTH, 540.f, 540.f, 180.f, 180.f)))
	//	return E_FAIL;

	//알베도 <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::ALBEDO, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::ALBEDO, 0.f, 0.f, 180.f, 180.f)))
		return E_FAIL;
	//노말 <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::NORMAL, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::NORMAL, 0.f, 180.f, 180.f, 180.f)))
		return E_FAIL;
	//뎁스 <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::DEPTH, ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::DEPTH, 0.f, 360.f, 180.f, 180.f)))
		return E_FAIL;
	//이미시브 <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::EMMISIVE, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::EMMISIVE, 0.f, 540.f, 180.f, 180.f)))
		return E_FAIL;
	//쉐이드 <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::SHADE, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::SHADE, 360.f, 0.f, 180.f, 180.f)))
		return E_FAIL;

	//finalIMAGE <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::FINALIMAGE, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::FINALIMAGE, 540.f, 180.f, 180.f, 180.f)))
		return E_FAIL;


	//Blur <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::BLUR, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::BLUR, 360.f, 540.f, 180.f, 180.f)))
		return E_FAIL;

	//HDRBase <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::HDR_BASE, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::HDR_BASE, 360.f, 180.f, 180.f, 180.f)))
		return E_FAIL;

	//Luminance GreyDownSample
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::LUMINANCE1, 243,243, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/*if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::LUMINANCE1, 540.f, 0.f, 180.f, 180.f)))
		return E_FAIL;*/

	//Luminance DownSample
	//1
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::LUMINANCE2, 81, 81, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/*if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::LUMINANCE2, 540.f, 180.f, 180.f, 180.f)))
		return E_FAIL;*/
	//2
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::LUMINANCE3,27, 27, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/*if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::LUMINANCE3, 540.f, 360.f, 180.f, 180.f)))
		return E_FAIL;*/
	//3
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::LUMINANCE4, 9, 9, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/*if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::LUMINANCE4, 720.f, 0.f, 180.f, 180.f)))
		return E_FAIL;*/
	//4
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::LUMINANCE5, 3, 3, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/*if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::LUMINANCE5, 720.f, 180.f, 180.f, 180.f)))
		return E_FAIL;*/
	//5
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::LUMINANCE6, 1, 1, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	//if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::LUMINANCE6, 360.f, 180.f, 180.f, 180.f)))
	//	return E_FAIL;

	//BrightPass <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::BRIGHTPASS, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::BRIGHTPASS, 360.f, 360.f, 180.f, 180.f)))
		return E_FAIL;

	//BrightPassDownSample
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::BRIGHTPASSDOWNSAMPLE, ViewPort.Width * 0.5f, ViewPort.Height * 0.5f, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/*if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::BRIGHTPASSDOWNSAMPLE, 900.f, 180.f, 180.f, 180.f)))
		return E_FAIL;*/

	//HorizontalBlur
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::HORIZONTALBLUR, ViewPort.Width * 0.5f, ViewPort.Height * 0.5f, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	//if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::HORIZONTALBLUR, 540.f, 180.f, 180.f, 180.f)))
	//	return E_FAIL;

	//VerticlaBlur
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::VERTICALBLUR, ViewPort.Width * 0.5f, ViewPort.Height * 0.5f,  D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	//if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::VERTICALBLUR, 540.f, 360.f, 180.f, 180.f)))
	//	return E_FAIL;

	//Outline_Ready
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::OUTLINE_READY, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/*if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::OUTLINE_READY, 180.f, 180.f, 180.f, 180.f)))
		return E_FAIL;*/

	//Outline <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::OUTLINE, ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::OUTLINE, 180.f, 0.f, 180.f, 180.f)))
		return E_FAIL;

	//Occlusion <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::OCCLUSION, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/*if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::OCCLUSION, 180.f, 540.f, 180.f, 180.f)))
		return E_FAIL;*/

	//Occlusion HORIZONTALBLUR
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::OCCLUSION_HORIZONTALBLUR, 160, 120, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/*if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::OCCLUSION_HORIZONTALBLUR, 360.f, 540.f, 180.f, 180.f)))
		return E_FAIL;*/

	//Occlusion VERTICALBLUR
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::OCCLUSION_VERTICALBLUR, 160, 120, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::OCCLUSION_VERTICALBLUR, 180.f, 360.f, 180.f, 180.f)))
		return E_FAIL;

	//HDR FINISH <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::HDRFINISH, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	//if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::HDRFINISH, 720.f, 0.f, 180.f, 180.f)))
	//	return E_FAIL;

	//DOF DOWNSAMPLE
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::DOFDOWNSAMPLE, _uint(ViewPort.Width * 0.5f), _uint(ViewPort.Height * 0.5f), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	//if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::DOFDOWNSAMPLE, 720.f, 180.f, 180.f, 180.f)))
	//	return E_FAIL;
	//DOF_HORIZONTALBLUR
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::DOFHORIZONTALBLUR, _uint(ViewPort.Width * 0.5f), _uint(ViewPort.Height * 0.5f), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	//if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::DOFHORIZONTALBLUR, 720.f, 360.f, 180.f, 180.f)))
	//	return E_FAIL;

	//DOF_VERTICALBLUR
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::DOFVERTICALBLUR, _uint(ViewPort.Width * 0.5f), _uint(ViewPort.Height * 0.5f), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::DOFVERTICALBLUR, 540.f, 0.f, 180.f, 180.f)))
		return E_FAIL;

	//LightShaft <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::LIGHTSHAFT, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::LIGHTSHAFT, 180.f, 540.f, 180.f, 180.f)))
		return E_FAIL;

	//OcclusionSub ,이거는 라이트섀프트 위함
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::OCCLUSIONSUB, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	//if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::OCCLUSIONSUB, 900.f, 180.f, 180.f, 180.f)))
	//	return E_FAIL;

	//Radial BLur
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::RADIALBLUR, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	//if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::RADIALBLUR, 360.f, 540.f, 180.f, 180.f)))
	//	return E_FAIL;

	//Shadow <--------------------
	if (FAILED(Engine::Ready_RenderTarget(pGraphicDev, Engine::SHADOW_PLAYER, 1280.f, 720.f, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(Engine::Ready_RenderTargetBuffer(Engine::SHADOW_PLAYER, 180.f, 180.f, 180.f, 180.f)))
		return E_FAIL;
	//7680.f, 4320.f
	//지오메트리 MRT list + SUB
	if (Engine::Ready_MRT(Engine::GEOMETRY, Engine::ALBEDO))
		return E_FAIL;
	if (Engine::Ready_MRT(Engine::GEOMETRY, Engine::NORMAL))
		return E_FAIL;
	if (Engine::Ready_MRT(Engine::GEOMETRY, Engine::DEPTH))
		return E_FAIL;
	if (Engine::Ready_MRT(Engine::GEOMETRY, Engine::EMMISIVE))
		return E_FAIL;

	if (Engine::Ready_MRT(Engine::CARTOON, Engine::OUTLINE_READY))
		return E_FAIL;
	if (Engine::Ready_MRT(Engine::CARTOON, Engine::OCCLUSIONSUB))
		return E_FAIL;


	return S_OK;
}

HRESULT CManagement::Ready_Renderer(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pRenderer = CRenderer::Create(pGraphicDev);
	m_pRenderer->Ready_Renderer(pGraphicDev);
	Engine::Add_Component(pComponent, L"RendererCom", RESOURCE_STATIC);
	return S_OK;
}

HRESULT CManagement::Ready_TextureMgr(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	CTextureMgr::GetInstance()->Ready_TextureMgr(L"../../Resource/Texture/EffectSub/%d.tga", 31, pGraphicDev);
	return S_OK;
}

_int CManagement::Update_Scene(const _double & dTimeDelta)
{
	if (nullptr == m_pScene)
		return ERROR_EXIT;

	int iExit = m_pScene->Update_Scene(dTimeDelta);
	return iExit;
}

_int CManagement::LateUpdate_Scene(const _double & dTimeDelta)
{
	if (nullptr == m_pScene)
		return ERROR_EXIT;

	if (Engine::KeyDown(DIK_F1))
		m_pRenderer->Set_DrawRenderTarget();

	int iExit = m_pScene->LateUpdate_Scene(dTimeDelta);

	return iExit;
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 & pGraphicDev, const _double& dTimeDelta)
{
	m_pRenderer->Render_GameObject(pGraphicDev, dTimeDelta);

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void CManagement::Free()
{
	for (auto& MyPair : m_Subjects)
		Safe_Release(MyPair.second);

	m_Subjects.clear();
	Safe_Release(m_pScene);
}
