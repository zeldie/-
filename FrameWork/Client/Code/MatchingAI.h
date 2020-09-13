#pragma once

#include "DynamicMeshObject.h"

class CMatchingAI :public CDynamicMeshObject
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
	explicit CMatchingAI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMatchingAI();

public:
	//Setter
	void Set_MatchingState(MATCHINGSTATE eState) { m_eNextMatchingState = eState; }

public:
	HRESULT				Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale);
	HRESULT				Ready_GameObject(_float fX, _float fY);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;
	virtual void		Render_PostEffect(const _double& dTimeDelta) override;

private:
	void				CheckState();
	void				Is_AnimationSetFinish();

private:
	_matrix				m_matProj; //직교투영행렬
	MATCHINGSTATE		m_eCurMatchingState;
	MATCHINGSTATE		m_eNextMatchingState;

private:
	virtual HRESULT							Clone_Component();
	HRESULT									Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CMatchingAI* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale); //원근투영용
	static CMatchingAI* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY); //직교투영용

	virtual void	Free();

};

