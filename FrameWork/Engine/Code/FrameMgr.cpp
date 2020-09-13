#include "FrameMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrameMgr)

Engine::CFrameMgr::CFrameMgr()
{

}

Engine::CFrameMgr::~CFrameMgr()
{
	Free();
}

_bool Engine::CFrameMgr::IsPermit_Call(const wstring& wstrFrameTag, const _double& dTimeDelta)
{
	CFrame*	pFrame = Find_Frame(wstrFrameTag);
	if (pFrame == nullptr)
		return false;

	return pFrame->IsPermit_Call(dTimeDelta);
}

HRESULT Engine::CFrameMgr::Ready_Frame(const wstring& wstrFrameTag, const _double& dCallLimit)
{
	CFrame*	pFrame = Find_Frame(wstrFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(dCallLimit);

	m_mapFrame.emplace(wstrFrameTag, pFrame);

	return S_OK;
}

CFrame* Engine::CFrameMgr::Find_Frame(const wstring& wstrFrameTag)
{
	auto	iter = m_mapFrame.find(wstrFrameTag);

	if (iter == m_mapFrame.end())
		return nullptr;

	return iter->second;
}

void Engine::CFrameMgr::Free()
{
	for_each(m_mapFrame.begin(), m_mapFrame.end(), CDeleteMap());
	m_mapFrame.clear();
}

