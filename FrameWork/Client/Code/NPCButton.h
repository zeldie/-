#pragma once
#ifndef __NPCButton_h__
#define __NPCButton_h__

#include "UIObject.h"
class CNPCButton : public CUIObject
{
public:
	enum BUTTONTYPE{AGREE1,AGREE2, DISAGREE1, DISAGREE2, OUTBUTTON1,OUTBUTTON2, BUTTONTYPE_END}; 
	// 1 ->클릭 전 ... 2-> 클릭 고민중 &후

public:
	explicit CNPCButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNPCButton();

public:
	//Getter
	BUTTONTYPE& Get_ButtonType() { return m_eButtonType; }
	_bool		Get_SceneChange() { return m_bSceneChange; }

public:
	HRESULT				Ready_GameObject(BUTTONTYPE eType, CQuestMgr::NPCTYPE eNPCType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	void				CheckNPCQuest();

private:
	CUIMgr*				m_pUIMgr;
	BUTTONTYPE			m_eButtonType;
	_bool				m_bChoose;
	_bool				m_bSceneChange;

	CQuestMgr::NPCTYPE	m_eNPCType;

	_bool				m_bSound;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CNPCButton*		Create(LPDIRECT3DDEVICE9 pGraphicDev, BUTTONTYPE eType, CQuestMgr::NPCTYPE eNPCType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void			Free();
};

#endif