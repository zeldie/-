#pragma once
#ifndef __FlagBackBar_h__
#define __FlagBackBar_h__

#include "UIObject.h"
class CFlagBackBar : public CUIObject
{
public:
	enum BACKBARTYPE {BLUE, RED, GAGE, BACKBARTYPE_END};
	enum GAGECOLOR { GAGECOLOR_BLUE, GAGECOLOR_RED, GAGECOLOR_END};

public:
	explicit CFlagBackBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlagBackBar();

public:
	HRESULT				Ready_GameObject(BACKBARTYPE eType, GAGECOLOR eColor, _double dTime, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	BACKBARTYPE			m_eBackBarType;
	GAGECOLOR			m_eGageColor;

	_double				m_dTime;
	_double				m_dMaxTime;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CFlagBackBar*	Create(LPDIRECT3DDEVICE9 pGraphicDev, BACKBARTYPE eType,GAGECOLOR eColor, _double dTime, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void			Free();
};

#endif