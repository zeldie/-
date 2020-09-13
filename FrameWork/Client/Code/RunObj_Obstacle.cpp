#include "stdafx.h"
#include "RunObj_Obstacle.h"


CRunObj_Obstacle::CRunObj_Obstacle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev)
	, m_eSect(SECT_1)
	, m_eLineType(LINE_BOTTOM_MID)
	, m_eObjCollType(OBJ_COLL_END)
	, m_dProgress(0.)
	, m_dRadius(0.)
	, m_bFly(false)
	, m_dDeadTiem(0.)
	, m_dDissolveCountDown(0)
	, m_iPass(0)
	, m_bDissolveStart(false)
{
	ZeroMemory(&m_vDir, sizeof(_vec3));
}


CRunObj_Obstacle::~CRunObj_Obstacle()
{
}

HRESULT CRunObj_Obstacle::Ready_GameObject(wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _vec3* vSelfRotate, RUN_LINE_SECT eSect,
	LINETYPE eLineType, RUNGAMEOBJECT_COLL eCollType, _double dProgress, _double dRadius)
{
	if (FAILED(Clone_Component(wstrObjectKey)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_SelfRotAngle(vSelfRotate);

	m_eSect = eSect;
	m_eLineType = eLineType;
	m_eObjCollType = eCollType;
	m_dProgress = dProgress;
	m_dRadius = dRadius;

	return S_OK;
}

_int CRunObj_Obstacle::Update_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::Update_GameObject(dTimeDelta);

	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	
	if (m_bFly)
	{
		if(m_dDeadTiem <= g_dAccumulatedTime)
			return Engine::OBJ_DEAD;

		m_pTransformCom->Move_Pos(&m_vDir, 1.0, dTimeDelta);
		m_pTransformCom->Rotation(Engine::ROT_X, 25.f);
		m_pTransformCom->Rotation(Engine::ROT_Z, 20.f);
	}

	return Engine::NO_EVENT;
}

_int CRunObj_Obstacle::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);

	_matrix matRotateX, matRotateY, matRotateZ, matRotate;
	D3DXMatrixIdentity(&matRotateX);
	D3DXMatrixIdentity(&matRotateY);
	D3DXMatrixIdentity(&matRotateZ);
	D3DXMatrixIdentity(&matRotate);
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(m_pTransformCom->Get_SelfRotAngle()->x));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(m_pTransformCom->Get_SelfRotAngle()->y));
	D3DXMatrixRotationZ(&matRotateZ, D3DXToRadian(m_pTransformCom->Get_SelfRotAngle()->z));
	matRotate = matRotateY * matRotateX * matRotateZ;
	matRotate = matRotate * (*m_pTransformCom->Get_WorldMatrix());
	m_pTransformCom->Set_Matrix(&matRotate);
	if(Culling())
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CRunObj_Obstacle::Render_Depth(const _double & dTimeDelta)
{
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();
	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	for (_ulong i = 0; i < dwSubset; ++i)
	{
		pEffect->BeginPass(9);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CRunObj_Obstacle::Render_Geometry(const _double & dTimeDelta)
{
	
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	if (m_bDissolveStart)
	{
		Engine::SetTexture(pEffect, "g_DissolveTexture", 6);
		Engine::SetTexture(pEffect, "g_DissolveEdgeTexture", 2);
		m_dDissolveCountDown += dTimeDelta * 0.5;
		pEffect->SetFloat("fTime", m_dDissolveCountDown);
		m_iPass = 14;
		if (m_dDissolveCountDown > 1)
			m_bIsDead = true;
	}

	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();
	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
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
		pEffect->BeginPass(m_iPass);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}


void CRunObj_Obstacle::Render_Shadow(const _double & dTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();

	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);

	for (_ulong i = 0; i < dwSubset; ++i)
	{
		pEffect->BeginPass(6);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

HRESULT CRunObj_Obstacle::Clone_Component(wstring wstrObjectKey)
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

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrObjectKey));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	//Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone(Engine::RESOURCE_STATIC, L"OptimizationCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::OPTIMIZATION, pComponent);

	return S_OK;
}

HRESULT CRunObj_Obstacle::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

void CRunObj_Obstacle::Set_Collision(RUNPLAYER_POWER eColl)
{
	// ÀÌÆåÆ®
	_vec3 vUp = m_pTransformCom->m_vInfo[Engine::INFO_UP];
	D3DXVec3Normalize(&vUp, &vUp);
	vUp *= 20.f;
	m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_RUNGAME_HIT, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + vUp), &m_pTransformCom->m_vAngle);

	if (eColl == RUN_POWER_NORMAL)
	{
		m_bDissolveStart = true;
	}

	if (eColl == RUN_POWER_BIG)
	{
		if (m_bFly)
			return;

		m_dDeadTiem = g_dAccumulatedTime + 2.0;
		m_bFly = true;

		_matrix matWorld = m_pTransformCom->m_matWorld;
		_matrix matTempLook;
		_matrix matTempRight;
		D3DXMatrixRotationAxis(&matTempLook, &m_pTransformCom->m_vInfo[Engine::INFO_LOOK], D3DXToRadian((_float)(rand() % 90 - 45)));
		D3DXMatrixRotationAxis(&matTempRight, &m_pTransformCom->m_vInfo[Engine::INFO_RIGHT], D3DXToRadian((_float)(rand() % 45 + 45)));
		matWorld *= matTempLook *matTempRight;
		D3DXVec3Normalize(&m_vDir, (_vec3*)&(matWorld.m[1][0]));

		m_vDir *= 1000.f;
	}
}

CRunObj_Obstacle * CRunObj_Obstacle::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _vec3* vSelfRotate, RUN_LINE_SECT eSect,
	LINETYPE eLineType, RUNGAMEOBJECT_COLL eCollType, _double dProgress, _double dRadius)
{
	CRunObj_Obstacle* pInstance = new CRunObj_Obstacle(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(wstrObjectKey, pPos, pAngle, pScale, vSelfRotate, eSect, eLineType, eCollType, dProgress, dRadius)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunObj_Obstacle::Free()
{
	CStaticMeshObject::Free();
}
