#pragma once
#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"
#include "Loading.h"

class CLogo final : public Engine::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _double& dTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _double& dTimeDelta) override;
	virtual void	Render_Scene() override;

private:
	HRESULT	Ready_Environment_Layer();
	HRESULT	Ready_GameObject_Layer();
	HRESULT	Ready_UI_Layer();

private:
	CLoading*		m_pLoading;
public:
	static CLogo*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free() override;
};

#endif // Logo_h__