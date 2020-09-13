#pragma once
#ifndef __CRunPlayer_H_
#define __CRunPlayer_H_

#include "DynamicMeshObject.h"
#include "NaviMeshRun.h"

class CRunPlayerState;
class CRunPlayer :public CDynamicMeshObject
{
public:
	enum RunPlayerStateEnum
	{
		STATE_DYING,
		STATE_IDLE_1,	// 웃는동작
		STATE_IDLE_2,
		STATE_RUN,
		STATE_START,	// 이상하다 조금 짤려있음
		STATE_TURN,
		STATE_WAIT,		// 이걸 아이들상태로 쓸 예정
		STATE_WALK,
		STATE_DOWN_END,
		STATE_DOWN_LOOP,
		STATE_DOWN_START,
		STATE_GROGGY_END,
		STATE_GROGGY_LOOP,
		STATE_GROGGY_START,		// 그로기->다운 모두 이어지는 동작, 그로기에서 바로 일어날수도있고 기절할수도있다
		STATE_SKILL_0_PULLSWING,	// 창 바닥에 강하게 내려친다
		STATE_SKILL_1_JUMP_TURN_SPIKE,				// 점프해서 한바퀴 돌면서 창으로 내려찍는다
		STATE_SKILL_2_TRUN_SWING,		// 뒤로 돌면서 창 여러번 휘두른다
		STATE_SKILL_3_OVERSWING,		// 창으로 홈런치는듯한동작
		STATE_SKILL_4_POINT,		// 창으로 지목한다
		STATE_SKILL_5_SPIKE,		// 왼손으로 창 바닥에 내려찍는다. 약공격
		STATE_SKILL_6_END,
		STATE_SKILL_6_LOOP,
		STATE_SKILL_6_START,	// 창들고 공중에서 돌진
		STATE_SKILL_6_STOP,
		STATE_SKILL_7_JUMP_ATTACK,		// 점프했다가 엉덩방아 찍는다
		STATE_SKILL_8_POWER_SPIKE,		// 창으로 바닥 강하게 내려찍는다
		STATE_SKILL_9_PRAY,		// 꿀렁거리면서 창 위로 올린다
		STATE_SKILL_10_SPIN_END,		//
		STATE_SKILL_10_SPIN_LOOP,	// 턴하면서 빙글빙글 돈다
		STATE_SKILL_10_SPIN_START,	//
		STATE_SKILL_11_JUMP_HOORAY,		// 공중으로 떠오르면서 기지개
		STATE_SKILL_12_POWER_DOWN,		// 땅 찍으면서 주저앉는다
		STATE_END
	};
private:
	explicit CRunPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRunPlayer();

public:
	//Setter
	void Set_MoveState(RUNPLAYERMOVESTATE eState) { m_eMoveState = eState; }
	void Set_OneCreateUI(_bool bBool) { m_bOneCreateUI = bBool; }
	void Set_GameStart(_bool bBool) { m_bGameStart = bBool; }
	void	Set_Shooting(_bool bBool) { m_bShooting = bBool; }

public:
	virtual HRESULT		Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;
	virtual void		Render_PostEffect(const _double& dTimeDelta) override;
	virtual void		Render_Shadow(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	void				Update_State();
	void				Change_State(RunPlayerStateEnum eState);
	void				ObserverNotify();	
	void				Running(const _double& dTimeDelta);	// 플레이어 달리기,조작
	void				Gravity(const _double& dTimeDelta);	// 중력,점프
	void				Power_Big(const _double& dTimeDelta);	// 플레이어 파워업 -> 거대화
	void				Power_SpeedUp(const _double& dTimeDelta);	// 플레이어 파워업 -> 스피드업
	void				Item_Looting(RUNGAME_ITEMTYPE eItemType);	// 아이템 획득처리 함수
	void				SavePoint_Set();	// 세이브 포인트 설정
	void				RunningFinish();

	void				Check_HitTime(const _double & dTimeDelta);
public:
	void				RunReady();	// 플레이어 부활지점으로 이동, 대기상태
	void				RunStart();	// 플레이어 달리기 시작

private:
	void				CollisionCheck();	// 런게임전용 충돌체크
	void				CollisionItem();	// 아이템 충돌
	void				CollisionResult();	// 충돌 이벤트처리

private:
	CRunPlayerState*				m_pState;	// 애니메이션 상태패턴 클래스포인터
	RUNPLAYERMOVESTATE				m_eMoveState; // 달리기 현재 상태
	RunPlayerStateEnum				m_eRunPlayerState;	// 애니메이션 상태패턴 enum값
	_double							m_dbAnimationSpeed;	// 애니메이션 재생속도
	_double							m_dbAniCtrlTime;	// 애니메이션 자르는 값
	CUIMgr*							m_pUIMgr;
	_double							m_dProgress;	// 현재 위치 (현재진행도)
	LINETYPE						m_eLineType;	// 현재 라인

	_vec3							m_vJump;	// 점프시 더해줄 위치값
	_float							m_fAccel;	// 점프 가속도
	_bool							m_bJump;	// 점프상태인지 true == 점프 , false == 아님
	RUN_LINE_SECT					m_eSect;	// 현재 어느구간에 있는지
	_double							m_dSavePonint;	// 부활위치 (진행도값) // 이넘으로 바꿀예정.		// 지금 안쓰는중
	_double							m_dPlayerSpeed;	// 플레이어의 자체 속도 ( 아이템등으로 변하는 속도 )
	_double							m_dLineAccel;	// 라인 가속도
	RUNPLAYER_POWER					m_ePower;		// 플레이어 파워상태 (일반, 거대화, 비행)
	_double							m_dItem_durationTime_Big;	// 거대화 아이템 지속시간;
	_double							m_dItem_durationTime_SpeedUp;	// 스피드업 아이템 지속시간;
	_double							m_dCollSlow;	// 충돌시 느려지는 배수
	_double							m_dCollSlowTime;	// 충돌시 느려지는거 지속시간
	_bool							m_bImmortal;	// 무적 치트
	_bool							m_bShooting;	// 슈팅스테이지 변신 더미용


	_uint							m_iPass;

	//희정 추가
	_bool							m_bOneCreateUI; 
	_uint							m_iDeathCnt;
	_bool							m_bGameStart; //게임시작시 한번만!

	//피격
	_bool							m_bIsHit;
	_double							m_dHitTime;

public:
	static CRunPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void	Free();
};

#endif // !__CApostle_h__
