#pragma once

#ifndef __AIConditionBar_h__
#define __AIConditionBar_h__

#include "NormalObject.h"

class CAIConditionBar : public CNormalObject
{
	// AI들 상태바를 관리하는 클래스, 각자의 아이들을 얘가 한 곳으로 모아주는역할
public:
	explicit CAIConditionBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAIConditionBar();

public:
	HRESULT					Ready_GameObject(CONTROLTYPE eAIType, Engine::CTransform* pTargetTransform);
	virtual _int			Update_GameObject(const _double& dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void			Render_Geometry(const _double& dTimeDelta) override;

private:
	void					CreateCondition(_vec3 vPos, _vec3 vSize);

private:
	_vec3					m_vScale;
	Engine::CTransform*		m_pTargetTransformCom;
	CONTROLTYPE				m_eAIType;

	_bool					m_bLateInit;

private:
	HRESULT					Clone_Component();
	HRESULT					Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CAIConditionBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CONTROLTYPE eAIType, Engine::CTransform* pTargetTransform);
	virtual void			Free();
};

#endif