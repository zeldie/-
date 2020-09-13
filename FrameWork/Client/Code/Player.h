#pragma once

#include "BasePlayer.h"

// Ai_Player(Enemy / Aliance)와 실제 플레이할 캐릭터에 사용할 부모 클레스
class CPlayerState;
class CPlayer :public CBasePlayer
{
public:
	enum BATTLETYPE {TYPE_BATTLE,TYPE_IDLE,TYPE_END};
	enum DEAD_STEP { DEAD1, DEAD2, DEAD3, DEAD_END };
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	_vec3	GetPlayerPos();
public:
	virtual HRESULT		Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, PLAYER_ID eID);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;
	virtual void		Render_PostEffect(const _double& dTimeDelta) override;
	virtual void		Render_Shadow(const _double& dTimeDelta) override;
	void				LateInit();

	// Get_Function
public:
	__forceinline Engine::CDynamicMesh*		Get_PlayerMesh()		{ return m_pDynamicMeshCom; }
	__forceinline PLAYER_STATE				Get_CurState()			{ return m_eCurState; }
	__forceinline Engine::CTransform*		Get_PlayerTransform()	{ return m_pTransformCom; }
	__forceinline Engine::CDynamicMesh*		Get_PlayerWeaponCom() { return m_pWeaponCom; }
	__forceinline Engine::CCalculator*		Get_CalculatorCom() { return m_pCalculatorCom; }
	
	__forceinline const _float&				Get_Speed() const		{ return m_fPlayerSpeed; }
	__forceinline const _float&				Get_Accelerate() const	{ return m_fAccelerate; }
	__forceinline const _float&				Get_MaxSpeed() const	{ return m_fMaxSpeed; }
	
	__forceinline const _double&			Get_JumpTime()			{ return m_dbJumpTime; }
	
	__forceinline Engine::CNaviMesh*		Get_NaviCom()			{ return m_pNaviMeshCom; }
	
	__forceinline _bool						Get_Jump()				{ return m_bIsJump; }
	__forceinline _bool						Get_Fall()				{ return m_bIsFall; }
	__forceinline _bool						Get_isDualSword_Hide()  { if (dynamic_cast<CDualSword*>(m_pDualSword_R)->Get_isHide()) { if (dynamic_cast<CDualSword*>(m_pDualSword_L)->Get_isHide()) { return true; } } return false; }

	const _matrix							Get_ViewMatrix() const;
	
	__forceinline const PLAYER_WEAPONTYPE&	Get_MainWeapon() const	{ return m_eMainWeapon; }
	__forceinline const PLAYER_WEAPONTYPE&	Get_SubWeapon() const	{ return m_eSubWeapon; }
	__forceinline _int						Get_Stamina()			{ return m_iStamina; }
	__forceinline _float					Get_Mp()				{ return m_iMp; }

	__forceinline _int						Get_AirComboLevel()		{ return m_iAirComboLevel; }
	__forceinline _bool						Get_Tired()				{ return m_bIsTired; }

	__forceinline Engine::STARTMOTION		Get_ReserveAction()		{ return m_eReserveAtk; }
	__forceinline _double					Get_Rage()				{ return m_dbRage; }
	__forceinline _bool						Get_Dash()				{ return m_bReserveDash; }
	__forceinline COLLISIONID				Get_CollisionID()		{ return m_eCollisionID; }
	__forceinline _vec3						Get_TargetPos()			{ return m_vTargetPos; }

	__forceinline _bool						Get_TargetLock()		{ return m_bTargetLock; }
	__forceinline _vec3						Get_Focus()				{ return m_vFocus; }

	__forceinline _bool						Get_Break()				{ return m_bBreak; }
 	// Set_Function
public:
	__forceinline void						Set_State(PLAYER_STATE eState)							{ m_eCurState = eState; }
	__forceinline void						Set_ReserveAction(Engine::STARTMOTION eReserveKey)		{ m_eReserveAtk = eReserveKey; }
	__forceinline void						Set_AnimationSpeed(const _double& dbAnimatoinSpeed)		{ m_dbAnimationSpeed = dbAnimatoinSpeed; }
	
	__forceinline void						Set_Jump_Start()				    { m_bIsJump = true; }
	__forceinline void						Set_Jump_Finish()				    { m_bIsJump = false; }
	__forceinline void						Set_Fall_Start()				    { m_bIsFall = true; }
	__forceinline void						Set_Fall_End()					    { m_bIsFall = false; }
	__forceinline void						Set_JumpTime(_double& dbJumptime)   { m_dbJumpTime = dbJumptime; }
	__forceinline void						Set_Index(_ulong& dwIndex)		    { m_dwNaviIndex = dwIndex; }
public:
	__forceinline void						Set_DualSowrd_Hide(_bool bValue)	{ if (m_eMainWeapon == DUALSWORD) { m_pDualSword_R->Set_Hide(bValue); m_pDualSword_L->Set_Hide(bValue); }}
	__forceinline void						Set_BattleType(BATTLETYPE eType)	{ m_eBattleType = eType; }
	__forceinline void						Set_Stamina(_int iStamina)			{ m_iStamina -= iStamina; }
	__forceinline void						Set_Mp(_float iMp)					{ m_iMp -= iMp; }
	__forceinline void						Set_SuperAmmor(_int iSuperAmmor)	{ m_tBaseInfo.iSuperAmmor = iSuperAmmor; }

	__forceinline void						Set_AirComboLevel(_int iLevel)		{ m_iAirComboLevel = iLevel; }

	__forceinline void						Reset_SuperAmmor() { m_tBaseInfo.iSuperAmmor = m_iBasicSuperArmor; }


	__forceinline void						Set_OrbAnimation(COrb::ORB_STATE eState) { m_pOrb->Set_OB_Animation(eState); }
	__forceinline void						Set_Rage(_double dbRage)			{ m_dbRage -= dbRage; }
	__forceinline void						Set_DashReserve()					{ m_bReserveDash = true; }
	__forceinline void						ReSet_DashReserve()					{ m_bReserveDash = false; }
	__forceinline void						ReSet_CollisionID()					{ m_eCollisionID = COLLISION_END; }
	__forceinline void						Set_Heart()							{ m_bIsAttacked = false; }
	__forceinline void						Set_Incibility()					{ m_bincibility = true; }
	void									ReSet_Incibility();
	__forceinline void						Set_Dead() { m_bIsDead = true; }

	void									Set_TransApostle(_bool bTrans) { m_bTransApostle = bTrans; }
	// 충돌
public:
	virtual	void							Set_Collision(CBaseObject* pObj, COLLIDER eColliderType);
private:
	void									Set_Collision_Effect(CBaseObject* pObj);
private:
	virtual HRESULT							Clone_Component() override;
	HRESULT									Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	// 상태 업데이트
	virtual void							Change_State() override;
	void									Update_Status(const _double dTimeDelta);
	void									Change_Weapon();
	void									Create_SubWeapon();

public:
	//활이나 오브 공격할때 일정거리 내면 자동 공격하게 만듬
	//0 : 찾지 못함 1 : 벨라토스 찾음 2 : AI1 찾음 3 : AI2 찾음
	_int									Check_Enemy();
	void									Dead_Player(const _double& dTimeDelta);
	////////////아래 코드는 추후 지울것
private:
	void									InGyunCodeTest(const _double& dTimeDelta);
	void									SiWonCodeTest(const _double& dTimeDelta);
	void									YongHoCodeTest(const _double& dTimeDelta);
	void									HeeJungCodeTest(const _double& dTimeDelta);
	void									YeongJoonCodeTest(const _double& dTimeDelta);
	////////////위 코드는 추후 지울것
public:
	// 상태 변화 체크 (외부 키 입력/애니메이션 종료/피격 등에 의해 상태 변경)	
	virtual void		ExChange_State(CPlayerState* pState, _bool bAtk = false);

public:
	//옵저버 패스
	void				ObserverPass();

public:
	//입구쪽에서 UI 뜨게하는 함수 -> 희정 만듬
	void				MissionEnterDistanceCheck();

private:
	COLLISIONID				m_eCollisionID;
	Engine::STARTMOTION		m_eReserveAtk;
	Engine::CDynamicMesh*	m_pWeaponCom;

	CDynamicMeshObject*		m_pWeapon;

	BATTLETYPE				m_eBattleType;
	map<EFFECTID, _double>	m_mapinvincibility;

	_bool					m_bLateInit;
	_bool					m_bIsJump;				// Checking jump
	_bool					m_bIsFall;
	_bool					m_bIsAttacked;
	_bool					m_bReserveDash;
	_bool					m_bincibility;
	_bool					m_bTargetLock;
	_bool					m_bPlayerDeadFinish;
	_bool					m_bRender;
	_bool					m_bCutscene;
	_bool					m_bIncTime;
	_bool					m_bDeadSound;
	_bool					m_bEnterSound;

	_float					m_fPlayerSpeed;
	_float					m_fRecordY;				// 점프 전 높이를 저장하는 y값

	_double					m_dbAnimationSpeed;
	_double					m_dbJumpTime;
	_double					m_dbRecoveryTime;
	_double					m_bTiredTime;
	_double					m_dbDeadTime;
	_double					m_dbInvTime;
	_double					m_dbFallTime;
	_double					m_dbEnterTime;
	_double					m_dbTiredTime;

	_int					m_iStamina;
	_int					m_iMaxStamina;
	_float					m_iMp;
	_float					m_iMaxMp;

	_int					m_iCurHp;
	_int					m_iDifHp;
	
	_int					m_iPreHp;
	_int					m_iDifPreHp;

	_int					m_iAirComboLevel;

	///////////////////
	//희정
	CUIMgr*					m_pUIMgr;
	_bool					m_bMissionMapCheck;
	_bool					m_bCreateRevivalUI;
	///////////////////

	// 시원 돼지천사 플레이어 변신하기위한변수 //
	_bool					m_bTransApostle;
	
	// 임시 변수
	_bool m_bBreak;
	_bool m_bIsTired;
	_bool m_bCritical;

	_int m_iBasicSuperArmor;
	_int m_iCritical;

	_vec3 m_vTargetPos;
	_vec3 m_vFocus;
	DEAD_STEP m_eDeadStep;

	_bool m_bCheat;
	_double m_dbHeartbeatSoundTime;
	_double m_dbHBSoundCycle;

	_vec3 vStartingPoint;
	//
	Engine::CTransform* m_pTargetTransform;

private: // Const Value
	const _float			m_fAccelerate;
	const _float			m_fMaxSpeed;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, PLAYER_ID eID);
	virtual void	Free();

};

