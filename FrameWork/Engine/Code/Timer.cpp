#include "Timer.h"

USING(Engine)

Engine::CTimer::CTimer()
	:m_dTimeDelta(0)
{
	ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

Engine::CTimer::~CTimer()
{

}

HRESULT Engine::CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);			
	QueryPerformanceCounter(&m_LastTime);			
	QueryPerformanceCounter(&m_FixTime);			
	QueryPerformanceFrequency(&m_CpuTick);	

	return S_OK;
}

void Engine::CTimer::Set_TimeDelta()
{
	QueryPerformanceCounter(&m_FrameTime);	// 1400 - 1048

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}
	
	m_dTimeDelta = (_double)(m_FrameTime.QuadPart - m_LastTime.QuadPart) / (_double)m_CpuTick.QuadPart;

	m_LastTime = m_FrameTime;
}


CTimer* Engine::CTimer::Create()
{
	CTimer*	pInstance = new CTimer;

	if (FAILED(pInstance->Ready_Timer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CTimer::Free()
{

}



