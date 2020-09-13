#include "stdafx.h"
#include "Particle.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseEffect(pGraphicDev),
	m_pParticleTex(nullptr),
	m_pTextureCom(nullptr),
	m_eType(PARTICLETYPE::PARTICLE_END),
	m_iTextureNum(0),
	m_bAlpha(false),
	m_dAge(0.),
	m_dLifeTime(0.)
{
}


CParticle::~CParticle()
{
}

HRESULT CParticle::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	return S_OK;
}

_int CParticle::Update_GameObject(const _double & dTimeDelta)
{
	CBaseEffect::Update_GameObject(dTimeDelta);

	if ((m_dAge += dTimeDelta) >= m_dLifeTime)
	{
		m_pEffectMgr->Return_ParticlePool(this);
		return Engine::OBJ_POOL;
	}

	for (auto iter = m_listParticleInfo.begin(); iter != m_listParticleInfo.end(); ++iter)
	{
		iter->vPos += iter->vDir * iter->fSpeed * (_float)dTimeDelta;
		//iter->vPos += iter->vDir * iter->fSpeed * iter->fAcceleration;	//가속도 추가

		if (m_eType == PARTICLETYPE::PARTICLE_LEAF)
		{
			if (!(BoundingBox(&(iter->vPos))))
				Reset_InBoundingBox(&(iter->vPos));
		}
	}

	return Engine::NO_EVENT;
}

_int CParticle::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseEffect::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return Engine::NO_EVENT;
}

void CParticle::Render_Geometry(const _double & dTimeDelta)
{
	////m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

	//m_pTextureCom->Render_Texture();
	//m_pParticleTex->Render_InstanceBuffer(m_listParticleInfo, 20.5f);	//수정해야함
	//==============================================================================
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	if (m_bAlpha)
	{
		pEffect->BeginPass(4);
	}
	else
	{
		pEffect->BeginPass(0);
	}

	//m_pTextureCom->Render_Texture();
	m_pParticleTex->Render_InstanceBuffer(m_listParticleInfo, 20.5f);	//수정해야함

	pEffect->EndPass();
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

HRESULT CParticle::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pParticleTex = dynamic_cast<Engine::CParticleTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_ParticleTex"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	//수정해야함 텍스쳐 설정
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_EffectALL"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);
	
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

void CParticle::Set_ParticleInfo(PARTICLETYPE eType, _vec3* pPos, _vec3* pBoundingMin, _vec3* pBoundingMax)
{
	if (eType == PARTICLETYPE::PARTICLE_END)
		return;

	m_eType = eType;
	m_listParticleInfo.clear();

	//방사형 이펙트
	if (eType == PARTICLETYPE::PARTICLE_RADICAL)
	{
		m_dLifeTime = 10.;
		m_dAge = 0.;
		for (int i = 0; i < RADICAL_CNT; ++i)
		{
			Engine::PARTICLE_INFO	ParticleInfo;
			ParticleInfo.vPos = *pPos;
			ParticleInfo.fSpeed = ((rand() % 101) + 100) * 2.f;
			ParticleInfo.fAcceleration = ((rand() % 10) * 0.1f) + 1.f;
			ParticleInfo.Color = D3DXCOLOR(1.f, 0.f, 0.7f, 1.f);
			ParticleInfo.fSize = ((rand() % 11) * 0.1f) + 10.f;
			// 텍스쳐 번호
			m_iTextureNum = 8;
			// 알파설정
			m_bAlpha = false;
			_vec3 vDir = Create_Angle();
			ParticleInfo.vDir = *D3DXVec3Normalize(&vDir, &vDir);
			m_listParticleInfo.push_back(ParticleInfo);
		}
	}
	//로비에서 떨어지는 나뭇잎
	else if(eType == PARTICLETYPE::PARTICLE_LEAF)
	{
		m_dLifeTime = 99999.;
		m_dAge = 0.;
		m_vBoundingBoxMin = *pBoundingMin;
		m_vBoundingBoxMax = *pBoundingMax;
		for (int i = 0; i < LOBBY_LEAF_CNT; ++i)
		{
			Engine::PARTICLE_INFO	ParticleInfo;
			Reset_InBoundingBox(&(ParticleInfo.vPos));
			ParticleInfo.fSpeed = ((rand() % 201) + 100.f);
			ParticleInfo.Color = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
			ParticleInfo.fSize = (rand() % 11) + 10.f;
			_vec3 vDir = { 0.f,0.f,0.f };
			vDir.y = ((rand() % 20) + 10.f) *-1.f;
			vDir.x = _float((rand() % 20));
			// 텍스쳐 번호
			m_iTextureNum = 5;
			// 알파설정
			m_bAlpha = false;
			ParticleInfo.vDir = *D3DXVec3Normalize(&vDir, &vDir);
			m_listParticleInfo.push_back(ParticleInfo);
		}
	}
}

_vec3 CParticle::Create_Angle()
{
	_vec3	vAngle;
	vAngle.x = ((rand() % 13) * 30.f) - 180.f;
	vAngle.y = ((rand() % 13) * 30.f) - 180.f;
	vAngle.z = ((rand() % 13) * 30.f) - 180.f;

	return vAngle;
}

_bool CParticle::BoundingBox(_vec3 * pPos)
{
	if (pPos->x >= m_vBoundingBoxMin.x && pPos->y >= m_vBoundingBoxMin.y && pPos->z >= m_vBoundingBoxMin.z &&
		pPos->x <= m_vBoundingBoxMax.x && pPos->y <= m_vBoundingBoxMax.y && pPos->z <= m_vBoundingBoxMax.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CParticle::Reset_InBoundingBox(_vec3 * pPos)
{
	pPos->x = (rand() % (int)(m_vBoundingBoxMax.x - m_vBoundingBoxMin.x)) + m_vBoundingBoxMin.x;
	pPos->z = (rand() % (int)(m_vBoundingBoxMax.z - m_vBoundingBoxMin.z)) + m_vBoundingBoxMin.z;
	pPos->y = m_vBoundingBoxMax.y;
}

HRESULT CParticle::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_iTextureNum);
	return S_OK;
}

CParticle * CParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticle* pInstance = new CParticle(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CParticle::Free()
{
	CBaseEffect::Free();
}
