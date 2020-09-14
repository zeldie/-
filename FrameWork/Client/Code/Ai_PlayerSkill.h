#pragma once
#include "Ai_PlayerState.h"

class CAi_PlayerSkill final : public CAi_PlayerState
{
public:
	enum SEQUENCE { SEQUENCE_1, SEQUENCE_2, SEQUENCE_3, SEQUENCE_END};
public:
	explicit CAi_PlayerSkill();
	virtual ~CAi_PlayerSkill();

public:
	virtual void Enter(CAi_Player* pPlayer);
	virtual void Update(CAi_Player* pPlayer, const _double dTimeDelta);
	virtual void ChangeState(CAi_PlayerState* pState);
	virtual void Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float* fCrossy, _vec3* vDir, _float* fDegree);

public:
	void					Enter_State(CBasePlayer* pPlayer) override;
	void					Update_State(const _double dTimeDelta) override;
	CAi_Player::STATE_ID	LateUpadte_State(const _double dTimeDelta) override;

private:
	void Update_PlayerPattern(const _double dTimeDelta);

private:
	//_bool m_bEnter;			// 이 상태에 처음 들어왔는지 체크
	//_bool m_bIsSkill;		// 상태 변경 시 스킬인지 Ex콤보인지

	_bool m_bReserveFinish;
	_bool m_bIsFinish;
	_bool m_bPass;

	_bool m_bLeapAtk;

	SEQUENCE m_eSequnce;
	
	_double m_dbPush;
	_double m_dbMinTime;
	_double m_dbLeapTime;
	_double m_dbKillingTime;

	_uint m_iCount;

	_vec3 vTargetPos;
	_vec3 vStartPos;
	_vec3 m_vOrbUltPos;

	_float fLength;

	//희정 추가 - 대검 ESkill용 
	_double		m_dbTS_ESkill_Time_Right;
	_double		m_dbTS_ESkill_Time_Left;
	_double		m_dbTS_Time;
	_bool		m_bTS_ESkill_CreateEffect_Right;
	_bool		m_bTS_ESkill_CreateEffect_Left;
	_bool		m_bTS_ESkill_Check_Right;
	_bool		m_bTS_ESkill_Check_Left;
	_uint		m_iTS_ESkill_Cnt;

	//아래는 활을 위한 변수들
	_double m_dArrowTerm;
	_double m_dDustTime;

	// DS...Tlqkf
	_bool		m_bDS_ESkill_Trail1;
	_bool		m_bDS_ESkill_Trail2;
	_bool		m_bDS_ESkill_Trail3;
	_bool		m_bDS_ESkill_Trail4;
	_bool		m_bDS_ESkill_Trail5;
	_bool		m_bDS_ESkill_Trail6;
};