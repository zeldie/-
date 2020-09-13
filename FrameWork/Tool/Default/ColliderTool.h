#pragma once
#include "afxwin.h"


// CColliderTool 대화 상자입니다.
class CToolDynamicMeshObject;
class CColliderTool : public CDialog
{
	DECLARE_DYNAMIC(CColliderTool)
public:
	enum COLLIDERTYPE {SPHERE, CUBE, COLLIDERTYPE_END};
	enum FOCUSTYPE {POSX, POSY, POSZ, RADIUS, FOCUS_END};
public:
	CColliderTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CColliderTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDERTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedColliderCube();
	afx_msg void OnBnClickedColliderSphere();
	afx_msg void OnBnClickedAnimationPlay();
	afx_msg void OnBnClickedAnimationPause();
	afx_msg void OnBnClickedPosResetButton();
	afx_msg void OnBnClickedResetAllButton();
	afx_msg void OnEnSetfocusPosX();
	afx_msg void OnEnUpdatePosX();
	afx_msg void OnEnSetfocusPosY();
	afx_msg void OnEnUpdatePosY();
	afx_msg void OnEnSetfocusPosZ();
	afx_msg void OnEnUpdatePosZ();
	afx_msg void OnEnSetfocusRadius();
	afx_msg void OnEnUpdateRadius();
	afx_msg void OnBnClickedColliderCreateButton();
	afx_msg void OnBnClickedColliderDeleteButton();
	afx_msg void OnBnClickedColliderSaveButton();
	afx_msg void OnBnClickedColliderLoadButton();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeMeshList();
	afx_msg void OnLbnSelchangeBoneList();
	afx_msg void OnLbnSelchangeAnimationList();
	afx_msg void OnLbnSelchangeColliderList();
public:
	void InitMeshList();
	void SearchRecursive(CString pFilePath);
public:
	CListBox				m_ListBoxMesh;
	CListBox				m_ListBoxAnimation;
	CListBox				m_ListBoxBone;
	CListBox				m_ListBoxCollider;

	_vec3					m_vecPos;
	_float					m_fRadius;
	COLLIDERTYPE			m_eColliderType;
	FOCUSTYPE				m_eFocusType;

	_uint					m_iMeshNum;

	CString					m_cstrSelectedMeshName;// 매쉬리스트에서 현재 선택된 매쉬 이름
	CString					m_cstrSelectedBoneName;// 본리스트에서 현재 선택된 매쉬 이름
	CString					m_cstrSelectedAnimationName;// 애니매이션리스트에서 현재 선택된 매쉬 이름
	CString					m_cstrSelectedColliderName;// 콜라이더리스트에서 현재 선택된 매쉬 이름
	
	CToolDynamicMeshObject*	m_pSelectedMesh;
	
	_uint					m_iColliderNum;
};
