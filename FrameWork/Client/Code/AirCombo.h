#pragma once
#include "PlayerState.h"

class CAirCombo final : public CPlayerState
{
public:
	enum END_TYPE { TYPE_CHANGE, TYPE_RESERVE, TYPE_END };
public:
	explicit CAirCombo();
	virtual ~CAirCombo();

public:
	virtual void Enter(CPlayer* pPlayer);
	virtual void Update(CPlayer* pPlayer, const _double dTimeDelta);
	virtual void ChangeState(CPlayerState* pState);
	virtual void Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float* fCrossy, _vec3* vDir, _float* fDegree);

public:
	void				Enter_State(CBasePlayer* pPlayer) override;
	void				Update_State(const _double dTimeDelta) override;
	CPlayer::STATE_ID	LateUpadte_State(const _double dTimeDelta) override;

public:
	void	TS_AirCombo(const _double dTimeDelta);
	void	DS_AirCombo(const _double dTimeDelta);
	void	OB_AirCombo(const _double dTimeDelta);
	void	LB_AirCombo(const _double dTimeDelta);

private:
	void Update_PlayerPattern(const _double dTimeDelta);

private:
	_bool m_bEnter;			// 이 상태에 처음 들어왔는지 체크
	_bool m_bIsSkill;		// 상태 변경 시 스킬인지 Ex콤보인지
	_bool m_bNextAtk;
	_bool m_bFaill;
	_bool m_bReserveFinish;
	_bool m_bLanding;
	_bool m_bPass;

	_bool m_bChange;

	_bool m_bCancel;

	_double m_dbHoldingTime; // 최소 유지 시간
	_double	m_dbMotionCancel;		// Check time value for playing motion cancel
	_double	m_dbJumpTime;
	_double m_dbAcc;
	_double m_dbDashTime;

	_int m_iRepeat;
	_uint m_iCount;

	END_TYPE m_eEndType;
	CPlayer::ATK_LEVEL	m_eLevel;	
};