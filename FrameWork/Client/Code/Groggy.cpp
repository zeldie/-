#include "stdafx.h"
#include "Groggy.h"
#include "ScreenTex.h"
#include "Belatos.h"
#include "ApostleObserver.h"

CGroggy::CGroggy(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eGroggyType(FULL),
	m_pMonster(nullptr),
	m_fResultValue(1.f),
	m_iMaxGroggy(0),
	m_bLateInit(true),
	m_pApostleObserver(nullptr)
{
}


CGroggy::~CGroggy()
{
}

HRESULT CGroggy::Ready_GameObject(GROGGYTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eGroggyType = eType;
	m_eUIType = CUIMgr::GetInstance()->Get_UIType();

	if (CUIMgr::UITYPE_SHOOTING == m_eUIType)
	{
		//탄막 옵저버 신청
		m_pApostleObserver = CApostleObserver::Create();
		if (nullptr == m_pApostleObserver)
			return E_FAIL;
		Engine::Subscribe(Engine::APOSTLE, m_pApostleObserver);
	}

	return S_OK;
}

_int CGroggy::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	
	if (m_bLateInit)
	{//스테이지별로 나눠서 초기화(UIType별)

		if (CUIMgr::UITYPE_SHOOTING == m_eUIType)
		{
			m_pMonster = nullptr;

			m_iMaxGroggy = m_pApostleObserver->Get_MaxSuperArmor();
		}
		else if (CUIMgr::UITYPE_BOSS == m_eUIType)
		{
			m_pMonster = dynamic_cast<CBelatos*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Belatos"));
			
			m_pApostleObserver = nullptr;

			m_iMaxGroggy = m_pMonster->Get_BaseInfo()->iMaxSuperAmmor;
		}		
		m_bLateInit = false;
	}

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CGroggy::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);
	
	int iGroggy = 0;
	if (CUIMgr::UITYPE_SHOOTING == m_eUIType)
	{
		iGroggy = m_pApostleObserver->Get_SuperArmor();
	}
	else if (CUIMgr::UITYPE_BOSS == m_eUIType)
	{
		if (nullptr == m_pMonster)
			return Engine::NO_EVENT;

		iGroggy = m_pMonster->Get_BaseInfo()->iSuperAmmor;
	}

	if (m_iMaxGroggy <= iGroggy)
		iGroggy = m_iMaxGroggy;


	/////////////////////TEST
	//if (Engine::KeyDown(DIK_4))
	//{

	//	m_fTest -= dTimeDelta*1000.f;

	//	m_bRenderUI = true;
	//}
	//////////////////////////////

	if (FULL == m_eGroggyType)
	{
		///////////////////////TEST	
		//m_fResultValue = (float)m_fTest / 100.f;
		//m_pScreenTexBufferCom->VertexXControl_LeftDir(m_fResultValue);
		//////////////////////////


		if (0 >= iGroggy)
			m_bRenderUI = false;
		else
		{
			m_bRenderUI = true;
			m_fResultValue = (float)iGroggy / (float)m_iMaxGroggy;
			m_pScreenTexBufferCom->VertexXControl_LeftDir(m_fResultValue);
		}

	}
	else if (EMPTY == m_eGroggyType)
	{
		///////////////////////TEST	
		//if (0 >= m_fTest)
		//	m_bRenderUI = true;
		//else
		//	m_bRenderUI = false;
		//////////////////////////

		if (0 >= iGroggy)
			m_bRenderUI = true;
		else
			m_bRenderUI = false;
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CGroggy::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CGroggy::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Groggy"));
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

HRESULT CGroggy::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eGroggyType);
	return S_OK;
}

CGroggy * CGroggy::Create(LPDIRECT3DDEVICE9 pGraphicDev, GROGGYTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CGroggy* pInstance = new CGroggy(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	
	return pInstance;
}

void CGroggy::Free()
{
	CUIObject::Free();

	if (CUIMgr::UITYPE_SHOOTING == m_eUIType)
	{
		Engine::UnSubscribe(Engine::APOSTLE, m_pApostleObserver);
		Engine::Safe_Release(m_pApostleObserver);
	}
}
