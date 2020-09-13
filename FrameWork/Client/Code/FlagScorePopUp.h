#pragma once

#ifndef __FlagScorePopUp_h__
#define __FlagScorePopUp_h__

#include "NormalObject.h"

class CFlagScorePopUp : public CNormalObject
{
public:
	enum FlagScorePopUpTYPE{ BG, BATTLE, FlagScorePopUpTYPE_END};
	enum POPUPCOLOR{BLUE, RED, POPUPCOLOR_END};

public:
	explicit CFlagScorePopUp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlagScorePopUp();

public:
	HRESULT				Ready_GameObject(FlagScorePopUpTYPE eType, POPUPCOLOR eColor, _float fX, _float fY);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	_matrix				m_matProj;
	FlagScorePopUpTYPE	m_eFlagScorePopUpType;
	POPUPCOLOR			m_ePopUpColor;

	_float				m_fAlphaBG;
	_float				m_fAlphaBATTLE;
	_double				m_dTime;
	_bool				m_bTimeStart;
	_bool				m_bRenderTXT;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CFlagScorePopUp*		Create(LPDIRECT3DDEVICE9 pGraphicDev, FlagScorePopUpTYPE eType, POPUPCOLOR eColor,  _float fX, _float fY);
	virtual void				Free();
};

#endif