#include "stdafx.h"
#include "Belatos_Missile.h"

CBelatos_Missile::CBelatos_Missile(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev),
	m_iPass(0),
	m_fLifeTime(0.f),
	m_pParentBoneMatrix(nullptr),
	m_pParentWorldMatrix(nullptr)
{
	ZeroMemory(&m_vDir, sizeof(_vec3));
}

CBelatos_Missile::~CBelatos_Missile()
{
}

void CBelatos_Missile::Move(const _double& dTimeDelta)
{
	m_pTransformCom->Move_Pos(&m_vDir, 1000.f, dTimeDelta);
	m_fLifeTime += dTimeDelta;
	if (m_fLifeTime > 3.f)
		m_bIsDead = true;
}

HRESULT CBelatos_Missile::Ready_GameObject(_vec3 * pPos, _vec3* pAngle, _vec3 vTargetPos)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh* pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Belatos", Engine::MESH, Engine::ID_STATIC));

		const Engine::D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName("Dum_spear");
		if (nullptr == pFrame)
			return Engine::ERROR_EXIT;

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		Engine::CTransform*			pBelatosTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Belatos", Engine::TRANSFORM, Engine::ID_DYNAMIC));
		if (nullptr == pBelatosTransformCom)
			return Engine::ERROR_EXIT;

		m_pParentWorldMatrix = pBelatosTransformCom->Get_WorldMatrix();
	}

	_matrix matBoneWorldPos = (*m_pParentBoneMatrix * (*m_pParentWorldMatrix));

	
	_vec3 pMissilePos;
	memcpy(&pMissilePos, matBoneWorldPos.m[3], sizeof(_vec3));

	//pMissilePos = *pPos;
	//pMissilePos.y += 70.f;
	
	vTargetPos.y += 70;

	m_pTransformCom->Set_Info(&pMissilePos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(0.f, 0.f, 0.f);
	m_pTransformCom->Set_Scale(0.25f, 0.25f, 0.7f);
	m_vDir = Engine::GetDir(vTargetPos, pMissilePos);

	_vec3 vLook = _vec3(0.f, 0.f, -1.f);

	D3DXVec3Normalize(&vLook, &vLook);
	_float fRotAngleY = Engine::GetAngleY(vLook, -m_vDir);
	m_pTransformCom->Rotation(Engine::ROT_Y, fRotAngleY);
	m_iPass = 0;
	


	// 충돌구체 붙이기
	Engine::CComponent*		pFirstCol = nullptr;
	Engine::CComponent*		pAttCol = nullptr;

	pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 150);
	pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 150);

	m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
	m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));

	BASE_INFO tBase;
	tBase.eCollisionID = COLLISIONID::COLLISION_HIT;
	tBase.eEffectID = EFFECTID::EFFECT_BELATOS_MISSILE;
	tBase.fEffectHitTime = 0.35;
	tBase.iAtt = 1000;

	m_tBaseInfo = tBase;

	return S_OK;
}

_int CBelatos_Missile::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	if (m_vecCollSphere[0].size() >= 1)
		m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER_EFFECT, this);

	Move(dTimeDelta);
	CStaticMeshObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CBelatos_Missile::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return Engine::NO_EVENT;
}

void CBelatos_Missile::Render_Geometry(const _double & dTimeDelta)
{
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect, dTimeDelta)))
		return;
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

HRESULT CBelatos_Missile::Clone_Component()
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

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_Belatos_Missile"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CBelatos_Missile::Setup_ShaderProps(LPD3DXEFFECT & pEffect, const _double& dTimeDelta)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	//if (m_eArrowType == RAIN_ARROW)
	//{
	//	if (m_bIsDissolve)
	//	{
	//		Engine::SetTexture(pEffect, "g_DissolveTexture", 6);
	//		m_fDissolveCountDown += dTimeDelta;
	//		pEffect->SetFloat("fTime", m_fDissolveCountDown);
	//		m_iPass = 14;
	//		if (m_fDissolveCountDown > 3.f)
	//			m_bIsDead = true;
	//	}
	//}
	//else
	//{
	//	pEffect->SetVector("vMaskingColor", &_vec4(0.f, 1.f, 0.f, 1.f));
	//	pEffect->SetValue("vMaskingColor", &_vec3(1.f, 1.f, 1.f), sizeof(_vec3));
	//}
	return S_OK;
}

CBelatos_Missile * CBelatos_Missile::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3* pAngle, _vec3 vTargetPos)
{
	CBelatos_Missile* pInstance = new CBelatos_Missile(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, vTargetPos)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBelatos_Missile::Free()
{
	CStaticMeshObject::Free();
}
