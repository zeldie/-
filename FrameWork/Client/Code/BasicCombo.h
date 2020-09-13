#pragma once

#include "PlayerState.h"

class CBasicCombo final : public CPlayerState
{
public:
	enum END_TYPE { TYPE_RESERVE, TYPE_CHANGE, TYPE_END };
public:
	explicit CBasicCombo();
	virtual ~CBasicCombo();

public:
	virtual void Enter(CPlayer* pPlayer);
	virtual void Update(CPlayer* pPlayer, const _double dTimeDelta);
	virtual void ChangeState(CPlayerState* pState);
	virtual void Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float* fCrossy, _vec3* vDir, _float* fDegree);

public:
	void			Enter_State(CBasePlayer* pPlayer) override;
	void			Update_State(const _double dTimeDelta) override;
	CPlayer::STATE_ID	LateUpadte_State(const _double dTimeDelta) override;

private:
	void Update_PlayerPattern(const _double dTimeDelta);
	//__forceinline void ReSet_Effect() { m_bEnter = true; }
private:
	_bool m_bEnter;			// 이 상태에 처음 들어왔는지 체크
	_bool m_bIsSkill;		// 상태 변경 시 스킬인지 Ex콤보인지
	_bool m_bNextAtk;

	_bool m_bTripleEffect;

	_double					m_dbMotionCancel;		// Check time value for playing motion cancel
	_double					m_dbCharging;

	END_TYPE m_eEndType;
};