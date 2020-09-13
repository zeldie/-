#include "RenderTarget.h"
#include "ScreenTex.h"
USING(Engine)

Engine::CRenderTarget::CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pTargetTexture(nullptr)
	, m_pTargetSurface(nullptr)
	, m_pOldTargetSurface(nullptr)
	, m_pScreenBuffer(nullptr)

{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CRenderTarget::~CRenderTarget()
{
	
}

HRESULT Engine::CRenderTarget::Ready_RenderTarget(const _uint& iWidth,
												const _uint& iHeight,
												D3DFORMAT Format, 
												D3DXCOLOR Color)
{
	if (FAILED(D3DXCreateTexture(m_pGraphicDev,
								iWidth,
								iHeight,
								1,
								D3DUSAGE_RENDERTARGET,
								Format,
								D3DPOOL_DEFAULT,	// 렌더 타겟 생성 시에는 managed로 설정하는 경우 클라이언트가 멈추는 경우가 발생할 수 있다.
								&m_pTargetTexture)))
								return E_FAIL;
	m_ClearColor = Color;

	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);	// 텍스쳐로부터 렌더타겟에 출력하기 위한 정보를 추출
	// 1인자 : 만약 miplevel을 여러개 만들었을 경우, 몇 번째를 사용할 것인가를 묻는 인자 값, 우리는 하나만 만들었으니 0번으로 세팅
	Safe_Release(m_pTargetTexture); // 레퍼런스 감소

	return S_OK;
}

void Engine::CRenderTarget::SetUp_OnGraphicDev(const _uint& iIndex)
{
	// 기존에 index 장치에 연결되어 있던 화면 텍스처를 얻어와 보관한다.(해제라는 개념이 삭제가 아니라 링크를 끊는 것이기 때문에 기존 데이터를 다른 곳에 임시 저장을 해둬야 한다)
	m_pGraphicDev->GetRenderTarget(iIndex, &m_pOldTargetSurface);

	m_pGraphicDev->SetRenderTarget(iIndex, m_pTargetSurface);
}

void CRenderTarget::Clear_RenderTarget()
{
	m_pGraphicDev->GetRenderTarget(0, &m_pOldTargetSurface);
	
	m_pGraphicDev->SetRenderTarget(0, m_pTargetSurface);

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET /*| D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL*/, m_ClearColor, 1.f, 0);

	m_pGraphicDev->SetRenderTarget(0, m_pOldTargetSurface);

	Safe_Release(m_pOldTargetSurface);

}


void CRenderTarget::Release_OnGraphicDev(const _uint & iIndex)
{
	m_pGraphicDev->SetRenderTarget(iIndex, m_pOldTargetSurface);
	Safe_Release(m_pOldTargetSurface);
}

HRESULT CRenderTarget::Ready_RenderTargetBuffer(const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	m_pScreenBuffer = CScreenTex::Create(m_pGraphicDev, fX, fY, fSizeX, fSizeY);
	return S_OK;
}

void CRenderTarget::Render_RenderTarget()
{
	m_pGraphicDev->SetTexture(0, m_pTargetTexture);

	m_pScreenBuffer->Render_Buffer();

}

void CRenderTarget::SetUp_OnShader(LPD3DXEFFECT & pEffect, const char* pConstantName)
{
	pEffect->SetTexture(pConstantName, m_pTargetTexture);
}

CRenderTarget* Engine::CRenderTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	CRenderTarget*		pInstance = new CRenderTarget(pGraphicDev);

	if (FAILED(pInstance->Ready_RenderTarget(iWidth, iHeight, Format, Color)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CRenderTarget::Free()
{
	Safe_Release(m_pOldTargetSurface);
	Safe_Release(m_pTargetTexture);
	Safe_Release(m_pScreenBuffer);
	Safe_Release(m_pGraphicDev);
}

