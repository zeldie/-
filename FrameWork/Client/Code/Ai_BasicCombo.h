#pragma once

#include "Ai_PlayerState.h"

class CAi_BasicCombo final : public CAi_PlayerState
{
public:
	enum END_TYPE { TYPE_RESERVE, TYPE_CHANGE, TYPE_END };
public:
	explicit CAi_BasicCombo();
	virtual ~CAi_BasicCombo();

public:
	virtual void Enter(CAi_Player* pPlayer);
	virtual void Update(CAi_Player* pPlayer, const _double dTimeDelta);
	virtual void ChangeState(CAi_PlayerState* pState);
	virtual void Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float* fCrossy, _vec3* vDir, _float* fDegree);

public:
	void			Enter_State(CBasePlayer* pPlayer) override;
	void			Update_State(const _double dTimeDelta) override;
	CAi_Player::STATE_ID	LateUpadte_State(const _double dTimeDelta) override;

private:
	void Update_PlayerPattern(const _double dTimeDelta);
	//__forceinline void ReSet_Effect() { m_bEnter = true; }
private:
	_bool m_bEnter;			// 이 상태에 처음 들어왔는지 체크
	_bool m_bIsSkill;		// 상태 변경 시 스킬인지 Ex콤보인지
	_bool m_bNextAtk;

	_bool m_bTripleEffect;

	_double					m_dbMotionCancel;		// Check time value for playing motion cancel
	//_double					m_dbCharging;

	END_TYPE m_eEndType;
};