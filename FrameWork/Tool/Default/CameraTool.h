#pragma once
#include "afxwin.h"


// CCameraTool ��ȭ �����Դϴ�.

class CCameraTool : public CDialog
{
	DECLARE_DYNAMIC(CCameraTool)

public:
	enum MODE { HEAD, TAIL, SPLINE, MODE_END };

public:
	CCameraTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCameraTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
