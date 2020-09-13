#include "stdafx.h"
#include "AIBackBar.h"
#include "RcTex.h"

CAIBackBar::CAIBackBar(LPDIRECT3DDEVICE9 pGraphicDev)
	:CNormalObject(pGraphicDev),
	m_eBackBarType(BACKBARTYPE_END),
	m_pParentTransformCom(nullptr),
	m_vScale(0.f, 0.f, 0.f)
{
	//fx = 0.f;
	//fy= 0.f;
}


CAIBackBar::~CAIBackBar()
{
}

HRESULT CAIBackBar::Ready_GameObject(BACKBARTYPE eType, Engine::CTransform* pParentTransform, _vec3 vPos, _vec3 vSize)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_eBackBarType = eType;
	m_pParentTransformCom = pParentTransform;

	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransformCom->Set_Scale(vSize.x, vSize.y, vSize.z);

	return S_OK;
}

_int CAIBackBar::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CNormalObject::Update_GameObject(dTimeDelta);
	m_pTransformCom->Set_ParentMatrix(m_pParentTransformCom->Get_WorldMatrix()); //부모행렬->빌보드적용되어있음

	return Engine::NO_EVENT;
}

_int CAIBackBar::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CAIBackBar::Render_Geometry(const _double & dTimeDelta)
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
	
	pEffect->BeginPass(24);	

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);	

}

HRESULT CAIBackBar::Clone_Component()
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

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_AIBackBar"));
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

HRESULT CAIBackBar::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	m_pTextureCom->SetTexture(pEffect,"g_DiffuseTexture", m_eBackBarType);

	return S_OK;
}

CAIBackBar * CAIBackBar::Create(LPDIRECT3DDEVICE9 pGraphicDev,BACKBARTYPE eType, Engine::CTransform* pParentTransform, _vec3 vPos, _vec3 vSize)
{
	CAIBackBar* pInstance = new CAIBackBar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, pParentTransform, vPos, vSize)))
		Engine::Safe_Release(pInstance);
	
	return pInstance;
}

void CAIBackBar::Free()
{
	CNormalObject::Free();
}
