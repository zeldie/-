#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"
#include "Texture.h"
BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	const _vec3*				Get_VtxPos(void) { return m_pVtxPos; }
	_ulong						Get_VtxCnt(void) const { return m_dwVtxCnt; }
	_ulong						Get_Stride(void) const { return m_dwStride; }
	LPD3DXMESH					Get_Mesh() { return m_pMesh; }
	_ulong						Get_SubsetNum() { return m_dwSubsetCnt; }
	LPDIRECT3DTEXTURE9			Get_Texture(Engine::MESHTEXTURE_TYPE eMeshTextureType, _uint iSubsetNum);

public:
	HRESULT						Ready_Prototype(wstring wstrFilePath, wstring wstrFileName);
	void						Render_Meshes(void);
	void						Render_Meshes(LPD3DXEFFECT& pEffect);
	void						Render_Meshes(_ulong dwSubsetNum);
private:
	LPD3DXMESH					m_pMesh;		// �޽��� ��ǥ�ϴ� �� ��ü

	LPD3DXBUFFER				m_pAdjacency;	// �̿��ϴ� ����(������) ������ �����ϱ� ���� �ּҰ�(������)
	LPD3DXBUFFER				m_pSubsetBuffer; // ����� ������ �����ϱ� ���� ������

	D3DXMATERIAL*				m_pSubsets;		// ���� ������ �����ϱ� ���� ����ü ������
	_ulong						m_dwSubsetCnt;	// ����� ���� == �ؽ��� ���� == ������ ����

	vector<LPDIRECT3DTEXTURE9*>	m_vecTexture;

	LPD3DXMESH					m_pOriMesh;		// collider�ʹ� �������, �븻 ������ �����ϱ� ���� ���� �޽�

	_ulong						m_dwVtxCnt;
	_ulong						m_dwStride;	   // ���� �� pos���� �ش��ϴ� �޸� ������ �����ϱ� ���� ����� ����
	_vec3*						m_pVtxPos;

public:
	static	CStaticMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										wstring wstrFilePath,
										wstring wstrFileName);
	virtual CComponent*			Clone(void);
	virtual void				Free(void);
};

END
#endif // StaticMesh_h__
