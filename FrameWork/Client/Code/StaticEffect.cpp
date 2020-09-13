#include "stdafx.h"
#include "StaticEffect.h"

CStaticEffect::CStaticEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseEffect(pGraphicDev),
	m_pStaticMeshCom(nullptr),
	m_bwstrMask(false),
	m_pParentTransformCom(nullptr),
	m_pMeshShaderCom(nullptr),
	m_dDissolveCountDown(0),
	m_bUseHDR(true)
{
}


CStaticEffect::~CStaticEffect()
{
}

HRESULT CStaticEffect::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	return S_OK;
}

_int CStaticEffect::Update_GameObject(const _double & dTimeDelta)
{
	if (m_pParentTransformCom != nullptr)
	{
		if (m_tBaseInfo.eEffectID == EFFECT_ARROW_AURA)
		{
			m_pTransformCom->Set_Pos(m_pParentTransformCom->Get_Info(Engine::INFO_POS));
			m_pTransformCom->Set_Angle(m_pTransformCom->Get_Angle());
		}

	}
	if (m_pParentTransformCom != nullptr)
	{
		if (m_tBaseInfo.eEffectID == EFFECT_BELATOS_ARROW)
		{
			m_pTransformCom->Set_Pos(m_pParentTransformCom->Get_Info(Engine::INFO_POS));
			m_pTransformCom->Set_Angle(m_pTransformCom->Get_Angle());
		}

	}
	Transform(dTimeDelta);
	CBaseEffect::Update_GameObject(dTimeDelta);

	//프레임증가 + 최대프레임도달시 객체반환
	//if ((m_dFrame += (dTimeDelta * m_dMaxFrame * m_dLifeTime)) >= m_dMaxFrame)

	//m_fFrameSPeed = 내가  x초뒤에 죽는다면    1 / X = m_fFrameSpeed가 된다.
	if (m_bLoop) // 루프를 돈다 = 아틀라스 이미지가 아니거나 아틀라스 이미지인데 계속 돈다
	{
		m_dFrame += dTimeDelta * m_fFrameSpeed * m_dMaxFrame;
		//루프를 다 돌아도 아래 m_dbTime(죽는 시간이 되지 않으면 다시 0으로 돌아가서 계속 루프탐)
		if (m_dFrame > m_dMaxFrame)
			m_dFrame = 0;
		//죽는시간이 되면 죽음
		if ((m_dbTime += dTimeDelta * m_fFrameSpeed ) > m_tEffectInfo.fDeadTime)
		{
			// %%%%%%%%%%%%%% Ai %%%%%%%%%%%%% //
			if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_CARTEL)
			{
				if (m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].size() != 0)
					CAiMgr::GetInstance()->Delete_Effect(m_tBaseInfo.eObjectID, m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].front()->Get_TransformCom());
			}
			// %%%%%%%%%%%%%% Ai %%%%%%%%%%%%% //

			CBaseEffect::Delete_Collider();
			m_pEffectMgr->Return_StaticPool(this);
			m_pParentTransformCom = nullptr;

			return Engine::OBJ_POOL;
		}
	}
	else // 루프를 안돈다 = 아틀라스 이미지이면서 한바퀴만 돈다
	{
		m_dFrame += dTimeDelta * m_fFrameSpeed * m_dMaxFrame / m_tEffectInfo.fDeadTime;

		if ((m_dbTime += dTimeDelta * m_fFrameSpeed /*/ m_tEffectInfo.fDeadTime*/ /** m_dMaxFrame*/) >  m_tEffectInfo.fDeadTime)
		{
			// %%%%%%%%%%%%%% Ai %%%%%%%%%%%%% //
			if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_CARTEL)
			{
				if (m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].size() != 0)
					CAiMgr::GetInstance()->Delete_Effect(m_tBaseInfo.eObjectID, m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].front()->Get_TransformCom());
			}
			// %%%%%%%%%%%%%% Ai %%%%%%%%%%%%% //

			CBaseEffect::Delete_Collider();
			m_pEffectMgr->Return_StaticPool(this);
			m_pParentTransformCom = nullptr;

			return Engine::OBJ_POOL;
		}
	}
	//트레일이 아니면 MainOffsetSpeed = 0, 0
	//트레일이면 m_tEffectInfo.fDeadTime * 2.f = vMainOffSetSpeed
	m_tEffectProps.vMainUVOffset += dTimeDelta * _vec2(m_tEffectProps.vMainOffsetSpeed.x / m_tEffectProps.vMainUV_U.y, m_tEffectProps.vMainOffsetSpeed.y / m_tEffectProps.vMainUV_V.y);
	m_tEffectProps.vMask1UVOffset += dTimeDelta * _vec2(m_tEffectProps.vMask1OffsetSpeed.x / m_tEffectProps.vMask1UV_U.y, m_tEffectProps.vMask1OffsetSpeed.y / m_tEffectProps.vMask1UV_V.y);
	m_tEffectProps.vMask2UVOffset += dTimeDelta * _vec2(m_tEffectProps.vMask2OffsetSpeed.x / m_tEffectProps.vMask2UV_U.y, m_tEffectProps.vMask2OffsetSpeed.y / m_tEffectProps.vMask2UV_V.y);
	////cout << " OffSet : " <<m_tEffectProps.vMainUVOffset.x << endl;
	if (m_iPass == 4 || m_iPass==5) // 아틀라스
	{
		m_tEffectProps.vMainUV_U.x = ((_uint)m_dFrame % (_uint)(m_tEffectProps.vMainUV_U.y)) / (m_tEffectProps.vMainUV_U.y);
		m_tEffectProps.vMainUV_V.x = ((_uint)m_dFrame / (_uint)(m_tEffectProps.vMainUV_U.y)) / (m_tEffectProps.vMainUV_V.y);
	}
	// UV 흐르는값 설정
	m_tEffectProps.vUVMovePosSum.x += m_tEffectProps.vMainUV_U.z * dTimeDelta;
	m_tEffectProps.vUVMovePosSum.y += m_tEffectProps.vMainUV_V.z * dTimeDelta;
	m_tEffectProps.vUVMovePosSum2.x += m_tEffectProps.vMask1UV_U.z * dTimeDelta;
	m_tEffectProps.vUVMovePosSum2.y += m_tEffectProps.vMask1UV_V.z * dTimeDelta;
	m_tEffectProps.vUVMovePosSum3.x += m_tEffectProps.vMask2UV_U.z * dTimeDelta;
	m_tEffectProps.vUVMovePosSum3.y += m_tEffectProps.vMask2UV_V.z * dTimeDelta;

	m_vDir = m_pTransformCom->m_vInfo[Engine::INFO_LOOK];
	m_pTransformCom->Set_Info(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + (m_vDir*_float(dTimeDelta * m_tEffectInfo.fSpeed))), Engine::INFO_POS);
	m_pTransformCom->Rotation(Engine::ROT_X, _float(m_tEffectInfo.vMoveRot.x*dTimeDelta));
	m_pTransformCom->Rotation(Engine::ROT_Y, _float(m_tEffectInfo.vMoveRot.y*dTimeDelta));
	m_pTransformCom->Rotation(Engine::ROT_Z, _float(m_tEffectInfo.vMoveRot.z*dTimeDelta));
	m_pTransformCom->Set_Scale(&(m_pTransformCom->m_vScale + (m_tEffectInfo.vMoveScale*_float(dTimeDelta))));
	// 충돌구체 크기 키우는함수
	Set_ColliderScaling(dTimeDelta);


	if (m_tBaseInfo.eEffectID == EFFECT_APOSTLE_LASER1)
	{
		if (m_vecCollSphere[0].size() >= 1)
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER_EFFECT_CAPSULE, this);
	}
	else
	{
		if (m_tBaseInfo.eObjectID == OBJECT_PLAYER || m_tBaseInfo.eObjectID == OBJECT_ALLIANCE)
		{
			if (m_vecCollSphere[0].size() >= 1)
				m_pCollisionMgr->Add_Collision(COLLLIST::COLL_PLAYER_EFFECT, this);
		}
		else if (m_tBaseInfo.eObjectID == OBJECT_MONSTER || m_tBaseInfo.eObjectID == OBJECT_ENEMY_1 || m_tBaseInfo.eObjectID == OBJECT_ENEMY_2)
		{
			if (m_vecCollSphere[0].size() >= 1)
				m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER_EFFECT, this);
		}


	}

	
	return Engine::NO_EVENT;
}

_int CStaticEffect::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseEffect::LateUpdate_GameObject(dTimeDelta);

	if (EFFECT_TS_LB_PRESSING == m_tBaseInfo.eEffectID
		|| EFFECT_TS_LB_PRESSING_ULTIMATE == m_tBaseInfo.eEffectID) //희정 추가
	{
		m_pTransformCom->Rotation(Engine::ROT_Y, _float(dTimeDelta*900));
	}

	if (m_tEffectInfo.iAlpha == 0)
	{
		if (m_bUseHDR)
			m_pRendererCom->Add_RenderGroup(Engine::RENDER_HDR_ALPHA, this);
		else 
			m_pRendererCom->Add_RenderGroup(Engine::RENDER_HDR_ALPHA, this);
	}
	else if (m_tEffectInfo.iAlpha == 1)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	}
	Compute_ViewZ(m_pTransformCom->Get_Info(Engine::INFO_POS));
	return Engine::NO_EVENT;
}

void CStaticEffect::Render_Geometry(const _double & dTimeDelta)
{	
	LPD3DXEFFECT	pEffect = nullptr;

	if (Engine::EFFECT_TS_F_ROCK == m_tBaseInfo.eEffectID)
	{
		pEffect = m_pMeshShaderCom->Get_EffectHandle();
	}
	else
	{
		pEffect = m_pShaderCom->Get_EffectHandle();
	}

	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();

	// 여기에서 X파일 이름으로 마스킹 추가 , 단 하나의 이펙트가 메쉬 여러개를 만들면 아래 IF에서 추가
	switch (m_tBaseInfo.eEffectID)
	{
	case EFFECT_APOSTLE_SPHERE_1:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 10);
		pEffect->SetVector("vChangeEffectColor", &_vec4(1.f, 1.f, 1.f, 1.f));
		break;
	case EFFECT_APOSTLE_SPHERE_2:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 9);
		pEffect->SetVector("vChangeEffectColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
	case EFFECT_APOSTLE_SPHERE_3:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 10);
		pEffect->SetVector("vChangeEffectColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
	case EFFECT_APOSTLE_SPHERE_4:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 9);
		pEffect->SetVector("vChangeEffectColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
	case EFFECT_APOSTLE_SPHERE_5:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 9);
		pEffect->SetVector("vChangeEffectColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
	case EFFECT_APOSTLE_LASER1:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 6);
		pEffect->SetVector("vChangeEffectColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
	case EFFECT_APOSTLE_TRAIL1:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 6);
		pEffect->SetVector("vChangeEffectColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
	case EFFECT_APOSTLE_LIGHT:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 6);
		pEffect->SetVector("vChangeEffectColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
	case EFFECT_APOSTLE_LIGHT2:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 6);
		pEffect->SetVector("vChangeEffectColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
	case EFFECT_APOSTLE_LIGHT_DOT:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 16);
		pEffect->SetVector("vChangeEffectColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
	case EFFECT_APOSTLE_LIGHT2_DOT:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 16);
		pEffect->SetVector("vChangeEffectColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
	case EFFECT_APOSTLE_DOME_HALF:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 13);
		pEffect->SetVector("vChangeEffectColor", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
		break;
		// 영준
	case EFFECT_BELATOS_HEMISPHERE:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 22);
		break;
	case EFFECT_BELATOS_HEMISPHERE2:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 22);
		break;
	case EFFECT_BELATOS_HEMISPHERE3:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 22);
		break;
	case EFFECT_BELATOS_SHOCKWAVE:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 22);
		break;
	case EFFECT_BELATOS_SHOCKWAVE2:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 22);
		break;
	case EFFECT_BELATOS_SHOCKWAVE_LAY:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 22);
		break;
	case EFFECT_BELATOS_SHOCKWAVE_LAY2:
		Engine::SetTexture(pEffect, "g_AddTexture", 22);
		break;
	case EFFECT_BELATOS_SHOCKWAVE_LAY3:
		Engine::SetTexture(pEffect, "g_AddTexture", 22);
		break;
	case EFFECT_BELATOS_JUMPLIGHT:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 22);
		break;
		//인균
	case EFFECT_WIND_DUST:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 22);
		break;
	case EFFECT_DUST:
		//m_tEffectProps.vDiminishRate -= _vec4(dTimeDelta * 3.f, dTimeDelta* 3.f, dTimeDelta* 3.f, 0.f);
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 24);
		break;
	case EFFECT_BOW_RING:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 25);
		break;
	case EFFECT_PHOENIX_WIND1:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 26);
		break;
	case EFFECT_PHOENIX_WIND2:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 26);
		break;
	case EFFECT_PHOENIX_WIND3:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 26);
		break;
	case EFFECT_ORB_WIND:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 23);
		break;
	case EFFECT_FIREBALL_WIND:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 23);
		break;
	case EFFECT_ORB_WIND2:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 22);
		break;
	case EFFECT_ORB_BEAM:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 22);
		break;
	case EFFECT_ORB_ULT_WIND:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 23);
		break;
	case EFFECT_TS_F_ROCK:
	{
		if (m_dbTime > 1.5)
		{
			Engine::SetTexture(pEffect, "g_DissolveTexture", 6);
			Engine::SetTexture(pEffect, "g_DissolveEdgeTexture", 2);
			m_dDissolveCountDown += dTimeDelta;
			pEffect->SetFloat("fTime", m_dDissolveCountDown);
			m_iPass = 14;
		}
	}
		break;
	case EFFECT_TS_LAY:
		Engine::SetTexture(pEffect, "g_MaskingTexture1", 22);
		break;
	default:
		break;
	}
	// 이펙트 한번에 여러개의 메쉬를 만들면 여기에서 X파일 이름으로 마스킹 추가
	if (m_bwstrMask)
	{
		if (m_tEffectInfo.wstrName == L"메쉬의 X파일 이름")
		{
			Engine::SetTexture(pEffect, "g_MaskingTexture1", 6);
		}
	}

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
		if (m_iPass == 4 || m_iPass == 5)
		{
			pEffect->SetValue("g_vInvAtlas", &_vec2(1.f / m_tEffectProps.vMainUV_U.y, 1.f / m_tEffectProps.vMainUV_V.y), sizeof(_vec2));
		}
		pEffect->BeginPass(m_iPass);
		pEffect->CommitChanges();
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();

	}
	pEffect->End();
	pEffect->SetTexture("g_MaskingTexture1", nullptr);
	pEffect->SetTexture("g_MaskingTexture2", nullptr);

	Engine::Safe_Release(pEffect);

	m_pGraphicDev->SetTexture(0, nullptr);

	CBaseObject::Render_Geometry(dTimeDelta);
}

void CStaticEffect::Set_Info(EFFECT_INFO tEffectInfo, const _matrix* matWorld, _vec3 * pAngle, BASE_INFO tBaseInfo)
{
	m_tEffectInfo = tEffectInfo;
	Clone_StaticCom();

	m_tBaseInfo = tBaseInfo;
	m_dFrame = 0.;
	m_dbTime = 0.;

	_vec3 vRight, vUp, vLook, vMatPos, vPos, vNewPos;
	vPos = m_tEffectInfo.vPos;
	memcpy(&vRight, matWorld->m[0], sizeof(_vec3));
	memcpy(&vUp, matWorld->m[1], sizeof(_vec3));
	memcpy(&vLook, matWorld->m[2], sizeof(_vec3));
	memcpy(&vMatPos, matWorld->m[3], sizeof(_vec3));

	float	fXScale = D3DXVec3Length(&vRight);
	float	fYScale = D3DXVec3Length(&vUp);
	float	fZScale = D3DXVec3Length(&vLook);

	_matrix	MymatWorld, matScale, NewWorld;
	D3DXMatrixIdentity(&MymatWorld);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixScaling(&matScale, fXScale, fYScale, fZScale);
	D3DXMatrixInverse(&matScale, NULL, &matScale);
	MymatWorld = MymatWorld * matScale;
	NewWorld = MymatWorld * *matWorld;

	_vec3 vNewRight, vNewUp, vNewLook, vNewvPos;

	memcpy(&vNewRight, &NewWorld.m[0], sizeof(_vec3));
	memcpy(&vNewUp, &NewWorld.m[1], sizeof(_vec3));
	memcpy(&vNewLook, &NewWorld.m[2], sizeof(_vec3));
	memcpy(&vNewvPos, &NewWorld.m[3], sizeof(_vec3));

	vNewPos.x = (vNewRight.x * vPos.x) + (vNewUp.x * vPos.y) + (vNewLook.x * vPos.z) + (vNewvPos.x * 1.f);
	vNewPos.y = (vNewRight.y * vPos.x) + (vNewUp.y * vPos.y) + (vNewLook.y * vPos.z) + (vNewvPos.y * 1.f);
	vNewPos.z = (vNewRight.z * vPos.x) + (vNewUp.z * vPos.z) + (vNewLook.z * vPos.z) + (vNewvPos.z * 1.f);

	// 스케일값 뺀 위치적용
	m_pTransformCom->Set_Pos(&(vNewPos));

	////cout << "X: " << vNewPos.x << "Y: " << vNewPos.y << "Z: " << vNewPos.z << endl;

	// 회전
	_matrix	Mymat_Rotate, Mymat_RotateU;
	D3DXMatrixIdentity(&Mymat_Rotate);
	D3DXMatrixIdentity(&Mymat_RotateU);
	D3DXVec3Normalize(&vNewRight, &vNewRight);
	//D3DXVec3Normalize(&vNewUp, &vNewUp);
	D3DXMatrixRotationAxis(&Mymat_Rotate, &vNewRight, D3DXToRadian(m_tEffectInfo.vRot.x));
	//D3DXMatrixRotationAxis(&Mymat_RotateU, &vNewUp, D3DXToRadian(m_tEffectInfo.vRot.y));

	_vec2 vvRot;
	vvRot.x = acosf((Mymat_Rotate.m[1][1]));
	//vvRot.y = asinf((Mymat_RotateU.m[2][0]));
	vvRot.x = D3DXToDegree(vvRot.x);
	//vvRot.y = D3DXToDegree(vvRot.y);

	m_pTransformCom->Set_Angle(&(*pAngle));
	m_pTransformCom->Rotation(Engine::ROT_X, vvRot.x);
	//m_pTransformCom->Rotation(Engine::ROT_Y, vvRot.y);
	m_pTransformCom->Rotation(Engine::ROT_Y, m_tEffectInfo.vRot.y);
	m_bUseHDR = false;
	m_pTransformCom->Set_Scale(&(m_tEffectInfo.vScale));
	Add_Collider();
	m_dMaxFrame = m_tEffectProps.vMainUV_U.y * m_tEffectProps.vMainUV_V.y;
	m_pTransformCom->Update_Component(0.f);
	m_dDissolveCountDown = 0;
}


HRESULT CStaticEffect::Clone_Component()
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

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Effect"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	//Shader
	pComponent = m_pMeshShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER2, pComponent);

	//Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_EffectALL"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	return S_OK;
}

_vec3 CStaticEffect::Create_Angle()
{
	_vec3	vAngle;
	vAngle.x = ((rand() % 13) * 30.f) - 180.f;
	vAngle.y = ((rand() % 13) * 30.f) - 180.f;
	vAngle.z = ((rand() % 13) * 30.f) - 180.f;

	return vAngle;
}

HRESULT CStaticEffect::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	pEffect->SetValue("g_EffectProps", &m_tEffectProps, sizeof(EFFECT_PROPS));
	pEffect->SetVector("vChangeEffectColor", &_vec4(1.f, 1.f, 1.f, 1.f));
	return S_OK;
}

HRESULT CStaticEffect::Clone_StaticCom()
{
	if (m_pStaticMeshCom != nullptr)
		m_pStaticMeshCom->Release();
	m_mapComponent[Engine::ID_STATIC].erase(Engine::MESH);

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, m_tEffectInfo.wstrName));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	return S_OK;
}

HRESULT CStaticEffect::Add_Collider()
{
	CBaseEffect::Delete_Collider();
	Reset_EffectProps();
	// m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
	// m_bLoop == 아틀라스가 아니거나, 아틀라스여도 반복할거면 트루

	Engine::CSphereCollider* pCol_First = nullptr;
	Engine::CSphereCollider* pCol_Att = nullptr;

	HoEffect();

	if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Thrust")
	{ 
		Engine::CSphereCollider* pCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 5, "", nullptr, &_vec3(-2.f, 0.f, 0.f));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol);
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Sweep")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 300, "", nullptr, &_vec3(0.f, 0.f, 100.f));
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 300, "", nullptr, &_vec3(0.f, 0.f, 100.f));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Sweep2")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Arrow")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 4.f, 0.f);
		//m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_tEffectInfo.fDeadTime = 10.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 5;
	}
	//else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Missile")
	//{
	//	Engine::CSphereCollider* pCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 100);
	//	m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol);
	//	m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol);

	 
	//}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_ShockWave")
	{
		//m_tEffectProps.vMainUV_U = _vec3(0.f, 3.f, 3.4f);
		//m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 3.4f);

		//m_fFrameSpeed = 1.f;
		//m_iPass = 4;
		m_tEffectProps.vMainUV_U = _vec3(0.f, 3.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 0.f);

		m_fFrameSpeed = 1.f;
		m_iPass = 4;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_ShockWave2")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 2.f, 2.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 2.f, 2.f);

		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f); //쓸거면 0001 안쓸거면 1111

		m_fFrameSpeed = 1.f;
		m_iPass = 4;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_ShockWave_Lay")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 300, "", nullptr, &_vec3(0.f, 0.f, 0.f));
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 300, "", nullptr, &_vec3(0.f, 0.f, 0.f));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f); //쓸거면 0001 안쓸거면 1111

		m_fFrameSpeed = 1.f;
		m_iPass = 5;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_JumpLight")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 2.f, 5.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 5.f);

		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f); //쓸거면 0001 안쓸거면 1111

		m_fFrameSpeed = 1.f;
		m_iPass = 4;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Attack")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 120, "", nullptr, &_vec3(0.f, 200.f, 200.f));
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 120, "", nullptr, &_vec3(0.f, 200.f, 200.f));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Attack2")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_ShieldFront")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 30);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 30);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);
	
		m_tEffectProps.vMainUV_U = _vec3(0.f, 4.f, 2.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 2.f, 2.f);

		m_fFrameSpeed = 1.f;
		m_iPass = 4;
	}
	//else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Circle2")
	//{
	//	m_tEffectProps.vMainUV_U = _vec3(0.f, 4.f, 5.f);
	//	m_tEffectProps.vMainUV_V = _vec3(0.f, 2.f, 5.f);

	//	m_fFrameSpeed = 1.f;
	//	m_iPass = 4;
	//}
	//else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Circle3")
	//{
	//	m_tEffectProps.vMainUV_U = _vec3(0.f, 4.f, 5.f);
	//	m_tEffectProps.vMainUV_V = _vec3(0.f, 2.f, 5.f);

	//	m_fFrameSpeed = 1.f;
	//	m_iPass = 4;
	//}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Hemisphere")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 23);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 23);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.3f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.3f);
		
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f); //쓸거면 0001 안쓸거면 1111

		m_fFrameSpeed = 1.f;
		m_iPass = 5; 
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Hemisphere2")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 23);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 23);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 1.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 1.f);

		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f); //쓸거면 0001 안쓸거면 1111

		m_fFrameSpeed = 1.f;
		m_iPass = 5;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Hemisphere3")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, -0.3f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, -0.3f);

		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f); //쓸거면 0001 안쓸거면 1111

		m_fFrameSpeed = 1.f;
		m_iPass = 5;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos_Thrust2")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 50, "", nullptr, &_vec3(-2.f, 0.f, -50.f));
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 50, "", nullptr, &_vec3(-2.f, 0.f, -50.f));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 3.f, 3.4f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 3.4f);

		// 5번 일때만 사용
		//m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		//m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		//m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		//m_tEffectProps.vMainOffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		//m_tEffectProps.vMask1OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		//m_tEffectProps.vMask2OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 1.f;
		m_iPass = 4;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_Attack2")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 30);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 30);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);
	}
	// 시원 =========================================================================
	else if (m_tEffectInfo.wstrName == L"Mesh_Apostle_Sphere1")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 50);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 50);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMask1UV_U = _vec3(0.f, 1.f, 0.5f);
		m_tEffectProps.vMask1UV_V = _vec3(0.f, 1.f, -0.5f);

		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		//m_tEffectProps.vUseMask2 = _vec4(0.f, 0.f, 0.f, 1.f);

		m_fFrameSpeed = 1.f;
		m_iPass = 5;
		m_bLoop = true;
		m_bUseHDR = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Cylinder")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 50);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMask1UV_U = _vec3(0.f, 1.f, 0.5f);
		m_tEffectProps.vMask1UV_V = _vec3(0.f, 1.f, -0.5f);

		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		//m_tEffectProps.vUseMask2 = _vec4(0.f, 0.f, 0.f, 1.f);

		m_fFrameSpeed = 1.f;
		m_iPass = 5;
		m_bLoop = true;
		m_bUseHDR = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_ApostleLight")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 40);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 40);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMask1UV_U = _vec3(0.f, 1.f, 0.5f);
		m_tEffectProps.vMask1UV_V = _vec3(0.f, 1.f, -0.5f);

		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		//m_tEffectProps.vUseMask2 = _vec4(0.f, 0.f, 0.f, 1.f);

		m_fFrameSpeed = 1.f;
		m_iPass = 5;
		m_bLoop = true;
		m_bUseHDR = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_ApostleLight2")
	{
		//Engine::CSphereCollider* pCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 40);
		//m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol);
		//m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMask1UV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMask1UV_V = _vec3(0.f, 1.f, -0.5f);

		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		//m_tEffectProps.vUseMask2 = _vec4(0.f, 0.f, 0.f, 1.f);

		m_fFrameSpeed = 1.f;
		m_iPass = 5;
		m_bLoop = true;
		m_bUseHDR = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TrailTest")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 50);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 50);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_Att);
		//m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		//m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
		//m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		//m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		//m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		//m_tEffectProps.vMainOffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		//m_tEffectProps.vMask1OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		//m_tEffectProps.vMask2OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		//m_fFrameSpeed = 1.f;
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		//m_bwstrMask = true; 키값으로 마스크 추가할때 트루로 줘야한다.
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.5f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);


		//m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		//m_tEffectProps.vUseMask2 = _vec4(0.f, 0.f, 0.f, 1.f);


		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 1.f;
		m_iPass = 5;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_ApostleTrail")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		//m_tEffectProps.vMask1UV_U = _vec3(0.f, 1.f, 0.f);
		//m_tEffectProps.vMask1UV_V = _vec3(0.f, 1.f, 0.f);

		//m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(3.5f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(3.5f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(3.5f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 1.f;
		m_iPass = 3;
		m_bUseHDR = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_ApostleDomeHalf")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMask1UV_U = _vec3(0.f, 1.f, 0.2f);
		m_tEffectProps.vMask1UV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_fFrameSpeed = 1.f;
		m_iPass = 5;
		m_bUseHDR = false;
	}
	// 시원 =========================================================================
	//희정 =========================================================================
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB1") // EFFECT_TS_LB
	{ // UV 반대로 흘러가게함
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB2")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB3")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);
		
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);	

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB4") //EFFECT_TS_AIRLB_SECOND
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB5")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB6")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB7") //EFFECT_TS_LB_SECOND
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB8")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB9")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB10")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB11") //EFFECT_TS_LB_THIRD1
	{//반대로...
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB12")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB13")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
		m_bUseHDR = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB14")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_AIRLB1") //EFFECT_TS_AIRLB_THIRD
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_AIRLB2")
	{//반대로
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_AIRLB3")
	{//반대로
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_AIRLB4") //EFFECT_TS_AIRLB_FOURTH
	{//반대로
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 3.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_AIRLB5")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 3.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_AIRLB6")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 3.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB15") //EFFECT_TS_LB_PRESSING
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB16")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB17")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB18")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB19") //EFFECT_TS_LB_PRESSING2
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB20")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB21")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB22") //EFFECT_TS_LB_AIRUP
	{//반대로...
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB23")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 250);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 250);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB24")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LB25")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-4.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_E1") // EFFECT_TS_E_FIRST
	{ // UV 반대로 흘러가게함
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_E2")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_E3")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_E4") //EFFECT_TS_E_SECOND
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(-6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(-6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 4.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_E5")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 4.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_E6")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 350);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(6.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 4.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_F_Rock") //EFFECT_TS_F_ROCK
	{
		m_iPass = 0;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_LAY") //EFFECT_TS_LAY
	{
		//pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 300, "", nullptr, &_vec3(0.f, 0.f, 0.f));
		//pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 300, "", nullptr, &_vec3(0.f, 0.f, 0.f));
		//m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		//m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f); //쓸거면 0001 안쓸거면 1111
		m_fFrameSpeed = 1.f;
		m_iPass = 5;
	}
	//희정 =========================================================================
	//인균 =========================================================================
	else if (m_tEffectInfo.wstrName == L"Mesh_Wind_Dust")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 6.f, 0.f);
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 5;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Bow_Ring")
	{
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_tEffectInfo.fDeadTime = 0.2f;
		m_fFrameSpeed = 1.f;
		m_iPass = 3;
		m_bLoop = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Dust")
	{
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_tEffectInfo.fDeadTime = 0.5f;
		m_fFrameSpeed = 1.f;
		m_iPass = 5;
		m_bLoop = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Arrow_Aura")
	{
		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 4.f, 0.f);
		//m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_tEffectInfo.fDeadTime = 10.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 5;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Phoenix_Wind1")
	{
		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_bLoop = true;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Phoenix_Wind2")
	{
		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_bLoop = true;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Phoenix_Wind3")
	{
		m_tEffectProps.vMainUVOffset = _vec2(0.f, 1.f / m_tEffectProps.vMainUV_U.y);
		m_tEffectProps.vMainOffsetSpeed = _vec2(0.f, -2.f / m_tEffectInfo.fDeadTime);
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_bLoop = true;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Orb_Trail")
	{
		m_tEffectInfo.fDeadTime = 2.f;
		//m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainUVOffset = _vec2(0.f, -1.f / m_tEffectProps.vMainUV_V.y);
		//m_tEffectProps.vMainOffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(0.f, 2.f / m_tEffectInfo.fDeadTime);
		m_fFrameSpeed = 1.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Orb_Trail2")
	{
		m_tEffectInfo.fDeadTime = 2.f;
		//m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainUVOffset = _vec2(0.f, -1.f / m_tEffectProps.vMainUV_V.y);
		//m_tEffectProps.vMainOffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(0.f, 2.f / m_tEffectInfo.fDeadTime);
		m_fFrameSpeed = 1.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Orb_Wind")
	{
		m_tEffectProps.vMainUVOffset = _vec2(1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(-2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_fFrameSpeed = 1.f;
		m_iPass = 3;
		m_bLoop = true;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Fireball_Wind")
	{
		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_bLoop = true;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Orb_Wind2")
	{
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.5f);
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_fFrameSpeed = 3.f;
		m_bLoop = true;
		m_iPass = 5;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Orb_Beam")
	{
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_fFrameSpeed = 1.f;
		m_iPass = 5;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Orb_R_Pre")
	{
		m_fFrameSpeed = 1.f;
		m_iPass = 5;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Orb_Wind3")
	{
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 1.f);
		m_bLoop = true;
		m_fFrameSpeed = 5.f;
		m_iPass = 5;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Orb_Wind4")
	{
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 1.f);
		m_bLoop = true;
		m_fFrameSpeed = 3.f;
		m_iPass = 5;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Orb_Ult_Wind")
	{
		m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 5;
	}
	//인균 =========================================================================
	
	if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE::UITYPE_CARTEL)
	{
		if (m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].size() != 0)
			CAiMgr::GetInstance()->Add_Effect(m_tBaseInfo.eObjectID, m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].front()->Get_TransformCom());
	}

	return S_OK;
}

void CStaticEffect::Reset_EffectProps()
{
	m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
	m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
	m_tEffectProps.vMask1UV_U = _vec3(0.f, 1.f, 0.f);
	m_tEffectProps.vMask1UV_V = _vec3(0.f, 1.f, 0.f);
	m_tEffectProps.vMask2UV_U = _vec3(0.f, 1.f, 0.f);
	m_tEffectProps.vMask2UV_V = _vec3(0.f, 1.f, 0.f);
	m_tEffectProps.vUseMask1 = _vec4(1.f, 1.f, 1.f, 1.f); //쓸거면 0001 안쓸거면 1111
	m_tEffectProps.vUseMask2 = _vec4(1.f, 1.f, 1.f, 1.f);//쓸거면 0001 안쓸거면 1111
	m_tEffectProps.vUseDistortion = _vec4(1.f, 1.f, 1.f, 1.f);;//쓸거면 0001 안쓸거면 1111
	m_tEffectProps.vMainUVOffset = _vec2(0.f, 0.f); // 트레일 쓸거면 -1.f / m_tEffectProps.vMainUV_U.y 트레일 안쓰면 그대로 0,0
	m_tEffectProps.vMask1UVOffset = _vec2(0.f, 0.f);// 트레일 쓸거면 -1.f / m_tEffectProps.vMainUV_U.y 트레일 안쓰면 그대로 0,0
	m_tEffectProps.vMask2UVOffset = _vec2(0.f, 0.f);// 트레일 쓸거면 -1.f / m_tEffectProps.vMainUV_U.y 트레일 안쓰면 그대로 0,0
	m_tEffectProps.vMainOffsetSpeed = _vec2(0.f, 0.f);// 트레일 쓸거면 2.f / m_tEffectInfo.fDeadTime 트레일 안쓰면 그대로 0,0
	m_tEffectProps.vMask1OffsetSpeed = _vec2(0.f, 0.f);// 트레일 쓸거면 2.f / m_tEffectInfo.fDeadTime 트레일 안쓰면 그대로 0,0
	m_tEffectProps.vMask2OffsetSpeed = _vec2(0.f, 0.f);// 트레일 쓸거면 2.f / m_tEffectInfo.fDeadTime, 트레일 안쓰면 그대로 0,0
	m_tEffectProps.vUVMovePosSum = _vec2(0.f, 0.f);
	m_tEffectProps.vUVMovePosSum2 = _vec2(0.f, 0.f);
	m_tEffectProps.vUVMovePosSum3 = _vec2(0.f, 0.f);
	m_tEffectProps.vDiminishRate = _vec4(1.f, 1.f, 1.f, 1.f); // 만약에 옅어지게 할거라면 switch에서 x,y,z에 dTimeDelta 빼면 됨
	m_tEffectProps.vUseAdd = _vec4(0.f, 0.f, 0.f, 0.f); //쓸거면 1110 안쓸거면 0000
}

void CStaticEffect::Transform(const _double & dTimeDelta)
{
	if (m_tBaseInfo.eEffectID == EFFECT_ORB_BEAM)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.f, -1.f, 0.f), 4000, dTimeDelta);
	}
}

void CStaticEffect::HoEffect()
{
	Engine::CSphereCollider* pCol_First = nullptr;
	Engine::CSphereCollider* pCol_Att = nullptr;

	if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail1")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail3")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail2")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		//Engine::CSphereCollider* pCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(50.f, 0.f, 0.f));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail4")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		//Engine::CSphereCollider* pCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(50.f, 0.f, 0.f));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail5")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail6")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		//Engine::CSphereCollider* pCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(50.f, 0.f, 0.f));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail7")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		//Engine::CSphereCollider* pCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(50.f, 0.f, 0.f));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail8")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail9")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail10")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail11")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail12")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail13")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail14")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail15")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Trail16")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Sp")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(300.f, 0.f, 0.f));
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(300.f, 0.f, 0.f));

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 4.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Spear1")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(300.f, 0.f, 0.f));
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(300.f, 0.f, 0.f));

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 15.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Spear2")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(150.f, 100.f, 0.f));
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(150.f, 100.f, 0.f));

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 15.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Spear3")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(300.f, 0.f, 0.f));
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(300.f, 0.f, 0.f));

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 15.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Spear4")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(150.f, 0.f, -50.f));
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(150.f, 0.f, -50.f));

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 15.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Spear5")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(175.f, 0.f, 75.f));
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(175.f, 0.f, 75.f));

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 15.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Test")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션				
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 75, "", nullptr, &_vec3(90.f, 0.f, 0.f));
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 75, "", nullptr, &_vec3(90.f, 0.f, 0.f));

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 3.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Test2")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 7.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Cross1")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Cross2")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateShot1")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
		//m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		//m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		//m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		//m_tEffectProps.vMainOffsetSpeed = _vec2(11.f / m_tEffectInfo.fDeadTime, 0.f);
		//m_tEffectProps.vMask1OffsetSpeed = _vec2(11.f / m_tEffectInfo.fDeadTime, 0.f);
		//m_tEffectProps.vMask2OffsetSpeed = _vec2(11.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Blade1")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 7.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Blade2")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		/*Engine::CSphereCollider* pCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200, "", nullptr, &_vec3(300.f, 0.f, 0.f));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol);*/

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 0.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 5;
		m_bLoop = false;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirUp1")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 1.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 7.f, 1.f);
		//m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirUp2")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 1.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 7.f, 1.f);
		//m_tEffectProps.vUseMask1 = _vec4(0.f, 0.f, 0.f, 1.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
		m_bLoop = false;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail1")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail2")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail3")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail4")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail5")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail6")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail7")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail8")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail9")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail10")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail11")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail12")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail13")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail14")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail15")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail16")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail17")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail18")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail19")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_FateTrail20")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Air1")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 1.f;
		m_iPass = 3;
		m_bLoop = true;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_Air2")
	{
		//m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 1.f;
		m_iPass = 3;
		m_bLoop = true;
	}
	//start
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirTrail1")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirTrail3")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirTrail2")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirTrail4")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(5.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirTrail5")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirTrail6")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirTrail7")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirTrail8")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirTrail9")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_AirTrail10")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_GenocideTrail1")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 1.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 7.f, 1.f);

		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
		m_bLoop = false;
	}

	else if (m_tEffectInfo.wstrName == L"Mesh_DS_GenocideTrail2")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);

		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(7.f / m_tEffectInfo.fDeadTime, 0.f);
		m_fFrameSpeed = 2.f;
		m_iPass = 3;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_GenocideTrail3")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 1.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 3.f, 1.f);

		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
		m_bLoop = false;
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_DS_BulletTrail")
	{
		pCol_First = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);
		pCol_Att = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 200);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol_First);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].push_back(pCol_Att);

		m_tEffectProps.vMainUV_U = _vec3(0.f, 2.f, 1.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 2.f, 1.f);

		m_tEffectInfo.fDeadTime = 2.f;
		m_fFrameSpeed = 5.f;
		m_iPass = 4;
		m_bLoop = false;
	}
}

CStaticEffect * CStaticEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaticEffect* pInstance = new CStaticEffect(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStaticEffect::Free()
{
	CBaseEffect::Free();
}
