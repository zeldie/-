#pragma once

#include "BaseObject.h"
#include "ScreenTex.h"
#include "Texture.h"

class CUIObject : public CBaseObject
{
protected:
	explicit CUIObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIObject();

public:
	//Setter
	//void				Set_Dead() { m_bIsDead = true; }
	void				Set_RenderUI(_bool _Bbool=true) { m_bRenderUI = _Bbool; }

	//Getter
	_bool				Get_RenderUI() { return m_bRenderUI; }

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

protected:
	Engine::CScreenTex*		m_pScreenTexBufferCom;
	Engine::CTexture*		m_pTextureCom;

	//_bool					m_bIsDead;
	_bool					m_bRenderUI;

public:
	virtual void	Free();
};

