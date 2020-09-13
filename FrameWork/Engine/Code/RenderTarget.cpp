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
								D3DPOOL_DEFAULT,	// ���� Ÿ�� ���� �ÿ��� managed�� �����ϴ� ��� Ŭ���̾�Ʈ�� ���ߴ� ��찡 �߻��� �� �ִ�.
								&m_pTargetTexture)))
								return E_FAIL;
	m_ClearColor = Color;

	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);	// �ؽ��ķκ��� ����Ÿ�ٿ� ����ϱ� ���� ������ ����
	// 1���� : ���� miplevel�� ������ ������� ���, �� ��°�� ����� ���ΰ��� ���� ���� ��, �츮�� �ϳ��� ��������� 0������ ����
	Safe_Release(m_pTargetTexture); // ���۷��� ����

	return S_OK;
}

void Engine::CRenderTarget::SetUp_OnGraphicDev(const _uint& iIndex)
{
	// ������ index ��ġ�� ����Ǿ� �ִ� ȭ�� �ؽ�ó�� ���� �����Ѵ�.(������� ������ ������ �ƴ϶� ��ũ�� ���� ���̱� ������ ���� �����͸� �ٸ� ���� �ӽ� ������ �ص־� �Ѵ�)
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

