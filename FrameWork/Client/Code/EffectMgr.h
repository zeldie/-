#pragma once
#ifndef __CEFFECTMGR_H__
#define __CEFFECTMGR_H__

#define	STATICEFFECT_POOL_SIZE 500
#define	DYNAMICEFFECT_POOL_SIZE 100
#define	TEXTUREEFFECT_POOL_SIZE 100
#define	PARTICLE_POOL_SIZE 100

class CStaticEffect;
class CDynamicEffect;
class CTextureEffect;
class CParticle;

class CEffectMgr
{
	DECLARE_SINGLETON(CEffectMgr)

private:
	explicit CEffectMgr();
	virtual ~CEffectMgr();

public:
	void	Create_Particle(PARTICLETYPE eType, _vec3* pPos);
	// eType == 이펙트 생성분기 / pPos == 생성위치 / pAngle == 방향 / tInfo == 공격력,ID
	void	Create_Effect(Engine::EFFECTTYPE eType, const _matrix* matWorld, _vec3* pAngle, BASE_INFO tInfo);
	HRESULT	Create_TextureEffect(TEXTUREEFFECT eEffectID, _vec3* pPos, _vec3* pAngle = &_vec3(0.f, 0.f, 0.f), BASE_INFO* pBaseInfo = nullptr, CONTROLTYPE eControlType = CTRL_END);
	void	Create_FanEffect(TEXTUREEFFECT eType, _vec3* pPos, _vec3 * pAngle, _vec3* pScale, BASE_INFO* pBaseInfo);
	void	Create_ArrowAura(Engine::EFFECTTYPE eType, const _matrix* matWorld, _vec3* pAngle, BASE_INFO tInfo, Engine::CTransform* pParentTransformCom);
	void	Create_BelatosArrowAura(Engine::EFFECTTYPE eType, const _matrix* matWorld, _vec3* pAngle, BASE_INFO tInfo, Engine::CTransform* pParentTransformCom);
	HRESULT	Create_Orb(TEXTUREEFFECT eEffectID, _vec3* pPos, Engine::CTransform* pParentTransformCom, _vec3* pAngle = &_vec3(0.f, 0.f, 0.f), BASE_INFO* pBaseInfo = nullptr);
	void	Create_Effect_Critical(_vec3* pPos, _vec3* pAngle = &_vec3(0.f, 0.f, 0.f), BASE_INFO* pBaseInfo = nullptr);
	void	Create_Effect_Break(_vec3* pPos, _vec3* pAngle = &_vec3(0.f, 0.f, 0.f), BASE_INFO* pBaseInfo = nullptr);

public:
	//오브젝트풀
	HRESULT		Ready_ObjPool();
	void		Return_StaticPool(CStaticEffect* pInstance);
	void		Return_DynamicPool(CDynamicEffect* pInstance);
	void		Return_TexturePool(CTextureEffect* pInstance);
	void		Return_ParticlePool(CParticle* pInstance);

private:
	CStaticEffect*		Get_StaticPool();
	CDynamicEffect*		Get_DynamicPool();
	CTextureEffect*		Get_TexturePool(TEXTUREEFFECT eEffectID, _vec3* pPos, _vec3* pAngle, BASE_INFO* tBaseInfo, CONTROLTYPE eControlType);
	CParticle*			Get_ParticlePool();

private:
	HRESULT		Load_EffectInfo();
	HRESULT		Load_Data(wstring wstrPath);
	void		Find_Effect(EFFECT_INFO tEffectInfo, const _matrix* matWorld, _vec3 * pAngle, BASE_INFO tBaseInfo);
	void		Find_Effect(EFFECT_INFO tEffectInfo, const _matrix* matWorld, _vec3 * pAngle, BASE_INFO tBaseInfo, Engine::CTransform* pParentTransformCom);

public:
	HRESULT		Set_Device(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	Engine::CManagement*	m_pManagement;

	//오브젝트풀
	list<CStaticEffect*>	m_listStaticPool;
	list<CDynamicEffect*>	m_listDynamicPool;
	list<CTextureEffect*>	m_listTexturePool;
	list<CParticle*>		m_listParticlePool;

	map<Engine::EFFECTTYPE, vector<EFFECT_INFO>>	m_mapEffectInfo;
};

#endif // !__CEFFECTMGR_H__
