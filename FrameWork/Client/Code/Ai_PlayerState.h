#pragma once

#include "Ai_Player.h"
#include "Calculator.h"
#include "Decal.h"
#include "Arrow.h"

class CEffectMgr;
class CBasePlayer;

class CAi_PlayerState : public CBase
{
public:
	explicit CAi_PlayerState();
	virtual ~CAi_PlayerState();

public:
	virtual void Enter(CAi_Player* pPlayer) PURE;
	virtual void Update(CAi_Player* pPlayer, const _double dTimeDelta) PURE;
	virtual void ChangeState(CAi_PlayerState* pState) PURE;
	
	// 카메라 방향 기준으로 플레이어를 회전하기 위한 연산을 진행하는 함수
	/*virtual */void Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _vec3 vMove, _float* fCrossy, _vec3* vDir, _float* fDegree);

	virtual void Calculate_FaillingSpeed(const _float& fPower, const _float& fAccel, const _float& fGravity, const _double& dJumpingTime, const _double& dbFall, _float* fSpeed);
	void Is_Attacked();
	void Attacked_End(CAi_Player* pPlayer);


	// NewFunction
public:
	virtual void					Enter_State(CBasePlayer* pPlayer);
	virtual void					Update_State(const _double dTimeDelta);
	virtual CAi_Player::STATE_ID	LateUpadte_State(const _double dTimeDelta);

public:
	void							Set_StartAction(Engine::STARTMOTION Button) { m_eStartMotion = Button; }
	void							Set_Effect()								   { m_bEffect = true; }

	// Get
	INPUT_KEY						Get_InputKey() { return m_eDown_Key; }
	//PLAYER_STATE					Get_CurState() { return m_pAi_Player->Get_CurState(); }

	// Set
	void							Set_DownKey(INPUT_KEY	eKey) { m_eDown_Key = eKey; }
	void							Set_UpKey(INPUT_KEY	eKey) { m_eUp_Key = eKey; }
	void							Set_PressKey(INPUT_KEY ePressingKey) { m_ePress_Key = ePressingKey; }
	void							Set_AiState_isAttacked(_bool bValue) { m_bIsAttacked = bValue; }

	void							WeaponDissolve();

protected:
	Engine::CDynamicMesh*				m_pMeshCom;				// Player Mesh
	Engine::CTransform*					m_pTransform;			// Player Transform
	Engine::STARTMOTION					m_eStartMotion;			// Check it out starting motion
	Engine::CNaviMesh*					m_pNaviCom;
	Engine::CCalculator*				m_pCalculatorCom;		//Ray_Picking
	_bool								m_bIsReserve;			// Value for next state check
	_bool								m_bCreateEffect;
	_bool								m_bDash;
	_bool								m_bIsAttacked;
	_bool								m_bEffect;
	_bool								m_bDoubleEffect;

	_ulong								m_dwNaviIndex;

	_float								m_fSpeed;				// Jump ~ failling speed
	_double								m_dbAccelTime;

	CAi_Player::PLAYER_STATE			m_eReserveState;		// Value for reserve to next state
	CAi_Player::PLAYER_ID				m_ePlayerID;

	CAi_Player::PLAYER_WEAPONTYPE		m_eWeapon;
	CAi_Player*							m_pPlayer;

	//CAiPlayer*						m_pAiPlayer;

	const BASE_INFO*					m_tBaseInfo;
	CEffectMgr*							m_pEffectMgr;

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
	INPUT_KEY							m_eDown_Key = KEY_END;
	INPUT_KEY							m_eUp_Key = KEY_END;
	INPUT_KEY							m_ePress_Key = KEY_END;

	_bool								m_bDecalCreateOnce = false;

public:
	virtual void Free(void) override;

	//public:
	//	static CAi_PlayerState* Create();
};