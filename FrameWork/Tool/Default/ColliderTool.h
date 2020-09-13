#pragma once
#include "afxwin.h"


// CColliderTool ��ȭ �����Դϴ�.
class CToolDynamicMeshObject;
class CColliderTool : public CDialog
{
	DECLARE_DYNAMIC(CColliderTool)
public:
	enum COLLIDERTYPE {SPHERE, CUBE, COLLIDERTYPE_END};
	enum FOCUSTYPE {POSX, POSY, POSZ, RADIUS, FOCUS_END};
public:
	CColliderTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CColliderTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDERTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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

	CString					m_cstrSelectedMeshName;// �Ž�����Ʈ���� ���� ���õ� �Ž� �̸�
	CString					m_cstrSelectedBoneName;// ������Ʈ���� ���� ���õ� �Ž� �̸�
	CString					m_cstrSelectedAnimationName;// �ִϸ��̼Ǹ���Ʈ���� ���� ���õ� �Ž� �̸�
	CString					m_cstrSelectedColliderName;// �ݶ��̴�����Ʈ���� ���� ���õ� �Ž� �̸�
	
	CToolDynamicMeshObject*	m_pSelectedMesh;
	
	_uint					m_iColliderNum;
};
