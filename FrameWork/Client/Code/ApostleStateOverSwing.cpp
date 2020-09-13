#include "stdafx.h"
#include "ApostleStateOverSwing.h"

CApostleStateOverSwing::CApostleStateOverSwing()
{
}


CApostleStateOverSwing::~CApostleStateOverSwing()
{
}

HRESULT CApostleStateOverSwing::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_SKILL_3_OVERSWING);

	m_dbAniCtrlTime = 0.1;
	m_dbAnimationSpeed = 1.f;

	_vec3	vAngle = m_pTransformCom->m_vAngle;
	vAngle.x = 90.f;
	CEffectMgr::GetInstance()->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_FAN_BIG, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 10.f, 0.f)), &vAngle, &m_tBaseInfo);
	m_dbTime = g_dAccumulatedTime + 2.75f;

	return S_OK;
}

void CApostleStateOverSwing::Update_State(const _double & dTimeDelta)
{
	if (m_iSkill != 0)
		return;

	if (m_dbTime < g_dAccumulatedTime)
	{
		++m_iSkill;
		CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_TRAIL1, m_pTransformCom->Get_WorldMatrix(), &m_pTransformCom->m_vAngle, m_tBaseInfo);
		CSoundMgr::Get_Instance()->SiwonSoundOn(2);
	}
	//_vec3	vAngle = m_pTransformCom->m_vAngle;
	//CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_47, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
	//vAngle.y -= 30.f;
	//CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_47, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
	//vAngle.y += 60.f;
	//CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_47, m_pTransformCom->Get_WorldMatrix(), &vAngle, m_tBaseInfo);
}

CApostleStateOverSwing * CApostleStateOverSwing::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStateOverSwing*	pInstance = new CApostleStateOverSwing();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStateOverSwing::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
