#pragma once
#include "ApostleState.h"
class CApostleStateJumpHooray :
	public CApostleState
{

	CApostleStateJumpHooray();
	virtual ~CApostleStateJumpHooray();

public:
	virtual HRESULT Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo) override;
	virtual void	Update_State(const _double& dTimeDelta) override;

public:
	static CApostleStateJumpHooray*	Create(Engine::CDynamicMesh* pMeshCom, Engine::CTransform* pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo);
	virtual void Free(void) override;

private:
	float	m_fPosY;
};

