#include "stdafx.h"
#include "WeaponUI.h"
#include "ScreenTex.h"

CWeaponUI::CWeaponUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eWeaponUIType(WEAPONUI_END)	,
	m_fFrame(0.f)
{
}


CWeaponUI::~CWeaponUI()
{
}

HRESULT CWeaponUI::Ready_GameObject(WEAPONUITYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{

	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eWeaponUIType = eType;
	return S_OK;
}

_int CWeaponUI::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);

	return Engine::NO_EVENT;
}

_int CWeaponUI::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CWeaponUI::Render_Geometry(const _double & dTimeDelta)
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

	if(WEAPONUI_FX == m_eWeaponUIType)
		pEffect->BeginPass(5);
	else if(WEAPONUI_LB == m_eWeaponUIType)
		pEffect->BeginPass(2);
	else
		pEffect->BeginPass(1);

	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

	if (WEAPONUI_LB == m_eWeaponUIType)
	{
		Engine::Render_Font(L"Font_GODICHEAVY", L"¹«±â È¹µæ ¼º°ø!!", &_vec2(280.f, 380.f), D3DXCOLOR(0.08f, 0.71f, 0.95f, 1.f));
	}
	else if (WEAPONUI_ORB == m_eWeaponUIType)
	{
		Engine::Render_Font(L"Font_GODICHEAVY", L"¹«±â È¹µæ ¼º°ø!!", &_vec2(970.f, 380.f), D3DXCOLOR(0.08f, 0.71f, 0.95f, 1.f));
	}
}

HRESULT CWeaponUI::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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
	pComponent = m_pScreenTexBufferCom = Engine::CScreenTex::Create(m_pGraphicDev,fX, fY, fSizeX, fSizeY, fViewZ);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_WeaponUI"));
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

HRESULT CWeaponUI::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);	
	
	if (WEAPONUI_FX == m_eWeaponUIType)
	{		
		pEffect->SetVector("g_vRGBA",&_vec4(0.64f, 1.f, 0.98f,0.7f));
	}

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eWeaponUIType);

	return S_OK;
}

CWeaponUI * CWeaponUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, WEAPONUITYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CWeaponUI* pInstance = new CWeaponUI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CWeaponUI::Free()
{
	CUIObject::Free();
}
