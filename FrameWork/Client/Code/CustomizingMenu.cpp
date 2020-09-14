#include "stdafx.h"
#include "CustomizingMenu.h"
#include "ScreenTex.h"

CCustomizingMenu::CCustomizingMenu(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_bChoose(false)
{
}


CCustomizingMenu::~CCustomizingMenu()
{
}

HRESULT CCustomizingMenu::Ready_GameObject(MENUTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eMenuType = eType;
	return S_OK;
}

_int CCustomizingMenu::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CCustomizingMenu::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (BG == m_eMenuType || LINE == m_eMenuType)
	{
		//얘넨 피킹검사 필요없어
	}
	else
	{
		_vec3 vMousePos;

		if (m_pScreenTexBufferCom->CheckAroundMouse(g_hWnd, &vMousePos))
			m_bChoose = true;
		else
			m_bChoose = false;


		if (m_bChoose && Engine::MouseUp(Engine::DIM_LB))
		{
			if (FACE1 == m_eMenuType)
			{
				CUIMgr::GetInstance()->CreateFaceUI(m_pGraphicDev);
			}
			else if (HAIR1 == m_eMenuType)
			{
				CUIMgr::GetInstance()->CreateHairUI(m_pGraphicDev);
			}
			else if (CLOTH1 == m_eMenuType)
			{
				CUIMgr::GetInstance()->CreateClothesUI(m_pGraphicDev);
			}
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CCustomizingMenu::Render_Geometry(const _double & dTimeDelta)
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

	//if(BG == m_eMenuType || LINE ==m_eMenuType)
		pEffect->BeginPass(1); 
	//else
	//	pEffect->BeginPass(2);

	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

	if (BG == m_eMenuType)
	{
		RenderCustomizeList();
	}
}

void CCustomizingMenu::RenderCustomizeList()
{
	if (CUIMgr::UITYPE_FACE_INCUSTOMIZE == CUIMgr::GetInstance()->Get_UIType())
	{
		Engine::Render_Font(L"Font_GODIC2", L"눈 모양", &_vec2(935.5f, 161.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); 
		Engine::Render_Font(L"Font_GODIC2", L"눈썹 모양", &_vec2(935.5f, 291.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //+130
		//Engine::Render_Font(L"Font_GODIC2", L"몸 색상", &_vec2(935.5f, 441.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //+150
	}
	else if (CUIMgr::UITYPE_HAIR_INCUSTOMIZE == CUIMgr::GetInstance()->Get_UIType())
	{
		Engine::Render_Font(L"Font_GODIC2", L"앞머리 모양", &_vec2(935.5f, 11.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Engine::Render_Font(L"Font_GODIC2", L"뒷머리 모양", &_vec2(935.5f, 141.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //+130
		Engine::Render_Font(L"Font_GODIC2", L"옆머리 모양", &_vec2(935.5f, 271.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //+130
		Engine::Render_Font(L"Font_GODIC2", L"꼬리머리 모양", &_vec2(935.5f, 401.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //+130
		//Engine::Render_Font(L"Font_GODIC2", L"머리 색상", &_vec2(935.5f, 531.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //+130
	}
	else if (CUIMgr::UITYPE_CLOTHES_INCUSTOMIZE == CUIMgr::GetInstance()->Get_UIType())
	{
		Engine::Render_Font(L"Font_GODIC2", L"상의", &_vec2(935.5f, 111.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Engine::Render_Font(L"Font_GODIC2", L"하의", &_vec2(935.5f, 241.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //+130
		Engine::Render_Font(L"Font_GODIC2", L"신발", &_vec2(935.5f, 371.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //+130
		Engine::Render_Font(L"Font_GODIC2", L"장갑", &_vec2(935.5f, 501.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //+130
	}
}

HRESULT CCustomizingMenu::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_CustomizingMenu"));
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

HRESULT CCustomizingMenu::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (BG == m_eMenuType || LINE == m_eMenuType)
	{
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eMenuType);
	}
	else
	{
		////임시test
		//pEffect->SetVector("g_vRGBA",&_vec4(1.f,0.f,0.f,1.f));
		//////////
		if (!m_bChoose)
			m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eMenuType);
		else if (m_bChoose)
			m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eMenuType + 1);
	}

	return S_OK;
}

CCustomizingMenu * CCustomizingMenu::Create(LPDIRECT3DDEVICE9 pGraphicDev, MENUTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CCustomizingMenu* pInstance = new CCustomizingMenu(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCustomizingMenu::Free()
{
	CUIObject::Free();
}
