#include "stdafx.h"
#include "DynamicMeshObject.h"


CDynamicMeshObject::CDynamicMeshObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseObject(pGraphicDev),
	m_pDynamicMeshCom(nullptr),
	m_pNaviMeshCom(nullptr),
	m_bNoRender(true),
	m_dwNaviIndex(0)
{
}


CDynamicMeshObject::~CDynamicMeshObject()
{
}

_vec3 CDynamicMeshObject::GetDir(Engine::INFO eInfo)
{
	_vec3 vDir;
	memcpy(&vDir, m_pTransformCom->Get_Info(eInfo), sizeof(_vec3));
	return *D3DXVec3Normalize(&vDir, &vDir);
}

HRESULT CDynamicMeshObject::Link_NaviMesh()
{
	if (m_pNaviMeshCom == nullptr)
	{
		m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Terrain", Engine::NAVI, Engine::ID_STATIC));
		m_pNaviMeshCom->Find_Index(m_dwNaviIndex, &((*m_pTransformCom->Get_Info(Engine::INFO_POS)) + _vec3(0.f, 30.f, 0.f)));
		if (m_dwNaviIndex == 1000)
			return false;
		m_pNaviMeshCom->Find_PosY(m_pTransformCom->Get_Info(Engine::INFO_POS), m_dwNaviIndex, *m_pTransformCom->Get_Info(Engine::INFO_POS));
		m_pTransformCom->Set_Pos(m_pTransformCom->Get_Info(Engine::INFO_POS));
		return true;
	}
	else
		return S_OK;
	return S_OK;
}

HRESULT CDynamicMeshObject::LoadOuterCollider(WCHAR * wstrPath)
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

HRESULT CDynamicMeshObject::LoadHitCollider(WCHAR * wstrPath)
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
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_DEF].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pComponent));
	}

	CloseHandle(hFile);
	return S_OK;
}

void CDynamicMeshObject::Calculate_Collision()
{
	//m_tBaseInfo.iHp-= 

}

HRESULT CDynamicMeshObject::LoadAtkCollider(WCHAR * wstrPath, _vec3 * pPos)
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

HRESULT CDynamicMeshObject::Ready_GameObject()
{
	return S_OK;
}

_int CDynamicMeshObject::Update_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::Update_GameObject(dTimeDelta);

	
	return iExit;
}

_int CDynamicMeshObject::LateUpdate_GameObject(const _double & dTimeDelta)
{
	_int iExit = CBaseObject::LateUpdate_GameObject(dTimeDelta);
	return iExit;
}

void CDynamicMeshObject::Render_Geometry(const _double & dTimeDelta)
{
	CBaseObject::Render_Geometry(dTimeDelta);
}

void CDynamicMeshObject::Render_PostEffect(const _double & dTimeDelta)
{
}

void CDynamicMeshObject::Render_Shadow(const _double & dTimeDelta)
{
}

void CDynamicMeshObject::Free()
{
	CBaseObject::Free();
}