// EffectTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "EffectTool.h"
#include "afxdialogex.h"

#include "Layer.h"
#include "ToolView.h"
#include "MapScene.h"
#include "GameObject.h"
#include "Transform.h"
#include "ToolGameEffectStaticMeshObject.h"
#include "ToolEffectDynamicMeshObject.h"
#include "ToolEffectTextureObject.h"

#include "ToolGameObject.h"

//#include "ToolGameStaticMeshObject.h"
//#include "ToolDynamicMeshObject.h"

// CEffectTool 대화 상자입니다.


CEffectTool*	g_pEffectTool;
//double			g_dTimeBegin;

IMPLEMENT_DYNAMIC(CEffectTool, CDialog)

CEffectTool::CEffectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EFFECTTOOL, pParent)
	, m_fPosX(0.f)
	, m_fPosY(0.f)
	, m_fPosZ(0.f)
	, m_fRotX(0.f)
	, m_fRotY(0.f)
	, m_fRotZ(0.f)
	, m_fScaleX(1.f)
	, m_fScaleY(1.f)
	, m_fScaleZ(1.f)
	, m_fUVPosU(0.f)
	, m_fUVPosV(0.f)
	, m_fUVAtlasU(1.f)
	, m_fUVAtlasV(1.f)
	, m_fMovePosX(0.f)
	, m_fMovePosY(0.f)
	, m_fMovePosZ(0.f)
	, m_fMoveRotX(0.f)
	, m_fMoveRotY(0.f)
	, m_fMoveRotZ(0.f)
	, m_fMoveScaleX(0.f)
	, m_fMoveScaleY(0.f)
	, m_fMoveScaleZ(0.f)
	, m_fMoveUV_U(0.f)
	, m_fMoveUV_V(0.f)
	, m_fBeginTime_Start(0.f)
	, m_fBeginTime_End(0),
	m_iStaticMeshNum(0),
	m_iDynamicMeshNum(0),
	m_iTextureObjNum(0),
	m_iTextureNum(0),
	m_cstrTreeSelectedName(_T("")),
	m_cstrListBoxSelectedName(_T("")),
	m_cstrMeshType(_T("")),
	m_cstrTextureTreeSelectedName(_T("")),
	m_cstrTextureListBoxSelectedName(_T("")),
	m_pCurrentTransform(nullptr),
	m_pCurrentMeshObj(nullptr),
	m_bIsPlay(false)
	, m_iPass(5)
	, m_iAlpha(0)
	, m_fObjTime_End(0),
	m_iComboBoxCount(0)
	, m_fSpeed(0)
	, m_fSCALING(1.f)
{

}

CEffectTool::~CEffectTool()
{
}

void CEffectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POSX, m_fPosX);
	DDX_Text(pDX, IDC_EDIT_POSY, m_fPosY);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT_ROTX, m_fRotX);
	DDX_Text(pDX, IDC_EDIT_ROTY, m_fRotY);
	DDX_Text(pDX, IDC_EDIT_ROTZ, m_fRotZ);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT_UVPOSU, m_fUVPosU);
	DDX_Text(pDX, IDC_EDIT_UVPOSV, m_fUVPosV);
	DDX_Text(pDX, IDC_EDIT_ATLAS_U, m_fUVAtlasU);
	DDX_Text(pDX, IDC_EDIT_ATLAS_V, m_fUVAtlasV);
	DDX_Control(pDX, IDC_SPIN_POS_X, m_SpinCtrl_PosX);
	DDX_Control(pDX, IDC_SPIN_POS_Y, m_SpinCtrl_PosY);
	DDX_Control(pDX, IDC_SPIN_POS_Z, m_SpinCtrl_PosZ);
	DDX_Control(pDX, IDC_SPIN_ROT_X, m_SpinCtrl_RotX);
	DDX_Control(pDX, IDC_SPIN_ROT_Y, m_SpinCtrl_RotY);
	DDX_Control(pDX, IDC_SPIN_ROT_Z, m_SpinCtrl_RotZ);
	DDX_Control(pDX, IDC_SPIN_SCALE_X, m_SpinCtrl_ScaleX);
	DDX_Control(pDX, IDC_SPIN_SCALE_Y, m_SpinCtrl_ScaleY);
	DDX_Control(pDX, IDC_SPIN_SCALE_Z, m_SpinCtrl_ScaleZ);
	DDX_Control(pDX, IDC_SPIN_UVPOS_U, m_SpinCtrl_UVPosU);
	DDX_Control(pDX, IDC_SPIN_UVPOS_V, m_SpinCtrl_UVPosV);
	DDX_Control(pDX, IDC_SPIN_ATLAS_U, m_SpinCtrl_AtlasU);
	DDX_Control(pDX, IDC_SPIN_ATLAS_V, m_SpinCtrl_AtlasV);
	DDX_Text(pDX, IDC_EDIT_MOVE_POS_X, m_fMovePosX);
	DDX_Text(pDX, IDC_EDIT_MOVE_POS_Y, m_fMovePosY);
	DDX_Text(pDX, IDC_EDIT_MOVE_POS_Z, m_fMovePosZ);
	DDX_Text(pDX, IDC_EDIT_MOVE_ROT_X, m_fMoveRotX);
	DDX_Text(pDX, IDC_EDIT_MOVE_ROT_Y, m_fMoveRotY);
	DDX_Text(pDX, IDC_EDIT_MOVE_ROT_Z, m_fMoveRotZ);
	DDX_Text(pDX, IDC_EDIT_MOVE_SCALE_X, m_fMoveScaleX);
	DDX_Text(pDX, IDC_EDIT_MOVE_SCALE_Y, m_fMoveScaleY);
	DDX_Text(pDX, IDC_EDIT_MOVE_SCALE_Z, m_fMoveScaleZ);
	DDX_Text(pDX, IDC_EDIT_MOVE_UV_U, m_fMoveUV_U);
	DDX_Text(pDX, IDC_EDIT_MOVE_UV_V, m_fMoveUV_V);
	DDX_Text(pDX, IDC_EDIT_BEGINTIME_START, m_fBeginTime_Start);
	DDX_Text(pDX, IDC_EDIT_BEGINTIME_END, m_fBeginTime_End);
	DDX_Control(pDX, IDC_SPIN_MOVEPOS_X, m_SpinCtrl_MOVE_Pos_X);
	DDX_Control(pDX, IDC_SPIN_MOVEPOS_Y, m_SpinCtrl_MOVE_Pos_Y);
	DDX_Control(pDX, IDC_SPIN_MOVEPOS_Z, m_SpinCtrl_MOVE_Pos_Z);
	DDX_Control(pDX, IDC_SPIN_MOVEROT_X, m_SpinCtrl_MOVE_Rot_X);
	DDX_Control(pDX, IDC_SPIN_MOVEROT_Y, m_SpinCtrl_MOVE_Rot_Y);
	DDX_Control(pDX, IDC_SPIN_MOVEROT_Z, m_SpinCtrl_MOVE_Rot_Z);
	DDX_Control(pDX, IDC_SPIN_MOVESCALE_X, m_SpinCtrl_MOVE_Scale_X);
	DDX_Control(pDX, IDC_SPIN_MOVESCALE_Y, m_SpinCtrl_MOVE_Scale_Y);
	DDX_Control(pDX, IDC_SPIN_MOVESCALE_Z, m_SpinCtrl_MOVE_Scale_Z);
	DDX_Control(pDX, IDC_SPIN_MOVEUV_U, m_SpinCtrl_MOVE_UV_U);
	DDX_Control(pDX, IDC_SPIN_MOVEUV_V, m_SpinCtrl_MOVE_UV_V);
	DDX_Control(pDX, IDC_SPIN_TIMEBEGIN, m_SpinCtrl_BeginTime_Start);
	DDX_Control(pDX, IDC_SPIN_TIMEEND, m_SpinCtrl_BeginTime_End);
	DDX_Control(pDX, IDC_TREE_MESHLIST, m_TreeCtrlMesh);
	DDX_Control(pDX, IDC_LIST_STATICMESH, m_ListBoxStaticMesh);
	DDX_Control(pDX, IDC_LIST_DYNAMICMESH, m_ListBoxDynamicMesh);
	DDX_Control(pDX, IDC_TREE_TEXTURE_OBJ, m_TreeCtrlTextureOBJ);
	DDX_Control(pDX, IDC_TREE_TEXTURE, m_TreeCtrlTexture);
	DDX_Control(pDX, IDC_LIST_TEXTURE_OBJ, m_ListBoxTextureObj);
	DDX_Control(pDX, IDC_LIST_TEXTURE, m_ListBoxTexture);
	DDX_Text(pDX, IDC_EDIT_PASS, m_iPass);
	DDX_Control(pDX, IDC_SPIN_PASS, m_SpinCtrl_Pass);
	DDX_Control(pDX, IDC_SPIN_OBJTIME_END, m_SpinCtrl_ObjTime_End);
	DDX_Text(pDX, IDC_EDIT_OBJTIME_END, m_fObjTime_End);
	DDX_Control(pDX, IDC_COMBO2, m_ComboBoxCtrl_Type);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_fSpeed);
	DDX_Control(pDX, IDC_SPIN_SPEED, m_SpinCtrl_Speed);
}

void CEffectTool::InitTreeCtrl()
{
	HTREEITEM hParent = m_TreeCtrlMesh.InsertItem(L"Mesh");

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
			m_TreeCtrlMesh.InsertItem(fileName, 0, 0, hParent, TVI_LAST);
	}
	m_TreeCtrlMesh.Expand(hParent, TVE_EXPAND);
}

void CEffectTool::InitTreeCtrlTextureOBJ()
{
	HTREEITEM hParent = m_TreeCtrlTextureOBJ.InsertItem(L"TextureOBJ");

	CFileFind finder;
	CString fileName = _T("");
	CString filePath = _T("..\\..\\Resource\\Texture\\Effect\\EffectALL\\");
	BOOL bWorking = finder.FindFile(filePath + _T("*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		fileName = finder.GetFileName();

		if (finder.IsDirectory())
		{
			SearchRecursiveCnt(hParent, fileName, filePath + fileName);
		}
		else
		{
			//int Position = fileName.Find(_T(".")) - 1;
			CString cstrexeName = fileName.Mid(9);
			TCHAR szFileName[MAX_STR] = L"";
			StrCpy(szFileName, cstrexeName);
			PathRemoveExtension(szFileName);
			m_TreeCtrlTextureOBJ.InsertItem(szFileName, 0, 0, hParent, TVI_LAST);
		}
	}
	m_TreeCtrlTextureOBJ.Expand(hParent, TVE_EXPAND);
}

void CEffectTool::InitTreeCtrlTexture()
{
	HTREEITEM hParent = m_TreeCtrlTexture.InsertItem(L"Texture");

	CFileFind finder;
	CString fileName = _T("");
	CString filePath = _T("..\\..\\Resource\\Texture\\Effect\\EffectALL\\");
	BOOL bWorking = finder.FindFile(filePath + _T("*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		fileName = finder.GetFileName();

		if (finder.IsDirectory())
		{
			SearchRecursiveCnt(hParent, fileName, filePath + fileName);
		}
		else
		{
			//int Position = fileName.Find(_T(".")) - 1;
			CString cstrexeName = fileName.Mid(9);
			TCHAR szFileName[MAX_STR] = L"";
			StrCpy(szFileName, cstrexeName);
			PathRemoveExtension(szFileName);
			m_TreeCtrlTexture.InsertItem(szFileName, 0, 0, hParent, TVI_LAST);
		}
	}
	m_TreeCtrlTexture.Expand(hParent, TVE_EXPAND);
}

void CEffectTool::SearchRecursive(HTREEITEM hParent, CString Child, CString pFilePath)
{
	HTREEITEM hChild = m_TreeCtrlMesh.InsertItem(Child, 0, 0, hParent, TVI_LAST);

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
			//여기서 tga 빼자
			int Position = fileName.Find(_T(".")) + 1;
			CString cstrexeName = fileName.Mid(Position);
			if ((cstrexeName == _T("X")))
				m_TreeCtrlMesh.InsertItem(fileName, 0, 0, hChild, TVI_LAST);
		}
	}
}

void CEffectTool::SearchRecursiveCnt(HTREEITEM hParent, CString Child, CString pFilePath)
{
	HTREEITEM hChild = m_TreeCtrlMesh.InsertItem(Child, 0, 0, hParent, TVI_LAST);

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
			SearchRecursiveCnt(hChild, fileName, filePath + fileName);
		}
		else
		{
			//여기서 tga 빼자
			int Position = fileName.Find(_T("."));
			CString cstrexeName = fileName.Mid(9, Position);
			m_TreeCtrlMesh.InsertItem(fileName, 0, 0, hChild, TVI_LAST);
		}
	}
}

_int CEffectTool::SearchSameName(CString cstrname, CString cstrMesyType)
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
	else if (cstrMesyType == _T("TextureOBJ"))
	{
		for (int i = 0; i < m_iTextureObjNum; ++i)
		{
			m_ListBoxTextureObj.GetText(i, FullName);
			AfxExtractSubString(SelectedName, FullName, 0, '-');
			if (cstrname == SelectedName)
				++iSameName;
		}
	}
	return iSameName;
}


BEGIN_MESSAGE_MAP(CEffectTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CEffectTool::OnBnClickedButtonSave)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POS_X, &CEffectTool::OnDeltaposSpinPosX)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CEffectTool::OnBnClickedButtonLoad)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POS_Y, &CEffectTool::OnDeltaposSpinPosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POS_Z, &CEffectTool::OnDeltaposSpinPosZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ROT_X, &CEffectTool::OnDeltaposSpinRotX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ROT_Y, &CEffectTool::OnDeltaposSpinRotY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ROT_Z, &CEffectTool::OnDeltaposSpinRotZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCALE_X, &CEffectTool::OnDeltaposSpinScaleX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCALE_Y, &CEffectTool::OnDeltaposSpinScaleY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCALE_Z, &CEffectTool::OnDeltaposSpinScaleZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_UVPOS_U, &CEffectTool::OnDeltaposSpinUvposU)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_UVPOS_V, &CEffectTool::OnDeltaposSpinUvposV)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ATLAS_U, &CEffectTool::OnDeltaposSpinAtlasU)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ATLAS_V, &CEffectTool::OnDeltaposSpinAtlasV)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVEPOS_X, &CEffectTool::OnDeltaposSpinMoveposX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVEPOS_Y, &CEffectTool::OnDeltaposSpinMoveposY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVEPOS_Z, &CEffectTool::OnDeltaposSpinMoveposZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVEROT_X, &CEffectTool::OnDeltaposSpinMoverotX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVEROT_Y, &CEffectTool::OnDeltaposSpinMoverotY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVEROT_Z, &CEffectTool::OnDeltaposSpinMoverotZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVESCALE_X, &CEffectTool::OnDeltaposSpinMovescaleX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVESCALE_Y, &CEffectTool::OnDeltaposSpinMovescaleY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVESCALE_Z, &CEffectTool::OnDeltaposSpinMovescaleZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVEUV_U, &CEffectTool::OnDeltaposSpinMoveuvU)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVEUV_V, &CEffectTool::OnDeltaposSpinMoveuvV)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TIMEBEGIN, &CEffectTool::OnDeltaposSpinTimebegin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TIMEEND, &CEffectTool::OnDeltaposSpinTimeend)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MESHLIST, &CEffectTool::OnTvnSelchangedTreeMeshlist)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MESH, &CEffectTool::OnBnClickedButtonAddMesh)
	ON_BN_CLICKED(IDC_RADIO_ALPHA, &CEffectTool::OnBnClickedRadioAlpha)
	ON_BN_CLICKED(IDC_RADIO_NONALPHA, &CEffectTool::OnBnClickedRadioNonalpha)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_TEXTURE_OBJ, &CEffectTool::OnTvnSelchangedTreeTextureObj)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_TEXTURE, &CEffectTool::OnTvnSelchangedTreeTexture)
	ON_LBN_SELCHANGE(IDC_LIST_STATICMESH, &CEffectTool::OnLbnSelchangeListStaticmesh)
	ON_LBN_SELCHANGE(IDC_LIST_DYNAMICMESH, &CEffectTool::OnLbnSelchangeListDynamicmesh)
	ON_BN_CLICKED(IDC_BUTTON_START, &CEffectTool::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CEffectTool::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CEffectTool::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TEXTURE, &CEffectTool::OnBnClickedButtonAddTexture)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PASS, &CEffectTool::OnDeltaposSpinPass)
	ON_LBN_SELCHANGE(IDC_LIST_TEXTURE, &CEffectTool::OnLbnSelchangeListTexture)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TEXTURE_OBJ, &CEffectTool::OnBnClickedButtonAddTextureObj)
	ON_LBN_SELCHANGE(IDC_LIST_TEXTURE_OBJ, &CEffectTool::OnLbnSelchangeListTextureObj)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_DYNAMICMESH, &CEffectTool::OnBnClickedButtonDeleteDynamicmesh)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_STATICMESH, &CEffectTool::OnBnClickedButtonDeleteStaticmesh)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TEXTURE_OBJ, &CEffectTool::OnBnClickedButtonDeleteTextureObj)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TEXTURE, &CEffectTool::OnBnClickedButtonDeleteTexture)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_OBJTIME_END, &CEffectTool::OnDeltaposSpinObjtimeEnd)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CEffectTool::OnCbnSelchangeCombo2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED, &CEffectTool::OnDeltaposSpinSpeed)
	ON_BN_CLICKED(IDC_BUTTON_ALL_APPLY, &CEffectTool::OnBnClickedButtonAllApply)
END_MESSAGE_MAP()


// CEffectTool 메시지 처리기입니다.

void CEffectTool::Update_OBJData()
{
	if (m_pCurrentTransform == nullptr)
		return;

	g_dTimeEnd = m_fBeginTime_End;

	_vec3 vPos = { m_fPosX,m_fPosY,m_fPosZ };
	_vec3 vScale = { m_fScaleX,m_fScaleY,m_fScaleZ };
	m_pCurrentTransform->Set_Info(&vPos, Engine::INFO_POS);
	m_pCurrentTransform->Set_Angle(m_fRotX, m_fRotY, m_fRotZ);
	m_pCurrentTransform->Set_Scale(&vScale);

	if (m_pCurrentMeshObj == nullptr)
		return;

	m_pCurrentMeshObj->m_vOrginPos.x = m_fPosX;
	m_pCurrentMeshObj->m_vOrginPos.y = m_fPosY;
	m_pCurrentMeshObj->m_vOrginPos.z = m_fPosZ;
	m_pCurrentMeshObj->m_vOrginRot.x = m_fRotX;
	m_pCurrentMeshObj->m_vOrginRot.y = m_fRotY;
	m_pCurrentMeshObj->m_vOrginRot.z = m_fRotZ;
	m_pCurrentMeshObj->m_vOrginScale.x = m_fScaleX;
	m_pCurrentMeshObj->m_vOrginScale.y = m_fScaleY;
	m_pCurrentMeshObj->m_vOrginScale.z = m_fScaleZ;

	m_pCurrentMeshObj->m_vMovePos.x = m_fMovePosX;
	m_pCurrentMeshObj->m_vMovePos.y = m_fMovePosY;
	m_pCurrentMeshObj->m_vMovePos.z = m_fMovePosZ;
	m_pCurrentMeshObj->m_vMoveRot.x = m_fMoveRotX;
	m_pCurrentMeshObj->m_vMoveRot.y = m_fMoveRotY;
	m_pCurrentMeshObj->m_vMoveRot.z = m_fMoveRotZ;
	m_pCurrentMeshObj->m_vMoveScale.x = m_fMoveScaleX;
	m_pCurrentMeshObj->m_vMoveScale.y = m_fMoveScaleY;
	m_pCurrentMeshObj->m_vMoveScale.z = m_fMoveScaleZ;

	m_pCurrentMeshObj->m_fDeadTime = m_fObjTime_End;
	m_pCurrentMeshObj->m_iAlpha = m_iAlpha;
	m_pCurrentMeshObj->m_fSpeed = m_fSpeed;


	if (m_cstrTextureListBoxSelectedName == _T(""))
		return;

	Engine::EFFECT_UV_INFO	eInfo;
	eInfo.iPass = m_iPass;
	eInfo.vUVAtlas.x = m_fUVAtlasU;
	eInfo.vUVAtlas.y = m_fUVAtlasV;
	eInfo.vUVMovePos.x = m_fMoveUV_U;
	eInfo.vUVMovePos.y = m_fMoveUV_V;
	eInfo.vUVPos.x = m_fUVPosU;
	eInfo.vUVPos.y = m_fUVPosV;

	m_pCurrentMeshObj->Set_TextureInfo((_uint)_tstoi(m_cstrTextureListBoxSelectedName), eInfo);
	//UpdateData(TRUE);
}

void CEffectTool::Update_ToolData()
{
	if (m_pCurrentTransform == nullptr)
		return;

	m_fPosX = m_pCurrentTransform->Get_Info(Engine::INFO_POS)->x;
	m_fPosY = m_pCurrentTransform->Get_Info(Engine::INFO_POS)->y;
	m_fPosZ = m_pCurrentTransform->Get_Info(Engine::INFO_POS)->z;
	m_fRotX = m_pCurrentTransform->Get_Angle()->x;
	m_fRotY = m_pCurrentTransform->Get_Angle()->y;
	m_fRotZ = m_pCurrentTransform->Get_Angle()->z;
	m_fScaleX = m_pCurrentTransform->Get_Scale()->x;
	m_fScaleY = m_pCurrentTransform->Get_Scale()->y;
	m_fScaleZ = m_pCurrentTransform->Get_Scale()->z;

	m_fMovePosX = m_pCurrentMeshObj->m_vMovePos.x;
	m_fMovePosY = m_pCurrentMeshObj->m_vMovePos.y;
	m_fMovePosZ = m_pCurrentMeshObj->m_vMovePos.z;
	m_fMoveRotX = m_pCurrentMeshObj->m_vMoveRot.x;
	m_fMoveRotY = m_pCurrentMeshObj->m_vMoveRot.y;
	m_fMoveRotZ = m_pCurrentMeshObj->m_vMoveRot.z;
	m_fMoveScaleX = m_pCurrentMeshObj->m_vMoveScale.x;
	m_fMoveScaleY = m_pCurrentMeshObj->m_vMoveScale.y;
	m_fMoveScaleZ = m_pCurrentMeshObj->m_vMoveScale.z;
	m_fSpeed = m_pCurrentMeshObj->m_fSpeed;

	m_fObjTime_End = m_pCurrentMeshObj->m_fDeadTime;
	m_iAlpha = m_pCurrentMeshObj->m_iAlpha;
}

void CEffectTool::Reset_TextrueList()
{
	if (m_pCurrentMeshObj == nullptr)
		return;

	m_ListBoxTexture.ResetContent();
	m_iTextureNum = 0;

	Engine::EFFECT_UV_INFO*	pEffectInfo = nullptr;

	for (auto iter : m_pCurrentMeshObj->m_vecTextureInfo)
	{
		CString str;
		str.Format(_T("%d"), (iter.iTextureName));
		m_ListBoxTexture.InsertString(m_iTextureNum++, str);
	}
}

BOOL CEffectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitTreeCtrl();
	InitTreeCtrlTexture();
	InitTreeCtrlTextureOBJ();

	m_SpinCtrl_PosX.SetRange(-30000, 30000);
	m_SpinCtrl_PosY.SetRange(-30000, 30000);
	m_SpinCtrl_PosZ.SetRange(-30000, 30000);

	m_SpinCtrl_RotX.SetRange(-30000, 30000);
	m_SpinCtrl_RotY.SetRange(-30000, 30000);
	m_SpinCtrl_RotZ.SetRange(-30000, 30000);

	m_SpinCtrl_ScaleX.SetRange(-30000, 30000);
	m_SpinCtrl_ScaleY.SetRange(-30000, 30000);
	m_SpinCtrl_ScaleZ.SetRange(-30000, 30000);

	m_SpinCtrl_UVPosU.SetRange(-30000, 30000);
	m_SpinCtrl_UVPosV.SetRange(-30000, 30000);

	m_SpinCtrl_AtlasU.SetRange(-30000, 30000);
	m_SpinCtrl_AtlasV.SetRange(-30000, 30000);

	m_SpinCtrl_MOVE_Pos_X.SetRange(-30000, 30000);
	m_SpinCtrl_MOVE_Pos_Y.SetRange(-30000, 30000);
	m_SpinCtrl_MOVE_Pos_Z.SetRange(-30000, 30000);

	m_SpinCtrl_MOVE_Rot_X.SetRange(-30000, 30000);
	m_SpinCtrl_MOVE_Rot_Y.SetRange(-30000, 30000);
	m_SpinCtrl_MOVE_Rot_Z.SetRange(-30000, 30000);

	m_SpinCtrl_MOVE_Scale_X.SetRange(-30000, 30000);
	m_SpinCtrl_MOVE_Scale_Y.SetRange(-30000, 30000);
	m_SpinCtrl_MOVE_Scale_Z.SetRange(-30000, 30000);

	m_SpinCtrl_MOVE_UV_U.SetRange(-30000, 30000);
	m_SpinCtrl_MOVE_UV_V.SetRange(-30000, 30000);
	m_SpinCtrl_BeginTime_Start.SetRange(-30000, 30000);
	m_SpinCtrl_BeginTime_End.SetRange(-30000, 30000);
	m_SpinCtrl_ObjTime_End.SetRange(-30000, 30000);

	m_SpinCtrl_Pass.SetRange(-30000, 30000);
	m_SpinCtrl_Speed.SetRange(-30000, 30000);


	//=== 이펙트 갯수만큼 만들어야함=====================================
	//m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"TS");
	/*m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"TEST");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"=END=");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"--YH--");*/

	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"Thrust_Belatos");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"ShockWave_Belatos");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"Arrow_Belatos");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"Circle_Belatos");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"ShieldFront_Belatos");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"Circle2_Belatos");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"Circle3_Belatos");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"Hemisphere_Belatos");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"Hemisphere2_Belatos");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"Attack_Belatos");

	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"Sweep_Belatos");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"JumpLight_Belatos");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");

	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");

	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");

	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");

	//			여기서부턴 용호

	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_BASICTRAIL1");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_BASICTRAIL2");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_BASICTRAIL3");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_BASICTRAIL4");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_BASICTRAIL5");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_BASICTRAIL6");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_BASICTRAIL7");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_BASICTRAIL8");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_AIRTRAIL1");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_AIRTRAIL2");

	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_AIRTRAIL3");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_AIRTRAIL4");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_AIRTRAIL5");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_AIRBULLET1");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EFFECT_AIRBULLET2");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");

	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");

	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");

	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	m_ComboBoxCtrl_Type.InsertString(m_iComboBoxCount++, L"EMPTY");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CEffectTool::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	//CFileDialog Dlg(FALSE,
	//	L"dat",
	//	L"*.dat",
	//	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	//	L"Data File(*.dat)||",
	//	this);
	CFileDialog Dlg(TRUE,
		L".txt", 
		L"*.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
		this);
	TCHAR szFilePath[MAX_STR] = L"";
	GetCurrentDirectory(MAX_STR, szFilePath);

	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\Effect");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (Dlg.DoModal())
	{
		wofstream fout;

		fout.open(Dlg.GetPathName());

		if (fout.fail())
			return;
		//CString StrPath = Dlg.GetPathName();
		//HANDLE hFile = CreateFile(StrPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		//if (INVALID_HANDLE_VALUE == hFile)
		//	return;

		DWORD dwByte = 0;
		DWORD dwstrLayerLen = 0;
		DWORD dwstrObjLen = 0;
		wstring wstrLayerTag = L"";
		CString cstrLayerTag = _T("");
		wstring wstrObjTag = L"";
		CString cstrObjTag = _T("");
		_vec3	vPos = _vec3(0.f, 0.f, 0.f);
		_vec3	vRot = _vec3(0.f, 0.f, 0.f);
		_vec3	vScale = _vec3(0.f, 0.f, 0.f);
		_vec3	vMovePos = _vec3(0.f, 0.f, 0.f);
		_vec3	vMoveRot = _vec3(0.f, 0.f, 0.f);
		_vec3	vMoveScale = _vec3(0.f, 0.f, 0.f);
		_int	iVecSize = 0;
		_int	iAlpha = 0;
		_float	fDeadTime = 0.f;
		_float	fSpeed = 0.f;
		vector<Engine::EFFECT_UV_INFO>	vecUVInfo;

		Engine::CLayer* pLayer = nullptr;
		map<wstring, list<Engine::CGameObject*>> mapGameObject;

		// 이펙트이넘값
		//WriteFile(hFile, &m_eEffectType, sizeof(_int), &dwByte, nullptr);
		fout << (int)m_eEffectType << endl;

		////다이나믹매쉬부터 저장
		//wstrLayerTag = L"DynamicMesh";
		//cstrLayerTag = CString::CStringT(wstrLayerTag.c_str());
		//dwstrLayerLen = sizeof(TCHAR) * (cstrLayerTag.GetLength() + 1);
		pLayer = g_pMapScene->Get_Layer(Engine::DYNAMICMESH);
		mapGameObject = pLayer->Get_mapGameObject();
		for (auto& Objectlist : mapGameObject)
		{
			wstrObjTag = Objectlist.first;
			cstrObjTag = CString::CStringT(wstrObjTag.c_str());
			dwstrObjLen = sizeof(TCHAR) * (cstrObjTag.GetLength() + 1);
			for (auto& Object : Objectlist.second)
			{
				vPos = dynamic_cast<CToolGameObject*>(Object)->m_vOrginPos;
				vRot = dynamic_cast<CToolGameObject*>(Object)->m_vOrginRot;
				vScale = dynamic_cast<CToolGameObject*>(Object)->m_vOrginScale;

				vMovePos = dynamic_cast<CToolGameObject*>(Object)->m_vMovePos;
				vMoveRot = dynamic_cast<CToolGameObject*>(Object)->m_vMoveRot;
				vMoveScale = dynamic_cast<CToolGameObject*>(Object)->m_vMoveScale;

				fSpeed = dynamic_cast<CToolGameObject*>(Object)->m_fSpeed;
				fDeadTime = dynamic_cast<CToolGameObject*>(Object)->m_fDeadTime;
				iAlpha = dynamic_cast<CToolGameObject*>(Object)->m_iAlpha;
				vecUVInfo = dynamic_cast<CToolGameObject*>(Object)->m_vecTextureInfo;
				iVecSize = vecUVInfo.size();

				////이제 저장
				////레이어이름 저장
				//WriteFile(hFile, &dwstrLayerLen, sizeof(DWORD), &dwByte, nullptr);
				//WriteFile(hFile, cstrLayerTag.GetString(), dwstrLayerLen, &dwByte, nullptr);

				//// 이펙트이넘값
				//WriteFile(hFile, &m_eEffectType, sizeof(_int), &dwByte, nullptr);

				//오브젝트이름 저장
				//WriteFile(hFile, &dwstrObjLen, sizeof(DWORD), &dwByte, nullptr);
				//WriteFile(hFile, cstrObjTag.GetString(), dwstrObjLen, &dwByte, nullptr);
				fout << L"DynamicMesh" << endl;
				fout << cstrObjTag.GetString() << endl;

				fout << vPos.x << endl;
				fout << vPos.y << endl;
				fout << vPos.z << endl;
				fout << vRot.x << endl;
				fout << vRot.y << endl;
				fout << vRot.z << endl;
				fout << vScale.x << endl;
				fout << vScale.y << endl;
				fout << vScale.z << endl;

				fout << vMovePos.x << endl;
				fout << vMovePos.y << endl;
				fout << vMovePos.z << endl;
				fout << vMoveRot.x << endl;
				fout << vMoveRot.y << endl;
				fout << vMoveRot.z << endl;
				fout << vMoveScale.x << endl;
				fout << vMoveScale.y << endl;
				fout << vMoveScale.z << endl;

				fout << fSpeed << endl;
				fout << fDeadTime << endl;
				fout << iAlpha << endl;
				fout << iVecSize << endl;

				for (int i = 0; i < iVecSize; ++i)
				{
					fout << vecUVInfo[i].iTextureName << endl;
					fout << vecUVInfo[i].vUVPos.x << endl;
					fout << vecUVInfo[i].vUVPos.y << endl;
					fout << vecUVInfo[i].vUVMovePos.x << endl;
					fout << vecUVInfo[i].vUVMovePos.y << endl;
					fout << vecUVInfo[i].vUVAtlas.x << endl;
					fout << vecUVInfo[i].vUVAtlas.y << endl;
					fout << vecUVInfo[i].iPass << endl;
				}
			}
		}

		////스태틱매쉬저장
		//wstrLayerTag = L"StaticMesh";
		//cstrLayerTag = CString::CStringT(wstrLayerTag.c_str());
		//dwstrLayerLen = sizeof(TCHAR) * (cstrLayerTag.GetLength() + 1);
		pLayer = g_pMapScene->Get_Layer(Engine::STATICMESH);
		mapGameObject = pLayer->Get_mapGameObject();
		for (auto& Objectlist : mapGameObject)
		{
			wstrObjTag = Objectlist.first;
			cstrObjTag = CString::CStringT(wstrObjTag.c_str());
			dwstrObjLen = sizeof(TCHAR) * (cstrObjTag.GetLength() + 1);
			for (auto& Object : Objectlist.second)
			{
				vPos = dynamic_cast<CToolGameObject*>(Object)->m_vOrginPos;
				vRot = dynamic_cast<CToolGameObject*>(Object)->m_vOrginRot;
				vScale = dynamic_cast<CToolGameObject*>(Object)->m_vOrginScale;

				vMovePos = dynamic_cast<CToolGameObject*>(Object)->m_vMovePos;
				vMoveRot = dynamic_cast<CToolGameObject*>(Object)->m_vMoveRot;
				vMoveScale = dynamic_cast<CToolGameObject*>(Object)->m_vMoveScale;

				fSpeed = dynamic_cast<CToolGameObject*>(Object)->m_fSpeed;
				fDeadTime = dynamic_cast<CToolGameObject*>(Object)->m_fDeadTime;
				iAlpha = dynamic_cast<CToolGameObject*>(Object)->m_iAlpha;
				vecUVInfo = dynamic_cast<CToolGameObject*>(Object)->m_vecTextureInfo;
				iVecSize = vecUVInfo.size();

				////이제 저장
				////레이어이름 저장
				//WriteFile(hFile, &dwstrLayerLen, sizeof(DWORD), &dwByte, nullptr);
				//WriteFile(hFile, cstrLayerTag.GetString(), dwstrLayerLen, &dwByte, nullptr);

				//// 이펙트이넘값
				//WriteFile(hFile, &m_eEffectType, sizeof(_int), &dwByte, nullptr);

				//오브젝트이름 저장
				//WriteFile(hFile, &dwstrObjLen, sizeof(DWORD), &dwByte, nullptr);
				fout << L"StaticMesh" << endl;
				fout << cstrObjTag.GetString() << endl;

				fout << vPos.x << endl;
				fout << vPos.y << endl;
				fout << vPos.z << endl;
				fout << vRot.x << endl;
				fout << vRot.y << endl;
				fout << vRot.z << endl;
				fout << vScale.x << endl;
				fout << vScale.y << endl;
				fout << vScale.z << endl;

				fout << vMovePos.x << endl;
				fout << vMovePos.y << endl;
				fout << vMovePos.z << endl;
				fout << vMoveRot.x << endl;
				fout << vMoveRot.y << endl;
				fout << vMoveRot.z << endl;
				fout << vMoveScale.x << endl;
				fout << vMoveScale.y << endl;
				fout << vMoveScale.z << endl;

				fout << fSpeed << endl;
				fout << fDeadTime << endl;
				fout << iAlpha << endl;
				fout << iVecSize << endl;

				for (int i = 0; i < iVecSize; ++i)
				{
					fout << vecUVInfo[i].iTextureName << endl;
					fout << vecUVInfo[i].vUVPos.x << endl;
					fout << vecUVInfo[i].vUVPos.y << endl;
					fout << vecUVInfo[i].vUVMovePos.x << endl;
					fout << vecUVInfo[i].vUVMovePos.y << endl;
					fout << vecUVInfo[i].vUVAtlas.x << endl;
					fout << vecUVInfo[i].vUVAtlas.y << endl;
					fout << vecUVInfo[i].iPass << endl;
				}
			}
		}
		//텍스쳐OBJ
		pLayer = g_pMapScene->Get_Layer(Engine::GAMEOBJECT);
		mapGameObject = pLayer->Get_mapGameObject();
		for (auto& Objectlist : mapGameObject)
		{
			wstrObjTag = Objectlist.first;
			cstrObjTag = CString::CStringT(wstrObjTag.c_str());
			dwstrObjLen = sizeof(TCHAR) * (cstrObjTag.GetLength() + 1);
			for (auto& Object : Objectlist.second)
			{
				vPos = dynamic_cast<CToolGameObject*>(Object)->m_vOrginPos;
				vRot = dynamic_cast<CToolGameObject*>(Object)->m_vOrginRot;
				vScale = dynamic_cast<CToolGameObject*>(Object)->m_vOrginScale;

				vMovePos = dynamic_cast<CToolGameObject*>(Object)->m_vMovePos;
				vMoveRot = dynamic_cast<CToolGameObject*>(Object)->m_vMoveRot;
				vMoveScale = dynamic_cast<CToolGameObject*>(Object)->m_vMoveScale;

				fSpeed = dynamic_cast<CToolGameObject*>(Object)->m_fSpeed;
				fDeadTime = dynamic_cast<CToolGameObject*>(Object)->m_fDeadTime;
				iAlpha = dynamic_cast<CToolGameObject*>(Object)->m_iAlpha;
				vecUVInfo = dynamic_cast<CToolGameObject*>(Object)->m_vecTextureInfo;
				iVecSize = vecUVInfo.size();

				////이제 저장
				fout << L"TextureOBJ" << endl;
				fout << cstrObjTag.GetString() << endl;

				fout << vPos.x << endl;
				fout << vPos.y << endl;
				fout << vPos.z << endl;
				fout << vRot.x << endl;
				fout << vRot.y << endl;
				fout << vRot.z << endl;
				fout << vScale.x << endl;
				fout << vScale.y << endl;
				fout << vScale.z << endl;

				fout << vMovePos.x << endl;
				fout << vMovePos.y << endl;
				fout << vMovePos.z << endl;
				fout << vMoveRot.x << endl;
				fout << vMoveRot.y << endl;
				fout << vMoveRot.z << endl;
				fout << vMoveScale.x << endl;
				fout << vMoveScale.y << endl;
				fout << vMoveScale.z << endl;

				fout << fSpeed << endl;
				fout << fDeadTime << endl;
				fout << iAlpha << endl;
				fout << iVecSize << endl;

				for (int i = 0; i < iVecSize; ++i)
				{
					fout << vecUVInfo[i].iTextureName << endl;
					fout << vecUVInfo[i].vUVPos.x << endl;
					fout << vecUVInfo[i].vUVPos.y << endl;
					fout << vecUVInfo[i].vUVMovePos.x << endl;
					fout << vecUVInfo[i].vUVMovePos.y << endl;
					fout << vecUVInfo[i].vUVAtlas.x << endl;
					fout << vecUVInfo[i].vUVAtlas.y << endl;
					fout << vecUVInfo[i].iPass << endl;
				}
			}
		}

		fout.close();
	}
}

void CEffectTool::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CEffectTool::OnDeltaposSpinPosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fPosX += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fPosX);

	SetDlgItemText(IDC_EDIT_POSX, Text);
	Update_OBJData();
}

void CEffectTool::OnDeltaposSpinPosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fPosY += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fPosY);

	SetDlgItemText(IDC_EDIT_POSY, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinPosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fPosZ += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fPosZ);

	SetDlgItemText(IDC_EDIT_POSZ, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinRotX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fRotX += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fRotX);

	SetDlgItemText(IDC_EDIT_ROTX, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinRotY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fRotY += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fRotY);

	SetDlgItemText(IDC_EDIT_ROTY, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinRotZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fRotZ += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fRotZ);

	SetDlgItemText(IDC_EDIT_ROTZ, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinScaleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fScaleX += Temp * m_fSCALING * 0.1f;

	CString Text;
	Text.Format(_T("%.2f"), m_fScaleX);

	SetDlgItemText(IDC_EDIT_SCALEX, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinScaleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fScaleY += Temp * m_fSCALING * 0.1f;

	CString Text;
	Text.Format(_T("%.2f"), m_fScaleY);

	SetDlgItemText(IDC_EDIT_SCALEY, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinScaleZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fScaleZ += Temp * m_fSCALING * 0.1f;

	CString Text;
	Text.Format(_T("%.2f"), m_fScaleZ);

	SetDlgItemText(IDC_EDIT_SCALEZ, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinUvposU(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fUVPosU += Temp * 0.01f;

	CString Text;
	Text.Format(_T("%.2f"), m_fUVPosU);

	SetDlgItemText(IDC_EDIT_UVPOSU, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinUvposV(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fUVPosV += Temp * 0.01f;

	CString Text;
	Text.Format(_T("%.2f"), m_fUVPosV);

	SetDlgItemText(IDC_EDIT_UVPOSV, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinAtlasU(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fUVAtlasU += Temp;

	CString Text;
	Text.Format(_T("%.2f"), m_fUVAtlasU);

	SetDlgItemText(IDC_EDIT_ATLAS_U, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinAtlasV(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;


	float Temp = (float)pNMUpDown->iDelta;
	m_fUVAtlasV += Temp;

	CString Text;
	Text.Format(_T("%.2f"), m_fUVAtlasV);

	SetDlgItemText(IDC_EDIT_ATLAS_V, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinMoveposX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fMovePosX += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fMovePosX);

	SetDlgItemText(IDC_EDIT_MOVE_POS_X, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinMoveposY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fMovePosY += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fMovePosY);

	SetDlgItemText(IDC_EDIT_MOVE_POS_Y, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinMoveposZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fMovePosZ += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fMovePosZ);

	SetDlgItemText(IDC_EDIT_MOVE_POS_Z, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinMoverotX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fMoveRotX += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fMoveRotX);

	SetDlgItemText(IDC_EDIT_MOVE_ROT_X, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinMoverotY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fMoveRotY += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fMoveRotY);

	SetDlgItemText(IDC_EDIT_MOVE_ROT_Y, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinMoverotZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fMoveRotZ += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fMoveRotZ);

	SetDlgItemText(IDC_EDIT_MOVE_ROT_Z, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinMovescaleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fMoveScaleX += Temp * m_fSCALING * 0.1f;

	CString Text;
	Text.Format(_T("%.2f"), m_fMoveScaleX);

	SetDlgItemText(IDC_EDIT_MOVE_SCALE_X, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinMovescaleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fMoveScaleY += Temp * m_fSCALING * 0.1f;

	CString Text;
	Text.Format(_T("%.2f"), m_fMoveScaleY);

	SetDlgItemText(IDC_EDIT_MOVE_SCALE_Y, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinMovescaleZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fMoveScaleZ += Temp * m_fSCALING * 0.1f;

	CString Text;
	Text.Format(_T("%.2f"), m_fMoveScaleZ);

	SetDlgItemText(IDC_EDIT_MOVE_SCALE_Z, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinMoveuvU(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fMoveUV_U += Temp * 0.01f;

	CString Text;
	Text.Format(_T("%.2f"), m_fMoveUV_U);

	SetDlgItemText(IDC_EDIT_MOVE_UV_U, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinMoveuvV(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fMoveUV_V += Temp * 0.01f;

	CString Text;
	Text.Format(_T("%.2f"), m_fMoveUV_V);

	SetDlgItemText(IDC_EDIT_MOVE_UV_V, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinTimebegin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fBeginTime_Start += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fBeginTime_Start);

	SetDlgItemText(IDC_EDIT_BEGINTIME_START, Text);
	Update_OBJData();
}


void CEffectTool::OnDeltaposSpinTimeend(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fBeginTime_End += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fBeginTime_End);

	SetDlgItemText(IDC_EDIT_BEGINTIME_END, Text);
	Update_OBJData();
}


void CEffectTool::OnTvnSelchangedTreeMeshlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM CurItem = m_TreeCtrlMesh.GetNextItem(NULL, TVGN_CARET);
	if (m_TreeCtrlMesh.ItemHasChildren(CurItem))
		return;
	HTREEITEM SelectedParent = m_TreeCtrlMesh.GetNextItem(CurItem, TVGN_PARENT);
	HTREEITEM SelectedParent2 = m_TreeCtrlMesh.GetNextItem(SelectedParent, TVGN_PARENT);//

	m_cstrMeshType = m_TreeCtrlMesh.GetItemText(SelectedParent2/*SelectedParent*/);
	if (!(m_cstrMeshType == _T("StaticMesh") || m_cstrMeshType == _T("DynamicMesh")))
	{
		HTREEITEM MeshType = m_TreeCtrlMesh.GetNextItem(SelectedParent2/*SelectedParent*/, TVGN_PARENT);
		m_cstrMeshType = m_TreeCtrlMesh.GetItemText(MeshType);
	}

	m_cstrTreeSelectedName = m_TreeCtrlMesh.GetItemText(CurItem);

	*pResult = 0;
}


void CEffectTool::OnBnClickedButtonAddMesh()
{
	UpdateData(TRUE);
	if (m_cstrTreeSelectedName == _T(""))
		return;

	if (m_cstrMeshType == _T("StaticMesh"))
	{
		TCHAR   szFileName[256];
		ZeroMemory(szFileName, 256);
		StrCpy(szFileName, m_cstrTreeSelectedName);
		PathRemoveExtension(szFileName); //뒤에 확장자만 잘라줌
		m_cstrTreeSelectedName = szFileName;
		int iNum = SearchSameName(m_cstrTreeSelectedName, m_cstrMeshType); //해당 이름으로 만들어진 개수 
		CString cstrNum = _T("");
		cstrNum.Format(_T("%d"), iNum); //List에 같은 이름있었을때의 그 개수를 세어서 Cstring형태로 전환
		m_ListBoxStaticMesh.InsertString(m_iStaticMeshNum++, m_cstrTreeSelectedName + _T("-") + cstrNum); // List에 넣어줄 이름
		CString cstrMeshName = _T(""); // 씬에서 넣어주는 Key 값
		cstrMeshName = _T("Mesh_") + m_cstrTreeSelectedName; //이 줄이 씬에서 메쉬 널어줄때의 wstringObjectKey값
															 //아래는 변환
		wstring GameObjectName = cstrMeshName.operator LPCWSTR();
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vPos = { m_fPosX,m_fPosY,m_fPosZ };
		_vec3 vRot = { m_fRotX,m_fRotY,m_fRotZ };
		_vec3 vScale = { m_fScaleX,m_fScaleY,m_fScaleZ };
		pGameObject = CToolGameEffectStaticMeshObject::Create(g_pToolView->Get_GraphicDev(), GameObjectName, &vPos, &vRot, &vScale);
		if (pGameObject == nullptr)
			return;
		g_pMapScene->Add_GameObject(Engine::STATICMESH, GameObjectName, pGameObject);
	}
	else if (m_cstrMeshType == _T("DynamicMesh"))
	{
		TCHAR   szFileName[256];
		ZeroMemory(szFileName, 256);
		StrCpy(szFileName, m_cstrTreeSelectedName);
		PathRemoveExtension(szFileName); //뒤에 확장자만 잘라줌
		m_cstrTreeSelectedName = szFileName;
		int iNum = SearchSameName(m_cstrTreeSelectedName, m_cstrMeshType); //해당 이름으로 만들어진 개수 
		CString cstrNum = _T("");
		cstrNum.Format(_T("%d"), iNum); //List에 같은 이름있었을때의 그 개수를 세어서 Cstring형태로 전환
		m_ListBoxDynamicMesh.InsertString(m_iDynamicMeshNum++, m_cstrTreeSelectedName + _T("-") + cstrNum); // List에 넣어줄 이름
		CString cstrMeshName = _T(""); // 씬에서 넣어주는 Key 값
		cstrMeshName = _T("Mesh_") + m_cstrTreeSelectedName; //이 줄이 씬에서 메쉬 널어줄때의 wstringObjectKey값
															 //아래는 변환
		wstring GameObjectName = cstrMeshName.operator LPCWSTR();
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vPos = { m_fPosX,m_fPosY,m_fPosZ };
		_vec3 vRot = { m_fRotX,m_fRotY,m_fRotZ };
		_vec3 vScale = { m_fScaleX,m_fScaleY,m_fScaleZ };
		pGameObject = CToolEffectDynamicMeshObject::Create(g_pToolView->Get_GraphicDev(), GameObjectName, &vPos, &vRot, &vScale);
		if (pGameObject == nullptr)
			return;
		g_pMapScene->Add_GameObject(Engine::DYNAMICMESH, GameObjectName, pGameObject);
	}
}


void CEffectTool::OnBnClickedRadioAlpha()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iAlpha = 0;
	Update_OBJData();
}


void CEffectTool::OnBnClickedRadioNonalpha()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iAlpha = 1;
	Update_OBJData();
}


void CEffectTool::OnTvnSelchangedTreeTextureObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM hTreeItem = m_TreeCtrlTextureOBJ.GetSelectedItem();
	m_cstrTreeSelectedName = m_TreeCtrlTextureOBJ.GetItemText(hTreeItem);

	*pResult = 0;
}


void CEffectTool::OnTvnSelchangedTreeTexture(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM hTreeItem = m_TreeCtrlTexture.GetSelectedItem();
	m_cstrTextureTreeSelectedName = m_TreeCtrlTexture.GetItemText(hTreeItem);

	*pResult = 0;
}


void CEffectTool::OnLbnSelchangeListStaticmesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	//map<wstring, list<Engine::CGameObject*>> mapStaticMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::STATICMESH)->Get_mapGameObject();
	//for (auto& ObjTag : mapStaticMesh)
	//{
	//	for (auto& rGameObject : ObjTag.second)
	//	{
	//		static_cast<CToolGameObject*>(rGameObject)->Set_WireFrame(false);
	//	}
	//}
	//map<wstring, list<Engine::CGameObject*>> mapDynamicMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::DYNAMICMESH)->Get_mapGameObject();
	//for (auto& ObjTag : mapDynamicMesh)
	//{
	//	for (auto& rGameObject : ObjTag.second)
	//	{
	//		static_cast<CToolGameObject*>(rGameObject)->Set_WireFrame(false);
	//	}
	//}
	pPickingObject = Engine::Get_GameObject(Engine::STATICMESH, L"Mesh_" + ObjectName, iNum);
	//static_cast<CToolGameObject*>(pPickingObject)->Set_WireFrame(true);
	m_pCurrentTransform = static_cast<CToolGameObject*>(pPickingObject)->Get_Transform();

	m_pCurrentMeshObj = static_cast<CToolGameObject*>(pPickingObject);
	if (m_pCurrentMeshObj == nullptr)
		return;
	if (m_pCurrentTransform == nullptr)
		return;

	Reset_TextrueList();

	Update_ToolData();

	UpdateData(FALSE);
}


void CEffectTool::OnLbnSelchangeListDynamicmesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	/*map<wstring, list<Engine::CGameObject*>> mapStaticMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::STATICMESH)->Get_mapGameObject();
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
	}*/
	pPickingObject = Engine::Get_GameObject(Engine::DYNAMICMESH, L"Mesh_" + ObjectName, iNum);
	//static_cast<CToolGameObject*>(pPickingObject)->Set_WireFrame(true);
	m_pCurrentTransform = static_cast<CToolGameObject*>(pPickingObject)->Get_Transform();

	m_pCurrentMeshObj = static_cast<CToolGameObject*>(pPickingObject);

	if (m_pCurrentTransform == nullptr)
		return;

	Reset_TextrueList();

	Update_ToolData();

	////이건 여기서할게 아니고, 텍스쳐리스트 클릭했을때 해야함
	//if (m_cstrTextureListBoxSelectedName == _T(""))
	//	return;

	//Engine::EFFECT_UV_INFO*	pEffectInfo = nullptr;
	//pEffectInfo = m_pCurrentMeshObj->Get_TextureInfo((_uint)_tstoi(m_cstrTextureListBoxSelectedName));

	//m_fUVPosU = pEffectInfo->vUVPos.x;
	//m_fUVPosV = pEffectInfo->vUVPos.y;
	//m_fUVAtlasU = pEffectInfo->vUVAtlas.x;
	//m_fUVAtlasV = pEffectInfo->vUVAtlas.y;
	//m_fMoveUV_U = pEffectInfo->vUVMovePos.x;
	//m_fMoveUV_V = pEffectInfo->vUVMovePos.y;
	//m_iPass = pEffectInfo->iPass;

	UpdateData(FALSE);
}


void CEffectTool::OnBnClickedButtonStart()
{
	g_bIsPlay = true;
}


void CEffectTool::OnBnClickedButtonPause()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bIsPlay = !m_bIsPlay;
	g_bIsPlay = m_bIsPlay;
}


void CEffectTool::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_bIsPlay = false;

	if (m_pCurrentMeshObj == nullptr)
		return;
	g_dTimeBegin = 0.f;


	Update_OBJData();
}


void CEffectTool::OnBnClickedButtonAddTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pCurrentMeshObj == nullptr)
		return;

	m_ListBoxTexture.InsertString(m_iTextureNum++, m_cstrTextureTreeSelectedName);

	Engine::EFFECT_UV_INFO	eInfo;
	eInfo.iTextureName = _tstoi(m_cstrTextureTreeSelectedName);
	eInfo.vUVPos.x = m_fUVPosU;
	eInfo.vUVPos.y = m_fUVPosV;
	eInfo.vUVAtlas.x = m_fUVAtlasU;
	eInfo.vUVAtlas.y = m_fUVAtlasV;
	eInfo.vUVMovePos.x = m_fMoveUV_U;
	eInfo.vUVMovePos.y = m_fMoveUV_V;
	eInfo.iPass = m_iPass;

	m_pCurrentMeshObj->Add_TextureInfo(eInfo);

}


void CEffectTool::OnDeltaposSpinPass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	int iNum = (float)pNMUpDown->iDelta;
	m_iPass += iNum;
	CString Text;
	Text.Format(_T("%d"), m_iPass);

	SetDlgItemText(IDC_EDIT_PASS, Text);
	Update_OBJData();
}


void CEffectTool::OnLbnSelchangeListTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//클릭된 객체의 텍스쳐정보 로드
	int nCursel = m_ListBoxTexture.GetCurSel();
	if (nCursel >= 0)
	{
		m_ListBoxTexture.GetText(nCursel, m_cstrTextureListBoxSelectedName);
	}
	if (m_cstrTextureListBoxSelectedName == _T(""))
		return;

	Engine::EFFECT_UV_INFO*	pEffectInfo = nullptr;
	pEffectInfo = m_pCurrentMeshObj->Get_TextureInfo((_uint)_tstoi(m_cstrTextureListBoxSelectedName));

	m_fUVPosU = pEffectInfo->vUVPos.x;
	m_fUVPosV = pEffectInfo->vUVPos.y;
	m_fUVAtlasU = pEffectInfo->vUVAtlas.x;
	m_fUVAtlasV = pEffectInfo->vUVAtlas.y;
	m_fMoveUV_U = pEffectInfo->vUVMovePos.x;
	m_fMoveUV_V = pEffectInfo->vUVMovePos.y;
	m_iPass = pEffectInfo->iPass;

	UpdateData(FALSE);
}


void CEffectTool::OnBnClickedButtonAddTextureObj()
{
	UpdateData(TRUE);
	if (m_cstrTreeSelectedName == _T(""))
		return;

	TCHAR   szFileName[256];
	ZeroMemory(szFileName, 256);
	StrCpy(szFileName, m_cstrTreeSelectedName);
	PathRemoveExtension(szFileName); //뒤에 확장자만 잘라줌
	m_cstrTreeSelectedName = szFileName;
	int iNum = SearchSameName(m_cstrTreeSelectedName, L"TextureOBJ"); //해당 이름으로 만들어진 개수 
	CString cstrNum = _T("");
	cstrNum.Format(_T("%d"), iNum); //List에 같은 이름있었을때의 그 개수를 세어서 Cstring형태로 전환
	m_ListBoxTextureObj.InsertString(m_iTextureObjNum++, m_cstrTreeSelectedName + _T("-") + cstrNum); // List에 넣어줄 이름
	CString cstrMeshName = _T(""); // 씬에서 넣어주는 Key 값
	cstrMeshName = m_cstrTreeSelectedName; //이 줄이 씬에서 메쉬 널어줄때의 wstringObjectKey값
														 //아래는 변환
	wstring GameObjectName = cstrMeshName.operator LPCWSTR();
	Engine::CGameObject* pGameObject = nullptr;
	_vec3 vPos = { m_fPosX,m_fPosY+ 100.f,m_fPosZ };
	_vec3 vRot = { m_fRotX,m_fRotY,m_fRotZ };
	_vec3 vScale = { m_fScaleX* 10.f,m_fScaleY* 10.f,m_fScaleZ * 10.f } ;
	pGameObject = CToolEffectTextureObject::Create(g_pToolView->Get_GraphicDev(), GameObjectName, &vPos, &vRot, &vScale, _tstoi(m_cstrTreeSelectedName));
	if (pGameObject == nullptr)
		return;
	g_pMapScene->Add_GameObject(Engine::GAMEOBJECT, GameObjectName, pGameObject);
	
}


void CEffectTool::OnLbnSelchangeListTextureObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int nCursel = m_ListBoxTextureObj.GetCurSel();
	if (nCursel >= 0)
	{
		m_ListBoxTextureObj.GetText(nCursel, m_cstrListBoxSelectedName);
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

	pPickingObject = Engine::Get_GameObject(Engine::GAMEOBJECT, ObjectName, iNum);
	m_pCurrentTransform = static_cast<CToolGameObject*>(pPickingObject)->Get_Transform();

	m_pCurrentMeshObj = static_cast<CToolGameObject*>(pPickingObject);
	if (m_pCurrentMeshObj == nullptr)
		return;
	if (m_pCurrentTransform == nullptr)
		return;

	Reset_TextrueList();

	Update_ToolData();

	UpdateData(FALSE);
}


void CEffectTool::OnBnClickedButtonDeleteDynamicmesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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


void CEffectTool::OnBnClickedButtonDeleteStaticmesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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


void CEffectTool::OnBnClickedButtonDeleteTextureObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCursel = m_ListBoxTextureObj.GetCurSel();
	int iDeleteNum = 0;
	CString cstrObjectName = _T("");
	if (nCursel >= 0)
	{
		m_ListBoxTextureObj.GetText(nCursel, m_cstrListBoxSelectedName);
		int Position = m_cstrListBoxSelectedName.Find(_T("-")) + 1;
		AfxExtractSubString(cstrObjectName, m_cstrListBoxSelectedName, 0, '-');
		CString cstrNum = m_cstrListBoxSelectedName.Mid(Position);
		int Num = _ttoi(cstrNum);
		wstring wstrSelectedGameObjectName = cstrObjectName.operator LPCWSTR();
		Engine::CGameObject*  pDeleteGameObject = g_pMapScene->Get_GameObject(Engine::GAMEOBJECT, wstrSelectedGameObjectName, Num);
		dynamic_cast<CToolGameObject*>(pDeleteGameObject)->Set_Dead();
		m_ListBoxTextureObj.DeleteString(nCursel);
		m_iStaticMeshNum--;
	}
	m_ListBoxTextureObj.ResetContent();
	m_iStaticMeshNum = 0;
	Engine::CLayer* m_Layer = g_pMapScene->Get_Layer(Engine::GAMEOBJECT);
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
			m_ListBoxTextureObj.InsertString(m_iStaticMeshNum, cstrCutObjTag + _T("-") + cstrObjNum);
			m_iStaticMeshNum++;
		}

	}
}


void CEffectTool::OnBnClickedButtonDeleteTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_pCurrentMeshObj->Delete_TextureInfoIdx((_uint)_tstoi(m_cstrTextureListBoxSelectedName));

	Reset_TextrueList();
}


void CEffectTool::OnDeltaposSpinObjtimeEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fObjTime_End += Temp * 0.1f;

	CString Text;
	Text.Format(_T("%.2f"), m_fObjTime_End);

	SetDlgItemText(IDC_EDIT_OBJTIME_END, Text);
	Update_OBJData();
}


void CEffectTool::OnCbnSelchangeCombo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eEffectType = (Engine::EFFECTTYPE)m_ComboBoxCtrl_Type.GetCurSel();
}


void CEffectTool::OnDeltaposSpinSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float Temp = (float)pNMUpDown->iDelta;
	m_fSpeed += Temp * m_fSCALING;

	CString Text;
	Text.Format(_T("%.2f"), m_fSpeed);

	SetDlgItemText(IDC_EDIT_SPEED, Text);
	Update_OBJData();
}


void CEffectTool::OnBnClickedButtonAllApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Update_OBJData();

}
