#include "stdafx.h"
#include "RunPlayerDown_End.h"


CRunPlayerDown_End::CRunPlayerDown_End()
{
}


CRunPlayerDown_End::~CRunPlayerDown_End()
{
}

HRESULT CRunPlayerDown_End::Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, BASE_INFO * pBaseInfo)
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

	m_pMeshCom->Set_AnimationSet(CRunPlayer::RunPlayerStateEnum::STATE_DOWN_LOOP);

	m_dbAniCtrlTime = 0.;
	m_dbAnimationSpeed = 1.f;
	return S_OK;
}

void CRunPlayerDown_End::Update_State(const _double & dTimeDelta)
{
}

CRunPlayerDown_End * CRunPlayerDown_End::Create(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, BASE_INFO * pBaseInfo)
{
	CRunPlayerDown_End*	pInstance = new CRunPlayerDown_End();
	if (FAILED(pInstance->Ready_State(pMeshCom, pTransformCom, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunPlayerDown_End::Free(void)
{
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pTransformCom);
}
