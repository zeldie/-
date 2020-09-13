#pragma once

#include "Ai_PlayerState.h"

class CAi_CsState final : public CAi_PlayerState
{
	enum PLAYER_WEAPONTYPE { COMMON, DUALSWORD, TWOHANDSWORD, ORB, LONGBOW };

public:
	explicit CAi_CsState();
	virtual ~CAi_CsState();

public:
	virtual void Enter(CAi_Player* pPlayer);
	virtual void Update(CAi_Player* pPlayer, const _double dTimeDelta);
	virtual void ChangeState(CAi_PlayerState* pState);
	virtual void Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float* fCrossy, _vec3* vDir, _float* fDegree);

	// New
	virtual void			Enter_State(CBasePlayer* pPlayer);
	virtual void			Update_State(const _double dTimeDelta);
	CAi_Player::STATE_ID	LateUpadte_State(const _double dTimeDelta);

private:
	void JumpAction(const _double dTimeDelta);
	_bool GraoundAction(CAi_Player* pPlayer, const _double dTimeDelta);
	_bool Finish_Animation(CAi_Player* pPlayer);
private:
	// test�� ����
	_vec3				m_vTestPosW[2];
	_vec3				m_vTestPosA[2];
	_vec3				m_vTestPosS[2];
	_int				m_iTestW = 0;
	_int				m_iTestA = 0;
	_int				m_iTestS = 0;
	// test�� ����

	_float				m_fAccelerate;
	_float				m_fMaxSpeed;
	_float				m_fJumpMoveSpeed;

	_vec3				m_vRecordY;


	_bool				m_bIsCurve;
	_bool				m_bIsPeak;
	_bool				m_bIsStop;
	_bool				m_bIsRun;		// �޸��� ���¸� Ȯ���� �� �ִ� ��
	_bool				m_bIsJumpMove;

	_bool				m_bSpecialMove;

	_bool				m_bJumpMove;

	_double				m_dbJumpTime;
	_double				m_dbFall;
	_double				m_dbDash;
	_double				m_dbCaptureTime;

	// �޸��� ����Ʈ ���鶧 ���� ����
	_double				m_dDustTime;
};