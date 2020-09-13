#pragma once
#ifndef __Yellow_h__
#define __Yellow_h__

#include "UIObject.h"
class CYellow : public CUIObject
{
public:
	enum YELLOWTYPE{GOLEFT, GORIGHT, YELLOWTYPE_END};

public:
	explicit CYellow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CYellow();

public:
	HRESULT				Ready_GameObject(YELLOWTYPE eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	YELLOWTYPE			m_eYellowType;
	_double				m_dTime;
	_float				m_fResultValue;
	_float				m_fAlpha;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CYellow*		Create(LPDIRECT3DDEVICE9 pGraphicDev, YELLOWTYPE eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif