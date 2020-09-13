#pragma once

#ifndef __FlagCountDown_h__
#define __FlagCountDown_h__

#include "NormalObject.h"

class CFlagCountDown : public CNormalObject
{
public:
	enum TEXTYPE{ COUNTDOWN, TEXT, TEXTYPE_END};

public:
	explicit CFlagCountDown(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlagCountDown();

public:
	HRESULT				Ready_GameObject(TEXTYPE eType, _float _fX, _float _fY);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	TEXTYPE				m_eTexType;
	_matrix				m_matProj; //직교투영행렬
	_double				m_dTime;
	_tchar				m_szCountDown[MIN_STR];
	_float				m_fRotAngle;

public:
	static CFlagCountDown*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTYPE eType, _float _fX, _float _fY);
	virtual void			Free();
};

#endif