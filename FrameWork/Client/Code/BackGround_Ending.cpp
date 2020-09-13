#include "stdafx.h"
#include "BackGround_Ending.h"


CBackGround_Ending::CBackGround_Ending(LPDIRECT3DDEVICE9 pGraphicDev)
	:CNormalObject(pGraphicDev),
	m_pSoundMgr(CSoundMgr::Get_Instance())
{
}


CBackGround_Ending::~CBackGround_Ending()
{
}

HRESULT CBackGround_Ending::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(2.05f, 2.05f, 0.f);
	_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	vPos.z = 0.1f;
	m_pTransformCom->Set_Info(&vPos, Engine::INFO_POS);

	m_pSoundMgr->AllStop();
	m_pSoundMgr->BGMSTART(97);

	return S_OK;
}

_int CBackGround_Ending::Update_GameObject(const _double & dTimeDelta)
{
	CNormalObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CBackGround_Ending::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::Update_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CBackGround_Ending::Render_Geometry(const _double & dTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);	// 1인자 : 현재 쉐이더 파일이 갖고 있는 최대 pass개수 , 2인자 : 시작하는 플래그 방식
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
	m_pGraphicDev->SetTexture(0, nullptr);

}

HRESULT CBackGround_Ending::Clone_Component()
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

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_BackGround_Ending"));
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

HRESULT CBackGround_Ending::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");


	return S_OK;
}

CBackGround_Ending * CBackGround_Ending::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround_Ending* pInstance = new CBackGround_Ending(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBackGround_Ending::Free()
{
	CNormalObject::Free();
}
