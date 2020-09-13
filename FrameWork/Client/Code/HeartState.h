#pragma once
#include "PlayerState.h"

class CHeartState final : public CPlayerState
{
public:
	explicit CHeartState();
	virtual ~CHeartState();

public:
	virtual void Enter(CPlayer* pPlayer);
	virtual void Update(CPlayer* pPlayer, const _double dTimeDelta);
	virtual void ChangeState(CPlayerState* pState);
	virtual void Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float* fCrossy, _vec3* vDir, _float* fDegree);

public:
	void				Enter_State(CBasePlayer* pPlayer) override;
	void				Update_State(const _double dTimeDelta) override;
	CPlayer::STATE_ID	LateUpadte_State(const _double dTimeDelta) override;

private:
	void Update_PlayerPattern(const _double dTimeDelta);
	void Key_Check();
	void Airborne(const _double dTimeDelta);

private:
	COLLISIONID m_eCollID;
	
	_vec3		m_vTargetPos;
	_vec3		m_vPurpose;

	_bool		m_bAvoid;
	_bool		m_bRotationAvoid;
	_bool		m_bRotation;
	_bool		m_bKnockBack;
	_bool		m_bJumpHit;

	_float		m_fAngle;

	_double		m_dbKnockBackTime;
};