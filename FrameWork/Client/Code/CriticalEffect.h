#pragma once
#ifndef __CCriticalEffect_H__
#define __CCriticalEffect_H__

#include "BaseEffect.h"
#include "RcTex.h"
#include "Texture.h"

class CCriticalEffect : public CBaseEffect
{
private:
	explicit CCriticalEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCriticalEffect();

public:
	virtual HRESULT		Ready_GameObject(_vec3* pPos, _vec3* pAngle, BASE_INFO* pBaseInfo);
	virtual _int		Update_GameObject(const _double& dTimeDelta) ;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) ;
	virtual void		Render_Geometry(const _double& dTimeDelta) ;

public:
	void		Set_Info(_vec3* pPos, _vec3* pAngle, BASE_INFO* pBaseInfo);
	void		Set_ParentTransformCom(Engine::CTransform* pParentTransformCom) { m_pParentTransformCom = pParentTransformCom; }
private:
	HRESULT		Clone_Component();
	//Shader
	HRESULT		Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	//상태(크기 등) 변하는것들은 여기에
	void		Transform(const _double& dTimeDelta);
private:
	void		ResetTextureInfo();

private:
	Engine::CRcTex*				m_pBufferCom;
	_vec2						m_vUVAnimation_UV_Sum;
	TEXTUREEFFECT_INFO			m_tTextureInfo;

	Engine::CTransform*			m_pParentTransformCom;
	//디졸브
	_double								m_dDissolveCountDown;
	_uint								m_iPass;
	_bool								m_bDissolveStart;
public:
	static	CCriticalEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle = &_vec3(0.f, 0.f, 0.f), BASE_INFO* pBaseInfo = nullptr);
	virtual void	Free();

};

#endif // !__CPARTICLE_H__
