#pragma once
#ifndef __SelectMission_h__
#define __SelectMission_h__

#include "UIObject.h"
class CSelectMission : public CUIObject
{
public:
	enum BARTYPE {  HORIZON, VERTICAL1, VERTICAL2, BARTYPE_END };

public:
	explicit CSelectMission(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSelectMission();

public:
	HRESULT				Ready_GameObject(BARTYPE eType, Engine::CGameObject* pTarget, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	BARTYPE					m_eBarType;
	Engine::CGameObject*	m_pTarget;
	_float					m_fLengthSizeX;
	_float					m_fLengthSizeY;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CSelectMission*		Create(LPDIRECT3DDEVICE9 pGraphicDev, BARTYPE eType, Engine::CGameObject* pTarget, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif