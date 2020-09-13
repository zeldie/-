#include "stdafx.h"
#include "ApostleStatePowerSpike.h"

CApostleStatePowerSpike::CApostleStatePowerSpike()
{
}


CApostleStatePowerSpike::~CApostleStatePowerSpike()
{
}

HRESULT CApostleStatePowerSpike::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_SKILL_8_POWER_SPIKE);

	m_dbAniCtrlTime = 0.2;
	m_dbAnimationSpeed = 1.f;

	m_dbTime = g_dAccumulatedTime + 0.65f;


	return S_OK;
}

void CApostleStatePowerSpike::Update_State(const _double & dTimeDelta)
{
	if (m_iSkill != 0)
		return;
	if (m_dbTime < g_dAccumulatedTime)
	{
		++m_iSkill;

		_vec3 vAngle = m_pTransformCom->m_vAngle;
		CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_LASER1, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
		vAngle.y -= 45.f;
		CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_LASER1, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
		vAngle.y += 90.f;
		CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_LASER1, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
	}
}

CApostleStatePowerSpike * CApostleStatePowerSpike::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStatePowerSpike*	pInstance = new CApostleStatePowerSpike();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStatePowerSpike::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
