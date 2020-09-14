#include "stdafx.h"
#include "RunPlayerItemSlot.h"
#include "ScreenTex.h"
#include "RunPlayerObserver.h"

CRunPlayerItemSlot::CRunPlayerItemSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eItemSlot(ITEMSLOT_END),
	m_pRunPlayerObserver(nullptr),
	m_bLateInit(true),
	m_dDuration(0.f),
	m_eRunPlayerPower(RUN_POWER_END),
	m_dMaxDuration(0.f),
	m_bOne(true)
{
}


CRunPlayerItemSlot::~CRunPlayerItemSlot()
{
}

HRESULT CRunPlayerItemSlot::Ready_GameObject(ITEMSLOT eType, ITEMTYPE eItemType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;
	
	m_eItemSlot = eType;
	m_eItemType = eItemType;
	m_bRenderUI = bBool;
	
	if (RECHARGE == m_eItemSlot)
	{
		//옵저버 신청
		m_pRunPlayerObserver = CRunPlayerObserver::Create();
		if (nullptr == m_pRunPlayerObserver)
			return E_FAIL;
		Engine::Subscribe(Engine::RUNPLAYER, m_pRunPlayerObserver);

	}
	return S_OK;
}

_int CRunPlayerItemSlot::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	
	if (RECHARGE == m_eItemSlot)
	{
		if (m_bOne)
		{
			m_eRunPlayerPower = m_pRunPlayerObserver->Get_Power();
			
			if (RUN_POWER_BIG == m_eRunPlayerPower || RUN_POWER_SPEEDUP == m_eRunPlayerPower)
			{
				m_bOne = false;
			}
		}
	}

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CRunPlayerItemSlot::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);
	
	if (RECHARGE == m_eItemSlot)
	{
		if (RUN_POWER_BIG == m_eRunPlayerPower)
		{
			if (BIG == m_eItemType)
			{
				m_bRenderUI = true;
				if (m_bLateInit)
				{
					m_dMaxDuration = 5.f/*m_pRunPlayerObserver->Get_Time_Big()*/;

					m_dDuration = m_dMaxDuration;
					m_bLateInit = false;
				}

				m_dDuration -= dTimeDelta;
				_float fResultValue = (_float)m_dDuration / (_float)m_dMaxDuration;
				m_pScreenTexBufferCom->VertexYControl_UpDir(fResultValue);


				if (0.f >= m_dDuration)
				{
					m_bRenderUI = false;
					m_bLateInit = true;
					m_dMaxDuration = 0.f;
					m_dDuration = 0.f;
					m_bOne = true;
				}
			}
		}
		else if(RUN_POWER_SPEEDUP == m_eRunPlayerPower)
		{
			if (SPEEDUP == m_eItemType)
			{
				m_bRenderUI = true;
				if (m_bLateInit)
				{
					m_dMaxDuration = 3.f/*m_pRunPlayerObserver->Get_Time_SpeedUp()*/;

					m_dDuration = m_dMaxDuration;
					m_bLateInit = false;
				}

				m_dDuration -= dTimeDelta;
				_float fResultValue = (_float)m_dDuration / (_float)m_dMaxDuration;
				m_pScreenTexBufferCom->VertexYControl_UpDir(fResultValue);


				if (0.f >= m_dDuration)
				{
					m_bRenderUI = false;
					m_bLateInit = true;
					m_dMaxDuration = 0.f;
					m_dDuration = 0.f;
					m_bOne = true;
				}
			}
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CRunPlayerItemSlot::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CRunPlayerItemSlot::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_SkillSlot"));
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

HRESULT CRunPlayerItemSlot::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (BASIC == m_eItemSlot)
	{
		pEffect->SetVector("g_vRGBA", &_vec4(0.3f, 0.2f, 0.06f, 1.f));
	}
	else if (RECHARGE == m_eItemSlot)
	{
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 0.9f, 0.20f, 0.8f));
	}
	else if (OUTLINE == m_eItemSlot)
	{
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 1.f, 1.f));
	}

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eItemSlot);
	return S_OK;
}

CRunPlayerItemSlot * CRunPlayerItemSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev, ITEMSLOT eType, ITEMTYPE eItemType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CRunPlayerItemSlot* pInstance = new CRunPlayerItemSlot(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, eItemType,bBool, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunPlayerItemSlot::Free()
{
	CUIObject::Free();

	if (RECHARGE == m_eItemSlot)
	{
		Engine::UnSubscribe(Engine::RUNPLAYER, m_pRunPlayerObserver);
		Engine::Safe_Release(m_pRunPlayerObserver);
	}
}
