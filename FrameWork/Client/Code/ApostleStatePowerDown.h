#pragma once
#include "ApostleState.h"
class CApostleStatePowerDown :
	public CApostleState
{

	CApostleStatePowerDown();
	virtual ~CApostleStatePowerDown();

public:
	virtual HRESULT Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo) override;
	virtual void	Update_State(const _double& dTimeDelta) override;

private:
	_uint		m_iPattern;
	_double		m_dbTime;

public:
	static CApostleStatePowerDown*	Create(Engine::CDynamicMesh* pMeshCom, Engine::CTransform* pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo);
	virtual void Free(void) override;
};

