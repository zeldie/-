#include "stdafx.h"
#include "ReadyBattle.h"
#include "ScreenTex.h"
#include "Waiting.h"

CReadyBattle::CReadyBattle(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CUIObject(pGraphicDev),		
		m_bStart(false),
		m_fTime(0.f),
		m_fSpeed(0.f)
{
}

CReadyBattle::~CReadyBattle()
{
}

HRESULT CReadyBattle::Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	return S_OK;
}

_int CReadyBattle::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);

	if (m_bStart)
	{
		if (2.f >= m_fTime)
			m_fSpeed += 0.01f;
		else if (2.f <= m_fTime && 3.5f >= m_fTime)
			m_fSpeed = m_fSpeed;
		else if (3.5f <= m_fTime)
		{
			m_fSpeed -= 0.01f;
			if (0.f >= m_fSpeed)
				m_fSpeed = 0.01f;
		}
	}


	return Engine::NO_EVENT;
}

_int CReadyBattle::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (m_bStart)
	{
		m_fTime += (_float)dTimeDelta*m_fSpeed;

		if (5.5f <= m_fTime)
		{
			m_bStart = false;
			m_fTime = 5.5f;
			dynamic_cast<CWaiting*>(Engine::Get_GameObject(Engine::UI, L"Waiting"))->Set_RenderWaiting();
		}

	}

	///////////////////////testÁß
	//if (Engine::KeyDown(DIK_K))
	//{
	//	m_bStart = true;		
	//	m_fTime = 0.f;
	//}
	/////////////////////////////////


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CReadyBattle::Render_Geometry(const _double & dTimeDelta)
{
		//SHADER
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		if (pEffect == nullptr)
			return;
		Engine::Safe_AddRef(pEffect);

		if (FAILED(Setup_ShaderProps(pEffect, dTimeDelta)))
			return;

		_uint iPassMax = 0;

		pEffect->Begin(&iPassMax, 0);

		pEffect->BeginPass(7);

		m_pScreenTexBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
}

HRESULT CReadyBattle::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_ReadyBattle"));
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

HRESULT CReadyBattle::Setup_ShaderProps(LPD3DXEFFECT & pEffect, const _double& dTimeDelta)
{
	CBaseObject::Set_ShaderMatrix(pEffect);		

	pEffect->SetVector("g_vUV_U", &_vec4(m_fTime, 0.125f, 0.f, 0.f));
	pEffect->SetVector("g_vUV_V", &_vec4(0.f, 1.f, 0.f, 0.f));

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");
	return S_OK;
}

CReadyBattle * CReadyBattle::Create(LPDIRECT3DDEVICE9 pGraphicDev,  _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CReadyBattle* pInstance = new CReadyBattle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CReadyBattle::Free()
{
	CUIObject::Free();
}
