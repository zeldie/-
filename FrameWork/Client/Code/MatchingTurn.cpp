#include "stdafx.h"
#include "MatchingTurn.h"
#include "ScreenTex.h"
#include "MatchingGameObserver.h"

CMatchingTurn::CMatchingTurn(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eTurn(CMatchingMgr::TURN_END),
	m_pMatchingGameObserver(nullptr),
	m_bLateInit(true),
	m_dTime(2.f),
	m_fAlpha(1.f)
{
}


CMatchingTurn::~CMatchingTurn()
{
}

HRESULT CMatchingTurn::Ready_GameObject(CMatchingMgr::MATCHINGTURN eType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eTurn = eType;
	m_bRenderUI = bBool;
	return S_OK;
}

_int CMatchingTurn::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	if (m_bLateInit)
	{
		//옵저버신청
		m_pMatchingGameObserver = CMatchingGameObserver::Create();
		if (nullptr == m_pMatchingGameObserver)
			return E_FAIL;
		Engine::Subscribe(Engine::MATCHING_GMAE, m_pMatchingGameObserver);

		m_bLateInit = false;
	}

	CUIObject::Update_GameObject(dTimeDelta);

	m_dTime -= dTimeDelta;

	return Engine::NO_EVENT;
}

_int CMatchingTurn::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (0.f >= m_dTime)
	{
		m_fAlpha -= (_float)dTimeDelta;
		if (0.f >= m_fAlpha)
		{
			m_dTime = 2.f;
			m_fAlpha = 1.f;
			m_bRenderUI = false;
		}
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CMatchingTurn::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CMatchingTurn::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_MatchingTurn"));
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

HRESULT CMatchingTurn::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	pEffect->SetVector("g_vRGBA",&_vec4(1.f, 1.f, 1.f, m_fAlpha));
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eTurn);
	return S_OK;
}

CMatchingTurn * CMatchingTurn::Create(LPDIRECT3DDEVICE9 pGraphicDev, CMatchingMgr::MATCHINGTURN eType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CMatchingTurn* pInstance = new CMatchingTurn(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, bBool, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMatchingTurn::Free()
{
	CUIObject::Free();

	Engine::UnSubscribe(Engine::MATCHING_GMAE, m_pMatchingGameObserver);
	Engine::Safe_Release(m_pMatchingGameObserver);

}
