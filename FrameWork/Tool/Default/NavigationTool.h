#pragma once
#include "afxcmn.h"


// CNavigationTool 대화 상자입니다.
#include "ToolNaviMeshObject.h"
#include "ToolNaviMeshObjectVertex.h"

class CNavigationTool : public CDialog
{
	DECLARE_DYNAMIC(CNavigationTool)
public:
	enum NAVIMODE {ADD, EDIT, MODE_END};
	enum FOCUSTYPE {POSX, POSY, POSZ, FOCUS_END};
public:
	CNavigationTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNavigationTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVIGATIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl									m_treeNavigation;
	NAVIMODE									m_eNaviMode;
	Engine::NAVIMESHTYPE						m_eNaviType;
	FOCUSTYPE									m_eFocusType;
	_vec3										m_vecPos;
	_uint										m_iMeshNum;
	vector<CToolNaviMeshObject*>				m_vecNaviMesh;

	CString										m_cstrTreeSelectedName; // Tree Control에서 현재 선택한 이름

	HTREEITEM									m_ParentItem;
	HTREEITEM									m_ChildItem;
public:
	CToolNaviMeshObject*	Get_LastMeshObject();
	HTREEITEM				Get_LastTreeItem();
public:
	afx_msg void OnTvnSelchangedNavigationTree(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedEdit();
	afx_msg void OnEnUpdatePosX();
	afx_msg void OnEnUpdatePosY();
	afx_msg void OnEnUpdatePosZ();
	afx_msg void OnEnSetfocusPosX();
	afx_msg void OnEnSetfocusPosY();
	afx_msg void OnEnSetfocusPosZ();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedResetButton();
	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnBnClickedLoadButton();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedSceneClearButton();
	afx_msg void OnBnClickedNormal();
	afx_msg void OnBnClickedTrigger();
	afx_msg void OnBnClickedEvent();
	int m_iOption;
};
