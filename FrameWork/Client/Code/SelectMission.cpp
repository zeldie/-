#include "stdafx.h"
#include "SelectMission.h"
#include "ScreenTex.h"
#include "MissionCard.h"

CSelectMission::CSelectMission(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUIObject(pGraphicDev),
	m_pTarget(nullptr),
	m_fLengthSizeX(0.f)
{
}


CSelectMission::~CSelectMission()
{
}

HRESULT CSelectMission::Ready_GameObject(BARTYPE eType, Engine::CGameObject* pTarget, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eBarType = eType;
	m_pTarget = pTarget;

	return S_OK;
}

_int CSelectMission::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);

	return Engine::NO_EVENT;
}

_int CSelectMission::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (dynamic_cast<CMissionCard*>(m_pTarget)->Get_Choose())
	{
		m_fLengthSizeX += (_float)dTimeDelta;
		m_fLengthSizeY = (1.f - m_fLengthSizeX);

		if (1.f <= m_fLengthSizeX)
		{
			m_fLengthSizeX = 0.f;
		}

		if (HORIZON == m_eBarType)
		{
			m_pScreenTexBufferCom->VertexXControl_LeftDir(m_fLengthSizeX);
		}
		else if (VERTICAL1 == m_eBarType)
		{
			m_pScreenTexBufferCom->VertexYControl_UpDir(m_fLengthSizeY);
		}
		else if (VERTICAL2 == m_eBarType)
		{
			m_pScreenTexBufferCom->VertexYControl_DownDir(m_fLengthSizeX);
		}

		m_bRenderUI = true;
	}
	else
	{
		m_fLengthSizeX = 0.f;
		m_bRenderUI = false;
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CSelectMission::Render_Geometry(const _double & dTimeDelta)
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

		pEffect->BeginPass(2);

		m_pScreenTexBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

HRESULT CSelectMission::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_SelectMission"));
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

HRESULT CSelectMission::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	//pEffect->SetFloat("g_fU",0.25f);
	//pEffect->SetFloat("g_fV", 0.25f);
	//pEffect->SetFloat("g_fUNum", 0.25f);
	//pEffect->SetFloat("g_fVNum", 0.25f);
	//pEffect->SetVector("g_vRGBA", &_vec4(0.f, 0.f, 1.f, 1.f));

	if (VERTICAL2 == m_eBarType)
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eBarType - 1);
	else
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eBarType);

	return S_OK;
}

CSelectMission * CSelectMission::Create(LPDIRECT3DDEVICE9 pGraphicDev, BARTYPE eType, Engine::CGameObject* pTarget, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CSelectMission* pInstance = new CSelectMission(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, pTarget, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSelectMission::Free()
{
	CUIObject::Free();
}
