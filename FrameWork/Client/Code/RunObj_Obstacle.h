#pragma once

#include "StaticMeshObject.h"
class CRunObj_Obstacle : public CStaticMeshObject
{
private:
	explicit CRunObj_Obstacle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRunObj_Obstacle();

public:
	HRESULT			Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _vec3* vSelfRotate, RUN_LINE_SECT eSect,
		LINETYPE eLineType, RUNGAMEOBJECT_COLL eCollType, _double dProgress, _double dRadius);
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual void	Render_Depth(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta)override;
	virtual void	Render_Shadow(const _double& dTimeDelta)override;
private:
	HRESULT			Clone_Component(wstring wstrObjectKey);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	RUN_LINE_SECT			Get_LineSect() { return m_eSect; }
	LINETYPE				Get_LineType() { return m_eLineType; }
	RUNGAMEOBJECT_COLL		Get_CollType() { return m_eObjCollType; }
	_double					Get_Progress() { return m_dProgress; }
	_double					Get_Radius() { return m_dRadius; }
	_bool					Get_Fly() { return m_bFly; }
	_bool					Get_Dissolve() { return m_bDissolveStart; }
	
	void					Set_Collision(RUNPLAYER_POWER eColl);
private:
	RUN_LINE_SECT			m_eSect;	// ����
	LINETYPE				m_eLineType;	// ���� ��ġ
	RUNGAMEOBJECT_COLL		m_eObjCollType;	// �浹 Ÿ��
	_double					m_dProgress;	// ���൵
	_double					m_dRadius;		// ���൵ ������ (+- �浹����)
	_bool					m_bFly;		// �÷��̾� �Ŵ��ѻ��¿� �ε����� ���ư�
	_double					m_dDeadTiem;	// ���ư��� ���ʵڿ� ������
	_vec3					m_vDir;	// ���ư� ����

	//������
	_double								m_dDissolveCountDown;
	_uint								m_iPass;
	_bool								m_bDissolveStart;

public:
	static	CRunObj_Obstacle*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale,
		_vec3* vSelfRotate = &_vec3(0.f, 0.f, 0.f), RUN_LINE_SECT eSect = SECT_1, LINETYPE eLineType = LINE_BOTTOM_MID, RUNGAMEOBJECT_COLL eCollType = OBJ_COLL_RUN, _double dProgress = 0.0, _double dRadius = 0.0);
	virtual void			Free();
};

