#pragma once
#include "afxcmn.h"



// CMyForm 폼 뷰입니다.

#include "MapTool.h"
class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)
public:
	enum TOOL_TYPE { MAPTOOL, MESHTOOL, NAVIGATIONTOOL, EFFECTTOOL, COLLIDERTOOL, PATHFINDTOOL, CAMTOOL, RUNTOOL, TOOLTYPE_END };
protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CDialog* Get_DialogTool(TOOL_TYPE eToolType);
public:
	CTabCtrl			m_Tab;
	vector<CDialog*>	m_vecDialog;
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	TOOL_TYPE			m_eCurToolType;
public:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
};


