#include "stdafx.h"
#include "ApostleStateSpinEnd.h"

CApostleStateSpinEnd::CApostleStateSpinEnd()
{
}


CApostleStateSpinEnd::~CApostleStateSpinEnd()
{
}

HRESULT CApostleStateSpinEnd::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	if (pMeshCom == nullptr)
		return E_FAIL;
	if (pTransformCom == nullptr)
		return E_FAIL;
	if (pPlayerObserver == nullptr)
		return E_FAIL;
	if (pBaseInfo == nullptr)
		return E_FAIL;

	m_pMeshCom = pMeshCom;
	m_pMeshCom->AddRef();

	m_pTransformCom = pTransformCom;
	m_pTransformCom->AddRef();

	m_pPlayerObserver = pPlayerObserver;
	m_pPlayerObserver->AddRef();

	m_tBaseInfo = *pBaseInfo;

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_SKILL_10_SPIN_END);

	m_dbAniCtrlTime = 0.2;
	m_dbAnimationSpeed = 1.f;
	return S_OK;
}

void CApostleStateSpinEnd::Update_State(const _double & dTimeDelta)
{

}

CApostleStateSpinEnd * CApostleStateSpinEnd::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStateSpinEnd*	pInstance = new CApostleStateSpinEnd();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStateSpinEnd::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}