#include "stdafx.h"
#include "OverHeadEffect.h"
#include "RcTex.h"

COverHeadEffect::COverHeadEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CNormalObject(pGraphicDev),
	m_eKarmaType(KARMA_END),
	m_pTargetTransformCom(nullptr),
	m_dTime(1.f),
	m_fAlpha(0.f),
	m_vScale(0.f, 0.f, 0.f)
{
}

COverHeadEffect::~COverHeadEffect()
{
}

HRESULT COverHeadEffect::Ready_GameObject(KARMATYPE eType, Engine::CTransform* pTargetTransform)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_eKarmaType = eType;
	m_pTargetTransformCom = pTargetTransform;

	m_vScale = _vec3(100.f, 100.f, 100.f);
	//m_pTransformCom->Set_Scale(&m_vScale);

	_vec3 vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);
	m_pTransformCom->Set_Pos(&vTargetPos);

	//Compute_ViewZ(&vTargetPos);

	//m_vScale *= (m_fViewZ*0.001f);

	return S_OK;
}

_int COverHeadEffect::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	//크기, 위치
	_vec3 vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);
	m_pTransformCom->Set_Pos(&vTargetPos);

	//Compute_ViewZ(&vTargetPos);

	//m_vScale *= (m_fViewZ*0.001f);

	CNormalObject::Update_GameObject(dTimeDelta);

	BillBoard(&m_vScale);

	m_dTime -= dTimeDelta;
	
	//m_vScale -= _vec3(dTimeDelta, dTimeDelta, dTimeDelta);

	return Engine::NO_EVENT;
}

_int COverHeadEffect::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	
	if (0.f >= m_dTime)
	{
		m_dTime = 0.f;
		m_bIsDead = true;
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void COverHeadEffect::Render_Geometry(const _double & dTimeDelta)
{
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(23);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT COverHeadEffect::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	//Buffer
	pComponent = m_pBufferCom = Engine::CRcTex::Create(m_pGraphicDev);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_OverHeadEffect"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT COverHeadEffect::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	switch (m_eKarmaType)
	{
	case TEXTURE_WEAPONCHANGE_DS:
		pEffect->SetVector("g_vRGBA", &_vec4(0.52f, 0.31f, 1.f, 0.8f));
		break;
	case TEXTURE_WEAPONCHANGE_TS:
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 0.f, m_dTime));
		break;
	case TEXTURE_WEAPONCHANGE_ORB:
		pEffect->SetVector("g_vRGBA", &_vec4(0.039f, 1.f, 0.67f, m_dTime));
		break;
	case TEXTURE_WEAPONCHANGE_LB:
		pEffect->SetVector("g_vRGBA", &_vec4(0.30f, 1.f, 0.15f, m_dTime));
		break;
	}
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture"); //baseplayer에서 이넘값 0번이 common 이므로 1빼주야 잘맞음
	return S_OK;
}

COverHeadEffect * COverHeadEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, KARMATYPE eType, Engine::CTransform* pTargetTransform)
{
	COverHeadEffect* pInstance = new COverHeadEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, pTargetTransform)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void COverHeadEffect::Free()
{
	CNormalObject::Free();
}
