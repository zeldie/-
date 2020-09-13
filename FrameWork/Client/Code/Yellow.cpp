#include "stdafx.h"
#include "Yellow.h"
#include "ScreenTex.h"

CYellow::CYellow(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CUIObject(pGraphicDev),
		m_eYellowType(YELLOWTYPE_END),
		m_dTime(0.f),
		m_fResultValue(0.f),
		m_fAlpha(0.8f)
{
}


CYellow::~CYellow()
{
}

HRESULT CYellow::Ready_GameObject(YELLOWTYPE eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eYellowType = eType;
	m_bRenderUI = bRender;

	if (GOLEFT == m_eYellowType)
		m_fResultValue = 1.f;
	else if (GORIGHT == m_eYellowType)
		m_fResultValue = 0.f;

	return S_OK;
}

_int CYellow::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CYellow::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (m_bRenderUI)
	{
		if (GOLEFT == m_eYellowType)
		{
			m_fResultValue -= _float(dTimeDelta);
			if (0.f >= m_fResultValue)
				m_bIsDead = true;

			m_pScreenTexBufferCom->VertexXControl_LeftDir(m_fResultValue);
		}
		else if (GORIGHT == m_eYellowType)
		{
			m_fResultValue += _float(dTimeDelta);
			if (1.f <= m_fResultValue)
				m_bIsDead = true;

			m_pScreenTexBufferCom->VertexXControl_RightDir(m_fResultValue);
		}

		m_fAlpha -= _float(dTimeDelta);
		if (0.1f >= m_fAlpha)
			m_fAlpha = 0.1f;

	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CYellow::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CYellow::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Yellow"));
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

HRESULT CYellow::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	pEffect->SetVector("g_vRGBA",&_vec4(1.f, 1.f, 1.f, m_fAlpha));
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");
	return S_OK;
}

CYellow * CYellow::Create(LPDIRECT3DDEVICE9 pGraphicDev, YELLOWTYPE eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CYellow* pInstance = new CYellow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, bRender, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CYellow::Free()
{
	CUIObject::Free();
}
