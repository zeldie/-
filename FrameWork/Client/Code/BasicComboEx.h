#pragma once
#include "PlayerState.h"

class CBasicComboEx final : public CPlayerState
{
public:
	enum END_TYPE	{ TYPE_LOOP, TYPE_RESERVE, TYPE_END };
	enum STEP		{STEP1,STEP2,STEP3,STEP4,STEP_END};	

public:
	explicit CBasicComboEx();
	virtual ~CBasicComboEx();

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

private:
	_bool m_bEnter;			// 이 상태에 처음 들어왔는지 체크
	_bool m_bIsSkill;		// 상태 변경 시 스킬인지 Ex콤보인지
	_bool m_bNextAtk;

	_bool m_bReserveFinish;

	_bool m_bPass;

	_bool m_bJumpReserve;
	_bool m_bLBDraw;

	_double m_dbHoldingTime; // 최소 유지 시간
	_double m_dbCharge;
	_double m_dbAcc;
	_double m_dbLoopTime;

	_uint m_iCount;

	END_TYPE m_eEndType;
	CPlayer::ATK_LEVEL	m_eLevel;
	STEP m_eStep;
};