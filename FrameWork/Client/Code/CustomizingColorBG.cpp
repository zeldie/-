#include "stdafx.h"
#include "CustomizingColorBG.h"
#include "ScreenTex.h"

CCustomizingColorBG::CCustomizingColorBG(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_bChoose(false),
	m_wstrType(L"")
{
	m_vChangeColor = _vec4(1.f,1.f,1.f,1.f);
}


CCustomizingColorBG::~CCustomizingColorBG()
{
}

HRESULT CCustomizingColorBG::Ready_GameObject(wstring wstrType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_wstrType = wstrType;
	m_hDC = GetDC(g_hWnd);
	return S_OK;
}

_int CCustomizingColorBG::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CCustomizingColorBG::LateUpdate_GameObject(const _double & dTimeDelta)
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

	

	if (m_bChoose && Engine::MousePressing(Engine::DIM_LB))
	{
		COLORREF rgb = GetPixel(m_hDC, vMousePos.x, vMousePos.y);

		m_vChangeColor.x = (_float)GetRValue(rgb) / 255.f;
		m_vChangeColor.y = (_float)GetGValue(rgb) / 255.f;
		m_vChangeColor.z = (_float)GetBValue(rgb) / 255.f;
		m_vChangeColor.w = 1.f;

		////cout << iRvalue << '\t' << iGRvalue << '\t' << iBvalue << endl;
		CUIMgr::GetInstance()->ChangeColor(m_wstrType,m_vChangeColor);
	}	

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CCustomizingColorBG::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CCustomizingColorBG::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_CustomizingColorBG"));
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

HRESULT CCustomizingColorBG::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");

	return S_OK;
}

CCustomizingColorBG * CCustomizingColorBG::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CCustomizingColorBG* pInstance = new CCustomizingColorBG(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(wstrType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCustomizingColorBG::Free()
{
	ReleaseDC(g_hWnd, m_hDC);
	CUIObject::Free();
}
