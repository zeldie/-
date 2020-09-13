#include "stdafx.h"
#include "DynamicEffect.h"

CDynamicEffect::CDynamicEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseEffect(pGraphicDev),
	m_pDynamicMeshCom(nullptr),
	m_dDissolveCountDown(0)
{
}


CDynamicEffect::~CDynamicEffect()
{
}

HRESULT CDynamicEffect::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	return S_OK;
}

_int CDynamicEffect::Update_GameObject(const _double & dTimeDelta)
{
	CBaseEffect::Update_GameObject(dTimeDelta);

	//프레임증가 + 최대프레임도달시 객체반환
	//if ((m_dFrame += (dTimeDelta * m_dMaxFrame * m_dLifeTime)) >= m_dMaxFrame)
	//if ((m_dFrame += dTimeDelta) >= m_tEffectInfo.fDeadTime *0.2f)
		//m_pDynamicMeshCom->Reset_Animation();		

	m_pDynamicMeshCom->Set_AnimationSet(0); //희정 변경 -> 다이나믹 이펙트 죽기전에 멈추게 할라고 

	if ((m_dFrame += dTimeDelta) >= m_tEffectInfo.fDeadTime)
	{
		// %%%%%%%%%%%%%% Ai %%%%%%%%%%%%% //
		if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_CARTEL)
		{
			if (m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].size() != 0)
				CAiMgr::GetInstance()->Delete_Effect(m_tBaseInfo.eObjectID, m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].front()->Get_TransformCom());
		}
		// %%%%%%%%%%%%%% Ai %%%%%%%%%%%%% //

		CBaseEffect::Delete_Collider();
		m_pEffectMgr->Return_DynamicPool(this);

		return Engine::OBJ_POOL;
	}

	m_vDir = m_pTransformCom->m_vInfo[Engine::INFO_LOOK];
	m_pTransformCom->Set_Info(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + (m_vDir*_float(dTimeDelta * m_tEffectInfo.fSpeed))), Engine::INFO_POS);
	m_pTransformCom->Rotation(Engine::ROT_X, _float(m_tEffectInfo.vMoveRot.x*dTimeDelta));
	m_pTransformCom->Rotation(Engine::ROT_Y, _float(m_tEffectInfo.vMoveRot.y*dTimeDelta));
	m_pTransformCom->Rotation(Engine::ROT_Z, _float(m_tEffectInfo.vMoveRot.z*dTimeDelta));
	m_pTransformCom->Set_Scale(&(m_pTransformCom->m_vScale + (m_tEffectInfo.vMoveScale*_float(dTimeDelta))));
	// 충돌구체 크기 키우는함수
	Set_ColliderScaling(dTimeDelta);

	if (m_tBaseInfo.eObjectID == OBJECT_PLAYER)
	{
		m_pCollisionMgr->Add_Collision(COLLLIST::COLL_PLAYER_EFFECT, this);
	}
	else if (m_tBaseInfo.eObjectID == OBJECT_MONSTER)
	{
		m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER_EFFECT, this);
	}

	return Engine::NO_EVENT;
}

_int CDynamicEffect::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseEffect::LateUpdate_GameObject(dTimeDelta);
	if (m_tEffectInfo.iAlpha == 0)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	}
	else if (m_tEffectInfo.iAlpha == 1)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	}

	return Engine::NO_EVENT;
}

void CDynamicEffect::Render_Geometry(const _double & dTimeDelta)
{
	if (m_pDynamicMeshCom->Get_TrackTime() >= m_pDynamicMeshCom->Get_Period()*0.98f) //희정 추가 -> 다이나믹 이펙트 죽기전에 멈추게 할라고 
		m_pDynamicMeshCom->GetAniCtrl()->Set_Stop(true);


	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta, 1);

	//Shader
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	if (m_tBaseInfo.eEffectID == EFFECT_TS_R1 || m_tBaseInfo.eEffectID == EFFECT_TS_R2 || m_tBaseInfo.eEffectID == EFFECT_TS_R3 || m_tBaseInfo.eEffectID == EFFECT_TS_R4)
	{
		if (m_dFrame > 2)
		{
			Engine::SetTexture(pEffect, "g_DissolveTexture", 6);
			Engine::SetTexture(pEffect, "g_DissolveEdgeTexture", 2);
			m_dDissolveCountDown += dTimeDelta;
			pEffect->SetFloat("fTime", m_dDissolveCountDown);
			m_iPass = 14;
		}
	}
	else if (m_tBaseInfo.eEffectID == EFFECT_TS_RIGHT_MID || m_tBaseInfo.eEffectID == EFFECT_TS_RIGHT_MIN || m_tBaseInfo.eEffectID == EFFECT_TS_AIRROCK_MID || m_tBaseInfo.eEffectID == EFFECT_TS_AIRROCK_MIN || m_tBaseInfo.eEffectID == EFFECT_TS_F)
	{
		if (m_dFrame > 3)
		{
			Engine::SetTexture(pEffect, "g_DissolveTexture", 6);
			Engine::SetTexture(pEffect, "g_DissolveEdgeTexture", 2);
			m_dDissolveCountDown += dTimeDelta;
			pEffect->SetFloat("fTime", m_dDissolveCountDown);
			m_iPass = 14;
		}
	}
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

			pEffect->BeginPass(m_iPass);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);

	m_pGraphicDev->SetTexture(0, nullptr);
	CBaseObject::Render_Geometry(dTimeDelta);
}

void CDynamicEffect::Set_Info(EFFECT_INFO tEffectInfo, const _matrix* matWorld, _vec3 * pAngle, BASE_INFO tBaseInfo)
{
	m_tEffectInfo = tEffectInfo;
	// 다이나믹메쉬 클론
	Clone_DynamicCom();

	m_tBaseInfo = tBaseInfo;
	m_dFrame = 0.;

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

	// 회전
	_matrix	Mymat_Rotate, Mymat_RotateU;
	D3DXMatrixIdentity(&Mymat_Rotate);
	D3DXMatrixIdentity(&Mymat_RotateU);
	D3DXVec3Normalize(&vNewRight, &vNewRight);
	D3DXVec3Normalize(&vNewUp, &vNewUp);
	D3DXMatrixRotationAxis(&Mymat_Rotate, &vNewRight, D3DXToRadian(m_tEffectInfo.vRot.x));
	D3DXMatrixRotationAxis(&Mymat_RotateU, &vNewUp, D3DXToRadian(m_tEffectInfo.vRot.y));

	_vec2 vvRot;
	vvRot.x = acosf((Mymat_Rotate.m[1][1]));
	vvRot.y = asinf((Mymat_RotateU.m[2][0]));
	vvRot.x = D3DXToDegree(vvRot.x);
	vvRot.y = D3DXToDegree(vvRot.y);

	m_pTransformCom->Set_Angle(&(*pAngle));
	m_pTransformCom->Rotation(Engine::ROT_X, vvRot.x);
	m_pTransformCom->Rotation(Engine::ROT_Y, vvRot.y);

	m_pTransformCom->Set_Scale(&(m_tEffectInfo.vScale));

	// 충돌체 추가
	Add_Collider();
	m_pTransformCom->Update_Component(0.f);
	m_dDissolveCountDown = 0;
}

HRESULT CDynamicEffect::Clone_Component()
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
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	//Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_EffectALL"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	return S_OK;
}

_vec3 CDynamicEffect::Create_Angle()
{
	_vec3	vAngle;
	vAngle.x = ((rand() % 13) * 30.f) - 180.f;
	vAngle.y = ((rand() % 13) * 30.f) - 180.f;
	vAngle.z = ((rand() % 13) * 30.f) - 180.f;

	return vAngle;
}

HRESULT CDynamicEffect::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	pEffect->SetValue("g_EffectProps", &m_tEffectProps, sizeof(EFFECT_PROPS));

	return S_OK;
}

HRESULT CDynamicEffect::Clone_DynamicCom()
{
	if (m_pDynamicMeshCom != nullptr)
		m_pDynamicMeshCom->Release();
	m_mapComponent[Engine::ID_STATIC].erase(Engine::MESH);

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, m_tEffectInfo.wstrName));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	return S_OK;
}

HRESULT CDynamicEffect::Add_Collider()
{
	CBaseEffect::Delete_Collider();

	if (m_tEffectInfo.wstrName == L"X파일이름")
	{
		LoadOuterCollider(L"메모장경로");
		LoadAtkCollider(L"메모장경로", &_vec3(0.f, 0.f, 0.f));

		m_tEffectProps.vMainUV_U = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUV_V = _vec3(0.f, 1.f, 0.f);
		m_tEffectProps.vMainUVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_U.y, 0.f);
		m_tEffectProps.vMask1UVOffset = _vec2(-1.f / m_tEffectProps.vMainUV_V.y, 0.f);
		m_tEffectProps.vMask2UVOffset = _vec2(-1.f, 0.f);
		m_tEffectProps.vMainOffsetSpeed = _vec2( 2.f / m_tEffectInfo.fDeadTime, 0.f);
		m_tEffectProps.vMask1OffsetSpeed = _vec2(1.f, 0.f);
		m_tEffectProps.vMask2OffsetSpeed = _vec2(1.f, 0.f);
		m_fFrameSpeed = 1.f;
		m_iPass = 3;	// 3== 한장 트레일 , 4 == 아틀라스 트레일 , 5 == 한장or아틀라스 UV 애니메이션

	}
	else if (m_tEffectInfo.wstrName == L"Mesh_Belatos")
	{
		LoadOuterCollider(L"../../Data/Collider/ColliderOuterBelatos.dat");
		LoadAtkCollider(L"../../Data/Collider/BelatosHit.dat", &_vec3(0.f, 0.f, 0.f));
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_Right_Min")
	{
		LoadOuterCollider(L"../../Data/Collider/TS_RIGHT_MIN_OUTER.dat");
		LoadAtkCollider(L"../../Data/Collider/TS_RIGHT_MIN_OUTER.dat", &_vec3(0.f, 0.f, 0.f));
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_R4")
	{
		LoadOuterCollider(L"../../Data/Collider/TS_R_OUTER.dat");
		LoadAtkCollider(L"../../Data/Collider/TS_R_OUTER.dat", &_vec3(0.f, 0.f, 0.f));
	}
	else if (m_tEffectInfo.wstrName == L"Mesh_TS_F")
	{
		LoadOuterCollider(L"../../Data/Collider/TS_F_OUTER.dat");
		LoadAtkCollider(L"../../Data/Collider/TS_F_OUTER.dat", &_vec3(0.f, 0.f, 0.f));
	}
	return S_OK;
}

HRESULT CDynamicEffect::LoadOuterCollider(WCHAR * wstrPath)
{
	HANDLE hFile = CreateFile(wstrPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	Engine::MESHTYPE eParentMeshType;
	char pFrameName[MIN_STR] = { 0, };
	_float fRadius = 0.f;
	Engine::CComponent*		pComponent = nullptr;
	while (true)
	{
		ReadFile(hFile, &pFrameName, MIN_STR, &dwByte, nullptr);
		if (dwByte == 0)
			break;
		ReadFile(hFile, &eParentMeshType, sizeof(Engine::MESHTYPE), &dwByte, nullptr);
		ReadFile(hFile, &fRadius, sizeof(_float), &dwByte, nullptr);

		pComponent = Engine::CSphereCollider::Create(m_pGraphicDev, eParentMeshType, m_pTransformCom, fRadius, pFrameName, m_pDynamicMeshCom);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pComponent));
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CDynamicEffect::LoadAtkCollider(WCHAR * wstrPath, _vec3 * pPos)
{
	HANDLE hFile = CreateFile(wstrPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	Engine::MESHTYPE eParentMeshType;
	char pFrameName[MIN_STR] = { 0, };
	_float fRadius = 0.f;
	Engine::CComponent*		pComponent = nullptr;
	while (true)
	{
		ReadFile(hFile, &pFrameName, MIN_STR, &dwByte, nullptr);
		if (dwByte == 0)
			break;
		ReadFile(hFile, &eParentMeshType, sizeof(Engine::MESHTYPE), &dwByte, nullptr);
		ReadFile(hFile, &fRadius, sizeof(_float), &dwByte, nullptr);

		pComponent = Engine::CSphereCollider::Create(m_pGraphicDev, eParentMeshType, m_pTransformCom, fRadius, pFrameName, m_pDynamicMeshCom);
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pComponent));
	}

	CloseHandle(hFile);
	return S_OK;
}

CDynamicEffect * CDynamicEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicEffect* pInstance = new CDynamicEffect(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDynamicEffect::Free()
{
	CBaseEffect::Free();
}
