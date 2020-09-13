#include "stdafx.h"
#include "MatchingResult.h"
#include "ScreenTex.h"

CMatchingResult::CMatchingResult(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUIObject(pGraphicDev),
	  m_eResultType(RESULT_END),
	  m_dTime(1.f)
{
}


CMatchingResult::~CMatchingResult()
{
}

HRESULT CMatchingResult::Ready_GameObject(RESULT eType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eResultType = eType;
	m_bRenderUI = bBool;

	return S_OK;
}

_int CMatchingResult::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	
	if (START == m_eResultType)
	{
		m_dTime -= dTimeDelta*0.5f;
	}

	return Engine::NO_EVENT;
}

_int CMatchingResult::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (START == m_eResultType)
	{
		if (0.f >= m_dTime)
		{
			m_dTime = 0.f;
			m_bIsDead = true;
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CMatchingResult::Render_Geometry(const _double & dTimeDelta)
{
	if (m_bRenderUI)
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

		pEffect->BeginPass(5);

		m_pScreenTexBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

HRESULT CMatchingResult::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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
	pComponent = m_pScreenTexBufferCom = Engine::CScreenTex::Create(m_pGraphicDev, fX, fY, fSizeX, fSizeY, fViewZ);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_MatchingResult"));
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

HRESULT CMatchingResult::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 1.f, (_float)m_dTime));
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eResultType);
	return S_OK;
}

CMatchingResult * CMatchingResult::Create(LPDIRECT3DDEVICE9 pGraphicDev, RESULT eType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CMatchingResult* pInstance = new CMatchingResult(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, bBool, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMatchingResult::Free()
{
	CUIObject::Free();
}
