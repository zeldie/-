#pragma once

#include "Scene.h"


class CCartel final : public Engine::CScene
{
public:
	explicit  CCartel(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCartel();

public:
	HRESULT			Load_Architecture(Engine::CLayer* pLayer, WCHAR* wstrPath);

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _double& dTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _double& dTimeDelta) override;
	virtual void	Render_Scene() override;

private:
	HRESULT	Ready_Environment_Layer();
	HRESULT	Ready_GameObject_Layer();
	HRESULT	Ready_UI_Layer();
	HRESULT	Ready_Light();

private:
	CUIMgr*					m_pUIMgr;
	CAiMgr*					m_pAiMgr;
	_bool					m_bLateInit;
	_double					m_dCreateCartelResultUITime;

	_bool					m_bCreateUI; //�ƾ������� �����ǰ� �ҷ���.
	_bool					m_bCreateResultUI; //�ι� Ÿ�°� �����ҷ���.
	//_float ftest;

public:
	static CCartel*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free() override;
};

