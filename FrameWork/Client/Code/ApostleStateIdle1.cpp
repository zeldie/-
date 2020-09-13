#include "stdafx.h"
#include "ApostleStateIdle1.h"

CApostleStateIdle1::CApostleStateIdle1()
{
}


CApostleStateIdle1::~CApostleStateIdle1()
{
}

HRESULT CApostleStateIdle1::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_IDLE_1);

	m_dbAniCtrlTime = 0.;
	m_dbAnimationSpeed = 1.f;
	return S_OK;
}

void CApostleStateIdle1::Update_State(const _double & dTimeDelta)
{
	//m_pTransformCom->Look_Target(m_pPlayerObserver->Get_Pos());
}


CApostleStateIdle1 * CApostleStateIdle1::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStateIdle1*	pInstance = new CApostleStateIdle1();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStateIdle1::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
