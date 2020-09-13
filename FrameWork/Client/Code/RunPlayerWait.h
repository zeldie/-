#pragma once
#include "RunPlayerState.h"
class CRunPlayerWait : public CRunPlayerState
{
public:
	CRunPlayerWait();
	virtual ~CRunPlayerWait();

	// CRunPlayerState��(��) ���� ��ӵ�
	virtual HRESULT Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, BASE_INFO * pBaseInfo) override;
	virtual void Update_State(const _double & dTimeDelta) override;

public:
	static CRunPlayerWait*	Create(Engine::CDynamicMesh* pMeshCom, Engine::CTransform* pTransformCom, BASE_INFO* pBaseInfo);
	virtual void Free(void) override;
};

