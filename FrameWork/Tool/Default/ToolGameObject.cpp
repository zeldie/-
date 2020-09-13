#include "stdafx.h"
#include "ToolGameObject.h"

double		g_dTimeBegin;
double		g_dTimeEnd;
bool		g_bIsPlay;

CToolGameObject::CToolGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev),
	m_pRendererCom(nullptr),
	m_pTransformCom(nullptr),
	m_pShaderCom(nullptr),
	m_bIsDead(false),
	m_bIsWireFrame(false),
	m_fDeadTime(0.f),
	m_iAlpha(0),
	m_fSpeed(0.f),
	m_fFrame(0.f)
{
	ZeroMemory(&m_vOrginPos, sizeof(_vec3));
	ZeroMemory(&m_vOrginRot, sizeof(_vec3));
	m_vOrginScale = { 1.f, 1.f, 1.f };
	ZeroMemory(&m_vMovePos, sizeof(_vec3));
	ZeroMemory(&m_vMoveRot, sizeof(_vec3));
	ZeroMemory(&m_vMoveScale, sizeof(_vec3));
	ZeroMemory(&m_vDir, sizeof(_vec3));
}


CToolGameObject::~CToolGameObject()
{
}

HRESULT CToolGameObject::Ready_GameObject()
{
	return S_OK;
}

_int CToolGameObject::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	Engine::CGameObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CToolGameObject::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	Engine::CGameObject::LateUpdate_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

void CToolGameObject::Render_Geometry(const _double& dTimeDelta)
{
	
}

HRESULT CToolGameObject::Set_ShaderMatrix(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("fFocalDistance", 400.f);
	pEffect->SetFloat("fFocalRange", 10000.f);
	pEffect->SetVector("vChangeColor", &_vec4(1.f, 1.f, 1.f, 1.f));
	return S_OK;
}

void CToolGameObject::Add_TextureInfo(Engine::EFFECT_UV_INFO eInfo)
{
	m_vecTextureInfo.push_back(eInfo);
}

void CToolGameObject::Delete_TextureInfo(Engine::EFFECT_UV_INFO eInfo)
{
	for (int i =0; i < m_vecTextureInfo.size(); i++)
	{
		if (m_vecTextureInfo[i].iTextureName == eInfo.iTextureName)
			m_vecTextureInfo.erase(m_vecTextureInfo.begin() + i);
	}
}

void CToolGameObject::Delete_TextureInfoIdx(_uint iIdx)
{
	for (int i = 0; i < m_vecTextureInfo.size(); i++)
	{
		if (m_vecTextureInfo[i].iTextureName == iIdx)
			m_vecTextureInfo.erase(m_vecTextureInfo.begin() + i);
	}
}

Engine::EFFECT_UV_INFO * CToolGameObject::Get_TextureInfo(_uint iIdx)
{
	for (auto iter : m_vecTextureInfo)
	{
		if (iter.iTextureName == iIdx)
		{
			return &iter;
		}
	}
	return nullptr;
}

void CToolGameObject::Set_TextureInfo(_uint iIdx, Engine::EFFECT_UV_INFO eInfo)
{
	for (auto& iter : m_vecTextureInfo)
	{
		if (iter.iTextureName == iIdx)
		{
			iter.iPass = eInfo.iPass;
			iter.vUVAtlas = eInfo.vUVAtlas;
			iter.vUVMovePos = eInfo.vUVMovePos;
			iter.vUVMovePosSum.x = 0.f;
			iter.vUVMovePosSum.y = 0.f;
			iter.vUVPos = eInfo.vUVPos;
			iter.dbFrame = 0.;
			iter.dbMaxFrame = eInfo.vUVAtlas.x * eInfo.vUVAtlas.y;
		}
	}
}

void CToolGameObject::Free()
{
	Engine::CGameObject::Free();
}
