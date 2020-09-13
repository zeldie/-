#include "GameObject.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CGameObject::~CGameObject()
{

}

CComponent * CGameObject::Get_Component(COMPONENTTYPE eComponentType, COMPONENTID eID)
{
	Engine::CComponent*		pComponent = Find_Component(eComponentType, eID);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}
CComponent * CGameObject::Find_Component(COMPONENTTYPE eComponentType, COMPONENTID eID)
{
	auto	iter = m_mapComponent[eID].find(eComponentType);

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

void CGameObject::EraseComponent(COMPONENTTYPE eComponentType, COMPONENTID eID)
{
	auto	iter = m_mapComponent[eID].find(eComponentType);

	if (iter == m_mapComponent[eID].end())
		return;

	m_mapComponent[eID].erase(eComponentType);
}

void CGameObject::Compute_ViewZ(const _vec3 * pPos)
{
	_matrix			matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_vec3		vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));
}


HRESULT Engine::CGameObject::Ready_GameObject()
{
	return S_OK;
}

_int Engine::CGameObject::Update_GameObject(const _double& dTimeDelta)
{
	_int iExit = 0;

	for (auto& iter : m_mapComponent[ID_DYNAMIC])
	{
		iExit = iter.second->Update_Component(dTimeDelta);

		if (iExit & 0x80000000)
			return ERROR_EXIT;
	}

	return iExit;
}

_int CGameObject::LateUpdate_GameObject(const _double & dTimeDelta)
{
	return NO_EVENT;
}

void CGameObject::Render_Depth(const _double & dTimeDelta)
{
}

void CGameObject::Render_Geometry(const _double & dTimeDelta)
{
}


void CGameObject::Render_PostEffect(const _double & dTimeDelta)
{
}

void CGameObject::Render_Shadow(const _double & dTimeDelta)
{
}


void Engine::CGameObject::Free()
{
	Safe_Release(m_pGraphicDev);

	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

}

