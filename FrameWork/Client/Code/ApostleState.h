#pragma once
#ifndef __CApostleState_h__
#define __CApostleState_h__
#include "Apostle.h"
#include "PlayerObserver.h"

//class CPlayerObserver;
class CApostleState : public CBase
{
protected:
	explicit CApostleState();
public:
	virtual ~CApostleState();

public:
	virtual HRESULT		Ready_State(Engine::CDynamicMesh * pMeshCom, Engine::CTransform * pTransformCom, CPlayerObserver* pPlayerObserver, BASE_INFO* pBaseInfo) = 0;
	virtual void		Update_State(const _double& dTimeDelta) = 0;
	virtual _double		Get_AniCtrlTime() { return m_dbAniCtrlTime; }
	virtual _double		Get_AniSpeed() { return m_dbAnimationSpeed; }

protected:
	Engine::CDynamicMesh*	m_pMeshCom;
	Engine::CTransform*		m_pTransformCom;
	CPlayerObserver*		m_pPlayerObserver;
	_double					m_dbAnimationSpeed;
	_double					m_dbAniCtrlTime;
	BASE_INFO				m_tBaseInfo;
	_double					m_dbTime;
	_int					m_iSkill;

public:
	virtual void Free(void) = 0;

};

#endif // !__CApostleState_h__
