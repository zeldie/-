#pragma once

#include "DynamicMeshObject.h"
class CNPC_Red :public CDynamicMeshObject
{
public:
	enum ANIMATION {
		HF_ENFJ_Wait,
		HF_ENFJ_Wait_Habit,
		HF_ENTP_Wait,
		HF_ENTP_Wait_Habit,
		HF_ESFP_Wait,
		HF_ESFP_Wait_Habit,
		HF_ESTJ_Wait,
		HF_ESTJ_Wait_Habit,
		HF_INFP_Wait,
		HF_INFP_Wait_Habit,
		HF_INTJ_Wait,
		HF_INTJ_Wait_Habit,
		HF_ISTJ_Wait,
		HF_ISTJ_Wait_Habit,
		HF_ISTP_Wait,
		HF_ISTP_Wait_Habit,
		HF_EMBreakDance,
		HF_EMDance_1,
		HF_EMDance_3,
		HF_EMDance_4,
		HF_EMHello,
		HF_EMLikeIt,
		HF_EMLoveLetter,
		HF_EMLoveyou_1,
		HF_EMProvoke_1,
		HF_EMProvoke_2,
		HF_EMProvoke_3,
		HF_EMSadness_Loop,
		HF_EMSadness_Start,
		HF_EMVictory_1,
		HF_EMWin_C_Loop,
		HF_EMWin_C_Start,
		HF_Win_A_Loop,
		HF_Win_A_Start,
		HF_Win_B_Loop,
		HF_Win_B_Start,
		HF_EMAngry,
		HF_EMYes,
		HF_EMDance_5
	};

private:
	explicit CNPC_Red(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNPC_Red();

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

	_bool				m_bOne; //ù��°����Ʈ �� �Ұ�
	_bool				m_bTwo; //�ι�°����Ʈ �� �Ұ�
	_bool				m_bMissionPossible; 
	_bool				m_bOneCreateChat; //��ȭâ �ѹ��������ϰ�
	_bool				m_bMissionCompleted; //�̼�(¦���߱�) �� �����ϰ���� �Ӹ� ? ->! �ٲ�鼭 ���� or �����̼�
	_bool				m_bSecondMissionCompleted; //2��° �̼�(�����) �� �����ϰ���� �Ӹ� ? ->! �ٲ�鼭 ���� or �����̼�


	CQuestMgr::NPCTYPE	m_eNPCType;
	_uint				m_iQuestCnt;

	_bool				m_bAllClear; //��� ����Ʈ �����ϸ� true (����õ�����)

private:
	virtual HRESULT							Clone_Component(wstring wstrObjectKey);
	HRESULT									Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CNPC_Red* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void	Free();

};

