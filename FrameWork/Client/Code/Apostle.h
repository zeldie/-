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
		STATE_IDLE_1,	// ���µ���
		STATE_IDLE_2,
		STATE_RUN,
		STATE_START,	// �̻��ϴ� ���� ©������
		STATE_TURN,
		STATE_WAIT,		// �̰� ���̵���·� �� ����
		STATE_WALK,
		STATE_DOWN_END,			
		STATE_DOWN_LOOP,
		STATE_DOWN_START,
		STATE_GROGGY_END,
		STATE_GROGGY_LOOP,
		STATE_GROGGY_START,		// �׷α�->�ٿ� ��� �̾����� ����, �׷α⿡�� �ٷ� �Ͼ�����ְ� �����Ҽ����ִ�
		STATE_SKILL_0_PULLSWING,	// â �ٴڿ� ���ϰ� ����ģ��
		STATE_SKILL_1_JUMP_TURN_SPIKE,				// �����ؼ� �ѹ��� ���鼭 â���� ������´�
		STATE_SKILL_2_TRUN_SWING,		// �ڷ� ���鼭 â ������ �ֵθ���
		STATE_SKILL_3_OVERSWING,		// â���� Ȩ��ġ�µ��ѵ���
		STATE_SKILL_4_POINT,		// â���� �����Ѵ�
		STATE_SKILL_5_SPIKE,		// �޼����� â �ٴڿ� ������´�. �����
		STATE_SKILL_6_END,
		STATE_SKILL_6_LOOP,
		STATE_SKILL_6_START,	// â��� ���߿��� ����
		STATE_SKILL_6_STOP,	
		STATE_SKILL_7_JUMP_ATTACK,		// �����ߴٰ� ������� ��´�
		STATE_SKILL_8_POWER_SPIKE,		// â���� �ٴ� ���ϰ� ������´�
		STATE_SKILL_9_PRAY,		// �ܷ��Ÿ��鼭 â ���� �ø���
		STATE_SKILL_10_SPIN_END,		//
		STATE_SKILL_10_SPIN_LOOP,	// ���ϸ鼭 ���ۺ��� ����
		STATE_SKILL_10_SPIN_START,	//
		STATE_SKILL_11_JUMP_HOORAY,		// �������� �������鼭 ������
		STATE_SKILL_12_POWER_DOWN,		// �� �����鼭 �����ɴ´�
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
	_uint							m_iPattern;	//���� ����
	_bool							m_bIsRender;	// �װ� �÷��̾� �����ϱ����� ���� ���°�

	_bool							m_bIsStart;	// ���� ������ ������
	_bool							m_bCutscene; // �ƾ�
	_double							m_dbCutscene; // Test

	_uint							m_iPass;
	_bool							m_bDying;
	//�ǰ�
	_bool							m_bIsHit;
	_double							m_dHitTime;

	//UI �ѹ��� ����
	_bool							m_bCreateUI;

	//������
	_double							m_dDissolveCountDown;
	_bool							m_bDissolveStart;
public:
	static CApostle* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void	Free();
};

#endif // !__CApostle_h__
