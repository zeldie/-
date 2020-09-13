#include "stdafx.h"
#include "PuzzleObj.h"
#include "ScreenTex.h"


CPuzzleObj::CPuzzleObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBaseObject(pGraphicDev)
	, m_pScreenTexBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_bChoose(false)
	, m_iIndex(0)
{
}

CPuzzleObj::~CPuzzleObj()
{
}

HRESULT CPuzzleObj::Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ, wstring wstrKey)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ, wstrKey)))
		return E_FAIL;

	return S_OK;
}

_int CPuzzleObj::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CBaseObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CPuzzleObj::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseObject::LateUpdate_GameObject(dTimeDelta);

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
		CPuzzleMgr::GetInstance()->Clicked_Puzzle(this);
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return Engine::NO_EVENT;
}

void CPuzzleObj::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CPuzzleObj::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ, wstring wstrKey)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrKey));
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

HRESULT CPuzzleObj::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_iTextureNum);

	return S_OK;
}

CPuzzleObj * CPuzzleObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ, wstring wstrKey)
{
	CPuzzleObj* pInstance = new CPuzzleObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(fX, fY, fSizeX, fSizeY, fViewZ, wstrKey)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPuzzleObj::Free()
{
	CBaseObject::Free();
}
