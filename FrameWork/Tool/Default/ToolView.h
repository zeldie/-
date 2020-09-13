
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

class CToolDoc;
class CToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	//MainApp같은 역할
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

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

