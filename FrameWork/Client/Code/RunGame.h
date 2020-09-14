#pragma once

#include "Scene.h"


class CRunGame final : public Engine::CScene
{
public:
	explicit  CRunGame(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRunGame();

public:
	HRESULT			Load_Architecture(Engine::CLayer* pLayer, WCHAR* wstrPath);
public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _double& dTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _double& dTimeDelta) override;
	virtual void	Render_Scene() override;

private:
	void			RenderTime();	//»Ò¡§√ﬂ∞°
	void			RenderPoints();	//»Ò¡§√ﬂ∞°
	void			StageTimeCheck(const _double& dTimeDelta); //»Ò¡§√ﬂ∞°
	void			MakeComma(_uint iData, _tchar * szResult); //»Ò¡§√ﬂ∞°

private:
	HRESULT	Ready_Environment_Layer();
	HRESULT	Ready_GameObject_Layer();
	HRESULT	Ready_UI_Layer();
	HRESULT	Ready_Light();

	void	LateInit();

private:
	CUIMgr*			m_pUIMgr;
	_bool			m_bInit;

	_tchar			m_szStageSecond[MIN_STR];	//»Ò¡§√ﬂ∞°
	_double			m_dTotalTime;				//»Ò¡§√ﬂ∞°
	_bool			m_bOneCreateUI;				//»Ò¡§√ﬂ∞°
	_bool			m_bRenderStart;				//»Ò¡§√ﬂ∞°
	_bool			m_bOne;						//»Ò¡§√ﬂ∞°

	

public:
	static CRunGame*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free() override;
};

