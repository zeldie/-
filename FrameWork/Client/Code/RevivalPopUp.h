#pragma once
#ifndef __RevivalPopUp_h__
#define __RevivalPopUp_h__

#include "UIObject.h"
class CRevivalPopUp : public CUIObject
{
public:
	explicit CRevivalPopUp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRevivalPopUp();

public:
	HRESULT				Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	_double				m_dTime;
	_tchar				m_szCountDown[MIN_STR];

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CRevivalPopUp*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif