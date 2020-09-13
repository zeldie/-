// NavigationTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "NavigationTool.h"
#include "afxdialogex.h"


#include "GameObject.h"
#include "ToolNaviMeshObject.h"
#include "ToolNaviMeshObjectVertex.h"
#include "ToolView.h"
#include "MapScene.h"
// CNavigationTool 대화 상자입니다.

CNavigationTool*	g_pNavigationTool;
IMPLEMENT_DYNAMIC(CNavigationTool, CDialog)

CNavigationTool::CNavigationTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_NAVIGATIONTOOL, pParent),
	m_eNaviMode(MODE_END),
	m_eNaviType(Engine::NAVIMESHTYPE_END),
	m_eFocusType(FOCUS_END),
	m_iMeshNum(0)
	, m_iOption(0)
{
	ZeroMemory(&m_vecPos, sizeof(_vec3));
}

CNavigationTool::~CNavigationTool()
{
	//for (auto& MyPair : m_mapNaviMesh)
	//	ENGINE::Safe_Release(MyPair.second);
}

void CNavigationTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeNavigation);
	DDX_Text(pDX, IDC_EDIT6, m_vecPos.x);
	DDX_Text(pDX, IDC_EDIT7, m_vecPos.y);
	DDX_Text(pDX, IDC_EDIT8, m_vecPos.z);
	DDX_Radio(pDX, IDC_RADIO3, m_iOption);
}


BEGIN_MESSAGE_MAP(CNavigationTool, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CNavigationTool::OnTvnSelchangedNavigationTree)
	ON_BN_CLICKED(IDC_RADIO1, &CNavigationTool::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_RADIO2, &CNavigationTool::OnBnClickedEdit)
	ON_EN_UPDATE(IDC_EDIT6, &CNavigationTool::OnEnUpdatePosX)
	ON_EN_UPDATE(IDC_EDIT7, &CNavigationTool::OnEnUpdatePosY)
	ON_EN_UPDATE(IDC_EDIT8, &CNavigationTool::OnEnUpdatePosZ)
	ON_EN_SETFOCUS(IDC_EDIT6, &CNavigationTool::OnEnSetfocusPosX)
	ON_EN_SETFOCUS(IDC_EDIT7, &CNavigationTool::OnEnSetfocusPosY)
	ON_EN_SETFOCUS(IDC_EDIT8, &CNavigationTool::OnEnSetfocusPosZ)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON1, &CNavigationTool::OnBnClickedResetButton)
	ON_BN_CLICKED(IDC_BUTTON10, &CNavigationTool::OnBnClickedSaveButton)
	ON_BN_CLICKED(IDC_BUTTON11, &CNavigationTool::OnBnClickedLoadButton)
	ON_BN_CLICKED(IDC_BUTTON2, &CNavigationTool::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_BUTTON12, &CNavigationTool::OnBnClickedSceneClearButton)
	ON_BN_CLICKED(IDC_RADIO3, &CNavigationTool::OnBnClickedNormal)
	ON_BN_CLICKED(IDC_RADIO4, &CNavigationTool::OnBnClickedTrigger)
	ON_BN_CLICKED(IDC_RADIO5, &CNavigationTool::OnBnClickedEvent)
END_MESSAGE_MAP()


// CNavigationTool 메시지 처리기입니다.


CToolNaviMeshObject * CNavigationTool::Get_LastMeshObject()
{
	if (m_vecNaviMesh.empty())
		return nullptr;

	return m_vecNaviMesh.back();
}

HTREEITEM CNavigationTool::Get_LastTreeItem()
{
	return m_treeNavigation.GetSelectedItem();
	//return HTREEITEM();
}

void CNavigationTool::OnTvnSelchangedNavigationTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM CurItem = m_treeNavigation.GetNextItem(NULL, TVGN_CARET);
	if (!m_treeNavigation.ItemHasChildren(CurItem))
		return;
	CString cstrNaviMeshName = m_treeNavigation.GetItemText(CurItem);
	int iSelectNum = _ttoi(cstrNaviMeshName);

	for (_ulong i = 0; i < m_vecNaviMesh.size(); ++i)
	{
		if (i == iSelectNum)
		{
			vector<CToolNaviMeshObjectVertex*> vertex = m_vecNaviMesh[i]->Get_vecVertex();
			for (auto& vecVertex : vertex)
			{
				vecVertex->Set_Pick(true);
			}
			m_vecNaviMesh[i]->Set_Pick(true);
		}
		else
		{
			vector<CToolNaviMeshObjectVertex*> vertex = m_vecNaviMesh[i]->Get_vecVertex();
			for (auto& vecVertex : vertex)
			{
				vecVertex->Set_Pick(false);
			}
			m_vecNaviMesh[i]->Set_Pick(false);
		}
	}
	*pResult = 0;
}


BOOL CNavigationTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	long style = GetWindowLong(m_treeNavigation.GetSafeHwnd(), GWL_STYLE) | TVS_HASBUTTONS | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_LINESATROOT | TVS_TRACKSELECT;
	SetWindowLong(m_treeNavigation.GetSafeHwnd(), GWL_STYLE, style);
	m_eNaviMode = ADD;
	m_eNaviType = Engine::NONE;
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	UpdateData(FALSE);
	g_pNavigationTool = this;
	m_iOption = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CNavigationTool::OnBnClickedAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_eNaviMode = ADD;
	UpdateData(FALSE);
}


void CNavigationTool::OnBnClickedEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_eNaviMode = EDIT;
	UpdateData(FALSE);
}


void CNavigationTool::OnEnUpdatePosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.
	UpdateData(TRUE);
	for (auto& NaviMeshObject : m_vecNaviMesh)
	{
		vector<CToolNaviMeshObjectVertex*> vertex = NaviMeshObject->Get_vecVertex();
		for (auto& vecVertex : vertex)
		{
			if (vecVertex->Get_Pick())
			{
				Engine::CTransform* pTransform = vecVertex->Get_Transform();
				pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
			}
		}
	}
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CNavigationTool::OnEnUpdatePosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.
	UpdateData(TRUE);
	for (auto& NaviMeshObject : m_vecNaviMesh)
	{
		vector<CToolNaviMeshObjectVertex*> vertex = NaviMeshObject->Get_vecVertex();
		for (auto& vecVertex : vertex)
		{
			if (vecVertex->Get_Pick())
			{
				Engine::CTransform* pTransform = vecVertex->Get_Transform();
				pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
			}
		}
	}
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CNavigationTool::OnEnUpdatePosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.
	UpdateData(TRUE);
	for (auto& NaviMeshObject : m_vecNaviMesh)
	{
		vector<CToolNaviMeshObjectVertex*> vertex = NaviMeshObject->Get_vecVertex();
		for (auto& vecVertex : vertex)
		{
			if (vecVertex->Get_Pick())
			{
				Engine::CTransform* pTransform = vecVertex->Get_Transform();
				pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
			}
		}
	}
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CNavigationTool::OnEnSetfocusPosX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eFocusType = POSX;
}


void CNavigationTool::OnEnSetfocusPosY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eFocusType = POSY;
}


void CNavigationTool::OnEnSetfocusPosZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eFocusType = POSZ;
}


BOOL CNavigationTool::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (m_eFocusType)
	{
	case CNavigationTool::POSX:
		if (zDelta > 0)
		{
			m_vecPos.x += 1.f;
		}
		else
		{
			m_vecPos.x -= 1.f;
		}
		break;
	case CNavigationTool::POSY:
		if (zDelta > 0)
		{
			m_vecPos.y += 1.f;
		}
		else
		{
			m_vecPos.y -= 1.f;
		}
		break;
	case CNavigationTool::POSZ:
		if (zDelta > 0)
		{
			m_vecPos.z += 1.f;
		}
		else
		{
			m_vecPos.z -= 1.f;
		}
		break;
	case CNavigationTool::FOCUS_END:
		break;
	default:
		break;
	}
	//if (m_cstrTreeSelectedName == _T(""))
	//	return CDialog::OnMouseWheel(nFlags, zDelta, pt);

	for (auto& NaviMeshObject : m_vecNaviMesh)
	{
		vector<CToolNaviMeshObjectVertex*> vertex = NaviMeshObject->Get_vecVertex();
		for (auto& vecVertex : vertex)
		{
			if (vecVertex->Get_Pick())
			{
				vecVertex->Get_Transform()->Set_Info(&m_vecPos, Engine::INFO_POS);
			}
		}
	}


	UpdateData(FALSE);
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CNavigationTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CNavigationTool::OnBnClickedResetButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	UpdateData(FALSE);
}


void CNavigationTool::OnBnClickedSaveButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CFileDialog Dlg(FALSE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data File(*.dat)||",
		this);
	TCHAR szFilePath[MAX_STR] = L"";
	GetCurrentDirectory(MAX_STR, szFilePath);

	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (Dlg.DoModal())
	{
		CString StrPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(StrPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		_vec3 vecPos[3];
		_bool bSorting = false;
		_uint iOption = 0;
		Engine::NAVIMESHTYPE eNaviMeshType;
		for (auto& NaviMesh : m_vecNaviMesh)
		{
			eNaviMeshType = NaviMesh->Get_NaviMeshType();
			vector<CToolNaviMeshObjectVertex*> vecNaviMeshNertex = NaviMesh->Get_vecVertex();
			vecPos[0] = *vecNaviMeshNertex[0]->Get_Transform()->Get_Info(Engine::INFO_POS);
			vecPos[1] = *vecNaviMeshNertex[1]->Get_Transform()->Get_Info(Engine::INFO_POS);
			vecPos[2] = *vecNaviMeshNertex[2]->Get_Transform()->Get_Info(Engine::INFO_POS);
			bSorting = NaviMesh->Get_Sorting();
			iOption = NaviMesh->Get_NaviMeshOption();
			WriteFile(hFile, &eNaviMeshType, sizeof(Engine::NAVIMESHTYPE), &dwByte, nullptr);
			WriteFile(hFile, &bSorting, sizeof(_bool), &dwByte, nullptr);
			WriteFile(hFile, &vecPos[0], sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &vecPos[1], sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &vecPos[2], sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &iOption, sizeof(_uint), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}

}


void CNavigationTool::OnBnClickedLoadButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE,// 열기
		L"dat",// 디폴트 확장자 명. 
		L"*.dat",//대화상자에 표시될 최초 파일명. 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,// OFN_HIDEREADONLY읽기 전용파일은 숨기겠다. OFN_OVERWRITEPROMPT중복파일 저장시 경고메시지 띄움. 
		L"Data File(*.dat)||",// 실제 사용할 필터. 
		this);// 부모창의 윈도우 핸들요구. 
	TCHAR szFilePath[MAX_STR] = L"";

	// 현재 파일의 경로를 얻어오는 함수. 
	GetCurrentDirectory(MAX_STR, szFilePath);

	// 전체경로의 파일이름만 잘라주는 함수. 
	// 제일 마지막에 있는 파일이 있으면 파일 명을 잘라내고 파일이 없으면 마지막 폴더명을 잘라준다. 
	PathRemoveFileSpec(szFilePath);
	//D:\박병건\103A\4개월차-떨림\DefaultWindow\Data
	lstrcat(szFilePath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	//DoModal - 대화창을 열어줌. 

	if (Dlg.DoModal())
	{
		// GetPathName - 파일 경로 얻어오는 함수. 
		CString StrPath = Dlg.GetPathName();

		//g_pMapScene->SceneClear();
		m_treeNavigation.DeleteAllItems();
		m_iMeshNum = 0;
		m_cstrTreeSelectedName = _T("");
		m_vecNaviMesh.clear();

		HANDLE hFile = CreateFile(StrPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		_vec3 vecPos[3];
		_bool bSorting = false;
		_uint iOption = 0;
		Engine::NAVIMESHTYPE eNaviMeshType;
		ZeroMemory(&vecPos, sizeof(_vec3) * 3);
		CString cstrMeshNum = _T("");
		CString cstrMeshVertexNum = _T("");
		Engine::CGameObject* pNaviMesh = nullptr;
		Engine::CGameObject* pNaviMeshVertex = nullptr;

		while (true)
		{
			int iVertexNum = 0;
			ReadFile(hFile, &eNaviMeshType, sizeof(Engine::NAVIMESHTYPE), &dwByte, nullptr);
			if (0 == dwByte)
				break;
			ReadFile(hFile, &bSorting, sizeof(_bool), &dwByte, nullptr);
			ReadFile(hFile, &vecPos[0], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vecPos[1], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vecPos[2], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &iOption, sizeof(_uint), &dwByte, nullptr);
			//내비매쉬 생성 
			pNaviMesh = CToolNaviMeshObject::Create(g_pToolView->Get_GraphicDev(), eNaviMeshType);
			static_cast<CToolNaviMeshObject*>(pNaviMesh)->Set_NaviMeshOption(iOption);
			Engine::Add_GameObject(Engine::NAVIMESH, L"NaviMesh", pNaviMesh);
			cstrMeshNum = _T("");
			cstrMeshNum.Format(_T("%d"), m_iMeshNum++);
			m_ParentItem = m_treeNavigation.InsertItem(cstrMeshNum);
			if (bSorting == true)
			{
				//네비매쉬 첫번째 꼭지점 생성
				pNaviMeshVertex = CToolNaviMeshObjectVertex::Create(g_pToolView->Get_GraphicDev(), vecPos[0]);
				dynamic_cast<CToolNaviMeshObject*>(pNaviMesh)->Add_Vertex(dynamic_cast<CToolNaviMeshObjectVertex*>(pNaviMeshVertex));
				Engine::Add_GameObject(Engine::NAVIMESH, L"Vertex", pNaviMeshVertex);
				cstrMeshVertexNum = _T("");
				cstrMeshVertexNum.Format(_T("%d"), iVertexNum++);
				m_ChildItem = m_treeNavigation.InsertItem(cstrMeshVertexNum, 0, 0, m_ParentItem, TVI_LAST);
				//네비매쉬 두번째 꼭지점 생성
				pNaviMeshVertex = CToolNaviMeshObjectVertex::Create(g_pToolView->Get_GraphicDev(), vecPos[1]);
				dynamic_cast<CToolNaviMeshObject*>(pNaviMesh)->Add_Vertex(dynamic_cast<CToolNaviMeshObjectVertex*>(pNaviMeshVertex));
				Engine::Add_GameObject(Engine::NAVIMESH, L"Vertex", pNaviMeshVertex);
				cstrMeshVertexNum = _T("");
				cstrMeshVertexNum.Format(_T("%d"), iVertexNum++);
				m_ChildItem = m_treeNavigation.InsertItem(cstrMeshVertexNum, 0, 0, m_ParentItem, TVI_LAST);
				//네비매쉬 세번째 꼭지점 생성
				pNaviMeshVertex = CToolNaviMeshObjectVertex::Create(g_pToolView->Get_GraphicDev(), vecPos[2]);
				dynamic_cast<CToolNaviMeshObject*>(pNaviMesh)->Add_Vertex(dynamic_cast<CToolNaviMeshObjectVertex*>(pNaviMeshVertex));
				Engine::Add_GameObject(Engine::NAVIMESH, L"Vertex", pNaviMeshVertex);
				cstrMeshVertexNum = _T("");
				cstrMeshVertexNum.Format(_T("%d"), iVertexNum++);
				m_ChildItem = m_treeNavigation.InsertItem(cstrMeshVertexNum, 0, 0, m_ParentItem, TVI_LAST);

				m_vecNaviMesh.emplace_back(dynamic_cast<CToolNaviMeshObject*>(pNaviMesh));

				// 위에서네비매쉬랑 꼭지점 다만들었음
			}
			else
			{
				//네비매쉬 첫번째 꼭지점 생성
				pNaviMeshVertex = CToolNaviMeshObjectVertex::Create(g_pToolView->Get_GraphicDev(), vecPos[0]);
				dynamic_cast<CToolNaviMeshObject*>(pNaviMesh)->Add_Vertex(dynamic_cast<CToolNaviMeshObjectVertex*>(pNaviMeshVertex));
				Engine::Add_GameObject(Engine::NAVIMESH, L"Vertex", pNaviMeshVertex);
				cstrMeshVertexNum = _T("");
				cstrMeshVertexNum.Format(_T("%d"), iVertexNum++);
				m_ChildItem = m_treeNavigation.InsertItem(cstrMeshVertexNum, 0, 0, m_ParentItem, TVI_LAST);
				//네비매쉬 두번째 꼭지점 생성
				pNaviMeshVertex = CToolNaviMeshObjectVertex::Create(g_pToolView->Get_GraphicDev(), vecPos[2]);
				dynamic_cast<CToolNaviMeshObject*>(pNaviMesh)->Add_Vertex(dynamic_cast<CToolNaviMeshObjectVertex*>(pNaviMeshVertex));
				Engine::Add_GameObject(Engine::NAVIMESH, L"Vertex", pNaviMeshVertex);
				cstrMeshVertexNum = _T("");
				cstrMeshVertexNum.Format(_T("%d"), iVertexNum++);
				m_ChildItem = m_treeNavigation.InsertItem(cstrMeshVertexNum, 0, 0, m_ParentItem, TVI_LAST);
				//네비매쉬 세번째 꼭지점 생성
				pNaviMeshVertex = CToolNaviMeshObjectVertex::Create(g_pToolView->Get_GraphicDev(), vecPos[1]);
				dynamic_cast<CToolNaviMeshObject*>(pNaviMesh)->Add_Vertex(dynamic_cast<CToolNaviMeshObjectVertex*>(pNaviMeshVertex));
				Engine::Add_GameObject(Engine::NAVIMESH, L"Vertex", pNaviMeshVertex);
				cstrMeshVertexNum = _T("");
				cstrMeshVertexNum.Format(_T("%d"), iVertexNum++);
				m_ChildItem = m_treeNavigation.InsertItem(cstrMeshVertexNum, 0, 0, m_ParentItem, TVI_LAST);

				m_vecNaviMesh.emplace_back(dynamic_cast<CToolNaviMeshObject*>(pNaviMesh));

				// 위에서네비매쉬랑 꼭지점 다만들었음
			}

			
			
		}
		CloseHandle(hFile);
	}
}


void CNavigationTool::OnBnClickedDeleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM CurItem = m_treeNavigation.GetNextItem(NULL, TVGN_CARET);
	if (!m_treeNavigation.ItemHasChildren(CurItem))
		return;

	CString CurItemName = m_treeNavigation.GetItemText(CurItem);
	if (CurItemName == _T(""))
		return;
	int iCurNum = _ttoi(CurItemName);
	m_vecNaviMesh[iCurNum]->Set_Dead();
	m_vecNaviMesh.erase(m_vecNaviMesh.begin() + iCurNum);
	m_iMeshNum--;
	m_treeNavigation.DeleteAllItems();

	int iParentNum = 0;
	CString cstrNaviMeshNum = _T("");
	list<Engine::CGameObject*> listGameObject = g_pMapScene->Get_GameObjectlist(Engine::NAVIMESH, L"NaviMesh");
	for (auto& NaviMeshObject : listGameObject)
	{
		if (!dynamic_cast<CToolNaviMeshObject*>(NaviMeshObject)->IsDead())
		{
			cstrNaviMeshNum.Format(_T("%d"), iParentNum);
			m_ParentItem = m_treeNavigation.InsertItem(cstrNaviMeshNum);

			vector<CToolNaviMeshObjectVertex*> vertex = dynamic_cast<CToolNaviMeshObject*>(NaviMeshObject)->Get_vecVertex();
			int iChildNum = 0;
			CString cstrNaviMeshVertexNum = _T("");
			for (auto& vecVertex : vertex)
			{
				cstrNaviMeshVertexNum.Format(_T("%d"), iChildNum);
				m_ChildItem = m_treeNavigation.InsertItem(cstrNaviMeshVertexNum, 0, 0, m_ParentItem, TVI_LAST);
				++iChildNum;
			}
			++iParentNum;
		}
		
	}
	
}


void CNavigationTool::OnBnClickedSceneClearButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	dynamic_cast<CMapScene*>(g_pToolView->Get_Management()->Get_Scene())->SceneClear();
	m_treeNavigation.DeleteAllItems();
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	m_iMeshNum = 0;
	m_vecNaviMesh.clear();
	m_cstrTreeSelectedName = _T("");
}


void CNavigationTool::OnBnClickedNormal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_eNaviType = Engine::NONE;
	UpdateData(FALSE);
}


void CNavigationTool::OnBnClickedTrigger()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_eNaviType = Engine::TRIGGER;
	UpdateData(FALSE);
}


void CNavigationTool::OnBnClickedEvent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_eNaviType = Engine::EVENT;
	UpdateData(FALSE);
}
