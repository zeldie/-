#include "stdafx.h"
#include "UltimateSlot.h"
#include "ScreenTex.h"
#include "UltimateRecharge.h"

CUltimateSlot::CUltimateSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eUltimateType(ULTIMATETYPE_END),
	m_dbRechareTime(0.f),
	m_fFrame(0.f),
	m_fResultValue(0.f)
{
}


CUltimateSlot::~CUltimateSlot()
{
}

HRESULT CUltimateSlot::Ready_GameObject(ULTIMATETYPE eType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eUltimateType = eType;
	m_bRenderUI = bBool;
	return S_OK;
}

_int CUltimateSlot::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	
	if (RECHARGE_OUTLINE == m_eUltimateType)
	{
		m_dbRechareTime += dTimeDelta;
		
		m_fFrame += 16.f * _float(dTimeDelta);
		if (16.f <= m_fFrame)
			m_fFrame = 0.f;

	}
	if (RECHARGE_INSIDE == m_eUltimateType)
	{
		if (m_bRenderUI)
		{			
			m_fResultValue += _float(dTimeDelta) * 0.05f; //ƒ≈∏¿” ->20√ 
			if (1.f <=m_fResultValue)
			{
				m_fResultValue = 1.f;
				CUIMgr::GetInstance()->Set_QSkillEnd();
			}
		}
	}
	return Engine::NO_EVENT;
}

_int CUltimateSlot::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (RECHARGE_INSIDE == m_eUltimateType)
	{		
		if (m_bRenderUI)
		{
			m_pScreenTexBufferCom->VertexYControl_UpDir(m_fResultValue);
		}
		else
			m_fResultValue = 0.f;
	}
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CUltimateSlot::Render_Geometry(const _double & dTimeDelta)
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

		if (RECHARGE_OUTLINE == m_eUltimateType)
		{
			pEffect->BeginPass(6);
		}
		else if (RECHARGE_INSIDE == m_eUltimateType)
		{
			pEffect->BeginPass(5);
		}
		else
			pEffect->BeginPass(1);

		m_pScreenTexBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

HRESULT CUltimateSlot::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_UltimateSlot"));
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

HRESULT CUltimateSlot::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);	

	if (RECHARGE_OUTLINE == m_eUltimateType)
	{
		pEffect->SetVector("g_vUV_U", &_vec4(((_ulong)m_fFrame % 4) * 0.25f, 0.25f, 0.f,0.f));
		pEffect->SetVector("g_vUV_V",&_vec4(((_ulong)m_fFrame / 4) * 0.25f, 0.25f, 0.f, 0.f));

		pEffect->SetVector("g_vRGBA", &_vec4(0.99f, 0.77f, 0.f, 1.f));
	}
	else if (RECHARGE_INSIDE == m_eUltimateType)
	{
		pEffect->SetVector("g_vRGBA", &_vec4(0.99f, 0.77f, 0.f, 1.f));
	}

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eUltimateType);

	return S_OK;
}

CUltimateSlot * CUltimateSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev, ULTIMATETYPE eType,_bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CUltimateSlot* pInstance = new CUltimateSlot(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eType, bBool, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);
	
	return pInstance;
}

void CUltimateSlot::Free()
{
	CUIObject::Free();
}
