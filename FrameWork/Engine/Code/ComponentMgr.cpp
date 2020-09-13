#include "ComponentMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponentMgr)

CComponentMgr::CComponentMgr()
	:m_pmapComponent(nullptr),
	m_wContainerSize(0)
{
}


CComponentMgr::~CComponentMgr()
{
	Free();
}

HRESULT CComponentMgr::Reserve_ContainerSize(const _ushort & wSize)
{
	if (nullptr != m_pmapComponent)
		return E_FAIL;

	m_pmapComponent = new map<wstring, CComponent*>[wSize];

	m_wContainerSize = wSize;

	return S_OK;
}

HRESULT CComponentMgr::Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, BUFFERID eID, const _ulong & dwVtxCntX, const _ulong & dwVtxCntZ, const _ulong & dwVtxItv, const _ulong & dwDetail)
{
	if (nullptr == m_pmapComponent)
	{
		MSG_BOX("Resources Container not Reserved");
		return S_OK;
	}

	CComponent*		pComponent = Find_Component(wContainerIdx, wstrBufferTag);

	if (nullptr != pComponent)
		return E_FAIL;

	switch (eID)
	{
	case BUFFER_TRICOL:
		pComponent = CTriCol::Create(pGraphicDev);
		break;
	case BUFFER_RCTEX:
		pComponent = CRcTex::Create(pGraphicDev);
		break;
	case BUFFER_CUBECOL:
		pComponent = CCubeCol::Create(pGraphicDev);
		break;
	case BUFFER_CUBETEX:
		pComponent = CCubeTex::Create(pGraphicDev);
		break;
	case BUFFER_TERRAINTEX:
		pComponent = CTerrainTex::Create(pGraphicDev, dwVtxCntX, dwVtxCntZ, dwVtxItv);
		break;
	case BUFFER_PARTICLETEX:
		pComponent = CParticleTex::Create(pGraphicDev);
		break;
	case BUFFER_DYNAMIC:
		pComponent = CDynamicBuffer::Create(pGraphicDev, dwVtxCntX);
		break;
	}
	if (pComponent == nullptr)
		return E_FAIL;

	m_pmapComponent[wContainerIdx].emplace(wstrBufferTag, pComponent);

	return S_OK;
}

HRESULT CComponentMgr::Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrTextureTag, TEXTURETYPE eType, const wstring & wstrPath, const _uint & iCnt)
{
	if (nullptr == m_pmapComponent)
	{
		MSG_BOX("Resources Container not Reserved");
		return S_OK;
	}

	CComponent*		pComponent = Find_Component(wContainerIdx, wstrTextureTag);

	if (nullptr != pComponent)
		return E_FAIL;

	pComponent = CTexture::Create(pGraphicDev, wstrPath, eType, iCnt);
	if (pComponent == nullptr)
		return E_FAIL;

	m_pmapComponent[wContainerIdx].emplace(wstrTextureTag, pComponent);

	return S_OK;
}

HRESULT CComponentMgr::Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, wstring wstrMeshTag, MESHTYPE eType, wstring wstrFilePath, wstring wstrFileName)
{
	if (nullptr == m_pmapComponent)
	{
		MSG_BOX("Resources Container not Reserved");
		return E_FAIL;
	}

	CComponent*		pComponent = Find_Component(wContainerIdx, wstrMeshTag);

	if (nullptr != pComponent)
		return E_FAIL;


	switch (eType)
	{
	case TYPE_STATIC:
		pComponent = CStaticMesh::Create(pGraphicDev, wstrFilePath, wstrFileName);
		//wcout << "[StaticMesh] / " << wstrFileName.c_str() << " / 로드 성공" << endl;
		break;
	case TYPE_DYNAMIC:
		pComponent = CDynamicMesh::Create(pGraphicDev, wstrFilePath, wstrFileName);
		//wcout << "[DynamicMesh] / " << wstrFileName.c_str() << " / 로드 성공" << endl;
		break;
	case TYPE_NAVI:
		pComponent = CNaviMesh::Create(pGraphicDev, wstrFileName);
		break;
	case TYPE_NAVI_RUN:
		pComponent = CNaviMeshRun::Create(pGraphicDev, wstrFileName);
		break;
	}

	if (pComponent == nullptr)
		return E_FAIL;

	m_pmapComponent[wContainerIdx].emplace(wstrMeshTag, pComponent);

	return S_OK;
}

HRESULT CComponentMgr::Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, wstring wstrShaderTag, wstring wstrFileName)
{
	if (nullptr == m_pmapComponent)
	{
		MSG_BOX("Resources Container not Reserved");
		return E_FAIL;
	}
	CComponent*		pComponent = Find_Component(wContainerIdx, wstrShaderTag);

	if (nullptr != pComponent)
		return E_FAIL;

	pComponent = CShader::Create(pGraphicDev, wstrFileName);

	m_pmapComponent[wContainerIdx].emplace(wstrShaderTag, pComponent);
	
	return S_OK;
}


void CComponentMgr::Render_Texture(const _ushort & wContainerIdx, const wstring & wstrTextureTag, const _uint & iIndex)
{
	CComponent*		pComponent = Find_Component(wContainerIdx, wstrTextureTag);

	if (nullptr == pComponent)
		return;

	dynamic_cast<CTexture*>(pComponent)->Render_Texture(iIndex);
}

void CComponentMgr::Render_Buffer(const _ushort & wContainerIdx, const wstring & wstrBufferTag)
{
	CComponent*		pComponent = Find_Component(wContainerIdx, wstrBufferTag);

	if (nullptr == pComponent)
		return;

	dynamic_cast<CVIBuffer*>(pComponent)->Render_Buffer();
}

CComponent * CComponentMgr::Clone(const _ushort & wContainerIdx, const wstring & wstrComponentTag)
{
	if (nullptr == m_pmapComponent)
		return nullptr;

	CComponent*		pPrototype = Find_Component(wContainerIdx, wstrComponentTag);
	

	return dynamic_cast<CComponent*>(pPrototype->Clone());
}

void CComponentMgr::DeleteResource(const _ushort & wContainerIdx, const wstring & wstrComponentTag)
{
	auto	iter = m_pmapComponent[wContainerIdx].find(wstrComponentTag);

	if (iter == m_pmapComponent[wContainerIdx].end())
		return;

	//iter->second->Free();
}

void CComponentMgr::ClearComponent(const _ushort & wContainerIdx)
{
	for_each(m_pmapComponent[wContainerIdx].begin(), m_pmapComponent[wContainerIdx].end(), CDeleteMap());
	m_pmapComponent[wContainerIdx].clear();
}

HRESULT CComponentMgr::LoadMesh(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath)
{
	wifstream fin;
	fin.open(wstrPath.c_str());
	TCHAR szResourceType[MAX_STR] = L"";
	TCHAR szMeshType[MAX_STR] = L"";
	TCHAR szObjTag[MAX_STR] = L"";
	TCHAR szFilePath[MAX_STR] = L"";
	TCHAR szFileName[MAX_STR] = L"";
	wstring wstrResourceType = L"";
	wstring wstrMeshType = L"";
	wstring wstrObjTag = L"";
	wstring wstrFilePath = L"";
	wstring wstrFileName = L"";
	while (true)
	{
		fin.getline(szResourceType, MAX_STR, '|');
		if (szResourceType == L"")
			break;
		fin.getline(szMeshType, MAX_STR, '|');
		fin.getline(szObjTag, MAX_STR, '|');
		fin.getline(szFilePath, MAX_STR, '|');
		fin.getline(szFileName, MAX_STR);

		if (fin.eof())
			break;
		//리소스 타입 받기
		wstrResourceType = szResourceType;

		//메쉬 타입 받기
		wstrMeshType = szMeshType;

		//Obj이름 받기
		wstrObjTag = szObjTag;

		//경로 받기
		wstrFilePath = szFilePath;

		//파일 이름 받기
		wstrFileName = szFileName;
		if (wstrMeshType == L"StaticMesh")
		{
			if (wstrResourceType == L"Static")
			{
				if (FAILED(Ready_Meshes(pGraphicDev, Engine::RESOURCE_STATIC, wstrObjTag, MESHTYPE::TYPE_STATIC, wstrFilePath, wstrFileName)))
				{
					wcout << "[StaticMesh] / " << wstrFileName.c_str() << " / ########로드 실패########" << endl;
					MSG_BOX("StaticMesh Load Failed");
					return E_FAIL;
				}
			}
			else if (wstrResourceType == L"Stage")
			{
				if (FAILED(Ready_Meshes(pGraphicDev, Engine::RESOURCE_STAGE, wstrObjTag, MESHTYPE::TYPE_STATIC, wstrFilePath, wstrFileName)))
				{
					MSG_BOX("StaticMesh Load Failed");
					return E_FAIL;
				}
			}

		}
		else if (wstrMeshType == L"DynamicMesh")
		{
			if (wstrResourceType == L"Static")
			{
				if (FAILED(Ready_Meshes(pGraphicDev, Engine::RESOURCE_STATIC, wstrObjTag, MESHTYPE::TYPE_DYNAMIC, wstrFilePath, wstrFileName)))
				{
					wcout << "[DynamicMesh] / " << wstrFileName.c_str() << " / ########로드 실패########" << endl;
					MSG_BOX("DynamicMesh Load Failed");
					return E_FAIL;
				}
			}
			else if (wstrResourceType == L"Stage")
			{
				if (FAILED(Ready_Meshes(pGraphicDev, Engine::RESOURCE_STAGE, wstrObjTag, MESHTYPE::TYPE_DYNAMIC, wstrFilePath, wstrFileName)))
				{
					MSG_BOX("DynamicMesh Load Failed");
					return E_FAIL;
				}
			}
		}
	}
	fin.close();
	return S_OK;
}


HRESULT CComponentMgr::LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath)
{
	wifstream fin;
	fin.open(wstrPath.c_str());
	TCHAR szResourceType[MAX_STR] = L"";
	TCHAR szTextureTag[MAX_STR] = L"";
	TCHAR szFilePath[MAX_STR] = L"";
	TCHAR szTextureNum[MAX_STR] = L"";
	wstring wstrResourceType = L"";
	wstring wstrTextureTag = L"";
	wstring wstrFilePath = L"";
	_int	iTextureNum = 0;
	while (true)
	{
		fin.getline(szResourceType, MAX_STR, '|');
		if (szResourceType == L"")
			break;
		fin.getline(szTextureTag, MAX_STR, '|');
		fin.getline(szFilePath, MAX_STR, '|');
		fin.getline(szTextureNum, MAX_STR);

		if (fin.eof())
			break;
		//텍스쳐 타입(STATIC 인지 STAGE인지 등) 받기
		wstrResourceType = szResourceType;

		//텍스쳐이름 받기
		wstrTextureTag = szTextureTag;

		//경로 받기
		wstrFilePath = szFilePath;

		//텍스쳐 개수 받기
		iTextureNum = _wtoi(szTextureNum);

		if (wstrResourceType == L"Static")
		{
			if (FAILED(Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_" + wstrTextureTag, TEXTURETYPE::TEX_NORMAL, wstrFilePath, iTextureNum)))
			{
				MSG_BOX("Texture Load Failed");
				fin.close();
				return E_FAIL;
			}
		}
		else if (wstrResourceType == L"Stage")
		{
			if (FAILED(Ready_Texture(pGraphicDev, RESOURCE_STAGE, L"Texture_" + wstrTextureTag, TEXTURETYPE::TEX_NORMAL, wstrFilePath, iTextureNum)))
			{
				MSG_BOX("Texture Load Failed");
				fin.close();
				return E_FAIL;
			}
		}
	}
	fin.close();
	return S_OK;
}

HRESULT CComponentMgr::Add_Component(CComponent* pComponent, const wstring& wstrComponentTag, const _ushort& wContainerIdx)
{
	if (pComponent == nullptr)
		return E_FAIL;

	m_pmapComponent[wContainerIdx].emplace(wstrComponentTag, pComponent);
	return S_OK;
}

CComponent * CComponentMgr::Find_Component(const _ushort & wContainerIdx, const wstring & wstrComponentTag)
{
	auto	iter = m_pmapComponent[wContainerIdx].find(wstrComponentTag);

	if (iter == m_pmapComponent[wContainerIdx].end())
		return nullptr;

	return iter->second;
}

void CComponentMgr::Free()
{
	for (_ushort i = 0; i < m_wContainerSize; ++i)
	{
		for (auto& iter : m_pmapComponent[i])
			Safe_Release(iter.second);
		//for_each(m_pmapComponent[i].begin(), m_pmapComponent[i].end(), CDeleteMap());
		m_pmapComponent[i].clear();
	}

	Safe_Delete_Array(m_pmapComponent);
}
