#include "Component.h"

USING(Engine)

Engine::CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev),
	m_bClone(false)
{
	Safe_AddRef(m_pGraphicDev);
}

CComponent::CComponent(const CComponent & rhs)
	:m_pGraphicDev(rhs.m_pGraphicDev),
	m_bClone(true)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CComponent::~CComponent()
{

}

_int Engine::CComponent::Update_Component(const _double& dTimeDelta)
{
	return NO_EVENT;
}

void CComponent::Free()
{
	Safe_Release(m_pGraphicDev);
}

