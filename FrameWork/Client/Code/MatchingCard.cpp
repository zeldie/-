#include "stdafx.h"
#include "MatchingCard.h"
#include "ScreenTex.h"
//#include "ReadyBattle.h"

CMatchingCard::CMatchingCard(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBaseObject(pGraphicDev)
	, m_pScreenTexBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_bChoose(false)
	, m_bOpen(false)
	, m_eCardNum(NUM_BACK)
	, m_eCardState(NUM_BACK)
	, m_bDelay(false)
	, m_bCheat(false)
	, m_iIndex(0)
{
}

CMatchingCard::~CMatchingCard()
{
}

HRESULT CMatchingCard::Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	return S_OK;
}

_int CMatchingCard::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CBaseObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CMatchingCard::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseObject::LateUpdate_GameObject(dTimeDelta);

	// 플레이어가 사용하는 함수
	if (!m_bDelay)
	{
		if (!m_bOpen)
		{
			_vec3 vMousePos;

			if (m_pScreenTexBufferCom->CheckAroundMouse(g_hWnd, &vMousePos))
			{
				m_eCardState = NUM_BACK_SELECT;
				m_bChoose = true;
			}
			else
			{
				m_eCardState = NUM_BACK;
				m_bChoose = false;
			}
		}

		if (m_bChoose && Engine::MouseUp(Engine::DIM_LB))
		{
			if (!m_bOpen)
				CMatchingMgr::GetInstance()->Add_SelectCard(this);
			m_bOpen = true;
		}
	}
	else
	{
		m_eCardState = NUM_BACK;
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return Engine::NO_EVENT;
}

void CMatchingCard::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CMatchingCard::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Card"));
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

HRESULT CMatchingCard::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (m_eCardNum == NUM_END)
	{
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", NUM_BACK);
	}
	else
	{
		if (m_bCheat) // 치트키
		{
			m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eCardNum);
		}
		else // 정상
		{
			if (m_bOpen)
				m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eCardNum);
			else
				m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eCardState);
		}
	}

	return S_OK;
}

CMatchingCard * CMatchingCard::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CMatchingCard* pInstance = new CMatchingCard(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMatchingCard::Free()
{
	CBaseObject::Free();
}
