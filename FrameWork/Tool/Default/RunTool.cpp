// RunTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "RunTool.h"
#include "afxdialogex.h"

#include "GameObject.h"
#include "ToolNaviMeshObject.h"
#include "ToolNaviMeshObjectVertex.h"
#include "ToolView.h"
#include "MapScene.h"
// CRunTool ��ȭ �����Դϴ�.
CRunTool*		g_pRunTool;
IMPLEMENT_DYNAMIC(CRunTool, CDialog)

CRunTool::CRunTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_RUNTOOL, pParent)
{

}

CRunTool::~CRunTool()
{
}

void CRunTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
	DDX_Text(pDX, IDC_EDIT_POSX, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT_POSY, m_vPos.y);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_vPos.z);
	//DDX_Radio(pDX, IDC_RADIO3, m_iOption);

}

CToolRunGameLine * CRunTool::Get_LastMeshObject()
{
	if (m_vecRunLine.empty())
		return nullptr;

	return m_vecRunLine.back();
}


BEGIN_MESSAGE_MAP(CRunTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SCENE_CLEAR, &CRunTool::OnBnClickedButtonSceneClear)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CRunTool::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CRunTool::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CRunTool::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CRunTool::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_RADIO1, &CRunTool::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CRunTool::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CRunTool::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CRunTool::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CRunTool::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &CRunTool::OnBnClickedRadio6)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CRunTool::OnTvnSelchangedTree1)
	ON_EN_UPDATE(IDC_EDIT_POSX, &CRunTool::OnEnUpdateEditPosx)
	ON_EN_SETFOCUS(IDC_EDIT_POSX, &CRunTool::OnEnSetfocusEditPosx)
	ON_EN_SETFOCUS(IDC_EDIT_POSY, &CRunTool::OnEnSetfocusEditPosy)
	ON_EN_SETFOCUS(IDC_EDIT_POSZ, &CRunTool::OnEnSetfocusEditPosz)
	ON_EN_UPDATE(IDC_EDIT_POSY, &CRunTool::OnEnUpdateEditPosy)
	ON_EN_UPDATE(IDC_EDIT_POSZ, &CRunTool::OnEnUpdateEditPosz)
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CRunTool �޽��� ó�����Դϴ�.


BOOL CRunTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	g_pRunTool = this;
	m_eRunMode = MODE_END;
	m_eFocusType = FOCUS_END;
	m_eOption = NORMAL;
	//m_iOption = 0;
	m_vPos = _vec3(0.f, 0.f, 0.f);
	m_iLineNum = 0;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CRunTool::OnBnClickedButtonSceneClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	dynamic_cast<CMapScene*>(g_pToolView->Get_Management()->Get_Scene())->SceneClear();
	m_treeCtrl.DeleteAllItems();
	m_vPos = _vec3(0.f, 0.f, 0.f);
	m_iLineNum = 0;
	m_vecRunLine.clear();
	m_cstrTreeSelectedName = _T("");
}


void CRunTool::OnBnClickedButtonSave()
{
	//UpdateData(TRUE);
	//CFileDialog Dlg(FALSE,
	//	L"dat",
	//	L"*.dat",
	//	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	//	L"Data File(*.dat)||",
	//	this);
	//TCHAR szFilePath[MAX_STR] = L"";
	//GetCurrentDirectory(MAX_STR, szFilePath);

	//PathRemoveFileSpec(szFilePath);
	//lstrcat(szFilePath, L"\\Data");
	//Dlg.m_ofn.lpstrInitialDir = szFilePath;

	//if (Dlg.DoModal())
	//{
	//	CString StrPath = Dlg.GetPathName();
	//	HANDLE hFile = CreateFile(StrPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	//	if (INVALID_HANDLE_VALUE == hFile)
	//		return;

	//	DWORD dwByte = 0;
	//	_vec3 vecPos[2];
	//	LINEOPTION	eLineOption;
	//	for (auto& LineObj : m_vecRunLine)
	//	{
	//		eLineOption = (LINEOPTION)LineObj->Get_LineOption();
	//		vector<CToolNaviMeshObjectVertex*> vecNaviMeshNertex = LineObj->Get_vecVertex();
	//		vecPos[0] = *vecNaviMeshNertex[0]->Get_Transform()->Get_Info(Engine::INFO_POS);
	//		vecPos[1] = *vecNaviMeshNertex[1]->Get_Transform()->Get_Info(Engine::INFO_POS);
	//		//bSorting = NaviMesh->Get_Sorting();
	//		//WriteFile(hFile, &eNaviMeshType, sizeof(Engine::NAVIMESHTYPE), &dwByte, nullptr);
	//		//WriteFile(hFile, &bSorting, sizeof(_bool), &dwByte, nullptr);
	//		WriteFile(hFile, &vecPos[0], sizeof(_vec3), &dwByte, nullptr);
	//		WriteFile(hFile, &vecPos[1], sizeof(_vec3), &dwByte, nullptr);
	//		WriteFile(hFile, &eLineOption, sizeof(LINEOPTION), &dwByte, nullptr);

	//	}
	//	CloseHandle(hFile);
	//}


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
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\Mesh");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (Dlg.DoModal())
	{
		wofstream fout;
		fout.open(Dlg.GetPathName());

		if (fout.fail())
			return;

		DWORD dwByte = 0;
		_vec3 vecPos[2];
		LINEOPTION	eLineOption;
		for (auto& LineObj : m_vecRunLine)
		{
			eLineOption = (LINEOPTION)LineObj->Get_LineOption();
			vector<CToolNaviMeshObjectVertex*> vecNaviMeshNertex = LineObj->Get_vecVertex();
			vecPos[0] = *vecNaviMeshNertex[0]->Get_Transform()->Get_Info(Engine::INFO_POS);
			vecPos[1] = *vecNaviMeshNertex[1]->Get_Transform()->Get_Info(Engine::INFO_POS);

			fout << vecPos[0].x << endl;
			fout << vecPos[0].y << endl;
			fout << vecPos[0].z << endl;
			fout << vecPos[1].x << endl;
			fout << vecPos[1].y << endl;
			fout << vecPos[1].z << endl;
			fout << eLineOption << endl;
		}
		fout.close();
	}
}


void CRunTool::OnBnClickedButtonLoad()
{
	//// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CFileDialog Dlg(TRUE,// ����
	//	L"dat",// ����Ʈ Ȯ���� ��. 
	//	L"*.dat",//��ȭ���ڿ� ǥ�õ� ���� ���ϸ�. 
	//	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,// OFN_HIDEREADONLY�б� ���������� ����ڴ�. OFN_OVERWRITEPROMPT�ߺ����� ����� ���޽��� ���. 
	//	L"Data File(*.dat)||",// ���� ����� ����. 
	//	this);// �θ�â�� ������ �ڵ�䱸. 
	//TCHAR szFilePath[MAX_STR] = L"";

	//// ���� ������ ��θ� ������ �Լ�. 
	//GetCurrentDirectory(MAX_STR, szFilePath);

	//// ��ü����� �����̸��� �߶��ִ� �Լ�. 
	//// ���� �������� �ִ� ������ ������ ���� ���� �߶󳻰� ������ ������ ������ �������� �߶��ش�. 
	//PathRemoveFileSpec(szFilePath);
	////D:\�ں���\103A\4������-����\DefaultWindow\Data
	//lstrcat(szFilePath, L"\\Data");

	//Dlg.m_ofn.lpstrInitialDir = szFilePath;
	////DoModal - ��ȭâ�� ������. 

	//if (Dlg.DoModal())
	//{
	//	// GetPathName - ���� ��� ������ �Լ�. 
	//	CString StrPath = Dlg.GetPathName();

	//	//g_pMapScene->SceneClear();
	//	m_treeCtrl.DeleteAllItems();
	//	m_iLineNum = 0;
	//	m_cstrTreeSelectedName = _T("");
	//	m_vecRunLine.clear();

	//	HANDLE hFile = CreateFile(StrPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	//	if (INVALID_HANDLE_VALUE == hFile)
	//		return;

	//	DWORD dwByte = 0;
	//	_vec3 vecPos[2];
	//	LINEOPTION	eLineOption;
	//	//Engine::NAVIMESHTYPE eNaviMeshType;
	//	ZeroMemory(&vecPos, sizeof(_vec3) * 2);
	//	CString cstrMeshNum = _T("");
	//	CString cstrMeshVertexNum = _T("");
	//	Engine::CGameObject* pLineObj = nullptr;
	//	Engine::CGameObject* pNaviMeshVertex = nullptr;

	//	while (true)
	//	{
	//		int iVertexNum = 0;

	//		ReadFile(hFile, &vecPos[0], sizeof(_vec3), &dwByte, nullptr);
	//		ReadFile(hFile, &vecPos[1], sizeof(_vec3), &dwByte, nullptr);
	//		ReadFile(hFile, &eLineOption, sizeof(LINEOPTION), &dwByte, nullptr);

	//		if (0 == dwByte)
	//			break;

	//		pLineObj = CToolRunGameLine::Create(g_pToolView->Get_GraphicDev(), eLineOption);
	//		Engine::Add_GameObject(Engine::NAVIMESH, L"RunGameLine", pLineObj);
	//		cstrMeshNum = _T("");
	//		cstrMeshNum.Format(_T("%d"), m_iLineNum++);
	//		m_ParentItem = m_treeCtrl.InsertItem(cstrMeshNum);

	//		pNaviMeshVertex = CToolNaviMeshObjectVertex::Create(g_pToolView->Get_GraphicDev(), vecPos[0]);
	//		dynamic_cast<CToolRunGameLine*>(pLineObj)->Add_Vertex(dynamic_cast<CToolNaviMeshObjectVertex*>(pNaviMeshVertex));
	//		Engine::Add_GameObject(Engine::NAVIMESH, L"Vertex", pNaviMeshVertex);
	//		cstrMeshVertexNum = _T("");
	//		cstrMeshVertexNum.Format(_T("%d"), iVertexNum++);
	//		m_ChildItem = m_treeCtrl.InsertItem(cstrMeshVertexNum, 0, 0, m_ParentItem, TVI_LAST);

	//		pNaviMeshVertex = CToolNaviMeshObjectVertex::Create(g_pToolView->Get_GraphicDev(), vecPos[1]);
	//		dynamic_cast<CToolRunGameLine*>(pLineObj)->Add_Vertex(dynamic_cast<CToolNaviMeshObjectVertex*>(pNaviMeshVertex));
	//		Engine::Add_GameObject(Engine::NAVIMESH, L"Vertex", pNaviMeshVertex);
	//		cstrMeshVertexNum = _T("");
	//		cstrMeshVertexNum.Format(_T("%d"), iVertexNum++);
	//		m_ChildItem = m_treeCtrl.InsertItem(cstrMeshVertexNum, 0, 0, m_ParentItem, TVI_LAST);

	//		m_vecRunLine.emplace_back(dynamic_cast<CToolRunGameLine*>(pLineObj));
	//	}
	//	CloseHandle(hFile);
	//}











	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE,// ����
		L"dat",// ����Ʈ Ȯ���� ��. 
		L"*.dat",//��ȭ���ڿ� ǥ�õ� ���� ���ϸ�. 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,// OFN_HIDEREADONLY�б� ���������� ����ڴ�. OFN_OVERWRITEPROMPT�ߺ����� ����� ���޽��� ���. 
		L"Data File(*.dat)||",// ���� ����� ����. 
		this);// �θ�â�� ������ �ڵ�䱸. 
	TCHAR szFilePath[MAX_STR] = L"";

	// ���� ������ ��θ� ������ �Լ�. 
	GetCurrentDirectory(MAX_STR, szFilePath);

	// ��ü����� �����̸��� �߶��ִ� �Լ�. 
	// ���� �������� �ִ� ������ ������ ���� ���� �߶󳻰� ������ ������ ������ �������� �߶��ش�. 
	PathRemoveFileSpec(szFilePath);
	//D:\�ں���\103A\4������-����\DefaultWindow\Data
	lstrcat(szFilePath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	//DoModal - ��ȭâ�� ������. 

	if (Dlg.DoModal())
	{
		CString StrPath = Dlg.GetPathName();

		wifstream fin;
		fin.open(StrPath);

		if (fin.fail())
			return;

		m_treeCtrl.DeleteAllItems();
		m_iLineNum = 0;
		m_cstrTreeSelectedName = _T("");
		m_vecRunLine.clear();

		_vec3 vecPos[2];
		_uint	iTempLineOption;
		LINEOPTION	eLineOption;

		ZeroMemory(&vecPos, sizeof(_vec3) * 2);
		CString cstrMeshNum = _T("");
		CString cstrMeshVertexNum = _T("");
		Engine::CGameObject* pLineObj = nullptr;
		Engine::CGameObject* pNaviMeshVertex = nullptr;

		while (true)
		{
			int iVertexNum = 0;

			fin >> vecPos[0].x;
			fin >> vecPos[0].y;
			fin >> vecPos[0].z;
			fin >> vecPos[1].x;
			fin >> vecPos[1].y;
			fin >> vecPos[1].z;
			fin >> iTempLineOption;
			eLineOption = (LINEOPTION)iTempLineOption;

			if (fin.eof())
				break;

			pLineObj = CToolRunGameLine::Create(g_pToolView->Get_GraphicDev(), eLineOption);
			Engine::Add_GameObject(Engine::NAVIMESH, L"RunGameLine", pLineObj);
			cstrMeshNum = _T("");
			cstrMeshNum.Format(_T("%d"), m_iLineNum++);
			m_ParentItem = m_treeCtrl.InsertItem(cstrMeshNum);

			pNaviMeshVertex = CToolNaviMeshObjectVertex::Create(g_pToolView->Get_GraphicDev(), vecPos[0]);
			dynamic_cast<CToolRunGameLine*>(pLineObj)->Add_Vertex(dynamic_cast<CToolNaviMeshObjectVertex*>(pNaviMeshVertex));
			Engine::Add_GameObject(Engine::NAVIMESH, L"Vertex", pNaviMeshVertex);
			cstrMeshVertexNum = _T("");
			cstrMeshVertexNum.Format(_T("%d"), iVertexNum++);
			m_ChildItem = m_treeCtrl.InsertItem(cstrMeshVertexNum, 0, 0, m_ParentItem, TVI_LAST);

			pNaviMeshVertex = CToolNaviMeshObjectVertex::Create(g_pToolView->Get_GraphicDev(), vecPos[1]);
			dynamic_cast<CToolRunGameLine*>(pLineObj)->Add_Vertex(dynamic_cast<CToolNaviMeshObjectVertex*>(pNaviMeshVertex));
			Engine::Add_GameObject(Engine::NAVIMESH, L"Vertex", pNaviMeshVertex);
			cstrMeshVertexNum = _T("");
			cstrMeshVertexNum.Format(_T("%d"), iVertexNum++);
			m_ChildItem = m_treeCtrl.InsertItem(cstrMeshVertexNum, 0, 0, m_ParentItem, TVI_LAST);

			m_vecRunLine.emplace_back(dynamic_cast<CToolRunGameLine*>(pLineObj));
		}
		fin.close();
	}
}


void CRunTool::OnBnClickedButtonDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	HTREEITEM CurItem = m_treeCtrl.GetNextItem(NULL, TVGN_CARET);
	if (!m_treeCtrl.ItemHasChildren(CurItem))
		return;

	CString CurItemName = m_treeCtrl.GetItemText(CurItem);
	if (CurItemName == _T(""))
		return;
	int iCurNum = _ttoi(CurItemName);
	m_vecRunLine[iCurNum]->Set_Dead();
	m_vecRunLine.erase(m_vecRunLine.begin() + iCurNum);
	m_iLineNum--;
	m_treeCtrl.DeleteAllItems();

	int iParentNum = 0;
	CString cstrNaviMeshNum = _T("");
	list<Engine::CGameObject*> listGameObject = g_pMapScene->Get_GameObjectlist(Engine::NAVIMESH, L"RunGameLine");
	for (auto& RunLineObj : listGameObject)
	{
		if (!dynamic_cast<CToolNaviMeshObject*>(RunLineObj)->IsDead())
		{
			cstrNaviMeshNum.Format(_T("%d"), iParentNum);
			m_ParentItem = m_treeCtrl.InsertItem(cstrNaviMeshNum);

			vector<CToolNaviMeshObjectVertex*> vertex = dynamic_cast<CToolNaviMeshObject*>(RunLineObj)->Get_vecVertex();
			int iChildNum = 0;
			CString cstrLineVertexNum = _T("");
			for (auto& vecVertex : vertex)
			{
				cstrLineVertexNum.Format(_T("%d"), iChildNum);
				m_ChildItem = m_treeCtrl.InsertItem(cstrLineVertexNum, 0, 0, m_ParentItem, TVI_LAST);
				++iChildNum;
			}
			++iParentNum;
		}
	}
}


void CRunTool::OnBnClickedButtonReset()
{
	UpdateData(TRUE);
	m_vPos = _vec3(0.f, 0.f, 0.f);
	UpdateData(FALSE);
}


void CRunTool::OnBnClickedRadio1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_eRunMode = ADD;
	UpdateData(FALSE);
}


void CRunTool::OnBnClickedRadio2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_eRunMode = EDIT;
	UpdateData(FALSE);
}


void CRunTool::OnBnClickedRadio3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_eOption = NORMAL;
	UpdateData(FALSE);
}


void CRunTool::OnBnClickedRadio4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_eOption = SPEED;
	UpdateData(FALSE);
}


void CRunTool::OnBnClickedRadio5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_eOption = SLOW;
	UpdateData(FALSE);
}


void CRunTool::OnBnClickedRadio6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_eOption = FALL;
	UpdateData(FALSE);
}


void CRunTool::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	HTREEITEM CurItem = m_treeCtrl.GetNextItem(NULL, TVGN_CARET);
	if (!m_treeCtrl.ItemHasChildren(CurItem))
		return;
	CString cstrNaviMeshName = m_treeCtrl.GetItemText(CurItem);
	int iSelectNum = _ttoi(cstrNaviMeshName);

	for (_ulong i = 0; i < m_vecRunLine.size(); ++i)
	{
		if (i == iSelectNum)
		{
			vector<CToolNaviMeshObjectVertex*> vertex = m_vecRunLine[i]->Get_vecVertex();
			for (auto& vecVertex : vertex)
			{
				vecVertex->Set_Pick(true);
			}
			m_vecRunLine[i]->Set_Pick(true);
		}
		else
		{
			vector<CToolNaviMeshObjectVertex*> vertex = m_vecRunLine[i]->Get_vecVertex();
			for (auto& vecVertex : vertex)
			{
				vecVertex->Set_Pick(false);
			}
			m_vecRunLine[i]->Set_Pick(false);
		}
	}
	*pResult = 0;
}


void CRunTool::OnEnUpdateEditPosx()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	for (auto& LineObject : m_vecRunLine)
	{
		vector<CToolNaviMeshObjectVertex*> vertex = LineObject->Get_vecVertex();
		for (auto& vecVertex : vertex)
		{
			if (vecVertex->Get_Pick())
			{
				Engine::CTransform* pTransform = vecVertex->Get_Transform();
				pTransform->Set_Info(&m_vPos, Engine::INFO_POS);
			}
		}
	}
}


void CRunTool::OnEnSetfocusEditPosx()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSX;
}


void CRunTool::OnEnSetfocusEditPosy()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSY;
}


void CRunTool::OnEnSetfocusEditPosz()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSZ;
}


void CRunTool::OnEnUpdateEditPosy()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	for (auto& LineObject : m_vecRunLine)
	{
		vector<CToolNaviMeshObjectVertex*> vertex = LineObject->Get_vecVertex();
		for (auto& vecVertex : vertex)
		{
			if (vecVertex->Get_Pick())
			{
				Engine::CTransform* pTransform = vecVertex->Get_Transform();
				pTransform->Set_Info(&m_vPos, Engine::INFO_POS);
			}
		}
	}
}


void CRunTool::OnEnUpdateEditPosz()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	for (auto& LineObject : m_vecRunLine)
	{
		vector<CToolNaviMeshObjectVertex*> vertex = LineObject->Get_vecVertex();
		for (auto& vecVertex : vertex)
		{
			if (vecVertex->Get_Pick())
			{
				Engine::CTransform* pTransform = vecVertex->Get_Transform();
				pTransform->Set_Info(&m_vPos, Engine::INFO_POS);
			}
		}
	}
}

BOOL CRunTool::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	CDialog::OnMouseHWheel(nFlags, zDelta, pt);
	UpdateData(TRUE);
	switch (m_eFocusType)
	{
	case FOCUSTYPE::POSX:
		if (zDelta > 0)
		{
			m_vPos.x += 1.f;
		}
		else
		{
			m_vPos.x -= 1.f;
		}
		break;
	case FOCUSTYPE::POSY:
		if (zDelta > 0)
		{
			m_vPos.y += 1.f;
		}
		else
		{
			m_vPos.y -= 1.f;
		}
		break;
	case FOCUSTYPE::POSZ:
		if (zDelta > 0)
		{
			m_vPos.z += 1.f;
		}
		else
		{
			m_vPos.z -= 1.f;
		}
		break;
	case FOCUSTYPE::FOCUS_END:
		break;
	default:
		break;
	}
	//if (m_cstrTreeSelectedName == _T(""))
	//	return CDialog::OnMouseWheel(nFlags, zDelta, pt);

	for (auto& LineObject : m_vecRunLine)
	{
		vector<CToolNaviMeshObjectVertex*> vertex = LineObject->Get_vecVertex();
		for (auto& vecVertex : vertex)
		{
			if (vecVertex->Get_Pick())
			{
				vecVertex->Get_Transform()->Set_Info(&m_vPos, Engine::INFO_POS);
			}
		}
	}


	UpdateData(FALSE);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
