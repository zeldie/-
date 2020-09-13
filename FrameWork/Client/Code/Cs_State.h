#pragma once

#include "PlayerState.h"
//#include "TS_State.h"
//#include "DS_State.h"

class CCsState final : public CPlayerState
{
	enum PLAYER_WEAPONTYPE { COMMON, DUALSWORD, TWOHANDSWORD, ORB, LONGBOW };

public:
	explicit CCsState();
	virtual ~CCsState();

public:
	virtual void Enter(CPlayer* pPlayer);
	virtual void Update(CPlayer* pPlayer, const _double dTimeDelta);
	virtual void ChangeState(CPlayerState* pState);
	virtual void Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float* fCrossy, _vec3* vDir, _float* fDegree);

	// New
	virtual void			Enter_State(CBasePlayer* pPlayer);
	virtual void			Update_State(const _double dTimeDelta);
	CPlayer::STATE_ID	LateUpadte_State(const _double dTimeDelta);

private:
	void JumpAction(const _double dTimeDelta);
	_bool GraoundAction(CPlayer* pPlayer, const _double dTimeDelta);
	_bool ChangeState_ATK(CPlayer* pPlayer);
	_bool Finish_Animation(CPlayer* pPlayer);
private:
	_float				m_fAccelerate;
	_float				m_fMaxSpeed;
	_float				m_fJumpMoveSpeed;

	_vec3				m_vRecordY;


	_bool				m_bIsCurve;
	_bool				m_bIsPeak;
	_bool				m_bIsStop;
	_bool				m_bIsRun;		// 달리는 상태를 확인할 수 있는 애
	_bool				m_bIsJumpMove;

	_bool				m_bSpecialMove;

	_bool				m_bJumpMove;

	_bool				m_bJumpSound;

	_double				m_dbJumpTime;
	_double				m_dbFall;
	_double				m_dbDash;
	_double				m_dbCaptureTime;
	_double				m_dbFlagCapture;

	// 달릴때 이펙트 만들때 쓰는 변수
	_double				m_dDustTime;

	//희정
	CUIMgr*					m_pUIMgr;

	// SoundTime
	_double m_dbRun=0.0;
};