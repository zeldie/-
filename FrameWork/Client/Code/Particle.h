#pragma once
#ifndef __CPARTICLE_H__
#define __CPARTICLE_H__

#include "BaseEffect.h"
#include "ParticleTex.h"
#include "Texture.h"

#define	RADICAL_CNT 100
#define	LOBBY_LEAF_CNT 50

class CParticle : public CBaseEffect
{
private:
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CParticle();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) ;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) ;
	virtual void		Render_Geometry(const _double& dTimeDelta) ;

public:
	void		Set_ParticleInfo(PARTICLETYPE eType, _vec3* pPos, _vec3* pBoundingMin = 0, _vec3* pBoundingMax = 0);

private:
	HRESULT		Clone_Component();
	_vec3		Create_Angle();
	_bool		BoundingBox(_vec3* pPos);
	void		Reset_InBoundingBox(_vec3* pPos);

	//Shader
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	Engine::CParticleTex*			m_pParticleTex;
	Engine::CTexture*				m_pTextureCom;
	list<Engine::PARTICLE_INFO>		m_listParticleInfo;

	PARTICLETYPE					m_eType;		//파티클 타입
	_double							m_dAge;			//지속된 시간
	_double							m_dLifeTime;	//최대 지속시간
	
	_uint							m_iTextureNum;
	_bool							m_bAlpha;

	_vec3							m_vBoundingBoxMin;
	_vec3							m_vBoundingBoxMax;

public:
	static	CParticle*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free();

};

#endif // !__CPARTICLE_H__
