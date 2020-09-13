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
	_double		m_dCallLimit;		// ���ѵ� �ð� ��
	_double		m_dAccTimeDelta;	// ������ �ð� �����ϱ� ���� ����

public:
	static CFrame*		Create(const _double& dCallLimit);
	virtual void		Free();
};

END
#endif // Frame_h__
