#include "stdafx.h"
#include "ToolGameEffectStaticMeshObject.h"


CToolGameEffectStaticMeshObject::CToolGameEffectStaticMeshObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CToolGameObject(pGraphicDev),
	m_pStaticMeshCom(nullptr)
{
}

CToolGameEffectStaticMeshObject::~CToolGameEffectStaticMeshObject()
{
}

HRESULT CToolGameEffectStaticMeshObject::Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale)
{
	if (FAILED(Clone_Component(wstrObjectKey)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	return S_OK;
}

_int CToolGameEffectStaticMeshObject::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::Update_GameObject(dTimeDelta);

	if (!g_bIsPlay)
	{
		m_fFrame = 0.f;
		m_pTransformCom->Set_Info(&m_vOrginPos, Engine::INFO_POS);
		m_pTransformCom->Set_Angle(&m_vOrginRot);
		m_pTransformCom->Set_Scale(&m_vOrginScale);
		return Engine::NO_EVENT;
	}
	m_fFrame += dTimeDelta;

	if (g_dTimeBegin > m_fDeadTime)
	{
		//m_pTransformCom->Set_Info(&m_vOrginPos, Engine::INFO_POS);
		//m_pTransformCom->Set_Angle(&m_vOrginRot);
		//m_pTransformCom->Set_Scale(&m_vOrginScale);
		g_bIsPlay = false;
	}

	m_vDir = m_pTransformCom->m_vInfo[Engine::INFO_LOOK];
	m_pTransformCom->Set_Info(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + (m_vDir*dTimeDelta * m_fSpeed)), Engine::INFO_POS);
	m_pTransformCom->Set_Angle(&(m_pTransformCom->m_vAngle + (m_vMoveRot*dTimeDelta)));
	m_pTransformCom->Set_Scale(&(m_pTransformCom->m_vScale + (m_vMoveScale*dTimeDelta)));


	return Engine::NO_EVENT;
}

_int CToolGameEffectStaticMeshObject::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CToolGameObject::LateUpdate_GameObject(dTimeDelta);

	if (m_iAlpha == 0)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	}
	else if (m_iAlpha == 1)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	}
	return Engine::NO_EVENT;
}

void CToolGameEffectStaticMeshObject::Render_Geometry(const _double& dTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();

	_uint iPassMax = 0;
	for (_ulong i = 0; i < dwSubset; ++i)
	{
		if (FAILED(pEffect->SetTexture("g_DiffuseTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_DIFFUSE, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_NormalTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_NORMAL, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_SpecularTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_SPECULAR, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_EmmisiveTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_EMMISIVE, i))))
			return;
		pEffect->CommitChanges();
		pEffect->Begin(&iPassMax, 0);
		if (m_iAlpha == 0)
		{
			pEffect->BeginPass(5);
		}
		else if (m_iAlpha == 1)
		{
			pEffect->BeginPass(0);
		}
		m_pStaticMeshCom->Render_Meshes(i);

		pEffect->EndPass();
		pEffect->End();
	}

	//텍스쳐 갯수만큼 랜더 한번 더 돌린다.
	for (auto J = 0; J < m_vecTextureInfo.size(); ++J)
	{
		for (_ulong i = 0; i < dwSubset; ++i)
		{
			m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_vecTextureInfo[J].iTextureName);
			//pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
			//pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
			//pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
			pEffect->CommitChanges();

			// UV 업데이트
			m_vecTextureInfo[J].dbFrame += dTimeDelta;
			if (m_vecTextureInfo[J].dbFrame >= m_vecTextureInfo[J].dbMaxFrame)
			{
				m_vecTextureInfo[J].dbFrame = 0.;
			}
			m_vecTextureInfo[J].vUVMovePosSum += m_vecTextureInfo[J].vUVMovePos * dTimeDelta;
			_float	fU = ((_uint)m_vecTextureInfo[J].dbFrame % (_uint)(m_vecTextureInfo[J].vUVAtlas.x)) / (m_vecTextureInfo[J].vUVAtlas.x);
			_float	fV = ((_uint)m_vecTextureInfo[J].dbFrame / (_uint)(m_vecTextureInfo[J].vUVAtlas.x)) / (m_vecTextureInfo[J].vUVAtlas.y);

			pEffect->SetVector("g_vUV_U", &_vec4(fU, 1 / m_vecTextureInfo[J].vUVAtlas.x, m_vecTextureInfo[J].vUVMovePosSum.x, 0.f));
			pEffect->SetVector("g_vUV_V", &_vec4(fV, 1 / m_vecTextureInfo[J].vUVAtlas.y, m_vecTextureInfo[J].vUVMovePosSum.y, 0.f));
			pEffect->Begin(&iPassMax, 0);
			pEffect->BeginPass(m_vecTextureInfo[J].iPass);
			m_pStaticMeshCom->Render_Meshes(i);
			pEffect->EndPass();
			pEffect->End();
		}
	}
	pEffect->SetVector("g_vUV_U", &_vec4(0.f, 1.f, 0.f, 0.f));
	pEffect->SetVector("g_vUV_V", &_vec4(0.f, 1.f, 0.f, 0.f));

	Engine::Safe_Release(pEffect);

	m_pGraphicDev->SetTexture(0, nullptr);
}

HRESULT CToolGameEffectStaticMeshObject::Clone_Component(wstring wstrObjectKey)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrObjectKey));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	//Renderer
	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	//Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_EffectALL"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	return S_OK;
}

HRESULT CToolGameEffectStaticMeshObject::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CToolGameObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

CToolGameEffectStaticMeshObject * CToolGameEffectStaticMeshObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale)
{
	CToolGameEffectStaticMeshObject* pInstance = new CToolGameEffectStaticMeshObject(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(wstrObjectKey, pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CToolGameEffectStaticMeshObject::Free(void)
{
	CToolGameObject::Free();
}
