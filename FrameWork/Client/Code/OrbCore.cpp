#include "stdafx.h"
#include "OrbCore.h"


COrbCore::COrbCore(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseObject(pGraphicDev),
	m_eOrbType(ORB_END),
	m_pOrbCenterTrail(nullptr),
	m_dAccTime(0.),
	m_pOrbWing(nullptr),
	m_dCreateEffectTime(0.),
	m_dCountDown(0.),
	m_eControlType(CTRL_END)
{
	ZeroMemory(&m_vDir, sizeof(_vec3));
	D3DXMatrixIdentity(&m_matParent);
}


COrbCore::~COrbCore()
{
}

HRESULT COrbCore::Ready_GameObject(ORBTYPE eOrbType, _vec3 * pPos, _vec3* pAngle, _vec3 * pDir, _double dCountDown, CONTROLTYPE eControlType)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Pos(pPos);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(0.3f, 0.3f, 0.3f);
	m_vDir = *pDir;
	m_eOrbType = eOrbType;
	// 충돌구체 붙이기
	Engine::CComponent*		pFirstCol = nullptr;
	Engine::CComponent*		pAttCol = nullptr;

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
	tBase.eEffectID = EFFECTID::EFFECT_ORB;
	tBase.iAtt = 1200;
	tBase.iCritical = 4;
	m_tBaseInfo.iCheatATK = 0;
	switch (m_eOrbType)
	{
	case COrbCore::ORB_NORMAL:
		m_pEffectMgr->Create_Orb(TEXTURE_ORB3, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORB1, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORB2, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 150);
		pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 150);
		tBase.fEffectHitTime = 1.f;
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));
		break;
	case COrbCore::ORB_SPIN_FRONT_RIGHT:
		m_pEffectMgr->Create_Orb(TEXTURE_ORB3, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORB1, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORB2, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 200);
		pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 200);
		tBase.fEffectHitTime = 1.f;
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));
		break;
	case COrbCore::ORB_SPIN_FRONT_LEFT:
		m_pEffectMgr->Create_Orb(TEXTURE_ORB3, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORB1, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORB2, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 200);
		pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 200);
		tBase.fEffectHitTime = 1.f;
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));
		break;
	case COrbCore::ORB_SPIN_UP_RIGHT:
		m_pEffectMgr->Create_Orb(TEXTURE_ORB3, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORB1, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORB2, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 500);
		pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 500);
		tBase.fEffectHitTime = 0.5f;
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));
		break;
	case COrbCore::ORB_SPIN_UP_LEFT:
		m_pEffectMgr->Create_Orb(TEXTURE_ORB3, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORB1, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORB2, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 500);
		pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 500);
		tBase.fEffectHitTime = 0.5f;
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));
		break;
	case COrbCore::ORB_BIG:
		m_pEffectMgr->Create_Orb(TEXTURE_ORBBIG3, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORBBIG1, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		m_pEffectMgr->Create_Orb(TEXTURE_ORBBIG2, &_vec3(0.f, 0.f, 0.f), m_pTransformCom);
		pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 500);
		pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 500);
		tBase.fEffectHitTime = 0.3f;
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));
		break;
	case COrbCore::ORB_END:
		break;
	default:
		break;
	}
	m_tBaseInfo = tBase;
	//센터 트레일
	m_pOrbCenterTrail = COrbTrail::Create(m_pGraphicDev, COrbTrail::TRAIL_CENTER, m_pTransformCom);

	////오브 날개
	//m_pOrbWing = COrbWing::Create(m_pGraphicDev, pPos, pAngle, m_pTransformCom);
	//Engine::Add_GameObject(Engine::GAMEOBJECT, L"Orb_Wing", m_pOrbWing);

	if (eOrbType == ORB_SPIN_UP_RIGHT || eOrbType == ORB_SPIN_UP_LEFT || eOrbType == ORB_SPIN_FRONT_RIGHT || eOrbType == ORB_SPIN_FRONT_LEFT)
	{
		//중심 강제로 정해줌

		m_matParent._41 = pPos->x;
		m_matParent._42 = pPos->y;
		m_matParent._43 = pPos->z;
		m_pTransformCom->Set_RevParentMatrix(&m_matParent);
	}
	m_dCountDown = dCountDown;
	m_eControlType = eControlType;

	
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
	if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE::UITYPE_CARTEL)
	{
		if (m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].size() != 0)
			CAiMgr::GetInstance()->Add_Effect(m_tBaseInfo.eObjectID, m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].front()->Get_TransformCom());
	}
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

	return S_OK;
}

_int COrbCore::Update_GameObject(const _double & dTimeDelta)
{
	if (m_dCountDown < 0)
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

	// yh test code
	m_tBaseInfo.vObjectPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	// yh test end

	m_dCreateEffectTime += dTimeDelta;
	m_dCountDown -= dTimeDelta;
	Move(dTimeDelta);
	CBaseObject::Update_GameObject(dTimeDelta);
	if (m_dCreateEffectTime > 0.5)
	{
		if(m_eOrbType == ORB_BIG)
			m_pEffectMgr->Create_Orb(TEXTURE_ORB_ULT_SWIRL, m_pTransformCom->Get_Info(Engine::INFO_POS), m_pTransformCom);
		m_dCreateEffectTime = 0;
	}
	return Engine::NO_EVENT;
}

_int COrbCore::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_dCountDown < 0)
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

	CBaseObject::LateUpdate_GameObject(dTimeDelta);
	_vec3 vPos, vRight;
	vPos = m_pTransformCom->Get_Info_byWorld(Engine::INFO_POS);
	vRight = m_pTransformCom->Get_Info_byWorld(Engine::INFO_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);
	_vec3 vUp = vPos + vRight * 30.f;
	_vec3 vDown = vPos - vRight * 30.f;
	m_pOrbCenterTrail->Update_GameObject(dTimeDelta, vUp, vDown);
	m_pOrbCenterTrail->LateUpdate_GameObject(dTimeDelta);
	if (m_vecCollSphere[0].size() >= 1)
	{
		if (m_eControlType == CTRL_PLAYER || m_eControlType == CTRL_AI_ALLIANCE)
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_PLAYER_EFFECT, this);
		else
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER_EFFECT, this);
	}
	return Engine::NO_EVENT;
}

void COrbCore::Render_Geometry(const _double & dTimeDelta)
{
	CBaseObject::Render_Geometry(dTimeDelta);
}

HRESULT COrbCore::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	return S_OK;
}

void COrbCore::Move(const _double & dTimeDelta)
{
	switch (m_eOrbType)
	{
	case COrbCore::ORB_NORMAL:
		m_pTransformCom->Move_Pos(&m_vDir, 1500.f, dTimeDelta);
		break;
	case COrbCore::ORB_SPIN_FRONT_RIGHT:
		m_dAccTime += dTimeDelta * 10;
		m_pTransformCom->Revolution_XZ(m_dAccTime, dTimeDelta, true);
		m_pTransformCom->Move_Parent(&m_vDir, 1000, dTimeDelta);
		m_pTransformCom->Set_Pos(20.f, 0.f, 20.f);
		break;
	case COrbCore::ORB_SPIN_FRONT_LEFT:
		m_dAccTime += dTimeDelta * 10;
		m_pTransformCom->Revolution_XZ(m_dAccTime, dTimeDelta, false);
		m_pTransformCom->Move_Parent(&m_vDir, 1000, dTimeDelta);
		m_pTransformCom->Set_Pos(20.f, 0.f, 20.f);
		break;
	case COrbCore::ORB_SPIN_UP_RIGHT:
		m_pTransformCom->Move_Parent(&_vec3(0.f, 1.f, 0.f), 100, dTimeDelta);
		m_pTransformCom->Set_Pos(120.f, 0.f, 120.f);
		m_pTransformCom->Revolution(Engine::ROT_Y, 10 * dTimeDelta);
		break;
	case COrbCore::ORB_SPIN_UP_LEFT:
		m_pTransformCom->Move_Parent(&_vec3(0.f, 1.f, 0.f), 100, dTimeDelta);
		m_pTransformCom->Set_Pos(120.f, 0.f, 120.f);
		m_pTransformCom->Revolution(Engine::ROT_Y, -10 * dTimeDelta);
		break;
	case COrbCore::ORB_BIG:
		m_pTransformCom->Move_Pos(&m_vDir, 400.f, dTimeDelta);
		break;
	case COrbCore::ORB_END:
		break;
	default:
		break;
	}
}

COrbCore * COrbCore::Create(LPDIRECT3DDEVICE9 pGraphicDev, ORBTYPE eOrbType, _vec3 * pPos, _vec3* pAngle, _vec3 * pDir, _double dCountDown, CONTROLTYPE eControlType)
{
	COrbCore* pInstance = new COrbCore(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eOrbType, pPos, pAngle, pDir, dCountDown, eControlType)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void COrbCore::Free()
{
	Engine::Safe_Release(m_pOrbCenterTrail);
	//Engine::Safe_Release(m_pOrbWing);
	CBaseObject::Free();
}
