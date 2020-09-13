#ifndef Component_h__
#define Component_h__

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	virtual _int	Update_Component(const _double& dTimeDelta);

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool				m_bClone;
public:
	virtual	CComponent*		Clone(void)PURE;

protected:
	virtual void Free();
};

END
#endif // Component_h__
