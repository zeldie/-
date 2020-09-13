#pragma once

#ifndef __SUBJECT_H__
#define __SUBJECT_H__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CObserver;
class CSubject : public CBase
{
private:
	explicit CSubject();
	virtual ~CSubject();

public:
	void Subscribe(CObserver* pObserver);
	void UnSubscribe(CObserver* pObserver);
	void Notify(int iMessage, void* pData);

public:
	static CSubject* Create();

private:
	typedef list<CObserver*>	LIST_OBSERVER;
	LIST_OBSERVER m_Observers;

public:
	virtual void Free();
};

END

#endif