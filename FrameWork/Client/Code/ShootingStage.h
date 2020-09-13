#pragma once

#include "Scene.h"


class CShootingStage final : public Engine::CScene
{
public:
	explicit  CShootingStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShootingStage();

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
	_double					m_dCreateShootingResultUITime;
	Engine::CGameObject*	m_pPlayer;	// 변신처리용
	Engine::CGameObject*	m_pApostle;	// 변신처리용
	_double					m_dTransDelay;
	_bool					m_bIsTrans;

	_bool					m_bOneClearUI;
	_double					m_dChangeScene;

public:
	static CShootingStage*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free() override;
};

