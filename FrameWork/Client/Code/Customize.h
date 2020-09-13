#pragma once

#include "Scene.h"
class CCustomize final : public Engine::CScene
{
private:
	explicit CCustomize(LPDIRECT3DDEVICE9 pGraphivDev);
	virtual ~CCustomize();
public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _double& dTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _double& dTimeDelta) override;
	virtual void	Render_Scene() override;

private:
	HRESULT			Ready_Environment_Layer();
	HRESULT			Ready_GameObject_Layer();
	HRESULT			Ready_UI_Layer();
	HRESULT			Ready_Light();

private:
	CUIMgr*				m_pUIMgr;
	_bool				m_bLateInit;
	_double				m_dSceneChange;

public:
	static CCustomize*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free() override;
};

