#pragma once
#include "afxcmn.h"


// CMapTool 대화 상자입니다.
class CTransform;
class CMapTool : public CDialog
{
public:
	enum SETFOCUS {POSX, POSY, POSZ, ROTX, ROTY, ROTZ};
public:
	enum RENDERINGOPTION {WIREFRAME, SOLID};
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void InitTreeCtrl();
	void SearchRecursive(HTREEITEM hParent, CString Child, CString pFilePath);
	virtual BOOL OnInitDialog();

public:
	CTreeCtrl				m_treeMapTexture;
	RENDERINGOPTION			m_eRenderingOption;
	_ulong					m_dwVtxCntX;
	_ulong					m_dwVtxCntZ;
	_ulong					m_dwVtxItv;
	_ulong					m_dwDetail;
	_vec3					m_vecPos;
	_vec3					m_vecRot;
	SETFOCUS				m_eFocusType;
public:
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnBnClickedLoadButton();
	afx_msg void OnBnClickedSceneClearButton();
	afx_msg void OnBnClickedPosResetButton();
	afx_msg void OnBnClickedRotResetButton();
	afx_msg void OnBnClickedResetAllButton();
	afx_msg void OnBnClickedRenderingOptionWire();
	afx_msg void OnBnClickedRenderingOptionSolid();
	afx_msg void OnBnClickedApplyButton();
	afx_msg void OnEnUpdatePosX();
	afx_msg void OnEnUpdatePosY();
	afx_msg void OnEnUpdatePosZ();
	afx_msg void OnEnUpdateRotX();
	afx_msg void OnEnUpdateRotY();
	afx_msg void OnEnUpdateRotZ();
	afx_msg void OnEnSetfocusPosX();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocusPosY();
	afx_msg void OnEnSetfocusPosZ();
	afx_msg void OnEnSetfocusRotX();
	afx_msg void OnEnSetfocusRotY();
	afx_msg void OnEnSetfocusRotZ();
};
