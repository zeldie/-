#pragma once
#ifndef __RunPlayerHP_h__
#define __RunPlayerHP_h__

#include "UIObject.h"

class CRunPlayerObserver;
class CRunPlayerHP : public CUIObject
{
public:
	enum BARTYPE { HP, BACKBAR, BARTYPE_END };

public:
	explicit CRunPlayerHP(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRunPlayerHP();

public:
	HRESULT				Ready_GameObject(BARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	//¿ÉÀú¹ö
	CRunPlayerObserver*		m_pRunPlayerObserver;

	BARTYPE					m_eBarType;
	_int					m_iRunPlayerMaxHp;
	_float					m_fResultValue;
	_bool					m_bLateInit;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CRunPlayerHP*	Create(LPDIRECT3DDEVICE9 pGraphicDev, BARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ = 0.f);
	virtual void			Free();
};

#endif