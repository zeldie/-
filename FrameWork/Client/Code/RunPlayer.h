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
	void				Running(const _double& dTimeDelta);	// �÷��̾� �޸���,����
	void				Gravity(const _double& dTimeDelta);	// �߷�,����
	void				Power_Big(const _double& dTimeDelta);	// �÷��̾� �Ŀ��� -> �Ŵ�ȭ
	void				Power_SpeedUp(const _double& dTimeDelta);	// �÷��̾� �Ŀ��� -> ���ǵ��
	void				Item_Looting(RUNGAME_ITEMTYPE eItemType);	// ������ ȹ��ó�� �Լ�
	void				SavePoint_Set();	// ���̺� ����Ʈ ����
	void				RunningFinish();

	void				Check_HitTime(const _double & dTimeDelta);
public:
	void				RunReady();	// �÷��̾� ��Ȱ�������� �̵�, ������
	void				RunStart();	// �÷��̾� �޸��� ����

private:
	void				CollisionCheck();	// ���������� �浹üũ
	void				CollisionItem();	// ������ �浹
	void				CollisionResult();	// �浹 �̺�Ʈó��

private:
	CRunPlayerState*				m_pState;	// �ִϸ��̼� �������� Ŭ����������
	RUNPLAYERMOVESTATE				m_eMoveState; // �޸��� ���� ����
	RunPlayerStateEnum				m_eRunPlayerState;	// �ִϸ��̼� �������� enum��
	_double							m_dbAnimationSpeed;	// �ִϸ��̼� ����ӵ�
	_double							m_dbAniCtrlTime;	// �ִϸ��̼� �ڸ��� ��
	CUIMgr*							m_pUIMgr;
	_double							m_dProgress;	// ���� ��ġ (�������൵)
	LINETYPE						m_eLineType;	// ���� ����

	_vec3							m_vJump;	// ������ ������ ��ġ��
	_float							m_fAccel;	// ���� ���ӵ�
	_bool							m_bJump;	// ������������ true == ���� , false == �ƴ�
	RUN_LINE_SECT					m_eSect;	// ���� ��������� �ִ���
	_double							m_dSavePonint;	// ��Ȱ��ġ (���൵��) // �̳����� �ٲܿ���.		// ���� �Ⱦ�����
	_double							m_dPlayerSpeed;	// �÷��̾��� ��ü �ӵ� ( �����۵����� ���ϴ� �ӵ� )
	_double							m_dLineAccel;	// ���� ���ӵ�
	RUNPLAYER_POWER					m_ePower;		// �÷��̾� �Ŀ����� (�Ϲ�, �Ŵ�ȭ, ����)
	_double							m_dItem_durationTime_Big;	// �Ŵ�ȭ ������ ���ӽð�;
	_double							m_dItem_durationTime_SpeedUp;	// ���ǵ�� ������ ���ӽð�;
	_double							m_dCollSlow;	// �浹�� �������� ���
	_double							m_dCollSlowTime;	// �浹�� �������°� ���ӽð�
	_bool							m_bImmortal;	// ���� ġƮ
	_bool							m_bShooting;	// ���ý������� ���� ���̿�


	_uint							m_iPass;

	//���� �߰�
	_bool							m_bOneCreateUI; 
	_uint							m_iDeathCnt;
	_bool							m_bGameStart; //���ӽ��۽� �ѹ���!

	//�ǰ�
	_bool							m_bIsHit;
	_double							m_dHitTime;

public:
	static CRunPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void	Free();
};

#endif // !__CApostle_h__
