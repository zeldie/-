
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MapScene.h"

#include "CameraTool.h"

#include "Calculator.h"
#include "Transform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND		g_hWnd;
HINSTANCE	g_hInst;
CToolView*	g_pToolView;
// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
	Free();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Engine::Set_TimeDelta(L"Timer_Immediate");
	_double	dTimeDelta = Engine::Get_TimeDelta(L"Timer_Immediate");

	if (Engine::IsPermit_Call(L"Frame60", dTimeDelta))
	{
		Engine::Set_TimeDelta(L"Timer_FPS60");
		_double	dTimeDelta60 = Engine::Get_TimeDelta(L"Timer_FPS60");
		Update(dTimeDelta60);
		LateUpdate(dTimeDelta60);
		Render(dTimeDelta60);
	}
	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	RECT rcMain = {};
	pMainFrame->GetWindowRect(&rcMain);
	::SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	RECT rcView = {};
	GetClientRect(&rcView);
	int iWidth = rcMain.right - rcView.right;
	int iHeight = rcMain.bottom - rcView.bottom;

	pMainFrame->SetWindowPos(nullptr, 10, 5, TOOL_WINCX + iWidth, TOOL_WINCY + iHeight, SWP_NOZORDER);

	
	SetUp_DefaultSetting(&m_pGraphicDev);
	Ready_Scene(m_pGraphicDev, &m_pManagementClass);
	if (FAILED(Engine::Ready_InputDev(g_hInst, g_hWnd)))
		return ;
	Engine::Ready_Timer(L"Timer_Immediate");
	Engine::Ready_Timer(L"Timer_FPS60");
	Engine::Ready_Frame(L"Frame60", 10000.f);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pToolView = this;
	SetTimer(0, 1, nullptr);
}

_int CToolView::Update(const _double& dTimeDelta)
{
	if (nullptr == m_pManagementClass)
		return -1;

	Engine::Set_InputDev();

	m_pManagementClass->Update_Scene(dTimeDelta);

	//����Ʈ��
	if (g_bIsPlay)
		g_dTimeBegin += dTimeDelta;

	return 0;
}

_int CToolView::LateUpdate(const _double& dTimeDelta)
{
	if (nullptr == m_pManagementClass)
		return -1;

	m_pManagementClass->LateUpdate_Scene(dTimeDelta);

	return 0;
}

void CToolView::Render(const _double& dTimeDelta)
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	if (nullptr == m_pManagementClass)
		return;

	m_pManagementClass->Render_Scene(m_pGraphicDev, dTimeDelta);

	g_pCamTool->Render_Spline(dTimeDelta);

	Engine::Render_End();
}

void CToolView::Free()
{
	Engine::Safe_Release(m_pGraphicDev);
	//Engine::Safe_Release(m_pManagementClass);
	Engine::Release_Engine();
}

void CToolView::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	if (FAILED(Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, TOOL_WINCX, TOOL_WINCY, &m_pDeviceClass)))
		return;

	*ppGraphicDev = m_pDeviceClass->GetDevice();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);
	Engine::Reserve_ContainerSize(Engine::RESOURCE_END);
	Ready_BasicComponent();
}

void CToolView::Ready_Scene(LPDIRECT3DDEVICE9 & pGraphicDev, Engine::CManagement ** ppManagementClass)
{
	Engine::CScene*		pScene = nullptr;
	if (FAILED(Engine::Create_Management(pGraphicDev, ppManagementClass)))
		return;
	(*ppManagementClass)->AddRef();
	pScene = CMapScene::Create(pGraphicDev);
	if (pScene == nullptr)
		return;

	if (FAILED((*ppManagementClass)->SetUp_Scene(pScene)))
		return;
}

HRESULT CToolView::Ready_BasicComponent()
{
	//Buffer
	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_TriCol", Engine::BUFFER_TRICOL)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_CubeCol", Engine::BUFFER_CUBECOL)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX)))
		return E_FAIL;

	/*if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_TerrainTex", Engine::BUFFER_TERRAINTEX, 129, 129, 1, 1)))
	return 0;*/

	//if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_ScreenTex", Engine::BUFFER_SCREENTEX, )))
	//	return 0;


	//Component
	Engine::CComponent* pComponent = nullptr;
	pComponent = Engine::CTransform::Create(m_pGraphicDev);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Engine::Add_Component(pComponent, L"TransformCom", Engine::RESOURCE_STATIC)))
		return E_FAIL;

	pComponent = Engine::CCalculator::Create(m_pGraphicDev);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Engine::Add_Component(pComponent, L"CalculatorCom", Engine::RESOURCE_STATIC)))
		return E_FAIL;

	return S_OK;
}


void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnTimer(nIDEvent);
	InvalidateRect(nullptr, false);
}
