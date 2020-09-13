#include "stdafx.h"
#include "ApostleStatePowerDown.h"

CApostleStatePowerDown::CApostleStatePowerDown()
	:m_iPattern(0)
{
}


CApostleStatePowerDown::~CApostleStatePowerDown()
{
}

HRESULT CApostleStatePowerDown::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_SKILL_12_POWER_DOWN);

	m_dbAniCtrlTime = 0.2;
	m_dbAnimationSpeed = 1.f;

	m_iPattern = 1;
	m_dbTime = g_dAccumulatedTime;

	return S_OK;
}

void CApostleStatePowerDown::Update_State(const _double & dTimeDelta)
{
	if (m_dbTime < g_dAccumulatedTime)
	{
		m_dbTime = g_dAccumulatedTime + 0.4f;

		if (m_iPattern == 1)
		{
			++m_iPattern;

			_vec3	vAngle = m_pTransformCom->m_vAngle;
			for (int i = 0; i < 6; i++)
			{
				CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_SPHERE_2, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
				vAngle.y += 60.f;
			}
		}
		else if (m_iPattern == 2)
		{
			++m_iPattern;

			_vec3	vAngle = m_pTransformCom->m_vAngle;
			vAngle.y -= 30.f;
			for (int i = 0; i < 6; i++)
			{
				CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_SPHERE_2, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
				vAngle.y += 60.f;
			}
		}
		else if(m_iPattern ==3)
		{
			m_iPattern = 1;

			_vec3	vAngle = m_pTransformCom->m_vAngle;
			for (int i = 0; i < 8; i++)
			{
				CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_SPHERE_2, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
				vAngle.y += 45.f;
			}
		}
	}
}

CApostleStatePowerDown * CApostleStatePowerDown::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStatePowerDown*	pInstance = new CApostleStatePowerDown();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStatePowerDown::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
