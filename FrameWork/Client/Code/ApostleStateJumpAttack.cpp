#include "stdafx.h"
#include "ApostleStateJumpAttack.h"

CApostleStateJumpAttack::CApostleStateJumpAttack()
{
}


CApostleStateJumpAttack::~CApostleStateJumpAttack()
{
}

HRESULT CApostleStateJumpAttack::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_SKILL_7_JUMP_ATTACK);

	m_dbAniCtrlTime = 0.2;
	m_dbAnimationSpeed = 1.f;
	m_dbTime = g_dAccumulatedTime + 0.3f;
	return S_OK;
}

void CApostleStateJumpAttack::Update_State(const _double & dTimeDelta)
{
	//CEffectMgr::GetInstance()->Create_Effect(Engine::EFFECTTYPE::EFFECT_47, m_pTransformCom->Get_WorldMatrix(), &(m_pTransformCom->m_vAngle), m_tBaseInfo);

	if (m_iSkill != 0)
		return;

	if (m_dbTime < g_dAccumulatedTime)
	{
		++m_iSkill;

		_vec3	vAngle = m_pTransformCom->m_vAngle;
		vAngle.x = 90.f;

		for (int i = 0; i < 10; i++)
		{
			_vec3 vPos = m_pTransformCom->m_vInfo[Engine::INFO_POS];
			vPos.y = 85.f + (_float)i;
			vPos.x += (rand() % 1500) - 750.f;
			vPos.z += (rand() % 1500) - 750.f;
			CEffectMgr::GetInstance()->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_SPHERE_GOLD2, &vPos, &vAngle, &m_tBaseInfo);
		}
	}
}

CApostleStateJumpAttack * CApostleStateJumpAttack::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStateJumpAttack*	pInstance = new CApostleStateJumpAttack();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStateJumpAttack::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
