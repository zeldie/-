#pragma once

#include "ToolGameObject.h"
#include "TriCol.h"
#include "ToolNaviMeshObjectVertex.h"
class CToolNaviMeshObject : public CToolGameObject
{
private:
	explicit CToolNaviMeshObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolNaviMeshObject();

private:
	// CToolGameObject��(��) ���� ��ӵ�
	virtual HRESULT						Ready_GameObject(Engine::NAVIMESHTYPE eNaviMeshType);
	virtual _int						Update_GameObject(const _double& dTimeDelta) override;
	virtual _int						LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void						Render_Geometry(const _double& dTimeDelta) override;

public:
	_uint								Get_VertexNum();
	vector<CToolNaviMeshObjectVertex*>& Get_vecVertex() { return m_vecVertex; }
	_bool								Get_Sorting() { return m_bSorting; }
	Engine::NAVIMESHTYPE				Get_NaviMeshType() { return m_eNaviMeshType; }
	_uint								Get_NaviMeshOption() { return m_iOption; }

	void								Set_BufferPos();
	void								Set_Color();
	void								Set_Sorting(_bool bSorting) { m_bSorting = bSorting; }
	void								Set_Pick(_bool bPick) { m_bIsPick = bPick; }
	void								Set_NaviMeshOption(_uint iOption) { m_iOption = iOption; }

	void								Sorting();
	void								Add_Vertex(CToolNaviMeshObjectVertex* MeshObjectVertex);
private:
	HRESULT								Clone_Component();

private:
	Engine::CTriCol*					m_pBufferCom;
	Engine::NAVIMESHTYPE				m_eNaviMeshType;
	_uint								m_iNum; // ���° �׺�Ž�����
	_ulong								m_dwColor;
	_bool								m_bSorting; // Ŭ�󿡼� sorting �ڵ�
	vector<CToolNaviMeshObjectVertex*>	m_vecVertex; // �ϳ��� �׺�Ž��� 3���� ���ؽ� ����
	_bool								m_bIsPick; // �������� ���� �ٲٱ�
	_uint								m_iOption; // �׺�޽� �ɼ�
public:
	static CToolNaviMeshObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::NAVIMESHTYPE eNaviMeshType);
	virtual void	Free(void);
};

