#pragma once
#ifndef __PlayerHp_h__
#define __PlayerHp_h__

#include "UIObject.h"

class CPlayerObserver;

class CPlayerHp : public CUIObject
{
public:
	enum PLAYERHPTYPE {HP, PREVHP, PLAYERHPTYPE_END};

public:
	explicit CPlayerHp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerHp();

public:
	HRESULT				Ready_GameObject(PLAYERHPTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	//옵저버
	CPlayerObserver*		m_pPlayerObserver;

	PLAYERHPTYPE			m_ePlayerHpType;
	_int					m_iPlayerMaxHp;
	_float					m_fResultValue;

	_uint					m_iHp_Before; //한 프레임 전의 HP
	_bool					m_bLateInit;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CPlayerHp*	Create(LPDIRECT3DDEVICE9 pGraphicDev, PLAYERHPTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif