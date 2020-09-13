#pragma once

#include "Scene.h"
#include "Loading.h"

class CPuzzleGame : public Engine::CScene
{
public:
	explicit  CPuzzleGame(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPuzzleGame();

public:
	HRESULT			Ready_Scene();
	virtual _int	Update_Scene(const _double& dTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _double& dTimeDelta) override;
	virtual void	Render_Scene() override;
	virtual void	LateInit();

private:
	HRESULT	Ready_Environment_Layer();
	HRESULT	Ready_GameObject_Layer();
	HRESULT	Ready_UI_Layer();
	HRESULT	Ready_Light();

private:
	CUIMgr*					m_pUIMgr;
	CLoading*				m_pLoading;
	_bool					m_bInit;
public:
	static CPuzzleGame*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free() override;
};

