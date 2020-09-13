#pragma once
#ifndef __QuestBackBar_h__
#define __QuestBackBar_h__

#include "UIObject.h"

class CQuestBackBar : public CUIObject
{
public:
	enum QUESTBACKBARTYPE {CHAT, QUEST, NPCPICTURE, NPCPICTURE_YELLOW, QUESTBACKBAR_END}; //NPCPICTURE->RED

public:
	explicit CQuestBackBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuestBackBar();

public:
	//Getter
	QUESTBACKBARTYPE&	Get_QuestBackBarType() { return m_eQuestBackBarType; }

public:
	HRESULT				Ready_GameObject(QUESTBACKBARTYPE eType, CQuestMgr::NPCTYPE eNPCType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	void				CheckQuestNarration();
	void				CheckQuest();

private:
	CQuestMgr*			m_pQuestMgr;
	QUESTBACKBARTYPE	m_eQuestBackBarType;
	_bool				m_bLateInit;
	
	CQuestMgr::NPCTYPE	m_eNPCType;
	wstring				m_wstrQuestNarration;

	_bool				m_bTwoLine; //한미션이 두줄됬을때 적용해야하는 위치..

	////자리 이동 변수
	//_float fx;
	//_float fy;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CQuestBackBar*	Create(LPDIRECT3DDEVICE9 pGraphicDev, QUESTBACKBARTYPE eType, CQuestMgr::NPCTYPE eNPCType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif