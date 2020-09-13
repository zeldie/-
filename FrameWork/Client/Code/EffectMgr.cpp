#include "stdafx.h"
#include "EffectMgr.h"

#include "StaticEffect.h"
#include "DynamicEffect.h"
#include "TextureEffect.h"
#include "Particle.h"
#include "FanShapeEffect.h"

#include "CriticalEffect.h"
#include "BreakEffect.h"

IMPLEMENT_SINGLETON(CEffectMgr)

CEffectMgr::CEffectMgr()
	:m_pGraphicDev(nullptr),
	m_pManagement(Engine::CManagement::GetInstance())
{
}

CEffectMgr::~CEffectMgr()
{
	for (auto iter = m_listStaticPool.begin(); iter != m_listStaticPool.end(); ++iter)
	{
		Engine::Safe_Release(*iter);
	}

	for (auto iter = m_listDynamicPool.begin(); iter != m_listDynamicPool.end(); ++iter)
	{
		Engine::Safe_Release(*iter);
	}

	for (auto iter = m_listTexturePool.begin(); iter != m_listTexturePool.end(); ++iter)
	{
		Engine::Safe_Release(*iter);
	}

	for (auto iter = m_listParticlePool.begin(); iter != m_listParticlePool.end(); ++iter)
	{
		Engine::Safe_Release(*iter);
	}

	Engine::Safe_Release(m_pGraphicDev);
}

void CEffectMgr::Create_Particle(PARTICLETYPE eType, _vec3* pPos)
{
	if (eType == PARTICLETYPE::PARTICLE_RADICAL)
	{
		CParticle*	pInstance = Get_ParticlePool();
		pInstance->Set_ParticleInfo(eType, pPos);

		m_pManagement->Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"Particle", pInstance);
	}

	else if (eType == PARTICLETYPE::PARTICLE_LEAF)
	{
		CParticle*	pInstance = Get_ParticlePool();

		//=======πŸøÓµ˘π⁄Ω∫ ≈©±‚, ∏∂¿ª¡¬«• ¡§«ÿ¡ˆ∏È ºˆ¡§«ÿæﬂ«‘=========
		_vec3 vMin = { -2500.f, 0.f ,-2500.f };
		_vec3 vMax = { 2500.f, 1500.f ,2500.f };
		//=============================================================
		pInstance->Set_ParticleInfo(eType, pPos, &vMin, &vMax);

		m_pManagement->Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"Particle", pInstance);
	}
}

void CEffectMgr::Create_Effect(Engine::EFFECTTYPE eType, const _matrix* matWorld, _vec3 * pAngle, BASE_INFO tInfo)
{
	BASE_INFO	tTempInfo = tInfo;
	//EFFECTID	eID;
	//¿Ã∆Â∆Æ æ∆¿Ãµ º≥¡§ , √Êµπø°º≠ æµ enum∞™, ¿Ã∆Â∆Æ ≈¯ø°º≠ ª˝º∫«“∂ß∏∂¥Ÿ case √ﬂ∞°«ÿæﬂ«—¥Ÿ.
	switch (eType)
	{
	case Engine::EFFECT_BELATOS_THRUST2:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_THRUST2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_THRUST:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_THRUST;		
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_SHOCKWAVE:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_SHOCKWAVE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 2000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_SHOCKWAVE2:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_SHOCKWAVE2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 2000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_SHOCKWAVE_LAY:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_SHOCKWAVE_LAY;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_AIR_UP;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 2000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_SHOCKWAVE_LAY2:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_SHOCKWAVE_LAY2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 2000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_SHOCKWAVE_LAY3:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_SHOCKWAVE_LAY3;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 2000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_ARROW:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_ARROW;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.35f;
		tTempInfo.iAtt = 1500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_MISSILE:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_MISSILE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.35f;
		tTempInfo.iAtt = 1500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_CIRCLE:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_CIRCLE;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_SHIELDFRONT:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_SHIELDFRONT;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_CIRCLE2:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_CIRCLE2;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 2000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_CIRCLE3:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_CIRCLE3;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 7000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_HEMISPHERE:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_HEMISPHERE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_HEMISPHERE2:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_HEMISPHERE2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_HEMISPHERE3:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_HEMISPHERE3;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_ATTACK:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_ATTACK;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_SWEEP:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_SWEEP;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BELATOS_JUMPLIGHT:
	{
		tTempInfo.eEffectID = EFFECT_BELATOS_JUMPLIGHT;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_SPEAR:
	{
		tTempInfo.eEffectID = EFFECT_SPEAR;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 850;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_SPEAR2:
	{
		tTempInfo.eEffectID = EFFECT_SPEAR2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 750;
		tTempInfo.iCritical = 3;
		break; 
	}

	case Engine::EFFECT_AIRUP:
	{
		tTempInfo.eEffectID = EFFECT_AIRUP;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_AIR_UP;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_AIRUP2:
	{
		tTempInfo.eEffectID = EFFECT_AIRUP2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_FATETRAIL1:
	{
		tTempInfo.eEffectID = EFFECT_FATETRAIL1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_FATETRAIL2:
	{
		tTempInfo.eEffectID = EFFECT_FATETRAIL2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_FATETRAIL3:
	{
		tTempInfo.eEffectID = EFFECT_FATETRAIL3;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_FATETRAIL4:
	{
		tTempInfo.eEffectID = EFFECT_FATETRAIL4;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_FATETRAIL5:
	{
		tTempInfo.eEffectID = EFFECT_FATETRAIL5;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_FATETRAIL6:
	{
		tTempInfo.eEffectID = EFFECT_FATETRAIL6;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_FATETRAIL7:
	{
		tTempInfo.eEffectID = EFFECT_FATETRAIL7;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_FATETRAIL8:
	{
		tTempInfo.eEffectID = EFFECT_FATETRAIL8;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_FATETRAIL9:
	{
		tTempInfo.eEffectID = EFFECT_FATETRAIL9;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_FATETRAIL10:
	{
		tTempInfo.eEffectID = EFFECT_FATETRAIL10;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_FATE1:
	{
		tTempInfo.eEffectID = EFFECT_FATE1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}

	case Engine::EFFECT_BASICTRAIL1:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.7f;
		tTempInfo.iAtt = 600;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BASICTRAIL2:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 600;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BASICTRAIL3:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL3;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.6f;
		tTempInfo.iAtt = 700;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BASICTRAIL4:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL4;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.6f;
		tTempInfo.iAtt = 700;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BASICTRAIL5:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL5;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.6f;
		tTempInfo.iAtt = 800;
		tTempInfo.iCritical = 3;
		break;
	}

	case Engine::EFFECT_BASICTRAIL6:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL6;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.6f;
		tTempInfo.iAtt = 800;
		tTempInfo.iCritical = 3;
		break;
	}

	case Engine::EFFECT_BASICTRAIL7:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL7;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.6f;
		tTempInfo.iAtt = 900;
		tTempInfo.iCritical = 3;
		break;
	}

	case Engine::EFFECT_BASICTRAIL8:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL8;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.6f;
		tTempInfo.iAtt = 900;
		tTempInfo.iCritical = 3;
		break;
	}

	case Engine::EFFECT_AIRTRAIL1:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.7f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_AIRTRAIL2:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_AIRTRAIL3:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL3;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.6f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_AIRTRAIL4:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL4;		
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.6f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_AIRTRAIL5:
	{
		tTempInfo.eEffectID = EFFECT_BASICTRAIL5;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.6f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BLADE1:
	{
		tTempInfo.eEffectID = EFFECT_BLADE1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 1.6f;
		tTempInfo.iAtt = 2500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_CROSS:
	{
		tTempInfo.eEffectID = EFFECT_CROSS;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.6f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BASICSP1:
	{
		tTempInfo.eEffectID = EFFECT_BASICSP1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 0.6f;
		tTempInfo.iAtt = 650;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BASICSP2:
	{
		tTempInfo.eEffectID = EFFECT_BASICSP2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 0.6f;
		tTempInfo.iAtt = 650;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BASICSP3:
	{
		tTempInfo.eEffectID = EFFECT_BASICSP3;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 0.6f;
		tTempInfo.iAtt = 650;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BASICSP4:
	{
		tTempInfo.eEffectID = EFFECT_BASICSP4;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 0.6f;
		tTempInfo.iAtt = 650;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_BASICSP5:
	{
		tTempInfo.eEffectID = EFFECT_BASICSP5;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.6f;
		tTempInfo.iAtt = 650;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TEST:
	{
		tTempInfo.eEffectID = EFFECT_TEST;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1650;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_GENICIDE:
	{
		tTempInfo.eEffectID = EFFECT_GENICIDE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 3000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_AIRBULLET1:
	{
		tTempInfo.eEffectID = EFFECT_AIRBULLET1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;

		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 850;
		tTempInfo.iCritical = 3;
		break;
	}

	case Engine::EFFECT_AIRBULLET2:
	{
		tTempInfo.eEffectID = EFFECT_AIRBULLET2;
		break;
	}

	case Engine::EFFECT_APOSTLE_SPHERE_1:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_SPHERE_1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_APOSTLE_SPHERE_2:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_SPHERE_2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_APOSTLE_SPHERE_3:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_SPHERE_3;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_APOSTLE_SPHERE_4:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_SPHERE_4;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.2f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_APOSTLE_LASER1:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_LASER1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.2f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_APOSTLE_SPHERE_5:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_SPHERE_5;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 0.2f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_200:
	{
		tTempInfo.eEffectID = EFFECT_200;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 0.2f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_SECOND: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_SECOND;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1800;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_AIRLB_SECOND: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_AIRLB_SECOND;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_THIRD1: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_THIRD1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1800;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_THIRD2: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_THIRD2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1800;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_AIRLB_THIRD: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_AIRLB_THIRD;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_AIRLB_FOURTH: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_AIRLB_FOURTH;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 2000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_RIGHT_MIN: //»Ò¡§ - dynamic
	{
		tTempInfo.eEffectID = EFFECT_TS_RIGHT_MIN;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 5.f;
		tTempInfo.iAtt = 2300;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_RIGHT_MID: //»Ò¡§ - dynamic
	{
		tTempInfo.eEffectID = EFFECT_TS_RIGHT_MID;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 5.f;
		tTempInfo.iAtt = 0;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_PRESSING: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_PRESSING;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1200;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_PRESSING2: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_PRESSING2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_AIRUP: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_AIRUP;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_AIR_UP;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1300;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_E_FIRST: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_E_FIRST;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 2.f;
		tTempInfo.iAtt = 1300;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_E_SECOND: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_E_SECOND;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 2.f;
		tTempInfo.iAtt = 1300;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_E_THIRD: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_E_THIRD;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.75f;
		tTempInfo.iAtt = 1300;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_E_FOURTH: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_E_FOURTH;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.75f;
		tTempInfo.iAtt = 1300;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_R1: //»Ò¡§ - dynamic
	{
		tTempInfo.eEffectID = EFFECT_TS_R1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_AIR_UP;
		tTempInfo.fEffectHitTime = 3.f;
		tTempInfo.iAtt = 0;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_F: //»Ò¡§ - dynamic
	{
		tTempInfo.eEffectID = EFFECT_TS_F;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 5.f;
		tTempInfo.iAtt = 1100;
		break;
	}
	case Engine::EFFECT_TS_F_ROCK: //»Ò¡§ 
	{
		tTempInfo.eEffectID = EFFECT_TS_F_ROCK;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 0;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_AIRROCK_MIN: //»Ò¡§ - dynamic
	{
		tTempInfo.eEffectID = EFFECT_TS_AIRROCK_MIN;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 5.f;
		tTempInfo.iAtt = 2500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_AIRROCK_MID: //»Ò¡§ - dynamic
	{
		tTempInfo.eEffectID = EFFECT_TS_AIRROCK_MID;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 5.f;
		tTempInfo.iAtt = 0;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_R2: //»Ò¡§ - dynamic
	{
		tTempInfo.eEffectID = EFFECT_TS_R2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_AIR_UP;
		tTempInfo.fEffectHitTime = 3.f;
		tTempInfo.iAtt = 0;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_R3: //»Ò¡§ - dynamic
	{
		tTempInfo.eEffectID = EFFECT_TS_R3;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_AIR_UP;
		tTempInfo.fEffectHitTime = 3.f;
		tTempInfo.iAtt = 0;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_R4: //»Ò¡§ - dynamic
	{
		tTempInfo.eEffectID = EFFECT_TS_R4;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_AIR_UP;
		tTempInfo.fEffectHitTime = 3.f;
		tTempInfo.iAtt = 1800;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_SECOND_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_SECOND_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_AIRLB_SECOND_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_AIRLB_SECOND_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1800;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_THIRD1_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_THIRD1_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1800;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_THIRD2_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_THIRD2_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1800;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_AIRLB_THIRD_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_AIRLB_THIRD_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1700;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_AIRLB_FOURTH_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_AIRLB_FOURTH_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 2000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_PRESSING_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_PRESSING_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1400;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_PRESSING2_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_PRESSING2_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1300;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_AIRUP_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_AIRUP_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_AIR_UP;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_E_FIRST_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_E_FIRST_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 2.f;
		tTempInfo.iAtt = 1500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_E_SECOND_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_E_SECOND_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 2.f;
		tTempInfo.iAtt = 1500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_E_THIRD_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_E_THIRD_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.75f;
		tTempInfo.iAtt = 1500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_E_FOURTH_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_E_FOURTH_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 1.75f;
		tTempInfo.iAtt = 1500;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LB_ULTIMATE: //»Ò¡§
	{
		tTempInfo.eEffectID = EFFECT_TS_LB_ULTIMATE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.5f;
		tTempInfo.iAtt = 1200;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_TS_LAY:
	{
		tTempInfo.eEffectID = EFFECT_TS_LAY;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_AIR_UP;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 2000;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_APOSTLE_LIGHT:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_LIGHT;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.2f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_APOSTLE_LIGHT_DOT:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_LIGHT_DOT;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.2f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_APOSTLE_LIGHT2:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_LIGHT2;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.2f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_APOSTLE_LIGHT2_DOT:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_LIGHT2_DOT;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.2f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_APOSTLE_TRAIL1:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_TRAIL1;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.2f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_APOSTLE_DOME_HALF:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_DOME_HALF;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
		tTempInfo.fEffectHitTime = 0.2f;
		tTempInfo.iAtt = 1100;
		tTempInfo.iCritical = 3;
		break;
	}
	case Engine::EFFECT_WIND_DUST: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_WIND_DUST;
		break;
	}
	case Engine::EFFECT_DUST: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_DUST;
		break;
	}
	case Engine::EFFECT_BOW_RING: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_DUST;
		break;
	}
	case Engine::EFFECT_ARROW_AURA: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ARROW_AURA;
		break;
	}
	case Engine::EFFECT_PHOENIX_WIND1: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_PHOENIX_WIND1;
		break;
	}
	case Engine::EFFECT_PHOENIX_WIND2: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_PHOENIX_WIND2;
		break;
	}
	case Engine::EFFECT_PHOENIX_WIND3: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_PHOENIX_WIND3;
		break;
	}
	case Engine::EFFECT_ORB_TRAIL: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ORB_TRAIL;
		break;
	}
	case Engine::EFFECT_ORB_TRAIL2: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ORB_TRAIL2;
		break;
	}
	case Engine::EFFECT_ORB_WIND: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ORB_WIND;
		break;
	}
	case Engine::EFFECT_FIREBALL_WIND: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_FIREBALL_WIND;
		break;
	}
	case Engine::EFFECT_ORB_WIND2: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ORB_WIND2;
		break;
	}
	case Engine::EFFECT_ORB_BEAM: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ORB_BEAM;
		break;
	}
	case Engine::EFFECT_ORB_R_PRE: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ORB_R_PRE;
		break;
	}
	case Engine::EFFECT_ORB_WIND3: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ORB_WIND3;
		break;
	}
	case Engine::EFFECT_ORB_WIND4: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ORB_WIND4;
		break;
	}
	case Engine::EFFECT_ORB_ULT_WIND: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ORB_ULT_WIND;
		break;
	}
	case Engine::EFFECT_ARROW: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ARROW;
		break;
	}
	case Engine::EFFECT_ORB: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_ORB;
		break;
	}
	case Engine::EFFECT_PHOENIX: //¿Œ±’
	{
		tTempInfo.eEffectID = EFFECT_PHOENIX;
		break;
	}
	case Engine::EFFECT_END:
		break;
	default:
	{
		break;
	}
	}
	for (auto iter : m_mapEffectInfo.find(eType)->second)
	{
		Find_Effect(iter, matWorld, pAngle, tTempInfo);
	}
}

void CEffectMgr::Create_FanEffect(TEXTUREEFFECT eType, _vec3* pPos, _vec3 * pAngle, _vec3* pScale, BASE_INFO* tBaseInfo)
{
	BASE_INFO	tTempInfo = *tBaseInfo;

	switch (eType)
	{
	case TEXTURE_FAN_TRIANGLE:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_FAN_TRIANGIE;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		break;
	}
	case TEXTURE_FAN_HALF:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_FAN_HALF;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		break;
	}
	case TEXTURE_FAN_BIG:
	{
		tTempInfo.eEffectID = EFFECT_APOSTLE_FAN_BIG;
		tTempInfo.eCollisionID = COLLISIONID::COLLISION_KNOCKDOWN;
		tTempInfo.fEffectHitTime = 1.f;
		tTempInfo.iAtt = 1100;
		break;
	}
	case TEXTURE_END:
		break;
	default:
		break;
	}

	CFanShapeEffect* pInstance = CFanShapeEffect::Create(m_pGraphicDev);
	pInstance->Set_Info(eType, pPos, pAngle, pScale, tTempInfo);

	m_pManagement->Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"FanEffect", pInstance);
}

void CEffectMgr::Create_ArrowAura(Engine::EFFECTTYPE eType, const _matrix * matWorld, _vec3 * pAngle, BASE_INFO tInfo, Engine::CTransform * pParentTransformCom)
{
	BASE_INFO	tTempInfo = tInfo;
	tTempInfo.eEffectID = EFFECT_ARROW_AURA;
	for (auto iter : m_mapEffectInfo.find(eType)->second)
	{
		Find_Effect(iter, matWorld, pAngle, tTempInfo, pParentTransformCom);
	}
}

void CEffectMgr::Create_BelatosArrowAura(Engine::EFFECTTYPE eType, const _matrix * matWorld, _vec3 * pAngle, BASE_INFO tInfo, Engine::CTransform * pParentTransformCom)
{
	BASE_INFO	tTempInfo = tInfo;
	tTempInfo.eEffectID = EFFECT_BELATOS_ARROW;
	for (auto iter : m_mapEffectInfo.find(eType)->second)
	{
		Find_Effect(iter, matWorld, pAngle, tTempInfo, pParentTransformCom);
	}
}

HRESULT CEffectMgr::Create_Orb(TEXTUREEFFECT eEffectID, _vec3 * pPos, Engine::CTransform * pParentTransformCom, _vec3 * pAngle, BASE_INFO * pBaseInfo)
{
	Engine::CGameObject* pTextureEffect = nullptr;
	pTextureEffect = Get_TexturePool(eEffectID, pPos, pAngle, pBaseInfo, CTRL_END);
	static_cast<CTextureEffect*>(pTextureEffect)->Set_ParentTransformCom(pParentTransformCom);
	if (FAILED(Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"Effect", pTextureEffect)))
		return E_FAIL;
	return S_OK;
}

void CEffectMgr::Create_Effect_Critical(_vec3 * pPos, _vec3 * pAngle, BASE_INFO * pBaseInfo)
{
	CCriticalEffect* pInstance = nullptr;
	pInstance = CCriticalEffect::Create(m_pGraphicDev, pPos, pAngle, pBaseInfo);
	if (FAILED(Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"Effect", pInstance)))
		return;
	return;
}

void CEffectMgr::Create_Effect_Break(_vec3 * pPos, _vec3 * pAngle, BASE_INFO * pBaseInfo)
{
	CBreakEffect* pInstance = nullptr;
	pInstance = CBreakEffect::Create(m_pGraphicDev, pPos, pAngle, pBaseInfo);
	if (FAILED(Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"Effect", pInstance)))
		return;
	return;
}

HRESULT CEffectMgr::Create_TextureEffect(TEXTUREEFFECT eEffectID, _vec3 * pPos, _vec3 * pAngle, BASE_INFO* pBaseInfo, CONTROLTYPE eControlType)
{
	Engine::CGameObject* pTextureEffect = nullptr;
	pTextureEffect = Get_TexturePool(eEffectID, pPos, pAngle, pBaseInfo, eControlType);
	if (FAILED(Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"Effect", pTextureEffect)))
		return E_FAIL;
	return S_OK;
}

HRESULT CEffectMgr::Ready_ObjPool()
{
	//Ω∫≈◊∆Ω¿Ã∆Â∆Æ ø¿∫Í¡ß∆Æ«Æ
	for (int i = 0; i < STATICEFFECT_POOL_SIZE; ++i)
	{
		CStaticEffect*	pInstance = CStaticEffect::Create(m_pGraphicDev);
		m_listStaticPool.push_back(pInstance);
	}
	//¥Ÿ¿Ã≥™πÕ¿Ã∆Â∆Æ ø¿∫Í¡ß∆Æ«Æ
	for (int i = 0; i < DYNAMICEFFECT_POOL_SIZE; ++i)
	{
		CDynamicEffect*	pInstance = CDynamicEffect::Create(m_pGraphicDev);
		m_listDynamicPool.push_back(pInstance);
	}
	//≈ÿΩ∫√ƒ¿Ã∆Â∆Æ ø¿∫Í¡ß∆Æ«Æ
	for (int i = 0; i < TEXTUREEFFECT_POOL_SIZE; ++i)
	{
		CTextureEffect*	pInstance = CTextureEffect::Create(m_pGraphicDev, TEXTURE_DUSTRUNSTOP, &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f));
		m_listTexturePool.push_back(pInstance);
	}
	//∆ƒ∆º≈¨ ø¿∫Í¡ß∆Æ«Æ
	for (int i = 0; i < PARTICLE_POOL_SIZE; ++i)
	{
		CParticle*	pInstance = CParticle::Create(m_pGraphicDev);
		m_listParticlePool.push_back(pInstance);
	}
	Load_EffectInfo();
	return S_OK;
}

void CEffectMgr::Return_StaticPool(CStaticEffect * pInstance)
{
	if (pInstance == nullptr)
		return;

	m_listStaticPool.push_back(pInstance);
}

void CEffectMgr::Return_DynamicPool(CDynamicEffect * pInstance)
{
	if (pInstance == nullptr)
		return;

	m_listDynamicPool.push_back(pInstance);
}

void CEffectMgr::Return_TexturePool(CTextureEffect * pInstance)
{
	if (pInstance == nullptr)
		return;

	m_listTexturePool.push_back(pInstance);
}

void CEffectMgr::Return_ParticlePool(CParticle * pInstance)
{
	if (pInstance == nullptr)
		return;

	m_listParticlePool.push_back(pInstance);
}

CStaticEffect * CEffectMgr::Get_StaticPool()
{
	CStaticEffect* pInstance = nullptr;

	if (m_listStaticPool.empty())
	{
		pInstance = CStaticEffect::Create(m_pGraphicDev);
	}
	else
	{
		pInstance = m_listStaticPool.front();
		m_listStaticPool.pop_front();
	}

	return pInstance;
}

CDynamicEffect * CEffectMgr::Get_DynamicPool()
{
	CDynamicEffect* pInstance = nullptr;

	if (m_listDynamicPool.empty())
	{
		pInstance = CDynamicEffect::Create(m_pGraphicDev);
	}
	else
	{
		pInstance = m_listDynamicPool.front();
		m_listDynamicPool.pop_front();
	}

	return pInstance;
}

CTextureEffect * CEffectMgr::Get_TexturePool(TEXTUREEFFECT eEffectID, _vec3* pPos, _vec3* pAngle, BASE_INFO* tBaseInfo, CONTROLTYPE eControlType)
{
	CTextureEffect* pInstance = nullptr;

	if (m_listTexturePool.empty())
	{
		pInstance = CTextureEffect::Create(m_pGraphicDev, eEffectID, pPos, pAngle, tBaseInfo, eControlType);
	}
	else
	{
		pInstance = m_listTexturePool.front();
		pInstance->Set_Info(eEffectID, pPos, pAngle,tBaseInfo, eControlType);
		m_listTexturePool.pop_front();
	}

 	return pInstance;
}

CParticle * CEffectMgr::Get_ParticlePool()
{
	CParticle* pInstance = nullptr;

	if (m_listParticlePool.empty())
	{
		pInstance = CParticle::Create(m_pGraphicDev);
	}
	else
	{
		pInstance = m_listParticlePool.front();
		m_listParticlePool.pop_front();
	}

	return pInstance;
}

HRESULT CEffectMgr::Load_EffectInfo()
{
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_Thrust2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_Thrust.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_ShockWave.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_ShockWave2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_ShockWave_Lay.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_ShockWave_Lay2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_ShockWave_Lay3.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_Arrow.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_Circle.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_ShieldFront.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_Circle2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_Circle3.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_Hemisphere.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_Hemisphere2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_Hemisphere3.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_Attack.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_Sweep.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Belatos_JumpLight.txt")))
		return E_FAIL;

	if (FAILED(Load_Data(L"../../Data/Effect/DsBasic1.txt")))			//yh ds atk1
		return E_FAIL;													
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasic2.txt")))			//yh ds atk1
		return E_FAIL;													
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasic3.txt")))			//yh ds atk2
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasic4.txt")))			//yh ds atk2
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasic5.txt")))			//yh ds atk4-1
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasic6.txt")))			//yh ds atk4-1
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasic7.txt")))			//yh ds atk4-2
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasic8.txt")))			//yh ds atk4-2
		return E_FAIL;


	if (FAILED(Load_Data(L"../../Data/Effect/DsAir1.txt")))			//yh ds airatk2
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsAir2.txt")))			//yh ds airatk2
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsAir3.txt")))			//yh ds airatk3
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsAir4.txt")))			//yh ds airatk3
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsAir5.txt")))			//yh ds airatk4
		return E_FAIL;

	if (FAILED(Load_Data(L"../../Data/Effect/DsAirB1.txt")))		//yh ds air
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsSpear.txt")))		//yh ds atk spear
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsSpear2.txt")))		//yh ds atk spear
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsAirUp.txt")))		//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsAirUp2.txt")))		//yh ds air up
		return E_FAIL;

	if (FAILED(Load_Data(L"../../Data/Effect/DsFate1.txt")))		//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsBlade1.txt")))		//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsCross.txt")))		//yh ds air up
		return E_FAIL;

	if (FAILED(Load_Data(L"../../Data/Effect/DsFateTrail1.txt")))	//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsFateTrail2.txt")))	//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsFateTrail3.txt")))	//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsFateTrail4.txt")))	//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsFateTrail5.txt")))	//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsFateTrail6.txt")))	//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsFateTrail7.txt")))	//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsFateTrail8.txt")))	//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsFateTrail9.txt")))	//yh ds air up
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsFateTrail10.txt")))	//yh ds air up
		return E_FAIL;

	if (FAILED(Load_Data(L"../../Data/Effect/DsSkillE4.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasicSp1.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasicSp2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasicSp3.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasicSp4.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsBasicSp5.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsTest.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/DsGenocide1.txt")))
		return E_FAIL;

	if (FAILED(Load_Data(L"../../Data/Effect/test2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/test3.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_ATTACK2_1.txt")))
		return E_FAIL;	

	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_Sphere1.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_Sphere2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_Sphere3.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_Sphere4.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_Laser.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_Sphere5.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_Trail1.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_Light.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_Light2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_DomeHalf.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_Light_Dot.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Apostle_Light2_Dot.txt")))
		return E_FAIL;

	// ≈◊Ω∫∆ÆøÎ
	if (FAILED(Load_Data(L"../../Data/Effect/TrailTest.txt")))
		return E_FAIL;
	
	//»Ò¡§ static Mesh
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_SECOND.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_AIRLB_SECOND.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_THIRD1.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_THIRD2.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_AIRLB_THIRD.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_AIRLB_FOURTH.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_PRESSING.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_PRESSING2.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_AIRUP.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_E_FIRST.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_E_SECOND.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_E_THIRD.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_E_FOURTH.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_F_ROCK.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LAY.txt"))) //»Ò¡§
		return E_FAIL;

	//-->±√±ÿ±‚
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_SECOND_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_AIRLB_SECOND_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_THIRD1_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_THIRD2_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_AIRLB_THIRD_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_AIRLB_FOURTH_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_PRESSING_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;	
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_PRESSING2_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_AIRUP_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_AIRUP_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_E_FIRST_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_E_SECOND_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_E_THIRD_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_E_FOURTH_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_LB_ULTIMATE.txt"))) //»Ò¡§
		return E_FAIL;


	//»Ò¡§ dynamic mesh
	if (FAILED(Load_Data(L"../../Data/Effect/TS_RIGHT_MIN.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_RIGHT_MID.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_AIRROCK_MIN.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_AIRROCK_MID.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_R1.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_R2.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_R3.txt"))) //»Ò¡§
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/TS_R4.txt"))) //»Ò¡§
		return E_FAIL;

	if (FAILED(Load_Data(L"../../Data/Effect/TS_F.txt"))) //»Ò¡§
		return E_FAIL;

	//¿Œ±’
	if (FAILED(Load_Data(L"../../Data/Effect/Wind_Dust.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Bow_Ring.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Dust.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Arrow_Aura.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Phoenix_Wind1.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Phoenix_Wind2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Phoenix_Wind3.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Orb_Trail.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Orb_Trail2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Orb_Wind.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Fireball_Wind.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Orb_Wind2.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Orb_Beam.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Orb_R_Pre.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Orb_Wind3.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Orb_Wind4.txt")))
		return E_FAIL;
	if (FAILED(Load_Data(L"../../Data/Effect/Orb_Ult_Wind.txt")))
		return E_FAIL;
	return S_OK;
}

HRESULT CEffectMgr::Load_Data(wstring wstrPath)
{
	wifstream fin;
	fin.open(wstrPath);

	if (fin.fail())
		return E_FAIL;

	//Engine::EFFECTTYPE	eType;
	int		iType = -1;
	int		iVectorSize = 0;
	EFFECT_INFO	tInfo;
	Engine::EFFECT_UV_INFO	tEffectInfo;

	vector<EFFECT_INFO> vec;
	
	fin >> iType;
	while (true)
	{
		fin >> tInfo.wstrType;
		fin >> tInfo.wstrName;
		fin >> tInfo.vPos.x;
		fin >> tInfo.vPos.y;
		fin >> tInfo.vPos.z;
		fin >> tInfo.vRot.x;
		fin >> tInfo.vRot.y;
		fin >> tInfo.vRot.z;
		fin >> tInfo.vScale.x;
		fin >> tInfo.vScale.y;
		fin >> tInfo.vScale.z;
		fin >> tInfo.vMovePos.x;
		fin >> tInfo.vMovePos.y;
		fin >> tInfo.vMovePos.z;
		fin >> tInfo.vMoveRot.x;
		fin >> tInfo.vMoveRot.y;
		fin >> tInfo.vMoveRot.z;
		fin >> tInfo.vMoveScale.x;
		fin >> tInfo.vMoveScale.y;
		fin >> tInfo.vMoveScale.z;

		fin >> tInfo.fSpeed;
		fin >> tInfo.fDeadTime;
		fin >> tInfo.iAlpha;
		fin >> iVectorSize;

		for (int i = 0; i < iVectorSize; i++)
		{
			fin >>tEffectInfo.iTextureName;
			fin >>tEffectInfo.vUVPos.x;
			fin >>tEffectInfo.vUVPos.y;
			fin >>tEffectInfo.vUVMovePos.x;
			fin >>tEffectInfo.vUVMovePos.y;
			fin >>tEffectInfo.vUVAtlas.x;
			fin >>tEffectInfo.vUVAtlas.y;
			fin >>tEffectInfo.iPass;
			tInfo.vectorUVInfo.push_back(tEffectInfo);
		}

		if (fin.eof())
			break;
		vec.push_back(tInfo);
	}
	fin.close();

	m_mapEffectInfo.emplace((Engine::EFFECTTYPE)iType, vec);


	return S_OK;
}

void CEffectMgr::Find_Effect(EFFECT_INFO tEffectInfo, const _matrix* matWorld, _vec3 * pAngle, BASE_INFO tBaseInfo)
{
	if (tEffectInfo.wstrType == L"DynamicMesh")
	{
		CDynamicEffect* pInstance = Get_DynamicPool();
		pInstance->Set_Info(tEffectInfo, matWorld, pAngle, tBaseInfo);
		m_pManagement->Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DynamicEffect", pInstance);
	}
	else if (tEffectInfo.wstrType == L"StaticMesh")
	{
		CStaticEffect* pInstance = Get_StaticPool();
		pInstance->Set_Info(tEffectInfo, matWorld, pAngle, tBaseInfo);
		m_pManagement->Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"StaticEffect", pInstance);
	}
	else if (tEffectInfo.wstrType == L"TextureOBJ")
	{
		/*CTextureEffect* pInstance = Get_TexturePool();
		pInstance->Set_Info(tEffectInfo, matWorld, pAngle, tBaseInfo);

		m_pManagement->Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"TextureEffect", pInstance);*/
	}
}

void CEffectMgr::Find_Effect(EFFECT_INFO tEffectInfo, const _matrix * matWorld, _vec3 * pAngle, BASE_INFO tBaseInfo, Engine::CTransform * pParentTransformCom)
{
	CStaticEffect* pInstance = Get_StaticPool();
	pInstance->Set_Info(tEffectInfo, matWorld, pAngle, tBaseInfo);
	pInstance->Set_ParentTransformCom(pParentTransformCom);
	m_pManagement->Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"StaticEffect", pInstance);
}


HRESULT CEffectMgr::Set_Device(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (pGraphicDev == nullptr)
		return E_FAIL;

	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	return S_OK;
}
