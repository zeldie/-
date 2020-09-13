#pragma once


// CRunTool ��ȭ �����Դϴ�.
#include "ToolRunGameLine.h"
#include "ToolNaviMeshObjectVertex.h"
#include "afxcmn.h"

class CRunTool : public CDialog
{
	DECLARE_DYNAMIC(CRunTool)
public:
	enum RUNMODE { ADD, EDIT, MODE_END };
	enum FOCUSTYPE { POSX, POSY, POSZ, FOCUS_END };
	enum LINEOPTION { NORMAL, SPEED, SLOW, FALL, OPTION_END };
public:
	CRunTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRunTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RUNTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CToolRunGameLine*	Get_LastMeshObject();

public:
	RUNMODE									m_eRunMode;
	FOCUSTYPE								m_eFocusType;
	LINEOPTION								m_eOption; 
	//_int									m_iOption;
	vector<CToolRunGameLine*>				m_vecRunLine;
	_vec3									m_vPos;
	_uint									m_iLineNum;

	CString									m_cstrTreeSelectedName; // Tree Control���� ���� ������ �̸�

	HTREEITEM								m_ParentItem;
	HTREEITEM								m_ChildItem;
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_treeCtrl;
	afx_msg void OnBnClickedButtonSceneClear();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnUpdateEditPosx();
	afx_msg void OnEnSetfocusEditPosx();
	afx_msg void OnEnSetfocusEditPosy();
	afx_msg void OnEnSetfocusEditPosz();
	afx_msg void OnEnUpdateEditPosy();
	afx_msg void OnEnUpdateEditPosz();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
