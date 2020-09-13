#pragma once

#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CObserver : public CBase
{
protected:
	explicit CObserver();
	virtual ~CObserver();

public:
	virtual void Update(int iMessage, void* pData) PURE;

public:
	virtual void Free();
};

END

#endif

