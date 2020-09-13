#pragma once

#include "DynamicMeshObject.h"
#include "SphereCollider.h"
#include "TwohandSword.h"
#include "DualSword.h"
#include "LongBow.h"
#include "Orb.h"
#include "Calculator.h"
#include "Decal.h"
//#include "PlayerState.h"
//#include "Ai_PlayerState.h"
// Ai_Player(Enemy / Aliance)와 실제 플레이할 캐릭터에 사용할 부모 클레스

class CPlayerState;
class CAi_PlayerState;

class CBasePlayer :public CDynamicMeshObject
{
public:
	enum PLAYER_WEAPONTYPE { COMMON, DUALSWORD, TWOHANDSWORD, ORB, LONGBOW };
	enum ATK_LEVEL { LEVEL1, LEVEL2, LEVEL3, LEVEL4, LEVEL5, LEVEL_END };
	enum PLAYER_ID { ID_PLAYER, ID_AI, ID_END };
	enum STATE_ID { ID_COMMON, ID_BASICCOMBO, ID_BASICCOMBOEX, ID_AIRCOMBO, ID_SKILL, ID_HEART, STATEID_END };

	enum PLAYER_STATE {
		COMMON_AIR_JUMP,
		COMMON_AIR_JUMP_FAST,
		COMMON_AIR_JUMPDOWN,
		COMMON_AIR_JUMPDOWN_FAST,
		COMMON_AIR_JUMPLANDING,
		COMMON_AIR_JUMPLANDING_FAST,
		COMMON_AIR_JUMPUP,
		COMMON_AIR_JUMPUP_FAST,
		COMMON_AIR_START,
		COMMON_CAPTUREFLAG_END,
		COMMON_CAPTUREFLAG_LOOP,
		COMMON_CAPTUREFLAG_START,
		COMMON_CHASE,
		COMMON_COMBATWAIT,
		COMMON_DASH,
		COMMON_DOWN_B_B,
		COMMON_DOWN_B_L,
		COMMON_DOWN_B_R,
		COMMON_DOWN_F_F,
		COMMON_DOWN_F_L,
		COMMON_DOWN_F_R,
		COMMON_DYING,
		COMMON_RESPAWN,
		COMMON_RTCHASE_BIG_B,
		COMMON_RTCHASE_BIG_F,
		COMMON_RTCHASE_BIG_L,
		COMMON_RTCHASE_BIG_R,
		COMMON_RTCHASE_SMALL,
		COMMON_RTDOWN_AIR_FALL_B_SMALL_B,
		COMMON_RTDOWN_AIR_FALL_B_SMALL_L,
		COMMON_RTDOWN_AIR_FALL_B_SMALL_R,
		COMMON_RTDOWN_AIR_FALL_F_SMALL_F,
		COMMON_RTDOWN_AIR_FALL_F_SMALL_L,
		COMMON_RTDOWN_AIR_FALL_F_SMALL_R,
		COMMON_RTDOWN_AIR_LANDING_B_B,
		COMMON_RTDOWN_AIR_LANDING_B_L,
		COMMON_RTDOWN_AIR_LANDING_B_R,
		COMMON_RTDOWN_AIR_LANDING_F_F,
		COMMON_RTDOWN_AIR_LANDING_F_L,
		COMMON_RTDOWN_AIR_LANDING_F_R,
		COMMON_RTDOWN_B_B,
		COMMON_RTDOWN_B_L,
		COMMON_RTDOWN_B_R,
		COMMON_RTDOWN_F_F,
		COMMON_RTDOWN_F_L,
		COMMON_RTDOWN_F_R,
		COMMON_RTSTAND_AIR_BIG_B,
		COMMON_RTSTAND_AIR_BIG_F,
		COMMON_RTSTAND_AIR_BIG_L,
		COMMON_RTSTAND_AIR_BIG_R,
		COMMON_RTSTAND_AIR_FALL_BIG_B,
		COMMON_RTSTAND_AIR_FALL_BIG_F,
		COMMON_RTSTAND_AIR_FALL_BIG_L,
		COMMON_RTSTAND_AIR_FALL_BIG_R,
		COMMON_RTSTAND_AIR_FALL_SMALL_B,
		COMMON_RTSTAND_AIR_FALL_SMALL_F,
		COMMON_RTSTAND_AIR_FALL_SMALL_L,
		COMMON_RTSTAND_AIR_FALL_SMALL_R,
		COMMON_RTSTAND_AIR_SMALL_B,
		COMMON_RTSTAND_AIR_SMALL_F,
		COMMON_RTSTAND_AIR_SMALL_L,
		COMMON_RTSTAND_AIR_SMALL_R,
		COMMON_RTSTAND_BIG_B,
		COMMON_RTSTAND_BIG_B_DOWN,
		COMMON_RTSTAND_BIG_B_L,
		COMMON_RTSTAND_BIG_B_R,
		COMMON_RTSTAND_BIG_B_UP,
		COMMON_RTSTAND_BIG_F,
		COMMON_RTSTAND_BIG_F_DOWN,
		COMMON_RTSTAND_BIG_F_L,
		COMMON_RTSTAND_BIG_F_R,
		COMMON_RTSTAND_BIG_F_UP,
		COMMON_RTSTAND_BIG_L,
		COMMON_RTSTAND_BIG_L_DOWN,
		COMMON_RTSTAND_BIG_L_L,
		COMMON_RTSTAND_BIG_L_R,
		COMMON_RTSTAND_BIG_L_UP,
		COMMON_RTSTAND_BIG_R,
		COMMON_RTSTAND_BIG_R_DOWN,
		COMMON_RTSTAND_BIG_R_L,
		COMMON_RTSTAND_BIG_R_R,
		COMMON_RTSTAND_BIG_R_UP,
		COMMON_RTSTAND_SMALL_B,
		COMMON_RTSTAND_SMALL_B_DOWN,
		COMMON_RTSTAND_SMALL_B_L,
		COMMON_RTSTAND_SMALL_B_R,
		COMMON_RTSTAND_SMALL_B_UP,
		COMMON_RTSTAND_SMALL_F,
		COMMON_RTSTAND_SMALL_F_DOWN,
		COMMON_RTSTAND_SMALL_F_L,
		COMMON_RTSTAND_SMALL_F_R,
		COMMON_RTSTAND_SMALL_F_UP,
		COMMON_RTSTAND_SMALL_L,
		COMMON_RTSTAND_SMALL_L_DOWN,
		COMMON_RTSTAND_SMALL_L_L,
		COMMON_RTSTAND_SMALL_L_R,
		COMMON_RTSTAND_SMALL_L_UP,
		COMMON_RTSTAND_SMALL_R,
		COMMON_RTSTAND_SMALL_R_DOWN,
		COMMON_RTSTAND_SMALL_R_L,
		COMMON_RTSTAND_SMALL_R_R,
		COMMON_RTSTAND_SMALL_R_UP,
		COMMON_RUN,
		COMMON_RUN_FAST,
		COMMON_RUN_FAST_L,
		COMMON_RUN_FAST_R,
		COMMON_RUN_L,
		COMMON_RUN_R,
		COMMON_RUNEND,
		COMMON_RUNEND_FAST,
		COMMON_STANDUP_ROLLING,
		COMMON_STANDUPATTACK,
		COMMON_START,
		COMMON_STUN_LOOP,
		COMMON_TIRED,
		COMMON_TIRED_WALK,
		COMMON_WAIT,
		COMMON_WAIT_HABIT,
		COMMON_WALK,
		COMMON_WALL_LANDING,
		HF_COMBATWAIT_WAIT,
		DS_AIRCOMBO_01,
		DS_AIRCOMBO_01_DOWN,
		DS_AIRCOMBO_01_UP,
		DS_AIRCOMBO_02,
		DS_AIRCOMBO_02_DOWN,
		DS_AIRCOMBO_02_UP,
		DS_AIRCOMBO_03,
		DS_AIRCOMBO_03_DOWN,
		DS_AIRCOMBO_03_UP,
		DS_AIRCOMBO_04,
		DS_AIRCOMBO_04_DOWN,
		DS_AIRCOMBO_04_UP,
		DS_AVOID_DAMAGE,
		DS_AVOID_LANDING,
		DS_AVOID_NONTARGET,
		DS_AVOID_READY,
		DS_AVOID_TARGET,
		DS_BASICCOMBO_01,
		DS_BASICCOMBO_02,
		DS_BASICCOMBO_03,
		DS_BASICCOMBO_04,
		DS_BASICCOMBO_05,
		DS_BASICCOMBOEX1,
		DS_BASICCOMBOEX2_END,
		DS_BASICCOMBOEX2_LOOP,
		DS_BASICCOMBOEX2_START,
		DS_BASICCOMBOEX3_03,
		DS_BASICCOMBOEX3_03_LANDING,
		DS_DASHCOMBO,
		DS_RAGESKILL_GENOCIDE,
		DS_RAGESKILL_GENOCIDE_START,
		DS_RAGESKILL_PHANTOMBLADE,
		DS_RAGESKILL_PHANTOMBLADE_ATTACK,
		DS_RAGESKILL_RAPIDSWORD,
		DS_SKILL_CRITICALX,
		DS_SKILL_EXTREMEBLADE,
		DS_SKILL_FATEDCIRCLE_END,
		DS_SKILL_FATEDCIRCLE_START,
		DS_SKILL_GHOSTEVASION,
		DS_SKILL_SCART,
		DS_SKILL_SPINNINGSLASH,
		DS_SPECIALCOMBO_CHARGE,
		DS_SPECIALCOMBO_CHARGE_LOOP,
		DS_SPECIALCOMBO_END,
		DS_SPECIALCOMBO_LOOP,
		DS_SPECIALCOMBO_START,
		DS_SPECIALCOMBOEX1_ATTACK,
		DS_SPECIALCOMBOEX1_ATTACK_01,
		DS_SPECIALCOMBOEX1_ATTACK_02,
		DS_SPECIALCOMBOEX1_ATTACK_03,
		DS_SPECIALCOMBOEX1_END,
		DS_SPECIALCOMBOEX1_LANDING,
		DS_SPECIALCOMBOEX1_LOOP,
		DS_SPECIALCOMBOEX1_START,
		DS_SPECIALMOVE,
		DS_SWITCHING_AIR,
		DS_SWITCHING_GROUND,
		OB_AIR_COMBO_01,
		OB_AIR_COMBO_01_DOWN,
		OB_AIR_COMBO_01_UP,
		OB_AIR_COMBO_02,
		OB_AIR_COMBO_02_DOWN,
		OB_AIR_COMBO_02_UP,
		OB_AIR_COMBO_03,
		OB_AIR_COMBO_03_DOWN,
		OB_AIR_COMBO_03_UP,
		OB_AVOID,
		OB_AVOID_ATTACK,
		OB_AVOIDATTACK_END,
		OB_BASICCOMBO_01,
		OB_BASICCOMBO_01_DOWN,
		OB_BASICCOMBO_01_UP,
		OB_BASICCOMBO_02,
		OB_BASICCOMBO_02_DOWN,
		OB_BASICCOMBO_02_UP,
		OB_BASICCOMBO_03,
		OB_BASICCOMBO_03_DOWN,
		OB_BASICCOMBO_03_UP,
		OB_BASICCOMBO_04,
		OB_BASICCOMBO_04_DOWN,
		OB_BASICCOMBO_04_UP,
		OB_BASICCOMBO_05,
		OB_BASICCOMBO_05_DOWN,
		OB_BASICCOMBO_05_UP,
		OB_BASICCOMBO_06,
		OB_BASICCOMBOEX1,
		OB_BASICCOMBOEX2_01_B,
		OB_BASICCOMBOEX2_01_F,
		OB_BASICCOMBOEX2_01_L,
		OB_BASICCOMBOEX2_01_R,
		OB_CHASE_ATTACK,
		OB_CHASE_ATTACK_END,
		OB_CHASE_START,
		OB_DASHCOMBO,
		OB_RAGESKILL_GIANTTWINKLE,
		OB_RAGESKILL_GIANTTWINKLE_READY,
		OB_RAGESKILL_SPIRITCOUNTER,
		OB_SKILL_CHAINWHIP,
		OB_SKILL_CHAINWHIP_READY,
		OB_SKILL_GIANTCOFFIN,
		OB_SKILL_GIANTCOFFIN_AIR,
		OB_SKILL_GIANTCOFFIN_START,
		OB_SKILL_OVERFLASHING_END,
		OB_SKILL_OVERFLASHING_START,
		OB_SKILL_PATROLMOVEMENT_AIR,
		OB_SKILL_PATROLMOVEMENT_LANDING,
		OB_SKILL_PATROLMOVEMENT_READY,
		OB_SKILL_PATROLMOVEMENT_START,
		OB_SKILL_SPINNINGCRUSHER,
		OB_SKILL_SPINNINGCRUSHER_START,
		OB_SKILL_SPIRITTWINKLE_READY,
		OB_SKILL_SPIRITTWINKLE_START,
		OB_SPECIALCOMBO_READY,
		OB_SPECIALCOMBO_READY_WALK_B,
		OB_SPECIALCOMBO_READY_WALK_B_L,
		OB_SPECIALCOMBO_READY_WALK_F,
		OB_SPECIALCOMBO_READY_WALK_F_R,
		OB_SPECIALCOMBO_READY_WALK_L,
		OB_SPECIALCOMBO_READY_WALK_R,
		OB_SPECIALMOVE,
		OB_SWITCHING_AIR,
		OB_SWITCHING_AIR_DOWN,
		OB_SWITCHING_AIR_UP,
		OB_SWITCHING_GROUND,
		LB_AIRCOMBO_01,
		LB_AIRCOMBO_01_DOWN,
		LB_AIRCOMBO_01_UP,
		LB_AIRCOMBO_02,
		LB_AIRCOMBO_02_DOWN,
		LB_AIRCOMBO_02_UP,
		LB_AIRCOMBO_03,
		LB_AIRCOMBO_03_DOWN,
		LB_AIRCOMBO_03_UP,
		LB_AIRCOMBOEX1_END,
		LB_AIRCOMBOEX1_END_DOWN,
		LB_AIRCOMBOEX1_END_UP,
		LB_AIRCOMBOEX1_LOOP,
		LB_AIRCOMBOEX1_LOOP_DOWN,
		LB_AIRCOMBOEX1_LOOP_UP,
		LB_AIRCOMBOEX1_START,
		LB_AIRCOMBOEX1_START_DOWN,
		LB_AIRCOMBOEX1_START_UP,
		LB_AVOID,
		LB_AVOID_LANDING,
		LB_AVOIDCOMBO_01,
		LB_BASICCOMBO_01,
		LB_BASICCOMBO_01_DOWN,
		LB_BASICCOMBO_01_UP,
		LB_BASICCOMBO_02,
		LB_BASICCOMBO_02_DOWN,
		LB_BASICCOMBO_02_UP,
		LB_BASICCOMBO_03,
		LB_BASICCOMBO_03_DOWN,
		LB_BASICCOMBO_03_UP,
		LB_BASICCOMBOEX1_END,
		LB_BASICCOMBOEX1_LOOP,
		LB_BASICCOMBOEX1_START,
		LB_BASICCOMBOEX2_END,
		LB_BASICCOMBOEX2_START,
		LB_BASICCOMBOEX3_01,
		LB_BASICCOMBOEX3_01_DOWN,
		LB_BASICCOMBOEX3_01_F,
		LB_BASICCOMBOEX3_01_L,
		LB_BASICCOMBOEX3_01_R,
		LB_BASICCOMBOEX3_01_UP,
		LB_BASICCOMBOEX3_02,
		LB_BASICCOMBOEX3_02_CHARGE,
		LB_BASICCOMBOEX3_02_CHARGESHOT,
		LB_BASICCOMBOEX3_02_DOWN,
		LB_BASICCOMBOEX3_02_UP,
		LB_CHASE,
		LB_CHASE_END,
		LB_CHASE_LOOP,
		LB_CHASE_LOOP_UP,
		LB_DASHCOMBO_01,
		LB_DOUBLEJUMP,
		LB_DOUBLEJUMP_ATTACK,
		LB_DOUBLEJUMP_ATTACK_DOWN,
		LB_DOUBLEJUMP_ATTACK_UP,
		LB_SKILL_SIEGEPOSITION_AIMING_CHARGING,
		LB_SKILL_SIEGEPOSITION_AIMING_CHARGING_UP,
		LB_SKILL_SIEGEPOSITION_END,
		LB_SKILL_SIEGEPOSITION_LOOP,
		LB_SKILL_SIEGEPOSITION_SHOT,
		LB_SKILL_SIEGEPOSITION_SHOT_UP,
		LB_SKILL_SIEGEPOSITION_START,
		LB_SPECIALCOMBO_AVOID_B,
		LB_SPECIALCOMBO_AVOID_F,
		LB_SPECIALCOMBO_AVOID_L,
		LB_SPECIALCOMBO_AVOID_R,
		LB_SPECIALCOMBO_CHARGE,
		LB_SPECIALCOMBO_CHARGELIMIT,
		LB_SPECIALCOMBO_CHARGESHOT,
		LB_SPECIALCOMBO_CHARGESHOT_DOWN,
		LB_SPECIALCOMBO_CHARGESHOT_LAST,
		LB_SPECIALCOMBO_CHARGESHOT_UP,
		LB_SPECIALCOMBO_END,
		LB_SPECIALCOMBO_READY,
		LB_SPECIALCOMBO_WAIT,
		LB_SPECIALCOMBO_WALK_R,
		LB_SPECIALCOMBO_WALK_R_50,
		LB_SPECIALCOMBO_WALK_R_200,
		LB_SPECIALCOMBOEX1_CHARGE,
		LB_SWICHING_GROUND,
		LB_SWITCHING_AIR_C,
		TS_AIR_COMBO_01,
		TS_AIR_COMBO_02,
		TS_AIR_COMBO_03,
		TS_AIR_COMBO_04,
		TS_AIR_COMBO_04_LANDING,
		TS_AVOID_ATTACK,
		TS_AVOID_LANDING,
		TS_AVOID_LANDING_B,
		TS_AVOID_START,
		TS_AVOID_START_B,
		TS_BASICCOMBO_01,
		TS_BASICCOMBO_02,
		TS_BASICCOMBO_03,
		TS_BASICCOMBOEX01,
		TS_BASICCOMBOEX02_END,
		TS_BASICCOMBOEX02_LOOP,
		TS_BASICCOMBOEX02_START,
		TS_BASICCOMBOEX03_01,
		TS_BASICCOMBOEX03_02_END,
		TS_BASICCOMBOEX03_02_LOOP,
		TS_BASICCOMBOEX03_02_LOOP_02,
		TS_BASICCOMBOEX03_03,
		TS_BASICCOMBOEX3_END,
		TS_BASICCOMBOEX3_LOOP_01,
		TS_BASICCOMBOEX3_LOOP_02,
		TS_DASHCOMBO,
		TS_GROUNDCRASH,
		TS_GROUNDCRASH_READY,
		TS_GROUNDCRASH_READY_STOP,
		TS_GROUNDCRASH_RUN,
		TS_GROUNDCRASH_RUN_50,
		TS_GROUNDCRASH_RUN_400,
		TS_GROUNDCRASH_WALK,
		TS_GROUNDCRASH_WALK_50,
		TS_LEAP_ATTACK_AIR,
		TS_LEAP_ATTACK_AIRDOWN,
		TS_LEAP_ATTACK_AIRUP,
		TS_LEAP_ATTACK_LANDING,
		TS_LEAP_ATTACK_READY,
		TS_LEAP_ATTACK_READY_STOP,
		TS_LEAP_ATTACK_RUN,
		TS_LEAP_ATTACK_RUN_50,
		TS_LEAP_ATTACK_RUN_400,
		TS_LEAP_ATTACK_START,
		TS_LEAP_ATTACK_WALK,
		TS_LEAP_ATTACK_WALK_50,
		TS_OUTRAGE_MOVE,
		TS_OUTRAGE_START,
		TS_PIERCINGRUSH,
		TS_PIERCINGRUSH_END,
		TS_RAGESKILL_ARMAGEDDONBLADE,
		TS_RAGESKILL_DOUBLESLASH,
		TS_ROCKBREAK,
		TS_ROCKSHOT,
		TS_SPECIALCOMBO_CRASH,
		TS_SPECIALCOMBO_CRASH_BIG,
		TS_SPECIALCOMBO_READY,
		TS_SPECIALCOMBO_READY_STOP,
		TS_SPECIALCOMBOEX1_AVOIDSLASH,
		TS_SPECIALCOMBOEX1_AVOIDSLASH_B,
		TS_SPECIALCOMBOEX1_AVOIDSLASH_L,
		TS_SPECIALCOMBOEX1_AVOIDSLASH_R,
		TS_SPECIALCOMBOEX1_CHARGECRASH,
		TS_SPECIALCOMBOEX1_CRASH,
		TS_SPECIALCOMBOEX1_READY_RE,
		TS_SPECIALCOMBOEX1_READY_RE02,
		TS_SWITCHING_AIR,
		TS_SWITCHING_GROUND,
		LB_RAGESKILL_SOULOFGRANDARCHER,
		LB_RAGESKILL_PHOENIXARROW,
		LB_SKILL_ARROWSTORM_G2_SHOT,
		LB_SKILL_ARROWSTORM_G3_SHOT,
		LB_SKILL_ARROWSTORM_READY,
		LB_SKILL_ARROWSTORM_SHOT,
		LB_SKILL_ARROWSTORM_SHOT_UP,
		LB_SKILL_BINDINGSHOT_END,
		LB_SKILL_BINDINGSHOT_G2_WALK,
		LB_SKILL_BINDINGSHOT_G3_WALK,
		LB_SKILL_BINDINGSHOT_READY,
		LB_SKILL_BINDINGSHOT_WAIT,
		LB_SKILL_BINDINGSHOT_WALK,
		LB_SKILL_SPIRITARROW_SHOT,
		LB_SKILL_SPLITARROW_SHOT, 

		STATE_END
	};
	enum PLAYER_TYPE { PLAYINGCHARACTER, ALIANCECHARACTER, ENEMYCHARACTEER };
public:
	explicit CBasePlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBasePlayer();

public:
	_vec3		GetPlayerPos();
	PLAYER_ID	GetPlayerID() { return m_ePlayerID; }

	__forceinline ATK_LEVEL				Get_AtkLevel() { return m_eAtkLevel; }

	__forceinline void					Set_AtkLevel(ATK_LEVEL eLevel) { m_eAtkLevel = eLevel; }

public:
	virtual HRESULT		Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

	// Get
	const PLAYER_WEAPONTYPE&	Get_MainWeapon() const { return m_eMainWeapon; }
	const PLAYER_WEAPONTYPE&	Get_SubWeapon() const { return m_eSubWeapon; }
	STATE_ID					Get_CurStateID() { return m_eIdCurrent; }
	STATE_ID					Get_NextStateID() { return m_eIdNext; }

	STATE_ID					Get_StateID() { return m_eIdCurrent; }
	PLAYER_STATE				Get_CurState() { return m_eCurState; }	// 플레이어랑 Ai 둘 다 가능

	// Set
	void						Set_DownKey(INPUT_KEY eKey);
	void						Set_UpKey(INPUT_KEY eKey);
	void						Set_PressKey(INPUT_KEY eKey);
	void						Set_RageZero();
	void						Set_CurState(PLAYER_STATE eState) { m_eCurState = eState; }
	void						Set_MainWeapon(_uint iWeapon) { m_eMainWeapon = (PLAYER_WEAPONTYPE)iWeapon; } //희정 추가(카르마스왑시 필요)
	void						Set_SubWeapon(_uint iWeapon) {m_eSubWeapon = (PLAYER_WEAPONTYPE)iWeapon;} //희정 추가(카르마스왑시 필요)

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	CAi_PlayerState*			Get_AiState() { return m_pAiState; }
	CONTROLTYPE					Get_ControlType() { return m_eCtrlType; }
	CBasePlayer*				Get_PlayerPointer() { return this; }
	CBasePlayer*				Get_TargetPlayer() { return m_pTargetPlayer; }
	CONTROLTYPE					Get_Target_ControlType() { return m_pTargetPlayer->Get_ControlType(); }
	_vec3						Get_TargetPos() { return m_pTargetPlayer->GetPlayerPos(); }
	_matrix						Get_Target_WorldMatrix() { return *m_pTargetPlayer->Get_TransformCom()->Get_WorldMatrix(); }
	_vec3						Get_FlagPos() { return m_vFlagPos; }
	STATETYPE					Get_StateState() { return m_eCur_StateState; }
	_bool						Get_isCheckLookDir() { return m_bisCheckLookDir; }
	_bool						Get_CaptureFlag() { return m_bCaptureFlag; }
	_bool						Get_ActionEnd() { return m_bActionEnd; }
	_vec3						Get_DashDir() { return m_vDashDir; }
	_bool						Get_CartelBlueStart() { return m_bCartelBlueStart; }
	_bool						Get_CartelRedStart() { return m_bCartelRedStart; }
	CDecal*						Get_Decal() { return m_pDecal; }
	_bool						Get_isRunToFlag() { return m_bisRunToFlag; }
	_bool						Get_MoveKey() { return m_bMoveKey; }

	Engine::CDynamicMesh*		Get_DyanmicMesh() { return m_pDynamicMeshCom; }
	Engine::CNaviMesh*			Get_NaviMesh() { return m_pNaviMeshCom; }

	void						Set_TargetPlayer(CBasePlayer* pObj) { m_pTargetPlayer = pObj; }
	void						Set_FlagPos(_vec3 vPos) { m_vFlagPos = vPos; }
	void						Set_StateState(STATETYPE eState) { m_eNext_StateState = eState; }
	void						Set_isCheckLookDir(_bool bValue) { m_bisCheckLookDir = bValue; }
	void						Set_isSetLookDir(_bool bValue) { m_bisSetLookDir = bValue; }
	void						Set_CaptureFlag(_bool bValue) { m_bCaptureFlag = bValue; }
	void						Set_LookDir_ToTarget(const _double& dTimeDelta);
	void						Set_ActionEnd(_bool bValue) { m_bActionEnd = bValue; }
	void						Set_DashDir(_vec3 vDir) { m_vDashDir = vDir; }
	void						Set_CartelBlueStart(_bool bValue) { m_bCartelBlueStart = bValue; }
	void						Set_CartelRedStart(_bool bValue) { m_bCartelRedStart = bValue; }
	void						Set_isRunToFlag(_bool bValue) { m_bisRunToFlag = bValue; }
	void						Set_Hp(_int iHp) { m_tBaseInfo.iHp = iHp; }
	void						Set_PrevHp(_int iPrevHp) { m_tBaseInfo.iPrevHp = iPrevHp; }
	void						Set_MoveKey(_bool bValue) { m_bMoveKey = bValue; }

	_bool						CheckLookDir(const _double& dTimeDelta);
	_float						GetDist_ToEnermy();
	_float						GetDist_ToFlag();
	TEAM						Get_Team() { return m_eFlagTeam; }

	CDynamicMeshObject*			Get_MainWeaponPointer();
	CDualSword*					Get_DualSwordL() { return m_pDualSword_L; }

	// %%%%%%%%%%%% 타이머 관련 함수 %%%%%%%%%%%%%%% //
	void						Set_Timer(wstring TimerName) { m_mapTimer.emplace(TimerName, 0.f); }
	_double						Get_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return 0; } return m_mapTimer.find(TimerName)->second; }
	void						Start_Timer(wstring TimerName) { m_mapTimer.find(TimerName)->second += Engine::Get_TimeDelta(L"Timer_FPS60"); }
	_bool						IsReady_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return false; } return true; }
	const size_t				Get_TimerSize() { return m_mapTimer.size(); }
	void						Clear_Timer() { m_mapTimer.clear(); }
	void						Delete_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return; } else { auto Obj = m_mapTimer.find(TimerName); m_mapTimer.erase(Obj); } }
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

protected:
	map<wstring, _double>			m_mapTimer;						// 타이머
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

public:
	//데칼
	__forceinline _vec3*					Get_DecalPos() { return &m_vDecalPos; }
	void									Release_Decal();
	void									Set_DecalPos(_vec3* pDecalPos) { m_vDecalPos = *pDecalPos; }
	HRESULT									Create_Decal(CDecal::DECAL_TYPE eDecalType, _double dCountDown, _vec3 vPlayerAngle = _vec3(0.f, 0.f, 0.f));
	void									RotationToDecal();
protected:
	virtual HRESULT		Clone_Component();

	// 상태 변화 체크 (외부 키 입력/애니메이션 종료/피격 등에 의해 상태 변경)
	// 매개변수로 들어갈 인자 : 플레이어 상태(COMMON/DUALSWORD/TWOHANDSWORD/ORB/LONGBOW), 
	// InputKey, fTime, Type(PLAYINGCHARACTER/ALIANCECHARACTER/ENEMYCHARACTER)
	virtual void		Change_State();
	virtual void		ExChange_State(CPlayerState* pState, _bool bAtk = false);



protected:
	Engine::CSphereCollider*	m_pColldier;

	// 시험 변수 시작
	_uint						m_iAnimation; // Test Value for Playing Animation 
	// 시험 변수 종료

	  // 전투 상태(사용하는 무기타입)
	PLAYER_WEAPONTYPE		m_eMainWeapon;
	PLAYER_WEAPONTYPE		m_eSubWeapon;

	PLAYER_ID				m_ePlayerID;


	// Animation State(현재 애니메이션 상태)
	PLAYER_STATE			m_eCurState;
	// 플레이어 타입(플레이어, 아군 AI, 적 AI)
	PLAYER_TYPE				m_ePlayingType;

	CPlayerState*			m_pState;
	CAi_PlayerState*		m_pAiState;
	
	STATE_ID				m_eIdCurrent;
	STATE_ID				m_eIdNext;

	ATK_LEVEL				m_eAtkLevel;

	/////////////////// 플레이어와 AI들 공통으로 쓸 변수(옵저버로 UI에 전달)
	_double					m_dbRage;
	_double					m_dbMaxRage;

	_uint					m_iDeathCnt;
	_uint					m_iGetFlagCnt;

	CONTROLTYPE				m_eCtrlType;
	TEAM					m_eFlagTeam;
	
	// %%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%
	STATETYPE				m_eCur_StateState;
	STATETYPE				m_eNext_StateState;

	CBasePlayer*			m_pTargetPlayer;
	_vec3					m_vFlagPos;
	_bool					m_bisCheckLookDir = false;
	_bool					m_bisSetLookDir = false;
	_bool					m_bCaptureFlag = false;
	_bool					m_bActionEnd = false;

	_vec3					m_vDashDir;

	_bool					m_bCartelBlueStart = false;
	_bool					m_bCartelRedStart = false;
	_bool					m_bisRunToFlag = false;
	_bool					m_bMoveKey = false;
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	//데칼
	//Ray-Picking, For Decal
	Engine::CCalculator*	m_pCalculatorCom;
	_vec3					m_vDecalPos; //데칼 좌표 실시간으로
	CDecal*					m_pDecal; // 데칼 포인터가 필요

	// Weapon pointer
	CDualSword*				m_pDualSword_R;
	CDualSword*				m_pDualSword_L;

	COrb*					m_pOrb;
	CTwohandSword*			m_pTwoHandSword;
	CLongBow*				m_pLongBow;
	// Weapon Pointer End
public:
	static CBasePlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free();

};

