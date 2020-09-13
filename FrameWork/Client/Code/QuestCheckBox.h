#pragma once
#ifndef __QuestCheckBox_h__
#define __QuestCheckBox_h__

#include "UIObject.h"

class CQuestCheckBox : public CUIObject
{
public:
	enum CHECKBOXTYPE {RECT,CHECK, CHECKBOXTYPE_END}; 
	enum SEQUENCE {FIRST, SECOND, SEQUENCE_END};

public:
	explicit CQuestCheckBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuestCheckBox();

public:
	//Getter
	CHECKBOXTYPE&		Get_QuestCheckBox() { return m_eQuestCheckBoxType; }
	SEQUENCE&			Get_Sequence() { return m_eSequenceType; }

public:
	HRESULT				Ready_GameObject(CHECKBOXTYPE eType, SEQUENCE eSequence, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	CQuestMgr*			m_pQuestMgr;
	CHECKBOXTYPE		m_eQuestCheckBoxType;
	SEQUENCE			m_eSequenceType;	

	////자리 이동 변수
	//_float fx;
	//_float fy;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CQuestCheckBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev, CHECKBOXTYPE eType, SEQUENCE eSequence, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif