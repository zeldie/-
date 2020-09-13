#pragma once

#ifndef __FlagScoreFlash_h__
#define __FlagScoreFlash_h__

#include "NormalObject.h"

class CFlagScoreFlash : public CNormalObject
{
public:
	enum FLASHSEQUENCETYPE{ FIRST, SECOND, FLASHSEQUENCETYPE_END};

public:
	explicit CFlagScoreFlash(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlagScoreFlash();

public:
	//Setter
	void				Set_Start(_bool bBool) { m_bStart = bBool; }

public:
	HRESULT				Ready_GameObject(FLASHSEQUENCETYPE eType, _float fX, _float fY);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	FLASHSEQUENCETYPE	m_eFlashSequenceType;
	_matrix				m_matProj; //직교투영행렬

	_double				m_dSize;
	_double				m_dTime;
	_bool				m_bStart;
	_bool m_bFirstStart;

public:
	static CFlagScoreFlash*		Create(LPDIRECT3DDEVICE9 pGraphicDev, FLASHSEQUENCETYPE eType, _float fX, _float fY);
	virtual void				Free();
};

#endif