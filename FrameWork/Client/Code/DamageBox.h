#pragma once

#ifndef __DamageBox_h__
#define __DamageBox_h__

#include "NormalObject.h"

class CDamageBox : public CNormalObject
{
public:
	enum DAMAGEBOXTYPE { BASIC, CRITICAL, DAMAGEBOX_END };


public:
	explicit CDamageBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDamageBox();

public:
	HRESULT					Ready_GameObject(Engine::CTransform* pTargetTransform, DAMAGEBOXTYPE eType, _uint iDamage);
	virtual _int			Update_GameObject(const _double& dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void			Render_Geometry(const _double& dTimeDelta) override;

private:
	void					CreateNumber(DAMAGEBOXTYPE eType,  _vec3 vPos, _vec3 vSize, _uint iDamage);
	void					CalculateDigitCnt(_uint iDamage);

private:
	DAMAGEBOXTYPE			m_eDamageBoxType;
	_uint					m_iDamage;	//받은 데미지
	_uint					m_iDigitCnt;
	_vec3					m_vScale;
	Engine::CTransform*		m_pTargetTransformCom;
	_matrix					m_matCamWorld;
	
	_bool					m_bLateInit;
	_double					m_dTime;


private:
	HRESULT					Clone_Component();
	HRESULT					Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CDamageBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTargetTransform, DAMAGEBOXTYPE eType, _uint iDamage);
	virtual void			Free();
};

#endif