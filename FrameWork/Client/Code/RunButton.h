#pragma once
#ifndef __RunButton_h__
#define __RunButton_h__

#include "UIObject.h"
class CRunButton : public CUIObject
{
public:
	enum BUTTONTYPE{NONE0, NONE1, NONE2, EXIT1, EXIT2, START1, START2, NONE3, NONE4, BUTTONTYPE_END};
	// 1 ->클릭 전 ... 2-> 클릭 고민중 & 후

public:
	explicit CRunButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRunButton();
	
public:
	//Getter
	BUTTONTYPE& Get_ButtonType() { return m_eButtonType; }

public:
	HRESULT				Ready_GameObject(BUTTONTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	CUIMgr*				m_pUIMgr;
	BUTTONTYPE			m_eButtonType;
	_bool				m_bChoose;
	_bool				m_bOneClick;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CRunButton*		Create(LPDIRECT3DDEVICE9 pGraphicDev, BUTTONTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif