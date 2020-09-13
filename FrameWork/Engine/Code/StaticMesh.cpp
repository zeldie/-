#include "StaticMesh.h"
USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
{
}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubsetBuffer(rhs.m_pSubsetBuffer)
	, m_pSubsets(rhs.m_pSubsets)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwStride(rhs.m_dwStride)
	, m_pVtxPos(rhs.m_pVtxPos)
{
	m_vecTexture.reserve(Engine::MESHTEXTURE_END);
	for (_uint i = 0; i < Engine::MESHTEXTURE_END; ++i)
	{
		LPDIRECT3DTEXTURE9* ppTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];
		for (_ulong j = 0; j < rhs.m_dwSubsetCnt; ++j)
		{
			if (rhs.m_vecTexture[i][j] != nullptr)
			{
				ppTexture[j] = rhs.m_vecTexture[i][j];
				Safe_AddRef(ppTexture[j]);
			}
			else
				ppTexture[j] = nullptr;
		}
		m_vecTexture.emplace_back(ppTexture);
	}
	m_pMesh->AddRef();
	m_pOriMesh->AddRef();
	m_pAdjacency->AddRef();
	m_pSubsetBuffer->AddRef();

}

Engine::CStaticMesh::~CStaticMesh()
{

}

LPDIRECT3DTEXTURE9 CStaticMesh::Get_Texture(Engine::MESHTEXTURE_TYPE eMeshTextureType, _uint iSubsetNum)
{
	if (m_vecTexture[eMeshTextureType] == nullptr)
		return nullptr;
	return m_vecTexture[eMeshTextureType][iSubsetNum];
}

HRESULT Engine::CStaticMesh::Ready_Prototype(wstring wstrFilePath, wstring wstrFileName)
{
	m_vecTexture.reserve(Engine::MESHTEXTURE_END);
	_tchar	szFullPath[256] = L"";

	lstrcpy(szFullPath, wstrFilePath.c_str());
	lstrcat(szFullPath, wstrFileName.c_str());

	if (FAILED(D3DXLoadMeshFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pGraphicDev,
		&m_pAdjacency,
		&m_pSubsetBuffer,
		NULL,			// �޽��� ���� Ư���� ����Ʈ ���ҽ��� �ּ� , �׷��� �츮�� ��� �� �� ����
		&m_dwSubsetCnt,
		&m_pOriMesh)))
		return E_FAIL;

	_ulong	dwFVF = m_pOriMesh->GetFVF(); // �޽��� �����ϴ� ���ؽ��� fvf������ ��ȯ�ϴ� �Լ�

	// �޽� FVF �Ӽ� �� NORMAL�� ���� ���
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}

	//////////////////////////////////////////////////////////////////////////
	void*		pVertex = nullptr;

	m_dwVtxCnt = m_pMesh->GetNumVertices(); // �޽��� ���� ������ ������ ��ȯ�ϴ� �Լ�
	m_pVtxPos = new _vec3[m_dwVtxCnt];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE]; // �޽� ���� �ȿ� ����ִ� FVF ������ �����ϴ� ����ü

	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl); // �޽� ���� �ȿ� ����ִ� FVF ������ �����ϴ� ����ü�� ���� ������ �Լ�

	_ubyte byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffset = (_ubyte)Decl[i].Offset;
			break;
		}
	}

	// fvf�ɼ� ���� �̿��Ͽ� �޽� ������ ũ�⸦ ����
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (i * m_dwStride + byOffset)));
	}
	
	// d3dxbuffer�� ��� �ִ� ���� �� ù ��° �ּҸ� ��ȯ�ϴ� �Լ�
	m_pSubsets = (D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer();


	for (_uint i = 0; i < Engine::MESHTEXTURE_END; ++i)
	{
		LPDIRECT3DTEXTURE9* ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
		m_vecTexture.emplace_back(ppTexture);
	}

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar	szFileName[256] = L"";

		lstrcpy(szFullPath, wstrFilePath.c_str());
		if (m_pSubsets[i].pTextureFilename == nullptr)
			continue;
		MultiByteToWideChar(CP_ACP, 
							0, 
							m_pSubsets[i].pTextureFilename, 
							_int(strlen(m_pSubsets[i].pTextureFilename)), 
							szFileName, 
							256);

		lstrcat(szFullPath, szFileName);
		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_vecTexture[Engine::MESHTEXTURE_DIFFUSE][i])))
			m_vecTexture[Engine::MESHTEXTURE_DIFFUSE][i] = nullptr;
		
		Engine::ChangeFileName(szFullPath, L"D", L"N");

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_vecTexture[Engine::MESHTEXTURE_NORMAL][i])))
			m_vecTexture[Engine::MESHTEXTURE_NORMAL][i] = nullptr;

		Engine::ChangeFileName(szFullPath, L"N", L"S");

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_vecTexture[Engine::MESHTEXTURE_SPECULAR][i])))
			m_vecTexture[Engine::MESHTEXTURE_SPECULAR][i] = nullptr;

		Engine::ChangeFileName(szFullPath, L"S", L"E");

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_vecTexture[Engine::MESHTEXTURE_EMMISIVE][i])))
			m_vecTexture[Engine::MESHTEXTURE_EMMISIVE][i] = nullptr;
	}

	m_pMesh->UnlockVertexBuffer();
	
	return S_OK;
}

void Engine::CStaticMesh::Render_Meshes()
{
	for (_uint i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[Engine::MESHTEXTURE_DIFFUSE][i]);
		m_pMesh->DrawSubset(i);
	}
}

void CStaticMesh::Render_Meshes(LPD3DXEFFECT & pEffect)
{
	for (_uint i = 0; i < m_dwSubsetCnt; ++i)
	{
		pEffect->SetTexture("g_DiffuseTexture", m_vecTexture[Engine::MESHTEXTURE_DIFFUSE][i]);
		pEffect->SetTexture("g_NormalTexture", m_vecTexture[Engine::MESHTEXTURE_NORMAL][i]);
		pEffect->SetTexture("g_SpecularTexture", m_vecTexture[Engine::MESHTEXTURE_SPECULAR][i]);
		pEffect->SetTexture("g_EmmisiveTexture", m_vecTexture[Engine::MESHTEXTURE_EMMISIVE][i]);
		pEffect->CommitChanges();		// ������ ������ ��ҵ��� �����Ͽ� ���ο� ������ ��ҷ� ��ü���ִ� �Լ�
		m_pMesh->DrawSubset(i);
	}
}

void CStaticMesh::Render_Meshes(_ulong dwSubsetNum)
{
	m_pMesh->DrawSubset(dwSubsetNum);
}

CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrFilePath, wstring wstrFileName)
{
	CStaticMesh*	pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Prototype(wstrFilePath, wstrFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CStaticMesh::Clone()
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free()
{
	CMesh::Free();
	for (auto& vecTexture : m_vecTexture)
	{
		for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		{
			if(vecTexture[i] != nullptr)
				Safe_Release(vecTexture[i]);
		}
		Safe_Delete_Array(vecTexture);
	}
	m_vecTexture.clear();

	if (false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);

	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubsetBuffer);
	Safe_Release(m_pMesh);
	Safe_Release(m_pOriMesh);
}


