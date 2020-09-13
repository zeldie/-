#pragma once

#ifndef __CNumber_h__
#define __CNumber_h__

#include "NormalObject.h"

class CNumber : public CNormalObject
{
public:
	enum NUMBERTYPE { BASIC, CRITICAL, NUMBERTYPE_END };

public:
	explicit CNumber(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNumber();

public:
	HRESULT				Ready_GameObject(CDamageBox* pTarget, Engine::CTransform* pParentTransform, NUMBERTYPE eType, _vec3 vPos, _vec3 vSize, _uint iNum);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	NUMBERTYPE			m_eNumberType;
	_uint				m_iNumber;
	CDamageBox*			m_pTarget;
	Engine::CTransform* m_pParentTransformCom;

	_double				m_dTime;
	_float				m_fAlpha;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CNumber*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CDamageBox* pTarget, Engine::CTransform* pParentTransform, NUMBERTYPE eType, _vec3 vPos, _vec3 vSize, _uint iNum);
	virtual void		Free();
};

#endif