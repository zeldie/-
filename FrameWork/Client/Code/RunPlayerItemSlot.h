#pragma once
#ifndef __RunPlayerItemSlot_h__
#define __RunPlayerItemSlot_h__

#include "UIObject.h"

class CRunPlayerObserver;
class CRunPlayerItemSlot : public CUIObject
{
public:
	enum ITEMSLOT {BASIC, RECHARGE, OUTLINE, ITEMSLOT_END};
	enum ITEMTYPE{BIG, SPEEDUP, ITEMTYPE_END};
public:
	explicit CRunPlayerItemSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRunPlayerItemSlot();

public:
	HRESULT				Ready_GameObject(ITEMSLOT eType, ITEMTYPE eItemType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	//¿ÉÀú¹ö
	CRunPlayerObserver*		m_pRunPlayerObserver;

	ITEMSLOT			m_eItemSlot;
	ITEMTYPE			m_eItemType;
	RUNPLAYER_POWER		m_eRunPlayerPower;
	_bool				m_bLateInit;

	_double				m_dMaxDuration;
	_double				m_dDuration;

	_bool				m_bOne;
	_bool				m_bOtherOne;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ=0.f);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CRunPlayerItemSlot*	Create(LPDIRECT3DDEVICE9 pGraphicDev, ITEMSLOT eTyep, ITEMTYPE eItemType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif