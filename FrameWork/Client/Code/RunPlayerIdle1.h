#pragma once
#include "RunPlayerState.h"
class CRunPlayerIdle1 : public CRunPlayerState
{
public:
	CRunPlayerIdle1();
	virtual ~CRunPlayerIdle1();

	// CRunPlayerState��(��) ���� ��ӵ�
	virtual HRESULT Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, BASE_INFO * pBaseInfo) override;
	virtual void Update_State(const _double & dTimeDelta) override;

public:
	static CRunPlayerIdle1*	Create(Engine::CDynamicMesh* pMeshCom, Engine::CTransform* pTransformCom, BASE_INFO* pBaseInfo);
	virtual void Free(void) override;
};

