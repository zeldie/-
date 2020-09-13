#pragma once
#ifndef __MatcingTurn_h__
#define __MatcingTurn_h__

#include "UIObject.h"

class CMatchingGameObserver;
class CMatchingTurn : public CUIObject
{
public:
	explicit CMatchingTurn(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMatchingTurn();

public:
	//Getter
	CMatchingMgr::MATCHINGTURN& Get_Turn() { return m_eTurn; }

public:
	HRESULT						Ready_GameObject(CMatchingMgr::MATCHINGTURN eType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int				Update_GameObject(const _double& dTimeDelta) override;
	virtual _int				LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void				Render_Geometry(const _double& dTimeDelta) override;

private:
	//¿ÉÀú¹ö
	CMatchingGameObserver*		m_pMatchingGameObserver;
	_bool						m_bLateInit;

	CMatchingMgr::MATCHINGTURN	m_eTurn;
	_double						m_dTime;
	_float						m_fAlpha;

private:
	HRESULT						Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT						Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CMatchingTurn*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CMatchingMgr::MATCHINGTURN eType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif