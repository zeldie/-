#include "stdafx.h"
#include "BaseObject.h"
#include "LightCamera.h"
float Gamma = 2.2f;
CBaseObject::CBaseObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_pTransformCom(nullptr),
	m_pRendererCom(nullptr),
	m_pOptimizationCom(nullptr),
	m_pShaderCom(nullptr),
	m_pEffectMgr(CEffectMgr::GetInstance()),
	m_pCollisionMgr(CCollisionMgr::GetInstance()),
	m_bIsDead(false)
{
	ZeroMemory(&m_tBaseInfo, sizeof(BASE_INFO));
	//m_tBaseInfo = BASE_INFO();
}


CBaseObject::~CBaseObject()
{
}

HRESULT CBaseObject::Set_ShaderMatrix(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView, matProj/*, matLightView, matLightProj*/;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	Engine::Get_CamView(&matView);
	Engine::Get_CamProj(&matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetMatrix("g_matLightView", &m_pLightCamera->Get_LightView());
	pEffect->SetMatrix("g_matLightProj", &m_pLightCamera->Get_LightProj());

	pEffect->SetFloat("fFocalDistance", 400.f);
	pEffect->SetFloat("fFocalRange", 10000.f);

	pEffect->SetVector("vOcclusionColor", &_vec4(0.f, 0.f, 0.f, 0.f));
	pEffect->SetVector("vChangeColor", &_vec4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetFloat("fInvCamFar", 1 / CCameraMgr::GetInstance()->Get_CamFar());

	//pEffect->SetFloat("fShadowBias", temp);

	D3DVIEWPORT9			ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	_vec2 fTexScale = _vec2(1.f / ViewPort.Width, 1.f / ViewPort.Height);
	pEffect->SetValue("g_texmapscale", &fTexScale, sizeof(_vec2));
	return S_OK;
}

HRESULT CBaseObject::Set_ShaderShadow(LPD3DXEFFECT & pEffect)
{
	return S_OK;
}

_bool CBaseObject::Culling()
{
	_vec3 vPos = *(m_pTransformCom->Get_Info(Engine::INFO_POS));
	return m_pOptimizationCom->IsIn_Frustum_ForObject(&vPos, 2000.f);
}

void CBaseObject::BillBoard(_vec3* vScale)
{
	_matrix matView;
	Engine::Get_CamView(&matView);

	memset(&matView._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matView, 0, &matView);

	_matrix matWorld = *m_pTransformCom->Get_WorldMatrix();

	//Pos
	_vec3 vBillPos = _vec3(0.f, 0.f, 0.f);

	//Scale
	float fScale[SCALE_END] = { 0.f };

	fScale[SCALE_X] = vScale->x;
	fScale[SCALE_Y] = vScale->y;
	fScale[SCALE_Z] = 1.f;


	//Scale
	for (_uint i = 0; i < SCALE_END; ++i)
	{
		for (_uint j = 0; j < 4; ++j)
			matView(i, j) *= fScale[i];
	}
	
	matWorld = matView * matWorld;
	m_pTransformCom->Set_Matrix(&matWorld);
}

HRESULT CBaseObject::Ready_GameObject()
{
	return S_OK;
}

_int CBaseObject::Update_GameObject(const _double & dTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(dTimeDelta);
	for (int i = 0; i < COLLSPHERETYPE::SPHERE_END; ++i)
	{
		for (auto& rObj : m_vecCollSphere[i])
			rObj->Update_Component(dTimeDelta);
	}
	for (int i = 0; i < COLLBOXTYPE::BOX_END; ++i)
	{
		for (auto& rObj : m_vecCollBox[i])
			rObj->Update_Component(dTimeDelta);
	}
	return iExit;
}

_int CBaseObject::LateUpdate_GameObject(const _double & dTimeDelta)
{
	_int iExit = Engine::CGameObject::LateUpdate_GameObject(dTimeDelta);
	
	return iExit;
}

void CBaseObject::Render_Depth(const _double & dTimeDelta)
{
}	

void CBaseObject::Render_Geometry(const _double & dTimeDelta)
{
#ifdef _DEBUG
	if (g_RenderCollider)
	{
		for (int i = 0; i < COLLSPHERETYPE::SPHERE_END; ++i)
		{
			for (auto& rObj : m_vecCollSphere[i])
				rObj->Render_Collider();
		}
		for (int i = 0; i < COLLBOXTYPE::BOX_END; ++i)
		{
			for (auto& rObj : m_vecCollBox[i])
				rObj->Render_Collider();
		}
	}
#endif
}

void CBaseObject::Render_PostEffect(const _double & dTimeDelta)
{
}

void CBaseObject::Render_Shadow(const _double & dTimeDelta)
{
}

_vec3 CBaseObject::Create_Pos(const _matrix * pMatWorld, _vec3 * pPos)
{
	_vec3 vRight, vUp, vLook, vMatPos, vNewPos;
	memcpy(&vRight, pMatWorld->m[0], sizeof(_vec3));
	memcpy(&vUp, pMatWorld->m[1], sizeof(_vec3));
	memcpy(&vLook, pMatWorld->m[2], sizeof(_vec3));
	memcpy(&vMatPos, pMatWorld->m[3], sizeof(_vec3));

	float	fXScale = D3DXVec3Length(&vRight);
	float	fYScale = D3DXVec3Length(&vUp);
	float	fZScale = D3DXVec3Length(&vLook);

	_matrix	MymatWorld, matScale, NewWorld;
	D3DXMatrixIdentity(&MymatWorld);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixScaling(&matScale, fXScale, fYScale, fZScale);
	D3DXMatrixInverse(&matScale, NULL, &matScale);
	MymatWorld = MymatWorld * matScale;
	NewWorld = MymatWorld * *pMatWorld;

	_vec3 vNewRight, vNewUp, vNewLook, vNewvPos;

	memcpy(&vNewRight, &NewWorld.m[0], sizeof(_vec3));
	memcpy(&vNewUp, &NewWorld.m[1], sizeof(_vec3));
	memcpy(&vNewLook, &NewWorld.m[2], sizeof(_vec3));
	memcpy(&vNewvPos, &NewWorld.m[3], sizeof(_vec3));

	vNewPos.x = (vNewRight.x * pPos->x) + (vNewUp.x * pPos->y) + (vNewLook.x * pPos->z) + (vNewvPos.x * 1.f);
	vNewPos.y = (vNewRight.y * pPos->x) + (vNewUp.y * pPos->y) + (vNewLook.y * pPos->z) + (vNewvPos.y * 1.f);
	vNewPos.z = (vNewRight.z * pPos->x) + (vNewUp.z * pPos->z) + (vNewLook.z * pPos->z) + (vNewvPos.z * 1.f);

	return vNewPos;
}


void CBaseObject::Free()
{
	for (int i = 0; i < COLLSPHERETYPE::SPHERE_END; ++i)
	{
		for (auto& rObj : m_vecCollSphere[i])
			Engine::Safe_Release(rObj);
		m_vecCollSphere[i].clear();
	}
	for (int i = 0; i < COLLBOXTYPE::BOX_END; ++i)
	{
		for (auto& rObj : m_vecCollBox[i])
			Engine::Safe_Release(rObj);
		m_vecCollBox[i].clear();
	}
	CGameObject::Free();
}
