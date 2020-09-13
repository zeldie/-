#include "stdafx.h"
#include "ApostleStateSpinLoop.h"

CApostleStateSpinLoop::CApostleStateSpinLoop()
{
}


CApostleStateSpinLoop::~CApostleStateSpinLoop()
{
}

HRESULT CApostleStateSpinLoop::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_SKILL_10_SPIN_LOOP);

	m_dbAniCtrlTime = 0.1;
	m_dbAnimationSpeed = 1.f;
	return S_OK;
}

void CApostleStateSpinLoop::Update_State(const _double & dTimeDelta)
{
	if ((rand() % 2) == 0)
	{
		_vec3	vAngle = m_pTransformCom->m_vAngle;
		vAngle.y -= (rand() % 361) * 1.f;
		CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_SPHERE_1, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
	}
}

CApostleStateSpinLoop * CApostleStateSpinLoop::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStateSpinLoop*	pInstance = new CApostleStateSpinLoop();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStateSpinLoop::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
