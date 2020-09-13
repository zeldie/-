#pragma once
#ifndef __CRunPlayerState_h__
#define __CRunPlayerState_h__
#include "RunPlayer.h"

class CRunPlayerState : public CBase
{
protected:
	explicit CRunPlayerState();
public:
	virtual ~CRunPlayerState();

public:
	virtual HRESULT		Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, BASE_INFO* pBaseInfo) = 0;
	virtual void		Update_State(const _double& dTimeDelta) = 0;
	virtual _double		Get_AniCtrlTime() { return m_dbAniCtrlTime; }
	virtual _double		Get_AniSpeed() { return m_dbAnimationSpeed; }

protected:
	Engine::CDynamicMesh*	m_pMeshCom;
	Engine::CTransform*		m_pTransformCom;
	_double					m_dbAnimationSpeed;
	_double					m_dbAniCtrlTime;
	BASE_INFO				m_tBaseInfo;
	_double					m_dbTime;
	_int					m_iSkill;

public:
	virtual void Free(void) = 0;

};

#endif // !__CApostleState_h__
