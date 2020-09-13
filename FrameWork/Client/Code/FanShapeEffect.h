#pragma once
#ifndef __CFanShapeEffect_H__
#define __CFanShapeEffect_H__

#include "BaseEffect.h"
#include "StaticMesh.h"
#include "RcTex.h"
#include "Texture.h"

class CFanShapeEffect : public CBaseEffect
{
private:
	explicit CFanShapeEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFanShapeEffect();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) ;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) ;
	virtual void		Render_Geometry(const _double& dTimeDelta) ;

public:
	void		Set_Info(TEXTUREEFFECT eType, _vec3* pPos, _vec3 * pAngle, _vec3* pScale, BASE_INFO tBaseInfo);

private:
	HRESULT		Clone_Component();
	//Shader
	HRESULT		Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	wstring		Find_Texture(TEXTUREEFFECT eEffectID);
	HRESULT		Clone_TextureCom();

private:
	Engine::CRcTex*				m_pBufferCom;
	_vec2						m_vUVAnimation_U;
	_vec2						m_vUVAnimation_V;
	_uint						m_iPass;
	TEXTUREEFFECT				m_eTextureEffect;
	_double						m_dbTime;
	_vec2						m_vUVAnimation_UV_Sum;
	_float						 m_fDiminishRate;

public:
	static	CFanShapeEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free();

};

#endif // !__CPARTICLE_H__
