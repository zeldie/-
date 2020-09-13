// CameraTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "CameraTool.h"
#include "afxdialogex.h"

#include "ToolView.h"

CCameraTool* g_pCamTool;


// CCameraTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCameraTool, CDialog)

CCameraTool::CCameraTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CAMTOOL, pParent)
{
}

CCameraTool::~CCameraTool()
{
	//Engine::Safe_Release(m_pD3DXLine);
//	delete m_pD3DXLine;
	/*for (auto& iter : m_vecEye)
		m_vecEye.erase(iter);*/

	//auto& iter_begin = m_vecEye.begin();
	//auto& iter_end = m_vecEye.end();

	//for (; iter_begin != iter_end;)
	//{
	//	m_vecEye.erase(iter_begin);
	//	++iter_begin;
	//}

	//m_vecEye.clear();
	//m_vecAt.clear();
}

void CCameraTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_LstEyeSection);
	DDX_Control(pDX, IDC_LIST3, m_LstAtSection);
	DDX_Control(pDX, IDC_LIST5, m_LstEyeNode);
	DDX_Control(pDX, IDC_LIST6, m_LstAtNode);
}


BEGIN_MESSAGE_MAP(CCameraTool, CDialog)
END_MESSAGE_MAP()


// CCameraTool 메시지 처리기입니다.


BOOL CCameraTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	g_pCamTool = this;
	m_eMode = HEAD;

	//if (FAILED(D3DXCreateLine(g_pToolView->Get_GraphicDev(), &m_pD3DXLine)))
		//return false;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCameraTool::Create_Eye()
{
	Engine::CAM_INFO* pEye = new Engine::CAM_INFO;
	m_vecEye.push_back(pEye);
}

void CCameraTool::Set_Spline()
{
	m_vecEye.back()->vSpline2 = m_vecEye.back()->vSpline1;	
}

void CCameraTool::Render_Spline(const _double & dTimeDelta)
{
	//if (m_vecEye.empty())
	//	return;

	//_matrix matView, matProj;

	//g_pToolView->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	//g_pToolView->Get_GraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);

	//_vec3 vPoint[10];
	//_float fWeight = 0.f;

	//for (_uint Eye = 0; Eye < m_vecEye.size(); ++Eye)
	//{
	//	for (_uint i = 0; i < 10; ++i)
	//	{
	//		D3DXVec3CatmullRom(&vPoint[i], &(*m_vecEye[Eye]).vSpline1, &(*m_vecEye[Eye]).vHead, &(*m_vecEye[Eye]).vTail, &(*m_vecEye[Eye]).vSpline2, fWeight);
	//		fWeight += 0.1f;
	//	}

	//	//D3DXVec3CatmullRom(&vPoint[10], &(*m_vecEye[Eye]).vSpline1, &(*m_vecEye[Eye]).vHead, &(*m_vecEye[Eye]).vTail, &(*m_vecEye[Eye]).vSpline2, 0.f);

	//	for (_ulong i = 0; i < 10; ++i)
	//	{
	//		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
	//		if (vPoint[i].z <= 0.1f)
	//			vPoint[i].z = 0.1f;

	//		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	//	}

	//m_pD3DXLine->SetWidth(3.f);	//라인의 굵기 결정

	//g_pToolView->Get_GraphicDev()->EndScene();
	//g_pToolView->Get_GraphicDev()->BeginScene();

	//m_pD3DXLine->Begin();

	//_matrix		matTemp;
	//m_pD3DXLine->DrawTransform(vPoint, 10, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	//m_pD3DXLine->End();
	//}
}
