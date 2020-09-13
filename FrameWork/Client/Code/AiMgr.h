#pragma once
#ifndef __CAIMGR_H__
#define __CAIMGR_H__

#include "BasePlayer.h"
#include "Flag.h"

class CPlayer;
class CAi_Player;
class CBaseEffect;

class CAiMgr
{
	DECLARE_SINGLETON(CAiMgr)
public:
	enum STAGE { STATE_BOSS, STAGE_CARTEL, STAGE_END };
	enum ATTACKTYPE { ATTACK_LB1, ATTACK_LB2, ATTACK_LB3, ATTACK_RB, ATTACK_Q, ATTACK_E, ATTACK_R, ATTACK_F, ATTACK_END };

private:
	explicit CAiMgr();
	virtual ~CAiMgr();

public:
	void					Update(const _double& dTimeDelta);
	void					LateUpdate(const _double& dTimeDelta);
	void					Release();



public:
	Engine::CGameObject*	Create_Alliance();
	Engine::CGameObject*	Create_Enermy_1();
	Engine::CGameObject*	Create_Enermy_2();

public:
	// Get
	wstring					Get_NameAlliance() { return m_wstrNameAliiance; }
	wstring					Get_NameEnermy_1() { return m_wstrNameEnermy_1; }
	wstring					Get_NameEnermy_2() { return m_wstrNameEnermy_2; }

	CBasePlayer*			Get_Player() { return m_pPlayer; }
	CBasePlayer*			Get_Alliance() { return m_pAi_Alliance; }
	CBasePlayer*			Get_Enemy_1() { return m_pAi_Enermy_1; }
	CBasePlayer*			Get_Enemy_2() { return m_pAi_Enermy_2; }


	STAGE					Get_StageType() { return m_eStageType; }
	_bool					Get_CartelStart() { return m_bCartelStart; }
	_bool					Get_CartelReady() { return m_bCartelReady; }

	// Set
	HRESULT		Set_GraphicDev(LPDIRECT3DDEVICE9 pGraphicDev) { if (pGraphicDev == nullptr) { return E_FAIL; }  m_pGraphicDev = pGraphicDev; m_pGraphicDev->AddRef(); return S_OK; }
	void		Set_Player(CBasePlayer* pPlayer) { m_pPlayer = pPlayer; }
	void		Set_FlagPos(_vec3 vPos) { m_vFlagPos = vPos; }
	void		Set_Flag(CFlag* pFlag) { m_pFlag = pFlag; }
	void		Set_Stage(STAGE eStage) { m_eStageType = eStage; }
	void		Set_AttackType(CBasePlayer* pAi, ATTACKTYPE eType);
	void		Set_StartSkillCoolDown(CBasePlayer* pAi, AI_SKILL eType);
	void		Set_CartelStart(_bool bValue) { m_bCartelStart = bValue; }
	void		Set_CartelReady(_bool bValue) { m_bCartelReady = bValue; }

	// Add
	void		Add_Effect(OBJID eType, Engine::CTransform* pEffectTrans);
	
	// Delete
	void		Delete_Effect(OBJID eID, Engine::CTransform* pTrans);
	
	// Etc
	HRESULT		Team_Setting();

	//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
public:
	void		Ai_ControlCenter(const _double& dTimeDelta);
				
	void		Ai_DualSword(CBasePlayer* pAi, const _double& dTimeDelta);
	void		Ai_TwoHandSword(CBasePlayer* pAi, const _double& dTimeDelta);
	void		Ai_LongBow(CBasePlayer* pAi, const _double& dTimeDelta);
	void		Ai_Orb(CBasePlayer* pAi, const _double& dTimeDelta);
				
	void		Ai_Common_ShortRange(CBasePlayer* pAi, const _double& dTimeDelta);
	void		Ai_Common_LongRange(CBasePlayer* pAi, const _double& dTimeDelta);
				
	void		Ai_Attack_ShortRange(CBasePlayer* pAi, const _double& dTimeDelta);
	void		Ai_Attack_LongRange(CBasePlayer* pAi, const _double& dTimeDelta);

	AI_TYPE		Get_AiType(CBasePlayer* pAi);
	OBJID		Get_OBJID(CBasePlayer* pAi);
	void		Select_AttackType(CBasePlayer* pAi, const _double& dTimeDelta);
	AI_SKILL	Get_SkillType(CBasePlayer* pAi);
	void		Run_AttackType(CBasePlayer* pAi, const _double& dTimeDelta);
				
	void		Move_ToBackward(CBasePlayer* pAi, const _double& dTimeDelta);
	_bool		Avoid(CBasePlayer* pAi, const _double& dTimeDelta);
	void		Capture(CBasePlayer* pAi, const _double& dTimeDelta);

	// 주변	조건 체크
	void		Check_FlagArea_RedTeam(CBasePlayer*	pAi);
	void		Check_FlagArea_BlueTeam(CBasePlayer* pAi);
	void		Check_CapturePlayer();
	void		Check_TargetDead(CBasePlayer* pAi);
	_bool		Check_Dead(CBasePlayer* pAi);
	void		Check_Animation();

	void		Debug();
	void		Cheat();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 무기 콤보들 ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
	// DualSword
	void	DS_BasicCombo(CBasePlayer* pAi, const _double& dTimeDelta);
	void	DS_BasicComboEx01(CBasePlayer* pAi, const _double& dTimeDelta);	// 막타에 위로 올려치는거
	void	DS_BasicComboEx02(CBasePlayer* pAi, const _double& dTimeDelta);	// 계속찌르는거
	void	DS_MouseRight(CBasePlayer* pAi, const _double& dTimeDelta);		// 기모으는거 삭제하고 바로 발동되게 만듬
	void	DS_Q(CBasePlayer* pAi, const _double& dTimeDelta);
	void	DS_E(CBasePlayer* pAi, const _double& dTimeDelta);
	void	DS_R(CBasePlayer* pAi, const _double& dTimeDelta);
	void	DS_F(CBasePlayer* pAi, const _double& dTimeDelta);

	// TwoHandSword
	void	TS_BasicCombo(CBasePlayer* pAi, const _double& dTimeDelta);
	void	TS_BasicComboEx01(CBasePlayer* pAi, const _double& dTimeDelta);	// 올려치는거
	void	TS_BasicComboEx02(CBasePlayer* pAi, const _double& dTimeDelta);	// 빙빙도는거
	void	TS_MouseRight(CBasePlayer* pAi, const _double& dTimeDelta);
	void	TS_Q(CBasePlayer* pAi, const _double& dTimeDelta);
	void	TS_E(CBasePlayer* pAi, const _double& dTimeDelta);
	void	TS_R(CBasePlayer* pAi, const _double& dTimeDelta);
	void	TS_F(CBasePlayer* pAi, const _double& dTimeDelta);

	// LongBow
	void	LB_BasicCombo(CBasePlayer* pAi, const _double& dTimeDelta);
	void	LB_BasicComboEx(CBasePlayer* pAi, const _double& dTimeDelta);
	void	LB_BasicCombo_PiercingArrow(CBasePlayer* pAi, const _double& dTimeDelta);
	void	LB_LBRB(CBasePlayer* pAi, const _double& dTimeDelta);
	void	LB_Q(CBasePlayer* pAi, const _double& dTimeDelta);	
	void	LB_E(CBasePlayer* pAi, const _double& dTimeDelta);	// 데칼스킬
	void	LB_R(CBasePlayer* pAi, const _double& dTimeDelta);
	void	LB_F(CBasePlayer* pAi, const _double& dTimeDelta);	// 앉아쏘기

	// Orb
	void	OB_BasicCombo(CBasePlayer* pAi, const _double& dTimeDelta);
	void	OB_BasicComboEx01(CBasePlayer* pAi, const _double& dTimeDelta);	 // 미완성
	void	OB_BasicComboEx02(CBasePlayer* pAi, const _double& dTimeDelta);
	void	OB_MouseRight(CBasePlayer* pAi, const _double& dTimeDelta);
	void	OB_Q(CBasePlayer* pAi, const _double& dTimeDelta);
	void	OB_E(CBasePlayer* pAi, const _double& dTimeDelta);
	void	OB_R(CBasePlayer* pAi, const _double& dTimeDelta);
	void	OB_F(CBasePlayer* pAi, const _double& dTimeDelta);				// 미완성
	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 무기 콤보들 ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ



	void	View_State(CBasePlayer* pAi, const _double& dTimeDelta);


	// AI들의 스킬쿨을 돌리는 역할
	void	Update_Skill_Cooldown(const _double& dTimeDelta);
	_bool	Check_Effect(CBasePlayer* pAi);
	void	Update_FlagPos();
	void	Reset_Key();


	void	Clear_All();

private:

	// ㅡㅡㅡ 타이머 관련 함수 ㅡㅡㅡ //
	void				Set_Timer(wstring TimerName) { m_mapTimer.emplace(TimerName, 0.f); }
	_double				Get_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return 0; } return m_mapTimer.find(TimerName)->second; }
	void				Start_Timer(wstring TimerName) { m_mapTimer.find(TimerName)->second += Engine::Get_TimeDelta(L"Timer_FPS60"); }
	_bool				IsReady_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return false; } return true; }
	const size_t		Get_TimerSize() { return m_mapTimer.size(); }
	void				Clear_Timer() { m_mapTimer.clear(); }
	void				Delete_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return; } else { auto iter = m_mapTimer.find(TimerName); m_mapTimer.erase(iter); } }
	//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ//

	map<wstring, _double>				m_mapTimer;						// 타이머


	//map<wstring, _double>				m_mapAlliance_Timer;					// 타이머
	//map<wstring, _double>				m_mapEnermy1_Timer;						// 타이머
	//map<wstring, _double>				m_mapEnermy2_Timer;						// 타이머

private:
	// 플래그 정보받음
	CFlag*			m_pFlag;

	// 플레이어 정보받기위함
	CBasePlayer*	m_pPlayer;

	// ㅡㅡㅡㅡㅡㅡㅡ 포인터들 가지고 있음 ㅡㅡㅡㅡㅡㅡㅡ
	CBasePlayer*	m_pAi_Alliance;
	CBasePlayer*	m_pAi_Enermy_1;
	CBasePlayer*	m_pAi_Enermy_2;

	// 팀 구분은 각 플레이어들이 가지고 있는 m_eFlagTeam 변수를 통해 판단
	list<CBasePlayer*>	m_listTeamBlue;
	list<CBasePlayer*>	m_listTeamRed;
	
	vector<CBasePlayer*>	m_vecAiCharacter;
	vector<CBasePlayer*>	m_vecAllCharacter;
	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	// 예약 키
	vector<INPUT_KEY>		m_vecReserveKey;

	// 스킬저장소
	list<Engine::CTransform*>		m_list_Effect_Storage[OBJID::OBJECT_MONSTER];
	
	// 스킬 피할때 Collider의 Pos를 통해 Dir을 구할려고 저장
	_vec3							m_vFirstPos[OBJID::OBJECT_MONSTER];
	_vec3							m_vSecondPos[OBJID::OBJECT_MONSTER];
	_vec3							m_vAvoidDir[OBJID::OBJECT_MONSTER];
	_vec3							m_vInitialPos;

	// 스킬을 사용하면 여기에다가 추가해서 쿨타임을 돌림. 스킬쓸때 여기를 검색해서 있으면 사용불가.
	map<AI_SKILL, _double>			m_map_Skill_Cooldown[AI_TYPE::AI_END];

	wstring					m_wstrNameAliiance;
	wstring					m_wstrNameEnermy_1;
	wstring					m_wstrNameEnermy_2;

	_vec3					m_vFlagPos;
	_bool					m_bCartelReady = false;
	_bool					m_bCartelStart = false;

	ATTACKTYPE				m_eAttackType[AI_TYPE::AI_END];
	//ATTACKTYPE			m_eNext_AttackType[AI_TYPE::AI_END];

	STAGE					m_eStageType;


	Engine::CManagement*	m_pManagement;
	LPDIRECT3DDEVICE9		m_pGraphicDev;	
};

#endif // !_CAIMGR_H__
