#include "Frame.h"

USING(Engine)

Engine::CFrame::CFrame()
	: m_dCallLimit(0)
	, m_dAccTimeDelta(0)
{

}

Engine::CFrame::~CFrame()
{

}

_bool Engine::CFrame::IsPermit_Call(const _double& dTimeDelta)
{
	m_dAccTimeDelta += dTimeDelta;

	if (m_dAccTimeDelta >= m_dCallLimit)
	{
		m_dAccTimeDelta = 0;
		return true;
	}

	return false;
}

HRESULT Engine::CFrame::Ready_Frame(const _double& dCallLimit)
{
	m_dCallLimit = 1 / dCallLimit;

	return S_OK;
}

CFrame* Engine::CFrame::Create(const _double& dCallLimit)
{
	CFrame*		pInstance = new CFrame;

	if (FAILED(pInstance->Ready_Frame(dCallLimit)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CFrame::Free()
{

}

