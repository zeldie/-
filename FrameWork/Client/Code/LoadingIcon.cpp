#include "stdafx.h"
#include "LoadingIcon.h"
#include "RcTex.h"
#include "BossStage.h"

CLoadingIcon::CLoadingIcon(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CNormalObject(pGraphicDev),
		m_fFrame(0.f),
		m_fRotAngle(0.f),
		m_bRenderWaiting(false),
		m_dTime(0.f),
		m_bSceneChange(false)
{
}

CLoadingIcon::~CLoadingIcon()
{
}

HRESULT CLoadingIcon::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(50.f, 50.f, 1.f);
	//m_pTransformCom->Set_Pos(&_vec3(700.f, 400.f, 0.f));

	m_pTransformCom->Set_Pos(820.f - (WINCX >> 1), -397.f + (WINCY >> 1), 0.f);

	return S_OK;
}

_int CLoadingIcon::Update_GameObject(const _double & dTimeDelta )
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CNormalObject::Update_GameObject(dTimeDelta);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f); //->직교투영

	return Engine::NO_EVENT;
}

_int CLoadingIcon::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	m_pTransformCom->Rotation(Engine::ROT_Z, (_float)dTimeDelta*300.f);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return Engine::NO_EVENT;

}

void CLoadingIcon::Render_Geometry(const _double & dTimeDelta)
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
		//pEffect->BeginPass(5);
		pEffect->BeginPass(1);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	
}

HRESULT CLoadingIcon::Clone_Component()
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
	pComponent = m_pBufferCom = Engine::CRcTex::Create(m_pGraphicDev);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_LogoIcon"));
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

HRESULT CLoadingIcon::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	//CBaseObject::Set_ShaderMatrix(pEffect);
	_matrix		matWorld, matView;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixIdentity(&matView); //직교투영

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	//pEffect->SetVector("g_vRGBA", &_vec4(0.3f, 0.59f, 1.f, 0.81f));
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", 0);

	return S_OK;
}

CLoadingIcon * CLoadingIcon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLoadingIcon* pInstance = new CLoadingIcon(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLoadingIcon::Free()
{
	CNormalObject::Free();
}
