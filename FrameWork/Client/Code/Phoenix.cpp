#include "stdafx.h"
#include "Phoenix.h"


CPhoenix::CPhoenix(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_eAnimation(PHOENIX_UP),
	m_eControlType(CONTROLTYPE::CTRL_END),
	m_dCountDown(10)
{
	ZeroMemory(&m_vDir, sizeof(_vec3));
}


CPhoenix::~CPhoenix()
{
}

HRESULT CPhoenix::Ready_GameObject(_vec3 * pPos, _vec3* pDir, CONTROLTYPE eControlType)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Scale(0.7f, 0.7f, 0.7f);
	m_pDynamicMeshCom->Set_AnimationSet(m_eAnimation);
	_vec3 vLook = _vec3(0.f, 0.f, 1.f);
	D3DXVec3Normalize(&vLook, &vLook);
	m_vDir = *pDir;
	_float fRotAngleY = Engine::GetAngleY(vLook, m_vDir);
	m_pTransformCom->Rotation(Engine::ROT_Y, fRotAngleY);

	// 충돌구체 붙이기
	Engine::CComponent*		pFirstCol = nullptr;
	Engine::CComponent*		pAttCol = nullptr;

	pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 500.f);
	pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 500.f);

	m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
	m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));

	BASE_INFO tBase;
	if (eControlType == CTRL_PLAYER)
		tBase.eObjectID = OBJECT_PLAYER;
	else if (eControlType == CTRL_AI_ALLIANCE)
		tBase.eObjectID = OBJECT_ALLIANCE;
	else if (eControlType == CTRL_AI_ENERMY_1)
		tBase.eObjectID = OBJECT_ENEMY_1;
	else if (eControlType == CTRL_AI_ENERMY_2)
		tBase.eObjectID = OBJECT_ENEMY_2;
	tBase.eCollisionID = COLLISIONID::COLLISION_HIT;
	tBase.eEffectID = EFFECTID::EFFECT_PHOENIX;
	tBase.fEffectHitTime = 10.f;
	tBase.iAtt = 2000;
	tBase.iCritical = 4;

	m_tBaseInfo = tBase;
	m_eControlType = eControlType;
	m_tBaseInfo.iCheatATK = 0;
	return S_OK;
}

_int CPhoenix::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::NO_EVENT;
	
	// 맞은 적에게 내 위치 알려주는 코드
	if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE::UITYPE_CARTEL)
	{
		switch (m_tBaseInfo.eObjectID)
		{
		case OBJID::OBJECT_PLAYER:
			m_tBaseInfo.vObjectPos = CAiMgr::GetInstance()->Get_Player()->GetPlayerPos();
		case OBJID::OBJECT_ALLIANCE:
			m_tBaseInfo.vObjectPos = CAiMgr::GetInstance()->Get_Player()->GetPlayerPos();
		case OBJID::OBJECT_ENEMY_1:
			m_tBaseInfo.vObjectPos = CAiMgr::GetInstance()->Get_Player()->GetPlayerPos();
		case OBJID::OBJECT_ENEMY_2:
			m_tBaseInfo.vObjectPos = CAiMgr::GetInstance()->Get_Player()->GetPlayerPos();
		}
	}

	if (m_eAnimation == PHOENIX_MOVE)
		m_pTransformCom->Move_Pos(&m_vDir, 400.f, dTimeDelta);
	m_dCountDown -= dTimeDelta;
	if (m_dCountDown < 0)
		m_bIsDead = true;
	CDynamicMeshObject::Update_GameObject(dTimeDelta);
	if (m_eAnimation == PHOENIX_UP)
	{
		if (m_pDynamicMeshCom->Is_AnimationSetFinish(0.15))
		{
			m_eAnimation = PHOENIX_MOVE;
			m_pDynamicMeshCom->Set_AnimationSet(m_eAnimation);
		}
			
	}
	return Engine::NO_EVENT;
}

_int CPhoenix::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::NO_EVENT;
	
	// 맞은 적에게 내 위치 알려주는 코드
	if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE::UITYPE_CARTEL)
	{
		switch (m_tBaseInfo.eObjectID)
		{
		case OBJID::OBJECT_PLAYER:
			m_tBaseInfo.vObjectPos = CAiMgr::GetInstance()->Get_Player()->GetPlayerPos();
		case OBJID::OBJECT_ALLIANCE:
			m_tBaseInfo.vObjectPos = CAiMgr::GetInstance()->Get_Player()->GetPlayerPos();
		case OBJID::OBJECT_ENEMY_1:
			m_tBaseInfo.vObjectPos = CAiMgr::GetInstance()->Get_Player()->GetPlayerPos();
		case OBJID::OBJECT_ENEMY_2:
			m_tBaseInfo.vObjectPos = CAiMgr::GetInstance()->Get_Player()->GetPlayerPos();
		}
	}

	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);
	if (m_vecCollSphere[0].size() >= 1)
	{
		if (m_eControlType == CTRL_PLAYER || m_eControlType == CTRL_AI_ALLIANCE)
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_PLAYER_EFFECT, this);
		else
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER_EFFECT, this);
	}
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CPhoenix::Render_Geometry(const _double & dTimeDelta)
{
	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta);

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
			pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
			pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
			pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
			pEffect->BeginPass(18);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);

	CDynamicMeshObject::Render_Geometry(dTimeDelta);
}

void CPhoenix::Render_Shadow(const _double & dTimeDelta)
{
	////Shader
	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//if (pEffect == nullptr)
	//	return;
	//Engine::Safe_AddRef(pEffect);
	//if (FAILED(Setup_ShaderProps(pEffect)))
	//	return;
	////if (FAILED(CBaseObject::Set_ShaderShadow(pEffect)))
	////	return;
	//_uint iPassMax = 0;
	//pEffect->Begin(&iPassMax, 0);
	//list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	//for (auto& iter : *plistMeshContainer)
	//{
	//	_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
	//	m_pDynamicMeshCom->Render_Meshes_Begin(iter);
	//	for (_ulong i = 0; i < dwSubsetNum; ++i)
	//	{
	//		pEffect->BeginPass(6);
	//		pEffect->CommitChanges();
	//		m_pDynamicMeshCom->Render_Meshes(iter, i);
	//		pEffect->EndPass();
	//	}
	//	m_pDynamicMeshCom->Render_Meshes_End(iter);
	//}
	//pEffect->End();

	//Engine::Safe_Release(pEffect);
}

HRESULT CPhoenix::Clone_Component()
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

	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_Phoenix"));
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

HRESULT CPhoenix::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	pEffect->SetVector("vDyeColor", &_vec4(0.4f, 5.f, 0.4f, 1.f));
	return S_OK;
}

CPhoenix * CPhoenix::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3* pDir, CONTROLTYPE eControlType)
{
	CPhoenix* pInstance = new CPhoenix(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pDir, eControlType)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPhoenix::Free()
{
	CDynamicMeshObject::Free();
}
