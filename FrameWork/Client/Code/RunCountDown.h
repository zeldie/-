#pragma once
#ifndef __RunCountDown_h__
#define __RunCountDown_h__

#include "UIObject.h"
class CRunCountDown : public CUIObject
{
public:
	enum RunCountDownType{ RUNCOUNTDOWNTYPE_GAMESTART, RUNCOUNTDOWNTYPE_REVIVAL, RUNCOUNTDOWNTYPE_END};

public:
	explicit CRunCountDown(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRunCountDown();

public:
	//Setter
	void				Set_TimeOver(_bool bBool) { m_bTimeOver = bBool; }

public:
	HRESULT				Ready_GameObject(RunCountDownType eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	CUIMgr*				m_pUIMgr;
	RunCountDownType	m_eRunCountDownType;
	_uint				m_iNumber;
	_double				m_dTime;

	_bool				m_bTimeOver; //부활여부 카운트다운시 구분필요해서 만든 변수(시간초과인지. 버튼을 눌러서인지)

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CRunCountDown*		Create(LPDIRECT3DDEVICE9 pGraphicDev, RunCountDownType eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif