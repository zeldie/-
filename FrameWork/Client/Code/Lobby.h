#pragma once

#include "Scene.h"

class CLobby final : public Engine::CScene
{
public:
	enum STAGE_ORDER
	{
		BELATOSPUZZLE,
		MATCHINGGAME,
		AIGAME,
		SHOOTINGGAME,
		STAGE_ORDER_END
	};
private:
	explicit  CLobby(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLobby();

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
public:
	static CLobby*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free() override;
};

