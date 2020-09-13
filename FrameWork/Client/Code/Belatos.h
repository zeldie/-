#pragma once

#include "DynamicMeshObject.h"
#include "Player.h"
#include "Transform.h"

class CBelatos_Missile;
class CPlayerObserver;
class CBelatos final :public CDynamicMeshObject
{
	enum STATE
	{
		// �����̴� ����
		STATE_START,
		STATE_READY,
		STATE_IDLE,
		STATE_MOVE,
		STATE_RUN,
		STATE_BACKJUMP,
		
		// �ǰ�
		STATE_HIT_FRONT,
		STATE_HIT_BACK,

		STATE_FRONT_HIT_UP,
		STATE_FRONT_HIT_DOWN,
		STATE_FRONT_HIT_GROUND,
		STATE_FRONT_HIT_AIRUP,
		STATE_FRONT_HIT_AIRDOWN,

		STATE_BACK_HIT_UP,
		STATE_BACK_HIT_DOWN,
		STATE_BACK_HIT_GROUND,
		STATE_BACK_HIT_AIRUP,
		STATE_BACK_HIT_AIRDOWN,

		STATE_FRONT_STANDUP,
		STATE_BACK_STANDUP,

		// ����
		STATE_SHIELDUP,				// X
		STATE_ATTACK,				// X
		STATE_SWEEP,				// X
		STATE_JUMPATTACK,			// A
		STATE_JUMPSHIELDATTACK,		
		STATE_SHIELDRUSH,			// O
		STATE_MISSILE,				// O
		STATE_THRUST,				// O

		// ��������
		STATE_BREAK,
		STATE_BREAK_1,
		STATE_BREAK_2,
		STATE_BREAK_JUMP,
		STATE_AIRUP,			// �� �ȿ��� STATE_FRONT_HIT_AIRUP / STATE_BACK_HIT_AIRUP �� ���� ���

		STATE_DEAD,
		STATE_END
	};

	enum STATE_JUMP { JUMP_UP, JUMP_DOWN, JUMP_ATTACK, JUMP_BACK, JUMP_END };

private:
	explicit CBelatos(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBelatos();

public:
	virtual HRESULT		Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;
	virtual void		Render_PostEffect(const _double& dTimeDelta) override;
	virtual void		Render_Shadow(const _double& dTimeDelta) override;
	void				LateInit();


	void				Check_State();
	void					Auto_Check_State();
	void					NonAuto_Check_State();
	void				Run_State(const _double & dTimeDelta);

	void				Change_State(STATE eState) { m_eState = eState; m_mapTimer.clear(); }

	void				Select_AttackState();
	void				Check_Dist();

	// �Ѥ��ش� State�϶� ����Ǵ�  �Լ���Ѥ� //
	// ���� �� �̵�
	void				Start(const _double & dTimeDelta);
	void				Ready(const _double & dTimeDelta);

	void				Idle(const _double & dTimeDelta);
	void				Move(const _double & dTimeDelta);
	void				Run(const _double & dTimeDelta);
	void				BackJump(const _double & dTimeDelta);
	// ����
	void				ShieldUp(const _double & dTimeDelta); //��
	void				Attack(const _double & dTimeDelta); // �÷�ġ��
	void				Sweep(const _double & dTimeDelta); // ������ ġ��
	void				ShieldRush(const _double & dTimeDelta); // ���е���
	void				Missile(const _double & dTimeDelta); //�̻���
	void				Thrust(const _double & dTimeDelta); // ���
	// ��������
	void				JumpAttack(const _double & dTimeDelta); // �������
	void				JumpShieldAttack(const _double & dTimeDelta); 
	// �ǰ�
	void				Hit_Front(const _double & dTimeDelta);
	void				Hit_Back(const _double & dTimeDelta);

	void				Front_Hit_Up(const _double & dTimeDelta);
	void				Front_Hit_Down(const _double & dTimeDelta);
	void				Front_Hit_Ground(const _double & dTimeDelta);
	void				Front_Hit_AirUp(const _double & dTimeDelta);
	void				Front_Hit_AirDown(const _double & dTimeDelta);

	void				Back_Hit_Up(const _double & dTimeDelta);
	void				Back_Hit_Down(const _double & dTimeDelta);
	void				Back_Hit_Ground(const _double & dTimeDelta);
	void				Back_Hit_AirUp(const _double & dTimeDelta);
	void				Back_Hit_AirDown(const _double & dTimeDelta);
	
	// �����ٰ� �Ͼ�� ���
	void				Front_StandUp(const _double & dTimeDelta);
	void				Back_StandUp(const _double & dTimeDelta);

	// ��������
	void				Break(const _double & dTimeDelta);
	void				Break_1(const _double & dTimeDelta);
	void				Break_2(const _double & dTimeDelta);
	void				Break_Jump(const _double & dTimeDelta);
	void				AirUp(const _double & dTimeDelta);

	void				Dead(const _double & dTimeDelta);
	//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�//

	// �ѤѤ� Ÿ�̸� ���� �Լ� �ѤѤ� //
	void				Set_Timer(wstring TimerName) { m_mapTimer.emplace(TimerName, 0.f); }
	_double				Get_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return 0; } return m_mapTimer.find(TimerName)->second; }
	void				Start_Timer(wstring TimerName) { m_mapTimer.find(TimerName)->second += Engine::Get_TimeDelta(L"Timer_FPS60"); }
	_bool				IsReady_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return false; } return true; }
	const size_t		Get_TimerSize() { return m_mapTimer.size(); }
	void				Clear_Timer() { m_mapTimer.clear(); }
	void				Delete_Timer(wstring TimerName) { if (m_mapTimer.end() == m_mapTimer.find(TimerName)) { return; } else { auto Obj = m_mapTimer.find(TimerName); m_mapTimer.erase(Obj); } }
	//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�//

	_bool				Jump(STATE_JUMP eState, float fPower, float ColPower, float fGravity, float fAccel, float fDecel, _vec3 _vDir, const _double & dTimeDelta);

	void				View_State();

	// Test : Kyh
	virtual	void		Set_Collision(CBaseObject* pObj, COLLIDER eColliderType);
	virtual void		Set_Collision_Effect(CBaseObject* pObj);
	// Test End

	

	void				Check_SuperAmor(const _double & dTimeDelta);
	void				Check_AirStunTime(const _double & dTimeDelta) { if (m_fAirStunTime > 1) { m_fAirStunTime = 1.f; } }
	void				Check_HitTime(const _double & dTimeDelta);
	void				Check_PlayerDead(const _double & dTimeDelta);

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect, const _double& dTimeDelta);

	void				SaveSoundNum();

private:
	_bool								m_bisStart = false;
	_bool								m_bisReady = false;
	_bool								m_bSound_Dead = false;
	_bool								m_bSound_PlayerDead = false;
	_float								m_fPlayerDeadTime = 0.f;
	_float								m_fHitSoundTime = 0.f;

	_bool								m_bLateInit;
	_bool								m_bAuto = true;							// �ڵ����� �������� ������. true == �ڵ� / false == ����
	_uint								m_iPlayingAniNum;						// ���� Ʋ������ �ִ� ���ϸ��̼� ��ȣ
	STATE								m_eState = STATE_END;					// ���� ����
	STATE								m_eNextState = STATE_END;				// ���� ����
	STATE								m_ePreviousAttackState = STATE_END;		// ���� ���ݻ���
	STATE								m_ePreviousState = STATE_END;			// View_State()�Լ����� STATEȮ�ο� ����
	_double								m_dAnimationPeriod = 0;					// ���� �ִϸ��̼��� ��ü ����ð�
	_int								m_iAttackState = 0;
	_int								m_iPreviousAttack = 99.f;

	_bool								m_bBreak = false;
	_float								m_fBreakTime = 0.f;
	_float								m_fAirStunTime = 0.f;
	_float								m_fDeadTime = 0.f;

	_float								m_fSuperAmorTimer = 0.f;
	_bool								m_bBreak_JumpHit;

	_bool								m_bFrontHit = false;
	_bool								m_bBackHit = false;

	_float								m_fAnimationSpeed = 1.f;

	// �Ѥ����� ���äѤ�
	float								m_fPower = 0.f;
	float								m_fAccel = 0.f;
	float								m_fDecel = 0.f;
	float								m_fDegree = 0.f;
	
	_vec3								m_vJumpDir;
	_float								m_fJump_FirstDist;
	_vec3								m_vJump_TargetPos;

	// �ѤѤѤѤѤѤѤѤ�

	map<wstring, _double>				m_mapTimer;						// Ÿ�̸�
	map<EFFECTID, _double>				m_mapinvincibility;
	//������
	CPlayerObserver*					m_pPlayerObserver;
	CUIMgr*								m_pUIMgr;

	// yh - critical
	_bool								m_bCritical;

	//������
	_double								m_dDissolveCountDown;
	_uint								m_iPass;
	_bool								m_bDissolveStart;


	//�ǰ�
	_bool								m_bIsHit;
	_double								m_dHitTime;

	//������Ʈ
	_bool								m_bUseSpearRim;
	_bool								m_bUseShieldRim;
public:
	static CBelatos* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void	Free();
};

// Collider ���̴� �� ��ġ	Bip001-R-Forearm
//							Bip001-L-Forearm
//							BN_R_Skirt_02
//							BN_L_Skirt_02
//							