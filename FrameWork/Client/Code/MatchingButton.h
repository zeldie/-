#pragma once
#ifndef __MatchingButton_h__
#define __MatchingButton_h__

#include "UIObject.h"
class CMatchingButton : public CUIObject
{
public:
	enum BUTTONTYPE{NONE0, NONE1, NONE2, EXIT1, EXIT2, START1, START2, RESET1, RESET2, BUTTONTYPE_END}; //RESET1, RESET2 --> 퍼즐에서 쓸 그림들
	// 1 ->클릭 전 ... 2-> 클릭 고민중 & 후

public:
	explicit CMatchingButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMatchingButton();

public:
	//Getter
	BUTTONTYPE& Get_ButtonType() { return m_eButtonType; }
	_bool		Get_GoLobby() { return m_bGoLobby; }

public:
	HRESULT				Ready_GameObject(BUTTONTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	BUTTONTYPE			m_eButtonType;
	_bool				m_bChoose;
	_bool				m_bGoLobby;
	_bool				m_bGoStart;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CMatchingButton*		Create(LPDIRECT3DDEVICE9 pGraphicDev, BUTTONTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif