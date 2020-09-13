#ifndef Frame_h__
#define Frame_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFrame final : public CBase
{
private:
	explicit CFrame();
	virtual ~CFrame();
	
public:
	_bool		IsPermit_Call(const _double& dTimeDelta);

public:
	HRESULT		Ready_Frame(const _double& dCallLimit);

private:
	_double		m_dCallLimit;		// 제한된 시간 값
	_double		m_dAccTimeDelta;	// 누적된 시간 저장하기 위한 변수

public:
	static CFrame*		Create(const _double& dCallLimit);
	virtual void		Free();
};

END
#endif // Frame_h__
