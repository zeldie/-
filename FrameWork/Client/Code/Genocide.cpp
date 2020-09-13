#include "stdafx.h"
#include "Genocide.h"

CGenocide::CGenocide(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev),
	m_iPass(0),
	m_dbEffectCreateTime(0.0),
	m_dbKillingTime(0.0)
{
		ZeroMemory(&m_vDir, sizeof(_vec3));
}

CGenocide::~CGenocide()
{
}

HRESULT CGenocide::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 vTargetPos, OBJID eID)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);

	Engine::CSphereCollider* pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
	Engine::CSphereCollider* pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
	m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
	m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_pTransformCom->Set_Angle(pAngle);
		m_pTransformCom->Set_Scale(1.0f, 1.f, 1.f);
		m_vDir = Engine::GetDir(vTargetPos, *pPos);

		//m_vDir = vTargetPos;
		_vec3 vLook = _vec3(-1.f, 0.f, 0.f);
		D3DXVec3Normalize(&vLook, &vLook);
		_float fRotAngleY = Engine::GetAngleY(vLook, -m_vDir);
		/*m_pTransformCom->Rotation(Engine::ROT_Y, pAngle->y);
		m_pTransformCom->Rotation(Engine::ROT_X, pAngle->x);
		m_pTransformCom->Rotation(Engine::ROT_Z, pAngle->z);*/
		m_iPass = 3;

	m_tBaseInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
	m_tBaseInfo.fEffectHitTime = 0.5f;
	m_tBaseInfo.iAtt = 2000;

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
	if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE::UITYPE_CARTEL)
	{
		if (m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].size() != 0)
			CAiMgr::GetInstance()->Add_Effect(m_tBaseInfo.eObjectID, m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].front()->Get_TransformCom());
	}
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

	m_tBaseInfo.eObjectID = eID;
	return S_OK;
}

_int CGenocide::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{
		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
		if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE::UITYPE_CARTEL)
		{
			if (m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].size() != 0)
				CAiMgr::GetInstance()->Delete_Effect(m_tBaseInfo.eObjectID, m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].front()->Get_TransformCom());
		}
		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
		return Engine::OBJ_DEAD;
	}

	m_pTransformCom->Move_Pos(&m_vDir, 350.0, dTimeDelta);

	CCollisionMgr::GetInstance()->Add_Collision(COLL_PLAYER, this);

	for (_uint i = SPHERE_FIRST; i < SPHERE_END; ++i)
	{
		for (auto& iter : m_vecCollSphere[i])
			iter->Update_Component(dTimeDelta);
	}
	m_dbEffectCreateTime += dTimeDelta;
	m_dbKillingTime += dTimeDelta;
	if (1.0 <= m_dbEffectCreateTime)
	{
		m_dbEffectCreateTime = 0.0;
		m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_GENICIDE, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	}
	
	m_pTransformCom->Rotation(Engine::ROT_Y, -360.f*(_float)dTimeDelta);

	if (10.0 <= m_dbKillingTime)
		m_bIsDead = true;

	CStaticMeshObject::Update_GameObject(dTimeDelta);

	//if (m_eSkillType == TYPE_END)
	//{
	//	if (1.0 <= m_dbDeadTime)
	//		m_bIsDead = true;
	//}
	//else
	//{
	//	if (5.0 <= m_dbDeadTime)
	//		m_bIsDead = true;
	//}
	return Engine::NO_EVENT;
}

_int CGenocide::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{
		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
		if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE::UITYPE_CARTEL)
		{
			if (m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].size() != 0)
				CAiMgr::GetInstance()->Delete_Effect(m_tBaseInfo.eObjectID, m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].front()->Get_TransformCom());
		}
		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
		return Engine::OBJ_DEAD;
	}
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_HDR_ALPHA, this);
	return Engine::NO_EVENT;
}

void CGenocide::Render_Geometry(const _double & dTimeDelta)
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
		pEffect->BeginPass(3);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);

	CBaseObject::Render_Geometry(dTimeDelta);
}

HRESULT CGenocide::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;
	//wstring wstrArrowID = L"";
	//if (eSkillType == TYPE_MOVE)
	//	wstrArrowID = L"Mesh_BasicArrow";
	//else if (eSkillType == TYPE_HOLD)
	//	wstrArrowID = L"Mesh_RainArrow";
	//else
	//	wstrArrowID = L"Mesh_StormArrow";

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_DS_Genocide"));
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

HRESULT CGenocide::Setup_ShaderProps(LPD3DXEFFECT & pEffect, const _double & dTimeDelta)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;;
}

CGenocide * CGenocide::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 vTargetPos, OBJID eID)
{
	CGenocide* pInstance = new CGenocide(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, vTargetPos, eID)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CGenocide::Free()
{
	CStaticMeshObject::Free();
}
