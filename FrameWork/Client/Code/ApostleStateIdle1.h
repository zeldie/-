#pragma once
#include "ApostleState.h"
class CApostleStateIdle1 :
	public CApostleState
{

	CApostleStateIdle1();
	virtual ~CApostleStateIdle1();

public:
	virtual HRESULT Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo) override;
	virtual void	Update_State(const _double & dTimeDelta) override;

public:
	static CApostleStateIdle1*	Create(Engine::CDynamicMesh* pMeshCom, Engine::CTransform* pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo);
	virtual void Free(void) override;
};

