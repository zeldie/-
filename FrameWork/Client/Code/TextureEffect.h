#pragma once
#ifndef __TEXTUREEFFECT_H__
#define __TEXTUREEFFECT_H__

#include "BaseEffect.h"
#include "RcTex.h"
#include "Texture.h"

class CTextureEffect : public CBaseEffect
{
private:
	explicit CTextureEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTextureEffect();

public:
	virtual HRESULT		Ready_GameObject(TEXTUREEFFECT eEffectID, _vec3* pPos, _vec3* pAngle, BASE_INFO* tBaseInfo, CONTROLTYPE eControlType);
	virtual _int		Update_GameObject(const _double& dTimeDelta) ;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) ;
	virtual void		Render_Geometry(const _double& dTimeDelta);

public:
	Engine::CTransform*	Get_TransformCom() { return m_pTransformCom; }

public:
	void		Set_Info(TEXTUREEFFECT eEffectID, _vec3* pPos, _vec3* pAngle, BASE_INFO* tBaseInfo, CONTROLTYPE eControlType);
	HRESULT		Clone_TextureCom(TEXTUREEFFECT eEffectID);
	void		Set_ParentTransformCom(Engine::CTransform* pParentTransformCom) { m_pParentTransformCom = pParentTransformCom; }
private:
	HRESULT		Clone_Component(TEXTUREEFFECT eEffectID);
	_vec3		Create_Angle();
	//Shader
	HRESULT		Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	wstring		Find_Texture(TEXTUREEFFECT eEffectID);
	void		DeadEvent();
	//상태(크기 등) 변하는것들은 여기에
	void		Transform(const _double& dTimeDelta);
private:
	void		ResetTextureInfo();

private:
	Engine::CRcTex*				m_pBufferCom;
	TEXTUREEFFECT				m_eTextureEffect;
	_vec2						m_vUVAnimation_UV_Sum;
	TEXTUREEFFECT_INFO			m_tTextureInfo;
	
	_float						m_fTS_Alpha; //희정
	_float						m_fTS_Scale; //희정
	_uint						m_iTextureNum; //희정

	//인균
	Engine::CTransform*			m_pParentTransformCom; //텍스쳐가 부모로 받고있는 트랜스폼
	_float						m_fRotation; // 텍스쳐 회전에 씀
	_double						m_dCountdown; //추가 이펙트 만듬
	_bool						m_bCreateEffect;
	CONTROLTYPE					m_eControlType;
public:
	static	CTextureEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									TEXTUREEFFECT eEffectID, 
									_vec3* pPos, 
									_vec3* pAngle,
									BASE_INFO* tBaseInfo = nullptr,
									CONTROLTYPE eControlType = CTRL_END);
	virtual void	Free();

};

#endif // !__CPARTICLE_H__
