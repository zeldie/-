#pragma once
#ifndef __MissionLock_h__
#define __MissionLock_h__

#include "UIObject.h"
class CMissionLock : public CUIObject
{
public:
	enum MISSIONLOCKTYPE{LOCK, LOCKWITHTXT, MISSIONLOCKTYPE_END};

public:
	explicit CMissionLock(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMissionLock();

public:
	HRESULT				Ready_GameObject(MISSIONLOCKTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	MISSIONLOCKTYPE		m_eMissionLockType;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CMissionLock*		Create(LPDIRECT3DDEVICE9 pGraphicDev, MISSIONLOCKTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif