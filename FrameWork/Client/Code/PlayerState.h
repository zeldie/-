#pragma once

#include "Player.h"
#include "Calculator.h"
#include "Decal.h"
#include "Arrow.h"
class CEffectMgr;
class CBasePlayer;

class CPlayerState : public CBase
{
public:
	explicit CPlayerState();
	virtual ~CPlayerState();

public:
	virtual void Enter(CPlayer* pPlayer) PURE;
	virtual void Update(CPlayer* pPlayer, const _double dTimeDelta) PURE;
	virtual void ChangeState(CPlayerState* pState) PURE;
	
	// 카메라 방향 기준으로 플레이어를 회전하기 위한 연산을 진행하는 함수
	/*virtual */void Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _vec3 vMove, _float* fCrossy, _vec3* vDir, _float* fDegree);

	virtual void Calculate_FaillingSpeed(const _float& fPower, const _float& fAccel, const _float& fGravity, const _double& dJumpingTime, const _double& dbFall, _float* fSpeed);
	void Is_Attacked();
	void Attacked_End(CPlayer* pPlayer);


	// NewFunction
public:
	virtual void				Enter_State(CBasePlayer* pPlayer);
	virtual void				Update_State(const _double dTimeDelta);
	virtual CPlayer::STATE_ID	LateUpadte_State(const _double dTimeDelta);
	//

public:
	__forceinline virtual void Set_StartAction(Engine::STARTMOTION Button) { m_eStartMotion = Button; }
	__forceinline virtual void Set_Effect()								   { m_bEffect = true; }

	void						WeaponDissolve();
	void						RotationTarget();
	void						RotationCam();
	void						RotationTarget_Range(_int iChooseTarget);

	//인균
	_bool						Get_FindTarget() { return m_bFindTarget; }
protected:
	Engine::CDynamicMesh*			m_pMeshCom;				// Player Mesh
	Engine::CTransform*				m_pTransform;			// Player Transform
	Engine::STARTMOTION				m_eStartMotion;			// Check it out starting motion
	Engine::CNaviMesh*				m_pNaviCom;
	Engine::CCalculator*			m_pCalculatorCom;		//Ray_Picking
	
	_bool							m_bIsReserve;			// Value for next state check
	_bool							m_bCreateEffect;
	_bool							m_bDash;
	_bool							m_bIsAttacked;
	_bool							m_bEffect;
	_bool							m_bDoubleEffect;
	_bool							m_bActionZoom;
	_bool							m_bRotation;

	_ulong							m_dwNaviIndex;

	_float							m_fSpeed;				// Jump ~ failling speed
	_double							m_dbAccelTime;

	CPlayer::PLAYER_STATE			m_eReserveState;		// Value for reserve to next state
	CPlayer::PLAYER_ID				m_ePlayerID;

	CPlayer::PLAYER_WEAPONTYPE		m_eWeapon;
	CPlayer*						m_pPlayer;

	//CAiPlayer*					m_pAiPlayer;

	const BASE_INFO*				m_tBaseInfo;
	CEffectMgr*						m_pEffectMgr;

	//주위에 누구가 있을때 그 적의 위치
	_vec3							m_vTargetPos;
	_bool							m_bFindTarget;
public:
	virtual void Free(void) override;

	//public:
	//	static CPlayerState* Create();
};