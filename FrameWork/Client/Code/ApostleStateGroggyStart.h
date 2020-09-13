#pragma once
#include "ApostleState.h"
class CApostleStateGroggyStart :
	public CApostleState
{

	CApostleStateGroggyStart();
	virtual ~CApostleStateGroggyStart();

public:
	virtual HRESULT Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo) override;
	virtual void	Update_State(const _double & dTimeDelta) override;
public:
	static CApostleStateGroggyStart*	Create(Engine::CDynamicMesh* pMeshCom, Engine::CTransform* pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo);
	virtual void Free(void) override;


};

