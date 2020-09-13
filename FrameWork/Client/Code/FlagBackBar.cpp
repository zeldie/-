#include "stdafx.h"
#include "FlagBackBar.h"
#include "ScreenTex.h"

CFlagBackBar::CFlagBackBar(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CUIObject(pGraphicDev),
		m_eBackBarType(BACKBARTYPE_END),
		m_eGageColor(GAGECOLOR_END),
		m_dTime(0.f),
		m_dMaxTime(0.f)
{
}


CFlagBackBar::~CFlagBackBar()
{
}

HRESULT CFlagBackBar::Ready_GameObject(BACKBARTYPE eType, GAGECOLOR eColor, _double dTime, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eBackBarType = eType;
	m_eGageColor = eColor;
	m_dTime = 0.f;
	m_dMaxTime = dTime;

	return S_OK;
}

_int CFlagBackBar::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{
		return Engine::OBJ_DEAD;
	}

	CUIObject::Update_GameObject(dTimeDelta);

	m_dTime += dTimeDelta;

	return Engine::NO_EVENT;
}

_int CFlagBackBar::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);	
	
	if (m_dMaxTime <= m_dTime)
	{
		m_dTime = m_dMaxTime;
		m_bIsDead = true;
	}

	if (GAGE == m_eBackBarType)
	{
		m_pScreenTexBufferCom->VertexXControl_LeftDir(m_dTime / m_dMaxTime);
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CFlagBackBar::Render_Geometry(const _double & dTimeDelta)
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
	
	if (GAGE == m_eBackBarType)
		pEffect->BeginPass(5);
	else
		pEffect->BeginPass(1);

	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CFlagBackBar::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_FlagBackBar"));
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

HRESULT CFlagBackBar::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (GAGE == m_eBackBarType)
	{
		if (GAGECOLOR_BLUE == m_eGageColor)
		{
			pEffect->SetVector("g_vRGBA", &_vec4(0.28f, 0.96f, 0.96f, 1.f));
		}
		else if (GAGECOLOR_RED == m_eGageColor)
		{
			pEffect->SetVector("g_vRGBA", &_vec4(0.99f, 0.35f, 0.34f, 1.f));
		}
	}

	m_pTextureCom->SetTexture(pEffect,"g_DiffuseTexture", m_eBackBarType);

	return S_OK;
}

CFlagBackBar * CFlagBackBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, BACKBARTYPE eType, GAGECOLOR eColor, _double dTime, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CFlagBackBar* pInstance = new CFlagBackBar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, eColor, dTime,fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);
	
	return pInstance;
}

void CFlagBackBar::Free()
{
	CUIObject::Free();
}
