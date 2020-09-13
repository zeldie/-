#pragma once
#ifndef __CApostle_h__
#define __CApostle_h__

#include "DynamicMeshObject.h"
#include "Player.h"

class CPlayerObserver;
class CApostleState;
class CApostle :public CDynamicMeshObject
{
public:
	enum ApostleStateEnum
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
	explicit CApostle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CApostle();

public:
	virtual HRESULT		Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;
	virtual void		Render_PostEffect(const _double& dTimeDelta) override;
	virtual void		Render_Shadow(const _double& dTimeDelta) override;
private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect, const _double& dTimeDelta);
	void				Update_State();
	void				Change_State(ApostleStateEnum eState);
	void				Update_Pattern();
	void				ObserverNotify();
	void				Check_HitTime(const _double & dTimeDelta);
public:
	virtual	void		Set_Collision(CBaseObject* pObj, COLLIDER eColliderType);
	void				Set_Render(_bool bRender) { m_bIsRender = bRender; }

private:
	virtual	void		Set_Collision_Effect(CBaseObject* pObj);



private:
	CPlayerObserver*				m_pPlayerObserver;
	map<EFFECTID, _double>			m_mapinvincibility;
	CApostleState*					m_pState;
	ApostleStateEnum				m_eApostleState;
	_double							m_dbAnimationSpeed;
	_double							m_dbAniCtrlTime;
	_uint							m_iPhase;
	_double							m_dbIdleTime;	
	_double							m_dbAddDelay;	
	_uint							m_iPatternLoop;	
	CUIMgr*							m_pUIMgr;
	_uint							m_iPattern;	//전투 패턴
	_bool							m_bIsRender;	// 죽고 플레이어 변신하기위해 렌더 끄는거

	_bool							m_bIsStart;	// 전투 시작전 대기상태
	_bool							m_bCutscene; // 컷씬
	_double							m_dbCutscene; // Test

	_uint							m_iPass;
	_bool							m_bDying;
	//피격
	_bool							m_bIsHit;
	_double							m_dHitTime;

	//UI 한번만 돌게
	_bool							m_bCreateUI;

	//디졸브
	_double							m_dDissolveCountDown;
	_bool							m_bDissolveStart;
public:
	static CApostle* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void	Free();
};

#endif // !__CApostle_h__
