#include "stdafx.h"
#include "CustomizingFrontHairUI.h"
#include "ScreenTex.h"

CCustomizingFrontHairUI::CCustomizingFrontHairUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eFrontHairType(FRONTHAIRTYPE_END),
	m_bChoose(false)
{
}


CCustomizingFrontHairUI::~CCustomizingFrontHairUI()
{
}

HRESULT CCustomizingFrontHairUI::Ready_GameObject(FRONTHAIRTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eFrontHairType = eType;
	return S_OK;
}

_int CCustomizingFrontHairUI::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CCustomizingFrontHairUI::LateUpdate_GameObject(const _double & dTimeDelta)
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
		//�ؽ��� �ֺ� �׵θ�
		CUIMgr::GetInstance()->CheckIsWearing(L"FrontHair", m_eFrontHairType);
		//���� ���� ����
		CUIMgr::GetInstance()->ChangeParts(L"FrontHair", m_eFrontHairType);
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CCustomizingFrontHairUI::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CCustomizingFrontHairUI::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_CustomizingFrontHairUI"));
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

HRESULT CCustomizingFrontHairUI::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eFrontHairType);

	return S_OK;
}

CCustomizingFrontHairUI * CCustomizingFrontHairUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, FRONTHAIRTYPE eType,  _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CCustomizingFrontHairUI* pInstance = new CCustomizingFrontHairUI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCustomizingFrontHairUI::Free()
{
	CUIObject::Free();
}
