#include "stdafx.h"
#include "Skill.h"
#include "ScreenTex.h"

CSkill::CSkill(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eSkillType(SKILLTYPE_END),
	m_eKeyType(Engine::KEYGUIDE_END),
	m_wstrTextureName(L"")
{
}


CSkill::~CSkill()
{
}

HRESULT CSkill::Ready_GameObject(const wstring& wstrTexture, SKILLTYPE eType, Engine::KEYGUIDE eKeyType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	m_wstrTextureName = wstrTexture;

	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eSkillType = eType;
	m_eKeyType = eKeyType;

	return S_OK;
}

_int CSkill::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CSkill::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CSkill::Render_Geometry(const _double & dTimeDelta)
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

	pEffect->BeginPass(2);

	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CSkill::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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
	pComponent = m_pScreenTexBufferCom = Engine::CScreenTex::Create(m_pGraphicDev,fX, fY, fSizeX, fSizeY, fViewZ);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	// Texture
	wstring ComponentTag = L"";
	ComponentTag = L"Texture_" + m_wstrTextureName;

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, ComponentTag));
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

HRESULT CSkill::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);	

	if (SKILL_BASIC == m_eSkillType)
	{
		if(Engine::KEYGUIDE_LSHIFT == m_eKeyType)
			m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", (int)m_eKeyType - (int)Engine::KEYGUIDE_LSHIFT);
		else if(Engine::KEYGUIDE_TAB == m_eKeyType)
			m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", (int)m_eKeyType - (int)Engine::KEYGUIDE_LSHIFT);
	}
	else
	{
		if(Engine::KEYGUIDE::KEYGUIDE_Q ==  m_eKeyType)
			m_pTextureCom->SetTexture(pEffect,"g_DiffuseTexture", (int)m_eKeyType - 2);
		else
			m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", (int)m_eKeyType);

	}

	return S_OK;
}

CSkill * CSkill::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrTexture, SKILLTYPE eType, Engine::KEYGUIDE eKeyType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CSkill* pInstance = new CSkill(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(wstrTexture, eType, eKeyType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSkill::Free()
{
	CUIObject::Free();
}
