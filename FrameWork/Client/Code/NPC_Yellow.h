#pragma once

#include "DynamicMeshObject.h"
class CNPC_Yellow :public CDynamicMeshObject
{
private:
	explicit CNPC_Yellow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNPC_Yellow();

public:
	//Setter
	void				Set_OneCreateChat(_bool bBool=true) { m_bOneCreateChat = bBool; }

public:
	virtual HRESULT		Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;
	virtual void		Render_PostEffect(const _double& dTimeDelta) override;
	virtual void		Render_Shadow(const _double& dTimeDelta) override;

private:
	void				CheckPlayerDist();
	void				CheckQuestCnt();

private:
	CUIMgr*				m_pUIMgr;
	CQuestMgr*			m_pQuestMgr;
	_bool				m_bLateInit;

	_bool				m_bOne; //UI Type �ѹ��� �ٲ��ٶ��
	_bool				m_bMissionPossible; 
	_bool				m_bOneCreateChat; //��ȭâ �ѹ��������ϰ�
	_bool				m_bMissionCompleted; //�̼Ǵ� �����ϰ���� �Ӹ� ? ->! �ٲ�鼭 ���� or �����̼�

	_bool				m_bPotalQCompleted; //ù��° �̼�->��Ż�̵�
	_bool				m_bOnePotalQ;

	CQuestMgr::NPCTYPE	m_eNPCType;
	_uint				m_iQuestCnt;


private:
	virtual HRESULT							Clone_Component(wstring wstrObjectKey);
	HRESULT									Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CNPC_Yellow* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void	Free();

};

