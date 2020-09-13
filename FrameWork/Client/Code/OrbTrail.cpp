#include "stdafx.h"
#include "OrbTrail.h"
#include "DynamicMesh.h"
#include "DynamicBuffer.h"
COrbTrail::COrbTrail(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseObject(pGraphicDev),
	m_pDynamicBufferCom(nullptr),
	m_pTextureCom(nullptr),
	m_AddTrailTime(0),
	m_eTrailType(TRAIL_END),
	m_iPass(0)
{
}


COrbTrail::~COrbTrail()
{
}

HRESULT COrbTrail::Ready_GameObject(TRAIL_TYPE eTrailType, Engine::CTransform* pParentTransformCom)
{
	if (FAILED(Clone_Component(eTrailType)))
		return E_FAIL;
	m_eTrailType = eTrailType;
	_vec3 vUpPos = *pParentTransformCom->Get_Info(Engine::INFO_POS);
	_vec3 vUpDown = *pParentTransformCom->Get_Info(Engine::INFO_POS);
	for (_uint i = 0; i < 20; ++i)
	{
		m_listPos[0].push_back(vUpPos);
		m_listPos[1].push_back(vUpDown);
	}
	switch (m_eTrailType)
	{
	case COrbTrail::TRAIL_CENTER:
		m_iPass = 1;
		break;
	case COrbTrail::TRAIL_WING:
		m_iPass = 19;
		break;
	case COrbTrail::TRAIL_END:
		break;
	default:
		break;
	}
	return S_OK;
}

_int COrbTrail::Update_GameObject(const _double& dTimeDelta, _vec3 pUpPos, _vec3 pDownPos)
{
	m_AddTrailTime += dTimeDelta;
	if (m_AddTrailTime > 0.03)
	{
		m_listPos[0].pop_front();
		m_listPos[1].pop_front();
		m_listPos[0].push_back(pUpPos);
		m_listPos[1].push_back(pDownPos);

		m_AddTrailTime = 0.;
	}
	m_pDynamicBufferCom->Update_Buffer(m_listPos);
	CBaseObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int COrbTrail::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CBaseObject::LateUpdate_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_HDR_ALPHA, this);
	return Engine::NO_EVENT;
}

void COrbTrail::Render_Geometry(const _double & dTimeDelta)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(m_iPass);

	m_pDynamicBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT COrbTrail::Clone_Component(TRAIL_TYPE eTrailType)
{
	Engine::CComponent*		pComponent = nullptr;

	wstring wstrTrailID = L"";
	switch (eTrailType)
	{
	case COrbTrail::TRAIL_CENTER:
		wstrTrailID = L"Texture_CenterTrail";
		break;
	case COrbTrail::TRAIL_WING:
		wstrTrailID = L"Texture_WingTrail";
		break;
	case COrbTrail::TRAIL_END:
		break;
	default:
		break;
	}
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	//Buffer
	pComponent = m_pDynamicBufferCom = dynamic_cast<Engine::CDynamicBuffer*>(Engine::CDynamicBuffer::Create(m_pGraphicDev, 20));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrTrailID));
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

HRESULT COrbTrail::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");
	switch (m_eTrailType)
	{
	case COrbTrail::TRAIL_CENTER:
		break;
	case COrbTrail::TRAIL_WING:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 23);
		break;
	case COrbTrail::TRAIL_END:
		break;
	default:
		break;
	}
	return S_OK;
}

COrbTrail * COrbTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, TRAIL_TYPE eTrailType, Engine::CTransform* pParentTransformCom)
{
	COrbTrail*	pInstance = new COrbTrail(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eTrailType, pParentTransformCom)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void COrbTrail::Free()
{
	CBaseObject::Free();
}
