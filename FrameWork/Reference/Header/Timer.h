#ifndef Timer_h__
#define Timer_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer final : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer();

public:
	void				Set_TimeDelta();
	_double				Get_TimeDelta() { return m_dTimeDelta; }

public:
	HRESULT				Ready_Timer();

private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_CpuTick;

	_double					m_dTimeDelta;

public:
	static CTimer*	Create();
	virtual void	Free();

};
END
#endif // Timer_h__
