#pragma once

#include "Scene.h"

class CBossStage final : public Engine::CScene
{
public:
	explicit  CBossStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossStage();

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
	_bool					m_bLateInit;
	_uint					m_iStainedGlassNum;

	_double					m_dCreateBossResultUITime; //»Ò¡§	

public:
	static CBossStage*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free() override;
};

