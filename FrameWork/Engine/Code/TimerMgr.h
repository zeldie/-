#ifndef TimerMgr_h__
#define TimerMgr_h__

#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL CTimerMgr : public CBase
{
	DECLARE_SINGLETON(CTimerMgr)

private:
	explicit CTimerMgr();
	virtual ~CTimerMgr();

public:
	_double		Get_TimeDelta(const wstring& wstrTimerTag);
	void		Set_TimeDelta(const wstring& wstrTimerTag);

public:
	HRESULT		Ready_Timer(const wstring& wstrTimerTag);

private:
	CTimer*		Find_Timer(const wstring& wstrTimerTag);

private:
	map<wstring, CTimer*>		m_mapTimer;	
		
public:
	virtual void Free();
};

END
#endif // TimerMgr_h__
