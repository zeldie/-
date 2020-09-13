#pragma once

#include "NormalObject.h"

class CBackGround : public CNormalObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackGround();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	CSoundMgr*				m_pSoundMgr;
public:
	static CBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;
};

