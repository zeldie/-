#pragma once

#include "NormalObject.h"

class CLogoIcon : public CNormalObject
{
private:
	explicit CLogoIcon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogoIcon();

public:
	virtual HRESULT		Ready_GameObject(_vec3* pPos);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	CSoundMgr*				m_pSoundMgr;
public:
	static CLogoIcon* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos);
	virtual void Free() override;
};

