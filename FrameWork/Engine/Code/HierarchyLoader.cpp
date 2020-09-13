#include "HierarchyLoader.h"

USING(Engine)

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
	: m_pGraphicDev(pGraphicDev)
	, m_wstrPath(wstrPath)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CHierarchyLoader::~CHierarchyLoader()
{
	
}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	//���� ������ִ� �Լ�
	//���� ����� �ʰ�ȭ
	D3DXFRAME_DERIVED*		pDerivedFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pDerivedFrame, sizeof(D3DXFRAME_DERIVED));

	//�����ٰ� �̸��� �־���
	Allocate_Name(&pDerivedFrame->Name, Name);
	//���� ����� �ʱ�ȭ���� ������ ����� �����ϴ°����� �־���(�ʱ�ȭ�ؼ�)
	pDerivedFrame->CombinedTransformationMatrix = *D3DXMatrixIdentity(&pDerivedFrame->TransformationMatrix);

	*ppNewFrame = pDerivedFrame;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name,
															CONST D3DXMESHDATA *pMeshData,
															CONST D3DXMATERIAL *pMaterials,
															CONST D3DXEFFECTINSTANCE *pEffectInstances,
															DWORD NumMaterials,
															CONST DWORD *pAdjacency, 
															LPD3DXSKININFO pSkinInfo,
															LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	//�Ž� �����̳� ���� ����, �ʱ�ȭ
	D3DXMESHCONTAINER_DERIVED*		pDerivedContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pDerivedContainer, sizeof(D3DXMESHCONTAINER_DERIVED));
	//�̸� ����
	Allocate_Name(&pDerivedContainer->Name, Name);

	pDerivedContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH pMesh = pMeshData->pMesh;

	_ulong	dwNumFaces = pMesh->GetNumFaces();	// �޽��� ���� �������� ������ ��ȯ�ϴ� �Լ�

	pDerivedContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pDerivedContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	_ulong	dwFVF = pMesh->GetFVF();

	// �޽����� ���� FVF �� �븻�� ���� ���
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(),
			dwFVF | D3DFVF_NORMAL,
			m_pGraphicDev,
			&pDerivedContainer->MeshData.pMesh);

		// ������ ���� ������ �������� �븻 ������ ������ִ� �Լ�
		D3DXComputeNormals(pDerivedContainer->MeshData.pMesh, pDerivedContainer->pAdjacency);
	}
	else
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(),
			dwFVF,
			m_pGraphicDev,
			&pDerivedContainer->MeshData.pMesh);
	}

	pDerivedContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pDerivedContainer->pMaterials = new D3DXMATERIAL[pDerivedContainer->NumMaterials];
	ZeroMemory(pDerivedContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedContainer->NumMaterials);

	pDerivedContainer->ppDiffuseTexture = new LPDIRECT3DTEXTURE9[pDerivedContainer->NumMaterials];
	ZeroMemory(pDerivedContainer->ppDiffuseTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedContainer->NumMaterials);

	pDerivedContainer->ppNormalTexture = new LPDIRECT3DTEXTURE9[pDerivedContainer->NumMaterials];
	ZeroMemory(pDerivedContainer->ppDiffuseTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedContainer->NumMaterials);

	pDerivedContainer->ppSpecularTexture = new LPDIRECT3DTEXTURE9[pDerivedContainer->NumMaterials];
	ZeroMemory(pDerivedContainer->ppDiffuseTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedContainer->NumMaterials);

	pDerivedContainer->ppEmmisiveTexture = new LPDIRECT3DTEXTURE9[pDerivedContainer->NumMaterials];
	ZeroMemory(pDerivedContainer->ppDiffuseTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedContainer->NumMaterials);




	if (0 != NumMaterials)
	{
		memcpy(pDerivedContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedContainer->NumMaterials);

		

		for (_ulong i = 0; i < pDerivedContainer->NumMaterials; ++i)
		{
			
			_tchar szFullPath[256] = L"";
			_tchar	szFileName[256] = L"";
			lstrcpy(szFullPath, m_wstrPath.c_str());

			MultiByteToWideChar(CP_ACP,
				0,
				pDerivedContainer->pMaterials[i].pTextureFilename,
				_int(strlen(pDerivedContainer->pMaterials[i].pTextureFilename)),
				szFileName,
				256);

			lstrcat(szFullPath, szFileName);

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedContainer->ppDiffuseTexture[i])))
				pDerivedContainer->ppDiffuseTexture[i] = nullptr;

			Engine::ChangeFileName(szFullPath, L"D", L"N");

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedContainer->ppNormalTexture[i])))
				pDerivedContainer->ppNormalTexture[i] = nullptr;

			Engine::ChangeFileName(szFullPath, L"N", L"S");

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedContainer->ppSpecularTexture[i])))
				pDerivedContainer->ppSpecularTexture[i] = nullptr;

			Engine::ChangeFileName(szFullPath, L"S", L"E");

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedContainer->ppEmmisiveTexture[i])))
				pDerivedContainer->ppEmmisiveTexture[i] = nullptr;
		}
	}

	else
	{
		pDerivedContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedContainer->pMaterials[0].MatD3D.Power = 0.f;

		pDerivedContainer->ppDiffuseTexture[0] = nullptr;
		pDerivedContainer->ppNormalTexture[0] = nullptr;
		pDerivedContainer->ppSpecularTexture[0] = nullptr;
		pDerivedContainer->ppEmmisiveTexture[0] = nullptr;
	}

	if (nullptr == pSkinInfo)
		return S_OK;

	pDerivedContainer->pSkinInfo = pSkinInfo;
	pDerivedContainer->pSkinInfo->AddRef();

	pDerivedContainer->MeshData.pMesh->CloneMeshFVF(pDerivedContainer->MeshData.pMesh->GetOptions(), 
													pDerivedContainer->MeshData.pMesh->GetFVF(), 
													m_pGraphicDev, 
													&pDerivedContainer->pOriMesh);
	// ���� ������ ����
	pDerivedContainer->dwNumBones = pDerivedContainer->pSkinInfo->GetNumBones();

	pDerivedContainer->pFrameOffsetMatrix = new _matrix[pDerivedContainer->dwNumBones];
	ZeroMemory(pDerivedContainer->pFrameOffsetMatrix, sizeof(_matrix) * pDerivedContainer->dwNumBones);

	pDerivedContainer->ppFrameCombinedTransformationMatrix = new _matrix*[pDerivedContainer->dwNumBones];
	ZeroMemory(pDerivedContainer->ppFrameCombinedTransformationMatrix, sizeof(_matrix*) * pDerivedContainer->dwNumBones);

	pDerivedContainer->pRenderingMatrix = new _matrix[pDerivedContainer->dwNumBones];
	ZeroMemory(pDerivedContainer->pRenderingMatrix, sizeof(_matrix) * pDerivedContainer->dwNumBones);

	for (_ulong i = 0; i < pDerivedContainer->dwNumBones; ++i)
		pDerivedContainer->pFrameOffsetMatrix[i] = *pDerivedContainer->pSkinInfo->GetBoneOffsetMatrix(i);	// �޽��� �׸��� ���� ������� �ε��� ���� �Ű��� �ִ�. �̷� ������ ��ȸ�ϸ鼭 ��� ������ ���ʴ�� ���´�.
	
	*ppNewMeshContainer = pDerivedContainer;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Safe_Delete(pFrameToFree);
	
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*	pDerivedContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_ulong i = 0; i < pDerivedContainer->NumMaterials; ++i)
	{
		Safe_Release(pDerivedContainer->ppDiffuseTexture[i]);
		Safe_Release(pDerivedContainer->ppNormalTexture[i]);
		Safe_Release(pDerivedContainer->ppSpecularTexture[i]);
		Safe_Release(pDerivedContainer->ppEmmisiveTexture[i]);
	}

	Safe_Delete_Array(pDerivedContainer->ppDiffuseTexture);
	Safe_Delete_Array(pDerivedContainer->ppNormalTexture);
	Safe_Delete_Array(pDerivedContainer->ppSpecularTexture);
	Safe_Delete_Array(pDerivedContainer->ppEmmisiveTexture);
	Safe_Delete_Array(pDerivedContainer->Name);
	Safe_Delete_Array(pDerivedContainer->pAdjacency);
	Safe_Delete_Array(pDerivedContainer->pMaterials);

	Safe_Delete_Array(pDerivedContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pDerivedContainer->ppFrameCombinedTransformationMatrix);
	Safe_Delete_Array(pDerivedContainer->pRenderingMatrix);

	Safe_Release(pDerivedContainer->pSkinInfo);
	Safe_Release(pDerivedContainer->MeshData.pMesh);
	Safe_Release(pDerivedContainer->pOriMesh);

	Safe_Delete(pDerivedContainer);

	return S_OK;
}

void Engine::CHierarchyLoader::Allocate_Name(char** ppName, const char* pFrameName)
{
	if (nullptr == pFrameName)
		return;

	_uint	iLength = _uint(strlen(pFrameName));

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);

}

CHierarchyLoader* Engine::CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
{
	return  new CHierarchyLoader(pGraphicDev, wstrPath);
}

_ulong Engine::CHierarchyLoader::Release(void)
{
	
	Safe_Release(m_pGraphicDev);
	delete this;

	return 0;
}

