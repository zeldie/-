#pragma once
#include "RunPlayerState.h"
class CRunPlayerDown_End : public CRunPlayerState
{
public:
	CRunPlayerDown_End();
	virtual ~CRunPlayerDown_End();

	// CRunPlayerState��(��) ���� ��ӵ�
	virtual HRESULT Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, BASE_INFO * pBaseInfo) override;
	virtual void Update_State(const _double & dTimeDelta) override;

public:
	static CRunPlayerDown_End*	Create(Engine::CDynamicMesh* pMeshCom, Engine::CTransform* pTransformCom, BASE_INFO* pBaseInfo);
	virtual void Free(void) override;
};

