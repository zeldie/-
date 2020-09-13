#pragma once
#ifndef __MissionCard_h__
#define __MissionCard_h__

#include "UIObject.h"
class CMissionCard : public CUIObject
{
public:
	enum MISSIONCARD{CTF , CTF_INV, AI, AI_INV, RAID, RAID_INV, MISSIONCARD_END }; //INV : 같은그림인데 반전효과있음 //AI ->돼지천사

public:
	explicit CMissionCard(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMissionCard();

public:
	//Getter
	_bool				Get_Choose() { return m_bChoose; }
	_bool				Get_SceneChange() { return m_bSceneChange; }
	//_bool				Get_CreateCartel() { return m_bCreateCartel; }
	MISSIONCARD&		Get_MissionCardType() { return m_eMissionCard; }

public:
	HRESULT				Ready_GameObject(MISSIONCARD eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	//_bool				CheckAroundMouse(HWND hWnd);
	void				CheckMouseClick();

private:
	MISSIONCARD			m_eMissionCard;
	_bool				m_bChoose; //마우스 근처에 있을 때 true로 바뀌면서 그림바뀜
	_vec2				m_vSize;

	_bool				m_bSceneChange;
	//_bool				m_bCreateCartel;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CMissionCard*		Create(LPDIRECT3DDEVICE9 pGraphicDev, MISSIONCARD eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif