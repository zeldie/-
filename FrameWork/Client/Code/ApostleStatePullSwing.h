#pragma once
#include "ApostleState.h"
class CApostleStatePullSwing :
	public CApostleState
{

	CApostleStatePullSwing();
	virtual ~CApostleStatePullSwing();

public:
	virtual HRESULT Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo) override;
	virtual void	Update_State(const _double& dTimeDelta) override;

public:
	static CApostleStatePullSwing*	Create(Engine::CDynamicMesh* pMeshCom, Engine::CTransform* pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo);
	virtual void Free(void) override;
};

