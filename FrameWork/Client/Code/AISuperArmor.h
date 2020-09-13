#pragma once
#ifndef __AISuperArmor_h__
#define __AISuperArmor_h__

#include "NormalObject.h"

class CAI_Alliance_Observer;
class CAI_Enemy1_Observer;
class CAI_Enemy2_Observer;

class CAISuperArmor : public CNormalObject
{
public:
	enum SUPERARMORTYPE { FULL, EMPTY, SUPERARMORTYPE_END };

public:
	explicit CAISuperArmor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAISuperArmor();

public:
	HRESULT				Ready_GameObject(SUPERARMORTYPE eType, Engine::CTransform* pParentTransform, CONTROLTYPE eAIType, _vec3 vPos, _vec3 vSize);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	//옵저버
	CAI_Alliance_Observer*	m_pAllianceObserver;
	CAI_Enemy1_Observer*	m_pEnemy1_Observer;
	CAI_Enemy2_Observer*	m_pEnemy2_Observer;

	//
	Engine::CTransform*		m_pParentTransformCom;
	CONTROLTYPE				m_eAIType;
	SUPERARMORTYPE			m_eSuperArmor;
	_float					m_fResultValue; //감소된 최종 값 
	_uint					m_iMaxSuperArmor;
	//_bool					m_bLateInit;

	_bool					m_bAISuperArmorRender;

private:
	HRESULT					Clone_Component();
	HRESULT					Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CAISuperArmor*	Create(LPDIRECT3DDEVICE9 pGraphicDev, SUPERARMORTYPE eType, Engine::CTransform* pParentTransform, CONTROLTYPE eAIType, _vec3 vPos, _vec3 vSize);
	virtual void			Free();
};

#endif