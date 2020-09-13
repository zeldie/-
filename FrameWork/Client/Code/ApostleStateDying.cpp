#include "stdafx.h"
#include "ApostleStateDying.h"

CApostleStateDying::CApostleStateDying()
{
}


CApostleStateDying::~CApostleStateDying()
{
}

HRESULT CApostleStateDying::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_DYING);

	m_dbAniCtrlTime = 0.;
	m_dbAnimationSpeed = 1.f;
	return S_OK;
}

void CApostleStateDying::Update_State(const _double & dTimeDelta)
{
}

CApostleStateDying * CApostleStateDying::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver * pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStateDying*	pInstance = new CApostleStateDying();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStateDying::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
