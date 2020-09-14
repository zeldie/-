#pragma once
#ifndef Ending_h__
#define Ending_h__

#include "Scene.h"
#include "Ending.h"

class CEnding final : public Engine::CScene
{
private:
	explicit CEnding(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEnding();

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
	_bool m_bLateInit;

public:
	static CEnding*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free() override;
};

#endif // Logo_h__