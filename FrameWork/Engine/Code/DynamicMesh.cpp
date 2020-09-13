#include "DynamicMesh.h"

USING(Engine)

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_pRootFrame(nullptr)
	, m_pLoader(nullptr)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
	:CMesh(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshContainerList(rhs.m_MeshContainerList)
{
	m_pAniCtrl = CAniCtrl::Clone(*rhs.m_pAniCtrl);
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

HRESULT Engine::CDynamicMesh::Ready_Prototype(const wstring& wstrFilePath, const wstring& wstrFileName)
{
	_tchar	szFullPath[256] = L"";

	lstrcpy(szFullPath, wstrFilePath.c_str());
	lstrcat(szFullPath, wstrFileName.c_str());

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, wstrFilePath);
	if (m_pLoader == nullptr)
		return E_FAIL;

	LPD3DXANIMATIONCONTROLLER	 pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath,			// 최종 경로
		D3DXMESH_MANAGED,	// 메모리 풀
		m_pGraphicDev,		// 작업을 수행하는 대표 객체
		m_pLoader,			// hierarchyLoader 포인터
		NULL,
		&m_pRootFrame,		// 최초의 뼈 정보를 보관하긴 위한 포인터
		&pAniCtrl)))		// 애니메이션 컨트롤러의 주소
		return E_FAIL;


	m_pAniCtrl = CAniCtrl::Create(pAniCtrl);
	if (m_pAniCtrl == nullptr)
		return E_FAIL;


	Safe_Release(pAniCtrl);

	_matrix		matTemp;

	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(270.f)));

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void Engine::CDynamicMesh::Render_Meshes()
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i]
				* (*pMeshContainer->ppFrameCombinedTransformationMatrix[i]);
		}

		void*		pDestVtx = nullptr;
		void*		pSourVtx = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pDestVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pSourVtx);

		// 소프트웨어 스키닝 수행하는 함수(스키닝 뿐만 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경을 동시에 수행해주기도 한다)

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix,  // 최종 뼈의 변환 상태 행렬
			NULL,							// 원래 상태로 되돌리기 위한 상태 행렬(본래 싹 다 역행렬을 구해줘서 넣어야 하지만 안넣어줘도 상관이 없다)
			pDestVtx,						// 변하지 않는 원본 메쉬의 정점 정보
			pSourVtx);						// 변환된 메쉬의 정점 정보

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, pMeshContainer->ppDiffuseTexture[i]);
			//m_pGraphicDev->SetTexture(0, pMeshContainer->ppNormalTexture[i]);
			//m_pGraphicDev->SetTexture(0, pMeshContainer->ppSpecularTexture[i]);
			//m_pGraphicDev->SetTexture(0, pMeshContainer->ppEmmisiveTexture[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

void CDynamicMesh::Render_Meshes(LPD3DXEFFECT & pEffect)
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i]
				* (*pMeshContainer->ppFrameCombinedTransformationMatrix[i]);
		}

		void*		pDestVtx = nullptr;
		void*		pSourVtx = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pDestVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pSourVtx);

		// 소프트웨어 스키닝 수행하는 함수(스키닝 뿐만 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경ohie을 동시에 수행해주기도 한다)

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix,  // 최종 뼈의 변환 상태 행렬
			NULL,							// 원래 상태로 되돌리기 위한 상태 행렬(본래 싹 다 역행렬을 구해줘서 넣어야 하지만 안넣어줘도 상관이 없다)
			pDestVtx,						// 변하지 않는 원본 메쉬의 정점 정보
			pSourVtx);						// 변환된 메쉬의 정점 정보

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			pEffect->SetTexture("g_DiffuseTexture", pMeshContainer->ppDiffuseTexture[i]);
			pEffect->SetTexture("g_NormalTexture", pMeshContainer->ppNormalTexture[i]);
			pEffect->SetTexture("g_SpecularTexture", pMeshContainer->ppSpecularTexture[i]);
			pEffect->SetTexture("g_EmmisiveTexture", pMeshContainer->ppEmmisiveTexture[i]);
			pEffect->CommitChanges();
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

void CDynamicMesh::Render_Meshes_Begin(D3DXMESHCONTAINER_DERIVED * pMeshContainer)
{
	for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
	{
		pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i]
			* (*pMeshContainer->ppFrameCombinedTransformationMatrix[i]);
	}

	void*		pDestVtx = nullptr;
	void*		pSourVtx = nullptr;

	pMeshContainer->pOriMesh->LockVertexBuffer(0, &pDestVtx);
	pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pSourVtx);

	// 소프트웨어 스키닝 수행하는 함수(스키닝 뿐만 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경ohie을 동시에 수행해주기도 한다)

	pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix,  // 최종 뼈의 변환 상태 행렬
		NULL,							// 원래 상태로 되돌리기 위한 상태 행렬(본래 싹 다 역행렬을 구해줘서 넣어야 하지만 안넣어줘도 상관이 없다)
		pDestVtx,						// 변하지 않는 원본 메쉬의 정점 정보
		pSourVtx);						// 변환된 메쉬의 정점 정보
}

void CDynamicMesh::Render_Meshes(D3DXMESHCONTAINER_DERIVED* pMeshContainer, _ulong iSubsetNum)
{
	pMeshContainer->MeshData.pMesh->DrawSubset(iSubsetNum);
}

void CDynamicMesh::Render_Meshes_End(D3DXMESHCONTAINER_DERIVED * pMeshContainer)
{
	pMeshContainer->pOriMesh->UnlockVertexBuffer();
	pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
}

D3DXFRAME_DERIVED* Engine::CDynamicMesh::Get_FrameByName(const char* pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

_bool Engine::CDynamicMesh::Is_AnimationSetFinish(const _double& dbRevisionTime)
{
	//CAniCtrl* a = m_pAniCtrl;

	return m_pAniCtrl->Is_AnimationSetFinish(dbRevisionTime);
}

_ulong CDynamicMesh::Get_SubsetNum(D3DXMESHCONTAINER_DERIVED * pMeshContainer)
{
	return pMeshContainer->NumMaterials;
}

void CDynamicMesh::Set_AnimationSet(const _uint & iIndex)
{
	m_pAniCtrl->Set_AnimationSet(iIndex);
}

void CDynamicMesh::Reset_Animation()
{
	m_pAniCtrl->Reset_Animation();
}

void CDynamicMesh::Play_AnimationSet(const _double & dTimeDelta, const _double dAccelTIme)
{
	m_pAniCtrl->Play_AnimationSet(dTimeDelta * dAccelTIme);

	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(270.f)));
}

void CDynamicMesh::GetBoneNames(vector<wstring>& vecBoneNames, D3DXFRAME_DERIVED* pFrame)
{
	if (pFrame == nullptr)
		pFrame = (D3DXFRAME_DERIVED*)m_pRootFrame;
	if (pFrame->Name != nullptr)
	{
		TCHAR szWchar[MAX_STR] = L"";
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pFrame->Name, _int(strlen(pFrame->Name)), szWchar, 64);
		wstring wstrConvertName = szWchar;
		vecBoneNames.emplace_back(wstrConvertName);
	}

	if (nullptr != pFrame->pFrameSibling)
		GetBoneNames(vecBoneNames, (D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		GetBoneNames(vecBoneNames, (D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
}

void Engine::CDynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame,
	const _matrix* pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);

}

CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const  wstring& wstrFileName)
{
	CDynamicMesh*	pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Prototype(wstrFilePath, wstrFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CDynamicMesh::Clone()
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Free(void)
{
	CMesh::Free();

	Safe_Release(m_pAniCtrl);

	if (false == m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}
	m_MeshContainerList.clear();
}

void Engine::CDynamicMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			const char*		pBoneName = pDerivedMeshContainer->pSkinInfo->GetBoneName(i);
			D3DXFRAME_DERIVED* pBone = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
			pDerivedMeshContainer->ppFrameCombinedTransformationMatrix[i] = &pBone->CombinedTransformationMatrix;
		}
		m_MeshContainerList.push_back(pDerivedMeshContainer);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
}

