#pragma once

#ifndef __CLoadingIcon_h__
#define __CLoadingIcon_h__

#include "NormalObject.h"

class CLoadingIcon : public CNormalObject
{
public:
	explicit CLoadingIcon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoadingIcon();

public:
	//Setter
	void				Set_RenderWaiting(_bool _bBool=true) { m_bRenderWaiting = _bBool; }

	//Getter
	_bool				Get_RenderWaiting() { return m_bRenderWaiting; }
	_bool				Get_SceneChange() { return m_bSceneChange; }

public:
	HRESULT				Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	_matrix				m_matProj; //직교투영행렬
	_float				m_fFrame;
	_float				m_fRotAngle;

	_bool				m_bRenderWaiting;

	//다음스테이지 넘어가게 하기위한 시간 변수 & 씬체인지 변수
	_double				m_dTime;
	_bool				m_bSceneChange;

public:
	static CLoadingIcon*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free();
};

#endif