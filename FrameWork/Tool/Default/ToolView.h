
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

class CToolDoc;
class CToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	//MainApp���� ����
	_int Update(const _double& dTimeDelta);
	_int LateUpdate(const _double& dTimeDelta);
	void Render(const _double& dTimeDelta);
	void Free();

	void		SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	void		Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, Engine::CManagement** ppManagementClass);

public:
	Engine::CManagement*	Get_Management() { return m_pManagementClass; }
	LPDIRECT3DDEVICE9		Get_GraphicDev() { return m_pGraphicDev; }
	HRESULT					Ready_BasicComponent();
public:
	Engine::CGraphicDev*	m_pDeviceClass;
	Engine::CManagement*	m_pManagementClass;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

