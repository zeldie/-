#pragma once
#ifndef __OverHeadEffect_h__
#define __OverHeadEffect_h__

#include "NormalObject.h"

class COverHeadEffect : public CNormalObject
{
public:
	enum KARMATYPE{KARMA_DS, KARMA_TS, KARMA_ORB, KARMA_LB, KARMA_END};

public:
	explicit COverHeadEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COverHeadEffect();

public:
	HRESULT				Ready_GameObject(KARMATYPE eType, Engine::CTransform* pTargetTransform);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	KARMATYPE			m_eKarmaType;
	Engine::CTransform* m_pTargetTransformCom;
	_double				m_dTime;
	_float				m_fAlpha;
	_vec3 m_vScale;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static COverHeadEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, KARMATYPE eType, Engine::CTransform* pTargetTransform);
	virtual void				Free();
};

#endif