#include "stdafx.h"
#include "Number.h"
#include "RcTex.h"
#include "DamageBox.h"

CNumber::CNumber(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CNormalObject(pGraphicDev),
		m_eNumberType(NUMBERTYPE_END),
		m_pTarget(nullptr),
		m_pParentTransformCom(nullptr),
		m_dTime(0.f),
		m_fAlpha(1.f)
{
}

CNumber::~CNumber()
{
}

HRESULT CNumber::Ready_GameObject(CDamageBox* pTarget, Engine::CTransform* pParentTransform, NUMBERTYPE eType, _vec3 vPos, _vec3 vSize, _uint iNum)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_eNumberType = eType;
	m_iNumber = iNum;
	m_pTarget = pTarget;
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransformCom->Set_Scale(vSize.x, vSize.y, vSize.z);
	m_pParentTransformCom = pParentTransform;

	return S_OK;
}

_int CNumber::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CNormalObject::Update_GameObject(dTimeDelta);
	m_pTransformCom->Set_ParentMatrix(m_pParentTransformCom->Get_WorldMatrix()); //부모행렬->빌보드적용되어있음

	m_dTime += dTimeDelta*10.f;
	
	return Engine::NO_EVENT;
}

_int CNumber::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	if (3.f <= m_dTime)
	{
		m_fAlpha -= 0.05f;
	}

	if (4.f <= m_dTime)
	{
		m_bIsDead = true;
		m_pTarget->Set_Dead();
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;

}

void CNumber::Render_Geometry(const _double & dTimeDelta)
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
	pEffect->BeginPass(10);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}


HRESULT CNumber::Clone_Component()
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

	//Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_DamageBox"));
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

HRESULT CNumber::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);	

	pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 1.f, m_fAlpha));

	if (BASIC == m_eNumberType)
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_iNumber);
	else if (CRITICAL == m_eNumberType)
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_iNumber + 10);

	return S_OK;
}

CNumber * CNumber::Create(LPDIRECT3DDEVICE9 pGraphicDev, CDamageBox* pTarget, Engine::CTransform* pParentTransform, NUMBERTYPE eType, _vec3 vPos, _vec3 vSize, _uint iNum)
{
	CNumber* pInstance = new CNumber(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTarget,pParentTransform, eType, vPos, vSize, iNum)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CNumber::Free()
{
	CNormalObject::Free();
}
