#include "stdafx.h"
#include "CylinderEffect.h"

CCylinderEffect::CCylinderEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseEffect(pGraphicDev)
	//,m_pStaticMeshCom(nullptr)
{
}


CCylinderEffect::~CCylinderEffect()
{
}

HRESULT CCylinderEffect::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	return S_OK;
}

_int CCylinderEffect::Update_GameObject(const _double & dTimeDelta)
{
	CBaseEffect::Update_GameObject(dTimeDelta);

	//프레임증가 + 최대프레임도달시 객체반환
	//if ((m_dFrame += (dTimeDelta * m_dMaxFrame * m_dLifeTime)) >= m_dMaxFrame)
	//if ((m_dFrame += dTimeDelta) >= m_tEffectInfo.fDeadTime)
	//{
	//	CBaseEffect::Delete_Collider();
	//	//m_pEffectMgr->Return_StaticPool(this);
	//	//return Engine::OBJ_POOL;
	//	return Engine::OBJ_DEAD;
	//}

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
		if (m_vecCollSphere[0].size() >= 1)
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_PLAYER_EFFECT, this);
	}
	else if (m_tBaseInfo.eObjectID == OBJECT_MONSTER)
	{
		if (m_vecCollSphere[0].size() >= 1)
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER_EFFECT, this);
	}

	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);


	//if (m_tEffectInfo.iAlpha == 0)
	//{
	//	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	//}
	//else if (m_tEffectInfo.iAlpha == 1)
	//{
	//	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	//}

	return Engine::NO_EVENT;
}

_int CCylinderEffect::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseEffect::LateUpdate_GameObject(dTimeDelta);

	return Engine::NO_EVENT;
}

void CCylinderEffect::Render_Geometry(const _double & dTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	//_ulong dwSubset = m_pMesh->

	_uint iPassMax = 0;

	//m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", (_ulong)0);

		pEffect->CommitChanges();
		pEffect->Begin(&iPassMax, 0);
		//if (m_tEffectInfo.iAlpha == 0) // 알파
		//{
		//	pEffect->BeginPass(5);
		//}
		//else if (m_tEffectInfo.iAlpha == 1) // 논알파
		//{
		//	pEffect->BeginPass(0);
		//}
		//m_pStaticMeshCom->Render_Meshes(i);
		pEffect->BeginPass(5);

		m_pMesh->DrawSubset(0);
		pEffect->EndPass();
		pEffect->End();

	//텍스쳐 갯수만큼 랜더 한번 더 돌린다.
	//for (auto J = 0; J < m_tEffectInfo.vectorUVInfo.size(); ++J)
	//{
	//	for (_ulong i = 0; i < dwSubset; ++i)
	//	{
	//		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_tEffectInfo.vectorUVInfo[J].iTextureName);
	//		//pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
	//		//pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
	//		//pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
	//		pEffect->CommitChanges();

	//		pEffect->SetVector("g_vUV_U", &_vec4((m_tEffectInfo.vectorUVInfo[J].vUVPos.x), (1.f / m_tEffectInfo.vectorUVInfo[J].vUVAtlas.x), (m_tEffectInfo.vectorUVInfo[J].vUVMovePos.x * _float(m_dFrame)), 0.f));
	//		pEffect->SetVector("g_vUV_V", &_vec4((m_tEffectInfo.vectorUVInfo[J].vUVPos.y), (1.f / m_tEffectInfo.vectorUVInfo[J].vUVAtlas.y), (m_tEffectInfo.vectorUVInfo[J].vUVMovePos.y * _float(m_dFrame)), 0.f));

	//		pEffect->Begin(&iPassMax, 0);
	//		pEffect->BeginPass(m_tEffectInfo.vectorUVInfo[J].iPass);
	//		m_pStaticMeshCom->Render_Meshes(i);
	//		pEffect->EndPass();
	//		pEffect->End();
	//	}
	//}
	pEffect->SetVector("g_vUV_U", &_vec4(0.f, 1.f, 0.f, 0.f));
	pEffect->SetVector("g_vUV_V", &_vec4(0.f, 1.f, 0.f, 0.f));

	Engine::Safe_Release(pEffect);

	m_pGraphicDev->SetTexture(0, nullptr);

	CBaseObject::Render_Geometry(dTimeDelta);
}

void CCylinderEffect::Set_Info(EFFECT_INFO tEffectInfo, const _matrix* matWorld, _vec3 * pAngle, BASE_INFO tBaseInfo)
{
	m_tEffectInfo = tEffectInfo;
	Clone_StaticCom();

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

	//cout << "X: " << vNewPos.x << "Y: " << vNewPos.y << "Z: " << vNewPos.z << endl;

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
	Add_Collider();
}


HRESULT CCylinderEffect::Clone_Component()
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Fire"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	D3DXCreateCylinder(m_pGraphicDev, 100.f, 100.f, 300.f, 10, 30, &m_pMesh, NULL);

	return S_OK;
}

_vec3 CCylinderEffect::Create_Angle()
{
	_vec3	vAngle;
	vAngle.x = ((rand() % 13) * 30.f) - 180.f;
	vAngle.y = ((rand() % 13) * 30.f) - 180.f;
	vAngle.z = ((rand() % 13) * 30.f) - 180.f;

	return vAngle;
}

HRESULT CCylinderEffect::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	return S_OK;
}

HRESULT CCylinderEffect::Clone_StaticCom()
{
	//if (m_pStaticMeshCom != nullptr)
	//	m_pStaticMeshCom->Release();
	//m_mapComponent[Engine::ID_STATIC].erase(Engine::MESH);

	//Engine::CComponent* pComponent = nullptr;

	//pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, m_tEffectInfo.wstrName));
	//if (pComponent == nullptr)
	//	return E_FAIL;
	//m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	return S_OK;
}

HRESULT CCylinderEffect::Add_Collider()
{
	CBaseEffect::Delete_Collider();


	// 시원 테스트 코드 =========================================================================

	return S_OK;
}

CCylinderEffect * CCylinderEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCylinderEffect* pInstance = new CCylinderEffect(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCylinderEffect::Free()
{
	CBaseEffect::Free();
	Engine::Safe_Release(m_pMesh);
}
