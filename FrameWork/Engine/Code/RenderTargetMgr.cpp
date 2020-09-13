#include "RenderTargetMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderTargetMgr)

Engine::CRenderTargetMgr::CRenderTargetMgr()

{

}

Engine::CRenderTargetMgr::~CRenderTargetMgr()
{
	Free();
}

HRESULT Engine::CRenderTargetMgr::Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev,
														RENDERTARGET eTargetTag,
														const _uint& iWidth,
														const _uint& iHeight,
														D3DFORMAT Format,
														D3DXCOLOR Color)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(eTargetTag);

	if (nullptr != pRenderTarget)
		return E_FAIL;

	pRenderTarget = CRenderTarget::Create(pGraphicDev, iWidth, iHeight, Format, Color);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	m_mapRenderTarget.emplace(eTargetTag, pRenderTarget);

	return S_OK;
}


CRenderTarget* Engine::CRenderTargetMgr::Find_RenderTarget(RENDERTARGET eTargetTag)
{
	auto iter = m_mapRenderTarget.find(eTargetTag);

	if (iter == m_mapRenderTarget.end())
		return nullptr;

	return iter->second;
}

HRESULT CRenderTargetMgr::Ready_MRT(RENDERTARGETLIST eMRTlistTag, RENDERTARGET eTargetTag)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(eTargetTag);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	list<CRenderTarget*>*	pMRTList = Find_MRT(eMRTlistTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);
		m_mapMRT.emplace(eMRTlistTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	return S_OK;
}

HRESULT CRenderTargetMgr::Begin_MRT(RENDERTARGETLIST eMRTlistTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(eMRTlistTag);
	if (pMRTList == nullptr)
		return E_FAIL;

	for (auto& iter : *pMRTList)
		iter->Clear_RenderTarget();

	_uint iIndex = 0;

	for (auto& iter : *pMRTList)
		iter->SetUp_OnGraphicDev(iIndex++);

	return S_OK;
}

HRESULT CRenderTargetMgr::End_MRT(RENDERTARGETLIST eMRTlistTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(eMRTlistTag);
	if (pMRTList == nullptr)
		return E_FAIL;

	_uint iIndex = 0;

	for (auto& iter : *pMRTList)
		iter->Release_OnGraphicDev(iIndex++);


	return S_OK;
}

HRESULT CRenderTargetMgr::Begin_RT(RENDERTARGET eTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(eTargetTag);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	pRenderTarget->Clear_RenderTarget();

	pRenderTarget->SetUp_OnGraphicDev(0);
	return S_OK;
}

HRESULT CRenderTargetMgr::End_RT(RENDERTARGET eTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(eTargetTag);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	pRenderTarget->Release_OnGraphicDev(0);
	return S_OK;
}

HRESULT CRenderTargetMgr::Ready_RenderTargetBuffer(RENDERTARGET eTargetTag, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(eTargetTag);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	if (FAILED(pRenderTarget->Ready_RenderTargetBuffer(fX, fY, fSizeX, fSizeY)))
		return E_FAIL;

	return S_OK;
}

void CRenderTargetMgr::Render_RenderTarget(RENDERTARGETLIST eMRTlistTag)
{
	list<CRenderTarget*>*	 pMRTList = Find_MRT(eMRTlistTag);
	if (pMRTList == nullptr)
		return;

	for (auto& iter : *pMRTList)
		iter->Render_RenderTarget();

}

void CRenderTargetMgr::Render_RenderTarget(RENDERTARGET eTargetTag)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(eTargetTag);
	if (pRenderTarget == nullptr)
		return;
	pRenderTarget->Render_RenderTarget();
}

void CRenderTargetMgr::SetUp_OnShader(LPD3DXEFFECT & pEffect, RENDERTARGET eTargetTag, const char* pConstantName)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(eTargetTag);

	if (nullptr == pRenderTarget)
		return;

	pRenderTarget->SetUp_OnShader(pEffect, pConstantName);
}

list<CRenderTarget*>* CRenderTargetMgr::Find_MRT(RENDERTARGETLIST eMRTlistTag)
{
	auto	iter = m_mapMRT.find(eMRTlistTag);

	if (iter == m_mapMRT.end())
		return nullptr;


	return &iter->second;
}

void Engine::CRenderTargetMgr::Free()
{
	for_each(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CDeleteMap());
	m_mapRenderTarget.clear();

	for (auto& iter : m_mapMRT)
		iter.second.clear();

	m_mapMRT.clear();
}

