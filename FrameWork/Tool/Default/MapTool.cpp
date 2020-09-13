// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"


#include "ToolView.h"
#include "MapScene.h"

#include "Transform.h"
// CMapTool ��ȭ �����Դϴ�.

CMapTool*	g_pMapTool;

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent),
	m_eRenderingOption(WIREFRAME)
	, m_dwVtxCntX(0)
	, m_dwVtxCntZ(0)
	, m_dwVtxItv(1)
	, m_dwDetail(1)
{
	ZeroMemory(&m_vecPos, sizeof(_vec3));
	ZeroMemory(&m_vecRot, sizeof(_vec3));
}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeMapTexture);
	DDX_Text(pDX, IDC_EDIT2, m_dwVtxCntX);
	DDX_Text(pDX, IDC_EDIT3, m_dwVtxCntZ);
	DDX_Text(pDX, IDC_EDIT4, m_dwVtxItv);
	DDX_Text(pDX, IDC_EDIT5, m_dwDetail);
	DDX_Text(pDX, IDC_EDIT6, m_vecPos.x);
	DDX_Text(pDX, IDC_EDIT7, m_vecPos.y);
	DDX_Text(pDX, IDC_EDIT8, m_vecPos.z);
	DDX_Text(pDX, IDC_EDIT9, m_vecRot.x);
	DDX_Text(pDX, IDC_EDIT10, m_vecRot.y);
	DDX_Text(pDX, IDC_EDIT11, m_vecRot.z);
	
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMapTool::OnTvnSelchangedTree)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedSaveButton)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::OnBnClickedLoadButton)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnBnClickedSceneClearButton)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedPosResetButton)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapTool::OnBnClickedRotResetButton)
	ON_BN_CLICKED(IDC_BUTTON7, &CMapTool::OnBnClickedResetAllButton)
	ON_BN_CLICKED(IDC_RADIO1, &CMapTool::OnBnClickedRenderingOptionWire)
	ON_BN_CLICKED(IDC_RADIO2, &CMapTool::OnBnClickedRenderingOptionSolid)
	ON_BN_CLICKED(IDC_BUTTON15, &CMapTool::OnBnClickedApplyButton)
	ON_EN_UPDATE(IDC_EDIT6, &CMapTool::OnEnUpdatePosX)
	ON_EN_UPDATE(IDC_EDIT7, &CMapTool::OnEnUpdatePosY)
	ON_EN_UPDATE(IDC_EDIT8, &CMapTool::OnEnUpdatePosZ)
	ON_EN_UPDATE(IDC_EDIT9, &CMapTool::OnEnUpdateRotX)
	ON_EN_UPDATE(IDC_EDIT10, &CMapTool::OnEnUpdateRotY)
	ON_EN_UPDATE(IDC_EDIT11, &CMapTool::OnEnUpdateRotZ)
	ON_EN_SETFOCUS(IDC_EDIT6, &CMapTool::OnEnSetfocusPosX)
	ON_WM_MOUSEWHEEL()
	ON_EN_SETFOCUS(IDC_EDIT7, &CMapTool::OnEnSetfocusPosY)
	ON_EN_SETFOCUS(IDC_EDIT8, &CMapTool::OnEnSetfocusPosZ)
	ON_EN_SETFOCUS(IDC_EDIT9, &CMapTool::OnEnSetfocusRotX)
	ON_EN_SETFOCUS(IDC_EDIT10, &CMapTool::OnEnSetfocusRotY)
	ON_EN_SETFOCUS(IDC_EDIT11, &CMapTool::OnEnSetfocusRotZ)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.


void CMapTool::InitTreeCtrl()
{
	HTREEITEM hParent = m_treeMapTexture.InsertItem(_T("Terrain"));

	CFileFind finder;
	CString fileName = _T("");
	CString filePath = _T("..\\..\\Resource\\Texture\\Terrain\\");
	BOOL bWorking = finder.FindFile(filePath + _T("*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		fileName = finder.GetFileName();

		if (finder.IsDirectory())
		{
			SearchRecursive(hParent, fileName, filePath+fileName);
		}
		else
			m_treeMapTexture.InsertItem(fileName, 0, 0, hParent, TVI_LAST);
	}
	m_treeMapTexture.Expand(hParent, TVE_EXPAND);
}

void CMapTool::SearchRecursive(HTREEITEM hParent, CString Child, CString pFilePath)
{
	HTREEITEM hChild = m_treeMapTexture.InsertItem(Child, 0, 0, hParent, TVI_LAST);

	CFileFind finder;
	CString fileName = _T("");
	CString filePath = pFilePath + _T("\\");
	BOOL bWorking = finder.FindFile(filePath +_T("*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		fileName = finder.GetFileName();
		if (finder.IsDirectory())
		{
			SearchRecursive(hChild, fileName, filePath + fileName);
		}
		else
			m_treeMapTexture.InsertItem(fileName, 0, 0, hChild, TVI_LAST);
	}
}


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//InitTreeCtrl();
	long style = GetWindowLong(m_treeMapTexture.GetSafeHwnd(), GWL_STYLE) |  TVS_HASBUTTONS | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_LINESATROOT | TVS_TRACKSELECT;
	SetWindowLong(m_treeMapTexture.GetSafeHwnd(), GWL_STYLE, style);
	Engine::CTransform* pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::ENVIRONMENT, L"Terrain", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	m_vecPos = *pTransformCom->Get_Info(Engine::INFO_POS);
	UpdateData(FALSE);
	g_pMapTool = this;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CMapTool::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	//���߿� ��ߵɼ���
	HTREEITEM CurItem = m_treeMapTexture.GetNextItem(NULL, TVGN_CARET);
	if (m_treeMapTexture.ItemHasChildren(CurItem))
		return;
	CString	  CurName = m_treeMapTexture.GetItemText(CurItem);

	*pResult = 0;
}


void CMapTool::OnBnClickedSaveButton()
{
	//// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	//	WriteFile(hFile, &m_dwVtxCntX, sizeof(_float), &dwByte, nullptr);
	//	WriteFile(hFile, &m_dwVtxCntZ, sizeof(_float), &dwByte, nullptr);
	//	WriteFile(hFile, &m_dwVtxItv, sizeof(_float), &dwByte, nullptr);
	//	WriteFile(hFile, &m_dwDetail, sizeof(_float), &dwByte, nullptr);
	//	WriteFile(hFile, &m_vecPos, sizeof(_vec3), &dwByte, nullptr);
	//	WriteFile(hFile, &m_vecRot, sizeof(_vec3), &dwByte, nullptr);

	//	CloseHandle(hFile);
	//}
}


void CMapTool::OnBnClickedLoadButton()
{
	//// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//// ���� ���� �� ���� �۾��� �ʿ��� ��ȭ���ڸ� �����ϴ� ��ü. 
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
	//	if (StrPath != L"*.dat")
	//	{
	//	}
	//	else
	//		return;

	//	HANDLE hFile = CreateFile(StrPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	//	if (INVALID_HANDLE_VALUE == hFile)
	//		return;

	//	DWORD	dwByte = 0;
	//	_float	fVtxCntX = 0.f;
	//	_float	fVtxCntZ = 0.f;
	//	_float	fVtxItv = 0.f;
	//	_float	fVtxDefinition = 0.f;
	//	_vec3	vecPos;
	//	_vec3	vecRot;
	//	ZeroMemory(&vecPos, sizeof(_vec3));
	//	ZeroMemory(&vecRot, sizeof(_vec3));
	//	while (true)
	//	{
	//		ReadFile(hFile, &fVtxCntX, sizeof(_float), &dwByte, nullptr);
	//		ReadFile(hFile, &fVtxCntZ, sizeof(_float), &dwByte, nullptr);
	//		ReadFile(hFile, &fVtxItv, sizeof(_float), &dwByte, nullptr);
	//		ReadFile(hFile, &fVtxDefinition, sizeof(_float), &dwByte, nullptr);
	//		ReadFile(hFile, &vecPos, sizeof(_vec3), &dwByte, nullptr);
	//		ReadFile(hFile, &vecRot, sizeof(_vec3), &dwByte, nullptr);

	//		//�ͷ��� �����
	//	}

	//	CloseHandle(hFile);
	//}
}


void CMapTool::OnBnClickedSceneClearButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	dynamic_cast<CMapScene*>(g_pToolView->Get_Management()->Get_Scene())->SceneClear();
}


void CMapTool::OnBnClickedPosResetButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	UpdateData(FALSE);
	OnEnUpdatePosX();
	OnEnUpdatePosY();
	OnEnUpdatePosZ();
}


void CMapTool::OnBnClickedRotResetButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_vecRot = _vec3(0.f, 0.f, 0.f);
	UpdateData(FALSE);
	OnEnUpdateRotX();
	OnEnUpdateRotY();
	OnEnUpdateRotZ();
}


void CMapTool::OnBnClickedResetAllButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	m_vecRot = _vec3(0.f, 0.f, 0.f);
	UpdateData(FALSE);
	OnEnUpdatePosX();
	OnEnUpdatePosY();
	OnEnUpdatePosZ();
	OnEnUpdateRotX();
	OnEnUpdateRotY();
	OnEnUpdateRotZ();
}

void CMapTool::OnBnClickedRenderingOptionWire()
{
	g_pToolView->Get_GraphicDev()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

void CMapTool::OnBnClickedRenderingOptionSolid()
{
	g_pToolView->Get_GraphicDev()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}


void CMapTool::OnBnClickedApplyButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	//dynamic_cast<CMapScene*>(g_pToolView->Get_Management()->Get_Scene())->CreateTerrain(m_dwVtxCntX, m_dwVtxCntZ, m_dwVtxItv, m_dwDetail);

}


void CMapTool::OnEnUpdatePosX()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	Engine::CTransform* pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::ENVIRONMENT, L"Terrain", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	pTransformCom->Set_Info(&m_vecPos, Engine::INFO_POS);
	//udpa
}


void CMapTool::OnEnUpdatePosY()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	Engine::CTransform* pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::ENVIRONMENT, L"Terrain", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	pTransformCom->Set_Info(&m_vecPos, Engine::INFO_POS);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMapTool::OnEnUpdatePosZ()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	Engine::CTransform* pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::ENVIRONMENT, L"Terrain", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	pTransformCom->Set_Info(&m_vecPos, Engine::INFO_POS);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMapTool::OnEnUpdateRotX()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	Engine::CTransform* pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::ENVIRONMENT, L"Terrain", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	pTransformCom->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMapTool::OnEnUpdateRotY()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	Engine::CTransform* pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::ENVIRONMENT, L"Terrain", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	pTransformCom->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMapTool::OnEnUpdateRotZ()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	Engine::CTransform* pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::ENVIRONMENT, L"Terrain", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	pTransformCom->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMapTool::OnEnSetfocusPosX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSX;
}


BOOL CMapTool::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);
	switch (m_eFocusType)
	{
	case CMapTool::POSX:
		if (zDelta > 0)
		{
			m_vecPos.x += 1.f;
		}
		else
		{
			m_vecPos.x -= 1.f;

		}
		break;
	case CMapTool::POSY:
		if (zDelta > 0)
		{
			m_vecPos.y += 1.f;
		}
		else
		{
			m_vecPos.y -= 1.f;

		}
		break;
	case CMapTool::POSZ:
		if (zDelta > 0)
		{
			m_vecPos.z += 1.f;
		}
		else
		{
			m_vecPos.z -= 1.f;

		}
		break;
	case CMapTool::ROTX:
		if (zDelta > 0)
		{
			m_vecRot.x += 0.1f;
		}
		else
		{
			m_vecRot.x -= 0.1f;

		}
		break;
	case CMapTool::ROTY:
		if (zDelta > 0)
		{
			m_vecRot.y += 0.1f;
		}
		else
		{
			m_vecRot.y -= 0.1f;

		}
		break;
	case CMapTool::ROTZ:
		if (zDelta > 0)
		{
			m_vecRot.z += 0.1f;
		}
		else
		{
			m_vecRot.z -= 0.1f;

		}
		break;
	default:
		break;
	}
	Engine::CTransform* pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::ENVIRONMENT, L"Terrain", Engine::TRANSFORM, Engine::ID_DYNAMIC));
	pTransformCom->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransformCom->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	UpdateData(FALSE);
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CMapTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CMapTool::OnEnSetfocusPosY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSY;
}


void CMapTool::OnEnSetfocusPosZ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSZ;
}


void CMapTool::OnEnSetfocusRotX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = ROTX;
}


void CMapTool::OnEnSetfocusRotY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType =ROTY;
}


void CMapTool::OnEnSetfocusRotZ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = ROTZ;
}
