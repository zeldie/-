#include "stdafx.h"
#include "ApostleStateJumpTurnSpike.h"

CApostleStateJumpTurnSpike::CApostleStateJumpTurnSpike()
{
}


CApostleStateJumpTurnSpike::~CApostleStateJumpTurnSpike()
{
}

HRESULT CApostleStateJumpTurnSpike::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_SKILL_1_JUMP_TURN_SPIKE);

	m_dbAniCtrlTime = 0.2;
	m_dbAnimationSpeed = 1.f;


	_vec3	vAngle = m_pTransformCom->m_vAngle;
	vAngle.x = 90.f;
	CEffectMgr::GetInstance()->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_FAN_HALF, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 10.f, 0.f)), &vAngle, &m_tBaseInfo);

	return S_OK;
}

void CApostleStateJumpTurnSpike::Update_State(const _double & dTimeDelta)
{
	//_vec3	vAngle = m_pTransformCom->m_vAngle;
	//CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_47, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
	//vAngle.y -= 45.f;
	//CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_47, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
	//vAngle.y += 90.f;
	//CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_47, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);

}

CApostleStateJumpTurnSpike * CApostleStateJumpTurnSpike::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStateJumpTurnSpike*	pInstance = new CApostleStateJumpTurnSpike();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStateJumpTurnSpike::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
