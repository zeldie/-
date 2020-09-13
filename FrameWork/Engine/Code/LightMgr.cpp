#include "LightMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightMgr)

Engine::CLightMgr::CLightMgr()
{

}

Engine::CLightMgr::~CLightMgr()
{
	Free();
}

const D3DLIGHT9 * CLightMgr::Get_Light(const _uint & iIndex)
{
	auto iter = m_LightList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Light();
}

HRESULT CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9 * pLightInfo, const _uint & iIndex)
{
	CLight*	pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	if (pLight == nullptr)
		return E_FAIL;

	m_LightList.push_back(pLight);

	return S_OK;
}

void CLightMgr::Render_Light(LPD3DXEFFECT & pEffect)
{
	for (auto& iter : m_LightList)
		iter->Render_Light(pEffect);
}

void CLightMgr::Delete_Light(_uint iIndex)
{
	auto iter = m_LightList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;
	Safe_Release(*iter);
	m_LightList.pop_back();


}

void Engine::CLightMgr::Free()
{
	for_each(m_LightList.begin(), m_LightList.end(), CDeleteObj());
	m_LightList.clear();
}

