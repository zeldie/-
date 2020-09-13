#pragma once
#ifndef __Button_h__
#define __Button_h__

#include "UIObject.h"
class CButton : public CUIObject
{
public:
	explicit CButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CButton();

public:
	//Setter
	void Set_Choose(_bool _bBool) { m_bChoose = _bBool; }

public:
	HRESULT				Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

//private:
//	_bool				CheckAroundMouse(HWND hWnd);

public:
	_bool				m_bChoose;
	_bool				m_bClick;
	_bool				m_bReady; //true가 되면 레디를 누른상태-> 시간초 안셈. 로딩 돌아가는 글씨뜸.
	_bool				m_bSound;
private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CButton*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif