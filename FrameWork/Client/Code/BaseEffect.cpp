#include "stdafx.h"
#include "BaseEffect.h"

CBaseEffect::CBaseEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseObject(pGraphicDev),
	m_pTextureCom(nullptr),
	m_dFrame(0.),
	m_dMaxFrame(0.),
	m_bLoop(false),
	m_fFrameSpeed(1.f),
	m_dbTime(0.),
	m_iPass(0)
{
	ZeroMemory(&m_vDir, sizeof(_vec3));
}


CBaseEffect::~CBaseEffect()
{
}

HRESULT CBaseEffect::Ready_GameObject()
{
	return S_OK;
}

_int CBaseEffect::Update_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::Update_GameObject(dTimeDelta);
	return iExit;
}

_int CBaseEffect::LateUpdate_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::LateUpdate_GameObject(dTimeDelta);
	return iExit;
}

void CBaseEffect::Render_Geometry(const _double & dTimeDelta)
{
}

void CBaseEffect::Delete_Collider()
{
	for (int i = 0; i < COLLSPHERETYPE::SPHERE_END; i++)
	{
		for (auto& rObj : m_vecCollSphere[i])
			Engine::Safe_Release(rObj);
		m_vecCollSphere[i].clear();
	}
}

void CBaseEffect::Set_ColliderScaling(const _double& dTimeDelta)
{
	// 구 충돌에서만 해당되는 코드, 박스충돌은 새로 구현해야한다.
	for (int i = 0; i < COLLSPHERETYPE::SPHERE_END; i++)
	{
		for (auto iter : m_vecCollSphere[i])
		{
			if (m_tEffectInfo.vMoveScale.x >= 0.09f)
			{
				iter->Set_Radius((_float)(m_tEffectInfo.vMoveScale.x * dTimeDelta) / m_tEffectInfo.vScale.x);
			}
		}

	}
}

void CBaseEffect::ResetEffectInfo()
{
	m_tEffectInfo.wstrType = L"";
	m_tEffectInfo.wstrName = L"";
	m_tEffectInfo.fSpeed = 0.f;
	m_tEffectInfo.fDeadTime = 0.f;
	m_tEffectInfo.iPass = 0;
	m_tEffectInfo.iAlpha = 0;
	m_tEffectInfo.vScale = _vec3(1.f,1.f,1.f);

	ZeroMemory(&m_tEffectInfo.vPos, sizeof(_vec3));
	ZeroMemory(&m_tEffectInfo.vRot, sizeof(_vec3));
	ZeroMemory(&m_tEffectInfo.vMovePos, sizeof(_vec3));
	ZeroMemory(&m_tEffectInfo.vMoveRot, sizeof(_vec3));
	ZeroMemory(&m_tEffectInfo.vMoveScale, sizeof(_vec3));
}

void CBaseEffect::Free()
{
	CBaseObject::Free();
}
