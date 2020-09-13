#pragma once
#ifndef __HpNumber_h__
#define __HpNumber_h__

#include "UIObject.h"

class CHpNumber : public CUIObject
{
public:
	enum DIGITTYPE{UNITS, TENS, HUNDREDS, THOUSANDS, DIGITTYPE_END};

public:
	explicit CHpNumber(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHpNumber();

public:
	//Setter
	void				Set_LineCnt(_uint iLineCnt) { m_iLineCnt = iLineCnt; }

public:
	HRESULT				Ready_GameObject(DIGITTYPE eType, _uint iLineCnt, _uint iNumber/*, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ*/);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	void				CalculateDigitCnt(_uint iLineCnt);
	void				CheckNumber();

private:
	DIGITTYPE			m_eDigitType;
	_uint				m_iNumber;

	_uint				m_iLineCnt;
	_uint				m_iDigitCnt; //몇자리수인지

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CHpNumber*	Create(LPDIRECT3DDEVICE9 pGraphicDev, DIGITTYPE eType, _uint iLineCnt, _uint iNumber/*, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f*/);
	virtual void		Free();
};

#endif