#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"
#include "Texture.h"
BEGIN(Engine)

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh();

public:
	//Getter
	D3DXFRAME_DERIVED*					Get_FrameByName(const char* pFrameName);
	void								GetBoneNames(vector<wstring>& vecBoneNames, D3DXFRAME_DERIVED* pFrame = nullptr);
	CAniCtrl*							GetAniCtrl() { return m_pAniCtrl; }
	__forceinline _double&				Get_Period() { return m_pAniCtrl->Get_AnimationTrackTime(); }
	__forceinline _double&		 	    Get_TrackTime() { return m_pAniCtrl->Get_TrackTime(); }
	list<D3DXMESHCONTAINER_DERIVED*>*	Get_MeshContainerlist() { return &m_MeshContainerList; }
	_ulong								Get_SubsetNum(D3DXMESHCONTAINER_DERIVED* pMeshContainer);
	//Setter
	void								Set_AnimationSet(const _uint& iIndex);
	void								Reset_Animation();

	//General
	_bool							Is_AnimationSetFinish(const _double& dbRevisionTime=0);
	void							Play_AnimationSet(const _double& dTimeDelta, const _double dAccelTIme = 1);
public:
	virtual HRESULT					Ready_Prototype(const wstring& wstrFilePath, const wstring& wstrFileName);
	virtual void					Render_Meshes(void);
	void							Render_Meshes(LPD3DXEFFECT& pEffect);
	void							Render_Meshes_Begin(D3DXMESHCONTAINER_DERIVED* pMeshContainer);
	void							Render_Meshes(D3DXMESHCONTAINER_DERIVED* pMeshContainer, _ulong iSubsetNum);
	void							Render_Meshes_End(D3DXMESHCONTAINER_DERIVED* pMeshContainer);
private:
	void							Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void							SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

private:
	CHierarchyLoader*						m_pLoader;
	CAniCtrl*								m_pAniCtrl;
	D3DXFRAME*								m_pRootFrame;
	list<D3DXMESHCONTAINER_DERIVED*>		m_MeshContainerList;
public:
	static CDynamicMesh*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const wstring& wstrFileName);
	virtual CComponent*				Clone();
	virtual void					Free();
};

END
#endif // DynamicMesh_h__

// �� ������ ��Ƶδ� ����ü

//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;					// ���� �̸��� �����ϴ� ���ڿ�(�ƽ�Ű �ڵ�)
//	D3DXMATRIX              TransformationMatrix;	// ���� �ε� ������ ���� ���� ���� ��� ��
//
//	LPD3DXMESHCONTAINER     pMeshContainer;			// ���������� ����ƽ �޽�
//
//	struct _D3DXFRAME       *pFrameSibling;			// ���� ������ ù ��° �ּ�
//	struct _D3DXFRAME       *pFrameFirstChild;		// �ڽ� ������ ù ��° �ּ�
//} D3DXFRAME, *LPD3DXFRAME;

// �޽��� �����ϴ� ���� ��Ҹ� �����ϴ� ����ü

//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;			// �޽� �����̳��� �̸��� �����ϴ� ���ڿ�(��ǻ� ���� ������� ����)
//
//	D3DXMESHDATA            MeshData;		// �����ϰ��� �ϴ� �޽��� �ɼ��� �����ϴ� ����ü
//
//	LPD3DXMATERIAL          pMaterials;		// ���� ������ ����ϴ� ����ü ������(������ ���� ���� ������ �����ϱ� ���� ����, �ؽ��� �̸�)
//	LPD3DXEFFECTINSTANCE    pEffects;		// �޽� �����̳ʰ� �����ϰ� �ִ� ��Ư�� ����Ʈ ������ �����ϴ� ������(������ �츮�� ����� �� ����)
//	DWORD                   NumMaterials;	// ������ ����
//	DWORD                  *pAdjacency;		// ������ ������ ù��° �ּҸ� �����ϱ� ���� ������
//
//	LPD3DXSKININFO          pSkinInfo;		// ��Ű�� �ִϸ��̼ǿ� ���õ� ���� ����� �����ϴ� �� ��ü
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;	// ���� �޽� �����̳��� �ּҰ��� �����ϴ� ������(�׷��� �츮�� ���� �޽� �����̳��� ���� ������ �����ϰų� Ž���ϴ� ���� ���� ����)
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;

// �޽��� Ÿ���� �����ϴ� ����ü
//typedef struct _D3DXMESHDATA
//{
//	D3DXMESHDATATYPE Type;			// � �޽��� ����� ������ �����ϴ� ����ü
//
//	// current mesh data interface
//	union
//	{
//		LPD3DXMESH              pMesh;			// �츮�� �������� �븻 �޽��� ����Ѵ�
//		LPD3DXPMESH             pPMesh;
//		LPD3DXPATCHMESH         pPatchMesh;
//	};
//} D3DXMESHDATA, *LPD3DXMESHDATA;
