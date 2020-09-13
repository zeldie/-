// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "ToolView.h"

#include "MapTool.h"
#include "MeshTool.h"
#include "NavigationTool.h"
#include "EffectTool.h"
#include "ColliderTool.h"
#include "PathFindTool.h"
#include "CameraTool.h"
#include "RunTool.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm*	g_pMyForm;

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM),
	m_eCurToolType(MAPTOOL)
{

}

CMyForm::~CMyForm()
{
	for (auto& Dialog : m_vecDialog)
		Engine::Safe_Delete(Dialog);
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

CDialog * CMyForm::Get_DialogTool(TOOL_TYPE eToolType)
{
	switch (eToolType)
	{
	case CMyForm::MAPTOOL:
		return dynamic_cast<CMapTool*>(m_vecDialog[eToolType]);
		break;
	case CMyForm::MESHTOOL:
		return dynamic_cast<CMeshTool*>(m_vecDialog[eToolType]);
		break;
	case CMyForm::NAVIGATIONTOOL:
		return dynamic_cast<CNavigationTool*>(m_vecDialog[eToolType]);
		break;
	case CMyForm::EFFECTTOOL:
		return dynamic_cast<CEffectTool*>(m_vecDialog[eToolType]);
		break;
	case CMyForm::COLLIDERTOOL:
		return dynamic_cast<CColliderTool*>(m_vecDialog[eToolType]);
		break;
	case CMyForm::PATHFINDTOOL:
		return dynamic_cast<CPathFindTool*>(m_vecDialog[eToolType]);
		break;
	case CMyForm::CAMTOOL:
		return dynamic_cast<CCameraTool*>(m_vecDialog[eToolType]);
		break;
	case CMyForm::RUNTOOL:
		return dynamic_cast<CRunTool*>(m_vecDialog[eToolType]);
		break;
	case CMyForm::TOOLTYPE_END:
		break;
	default:
		break;
	}
	return nullptr;
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIndex = m_Tab.GetCurSel();
	if (nIndex > TOOLTYPE_END)
		return;
	for (auto& Dialog : m_vecDialog)
		Dialog->ShowWindow(SW_HIDE);
	m_vecDialog[nIndex]->ShowWindow(SW_SHOW);
	m_eCurToolType = static_cast<TOOL_TYPE>(nIndex);
	*pResult = 0;
}


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_pMyForm = this;
	CString cTabOne = _T("Map");
	CString cTabTwo = _T("Mesh");
	CString cTabThree = _T("Navigation");
	CString cTabFour = _T("Effect");
	CString cTabFive = _T("Collider");
	CString cTabSix = _T("PathFind");
	CString cTabSeven = _T("Camera");
	CString cTabEight = _T("Run");

	m_Tab.InsertItem(0, cTabOne);
	m_Tab.InsertItem(1, cTabTwo);
	m_Tab.InsertItem(2, cTabThree);
	m_Tab.InsertItem(3, cTabFour);
	m_Tab.InsertItem(4, cTabFive);
	m_Tab.InsertItem(5, cTabSix);
	m_Tab.InsertItem(6, cTabSeven);
	m_Tab.InsertItem(7, cTabEight);

	CRect rect;
	m_Tab.SetWindowPos(nullptr, 0, 0, TOOL_FRAMECX, TOOL_WINCY -10 , SWP_NOZORDER);
	m_Tab.GetClientRect(&rect);

	CMapTool* pMaptool = new CMapTool;
	m_vecDialog.push_back(pMaptool);
	m_vecDialog[MAPTOOL]->Create(IDD_MAPTOOL, &m_Tab);
	m_vecDialog[MAPTOOL]->SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	CMeshTool* pMeshTool = new CMeshTool;
	m_vecDialog.push_back(pMeshTool);
	m_vecDialog[MESHTOOL]->Create(IDD_MESHTOOL, &m_Tab);	
	m_vecDialog[MESHTOOL]->SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	CNavigationTool* pNavigationTool = new CNavigationTool;
	m_vecDialog.push_back(pNavigationTool);
	m_vecDialog[NAVIGATIONTOOL]->Create(IDD_NAVIGATIONTOOL, &m_Tab);
	m_vecDialog[NAVIGATIONTOOL]->SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	CEffectTool* pEffectTool = new CEffectTool;
	m_vecDialog.push_back(pEffectTool);
	m_vecDialog[EFFECTTOOL]->Create(IDD_EFFECTTOOL, &m_Tab);
	m_vecDialog[EFFECTTOOL]->SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	CColliderTool* pColliderTool = new CColliderTool;
	m_vecDialog.push_back(pColliderTool);
	m_vecDialog[COLLIDERTOOL]->Create(IDD_COLLIDERTOOL, &m_Tab);
	m_vecDialog[COLLIDERTOOL]->SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	CPathFindTool* pPathFindTool = new CPathFindTool;
	m_vecDialog.push_back(pPathFindTool);
	m_vecDialog[PATHFINDTOOL]->Create(IDD_PATHFINDTOOL, &m_Tab);
	m_vecDialog[PATHFINDTOOL]->SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	CCameraTool* pCameraTool = new CCameraTool;
	m_vecDialog.push_back(pCameraTool);
	m_vecDialog[CAMTOOL]->Create(IDD_CAMTOOL, &m_Tab);
	m_vecDialog[CAMTOOL]->SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	CRunTool* pRunTool = new CRunTool;
	m_vecDialog.push_back(pRunTool);
	m_vecDialog[RUNTOOL]->Create(IDD_RUNTOOL, &m_Tab);
	m_vecDialog[RUNTOOL]->SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	for (auto& Dialog : m_vecDialog)
		Dialog->ShowWindow(SW_HIDE);
	m_vecDialog[MAPTOOL]->ShowWindow(SW_SHOW);
}