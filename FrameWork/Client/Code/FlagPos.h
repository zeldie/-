#pragma once

#ifndef __FlagPos_h__
#define __FlagPos_h__

#include "NormalObject.h"

class CFlagPos : public CNormalObject
{
public:
	enum FLAGPOSTYPE{BG, FLAG, FLAGPOSTYPE_END};

public:
	explicit CFlagPos(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlagPos();

public:
	HRESULT					Ready_GameObject(FLAGPOSTYPE eType, Engine::CTransform* pTargetTransform);
	virtual _int			Update_GameObject(const _double& dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void			Render_Geometry(const _double& dTimeDelta) override;

private:
	void					CheckFlagPos();

private:
	Engine::CTransform*		m_pTargetTransformCom;
	FLAGPOSTYPE				m_eFlagPosType;
	_vec3					m_vScale;
	_matrix					m_matProj; //직교투영행렬
	_bool					m_bFixLine;

private:
	HRESULT					Clone_Component();
	HRESULT					Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CFlagPos*		Create(LPDIRECT3DDEVICE9 pGraphicDev, FLAGPOSTYPE eType, Engine::CTransform* pTargetTransform);
	virtual void			Free();
};

#endif