#include "stdafx.h"
#include "ApostleStatePullSwing.h"

CApostleStatePullSwing::CApostleStatePullSwing()
{
}


CApostleStatePullSwing::~CApostleStatePullSwing()
{
}

HRESULT CApostleStatePullSwing::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_SKILL_0_PULLSWING);

	m_dbAniCtrlTime = 0.;
	m_dbAnimationSpeed = 1.f;

	_vec3	vAngle = m_pTransformCom->m_vAngle;
	vAngle.x = 90.f;
	CEffectMgr::GetInstance()->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_FAN_TRIANGLE, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 10.f, 0.f)), &vAngle, &m_tBaseInfo);


	//CEffectMgr::GetInstance()->Create_FanEffect(FANSHAPE::FAN_TRIANGLE, m_pTransformCom->Get_WorldMatrix(), &(m_pTransformCom->m_vAngle), m_tBaseInfo, 1.4f);
	//_vec3	vAngle = m_pTransformCom->m_vAngle;
	//CEffectMgr::GetInstance()->Create_FanEffect(FANSHAPE::FAN_TRIANGLE, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo, 1.f);
	//vAngle.y += 120.f;
	//CEffectMgr::GetInstance()->Create_FanEffect(FANSHAPE::FAN_TRIANGLE, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo, 1.f);
	//vAngle.y += 120.f;
	//CEffectMgr::GetInstance()->Create_FanEffect(FANSHAPE::FAN_TRIANGLE, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo, 1.f);

	return S_OK;
}

void CApostleStatePullSwing::Update_State(const _double & dTimeDelta)
{
}

CApostleStatePullSwing * CApostleStatePullSwing::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStatePullSwing*	pInstance = new CApostleStatePullSwing();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStatePullSwing::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
