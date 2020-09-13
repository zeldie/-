#include "stdafx.h"
#include "RunPlayerIdle1.h"


CRunPlayerIdle1::CRunPlayerIdle1()
{
}


CRunPlayerIdle1::~CRunPlayerIdle1()
{
}

HRESULT CRunPlayerIdle1::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, BASE_INFO * pBaseInfo)
{
	if (pMeshCom == nullptr)
		return E_FAIL;
	if (pTransformCom == nullptr)
		return E_FAIL;
	if (pBaseInfo == nullptr)
		return E_FAIL;


	m_pMeshCom = pMeshCom;
	m_pMeshCom->AddRef();

	m_pTransformCom = pTransformCom;
	m_pTransformCom->AddRef();


	m_tBaseInfo = *pBaseInfo;

	m_pMeshCom->Set_AnimationSet(CRunPlayer::RunPlayerStateEnum::STATE_IDLE_1);

	m_dbAniCtrlTime = 0.;
	m_dbAnimationSpeed = 1.f;
	return S_OK;
}

void CRunPlayerIdle1::Update_State(const _double & dTimeDelta)
{
}

CRunPlayerIdle1 * CRunPlayerIdle1::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, BASE_INFO * pBaseInfo)
{
	CRunPlayerIdle1*	pInstance = new CRunPlayerIdle1();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunPlayerIdle1::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
}
