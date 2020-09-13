#include "stdafx.h"
#include "ApostleStatePoint.h"

CApostleStatePoint::CApostleStatePoint()
{
}


CApostleStatePoint::~CApostleStatePoint()
{
}

HRESULT CApostleStatePoint::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_SKILL_4_POINT);

	m_dbAniCtrlTime = 0.25;
	m_dbAnimationSpeed = 0.5f;
	return S_OK;
}

void CApostleStatePoint::Update_State(const _double & dTimeDelta)
{
	//m_pTransformCom->Look_Target(m_pPlayerObserver->Get_Pos());
}


CApostleStatePoint * CApostleStatePoint::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStatePoint*	pInstance = new CApostleStatePoint();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStatePoint::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
