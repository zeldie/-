#include "stdafx.h"
#include "Belatos_Arrow.h"

CBelatos_Arrow::CBelatos_Arrow(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev),
	m_eArrowType(ARROW_END),
	m_fStopPos(0.f),
	m_iPass(0),
	m_bIsDissolve(false),
	m_dDissolveCountDown(0),
	m_bCreateEffect(false),
	m_eControlType(CTRL_END)
{
	ZeroMemory(&m_vDir, sizeof(_vec3));
}

CBelatos_Arrow::~CBelatos_Arrow()
{
}

void CBelatos_Arrow::Move(const _double& dTimeDelta)
{
	if (m_eArrowType == BASIC_ARROW)
	{
		m_pTransformCom->Move_Pos(&m_vDir, 2500.f, dTimeDelta);
	}
	else if(m_eArrowType == RAIN_ARROW)
	{
		_float fCurrentY = m_pTransformCom->Get_Info(Engine::INFO_POS)->y;
		if(m_fStopPos <= fCurrentY)
			m_pTransformCom->Move_Pos(&m_vDir, 5000.f, dTimeDelta);
		else
		{
			if (!m_bCreateEffect)
			{
				m_pEffectMgr->Create_TextureEffect(TEXTURE_RAINARROWHIT, &(*m_pTransformCom->Get_Info(Engine::INFO_POS) + _vec3(0.f, 20.f, 0.f)));
				m_bCreateEffect = true;
				m_bIsDissolve = true;
			}
		}
	}
	else
	{

	}
	
}

HRESULT CBelatos_Arrow::Ready_GameObject(ARROW_TYPE eArrowType, _vec3 * pPos, _vec3* pAngle, _float fStopPos, _vec3 vTargetPos, CONTROLTYPE eControlType)
{
	if (FAILED(Clone_Component(eArrowType)))
		return E_FAIL;
	m_eArrowType = eArrowType;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);

	// 충돌구체 붙이기
	Engine::CComponent*		pFirstCol = nullptr;
	Engine::CComponent*		pAttCol = nullptr;

	pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 25);
	pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 25);

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

	if (m_eArrowType == BASIC_ARROW)
	{
		m_pTransformCom->Set_Angle(pAngle);
		m_pTransformCom->Set_Scale(0.3f, 0.3f, 0.3f);
		m_vDir = Engine::GetDir(vTargetPos, *pPos);
		_vec3 vLook = _vec3(-1.f, 0.f, 0.f);
		D3DXVec3Normalize(&vLook, &vLook);
		_float fRotAngleY = Engine::GetAngleY(vLook, -m_vDir);
		m_pTransformCom->Rotation(Engine::ROT_Y, fRotAngleY);
		m_iPass = 15;
		m_pEffectMgr->Create_BelatosArrowAura(Engine::EFFECTTYPE::EFFECT_BELATOS_ARROW, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo, m_pTransformCom);

		tBase.eCollisionID = COLLISIONID::COLLISION_HIT;
		tBase.eEffectID = EFFECTID::EFFECT_BELATOS_ARROW;
		tBase.fEffectHitTime = 0.2f;
		tBase.iAtt = 1000;
		tBase.iCritical = 4;
	}
	else if (m_eArrowType == RAIN_ARROW)
	{
		_vec3 vRandomAngle = _vec3(Engine::RandomFloatRange(-20.f, 20.f), Engine::RandomFloatRange(-20.f, 20.f), Engine::RandomFloatRange(-20.f, 20.f));
		m_pTransformCom->Set_Angle(&vRandomAngle);
		m_fStopPos = fStopPos;
		m_pTransformCom->Set_Scale(0.4f, 0.4f, 0.4f);
		m_vDir = _vec3(0.f, -1.f, 0.f);

		tBase.eCollisionID = COLLISIONID::COLLISION_HIT;
		tBase.eEffectID = EFFECTID::EFFECT_ARROW;
		tBase.fEffectHitTime = 0.1f;
		tBase.iAtt = 700;
		tBase.iCritical = 4;
		m_iPass = 0;
	}
	else
	{
		m_pTransformCom->Set_Angle(pAngle);
		m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_vDir = Engine::GetDir(vTargetPos, *pPos);
		_vec3 vLook = _vec3(-1.f, 0.f, 0.f);
		D3DXVec3Normalize(&vLook, &vLook);
		_float fRotAngleY = Engine::GetAngleY(vLook, -m_vDir);
		m_pTransformCom->Rotation(Engine::ROT_Y, fRotAngleY);
		m_iPass = 15;
	}

	m_eControlType = eControlType;
	m_tBaseInfo = tBase;

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
	if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE::UITYPE_CARTEL)
	{
		if (m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].size() != 0)
			CAiMgr::GetInstance()->Add_Effect(m_tBaseInfo.eObjectID, m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].front()->Get_TransformCom());
	}
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Ai %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//


	m_fLifeTime = 3.f;

	return S_OK;
}

_int CBelatos_Arrow::Update_GameObject(const _double & dTimeDelta)
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

	Move(dTimeDelta);
	CStaticMeshObject::Update_GameObject(dTimeDelta);

	m_fLifeTime -= dTimeDelta;

	return Engine::NO_EVENT;
}

_int CBelatos_Arrow::LateUpdate_GameObject(const _double & dTimeDelta)
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

	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);
	if(m_eArrowType == BASIC_ARROW)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	else if(m_eArrowType == RAIN_ARROW)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	if (m_vecCollSphere[0].size() >= 1)
	{
		if(m_eControlType == CTRL_PLAYER || m_eControlType == CTRL_AI_ALLIANCE)
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_PLAYER_EFFECT, this);
		else
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER_EFFECT, this);
	}
	
	if (m_eArrowType == BASIC_ARROW)
	{
		if (m_fLifeTime < 0)
			m_bIsDead = true;
	}

	return Engine::NO_EVENT;
}

void CBelatos_Arrow::Render_Geometry(const _double & dTimeDelta)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
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
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	CStaticMeshObject::Render_Geometry(dTimeDelta);
}

HRESULT CBelatos_Arrow::Clone_Component(ARROW_TYPE eArrowType)
{
	Engine::CComponent* pComponent = nullptr;
	wstring wstrArrowID = L"";
	if (eArrowType == BASIC_ARROW)
		wstrArrowID = L"Mesh_Arrow_Aura";
	else if (eArrowType == RAIN_ARROW)
		wstrArrowID = L"Mesh_RainArrow";
	else
		wstrArrowID = L"Mesh_StormArrow";

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrArrowID));
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

HRESULT CBelatos_Arrow::Setup_ShaderProps(LPD3DXEFFECT & pEffect, const _double& dTimeDelta)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	if (m_eArrowType == RAIN_ARROW)
	{
		pEffect->SetVector("vChangeColor", &_vec4(3.f, 3.f, 3.f, 1.f));
		if (m_bIsDissolve)
		{
			Engine::SetTexture(pEffect, "g_DissolveTexture", 6);
			m_dDissolveCountDown += dTimeDelta;
			pEffect->SetFloat("fTime", m_dDissolveCountDown);
			m_iPass = 14;
			if (m_dDissolveCountDown > 1.f)
				m_bIsDead = true;
		}
	}
	else if (m_eArrowType == BASIC_ARROW)
	{
		pEffect->SetVector("vMaskingColor", &_vec4(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eArrowType == STORM_ARROW)
	{
		pEffect->SetVector("vMaskingColor", &_vec4(1.f, 1.f, 1.f, 1.f));
	}
	return S_OK;
}

CBelatos_Arrow * CBelatos_Arrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, ARROW_TYPE eArrowType, _vec3 * pPos, _vec3* pAngle, _float fStopPos, _vec3 vTargetPos, CONTROLTYPE eControlType)
{
	CBelatos_Arrow* pInstance = new CBelatos_Arrow(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eArrowType, pPos,pAngle, fStopPos, vTargetPos, eControlType)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBelatos_Arrow::Free()
{
	CStaticMeshObject::Free();
}
