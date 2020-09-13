#include "stdafx.h"
#include "SuperArmor.h"
#include "ScreenTex.h"
#include "PlayerObserver.h"

CSuperArmor::CSuperArmor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUIObject(pGraphicDev),
	m_pPlayerObserver(nullptr),
	m_fResultValue(1.f),
	m_iMaxSuperArmor(0),
	m_bLateInit(true)
{
}


CSuperArmor::~CSuperArmor()
{
}

HRESULT CSuperArmor::Ready_GameObject(SUPERARMORTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eSuperArmor = eType;

	//옵저버신청
	m_pPlayerObserver = CPlayerObserver::Create();
	if (nullptr == m_pPlayerObserver)
		return E_FAIL;
	Engine::Subscribe(Engine::PLAYER, m_pPlayerObserver);
	
	return S_OK;
}

_int CSuperArmor::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	
	if (m_bLateInit)
	{
		m_iMaxSuperArmor = m_pPlayerObserver->Get_MaxSuperArmor();
		m_bLateInit = false;
	}

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CSuperArmor::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	_uint iSuperArmor = m_pPlayerObserver->Get_SuperArmor();

	if (m_iMaxSuperArmor <= iSuperArmor)
		iSuperArmor = m_iMaxSuperArmor;

	if (FULL == m_eSuperArmor)
	{
		if (0 >= iSuperArmor)
			m_bRenderUI = false;
		else
		{
			m_bRenderUI = true;
			m_fResultValue = (_float)iSuperArmor / (_float)m_iMaxSuperArmor;
			m_pScreenTexBufferCom->VertexXControl_LeftDir(m_fResultValue);
		}
	}
	else if (EMPTY == m_eSuperArmor)
	{
		if (0 >= iSuperArmor)
			m_bRenderUI = true;
		else
			m_bRenderUI = false;
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CSuperArmor::Render_Geometry(const _double & dTimeDelta)
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
		pEffect->BeginPass(1);

		m_pScreenTexBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

HRESULT CSuperArmor::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_SuperArmor"));
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

HRESULT CSuperArmor::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eSuperArmor);

	return S_OK;
}

CSuperArmor * CSuperArmor::Create(LPDIRECT3DDEVICE9 pGraphicDev, SUPERARMORTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CSuperArmor* pInstance = new CSuperArmor(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSuperArmor::Free()
{
	CUIObject::Free();

	Engine::UnSubscribe(Engine::PLAYER, m_pPlayerObserver);
	Engine::Safe_Release(m_pPlayerObserver);

}
