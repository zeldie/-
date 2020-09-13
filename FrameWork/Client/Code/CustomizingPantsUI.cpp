#include "stdafx.h"
#include "CustomizingPantsUI.h"
#include "ScreenTex.h"

CCustomizingPantsUI::CCustomizingPantsUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_ePantsType(PANTSTYPE_END),
	m_bChoose(false)
{
}


CCustomizingPantsUI::~CCustomizingPantsUI()
{
}

HRESULT CCustomizingPantsUI::Ready_GameObject(PANTSTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_ePantsType = eType;
	return S_OK;
}

_int CCustomizingPantsUI::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CCustomizingPantsUI::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	_vec3 vMousePos;

	if (m_pScreenTexBufferCom->CheckAroundMouse(g_hWnd, &vMousePos))
	{
		m_bChoose = true;
	}
	else
	{
		m_bChoose = false;
	}


	if (m_bChoose && Engine::MouseUp(Engine::DIM_LB))
	{
		//텍스쳐 주변 테두리
		CUIMgr::GetInstance()->CheckIsWearing(L"Pants", m_ePantsType);
		//실제 파츠 변경
		CUIMgr::GetInstance()->ChangeParts(L"Pants", m_ePantsType);
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CCustomizingPantsUI::Render_Geometry(const _double & dTimeDelta)
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

		pEffect->BeginPass(2);

		m_pScreenTexBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);


}

HRESULT CCustomizingPantsUI::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_CustomizingPantsUI"));
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

HRESULT CCustomizingPantsUI::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_ePantsType);

	return S_OK;
}

CCustomizingPantsUI * CCustomizingPantsUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, PANTSTYPE eType,  _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CCustomizingPantsUI* pInstance = new CCustomizingPantsUI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCustomizingPantsUI::Free()
{
	CUIObject::Free();
}
