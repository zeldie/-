#include "stdafx.h"
#include "RunPlayerState.h"

CRunPlayerState::CRunPlayerState()
	:m_pMeshCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_dbAniCtrlTime(0.)
	, m_dbAnimationSpeed(1.)
	, m_dbTime(0.)
	, m_iSkill(0)
{
}

CRunPlayerState::~CRunPlayerState()
{
}

void CRunPlayerState::Free(void)
{
}
