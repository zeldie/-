#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMeshTool 대화 상자입니다.
#include "Transform.h"
class CMeshTool : public CDialog
{
	DECLARE_DYNAMIC(CMeshTool)
public:
	enum MESHTYPE {STATIC, DYNAMIC, MESHTYPE_END};
	enum FOCUSTYPE {POSX, POSY, POSZ, ROTX, ROTY, ROTZ, SCALEX, SCALEY, SCALEZ, SELFROTATEX, SELFROTATEY, SELFROTATEZ, SECT,FOCUSTYPE_END};
public:
	CMeshTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMeshTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void InitTreeCtrl();
	void SearchRecursive(HTREEITEM hParent, CString Child, CString pFilePath);
	_int SearchSameName(CString cstrname, CString cstrMesyType);
	virtual BOOL OnInitDialog();
	void Set_Transform(Engine::CTransform* pTransform) { m_pCurrentTransform = pTransform; }
public:
	CTreeCtrl	m_treeMesh;
	
	
	_vec3								m_vecPos;
	_vec3								m_vecRot;
	_vec3								m_vecScale;
	_vec3								m_vSelfRotAngle;
	CString								m_cstrListKey; // 저장할때 이름(아마 의미없을듯)
	_uint								m_iSect; // 런게임 메쉬 구간번호

	CListBox							m_ListBoxStaticMesh;
	CListBox							m_ListBoxDynamicMesh;
	_int								m_iStaticMeshNum; // Listbox에 insert할때 숫자
	_int								m_iDynamicMeshNum; // Listbox에 insert할때 숫자

	CString								m_cstrTreeSelectedName; // Tree Control에서 현재 선택한 이름
	CString								m_cstrListBoxSelectedName; // List Box에서 현재 선택한 이름
	CString								m_cstrMeshType; // 내가 누른 것이 StaticMesh인지 DynamicMesh인지 확인

	FOCUSTYPE							m_eFocusType;
	Engine::CTransform*					m_pCurrentTransform;
	map<CString, Engine::CGameObject*>	m_mapMesh[MESHTYPE_END];


public:
	afx_msg void OnTvnSelchangedMeshTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedPosResetButton();
	afx_msg void OnBnClickedRotResetButton();
	afx_msg void OnBnClickedScaleResetButton();
	afx_msg void OnEnUpdatePosX();
	afx_msg void OnEnUpdatePosY();
	afx_msg void OnEnUpdatePosZ();
	afx_msg void OnEnUpdateRotX();
	afx_msg void OnEnUpdateRotY();
	afx_msg void OnEnUpdateRotZ();
	afx_msg void OnEnUpdateScaleX();
	afx_msg void OnEnUpdateScaleY();
	afx_msg void OnEnUpdateScaleZ();
	afx_msg void OnBnClickedResetAllButton();
	afx_msg void OnBnClickedListBoxStaticMeshDelete();
	afx_msg void OnBnClickedListBoxDynamicMeshDelete();
	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnBnClickedLoadButton();
	afx_msg void OnBnClickedSceneClearButton();
	afx_msg void OnBnClickedAddMesh();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocusPosX();
	afx_msg void OnEnSetfocusPosY();
	afx_msg void OnEnSetfocusPosZ();
	afx_msg void OnEnSetfocusRotX();
	afx_msg void OnEnSetfocusRotY();
	afx_msg void OnEnSetfocusRotZ();
	afx_msg void OnEnSetfocusScaleX();
	afx_msg void OnEnSetfocusScaleY();
	afx_msg void OnEnSetfocusScaleZ();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLbnSelchangeStaticMeshList();
	afx_msg void OnLbnSelchangeDynamicMeshList();
	afx_msg void OnEnSetfocusSelfRotationX();
	afx_msg void OnEnSetfocusSelfRotationY();
	afx_msg void OnEnSetfocusSelfRotationZ();
	afx_msg void OnEnUpdateSelfRotateX();
	afx_msg void OnEnUpdateSelfRotateY();
	afx_msg void OnEnUpdateSelfRotateZ();
	afx_msg void OnBnClickedRunGameSaveButton();
	afx_msg void OnBnClickedRunGameLoadButton();
	afx_msg void OnEnSetfocusSect();
};
