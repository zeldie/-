#include "stdafx.h"
#include "ApostleStateJumpHooray.h"

CApostleStateJumpHooray::CApostleStateJumpHooray()
	:m_fPosY(0.f)
{
}


CApostleStateJumpHooray::~CApostleStateJumpHooray()
{
}

HRESULT CApostleStateJumpHooray::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CApostle::ApostleStateEnum::STATE_SKILL_11_JUMP_HOORAY);

	m_dbAniCtrlTime = 0.1;
	m_dbAnimationSpeed = 1.f;

	m_dbTime = g_dAccumulatedTime;

	return S_OK;
}

void CApostleStateJumpHooray::Update_State(const _double & dTimeDelta)
{
	if (m_dbTime < g_dAccumulatedTime)
	{
		m_dbTime = g_dAccumulatedTime + 0.4f;

		_vec3	vAngle = m_pTransformCom->m_vAngle;
		vAngle.x = 90.f;
		_vec3 vPos = m_pPlayerObserver->Get_Pos();
		vPos.y += ++m_fPosY;
		CEffectMgr::GetInstance()->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_SPHERE_GOLD, &vPos, &vAngle, &m_tBaseInfo);
	}
}

CApostleStateJumpHooray * CApostleStateJumpHooray::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo)
{
	CApostleStateJumpHooray*	pInstance = new CApostleStateJumpHooray();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pPlayerObserver, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CApostleStateJumpHooray::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pPlayerObserver);
}
