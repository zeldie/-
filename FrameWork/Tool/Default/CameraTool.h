#pragma once
#include "afxwin.h"


// CCameraTool 대화 상자입니다.

class CCameraTool : public CDialog
{
	DECLARE_DYNAMIC(CCameraTool)

public:
	enum MODE { HEAD, TAIL, SPLINE, MODE_END };

public:
	CCameraTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCameraTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();


public:
	void Create_Eye();
	void Set_Spline();

	void Render_Spline(const _double& dTimeDelta);

public:
	// // Eye Section
	CListBox m_LstEyeSection;
	// At Section
	CListBox m_LstAtSection;
	// Eye Node
	CListBox m_LstEyeNode;
	// At Node
	CListBox m_LstAtNode;

	vector<Engine::CAM_INFO*> m_vecEye;
	vector<Engine::CAM_INFO*> m_vecAt;

	MODE m_eMode;

	LPD3DXLINE				m_pD3DXLine;

};
