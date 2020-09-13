#include "stdafx.h"
#include "NoFuncObj.h"
#include "ScreenTex.h"
#include "Waiting.h"

CNoFuncObj::CNoFuncObj(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eNoFuncObjType(NOFUNCOBJ_END),
	m_dTime(0.f),
	m_fAlpha(0.f)
{
}


CNoFuncObj::~CNoFuncObj()
{
}

HRESULT CNoFuncObj::Ready_GameObject(NOFUNCOBJ eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eNoFuncObjType = eType;
	m_bRenderUI = bRender;

	return S_OK;
}

_int CNoFuncObj::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CNoFuncObj::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (NOFUNCOBJ_WAITINGTXT == m_eNoFuncObjType)		
	{
		if (dynamic_cast<CWaiting*>(Engine::Get_GameObject(Engine::UI, L"Waiting"))->Get_RenderWaiting())
			m_bRenderUI = true;
	}
	else if (NOFUNCOBJ_COOLDOWNTXT == m_eNoFuncObjType 
			|| NOFUNCOBJ_STAMINATXT == m_eNoFuncObjType
			|| NOFUNCOBJ_DEFICIENTMPTXT ==m_eNoFuncObjType)
	{
		if (m_bRenderUI)
		{
			m_dTime += dTimeDelta*10.f;
			m_fAlpha += _float(dTimeDelta);

			if (10.f <= m_dTime)
			{
				m_fAlpha -= 0.1f;
				m_pScreenTexBufferCom->VertexPositionYControl(_float(dTimeDelta*50.f));

				if (15.f <= m_dTime)
				{
					m_fAlpha = 0.f;
					m_dTime = 0.f;
					m_bRenderUI = false;
					
				}
			}
		}
		else
		{
			m_pScreenTexBufferCom->VertexOriginControl();
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CNoFuncObj::Render_Geometry(const _double & dTimeDelta)
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

		if (NOFUNCOBJ_TRIANGLE == m_eNoFuncObjType)
			pEffect->BeginPass(2);
		else if(NOFUNCOBJ_COOLDOWNTXT == m_eNoFuncObjType 
				|| NOFUNCOBJ_STAMINATXT == m_eNoFuncObjType
				|| NOFUNCOBJ_DEFICIENTMPTXT == m_eNoFuncObjType)
			pEffect->BeginPass(5);
		else
			pEffect->BeginPass(1);


		m_pScreenTexBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);

	}
}

HRESULT CNoFuncObj::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_NoFuncObj"));
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

HRESULT CNoFuncObj::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (NOFUNCOBJ_COOLDOWNTXT == m_eNoFuncObjType 
		|| NOFUNCOBJ_STAMINATXT == m_eNoFuncObjType
		|| NOFUNCOBJ_DEFICIENTMPTXT == m_eNoFuncObjType)
	{
		pEffect->SetVector("g_vRGBA",&_vec4(1.f,1.f,1.f,m_fAlpha));
	}

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eNoFuncObjType);
	return S_OK;
}

CNoFuncObj * CNoFuncObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, NOFUNCOBJ eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CNoFuncObj* pInstance = new CNoFuncObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, bRender, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CNoFuncObj::Free()
{
	CUIObject::Free();
}
