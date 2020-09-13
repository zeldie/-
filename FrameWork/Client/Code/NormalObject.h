#pragma once

#include "BaseObject.h"
#include "RcTex.h"
#include "Texture.h"

class CNormalObject : public CBaseObject
{
protected:
	explicit CNormalObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNormalObject();

public:
	//Setter
	//void				Set_Dead() { m_bIsDead = true; }

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

protected:
	Engine::CRcTex*		m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;

	//_bool				m_bIsDead;

public:
	virtual void	Free();
};

