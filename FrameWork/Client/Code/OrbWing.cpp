#include "stdafx.h"
#include "OrbWing.h"


COrbWing::COrbWing(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseObject(pGraphicDev),
	m_pOrbWingTrail(nullptr),
	m_dAccTime(0),
	m_pCoreTransformCom(nullptr),
	m_pBufferCom(nullptr),
	m_pTextureCom(nullptr)
{
}


COrbWing::~COrbWing()
{
}

HRESULT COrbWing::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, Engine::CTransform* pCore)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pCoreTransformCom = pCore;
	m_pTransformCom->Set_Pos(0.3f, 0.3f, 0.f);
	//m_pTransformCom->Set_Info(&(*pPos + _vec3(0.f, 10.f, 0.f)), Engine::INFO_POS);
	//m_pTransformCom->Set_Revolution(100.f, 100.f, 100.f);
	m_pTransformCom->Set_Scale(2.f, 2.f, 1.f);
	m_pTransformCom->Set_RevParentMatrix(m_pCoreTransformCom->Get_WorldMatrix());

	m_pOrbWingTrail = COrbTrail::Create(m_pGraphicDev, COrbTrail::TRAIL_WING, m_pTransformCom);

	return S_OK;
}

_int COrbWing::Update_GameObject(const _double & dTimeDelta)
{

	m_pTransformCom->Revolution(Engine::ROT_Z, 100 * dTimeDelta);
	CBaseObject::Update_GameObject(dTimeDelta);
	Compute_ViewZ(m_pTransformCom->Get_Info(Engine::INFO_POS));
	return Engine::NO_EVENT;
}

_int COrbWing::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseObject::LateUpdate_GameObject(dTimeDelta);
	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	_vec3 vPos, vRight;
	memcpy(&vPos, &matWorld.m[3][0], sizeof(_vec3));
	memcpy(&vRight, &matWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vRight, &vRight);
	_vec3 vUp = vPos + vRight * 20.f;
	_vec3 vDown = vPos - vRight * 20.f;
	m_pOrbWingTrail->Update_GameObject(dTimeDelta, vUp, vDown);
	m_pOrbWingTrail->LateUpdate_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	return Engine::NO_EVENT;
}

void COrbWing::Render_Geometry(const _double & dTimeDelta)
{
	/*m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
	return;
	Engine::Safe_AddRef(pEffect);


	CBaseObject::Set_ShaderMatrix(pEffect);
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");
	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);*/
}

HRESULT COrbWing::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_CenterTrail"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);
	return S_OK;
}

COrbWing * COrbWing::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, Engine::CTransform* pCore)
{
	COrbWing* pInstance = new COrbWing(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pCore)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void COrbWing::Free()
{
	Engine::Safe_Release(m_pOrbWingTrail);
	CBaseObject::Free();
}
