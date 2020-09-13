#include "stdafx.h"
#include "MatchingScore.h"
#include "ScreenTex.h"
#include "MatchingGameObserver.h"

CMatchingScore::CMatchingScore(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eTurn(CMatchingMgr::TURN_END),
	m_eDigitType(DIGITTYPE_END),
	m_iNumber(0),
	m_pMatchingGameObserver(nullptr),
	m_bLateInit(true)
{
}


CMatchingScore::~CMatchingScore()
{
}

HRESULT CMatchingScore::Ready_GameObject(CMatchingMgr::MATCHINGTURN eType, DIGITTYPE eDigitType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eTurn = eType;
	m_eDigitType = eDigitType;


	return S_OK;
}

_int CMatchingScore::Update_GameObject(const _double & dTimeDelta)
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
	return Engine::NO_EVENT;
}

_int CMatchingScore::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	CMatchingMgr::MATCHINGTURN eType = m_pMatchingGameObserver->Get_Turn();

	_uint iScore = 0;
	if (CMatchingMgr::TURN_PLAYER == eType)
	{
		if (eType == m_eTurn)
		{
			iScore = m_pMatchingGameObserver->Get_PlayerPoint();
			if (UNITS == m_eDigitType)
			{
				m_iNumber = iScore % 10;
			}
			else if (TENS == m_eDigitType)
			{
				m_iNumber = iScore / 10;
			}
		}
	}
	else if (CMatchingMgr::TURN_AI == eType)
	{
		if (eType == m_eTurn)
		{
			iScore = m_pMatchingGameObserver->Get_AIPoint();
			if (UNITS == m_eDigitType)
			{
				m_iNumber = iScore % 10;
			}
			else if (TENS == m_eDigitType)
			{
				m_iNumber = iScore / 10;
			}
		}
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CMatchingScore::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CMatchingScore::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

HRESULT CMatchingScore::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_iNumber);
	return S_OK;
}

CMatchingScore * CMatchingScore::Create(LPDIRECT3DDEVICE9 pGraphicDev, CMatchingMgr::MATCHINGTURN eType, DIGITTYPE eDigitType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CMatchingScore* pInstance = new CMatchingScore(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, eDigitType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMatchingScore::Free()
{
	CUIObject::Free();

	Engine::UnSubscribe(Engine::MATCHING_GMAE, m_pMatchingGameObserver);
	Engine::Safe_Release(m_pMatchingGameObserver);

}
