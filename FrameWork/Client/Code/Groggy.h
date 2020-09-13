#pragma once
#ifndef __Groggy_h__
#define __Groggy_h__

#include "UIObject.h"

class CBelatos;
class CApostleObserver;
class CGroggy : public CUIObject
{
public:
	enum GROGGYTYPE{FULL, EMPTY, GROGGYTYPE_END};

public:
	explicit CGroggy(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGroggy();

public:
	HRESULT				Ready_GameObject(GROGGYTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	GROGGYTYPE			m_eGroggyType;
	CBelatos*			m_pMonster;
	_float				m_fResultValue;
	int					m_iMaxGroggy;
	_bool				m_bLateInit;

	//_float m_fTest = 100.f;

	//미니게임 - 탄막
	CApostleObserver*		m_pApostleObserver;

	CUIMgr::UITYPE			m_eUIType;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CGroggy*	Create(LPDIRECT3DDEVICE9 pGraphicDev, GROGGYTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif