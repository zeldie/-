#include "TimerMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimerMgr)

Engine::CTimerMgr::CTimerMgr()
{

}

Engine::CTimerMgr::~CTimerMgr()
{
	Free();
}

HRESULT Engine::CTimerMgr::Ready_Timer(const wstring& wstrTimerTag)
{
	CTimer*		pInstance = Find_Timer(wstrTimerTag);

	if (nullptr != pInstance)
		return E_FAIL;

	pInstance = CTimer::Create();

	m_mapTimer.emplace(wstrTimerTag, pInstance);

	return S_OK;
}

void Engine::CTimerMgr::Free()
{
	for_each(m_mapTimer.begin(), m_mapTimer.end(), CDeleteMap());
	m_mapTimer.clear();
}

CTimer* Engine::CTimerMgr::Find_Timer(const wstring& wstrTimerTag)
{
	auto	iter = m_mapTimer.find(wstrTimerTag);

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}

_double CTimerMgr::Get_TimeDelta(const wstring & wstrTimerTag)
{
	CTimer*		pInstance = Find_Timer(wstrTimerTag);

	if (nullptr == pInstance)
		return 0;

	return pInstance->Get_TimeDelta();
}

void CTimerMgr::Set_TimeDelta(const wstring& wstrTimerTag)
{
	CTimer*		pInstance = Find_Timer(wstrTimerTag);

	if (nullptr == pInstance)
		return;

	pInstance->Set_TimeDelta();
}
