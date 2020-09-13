#include "stdafx.h"
#include "FateShot.h"

CFateShot::CFateShot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev),
	m_eSkillType(TYPE_END),
	m_fStopPos(0.f),
	m_iPass(0),
	m_bCreateEffect(false),
	m_dbDeadTime(0.0),
	m_pMeshCom(nullptr)
{
	ZeroMemory(&m_vDir, sizeof(_vec3));
}

CFateShot::~CFateShot()
{
}

void CFateShot::Move(const _double & dTimeDelta)
{
	//if (m_eSkillType == TYPE_MOVE)
	//{
	//	m_pTransformCom->Move_Pos(&m_vDir, 3000.f, dTimeDelta);
	//}
	//else if (m_eSkillType == TYPE_HOLD)
	//{
	//	int i = 0;

	//	/*else
	//	{
	//		if (!m_bCreateEffect)
	//		{
	//			m_pEffectMgr->Create_TextureEffect(TEXTURE_RAINARROWHIT, &(*m_pTransformCom->Get_Info(Engine::INFO_POS) + _vec3(0.f, 20.f, 0.f)));
	//			m_bCreateEffect = true;
	//		}
	//	}*/
	//}
}

HRESULT CFateShot::Ready_GameObject(SKILL_TYPE eSkillType, _vec3 * pPos, _vec3 * pAngle, _float fStopPos, _vec3 vTargetPos, OBJID eID)
{
	if (FAILED(Clone_Component(eSkillType)))
		return E_FAIL;
	m_eSkillType = eSkillType;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);

	Engine::CSphereCollider* pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
	Engine::CSphereCollider* pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
	m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
	m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);


	if (m_eSkillType == TYPE_MOVE)
	{		
		m_pTransformCom->Set_Angle(pAngle);
		m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_vDir = Engine::GetDir(vTargetPos, *pPos);

		//m_vDir = vTargetPos;
		_vec3 vLook = _vec3(-1.f, 0.f, 0.f);
		D3DXVec3Normalize(&vLook, &vLook);
		_float fRotAngleY = Engine::GetAngleY(vLook, -m_vDir);
		m_pTransformCom->Rotation(Engine::ROT_Y, pAngle->y);
		m_pTransformCom->Rotation(Engine::ROT_X, pAngle->x);
		m_pTransformCom->Rotation(Engine::ROT_Z, pAngle->z);
		m_iPass = 3;
		/*m_pEffectMgr->Create_ArrowAura(Engine::EFFECTTYPE::EFFECT_ARROW_AURA, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo, m_pTransformCom);*/

	}

	else if (m_eSkillType == TYPE_HOLD)
	{
		_vec3 vRandomAngle = _vec3(Engine::RandomFloatRange(-20.f, 20.f), Engine::RandomFloatRange(-20.f, 20.f), Engine::RandomFloatRange(-20.f, 20.f));
		m_pTransformCom->Set_Angle(&vRandomAngle);
		m_fStopPos = fStopPos;
		m_pTransformCom->Set_Scale(0.4f, 0.4f, 0.4f);
		m_vDir = _vec3(0.f, -1.f, 0.f);
	}
	else
	{
		m_pTransformCom->Set_Angle(pAngle);
		m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_vDir = vTargetPos;//Engine::GetDir(vTargetPos, *pPos);
		_vec3 vLook = _vec3(-1.f, 0.f, 0.f);
		D3DXVec3Normalize(&vLook, &vLook);
		_float fRotAngleY = Engine::GetAngleY(vLook, -m_vDir);
		m_pTransformCom->Rotation(Engine::ROT_Y, pAngle->y);
		m_pTransformCom->Rotation(Engine::ROT_X, pAngle->x);
		m_pTransformCom->Rotation(Engine::ROT_Z, pAngle->z);
		m_iPass = 3;
	}

	m_tBaseInfo.eCollisionID = COLLISIONID::COLLISION_HIT;
	m_tBaseInfo.fEffectHitTime = 1.f;
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

_int CFateShot::Update_GameObject(const _double & dTimeDelta)
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
	
	if (m_eSkillType == TYPE_MOVE)
		m_pTransformCom->Move_Pos(&m_vDir, 700.0, dTimeDelta);

	CCollisionMgr::GetInstance()->Add_Collision(COLL_PLAYER, this);

	for (_uint i = SPHERE_FIRST; i < SPHERE_END; ++i)
	{
		for (auto& iter : m_vecCollSphere[i])
			iter->Update_Component(dTimeDelta);
	}
	m_dbDeadTime += dTimeDelta;
	m_pTransformCom->Rotation(Engine::ROT_Y, -1720.f*(_float)dTimeDelta);
	

	CStaticMeshObject::Update_GameObject(dTimeDelta);
	
	if (m_eSkillType == TYPE_END)
	{
		if (1.0 <= m_dbDeadTime)
			m_bIsDead = true;
	}
	else
	{
		if (5.0 <= m_dbDeadTime)
			m_bIsDead = true;
	}
	return Engine::NO_EVENT;
}

_int CFateShot::LateUpdate_GameObject(const _double & dTimeDelta)
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
	/*if (m_bIsDissolve)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	else*/
	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_HDR_ALPHA, this);
	return Engine::NO_EVENT;
}

void CFateShot::Render_Geometry(const _double & dTimeDelta)
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
	//
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect, dTimeDelta)))
		return;
	dwSubset = m_pMeshCom->Get_SubsetNum();
	iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	for (_ulong i = 0; i < dwSubset; ++i)
	{
		if (FAILED(pEffect->SetTexture("g_DiffuseTexture", m_pMeshCom->Get_Texture(Engine::MESHTEXTURE_DIFFUSE, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_NormalTexture", m_pMeshCom->Get_Texture(Engine::MESHTEXTURE_NORMAL, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_SpecularTexture", m_pMeshCom->Get_Texture(Engine::MESHTEXTURE_SPECULAR, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_EmmisiveTexture", m_pMeshCom->Get_Texture(Engine::MESHTEXTURE_EMMISIVE, i))))
			return;
		pEffect->BeginPass(3);
		pEffect->CommitChanges();
		m_pMeshCom->Render_Meshes(i);
		pEffect->EndPass();
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);

	CBaseObject::Render_Geometry(dTimeDelta);
}

HRESULT CFateShot::Clone_Component(SKILL_TYPE eSkillType)
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

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_DS_FateShot1"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_DS_FateShot2"));
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

HRESULT CFateShot::Setup_ShaderProps(LPD3DXEFFECT & pEffect, const _double & dTimeDelta)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

CFateShot * CFateShot::Create(LPDIRECT3DDEVICE9 pGraphicDev, SKILL_TYPE eSkillType, _vec3 * pPos, _vec3 * pAngle, _float fStopPos, _vec3 vTargetPos, OBJID eID)
{
	CFateShot* pInstance = new CFateShot(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eSkillType, pPos, pAngle, fStopPos, vTargetPos, eID)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFateShot::Free()
{
	CStaticMeshObject::Free();
	Engine::Safe_Release(m_pMeshCom);
}
