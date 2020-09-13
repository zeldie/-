#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CEffectTool 대화 상자입니다.

class CToolGameObject;
class CEffectTool : public CDialog
{
	DECLARE_DYNAMIC(CEffectTool)

public:
	CEffectTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void InitTreeCtrl();
	void InitTreeCtrlTextureOBJ();
	void InitTreeCtrlTexture();
	void SearchRecursive(HTREEITEM hParent, CString Child, CString pFilePath);
	void SearchRecursiveCnt(HTREEITEM hParent, CString Child, CString pFilePath);
	_int SearchSameName(CString cstrname, CString cstrMesyType);

	void Update_OBJData();
	void Update_ToolData();
	void Reset_TextrueList();
	virtual BOOL OnInitDialog();

public:
	// 스핀 컨트롤 증가값 배율
	float m_fSCALING = 1.0f;

	_int								m_iStaticMeshNum; // Listbox에 insert할때 숫자
	_int								m_iDynamicMeshNum; // Listbox에 insert할때 숫자
	_int								m_iTextureObjNum; // ListTextureOBJ에 insert할때 숫자
	_int								m_iTextureNum; // ListTexture에 insert할때 숫자

	CString								m_cstrTreeSelectedName; // Tree Control에서 현재 선택한 이름
	CString								m_cstrListBoxSelectedName; // List Box에서 현재 선택한 이름
	CString								m_cstrMeshType; // 내가  누른 것이 StaticMesh인지 DynamicMesh인지 확인

	CString								m_cstrTextureTreeSelectedName; // 텍스쳐 Tree Control에서 현재 선택한 이름
	CString								m_cstrTextureListBoxSelectedName; // 텍스쳐 List Box에서 현재 선택한 이름

	Engine::CTransform*					m_pCurrentTransform;
	CToolGameObject*					m_pCurrentMeshObj;
	
	bool	m_bIsPlay;
	bool	m_bIsTexture_Alpha;	//어캐쓸지 아직미정

	Engine::EFFECTTYPE					m_eEffectType;

	_int	m_iComboBoxCount;

	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	float m_fRotX;
	float m_fRotY;
	float m_fRotZ;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	float m_fUVPosU;
	float m_fUVPosV;
	float m_fUVAtlasU;
	float m_fUVAtlasV;
	CSpinButtonCtrl m_SpinCtrl_PosX;
	CSpinButtonCtrl m_SpinCtrl_PosY;
	CSpinButtonCtrl m_SpinCtrl_PosZ;
	CSpinButtonCtrl m_SpinCtrl_RotX;
	CSpinButtonCtrl m_SpinCtrl_RotY;
	CSpinButtonCtrl m_SpinCtrl_RotZ;
	CSpinButtonCtrl m_SpinCtrl_ScaleX;
	CSpinButtonCtrl m_SpinCtrl_ScaleY;
	CSpinButtonCtrl m_SpinCtrl_ScaleZ;
	CSpinButtonCtrl m_SpinCtrl_UVPosU;
	CSpinButtonCtrl m_SpinCtrl_UVPosV;
	CSpinButtonCtrl m_SpinCtrl_AtlasU;
	CSpinButtonCtrl m_SpinCtrl_AtlasV;
	float m_fMovePosX;
	float m_fMovePosY;
	float m_fMovePosZ;
	float m_fMoveRotX;
	float m_fMoveRotY;
	float m_fMoveRotZ;
	float m_fMoveScaleX;
	float m_fMoveScaleY;
	float m_fMoveScaleZ;
	float m_fMoveUV_U;
	float m_fMoveUV_V;
	float m_fBeginTime_Start;
	float m_fBeginTime_End;
	CSpinButtonCtrl m_SpinCtrl_MOVE_Pos_X;
	CSpinButtonCtrl m_SpinCtrl_MOVE_Pos_Y;
	CSpinButtonCtrl m_SpinCtrl_MOVE_Pos_Z;
	CSpinButtonCtrl m_SpinCtrl_MOVE_Rot_X;
	CSpinButtonCtrl m_SpinCtrl_MOVE_Rot_Y;
	CSpinButtonCtrl m_SpinCtrl_MOVE_Rot_Z;
	CSpinButtonCtrl m_SpinCtrl_MOVE_Scale_X;
	CSpinButtonCtrl m_SpinCtrl_MOVE_Scale_Y;
	CSpinButtonCtrl m_SpinCtrl_MOVE_Scale_Z;
	CSpinButtonCtrl m_SpinCtrl_MOVE_UV_U;
	CSpinButtonCtrl m_SpinCtrl_MOVE_UV_V;
	CSpinButtonCtrl m_SpinCtrl_BeginTime_Start;
	CSpinButtonCtrl m_SpinCtrl_BeginTime_End;

	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnDeltaposSpinPosX(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnDeltaposSpinPosY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPosZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRotX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRotY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRotZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinScaleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinScaleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinScaleZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinUvposU(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinUvposV(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinAtlasU(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinAtlasV(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMoveposX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMoveposY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMoveposZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMoverotX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMoverotY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMoverotZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMovescaleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMovescaleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMovescaleZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMoveuvU(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMoveuvV(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTimebegin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTimeend(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeMeshlist(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_TreeCtrlMesh;
	CListBox m_ListBoxStaticMesh;
	CListBox m_ListBoxDynamicMesh;
	afx_msg void OnBnClickedButtonAddMesh();
	afx_msg void OnBnClickedRadioAlpha();
	afx_msg void OnBnClickedRadioNonalpha();
	afx_msg void OnTvnSelchangedTreeTextureObj(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_TreeCtrlTextureOBJ;
	afx_msg void OnTvnSelchangedTreeTexture(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_TreeCtrlTexture;
	CListBox m_ListBoxTextureObj;
	CListBox m_ListBoxTexture;
	afx_msg void OnLbnSelchangeListStaticmesh();
	afx_msg void OnLbnSelchangeListDynamicmesh();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonAddTexture();
	afx_msg void OnDeltaposSpinPass(NMHDR *pNMHDR, LRESULT *pResult);
	int m_iPass;
	CSpinButtonCtrl m_SpinCtrl_Pass;
	afx_msg void OnLbnSelchangeListTexture();
	afx_msg void OnBnClickedButtonAddTextureObj();
	afx_msg void OnLbnSelchangeListTextureObj();
	afx_msg void OnBnClickedButtonDeleteDynamicmesh();
	afx_msg void OnBnClickedButtonDeleteStaticmesh();
	afx_msg void OnBnClickedButtonDeleteTextureObj();
	afx_msg void OnBnClickedButtonDeleteTexture();
	int m_iAlpha;
	CSpinButtonCtrl m_SpinCtrl_ObjTime_End;
	float m_fObjTime_End;
	afx_msg void OnDeltaposSpinObjtimeEnd(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_ComboBoxCtrl_Type;
	afx_msg void OnCbnSelchangeCombo2();
	float m_fSpeed;
	CSpinButtonCtrl m_SpinCtrl_Speed;
	afx_msg void OnDeltaposSpinSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAllApply();
};
