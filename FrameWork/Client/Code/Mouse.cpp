#include "stdafx.h"
#include "Mouse.h"


CMouse::CMouse(LPDIRECT3DDEVICE9 pGraphicDev)
	:CNormalObject(pGraphicDev),
	m_eMouseType(MOUSE_END),
	m_ePass(2),
	m_fRotateAngle(0.f)
{
}


CMouse::~CMouse()
{
}

void CMouse::Change_MouseType(MOUSETYPE eMouseType)
{
	if (nullptr == m_pTransformCom)
		return;

	m_eMouseType = eMouseType;
	switch (eMouseType)
	{
	case CMouse::MOUSE_PICKING:
		m_pTransformCom->Set_Scale(30.f, 30.f, 0.f);
		m_pTransformCom->Set_Angle(0.f, 0.f, 0.f);
		break;
	case CMouse::MOUSE_CROSSHAIR:
		m_pTransformCom->Set_Scale(30.f, 30.f, 0.f);
		m_pTransformCom->Set_Angle(0.f, 0.f, 0.f);
		m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
		break;
	case CMouse::MOUSE_LOCK:
		break;
	case CMouse::MOUSE_INVISIBLE:
		m_pTransformCom->Set_Angle(0.f, 0.f, 0.f);
		m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
		break;
	case CMouse::MOUSE_ZOOM:
		m_pTransformCom->Set_Scale(100.f, 100.f, 0.f);
		m_pTransformCom->Set_Angle(0.f, 0.f, 0.f);
		m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
		break;
	case CMouse::MOUSE_END:
		break;
	default:
		break;
	}
}

HRESULT CMouse::Ready_GameObject(MOUSETYPE eMouseType)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Scale(30.f, 30.f, 0.f);
	m_eMouseType = eMouseType;
	return S_OK;
}

_int CMouse::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	_vec3 vMousePos;
	if (m_eMouseType == MOUSE_PICKING)
	{
		vMousePos = _vec3(_float(pt.x) - WINCX * 0.5f, -(_float(pt.y) - WINCY * 0.5f), 0.f);
		m_pTransformCom->Set_Info(&vMousePos, Engine::INFO_POS);
	}
	
	CNormalObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CMouse::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);
	if(m_eMouseType != MOUSE_INVISIBLE)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	return Engine::NO_EVENT;
}

void CMouse::Render_Geometry(const _double & dTimeDelta)
{
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;

	_uint iPassMax = 0;
	if (m_eMouseType == MOUSE_LOCK)
	{
		m_pTransformCom->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom->Set_Pos(0.f, -3.5f, 0.f);
		m_pTransformCom->Rotation(Engine::ROT_Z, _float(dTimeDelta * 100.f));
		m_pTransformCom->Update_Component(dTimeDelta);
		_matrix		matWorld;

		m_pTransformCom->Get_WorldMatrix(&matWorld);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", 3);
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 0.f, 0.f, 1.f));
		pEffect->Begin(&iPassMax, 0);
		pEffect->BeginPass(m_ePass);


		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
		m_pTransformCom->Set_Scale(20.f, 20.f, 0.f);
		m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
		m_pTransformCom->Rotation(Engine::ROT_Z, _float(dTimeDelta * 100.f));
		m_pTransformCom->Update_Component(dTimeDelta);
		m_pTransformCom->Get_WorldMatrix(&matWorld);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", 4);
		pEffect->SetVector("g_vRGBA", &_vec4(0.85f, 0.28f, 0.08f, 1.f));
		pEffect->Begin(&iPassMax, 0);
		pEffect->BeginPass(m_ePass);


		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}
	else if (m_eMouseType == MOUSE_ZOOM)
	{
		m_pTransformCom->Set_Scale(332.f, 190.f, 0.f);
		m_pTransformCom->Set_Pos(0.f, -3.5f, 0.5f);
		m_pTransformCom->Set_Angle(0.f, 0.f, 0.f);
		m_pTransformCom->Update_Component(dTimeDelta);
		_matrix		matWorld;

		m_pTransformCom->Get_WorldMatrix(&matWorld);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", 5);
		pEffect->Begin(&iPassMax, 0);
		pEffect->BeginPass(m_ePass);


		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
		m_fRotateAngle += _float(dTimeDelta * 20.f);
		m_pTransformCom->Set_Scale(306.f, 306.f, 0.f);
		m_pTransformCom->Set_Pos(0.f, 0.f, 0.5f);
		m_pTransformCom->Set_Angle(0.f, 0.f, m_fRotateAngle);
		m_pTransformCom->Update_Component(dTimeDelta);
		m_pTransformCom->Get_WorldMatrix(&matWorld);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", 6);
		pEffect->Begin(&iPassMax, 0);
		pEffect->BeginPass(m_ePass);


		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}
	else
	{
		pEffect->Begin(&iPassMax, 0);
		pEffect->BeginPass(m_ePass);


		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}
	Engine::Safe_Release(pEffect);
}

HRESULT CMouse::Clone_Component()
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
	pComponent = m_pBufferCom = Engine::CRcTex::Create(m_pGraphicDev);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	//Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Mouse"));
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

HRESULT CMouse::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);                                                                       
	_matrix	 matView, matProj;
	D3DXMatrixIdentity(&matView);
	D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	switch (m_eMouseType)
	{
	case CMouse::MOUSE_PICKING:
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");
		m_ePass = 2;
		break;
	case CMouse::MOUSE_CROSSHAIR:
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", 1);
		m_pTextureCom->SetTexture(pEffect, "g_MaskingTexture", 2);
		m_ePass = 13;
		break;
	case CMouse::MOUSE_LOCK:
		m_ePass = 5;
		break;
	case CMouse::MOUSE_END:
		break;
	default:
		break;
	}
	return S_OK;
}

CMouse * CMouse::Create(LPDIRECT3DDEVICE9 pGraphicDev, MOUSETYPE eMouseType)
{
	CMouse* pInstance = new CMouse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eMouseType)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMouse::Free()
{
	CNormalObject::Free();
}
