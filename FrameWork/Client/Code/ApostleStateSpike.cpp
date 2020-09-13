#include "stdafx.h"
#include "ApostleStateSpike.h"

CApostleStateSpike::CApostleStateSpike()
{
}


CApostleStateSpike::~CApostleStateSpike()
{
}

HRESULT CApostleStateSpike::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_SKILL_5_SPIKE);

	m_dbAniCtrlTime = 0.1;
	m_dbAnimationSpeed = 1.f;

	m_dbTime = g_dAccumulatedTime;

	return S_OK;
}

void CApostleStateSpike::Update_State(const _double & dTimeDelta)
{
	if (m_iSkill < 0)
		return;

	if (m_dbTime + 0.9f <= g_dAccumulatedTime)
	{
		m_iSkill = -1;
		_vec3	vAngle = m_pTransformCom->m_vAngle;
		_float	fAddAngle = 2.f;
		vAngle.y -= 10.f;
		for (int i = 0; i < 170; ++i)
		{
			vAngle.y -= fAddAngle;
			CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_SPHERE_1, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);

		}
	}

}

CApostleStateSpike * CApostleStateSpike::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStateSpike*	pInstance = new CApostleStateSpike();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStateSpike::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
