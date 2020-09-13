#pragma once
#include "Ai_PlayerState.h"

class CAi_BasicComboEx final : public CAi_PlayerState
{
public:
	enum END_TYPE	{ TYPE_LOOP, TYPE_RESERVE, TYPE_END };
	enum STEP		{STEP1,STEP2,STEP3,STEP4,STEP_END};	

public:
	explicit CAi_BasicComboEx();
	virtual ~CAi_BasicComboEx();

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

private:
	_bool m_bEnter;			// 이 상태에 처음 들어왔는지 체크
	_bool m_bIsSkill;		// 상태 변경 시 스킬인지 Ex콤보인지
	_bool m_bNextAtk;

	_bool m_bReserveFinish;

	_bool m_bPass;

	_double m_dbHoldingTime; // 최소 유지 시간
	_double m_dbCharge;
	_double m_dbAcc;

	_uint m_iCount;

	END_TYPE m_eEndType;
	CAi_Player::ATK_LEVEL	m_eLevel;
	STEP m_eStep;
};