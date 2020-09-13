#pragma once
#ifndef __NoFuncObj_h__
#define __NoFuncObj_h__

#include "UIObject.h"
class CNoFuncObj : public CUIObject
{
public:
	enum NOFUNCOBJ{NOFUNCOBJ_KEYGUIDE, NOFUNCOBJ_PING, NOFUNCOBJ_TRIANGLE, 
		NOFUNCOBJ_WAITINGTXT, NOFUNCOBJ_BOSSNAMETXT, NOFUNCOBJ_MAPTXT,
		NOFUNCOBJ_COOLDOWNTXT, NOFUNCOBJ_STAMINATXT, NOFUNCOBJ_DEFICIENTMPTXT,
		NOFUNCOBJ_APOSTLENAMETXT, NOFUNCOBJ_END};

public:
	explicit CNoFuncObj(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNoFuncObj();

public:
	//Getter
	NOFUNCOBJ&			Get_NoFuncObjType() { return m_eNoFuncObjType; }

public:
	HRESULT				Ready_GameObject(NOFUNCOBJ eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	NOFUNCOBJ			m_eNoFuncObjType;
	_double				m_dTime;
	_float				m_fAlpha;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CNoFuncObj*		Create(LPDIRECT3DDEVICE9 pGraphicDev, NOFUNCOBJ eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void			Free();
};

#endif