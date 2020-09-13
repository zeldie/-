#include "stdafx.h"
#include "CustomizingSelectBox.h"
#include "ScreenTex.h"

CCustomizingSelectBox::CCustomizingSelectBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eSelectBoxType(SELECTTYPE_END),
	m_bChoose(false),
	m_bIsWearing(false)
{
}


CCustomizingSelectBox::~CCustomizingSelectBox()
{
}

HRESULT CCustomizingSelectBox::Ready_GameObject(SELECTTYPE eType, _uint iIndex, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eSelectBoxType = eType;
	m_iIndex = iIndex;

	return S_OK;
}

_int CCustomizingSelectBox::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CCustomizingSelectBox::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	_vec3 vMousePos;

	if(m_bIsWearing)
		m_eSelectBoxType = AFTERSELECT;
	else
	{
		if (m_pScreenTexBufferCom->CheckAroundMouse(g_hWnd, &vMousePos))
		{
			m_bChoose = true;
			m_eSelectBoxType = BEFORESELECT;
		}
		else
		{
			m_bChoose = false;
			m_eSelectBoxType = BASIC;
		}

	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CCustomizingSelectBox::Render_Geometry(const _double & dTimeDelta)
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

	if (BASIC == m_eSelectBoxType)
		pEffect->BeginPass(5);
	else
		pEffect->BeginPass(2);

	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

}

HRESULT CCustomizingSelectBox::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_CustomizingSelectBox"));
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

HRESULT CCustomizingSelectBox::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (BASIC == m_eSelectBoxType)
		pEffect->SetVector("g_vRGBA", &_vec4(0.41f, 0.41f, 0.41f,0.8f));

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eSelectBoxType);

	return S_OK;
}

CCustomizingSelectBox * CCustomizingSelectBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, SELECTTYPE eType, _uint iIndex,  _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CCustomizingSelectBox* pInstance = new CCustomizingSelectBox(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, iIndex,fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCustomizingSelectBox::Free()
{
	CUIObject::Free();
}
