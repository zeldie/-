// ColliderTool.cpp : 구현 파일입니다.
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
// CColliderTool 대화 상자입니다.

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


// CColliderTool 메시지 처리기입니다.


void CColliderTool::OnBnClickedColliderCube()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eColliderType = COLLIDERTYPE::CUBE;
}


void CColliderTool::OnBnClickedColliderSphere()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eColliderType = COLLIDERTYPE::SPHERE;
}


void CColliderTool::OnBnClickedAnimationPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pSelectedMesh->Set_PlayAnimation(true);
}


void CColliderTool::OnBnClickedAnimationPause()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pSelectedMesh->Set_PlayAnimation(false);
}


void CColliderTool::OnBnClickedPosResetButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	UpdateData(FALSE);
}


void CColliderTool::OnBnClickedResetAllButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	m_fRadius = 1.f;
	UpdateData(FALSE);
}


void CColliderTool::OnEnSetfocusPosX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eFocusType = POSX;
}


void CColliderTool::OnEnUpdatePosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.
	UpdateData(TRUE);
	
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CColliderTool::OnEnSetfocusPosY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eFocusType = POSY;
}


void CColliderTool::OnEnUpdatePosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CColliderTool::OnEnSetfocusPosZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eFocusType = POSZ;
}


void CColliderTool::OnEnUpdatePosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CColliderTool::OnEnSetfocusRadius()
{
	m_eFocusType = RADIUS;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CColliderTool::OnEnUpdateRadius()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.
	UpdateData(TRUE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CColliderTool::OnBnClickedColliderCreateButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CColliderTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CColliderTool::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
			//여기서 tga 빼자
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
			//여기서 tga 빼자
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitMeshList();
	m_vecPos = _vec3(0.f, 0.f, 0.f);
	m_fRadius = 10.f;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CColliderTool::OnLbnSelchangeMeshList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	//뼈랑 애니메이션 리스트 쫙 불러와야함 & 해당 dynamicmesh 생성
	//DynamicMesh 생성
	g_pMapScene->LayerClear(Engine::DYNAMICMESH);

	Engine::CGameObject* pSelectedMesh = nullptr;
	pSelectedMesh = CToolDynamicMeshObject::Create(g_pToolView->Get_GraphicDev(), L"Mesh_" + wstrObjTag);
	m_pSelectedMesh = (CToolDynamicMeshObject*)pSelectedMesh;
	g_pMapScene->Add_GameObject(Engine::DYNAMICMESH, wstrObjTag, pSelectedMesh);


	//뼈 리스트 생성
	m_ListBoxBone.ResetContent();

	vector<wstring> vecBoneNames;
	((CToolDynamicMeshObject*)pSelectedMesh)->Get_DynamicMeshCom()->GetBoneNames(vecBoneNames);
	for (auto& iter = vecBoneNames.begin(); iter != vecBoneNames.end(); ++iter)
	{
		m_ListBoxBone.AddString((*iter).c_str());
	}


	//에니메이션 리스트 생성
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_uint nCursel = m_ListBoxBone.GetCurSel();
	if (nCursel < 0)
		return;
	m_ListBoxBone.GetText(nCursel, m_cstrSelectedBoneName);

}


void CColliderTool::OnLbnSelchangeAnimationList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_uint nCursel = m_ListBoxAnimation.GetCurSel();
	if (nCursel < 0)
		return;
	m_pSelectedMesh->Set_PlayingAniNum(nCursel);
}


void CColliderTool::OnLbnSelchangeColliderList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_uint iCurNum = m_ListBoxCollider.GetCurSel();
	if (iCurNum < 0)
		return;
	m_pSelectedMesh->Set_CollTrue(iCurNum);
}
