#include "stdafx.h"
#include "KarmaSlot.h"
#include "ScreenTex.h"

CKarmaSlot::CKarmaSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev)
{
}


CKarmaSlot::~CKarmaSlot()
{
}

HRESULT CKarmaSlot::Ready_GameObject(SELECTKARMATYPE eSelectSlotType, KARMASLOTTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eKarmaSlotType = eType;
	m_eSelectKarmaSlotType = eSelectSlotType;

	return S_OK;
}

_int CKarmaSlot::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CKarmaSlot::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CKarmaSlot::Render_Geometry(const _double & dTimeDelta)
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

	if (INRESULTUI == m_eKarmaSlotType)
		pEffect->BeginPass(5);
	else
		pEffect->BeginPass(1);

	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

}

HRESULT CKarmaSlot::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_KarmaSlot"));
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

HRESULT CKarmaSlot::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (INRESULTUI == m_eKarmaSlotType)
	{
		pEffect->SetVector("g_vRGBA",&_vec4(0.91f, 0.91f, 0.91f,0.5f)); //0.18f,0.25f,0.23f,0.5f
	}

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture",m_eKarmaSlotType);
	return S_OK;
}

CKarmaSlot * CKarmaSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev, SELECTKARMATYPE eSelectSlotType, KARMASLOTTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CKarmaSlot* pInstance = new CKarmaSlot(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eSelectSlotType,eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CKarmaSlot::Free()
{
	CUIObject::Free();
}
