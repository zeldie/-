#pragma once
#include "ApostleState.h"
class CApostleStateGroggyLoop :
	public CApostleState
{

	CApostleStateGroggyLoop();
	virtual ~CApostleStateGroggyLoop();

public:
	virtual HRESULT Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo) override;
	virtual void	Update_State(const _double & dTimeDelta) override;
public:
	static CApostleStateGroggyLoop*	Create(Engine::CDynamicMesh* pMeshCom, Engine::CTransform* pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo);
	virtual void Free(void) override;


};

