#pragma once
#ifndef __AIBackBar_h__
#define __AIBackBar_h__

#include "NormalObject.h"
class CAIBackBar : public CNormalObject
{
public:
	enum BACKBARTYPE { ROUNDTYPE, RECTTYPE, BACKBARTYPE_END };

public:
	explicit CAIBackBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAIBackBar();

public:
	HRESULT				Ready_GameObject(BACKBARTYPE eType, Engine::CTransform* pParentTransform, _vec3 vPos, _vec3 vSize);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;


private:
	BACKBARTYPE			m_eBackBarType;
	Engine::CTransform* m_pParentTransformCom;

	_vec3				m_vScale;

	////자리 이동 변수
	//_float fx;
	//_float fy;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CAIBackBar*	Create(LPDIRECT3DDEVICE9 pGraphicDev, BACKBARTYPE eType, Engine::CTransform* pParentTransform, _vec3 vPos, _vec3 vSize);
	virtual void		Free();
};

#endif