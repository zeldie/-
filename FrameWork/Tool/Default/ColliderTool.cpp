// ColliderTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ColliderTool.h"
#include "afxdialogex.h"

#include "MapScene.h"
#include "ToolView.h"

#include "SphereCollider.h"
#include "Transform.h"
#include "AniCtrl.h"
#include "DynamicMesh.h"
#include "ToolDynamicMeshObject.h"
// CColliderTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CColliderTool, CDialog)

CColliderTool::CColliderTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_COLLIDERTOOL, pParent),
	m_fRadius(10.f),
	m_eColliderType(COLLIDERTYPE_END),
	m_eFocusType(FOCUS_END),
	m_iMeshNum(0),
	m_cstrSelectedMeshName(_T("")),
	m_cstrSelectedBoneName(_T("")),
	m_cstrSelectedAnimationName(_T("")),
	m_cstrSelectedColliderName(_T("")),
	m_iColliderNum(0)
{
	ZeroMemory(&m_vecPos, sizeof(_vec3));
}

CColliderTool::~CColliderTool()
{
}

void CColliderTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxMesh);
	DDX_Control(pDX, IDC_LIST2, m_ListBoxAnimation);
	DDX_Control(pDX, IDC_LIST3, m_ListBoxBone);
	DDX_Control(pDX, IDC_LIST4, m_ListBoxCollider);
	DDX_Text(pDX, IDC_EDIT6, m_vecPos.x);
	DDX_Text(pDX, IDC_EDIT7, m_vecPos.y);
	DDX_Text(pDX, IDC_EDIT8, m_vecPos.z);
	DDX_Text(pDX, IDC_EDIT9, m_fRadius);
}


BEGIN_MESSAGE_MAP(CColliderTool, CDialog)
	ON_BN_CLICKED(IDC_RADIO2, &CColliderTool::OnBnClickedColliderCube)
	ON_BN_CLICKED(IDC_RADIO1, &CColliderTool::OnBnClickedColliderSphere)
	ON_BN_CLICKED(IDC_BUTTON2, &CColliderTool::OnBnClickedAnimationPlay)
	ON_BN_CLICKED(IDC_BUTTON10, &CColliderTool::OnBnClickedAnimationPause)
	ON_BN_CLICKED(IDC_BUTTON1, &CColliderTool::OnBnClickedPosResetButton)
	ON_BN_CLICKED(IDC_BUTTON8, &CColliderTool::OnBnClickedResetAllButton)
	ON_EN_SETFOCUS(IDC_EDIT6, &CColliderTool::OnEnSetfocusPosX)
	ON_EN_UPDATE(IDC_EDIT6, &CColliderTool::OnEnUpdatePosX)
	ON_EN_SETFOCUS(IDC_EDIT7, &CColliderTool::OnEnSetfocusPosY)
	ON_EN_UPDATE(IDC_EDIT7, &CColliderTool::OnEnUpdatePosY)
	ON_EN_SETFOCUS(IDC_EDIT8, &CColliderTool::OnEnSetfocusPosZ)
	ON_EN_UPDATE(IDC_EDIT8, &CColliderTool::OnEnUpdatePosZ)
	ON_EN_SETFOCUS(IDC_EDIT9, &CColliderTool::OnEnSetfocusRadius)
	ON_EN_UPDATE(IDC_EDIT9, &CColliderTool::OnEnUpdateRadius)
	ON_BN_CLICKED(IDC_BUTTON3, &CColliderTool::OnBnClickedColliderCreateButton)
	ON_BN_CLICKED(IDC_BUTTON4, &CColliderTool::OnBnClickedColliderDeleteButton)
	ON_BN_CLICKED(IDC_BUTTON5, &CColliderTool::OnBnClickedColliderSaveButton)
	ON_BN_CLICKED(IDC_BUTTON7, &CColliderTool::OnBnClickedColliderLoadButton)
	ON_WM_MOUSEWHEEL()
	ON_LBN_SELCHANGE(IDC_LIST1, &CColliderTool::OnLbnSelchangeMeshList)
	ON_LBN_SELCHANGE(IDC_LIST2, &CColliderTool::OnLbnSelchangeAnimationList)
	ON_LBN_SELCHANGE(IDC_LIST3, &CColliderTool::OnLbnSelchangeBoneList)
	ON_LBN_SELCHANGE(IDC_LIST4, &CColliderTool::OnLbnSelchangeColliderList)
END_MESSAGE_MAP()


// CColliderTool �޽��� ó�����Դϴ�.


void CColliderTool::OnBnClickedColliderCube()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eColliderType = COLLIDERTYPE::CUBE;
}


void CColliderTool::OnBnClickedColliderSphere()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eColliderType = COLLIDERTYPE::SPHERE;
}


void CColliderTool::OnBnClickedAnimationPlay()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pSelectedMesh->Set_PlayAnimation(true);
}


void CColliderTool::OnBnClickedAnimationPause()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pSelectedMesh->Set_PlayAnimation(false);
}


void CColliderTool::OnBnClickedPosResetButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	UpdateData(FALSE);
}


void CColliderTool::OnBnClickedResetAllButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	m_fRadius = 1.f;
	UpdateData(FALSE);
}


void CColliderTool::OnEnSetfocusPosX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSX;
}


void CColliderTool::OnEnUpdatePosX()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CColliderTool::OnEnSetfocusPosY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSY;
}


void CColliderTool::OnEnUpdatePosY()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CColliderTool::OnEnSetfocusPosZ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eFocusType = POSZ;
}


void CColliderTool::OnEnUpdatePosZ()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CColliderTool::OnEnSetfocusRadius()
{
	m_eFocusType = RADIUS;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CColliderTool::OnEnUpdateRadius()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.
	UpdateData(TRUE);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CColliderTool::OnBnClickedColliderCreateButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	Engine::CTransform* pTransform = m_pSelectedMesh->Get_Transform();
	Engine::CDynamicMesh* pDynamicMesh = m_pSelectedMesh->Get_DynamicMeshCom();
	CStringA cstraSelectedName = CStringA(m_cstrSelectedBoneName);
	const char* m_szSelectedName = cstraSelectedName;
	m_pSelectedMesh->Add_Collider(pTransform, m_fRadius, m_szSelectedName, pDynamicMesh);


	Engine::D3DXFRAME_DERIVED*	pDerivedFrame = ((CToolDynamicMeshObject*)m_pSelectedMesh)->Get_DynamicMeshCom()->Get_FrameByName((CStringA)m_cstrSelectedBoneName);
	memcpy(&m_vecPos, &pDerivedFrame->CombinedTransformationMatrix.m[3][0], sizeof(_vec3));

	CString cstrColliderNum = _T("");
	cstrColliderNum.Format(_T("%d"), m_iColliderNum);
	m_ListBoxCollider.InsertString(m_iColliderNum++, cstrColliderNum);

	UpdateData(FALSE);
}


void CColliderTool::OnBnClickedColliderDeleteButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	_uint iCurNum = m_ListBoxCollider.GetCurSel();
	if (iCurNum < 0)
		return;
	m_ListBoxCollider.GetText(iCurNum, m_cstrSelectedColliderName);
	int iCurColliderCum = _ttoi(m_cstrSelectedColliderName);

	m_pSelectedMesh->DeleteCollider(iCurColliderCum);

	m_ListBoxCollider.ResetContent();
	m_iColliderNum = 0;
	CString cstrColliderNum = _T("");
	
	for (_uint i = 0; i < m_pSelectedMesh->Get_ColliderNum(); ++i)
	{
		cstrColliderNum.Format(_T("%d"), m_iColliderNum);
		m_ListBoxCollider.InsertString(m_iColliderNum++, cstrColliderNum);
	}
}


void CColliderTool::OnBnClickedColliderSaveButton()
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
		DWORD dwstrLen = 0;
		Engine::MESHTYPE eParentMeshType;
		char pFrameName[64] = {0,};
		_float fRadius = 0.f;
		vector<Engine::CSphereCollider*> vecCollider;

		vecCollider = m_pSelectedMesh->Get_vecCollider();
		for (auto& Collider : vecCollider)
		{
			Collider->Get_FrameName(pFrameName);
			eParentMeshType = Collider->Get_ParentMeshType();
			fRadius = Collider->Get_Radius();


			WriteFile(hFile, &pFrameName, MIN_STR, &dwByte, nullptr);
			WriteFile(hFile, &eParentMeshType, sizeof(Engine::MESHTYPE), &dwByte, nullptr);
			WriteFile(hFile, &fRadius, sizeof(_float), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}

}


void CColliderTool::OnBnClickedColliderLoadButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


BOOL CColliderTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CColliderTool::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (m_eFocusType)
	{
	case CColliderTool::POSX:
		if (zDelta > 0)
		{
			m_vecPos.x += 1.f;
		}
		else
		{
			m_vecPos.x -= 1.f;
		}
		break;
	case CColliderTool::POSY:
		if (zDelta > 0)
		{
			m_vecPos.y += 1.f;
		}
		else
		{
			m_vecPos.y -= 1.f;
		}
		break;
	case CColliderTool::POSZ:
		if (zDelta > 0)
		{
			m_vecPos.z += 1.f;
		}
		else
		{
			m_vecPos.z -= 1.f;
		}
		break;
	case CColliderTool::RADIUS:
		if (zDelta > 0)
		{
			m_fRadius += 1.f;
		}
		else
		{
			m_fRadius -= 1.f;
		}
		break;
	case CColliderTool::FOCUS_END:
		break;
	default:
		break;
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CColliderTool::InitMeshList()
{
	CFileFind finder;
	CString fileName = _T("");
	CString filePath = _T("..\\..\\Resource\\Mesh\\DynamicMesh\\");
	BOOL bWorking = finder.FindFile(filePath + _T("*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		fileName = finder.GetFileName();

		if (finder.IsDirectory())
		{
			SearchRecursive(filePath + fileName);
		}
		else
		{
			//���⼭ tga ����
			int Position = fileName.Find(_T(".")) + 1;
			CString cstrexeName = fileName.Mid(Position);
			if ((cstrexeName == _T("X")))
				m_ListBoxMesh.InsertString(m_iMeshNum++, fileName);
		}
			
	}
}

void CColliderTool::SearchRecursive(CString pFilePath)
{
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
			SearchRecursive(filePath + fileName);
		}
		else
		{
			//���⼭ tga ����
			int Position = fileName.Find(_T(".")) + 1;
			CString cstrexeName = fileName.Mid(Position);
			if ((cstrexeName == _T("X")))
				m_ListBoxMesh.InsertString(m_iMeshNum++, fileName);
		}
	}
}


BOOL CColliderTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitMeshList();
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	m_fRadius = 10.f;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CColliderTool::OnLbnSelchangeMeshList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nCursel = m_ListBoxMesh.GetCurSel();
	if (nCursel >= 0)
	{
		m_ListBoxMesh.GetText(nCursel, m_cstrSelectedMeshName);
	}
	else
		return;
	if (m_cstrSelectedMeshName == _T(""))
		return;
	CString cstrSelectedRealName = _T("");

	AfxExtractSubString(cstrSelectedRealName, m_cstrSelectedMeshName, 0, '.');
	wstring wstrObjTag = cstrSelectedRealName;
	//���� �ִϸ��̼� ����Ʈ �� �ҷ��;��� & �ش� dynamicmesh ����
	//DynamicMesh ����
	g_pMapScene->LayerClear(Engine::DYNAMICMESH);

	Engine::CGameObject* pSelectedMesh = nullptr;
	pSelectedMesh = CToolDynamicMeshObject::Create(g_pToolView->Get_GraphicDev(), L"Mesh_" + wstrObjTag);
	m_pSelectedMesh = (CToolDynamicMeshObject*)pSelectedMesh;
	g_pMapScene->Add_GameObject(Engine::DYNAMICMESH, wstrObjTag, pSelectedMesh);


	//�� ����Ʈ ����
	m_ListBoxBone.ResetContent();

	vector<wstring> vecBoneNames;
	((CToolDynamicMeshObject*)pSelectedMesh)->Get_DynamicMeshCom()->GetBoneNames(vecBoneNames);
	for (auto& iter = vecBoneNames.begin(); iter != vecBoneNames.end(); ++iter)
	{
		m_ListBoxBone.AddString((*iter).c_str());
	}


	//���ϸ��̼� ����Ʈ ����
	m_ListBoxAnimation.ResetContent();
	LPD3DXANIMATIONCONTROLLER pAniCtrl = ((CToolDynamicMeshObject*)pSelectedMesh)->Get_DynamicMeshCom()->GetAniCtrl()->Get_AniCtrl();

	for (_uint i = 0; i < pAniCtrl->GetNumAnimationSets(); ++i)
	{
		CString cstrNum = _T("");
		cstrNum.Format(_T("%d"), i);
		m_ListBoxAnimation.AddString(cstrNum);
	}

	m_iColliderNum = 0;
}


void CColliderTool::OnLbnSelchangeBoneList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	_uint nCursel = m_ListBoxBone.GetCurSel();
	if (nCursel < 0)
		return;
	m_ListBoxBone.GetText(nCursel, m_cstrSelectedBoneName);

}


void CColliderTool::OnLbnSelchangeAnimationList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	_uint nCursel = m_ListBoxAnimation.GetCurSel();
	if (nCursel < 0)
		return;
	m_pSelectedMesh->Set_PlayingAniNum(nCursel);
}


void CColliderTool::OnLbnSelchangeColliderList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	_uint iCurNum = m_ListBoxCollider.GetCurSel();
	if (iCurNum < 0)
		return;
	m_pSelectedMesh->Set_CollTrue(iCurNum);
}
