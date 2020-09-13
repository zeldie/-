#include "stdafx.h"
#include "DSBullet.h"

CDsBullet::CDsBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev),
	m_iPass(0),
	m_dbEffectCreateTime(0.0),
	m_dbKillingTime(0.0),
	m_bRight(false)
{
	ZeroMemory(&m_vDir, sizeof(_vec3));
}

CDsBullet::~CDsBullet()
{
}

HRESULT CDsBullet::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 vTargetPos, Engine::EFFECTTYPE EffectType, OBJID eID, _bool Right)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_bRight = Right;
	
	Engine::CComponent*		pCol_First = nullptr;
	Engine::CComponent*		pCol_Att = nullptr;

	pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
	pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
	m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pCol_First));
	m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pCol_Att));

	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(5.f, 5.f, 5.f);
	m_vDir = Engine::GetDir(vTargetPos, *pPos);
	m_vAngle = *pAngle;
	m_eType = EffectType;
	//m_vDir = vTargetPos;
	_vec3 vLook = _vec3(-1.f, 0.f, 0.f);
	D3DXVec3Normalize(&vLook, &vLook);
	//m_pTransformCom->Rotation(Engine::ROT_Y, 90.f);
	_float fRotAngleY = Engine::GetAngleY(vLook, -m_vDir);
	m_pTransformCom->Rotation(Engine::ROT_Y, pAngle->y);
	m_pTransformCom->Rotation(Engine::ROT_X, pAngle->x);
	m_pTransformCom->Rotation(Engine::ROT_Z, pAngle->z);
	m_iPass = 3;
	//m_pTransformCom->Rotation(Engine::ROT_X, 45.f);
	//m_pTransformCom->Rotation(Engine::ROT_Y, 45.f);

	m_tBaseInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
	m_tBaseInfo.fEffectHitTime = 0.5f;
	m_tBaseInfo.iAtt = 850;

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

_int CDsBullet::Update_GameObject(const _double & dTimeDelta)
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
	
	m_dbEffectCreateTime += dTimeDelta;
	m_dbKillingTime += dTimeDelta;
	if (0.25 <= m_dbEffectCreateTime)
	{
		m_dbEffectCreateTime = 0.0;
		if(m_bRight)
			m_pEffectMgr->Create_Effect(m_eType, m_pTransformCom->Get_WorldMatrix(), &m_vAngle, m_tBaseInfo);
		else
			m_pEffectMgr->Create_Effect(m_eType, m_pTransformCom->Get_WorldMatrix(), &m_vAngle, m_tBaseInfo);
	}

	//m_pTransformCom->Rotation(Engine::ROT_Y, -1360.f*(_float)dTimeDelta);

	if (10.0 <= m_dbKillingTime)
		m_bIsDead = true;

	CStaticMeshObject::Update_GameObject(dTimeDelta);

	return Engine::NO_EVENT;
}

_int CDsBullet::LateUpdate_GameObject(const _double & dTimeDelta)
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
	if (m_vecCollSphere[0].size() >= 1)
		CCollisionMgr::GetInstance()->Add_Collision(COLL_PLAYER_EFFECT, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	return Engine::NO_EVENT;
}

void CDsBullet::Render_Geometry(const _double & dTimeDelta)
{
	CStaticMeshObject::Render_Geometry(dTimeDelta);
}

HRESULT CDsBullet::Clone_Component()
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

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_DS_Bullet"));
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

HRESULT CDsBullet::Setup_ShaderProps(LPD3DXEFFECT & pEffect, const _double & dTimeDelta)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;;
}

CDsBullet * CDsBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 vTargetPos, Engine::EFFECTTYPE EffectType, OBJID eID, _bool Right)
{
	CDsBullet* pInstance = new CDsBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, vTargetPos, EffectType,eID, Right)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDsBullet::Free()
{
	CStaticMeshObject::Free();
}
