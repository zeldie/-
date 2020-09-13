#pragma once
#ifndef __AIHP_h__
#define __AIHP_h__

#include "NormalObject.h"

class CAI_Alliance_Observer;
class CAI_Enemy1_Observer;
class CAI_Enemy2_Observer;

class CAIHP : public CNormalObject
{
public:
	enum HPTYPE{ HP, PREVHP, HPTYPE_END};

public:
	explicit CAIHP(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAIHP();

public:
	HRESULT				Ready_GameObject(HPTYPE eType, Engine::CTransform* pParentTransform, CONTROLTYPE eAIType, _vec3 vPos, _vec3 vSize);
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
	HPTYPE					m_eAIHPType;
	_float					m_fResultValue; //감소된 최종 값 
	_int					m_iMaxHp;
	//_bool					m_bLateInit;

	_matrix m_matProj;
private:
	HRESULT					Clone_Component();
	HRESULT					Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CAIHP*			Create(LPDIRECT3DDEVICE9 pGraphicDev, HPTYPE eType, Engine::CTransform* pParentTransform, CONTROLTYPE eAIType, _vec3 vPos, _vec3 vSize);
	virtual void			Free();
};

#endif