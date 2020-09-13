#pragma once

#include "BasePlayer.h"

class CAi_PlayerState;

class CAi_Player :public CBasePlayer
{
public:
	enum BATTLETYPE {TYPE_BATTLE,TYPE_IDLE,TYPE_END};
	enum DEAD_STEP { DEAD1, DEAD2, DEAD3, DEAD_END };
private:
	explicit CAi_Player(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAi_Player();

public:
	_vec3				GetPlayerPos();
public:
	virtual HRESULT		Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, CBasePlayer::PLAYER_ID eID);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;
	virtual void		Render_PostEffect(const _double& dTimeDelta) override;
	virtual void		Render_Shadow(const _double& dTimeDelta) override;
	void				LateInit();

	// Get_Function
public:
	Engine::CDynamicMesh*		Get_PlayerMesh()		{ return m_pDynamicMeshCom; }
	PLAYER_STATE				Get_CurState()			{ return m_eCurState; }
	Engine::CTransform*			Get_PlayerTransform()	{ return m_pTransformCom; }
	Engine::CDynamicMesh*		Get_PlayerWeaponCom()	{ return m_pWeaponCom; }
	Engine::CCalculator*		Get_CalculatorCom()		{ return m_pCalculatorCom; }
	
	const _float&				Get_Speed() const		{ return m_fPlayerSpeed; }
	const _float&				Get_Accelerate() const	{ return m_fAccelerate; }
	const _float&				Get_MaxSpeed() const	{ return m_fMaxSpeed; }
	
	const _double&				Get_JumpTime()			{ return m_dbJumpTime; }
	
	Engine::CNaviMesh*			Get_NaviCom()			{ return m_pNaviMeshCom; }
	
	_bool						Get_Jump()				{ return m_bIsJump; }
	_bool						Get_Fall()				{ return m_bIsFall; }
	_bool						Get_isDualSword_Hide()  { if (dynamic_cast<CDualSword*>(m_pDualSword_R)->Get_isHide()) { if (dynamic_cast<CDualSword*>(m_pDualSword_L)->Get_isHide()) { return true; } } return false; }

	const _matrix				Get_ViewMatrix() const;
	
	_int						Get_Stamina()			{ return m_iStamina; }
	_float						Get_Mp()				{ return m_iMp; }

	_int						Get_AirComboLevel()		{ return m_iAirComboLevel; }
	_bool						Get_Tired()				{ return m_bIsTired; }

	Engine::STARTMOTION			Get_ReserveAction()		{ return m_eReserveAtk; }
	 _double					Get_Rage()				{ return m_dbRage; }

	CDualSword*					Get_DualSword_R()		{ return m_pDualSword_R; }
	CDualSword*					Get_DualSword_L()		{ return m_pDualSword_L; }

	_bool						Get_Dash()				{ return m_bReserveDash; }

	COLLISIONID					Get_CollisionID()		{ return m_eCollisionID; }


	// Set_Function
public:
	void						Set_State(PLAYER_STATE eState)							{ m_eCurState = eState; }
	void						Set_ReserveAction(Engine::STARTMOTION eReserveKey)		{ m_eReserveAtk = eReserveKey; }
	void						Set_AnimationSpeed(const _double& dbAnimatoinSpeed)		{ m_dbAnimationSpeed = dbAnimatoinSpeed; }
	
	void						Set_Jump_Start()				    { m_bIsJump = true; }
	void						Set_Jump_Finish()				    { m_bIsJump = false; }
	void						Set_Fall_Start()				    { m_bIsFall = true; }
	void						Set_Fall_End()					    { m_bIsFall = false; }
	void						Set_JumpTime(_double& dbJumptime)   { m_dbJumpTime = dbJumptime; }
	void						Set_Index(_ulong& dwIndex)		    { m_dwNaviIndex = dwIndex; }
public:
	void						Set_DualSowrd_Hide(_bool bValue)	{ if (m_eMainWeapon == DUALSWORD) { m_pDualSword_R->Set_Hide(bValue); m_pDualSword_L->Set_Hide(bValue); }}
	void						Set_BattleType(BATTLETYPE eType)	{ m_eBattleType = eType; }
	void						Set_Stamina(_int iStamina)			{ m_iStamina -= iStamina; }
	void						Set_Mp(_float iMp)					{ m_iMp -= iMp; }
	void						Set_SuperAmmor(_int iSuperAmmor)	{ m_tBaseInfo.iSuperAmmor = iSuperAmmor; }

	void						Set_AirComboLevel(_int iLevel)		{ m_iAirComboLevel = iLevel; }

	void						Reset_SuperAmmor()			{ m_tBaseInfo.iSuperAmmor = m_iBasicSuperArmor; }
	void						Set_OrbAnimation(COrb::ORB_STATE eState) { m_pOrb->Set_OB_Animation(eState); }
	void						Set_Rage(_double dbRage)	{ m_dbRage -= dbRage; }
	void						Set_DashReserve()			{ m_bReserveDash = true; }
	void						ReSet_DashReserve()			{ m_bReserveDash = false; }
	void						ReSet_CollisionID()			{ m_eCollisionID = COLLISION_END; }
	void						Set_Heart()					{ m_bIsAttacked = false; }

	// 충돌
public:
	virtual	void						Set_Collision(CBaseObject* pObj, COLLIDER eColliderType);
private:
	void								Set_Collision_Effect(CBaseObject* pObj);
private:
	virtual HRESULT						Clone_Component() override;
	HRESULT								Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	// 상태 업데이트
	virtual void						Change_State() override;
	void								Update_Status(const _double dTimeDelta);

	void								Dead_Player(const _double& dTimeDelta);

public:
		void							Change_Weapon();
		void							Create_SubWeapon();

public:
	//옵저버 패스
	void								ObserverPass();

public:
	// 상태 변화 체크 (외부 키 입력/애니메이션 종료/피격 등에 의해 상태 변경)	
	virtual void						ExChange_State(CAi_PlayerState* pState, _bool bAtk = false);



	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Get
	wstring								Get_NameMainWeapon() { return m_wstrName_MainWeapon; }
	wstring								Get_NameDualSword_L() { return m_wstrName_DualSword_L; }

	// Set
	void								Set_AiName(wstring wstrName) { m_wstrAiName = wstrName; }
	void								Set_CtrlType(CONTROLTYPE eCtrlType) { m_eCtrlType = eCtrlType; }
	void								Set_WeaponType();
	void								Set_WeaponName();
	void								Set_isAttacked(_bool bValue) { m_bIsAttacked = bValue; }



	CAi_PlayerState*					Get_AiState() { return m_pAiState; }


	void								Change_StateState();
	void								Check_State();

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

private:
	COLLISIONID				m_eCollisionID;
	Engine::STARTMOTION		m_eReserveAtk;
	Engine::CDynamicMesh*	m_pWeaponCom;
	//Engine::CDynamicMesh*	m_pDualSword_L;		// 듀얼소드 왼손전용

	CDynamicMeshObject*		m_pWeapon;

	//CDualSword*				m_pDualSword_R;
	//CDualSword*				m_pDualSword_L;

	//COrb*					m_pOrb;
	//CTwohandSword*			m_pTwoHandSword;

	BATTLETYPE				m_eBattleType;
	map<EFFECTID, _double>	m_mapinvincibility;

	_bool					m_bLateInit;
	_bool					m_bIsJump;				// Checking jump
	_bool					m_bIsFall;
	_bool					m_bIsAttacked;
	_bool					m_bReserveDash;

	_float					m_fPlayerSpeed;
	_float					m_fRecordY;				// 점프 전 높이를 저장하는 y값

	_double					m_dbAnimationSpeed;
	_double					m_dbJumpTime;
	_double					m_dbRecoveryTime;
	_double					m_bTiredTime;
	_double					m_dbDeadTime;

	_int					m_iStamina;
	_int					m_iMaxStamina;
	_float					m_iMp;
	_float					m_iMaxMp;
	
	_int					m_iCurHp;
	_int					m_iDifHp;
	
	_int					m_iPreHp;
	_int					m_iDifPreHp;

	_int					m_iAirComboLevel;	

	// 임시 변수
	_bool m_bBreak;
	_bool m_bIsTired;
	_int m_iBasicSuperArmor;
	_bool m_bCritical;
	//
	_bool m_bTest=false;
	_vec3 m_vTargetPos;
	DEAD_STEP m_eDeadStep;

	_vec3 vStartingPoint;


	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
private: // Const Value
	const _float			m_fAccelerate;
	const _float			m_fMaxSpeed;



	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
private:
	wstring					m_wstrAiName;			// 자신의 객체이름
	wstring					m_wstrName_MainWeapon;
	wstring					m_wstrName_SubWeapon;
	wstring					m_wstrName_DualSword_L;

	_bool					m_bisCaptureFlag;		// 점령상태
	CBasePlayer*			m_pCheck_TargetPlayer = nullptr;
	_float					m_fCheckTime = 0.f;

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	Engine::CTransform* m_pPlayerTransform;
public:
	static CAi_Player* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, PLAYER_ID eID, CONTROLTYPE eCtrlType, wstring wstrAiName);
	virtual void	Free();

};

