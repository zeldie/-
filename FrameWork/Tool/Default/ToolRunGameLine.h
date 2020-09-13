#pragma once
#include "ToolGameObject.h"
#include "TriCol.h"
#include "ToolNaviMeshObjectVertex.h"

class CToolRunGameLine : public CToolGameObject
{
	//typedef struct tagLineInfo
	//{
	//	CToolNaviMeshObjectVertex*	pStartPos;	// 클라에선 벡터로
	//	CToolNaviMeshObjectVertex*	pEndPos;	// 클라에선 벡터로
	//	_uint						iOption;	// 클라에선 이넘으로
	//	_double						dProgressValueStart;
	//	_double						dProgressValueEbd;
	//}LINE_INFO;
private:
	explicit CToolRunGameLine(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolRunGameLine();

private:
	virtual HRESULT				Ready_GameObject(_uint iOption);
	virtual _int				Update_GameObject(const _double& dTimeDelta) override;
	virtual _int				LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void				Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT						Clone_Component();

public:
	void						Add_Vertex(CToolNaviMeshObjectVertex* MeshObjectVertex);


	_uint									Get_VertexNum();
	vector<CToolNaviMeshObjectVertex*>&		Get_vecVertex() { return m_vecVertex; }
	_uint									Get_LineOption() { return m_iOption; }


	void									Set_Pick(_bool bPick) { m_bIsPick = bPick; }
	void									Set_LineOption(_uint iOption) { m_iOption = iOption; }

private:
	//vector<LINE_INFO>	m_vecVertex;

	Engine::CTriCol*					m_pBufferCom;
	vector<CToolNaviMeshObjectVertex*>	m_vecVertex;
	_uint								m_iOption;
	_double								m_dProgressValueStart;
	_double								m_dProgressValueEbd;

	_bool								m_bIsPick; // 눌렀을때 색깔 바꾸기

public:
	static CToolRunGameLine* Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iOption);
	virtual void	Free(void);

};

