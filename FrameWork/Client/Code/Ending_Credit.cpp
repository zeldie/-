#include "stdafx.h"
#include "Ending_Credit.h"


CEnding_Credit::CEnding_Credit(LPDIRECT3DDEVICE9 pGraphicDev)
	:CNormalObject(pGraphicDev)
{
}


CEnding_Credit::~CEnding_Credit()
{
}

HRESULT CEnding_Credit::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(200.f, 2200.f, 0.f);

	return S_OK;
}

_int CEnding_Credit::Update_GameObject(const _double & dTimeDelta)
{
	CNormalObject::Update_GameObject(dTimeDelta);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f); //->직교투영

	_vec3 vDir = { 0.f, 10.f, 0.f };
	m_pTransformCom->Move_Pos(&vDir, 5, dTimeDelta);

	return Engine::NO_EVENT;
}

_int CEnding_Credit::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::Update_GameObject(dTimeDelta);

	_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	if (Engine::KeyDown(DIK_NUMPAD0))
		cout << vPos.x << " / " << vPos.y << " / " << vPos.z << endl;


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CEnding_Credit::Render_Geometry(const _double & dTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);	// 1인자 : 현재 쉐이더 파일이 갖고 있는 최대 pass개수 , 2인자 : 시작하는 플래그 방식
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
	m_pGraphicDev->SetTexture(0, nullptr);

}

HRESULT CEnding_Credit::Clone_Component()
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

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Ending_Credit"));
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

HRESULT CEnding_Credit::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	_matrix		matWorld, matView;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixIdentity(&matView); //직교투영

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");
	return S_OK;
}

CEnding_Credit * CEnding_Credit::Create(LPDIRECT3DDEVICE9 pGraphicDev, float X, float Y)
{
	CEnding_Credit* pInstance = new CEnding_Credit(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(X - (WINCX >> 1), -Y + (WINCY >> 1), 0.f);

	return pInstance;
}

void CEnding_Credit::Free()
{
	CNormalObject::Free();
}
