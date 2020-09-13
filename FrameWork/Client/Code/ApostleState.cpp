#include "stdafx.h"
#include "ApostleState.h"


CApostleState::CApostleState()
	:m_pMeshCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pPlayerObserver(nullptr)
	, m_dbAniCtrlTime(0.)
	, m_dbAnimationSpeed(1.)
	, m_dbTime(0.)
	, m_iSkill(0)
{
}


CApostleState::~CApostleState()
{
}

void CApostleState::Free(void)
{
}
