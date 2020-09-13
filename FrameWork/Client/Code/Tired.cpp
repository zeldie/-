#include "stdafx.h"
#include "Tired.h"

CTired::CTired(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNormalObject(pGraphicDev),
	m_pPlayerTransformCom(nullptr),
	m_vScale(1.f, 1.f, 1.f),
	m_bRender(true),
	m_pUIMgr(CUIMgr::GetInstance())
{
	D3DXMatrixIdentity(&m_matBill);
	D3DXMatrixIdentity(&m_matProj);
}

CTired::~CTired()
{
}

HRESULT CTired::Ready_GameObject(Engine::CTransform* pTargetTransform, _float fX, _float fY)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_pPlayerTransformCom = pTargetTransform;

	if (CUIMgr::UITYPE_SHOOTING == m_pUIMgr->Get_UIType())
	{
		m_pTransformCom->Set_Pos(fX - (WINCX >> 1), -fY + (WINCY >> 1), 0.f);
		m_pTransformCom->Set_Scale(128.f, 128.f, 1.f);
	}
	return S_OK;
}

_int CTired::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	if (CUIMgr::UITYPE_BOSS == m_pUIMgr->Get_UIType() || CUIMgr::UITYPE_CARTEL == m_pUIMgr->Get_UIType()
		|| CUIMgr::UITYPE_SHOOTING == m_pUIMgr->Get_UIType())
		m_bRender = true;
	else
		m_bRender = false;

	if (CUIMgr::UITYPE_BOSS == m_pUIMgr->Get_UIType())
	{
		//위치 지정
		_vec3 vPlayerPos;
		m_pPlayerTransformCom->Get_Info(Engine::INFO_POS, &vPlayerPos);

		m_pTransformCom->Set_Pos(&_vec3(vPlayerPos.x, vPlayerPos.y + 80.f, vPlayerPos.z));

		//크기 지정
		m_pTransformCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));

		Compute_ViewZ(&vPlayerPos);
		m_vScale = _vec3(m_fViewZ * 0.1f, m_fViewZ * 0.1f, 1.f);

		CNormalObject::Update_GameObject(dTimeDelta);
		BillBoard(&m_vScale);

	}
	else if (CUIMgr::UITYPE_SHOOTING == m_pUIMgr->Get_UIType())
	{
		CNormalObject::Update_GameObject(dTimeDelta);
		D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f); //->직교투영

	}


	return Engine::NO_EVENT;
}

_int CTired::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	if (CUIMgr::UITYPE_SHOOTING == m_pUIMgr->Get_UIType())
	{
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CTired::Render_Geometry(const _double & dTimeDelta)
{
	if (m_bRender)
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

		if (CUIMgr::UITYPE_SHOOTING == m_pUIMgr->Get_UIType())
			pEffect->BeginPass(2);
		else
			pEffect->BeginPass(11);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

HRESULT CTired::Clone_Component()
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Tired"));
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

HRESULT CTired::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	if (CUIMgr::UITYPE_SHOOTING == m_pUIMgr->Get_UIType())
	{
		_matrix		matWorld, matView;

		m_pTransformCom->Get_WorldMatrix(&matWorld);
		D3DXMatrixIdentity(&matView); //직교투영

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &m_matProj);

	}
	else
		CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");

	return S_OK;
}

CTired * CTired::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTargetTransform, _float fX, _float fY)
{
	CTired* pInstance = new CTired(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTargetTransform, fX, fY)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTired::Free()
{
	CNormalObject::Free();
}
