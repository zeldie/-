#pragma once

#include "PlayerState.h"
#include "CS_State.h"
#include "DualSword.h"
class CCsState;
class CDualSword;

class CDS_State : public CPlayerState
{
public:
	explicit CDS_State();
	virtual ~CDS_State();

public:
	virtual void	Enter(CPlayer* pPlayer);
	virtual void	Update(CPlayer* pPlayer, const _double dTimeDelta) override;
	virtual void	ChangeState(CPlayerState* pState) override;

	void			Check_Key(CPlayer * pPlayer, const _double dTimeDelta);
	void			Reserve_State(CPlayer * pPlayer, const _double dTimeDelta);
	void			Run_State(CPlayer * pPlayer, const _double dTimeDelta);
	void			End_DS_State(CPlayer * pPlayer);

public:
					// ��Ÿ 5��
	void			Attack_Normal_5(CPlayer * pPlayer, const _double dTimeDelta);
					// ��Ŭ�� ��������
	void			Attack_Charge(CPlayer * pPlayer, const _double dTimeDelta);
					// ���� �޺�
	void			Air_Combo(CPlayer * pPlayer, const _double dTimeDelta);
					// E ��ų
	void			E_Skill(CPlayer * pPlayer, const _double dTimeDelta);
					// R ��ų
	void			R_Skill(CPlayer * pPlayer, const _double dTimeDelta);
					// F ��ų
	void			F_Skill(CPlayer * pPlayer, const _double dTimeDelta);
					// Q ��ų
	void			Q_Skill(CPlayer * pPlayer, const _double dTimeDelta);

public:
	// �ѤѤ� Ÿ�̸� ���� �Լ� �ѤѤ� //
	void				Set_Timer(wstring TimerName) { m_mapTimer.emplace(TimerName, 0.f); }
	_double				Get_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return 0; } return m_mapTimer.find(TimerName)->second; }
	void				Start_Timer(wstring TimerName) { m_mapTimer.find(TimerName)->second += Engine::Get_TimeDelta(L"Timer_FPS60"); }
	_bool				IsReady_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return false; } return true; }
	const size_t		Get_TimerSize() { return m_mapTimer.size(); }
	void				Clear_Timer() { m_mapTimer.clear(); }
	void				Delete_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return; } else { auto Obj = m_mapTimer.find(TimerName); m_mapTimer.erase(Obj); } }
	//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�//

private:
	map<wstring, _double>				m_mapTimer;						// Ÿ�̸�
	_double								m_dTimer = 0;


private:
	//_bool					m_bIsReserve;			// Value for next state check

	_bool					m_bSecondRocBreak;		// Check it out skill Rockbreak
	_bool					m_bIsMinimumTime;		// BASICCOMBOEX2�� �ּ� ���� �ð� üũ
	_bool					m_bAfterEffect;
	//CPlayer::PLAYER_STATE	m_eReserveState;		// Value for reserve to next state

	//_double					m_dbMotionCancel;		// Check time value for playing motion cancel
	_double					m_dbBasicLoopTime;		// Check time value for basiccombo2 finish
	_double					m_dbIsChargingTime;		// Minimum value to charging
	_double					m_dbWaitingStartMotion;	// Branch RB Attack- Charging or RockBreak

private:											// Const value
	const _double m_dbIsBasicCombo2FinishTime = 5;
};