#pragma once

#include "PlayerState.h"
#include "CS_State.h"

class CTsState : public CPlayerState
{
public:
	explicit CTsState();
	virtual ~CTsState();

public:
	virtual void Enter(CPlayer* pPlayer);
	virtual void Update(CPlayer* pPlayer, const _double dTimeDelta) override;
	virtual void ChangeState(CPlayerState* pState) override;

private:
	//_bool					m_bIsReserve;			// Value for next state check

	_bool					m_bSecondRocBreak;		// Check it out skill Rockbreak
	_bool					m_bIsMinimumTime;		// BASICCOMBOEX2의 최소 동작 시간 체크

	//CPlayer::PLAYER_STATE	m_eReserveState;		// Value for reserve to next state

	_double					m_dbMotionCancel;		// Check time value for playing motion cancel
	_double					m_dbBasicLoopTime;		// Check time value for basiccombo2 finish
	_double					m_dbIsChargingTime;		// Minimum value to charging
	_double					m_dbWaitingStartMotion;	// Branch RB Attack- Charging or RockBreak
	_double					m_dbJumpTime;
	_double					m_dbPressingTime;

private:											// Const value
	const _double m_dbIsBasicCombo2FinishTime = 5;
};