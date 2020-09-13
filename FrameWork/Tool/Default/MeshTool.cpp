// MeshTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MeshTool.h"
#include "afxdialogex.h"

#include "Layer.h"
#include "ToolView.h"
#include "MapScene.h"
#include "GameObject.h"
#include "Transform.h"
#include "ToolGameStaticMeshObject.h"
#include "ToolDynamicMeshObject.h"
// CMeshTool ��ȭ �����Դϴ�.

CMeshTool*	g_pMeshTool;

IMPLEMENT_DYNAMIC(CMeshTool, CDialog)

CMeshTool::CMeshTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MESHTOOL, pParent)
	, m_cstrListKey(_T("")),
	m_iStaticMeshNum(0),
	m_iDynamicMeshNum(0),
	m_cstrTreeSelectedName(_T("")),
	m_cstrListBoxSelectedName(_T("")),
	m_cstrMeshType(_T("")),
	m_pCurrentTransform(nullptr)
{
	ZeroMemory(&m_vecPos, sizeof(_vec3));
	ZeroMemory(&m_vecRot, sizeof(_vec3));
	ZeroMemory(&m_vecScale, sizeof(_vec3));
	ZeroMemory(&m_vSelfRotAngle, sizeof(_vec3));
}

CMeshTool::~CMeshTool()
{
}

void CMeshTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeMesh);
	DDX_Text(pDX, IDC_EDIT6, m_vecPos.x);
	DDX_Text(pDX, IDC_EDIT7, m_vecPos.y);
	DDX_Text(pDX, IDC_EDIT8, m_vecPos.z);
	DDX_Text(pDX, IDC_EDIT9, m_vecRot.x);
	DDX_Text(pDX, IDC_EDIT10, m_vecRot.y);
	DDX_Text(pDX, IDC_EDIT11, m_vecRot.z);
	DDX_Text(pDX, IDC_EDIT12, m_vecScale.x);
	DDX_Text(pDX, IDC_EDIT13, m_vecScale.y);
	DDX_Text(pDX, IDC_EDIT14, m_vecScale.z);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxStaticMesh);
	DDX_Control(pDX, IDC_LIST2, m_ListBoxDynamicMesh);
	DDX_Text(pDX, IDC_EDIT19, m_vSelfRotAngle.x);
	DDX_Text(pDX, IDC_EDIT20, m_vSelfRotAngle.y);
	DDX_Text(pDX, IDC_EDIT15, m_vSelfRotAngle.z);
}


BEGIN_MESSAGE_MAP(CMeshTool, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMeshTool::OnTvnSelchangedMeshTree)
	ON_BN_CLICKED(IDC_BUTTON1, &CMeshTool::OnBnClickedPosResetButton)
	ON_BN_CLICKED(IDC_BUTTON5, &CMeshTool::OnBnClickedRotResetButton)
	ON_BN_CLICKED(IDC_BUTTON6, &CMeshTool::OnBnClickedScaleResetButton)
	ON_EN_UPDATE(IDC_EDIT6, &CMeshTool::OnEnUpdatePosX)
	ON_EN_UPDATE(IDC_EDIT7, &CMeshTool::OnEnUpdatePosY)
	ON_EN_UPDATE(IDC_EDIT8, &CMeshTool::OnEnUpdatePosZ)
	ON_EN_UPDATE(IDC_EDIT9, &CMeshTool::OnEnUpdateRotX)
	ON_EN_UPDATE(IDC_EDIT10, &CMeshTool::OnEnUpdateRotY)
	ON_EN_UPDATE(IDC_EDIT11, &CMeshTool::OnEnUpdateRotZ)
	ON_EN_CHANGE(IDC_EDIT12, &CMeshTool::OnEnUpdateScaleX)
	ON_EN_UPDATE(IDC_EDIT13, &CMeshTool::OnEnUpdateScaleY)
	ON_EN_UPDATE(IDC_EDIT14, &CMeshTool::OnEnUpdateScaleZ)
	ON_BN_CLICKED(IDC_BUTTON8, &CMeshTool::OnBnClickedResetAllButton)
	ON_BN_CLICKED(IDC_BUTTON2, &CMeshTool::OnBnClickedListBoxStaticMeshDelete)
	ON_BN_CLICKED(IDC_BUTTON9, &CMeshTool::OnBnClickedListBoxDynamicMeshDelete)
	ON_BN_CLICKED(IDC_BUTTON10, &CMeshTool::OnBnClickedSaveButton)
	ON_BN_CLICKED(IDC_BUTTON11, &CMeshTool::OnBnClickedLoadButton)
	ON_BN_CLICKED(IDC_BUTTON12, &CMeshTool::OnBnClickedSceneClearButton)
	ON_BN_CLICKED(IDC_BUTTON13, &CMeshTool::OnBnClickedAddMesh)
	ON_EN_SETFOCUS(IDC_EDIT6, &CMeshTool::OnEnSetfocusPosX)
	ON_EN_SETFOCUS(IDC_EDIT7, &CMeshTool::OnEnSetfocusPosY)
	ON_EN_SETFOCUS(IDC_EDIT8, &CMeshTool::OnEnSetfocusPosZ)
	ON_EN_SETFOCUS(IDC_EDIT9, &CMeshTool::OnEnSetfocusRotX)
	ON_EN_SETFOCUS(IDC_EDIT10, &CMeshTool::OnEnSetfocusRotY)
	ON_EN_SETFOCUS(IDC_EDIT11, &CMeshTool::OnEnSetfocusRotZ)
	ON_EN_SETFOCUS(IDC_EDIT12, &CMeshTool::OnEnSetfocusScaleX)
	ON_EN_SETFOCUS(IDC_EDIT13, &CMeshTool::OnEnSetfocusScaleY)
	ON_EN_SETFOCUS(IDC_EDIT14, &CMeshTool::OnEnSetfocusScaleZ)
	ON_WM_MOUSEWHEEL()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMeshTool::OnLbnSelchangeStaticMeshList)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMeshTool::OnLbnSelchangeDynamicMeshList)
	ON_EN_SETFOCUS(IDC_EDIT19, &CMeshTool::OnEnSetfocusSelfRotationX)
	ON_EN_SETFOCUS(IDC_EDIT20, &CMeshTool::OnEnSetfocusSelfRotationY)
	ON_EN_SETFOCUS(IDC_EDIT15, &CMeshTool::OnEnSetfocusSelfRotationZ)
	ON_EN_UPDATE(IDC_EDIT19, &CMeshTool::OnEnUpdateSelfRotateX)
	ON_EN_UPDATE(IDC_EDIT20, &CMeshTool::OnEnUpdateSelfRotateY)
	ON_EN_UPDATE(IDC_EDIT15, &CMeshTool::OnEnUpdateSelfRotateZ)
	ON_BN_CLICKED(IDC_BUTTON17, &CMeshTool::OnBnClickedRunGameSaveButton)
	ON_BN_CLICKED(IDC_BUTTON17, &CMeshTool::OnBnClickedRunGameSaveButton)
	ON_BN_CLICKED(IDC_BUTTON18, &CMeshTool::OnBnClickedRunGameLoadButton)
	ON_EN_SETFOCUS(IDC_EDIT3, &CMeshTool::OnEnSetfocusSect)
END_MESSAGE_MAP()


void CMeshTool::OnTvnSelchangedMeshTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	HTREEITEM CurItem = m_treeMesh.GetNextItem(NULL, TVGN_CARET);
	if (m_treeMesh.ItemHasChildren(CurItem))
		return;
	HTREEITEM SelectedParent = m_treeMesh.GetNextItem(CurItem, TVGN_PARENT);
	HTREEITEM SelectedParent2 = m_treeMesh.GetNextItem(SelectedParent, TVGN_PARENT);//

	m_cstrMeshType = m_treeMesh.GetItemText(SelectedParent2/*SelectedParent*/);
	if (!(m_cstrMeshType == _T("StaticMesh") || m_cstrMeshType == _T("DynamicMesh")))
	{
		HTREEITEM MeshType = m_treeMesh.GetNextItem(SelectedParent2/*SelectedParent*/, TVGN_PARENT);
		m_cstrMeshType = m_treeMesh.GetItemText(MeshType);
	}


	m_cstrTreeSelectedName = m_treeMesh.GetItemText(CurItem);
	*pResult = 0;
}


void CMeshTool::InitTreeCtrl()
{
	HTREEITEM hParent = m_treeMesh.InsertItem(L"Mesh");

	CFileFind finder;
	CString fileName = _T("");
	CString filePath = _T("..\\..\\Resource\\Mesh\\");
	BOOL bWorking = finder.FindFile(filePath + _T("*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		fileName = finder.GetFileName();

		if (finder.IsDirectory())
		{
			SearchRecursive(hParent, fileName, filePath + fileName);
		}
		else
			m_treeMesh.InsertItem(fileName, 0, 0, hParent, TVI_LAST);
	}
	m_treeMesh.Expand(hParent, TVE_EXPAND);
}

void CMeshTool::SearchRecursive(HTREEITEM hParent, CString Child, CString pFilePath)
{
	HTREEITEM hChild = m_treeMesh.InsertItem(Child, 0, 0, hParent, TVI_LAST);

	CFileFind finder;
	CString fileName = _T("");
	CString filePath = pFilePath + _T("\\");
	BOOL bWorking = finder.FindFile(filePath + _T("*.*"));
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
		{
			//���⼭ tga ����
			int Position = fileName.Find(_T(".")) + 1;
			CString cstrexeName = fileName.Mid(Position);
			if ((cstrexeName == _T("X")))
				m_treeMesh.InsertItem(fileName, 0, 0, hChild, TVI_LAST);
		}
	}
}

_int CMeshTool::SearchSameName(CString cstrname, CString cstrMesyType)
{
	int iSameName = 0;
	CString SelectedName = _T("");
	CString FullName = _T("");
	if (cstrMesyType == _T("StaticMesh"))
	{
		for (int i = 0; i < m_iStaticMeshNum; ++i)
		{
			m_ListBoxStaticMesh.GetText(i, FullName);
			AfxExtractSubString(SelectedName, FullName, 0, '-');
			if (cstrname == SelectedName)
				++iSameName;
		}
	}
	else if (cstrMesyType == _T("DynamicMesh"))
	{
		for (int i = 0; i < m_iDynamicMeshNum; ++i)
		{
			m_ListBoxDynamicMesh.GetText(i, FullName);
			AfxExtractSubString(SelectedName, FullName, 0, '-');
			if (cstrname == SelectedName)
				++iSameName;
		}
	}

	return iSameName;
}

BOOL CMeshTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitTreeCtrl();
	long style = GetWindowLong(m_treeMesh.GetSafeHwnd(), GWL_STYLE) | TVS_HASBUTTONS | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_LINESATROOT | TVS_TRACKSELECT;
	SetWindowLong(m_treeMesh.GetSafeHwnd(), GWL_STYLE, style);
	m_vecScale = _vec3(1.f, 1.f, 1.f);
	g_pMeshTool = this;

	m_iSect = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}



void CMeshTool::OnBnClickedPosResetButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_vecPos = _vec3(0.f, -500.f, 0.f);
	UpdateData(FALSE);
	OnEnUpdatePosX();
	OnEnUpdatePosY();
	OnEnUpdatePosZ();
}


void CMeshTool::OnBnClickedRotResetButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_vecRot = _vec3(0.f, 0.f, 0.f);
	UpdateData(FALSE);
	OnEnUpdateRotX();
	OnEnUpdateRotY();
	OnEnUpdateRotZ();
}


void CMeshTool::OnBnClickedScaleResetButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_vecScale = _vec3(0.f, 0.f, 0.f);
	UpdateData(FALSE);
	OnEnUpdateScaleX();
	OnEnUpdateScaleY();
	OnEnUpdateScaleZ();
}


void CMeshTool::OnEnUpdatePosX()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
	/*if (m_cstrListBoxSelectedName == _T(""))
	return;
	if (m_cstrMeshType == _T("StaticMesh"))
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::STATICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}
	else
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}*/
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnEnUpdatePosY()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
	/*if (m_cstrListBoxSelectedName == _T(""))
	return;
	if (m_cstrMeshType == _T("StaticMesh"))
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::STATICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}
	else
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}*/
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnEnUpdatePosZ()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
	/*if (m_cstrListBoxSelectedName == _T(""))
	return;
	if (m_cstrMeshType == _T("StaticMesh"))
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::STATICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}
	else
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}*/
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnEnUpdateRotX()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
	/*if (m_cstrListBoxSelectedName == _T(""))
	return;
	if (m_cstrMeshType == _T("StaticMesh"))
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::STATICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}
	else
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}*/
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnEnUpdateRotY()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
	/*if (m_cstrListBoxSelectedName == _T(""))
	return;
	if (m_cstrMeshType == _T("StaticMesh"))
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::STATICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}
	else
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}*/
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnEnUpdateRotZ()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
	/*if (m_cstrListBoxSelectedName == _T(""))
	return;
	if (m_cstrMeshType == _T("StaticMesh"))
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::STATICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}
	else
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}*/
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnEnUpdateScaleX()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
	/*if (m_cstrListBoxSelectedName == _T(""))
	return;
	if (m_cstrMeshType == _T("StaticMesh"))
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::STATICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}
	else
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}*/
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnEnUpdateScaleY()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
	/*if (m_cstrListBoxSelectedName == _T(""))
	return;
	if (m_cstrMeshType == _T("StaticMesh"))
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::STATICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}
	else
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}*/
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnEnUpdateScaleZ()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
	/*if (m_cstrListBoxSelectedName == _T(""))
	return;
	if (m_cstrMeshType == _T("StaticMesh"))
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::STATICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}
	else
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}*/
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedResetAllButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	m_vecRot = _vec3(0.f, 0.f, 0.f);
	m_vecScale = _vec3(1.f, 1.f, 1.f);
	m_vSelfRotAngle = _vec3(0.f, 0.f, 0.f);
	UpdateData(FALSE);
	OnEnUpdatePosX();
	OnEnUpdatePosY();
	OnEnUpdatePosZ();
	OnEnUpdateRotX();
	OnEnUpdateRotY();
	OnEnUpdateRotZ();
	OnEnUpdateScaleX();
	OnEnUpdateScaleY();
	OnEnUpdateScaleZ();
	OnEnUpdateSelfRotateX();
	OnEnUpdateSelfRotateY();
	OnEnUpdateSelfRotateZ();
}



void CMeshTool::OnBnClickedListBoxStaticMeshDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nCursel = m_ListBoxStaticMesh.GetCurSel();
	int iDeleteNum = 0;
	CString cstrObjectName = _T("");
	if (nCursel >= 0)
	{
		m_ListBoxStaticMesh.GetText(nCursel, m_cstrListBoxSelectedName);
		int Position = m_cstrListBoxSelectedName.Find(_T("-")) + 1;
		AfxExtractSubString(cstrObjectName, m_cstrListBoxSelectedName, 0, '-');
		CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
		int Num = _ttoi(cstrNum);
		wstring wstrSelectedGameObjectName = cstrObjectName.operator LPCWSTR();
		Engine::CGameObject*  pDeleteGameObject = g_pMapScene->Get_GameObject(Engine::STATICMESH, L"Mesh_" + wstrSelectedGameObjectName, Num);
		dynamic_cast<CToolGameObject*>(pDeleteGameObject)->Set_Dead();
		m_ListBoxStaticMesh.DeleteString(nCursel);
		m_iStaticMeshNum--;
	}
	m_ListBoxStaticMesh.ResetContent();
	m_iStaticMeshNum = 0;
	Engine::CLayer* m_Layer = g_pMapScene->Get_Layer(Engine::STATICMESH);
	map<wstring, list<Engine::CGameObject*>> m_mapGameObject = m_Layer->Get_mapGameObject();
	CString cstrObjTag = _T("");
	CString cstrCutObjTag = _T("");
	CString cstrObjNum = _T("");

	for (auto& MyPair : m_mapGameObject)
	{
		_uint iSize = 0;
		wstring wstrObjTag = MyPair.first;
		cstrObjTag = wstrObjTag.c_str();
		int Position = cstrObjTag.Find(_T("_")) + 1;
		cstrCutObjTag = cstrObjTag.Mid(Position);

		if (cstrObjectName == cstrCutObjTag)
			iSize = _uint((MyPair.second).size() - 1);
		else
			iSize = _uint((MyPair.second).size());

		for (_uint i = 0; i < iSize; ++i)
		{
			cstrObjNum.Format(_T("%d"), i);
			m_ListBoxStaticMesh.InsertString(m_iStaticMeshNum, cstrCutObjTag + _T("-") + cstrObjNum);
			m_iStaticMeshNum++;
		}

	}

}



void CMeshTool::OnBnClickedListBoxDynamicMeshDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nCursel = m_ListBoxDynamicMesh.GetCurSel();
	int iDeleteNum = 0;
	CString cstrObjectName = _T("");
	if (nCursel >= 0)
	{
		m_ListBoxDynamicMesh.GetText(nCursel, m_cstrListBoxSelectedName);
		int Position = m_cstrListBoxSelectedName.Find(_T("-")) + 1;
		AfxExtractSubString(cstrObjectName, m_cstrListBoxSelectedName, 0, '-');
		CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
		int Num = _ttoi(cstrNum);
		wstring wstrSelectedGameObjectName = cstrObjectName.operator LPCWSTR();
		Engine::CGameObject*  pDeleteGameObject = g_pMapScene->Get_GameObject(Engine::DYNAMICMESH, L"Mesh_" + wstrSelectedGameObjectName, Num);
		dynamic_cast<CToolGameObject*>(pDeleteGameObject)->Set_Dead();
		/*Engine::CLayer* m_Layer = g_pMapScene->Get_Layer(Engine::DYNAMICMESH);
		m_Layer->Update_Layer(0.01);
		m_Layer->LateUpdate_Layer(0.01);*/
		m_ListBoxDynamicMesh.DeleteString(nCursel);
		m_iDynamicMeshNum--;
	}
	m_ListBoxDynamicMesh.ResetContent();
	m_iDynamicMeshNum = 0;
	Engine::CLayer* m_Layer = g_pMapScene->Get_Layer(Engine::DYNAMICMESH);
	map<wstring, list<Engine::CGameObject*>> m_mapGameObject = m_Layer->Get_mapGameObject();
	CString cstrObjTag = _T("");
	CString cstrCutObjTag = _T("");
	CString cstrObjNum = _T("");

	for (auto& MyPair : m_mapGameObject)
	{
		_uint iSize = 0;
		wstring wstrObjTag = MyPair.first;
		cstrObjTag = wstrObjTag.c_str();
		int Position = cstrObjTag.Find(_T("_")) + 1;
		cstrCutObjTag = cstrObjTag.Mid(Position);

		if (cstrObjectName == cstrCutObjTag)
			iSize = _uint((MyPair.second).size() - 1);
		else
			iSize = _uint((MyPair.second).size());

		for (_uint i = 0; i < iSize; ++i)
		{
			cstrObjNum.Format(_T("%d"), i);
			m_ListBoxDynamicMesh.InsertString(m_iDynamicMeshNum, cstrCutObjTag + _T("-") + cstrObjNum);
			m_iDynamicMeshNum++;
		}

	}
}


void CMeshTool::OnBnClickedSaveButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		DWORD dwstrLayerLen = 0;
		DWORD dwstrObjLen = 0;
		wstring wstrLayerTag = L"";
		CString cstrLayerTag = _T("");
		wstring wstrObjTag = L"";
		CString cstrObjTag = _T("");
		_vec3	vecPos = _vec3(0.f, 0.f, 0.f);
		_vec3	vecRot = _vec3(0.f, 0.f, 0.f);
		_vec3	vecScale = _vec3(0.f, 0.f, 0.f);
		Engine::CLayer* pLayer = nullptr;
		map<wstring, list<Engine::CGameObject*>> mapGameObject;

		//���̳��͸Ž����� ����
		wstrLayerTag = L"DynamicMesh";
		cstrLayerTag = CString::CStringT(wstrLayerTag.c_str());
		dwstrLayerLen = sizeof(TCHAR) * (cstrLayerTag.GetLength() + 1);
		pLayer = g_pMapScene->Get_Layer(Engine::DYNAMICMESH);
		mapGameObject = pLayer->Get_mapGameObject();
		for (auto& Objectlist : mapGameObject)
		{
			wstrObjTag = Objectlist.first;
			cstrObjTag = CString::CStringT(wstrObjTag.c_str());
			dwstrObjLen = sizeof(TCHAR) * (cstrObjTag.GetLength() + 1);
			for (auto& Object : Objectlist.second)
			{
				Engine::CTransform* pTransform = dynamic_cast<CToolGameObject*>(Object)->Get_Transform();
				vecPos = *pTransform->Get_Info(Engine::INFO_POS);
				vecRot = *pTransform->Get_Angle();
				vecScale = *pTransform->Get_Scale();

				//���� ����
				//���̾��̸� ����
				WriteFile(hFile, &dwstrLayerLen, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, cstrLayerTag.GetString(), dwstrLayerLen, &dwByte, nullptr);

				//������Ʈ�̸� ����
				WriteFile(hFile, &dwstrObjLen, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, cstrObjTag.GetString(), dwstrObjLen, &dwByte, nullptr);

				//��ġ, ����, ������ ����
				WriteFile(hFile, &vecPos, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &vecRot, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &vecScale, sizeof(_vec3), &dwByte, nullptr);
			}
		}

		//����ƽ�Ž�����
		wstrLayerTag = L"StaticMesh";
		cstrLayerTag = CString::CStringT(wstrLayerTag.c_str());
		dwstrLayerLen = sizeof(TCHAR) * (cstrLayerTag.GetLength() + 1);
		pLayer = g_pMapScene->Get_Layer(Engine::STATICMESH);
		mapGameObject = pLayer->Get_mapGameObject();
		for (auto& Objectlist : mapGameObject)
		{
			wstrObjTag = Objectlist.first;
			cstrObjTag = CString::CStringT(wstrObjTag.c_str());
			dwstrObjLen = sizeof(TCHAR) * (cstrObjTag.GetLength() + 1);
			for (auto& Object : Objectlist.second)
			{
				Engine::CTransform* pTransform = dynamic_cast<CToolGameObject*>(Object)->Get_Transform();
				vecPos = *pTransform->Get_Info(Engine::INFO_POS);
				vecRot = *pTransform->Get_Angle();
				vecScale = *pTransform->Get_Scale();

				//���� ����
				//���̾��̸� ����
				WriteFile(hFile, &dwstrLayerLen, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, cstrLayerTag.GetString(), dwstrLayerLen, &dwByte, nullptr);

				//������Ʈ�̸� ����
				WriteFile(hFile, &dwstrObjLen, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, cstrObjTag.GetString(), dwstrObjLen, &dwByte, nullptr);

				//��ġ, ����, ������ ����
				WriteFile(hFile, &vecPos, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &vecRot, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &vecScale, sizeof(_vec3), &dwByte, nullptr);
			}
		}

		CloseHandle(hFile);
	}

}


void CMeshTool::OnBnClickedLoadButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ���� ���� �� ���� �۾��� �ʿ��� ��ȭ���ڸ� �����ϴ� ��ü. 
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
		// GetPathName - ���� ��� ������ �Լ�. 
		CString StrPath = Dlg.GetPathName();
		g_pMapScene->SceneClear();
		m_ListBoxStaticMesh.ResetContent();
		m_ListBoxDynamicMesh.ResetContent();
		m_iStaticMeshNum = 0;
		m_iDynamicMeshNum = 0;
		HANDLE hFile = CreateFile(StrPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		DWORD dwstrLayerLen = 0;
		DWORD dwstrObjLen = 0;
		wstring wstrLayerTag = L"";
		CString cstrLayerTag = _T("");
		TCHAR* pLayerTag = nullptr;
		wstring wstrObjTag = L"";
		CString cstrObjTag = _T("");
		TCHAR* pObjTag = nullptr;
		_vec3	vecPos = _vec3(0.f, 0.f, 0.f);
		_vec3	vecRot = _vec3(0.f, 0.f, 0.f);
		_vec3	vecScale = _vec3(0.f, 0.f, 0.f);

		while (true)
		{
			//���̾� �б�
			ReadFile(hFile, &dwstrLayerLen, sizeof(DWORD), &dwByte, nullptr);
			if (0 == dwByte)
				break;
			pLayerTag = new TCHAR[dwstrLayerLen];
			ReadFile(hFile, pLayerTag, dwstrLayerLen, &dwByte, nullptr);
			cstrLayerTag = (LPCTSTR)pLayerTag;
			wstrLayerTag = cstrLayerTag.operator LPCWSTR();

			//������Ʈ�̸� �б�
			ReadFile(hFile, &dwstrObjLen, sizeof(DWORD), &dwByte, nullptr);
			pObjTag = new TCHAR[dwstrObjLen];
			ReadFile(hFile, pObjTag, dwstrObjLen, &dwByte, nullptr);
			cstrObjTag = (LPCTSTR)pObjTag;
			wstrObjTag = cstrObjTag.operator LPCWSTR();

			//��ġ, ����, ������ �б�
			ReadFile(hFile, &vecPos, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vecRot, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vecScale, sizeof(_vec3), &dwByte, nullptr);

			if (wstrLayerTag == L"DynamicMesh")
			{
				int Position = cstrObjTag.Find(_T("_")) + 1;
				CString cstrRealName = cstrObjTag.Mid(Position);
				int iNum = SearchSameName(cstrRealName, L"DynamicMesh");
				CString cstrNum = _T("");
				cstrNum.Format(_T("%d"), iNum);
				m_ListBoxDynamicMesh.InsertString(m_iDynamicMeshNum++, cstrRealName + _T("-") + cstrNum);
				Engine::Add_GameObject(Engine::DYNAMICMESH, wstrObjTag, CToolDynamicMeshObject::Create(g_pToolView->Get_GraphicDev(), wstrObjTag, &vecPos, &vecRot, &vecScale));

			}
			else if (wstrLayerTag == L"StaticMesh")
			{
				int Position = cstrObjTag.Find(_T("_")) + 1;
				CString cstrRealName = cstrObjTag.Mid(Position);
				int iNum = SearchSameName(cstrRealName, L"StaticMesh");
				CString cstrNum = _T("");
				cstrNum.Format(_T("%d"), iNum);
				m_ListBoxStaticMesh.InsertString(m_iStaticMeshNum++, cstrRealName + _T("-") + cstrNum);
				Engine::Add_GameObject(Engine::STATICMESH, wstrObjTag, CToolGameStaticMeshObject::Create(g_pToolView->Get_GraphicDev(), wstrObjTag, &vecPos, &vecRot, &vecScale));

			}

			Engine::Safe_Delete_Array(pLayerTag);
			Engine::Safe_Delete_Array(pObjTag);
		}
		CloseHandle(hFile);
	}
}


void CMeshTool::OnBnClickedSceneClearButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	dynamic_cast<CMapScene*>(g_pToolView->Get_Management()->Get_Scene())->SceneClear();
	m_ListBoxStaticMesh.ResetContent();
	m_ListBoxDynamicMesh.ResetContent();
	m_iStaticMeshNum = 0;
	m_iDynamicMeshNum = 0;
	m_cstrTreeSelectedName = _T("");
	m_cstrListBoxSelectedName = _T("");
	//m_cstrMeshType = _T("");
}


void CMeshTool::OnBnClickedAddMesh()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_cstrTreeSelectedName == _T(""))
		return;

	if (m_cstrMeshType == _T("StaticMesh"))
	{
		TCHAR   szFileName[256];
		ZeroMemory(szFileName, 256);
		StrCpy(szFileName, m_cstrTreeSelectedName);
		PathRemoveExtension(szFileName); //�ڿ� Ȯ���ڸ� �߶���
		m_cstrTreeSelectedName = szFileName;
		int iNum = SearchSameName(m_cstrTreeSelectedName, m_cstrMeshType); //�ش� �̸����� ������� ���� 
		CString cstrNum = _T("");
		cstrNum.Format(_T("%d"), iNum); //List�� ���� �̸��־������� �� ������ ��� Cstring���·� ��ȯ
		m_ListBoxStaticMesh.InsertString(m_iStaticMeshNum++, m_cstrTreeSelectedName + _T("-") + cstrNum); // List�� �־��� �̸�
		CString cstrMeshName = _T(""); // ������ �־��ִ� Key ��
		cstrMeshName = _T("Mesh_") + m_cstrTreeSelectedName; //�� ���� ������ �޽� �ξ��ٶ��� wstringObjectKey��
															 //�Ʒ��� ��ȯ
		wstring GameObjectName = cstrMeshName.operator LPCWSTR();
		Engine::CGameObject* pGameObject = nullptr;
		pGameObject = CToolGameStaticMeshObject::Create(g_pToolView->Get_GraphicDev(), GameObjectName, &m_vecPos, &m_vecRot, &m_vecScale, &_vec3(0.f, 0.f, 0.f), m_iSect);
		if (pGameObject == nullptr)
			return;
		g_pMapScene->Add_GameObject(Engine::STATICMESH, GameObjectName, pGameObject);
	}
	else if (m_cstrMeshType == _T("DynamicMesh"))
	{
		TCHAR   szFileName[256];
		ZeroMemory(szFileName, 256);
		StrCpy(szFileName, m_cstrTreeSelectedName);
		PathRemoveExtension(szFileName); //�ڿ� Ȯ���ڸ� �߶���
		m_cstrTreeSelectedName = szFileName;
		int iNum = SearchSameName(m_cstrTreeSelectedName, m_cstrMeshType); //�ش� �̸����� ������� ���� 
		CString cstrNum = _T("");
		cstrNum.Format(_T("%d"), iNum); //List�� ���� �̸��־������� �� ������ ��� Cstring���·� ��ȯ
		m_ListBoxDynamicMesh.InsertString(m_iDynamicMeshNum++, m_cstrTreeSelectedName + _T("-") + cstrNum); // List�� �־��� �̸�
		CString cstrMeshName = _T(""); // ������ �־��ִ� Key ��
		cstrMeshName = _T("Mesh_") + m_cstrTreeSelectedName; //�� ���� ������ �޽� �ξ��ٶ��� wstringObjectKey��
															 //�Ʒ��� ��ȯ
		wstring GameObjectName = cstrMeshName.operator LPCWSTR();
		Engine::CGameObject* pGameObject = nullptr;
		pGameObject = CToolDynamicMeshObject::Create(g_pToolView->Get_GraphicDev(), GameObjectName, &m_vecPos, &m_vecRot, &m_vecScale);
		if (pGameObject == nullptr)
			return;
		g_pMapScene->Add_GameObject(Engine::DYNAMICMESH, GameObjectName, pGameObject);
	}


}



BOOL CMeshTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CMeshTool::OnEnSetfocusPosX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSX;
}


void CMeshTool::OnEnSetfocusPosY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSY;

}


void CMeshTool::OnEnSetfocusPosZ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSZ;

}


void CMeshTool::OnEnSetfocusRotX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = ROTX;
}


void CMeshTool::OnEnSetfocusRotY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = ROTY;
}


void CMeshTool::OnEnSetfocusRotZ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = ROTZ;
}


void CMeshTool::OnEnSetfocusScaleX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = SCALEX;
}


void CMeshTool::OnEnSetfocusScaleY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = SCALEY;

}


void CMeshTool::OnEnSetfocusScaleZ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = SCALEZ;

}


BOOL CMeshTool::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	////////////////��ũ�� ���氪 ���� 
	UpdateData(TRUE);
	switch (m_eFocusType)
	{
	case CMeshTool::POSX:
		if (zDelta > 0)
		{
			m_vecPos.x += 1.f;
		}
		else
		{
			m_vecPos.x -= 1.f;
		}
		break;
	case CMeshTool::POSY:
		if (zDelta > 0)
		{
			m_vecPos.y += 1.f;
		}
		else
		{
			m_vecPos.y -= 1.f;
		}
		break;
	case CMeshTool::POSZ:
		if (zDelta > 0)
		{
			m_vecPos.z += 5.f;
		}
		else
		{
			m_vecPos.z -= 5.f;
		}
		break;
	case CMeshTool::ROTX:
		if (zDelta > 0)
		{
			m_vecRot.x += 0.1f;
		}
		else
		{
			m_vecRot.x -= 0.1f;
		}
		break;
	case CMeshTool::ROTY:
		if (zDelta > 0)
		{
			m_vecRot.y += 0.1f;
		}
		else
		{
			m_vecRot.y -= 0.1f;
		}
		break;
	case CMeshTool::ROTZ:
		if (zDelta > 0)
		{
			m_vecRot.z += 0.1f;
		}
		else
		{
			m_vecRot.z -= 0.1f;
		}
		break;
	case CMeshTool::SCALEX:
		if (zDelta > 0)
		{
			m_vecScale.x += 0.01f;
		}
		else
		{
			m_vecScale.x -= 0.01f;
		}
		break;
	case CMeshTool::SCALEY:
		if (zDelta > 0)
		{
			m_vecScale.y += 0.01f;
		}
		else
		{
			m_vecScale.y -= 0.01f;
		}
		break;
	case CMeshTool::SCALEZ:
		if (zDelta > 0)
		{
			m_vecScale.z += 0.01f;
		}
		else
		{
			m_vecScale.z -= 0.01f;
		}
		break;
	case CMeshTool::SELFROTATEX:
		if (zDelta > 0)
		{
			m_vSelfRotAngle.x += 0.1f;
		}
		else
		{
			m_vSelfRotAngle.x -= 0.1f;
		}
		break;
	case CMeshTool::SELFROTATEY:
		if (zDelta > 0)
		{
			m_vSelfRotAngle.y += 0.1f;
		}
		else
		{
			m_vSelfRotAngle.y -= 0.1f;
		}
		break;
	case CMeshTool::SELFROTATEZ:
		if (zDelta > 0)
		{
			m_vSelfRotAngle.z += 0.1f;
		}
		else
		{
			m_vSelfRotAngle.z -= 0.1f;
		}
		break;
	case CMeshTool::SECT:
		if (zDelta > 0)
		{
			m_iSect += 1;
		}
		else
		{
			m_iSect -= 1;
		}
		break;
	default:
		break;
	}
	if (m_pCurrentTransform == nullptr)
		return CDialog::OnMouseWheel(nFlags, zDelta, pt);
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
	/*if (m_cstrListBoxSelectedName == _T(""))
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
	if (m_cstrMeshType == _T("StaticMesh"))
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::STATICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	if (pTransform == nullptr)
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}
	else
	{
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("_")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '_');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, Engine::TRANSFORM, Engine::ID_DYNAMIC, iNum));
	if (pTransform == nullptr)
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
	pTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	pTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	pTransform->Set_Scale(&m_vecScale);
	}*/

	UpdateData(FALSE);
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CMeshTool::OnLbnSelchangeStaticMeshList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	int nCursel = m_ListBoxStaticMesh.GetCurSel();
	if (nCursel >= 0)
	{
		m_ListBoxStaticMesh.GetText(nCursel, m_cstrListBoxSelectedName);
	}
	if (m_cstrListBoxSelectedName == _T(""))
		return;
	m_cstrMeshType = _T("StaticMesh");
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("-")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '-');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();

	Engine::CGameObject* pPickingObject = nullptr;
	map<wstring, list<Engine::CGameObject*>> mapStaticMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::STATICMESH)->Get_mapGameObject();
	for (auto& ObjTag : mapStaticMesh)
	{
		for (auto& rGameObject : ObjTag.second)
		{
			static_cast<CToolGameObject*>(rGameObject)->Set_WireFrame(false);
		}
	}
	map<wstring, list<Engine::CGameObject*>> mapDynamicMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::DYNAMICMESH)->Get_mapGameObject();
	for (auto& ObjTag : mapDynamicMesh)
	{
		for (auto& rGameObject : ObjTag.second)
		{
			static_cast<CToolGameObject*>(rGameObject)->Set_WireFrame(false);
		}
	}
	pPickingObject = Engine::Get_GameObject(Engine::STATICMESH, L"Mesh_" + ObjectName, iNum);
	static_cast<CToolGameObject*>(pPickingObject)->Set_WireFrame(true);
	m_pCurrentTransform = static_cast<CToolGameObject*>(pPickingObject)->Get_Transform();
	if (m_pCurrentTransform == nullptr)
		return;
	m_vecPos = *m_pCurrentTransform->Get_Info(Engine::INFO_POS);
	m_vecRot = _vec3((*m_pCurrentTransform->Get_Angle()).x, (*m_pCurrentTransform->Get_Angle()).y, (*m_pCurrentTransform->Get_Angle()).z);
	m_vecScale = *m_pCurrentTransform->Get_Scale();
	UpdateData(FALSE);
}


void CMeshTool::OnLbnSelchangeDynamicMeshList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	int nCursel = m_ListBoxDynamicMesh.GetCurSel();
	if (nCursel >= 0)
	{
		m_ListBoxDynamicMesh.GetText(nCursel, m_cstrListBoxSelectedName);
	}
	if (m_cstrListBoxSelectedName == _T(""))
		return;
	m_cstrMeshType = _T("DynamicMesh");
	CString cstrRealName = _T("");
	int Position = m_cstrListBoxSelectedName.Find(_T("-")) + 1;
	AfxExtractSubString(cstrRealName, m_cstrListBoxSelectedName, 0, '-');
	CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
	int iNum = _wtoi(cstrNum);
	wstring ObjectName = cstrRealName.operator LPCWSTR();


	Engine::CGameObject* pPickingObject = nullptr;
	map<wstring, list<Engine::CGameObject*>> mapStaticMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::STATICMESH)->Get_mapGameObject();
	for (auto& ObjTag : mapStaticMesh)
	{
		for (auto& rGameObject : ObjTag.second)
		{
			static_cast<CToolGameObject*>(rGameObject)->Set_WireFrame(false);
		}
	}
	map<wstring, list<Engine::CGameObject*>> mapDynamicMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::DYNAMICMESH)->Get_mapGameObject();
	for (auto& ObjTag : mapDynamicMesh)
	{
		for (auto& rGameObject : ObjTag.second)
		{
			static_cast<CToolGameObject*>(rGameObject)->Set_WireFrame(false);
		}
	}
	pPickingObject = Engine::Get_GameObject(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, iNum);
	static_cast<CToolGameObject*>(pPickingObject)->Set_WireFrame(true);
	m_pCurrentTransform = static_cast<CToolGameObject*>(pPickingObject)->Get_Transform();
	if (m_pCurrentTransform == nullptr)
		return;
	m_vecPos = *m_pCurrentTransform->Get_Info(Engine::INFO_POS);
	m_vecRot = _vec3((*m_pCurrentTransform->Get_Angle()).x, (*m_pCurrentTransform->Get_Angle()).y, (*m_pCurrentTransform->Get_Angle()).z);
	m_vecScale = *m_pCurrentTransform->Get_Scale();
	UpdateData(FALSE);
}


void CMeshTool::OnEnSetfocusSelfRotationX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = SELFROTATEX;
}


void CMeshTool::OnEnSetfocusSelfRotationY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = SELFROTATEY;
}


void CMeshTool::OnEnSetfocusSelfRotationZ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = SELFROTATEZ;
}


void CMeshTool::OnEnUpdateSelfRotateX()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
}


void CMeshTool::OnEnUpdateSelfRotateY()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
}


void CMeshTool::OnEnUpdateSelfRotateZ()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_pCurrentTransform == nullptr)
		return;
	m_pCurrentTransform->Set_Info(&m_vecPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_vecRot.x, m_vecRot.y, m_vecRot.z);
	m_pCurrentTransform->Set_Scale(&m_vecScale);
	m_pCurrentTransform->Set_SelfRotAngle(&m_vSelfRotAngle);
}



void CMeshTool::OnBnClickedRunGameSaveButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		DWORD dwstrLayerLen = 0;
		DWORD dwstrObjLen = 0;
		wstring wstrLayerTag = L"";
		CString cstrLayerTag = _T("");
		wstring wstrObjTag = L"";
		CString cstrObjTag = _T("");
		_vec3	vecPos = _vec3(0.f, 0.f, 0.f);
		_vec3	vecRot = _vec3(0.f, 0.f, 0.f);
		_vec3	vecScale = _vec3(0.f, 0.f, 0.f);
		_vec3	vecSelRotate = _vec3(0.f, 0.f, 0.f);
		_uint	iSect = 0;
		Engine::CLayer* pLayer = nullptr;
		map<wstring, list<Engine::CGameObject*>> mapGameObject;

		//���̳��͸Ž����� ����
		wstrLayerTag = L"DynamicMesh";
		cstrLayerTag = CString::CStringT(wstrLayerTag.c_str());
		dwstrLayerLen = sizeof(TCHAR) * (cstrLayerTag.GetLength() + 1);
		pLayer = g_pMapScene->Get_Layer(Engine::DYNAMICMESH);
		mapGameObject = pLayer->Get_mapGameObject();
		for (auto& Objectlist : mapGameObject)
		{
			wstrObjTag = Objectlist.first;
			cstrObjTag = CString::CStringT(wstrObjTag.c_str());
			dwstrObjLen = sizeof(TCHAR) * (cstrObjTag.GetLength() + 1);
			for (auto& Object : Objectlist.second)
			{
				Engine::CTransform* pTransform = dynamic_cast<CToolGameObject*>(Object)->Get_Transform();
				vecPos = *pTransform->Get_Info(Engine::INFO_POS);
				vecRot = *pTransform->Get_Angle();
				vecScale = *pTransform->Get_Scale();
				vecSelRotate = *pTransform->Get_SelfRotAngle();
				iSect = dynamic_cast<CToolGameStaticMeshObject*>(Object)->Get_Sect();
				//���� ����
				//���̾��̸� ����
				WriteFile(hFile, &dwstrLayerLen, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, cstrLayerTag.GetString(), dwstrLayerLen, &dwByte, nullptr);

				//������Ʈ�̸� ����
				WriteFile(hFile, &dwstrObjLen, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, cstrObjTag.GetString(), dwstrObjLen, &dwByte, nullptr);

				//��ġ, ����, ������, ���������̼� ����
				WriteFile(hFile, &vecPos, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &vecRot, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &vecScale, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &vecSelRotate, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &iSect, sizeof(_uint), &dwByte, nullptr);
			}
		}

		//����ƽ�Ž�����
		wstrLayerTag = L"StaticMesh";
		cstrLayerTag = CString::CStringT(wstrLayerTag.c_str());
		dwstrLayerLen = sizeof(TCHAR) * (cstrLayerTag.GetLength() + 1);
		pLayer = g_pMapScene->Get_Layer(Engine::STATICMESH);
		mapGameObject = pLayer->Get_mapGameObject();
		for (auto& Objectlist : mapGameObject)
		{
			wstrObjTag = Objectlist.first;
			cstrObjTag = CString::CStringT(wstrObjTag.c_str());
			dwstrObjLen = sizeof(TCHAR) * (cstrObjTag.GetLength() + 1);
			for (auto& Object : Objectlist.second)
			{
				Engine::CTransform* pTransform = dynamic_cast<CToolGameObject*>(Object)->Get_Transform();
				vecPos = *pTransform->Get_Info(Engine::INFO_POS);
				vecRot = *pTransform->Get_Angle();
				vecScale = *pTransform->Get_Scale();
				vecSelRotate = *pTransform->Get_SelfRotAngle();
				iSect = dynamic_cast<CToolGameStaticMeshObject*>(Object)->Get_Sect();
				//���� ����
				//���̾��̸� ����
				WriteFile(hFile, &dwstrLayerLen, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, cstrLayerTag.GetString(), dwstrLayerLen, &dwByte, nullptr);

				//������Ʈ�̸� ����
				WriteFile(hFile, &dwstrObjLen, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, cstrObjTag.GetString(), dwstrObjLen, &dwByte, nullptr);

				//��ġ, ����, ������ ����
				WriteFile(hFile, &vecPos, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &vecRot, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &vecScale, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &vecSelRotate, sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, &iSect, sizeof(_uint), &dwByte, nullptr);
			}
		}

		CloseHandle(hFile);
	}
}


void CMeshTool::OnBnClickedRunGameLoadButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ���� ���� �� ���� �۾��� �ʿ��� ��ȭ���ڸ� �����ϴ� ��ü. 
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
		// GetPathName - ���� ��� ������ �Լ�. 
		CString StrPath = Dlg.GetPathName();
		g_pMapScene->SceneClear();
		m_ListBoxStaticMesh.ResetContent();
		m_ListBoxDynamicMesh.ResetContent();
		m_iStaticMeshNum = 0;
		m_iDynamicMeshNum = 0;
		HANDLE hFile = CreateFile(StrPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		DWORD dwstrLayerLen = 0;
		DWORD dwstrObjLen = 0;
		wstring wstrLayerTag = L"";
		CString cstrLayerTag = _T("");
		TCHAR* pLayerTag = nullptr;
		wstring wstrObjTag = L"";
		CString cstrObjTag = _T("");
		TCHAR* pObjTag = nullptr;
		_vec3	vecPos = _vec3(0.f, 0.f, 0.f);
		_vec3	vecRot = _vec3(0.f, 0.f, 0.f);
		_vec3	vecScale = _vec3(0.f, 0.f, 0.f);
		_vec3	vecSelfRotate = _vec3(0.f, 0.f, 0.f);
		_uint	iSect = 0;

		while (true)
		{
			//���̾� �б�
			ReadFile(hFile, &dwstrLayerLen, sizeof(DWORD), &dwByte, nullptr);
			if (0 == dwByte)
				break;
			pLayerTag = new TCHAR[dwstrLayerLen];
			ReadFile(hFile, pLayerTag, dwstrLayerLen, &dwByte, nullptr);
			cstrLayerTag = (LPCTSTR)pLayerTag;
			wstrLayerTag = cstrLayerTag.operator LPCWSTR();

			//������Ʈ�̸� �б�
			ReadFile(hFile, &dwstrObjLen, sizeof(DWORD), &dwByte, nullptr);
			pObjTag = new TCHAR[dwstrObjLen];
			ReadFile(hFile, pObjTag, dwstrObjLen, &dwByte, nullptr);
			cstrObjTag = (LPCTSTR)pObjTag;
			wstrObjTag = cstrObjTag.operator LPCWSTR();

			//��ġ, ����, ������ �б�
			ReadFile(hFile, &vecPos, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vecRot, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vecScale, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vecSelfRotate, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &iSect, sizeof(_uint), &dwByte, nullptr);

			if (wstrLayerTag == L"DynamicMesh")
			{
				int Position = cstrObjTag.Find(_T("_")) + 1;
				CString cstrRealName = cstrObjTag.Mid(Position);
				int iNum = SearchSameName(cstrRealName, L"DynamicMesh");
				CString cstrNum = _T("");
				cstrNum.Format(_T("%d"), iNum);
				m_ListBoxDynamicMesh.InsertString(m_iDynamicMeshNum++, cstrRealName + _T("-") + cstrNum);
				Engine::Add_GameObject(Engine::DYNAMICMESH, wstrObjTag, CToolDynamicMeshObject::Create(g_pToolView->Get_GraphicDev(), wstrObjTag, &vecPos, &vecRot, &vecScale));

			}
			else if (wstrLayerTag == L"StaticMesh")
			{
				int Position = cstrObjTag.Find(_T("_")) + 1;
				CString cstrRealName = cstrObjTag.Mid(Position);
				int iNum = SearchSameName(cstrRealName, L"StaticMesh");
				CString cstrNum = _T("");
				cstrNum.Format(_T("%d"), iNum);
				m_ListBoxStaticMesh.InsertString(m_iStaticMeshNum++, cstrRealName + _T("-") + cstrNum);
				Engine::Add_GameObject(Engine::STATICMESH, wstrObjTag, CToolGameStaticMeshObject::Create(g_pToolView->Get_GraphicDev(), wstrObjTag, &vecPos, &vecRot, &vecScale, &vecSelfRotate, iSect));

			}

			Engine::Safe_Delete_Array(pLayerTag);
			Engine::Safe_Delete_Array(pObjTag);
		}
		CloseHandle(hFile);
	}
}


void CMeshTool::OnEnSetfocusSect()
{
	m_eFocusType = SECT;
}
