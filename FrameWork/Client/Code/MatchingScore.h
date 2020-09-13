#pragma once
#ifndef __MatcingScore_h__
#define __MatcingScore_h__

#include "UIObject.h"

class CMatchingGameObserver;
class CMatchingScore : public CUIObject
{
public:
	enum DIGITTYPE { UNITS, TENS, DIGITTYPE_END };
	//enum TURN {PLAYER, AI, TURN_END };

public:
	explicit CMatchingScore(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMatchingScore();

public:
	HRESULT						Ready_GameObject(CMatchingMgr::MATCHINGTURN eType, DIGITTYPE eDigitType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int				Update_GameObject(const _double& dTimeDelta) override;
	virtual _int				LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void				Render_Geometry(const _double& dTimeDelta) override;

private:
	//¿ÉÀú¹ö
	CMatchingGameObserver*		m_pMatchingGameObserver;
	_bool						m_bLateInit;

	CMatchingMgr::MATCHINGTURN	m_eTurn;
	DIGITTYPE					m_eDigitType;
	_uint						m_iNumber;

private:
	HRESULT						Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT						Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CMatchingScore*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CMatchingMgr::MATCHINGTURN eType, DIGITTYPE eDigitType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif