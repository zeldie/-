#pragma once
#include "afxwin.h"


// CPathFind 대화 상자입니다.

class CPathFindTool : public CDialog
{
	DECLARE_DYNAMIC(CPathFindTool)

public:
	enum RADIO { RADIO_MESH_DYNAMIC,RADIO_MESH_LOBBY, RADIO_MESH_CARTEL, RADIO_MESH_BOSS, RADIO_MESH_RUN, RADIO_MESH_EFFECT_STATIC, RADIO_MESH_EFFECT_DYNAMIC, RADIO_TEXTURE, RADIO_END };

private:
	list<IMAGEPATH*> m_listImagePath;

public:
	CPathFindTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathFindTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFIND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnDropFiles(HDROP hDropInfo);

public:
	void HorizontalScroll();
	
public:
	CListBox m_ListBox;
	CButton m_Radio[RADIO_END];
};
