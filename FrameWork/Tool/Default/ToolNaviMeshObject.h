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
	// CToolGameObject을(를) 통해 상속됨
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
	_uint								m_iNum; // 몇번째 네비매쉬인지
	_ulong								m_dwColor;
	_bool								m_bSorting; // 클라에서 sorting 자동
	vector<CToolNaviMeshObjectVertex*>	m_vecVertex; // 하나의 네비매쉬는 3개의 버텍스 가짐
	_bool								m_bIsPick; // 눌렀을때 색깔 바꾸기
	_uint								m_iOption; // 네비메쉬 옵션
public:
	static CToolNaviMeshObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::NAVIMESHTYPE eNaviMeshType);
	virtual void	Free(void);
};

