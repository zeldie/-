#include "stdafx.h"
#include "RunPlayerWait.h"


CRunPlayerWait::CRunPlayerWait()
{
}


CRunPlayerWait::~CRunPlayerWait()
{
}

HRESULT CRunPlayerWait::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, BASE_INFO * pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CRunPlayer::RunPlayerStateEnum::STATE_WAIT);

	m_dbAniCtrlTime = 0.;
	m_dbAnimationSpeed = 1.f;
	return S_OK;
}

void CRunPlayerWait::Update_State(const _double & dTimeDelta)
{
}

CRunPlayerWait * CRunPlayerWait::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, BASE_INFO * pBaseInfo)
{
	CRunPlayerWait*	pInstance = new CRunPlayerWait();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunPlayerWait::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
}
