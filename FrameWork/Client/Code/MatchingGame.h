#pragma once

#include "Scene.h"


class CMatchingGame : public Engine::CScene
{
public:
	explicit  CMatchingGame(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMatchingGame();

public:
	HRESULT			Load_Architecture(Engine::CLayer* pLayer, WCHAR* wstrPath);
public:
	virtual HRESULT Ready_Scene() override;
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
	_bool					m_bInit;
public:
	static CMatchingGame*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free() override;
};

