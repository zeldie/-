#pragma once
#ifndef __UltimateSlot_h__
#define __UltimateSlot_h__

#include "UIObject.h"
class CUltimateSlot : public CUIObject
{
public:
	enum ULTIMATETYPE {BASIC, RECHARGE_OUTLINE, OUTLINE, RECHARGE_INSIDE ,ULTIMATETYPE_END};

public:
	explicit CUltimateSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUltimateSlot();

public:
	//Getter
	ULTIMATETYPE&		Get_UltimateType() { return m_eUltimateType; }

public:
	HRESULT				Ready_GameObject(ULTIMATETYPE eType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	ULTIMATETYPE			m_eUltimateType;
	_double					m_dbRechareTime;
	_float					m_fFrame;
	_float					m_fResultValue;


private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ=0.f);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CUltimateSlot*	Create(LPDIRECT3DDEVICE9 pGraphicDev, ULTIMATETYPE eType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif