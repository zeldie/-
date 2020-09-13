#include "stdafx.h"
#include "Button.h"
#include "ScreenTex.h"
#include "ReadyBattle.h"

CButton::CButton(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CUIObject(pGraphicDev),
		m_bChoose(false),
		m_bClick(false),
		m_bReady(false),
		m_bSound(false)
{
}


CButton::~CButton()
{
}

HRESULT CButton::Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	return S_OK;
}

_int CButton::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CButton::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (!m_bReady)
	{
		_vec3 vMousePos;

		if (m_pScreenTexBufferCom->CheckAroundMouse(g_hWnd, &vMousePos))
			m_bChoose = true;
		else
			m_bChoose = false;
		m_bSound = false;
	}

	if (m_bChoose && Engine::MouseUp(Engine::DIM_LB))
	{
		m_bReady = true;
		m_bClick = true;
		dynamic_cast<CReadyBattle*>(Engine::Get_GameObject(Engine::UI, L"ReadyBattle"))->Set_Start();
		
		if (!m_bSound)
		{
			m_bSound = true;
			_uint i = rand() % 6 + 18;
			CSoundMgr::Get_Instance()->SoundOn(i);
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CButton::Render_Geometry(const _double & dTimeDelta)
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

//_bool CButton::CheckAroundMouse(HWND hWnd)
//{
//	//POINT	ptMouse{};
//
//	//GetCursorPos(&ptMouse);
//	//ScreenToClient(hWnd, &ptMouse);
//
//	//_vec3 vMousePos = _vec3((_float)ptMouse.x , (_float)ptMouse.y, 0.f);
//
//	//_vec3 vBufferVtx = m_pScreenTexBufferCom->Get_vStartPos();
//	//_vec3 vBufferSize = m_pScreenTexBufferCom->Get_vSize();
//
//	//_vec3 vVertex[4];
//	//vVertex[0] = _vec3(vBufferVtx.x , vBufferVtx.y,0.f);
//	//vVertex[1] = _vec3(vBufferVtx.x + vBufferSize.x, vBufferVtx.y, 0.f);
//	//vVertex[2] = _vec3(vBufferVtx.x + vBufferSize.x , vBufferVtx.y + vBufferSize.y, 0.f);
//	//vVertex[3] = _vec3(vBufferVtx.x , vBufferVtx.y + vBufferSize.y, 0.f);
//
//	//_vec2 vMouseDir[4] = {
//	//	vMousePos - vVertex[0],
//	//	vMousePos - vVertex[1],
//	//	vMousePos - vVertex[2],
//	//	vMousePos - vVertex[3]
//	//};
//
//	//_vec2 vLine[4] = {
//	//	vVertex[1] - vVertex[0],
//	//	vVertex[2] - vVertex[1],
//	//	vVertex[3] - vVertex[2],
//	//	vVertex[0] - vVertex[3]
//	//};
//
//	//_vec2 vNormal[4] = {};
//	//for (_uint i = 0; i < 4; ++i)
//	//{
//	//	vNormal[i] = _vec2(-vLine[i].y, vLine[i].x);
//
//	//	D3DXVec2Normalize(&vNormal[i], &vNormal[i]);
//	//	D3DXVec2Normalize(&vMouseDir[i], &vMouseDir[i]);
//	//}
//
//	//for (_uint i = 0; i < 4; ++i)
//	//{
//	//	if (0 > D3DXVec2Dot(&vNormal[i], &vMouseDir[i]))
//	//		return false;
//	//}
//
//	return true;
//}

HRESULT CButton::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Button"));
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

HRESULT CButton::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if(!m_bChoose)
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture",0);
	else if(m_bChoose && m_bClick)
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", 2);
	else if(m_bChoose)
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", 1);

	return S_OK;
}

CButton * CButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CButton* pInstance = new CButton(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CButton::Free()
{
	CUIObject::Free();
}
