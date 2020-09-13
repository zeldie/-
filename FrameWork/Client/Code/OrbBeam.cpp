#include "stdafx.h"
#include "OrbBeam.h"


COrbBeam::COrbBeam(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev),
	m_fCreateEffectPosY(0.f),
	m_bCreateEffect(false),
	m_dCountDown(3),
	m_eControlType(CTRL_END)
{
}


COrbBeam::~COrbBeam()
{
}

HRESULT COrbBeam::Ready_GameObject(_vec3 * pPos, _float fCreateEffectPosY, CONTROLTYPE eControlType)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Scale(3.f, 3.f, 3.f);
	m_fCreateEffectPosY = fCreateEffectPosY;

	//// 충돌구체 붙이기
	//Engine::CComponent*		pFirstCol = nullptr;
	//Engine::CComponent*		pAttCol = nullptr;

	//pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 300.f);
	//pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 300.f);

	//m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
	//m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));
	//BASE_INFO tBase;
	//if (eControlType == CTRL_PLAYER)
	//	tBase.eObjectID = OBJECT_PLAYER;
	//else if (eControlType == CTRL_AI_ALLIANCE)
	//	tBase.eObjectID = OBJECT_ALLIANCE;
	//else if (eControlType == CTRL_AI_ENERMY_1)
	//	tBase.eObjectID = OBJECT_ENEMY_1;
	//else if (eControlType == CTRL_AI_ENERMY_2)
	//	tBase.eObjectID = OBJECT_ENEMY_2;
	//tBase.eCollisionID = COLLISIONID::COLLISION_HIT;
	//tBase.eEffectID = EFFECTID::EFFECT_ORB_BEAM;
	//tBase.fEffectHitTime = 0.5f;
	//tBase.iAtt = 1000;
	//m_tBaseInfo = tBase;
	
	m_tBaseInfo.iCheatATK = 0;

	m_eControlType = eControlType;
	return S_OK;
}

_int COrbBeam::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	m_dCountDown -= dTimeDelta;
	if (m_dCountDown < 0)
		m_bIsDead = true;
	Move(dTimeDelta);
	CStaticMeshObject::Update_GameObject(dTimeDelta);
	Compute_ViewZ(m_pTransformCom->Get_Info(Engine::INFO_POS));
	return Engine::NO_EVENT;
}

_int COrbBeam::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_HDR_ALPHA, this);
	//if (m_vecCollSphere[0].size() >= 1)
	//{
	//	if (m_eControlType == CTRL_PLAYER || m_eControlType == CTRL_AI_ALLIANCE)
	//		m_pCollisionMgr->Add_Collision(COLLLIST::COLL_PLAYER_EFFECT, this);
	//	else
	//		m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER_EFFECT, this);
	//}
	return Engine::NO_EVENT;
}

void COrbBeam::Render_Geometry(const _double & dTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();
	_uint iRandomNumber = Engine::RandomNumber(2);
	if(iRandomNumber == 0)
		Engine::SetTexture(pEffect, "g_MaskingTexture", 22);
	else
		Engine::SetTexture(pEffect, "g_MaskingTexture", 23);
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
		pEffect->BeginPass(22);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
		
	}
	pEffect->End();

	Engine::Safe_Release(pEffect);
	CStaticMeshObject::Render_Geometry(dTimeDelta);
}

HRESULT COrbBeam::Clone_Component()
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

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_Orb_Beam"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT COrbBeam::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

void COrbBeam::Move(const _double & dTimeDelta)
{
	m_pTransformCom->Move_Pos(&_vec3(0.f, -1.f, 0.f), 2000, dTimeDelta);
	_float fCurrentPosY = m_pTransformCom->Get_Info(Engine::INFO_POS)->y;
	if (m_fCreateEffectPosY > fCurrentPosY && !m_bCreateEffect)
	{
		m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE, m_pTransformCom->Get_Info(Engine::INFO_POS), &_vec3(0.f, 0.f, 0.f), nullptr, m_eControlType);
		m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE2, m_pTransformCom->Get_Info(Engine::INFO_POS), &_vec3(0.f, 0.f, 0.f), nullptr, m_eControlType);
		m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE3, m_pTransformCom->Get_Info(Engine::INFO_POS), &_vec3(0.f, 0.f, 0.f), nullptr, m_eControlType);
		m_bCreateEffect = true;
	}
}

COrbBeam * COrbBeam::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _float fCreateEffectPosY, CONTROLTYPE eControlType)
{
	COrbBeam* pInstance = new COrbBeam(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, fCreateEffectPosY, eControlType)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void COrbBeam::Free()
{
	CStaticMeshObject::Free();
}
