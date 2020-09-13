#pragma once
#include "afxcmn.h"


// CNavigationTool ��ȭ �����Դϴ�.
#include "ToolNaviMeshObject.h"
#include "ToolNaviMeshObjectVertex.h"

class CNavigationTool : public CDialog
{
	DECLARE_DYNAMIC(CNavigationTool)
public:
	enum NAVIMODE {ADD, EDIT, MODE_END};
	enum FOCUSTYPE {POSX, POSY, POSZ, FOCUS_END};
public:
	CNavigationTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNavigationTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVIGATIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl									m_treeNavigation;
	NAVIMODE									m_eNaviMode;
	Engine::NAVIMESHTYPE						m_eNaviType;
	FOCUSTYPE									m_eFocusType;
	_vec3										m_vecPos;
	_uint										m_iMeshNum;
	vector<CToolNaviMeshObject*>				m_vecNaviMesh;

	CString										m_cstrTreeSelectedName; // Tree Control���� ���� ������ �̸�

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
