#pragma once
#ifndef __CustomizingMenu_h__
#define __CustomizingMenu_h__

#include "UIObject.h"
class CCustomizingMenu : public CUIObject
{
public:
	enum MENUTYPE{FACE1, FACE2, HAIR1, HAIR2, CLOTH1, CLOTH2, BG, LINE, MENUTYPE_END}; //FACE2, HAIR2, CLOTH2 -> ´­·¶À»¶§

public:
	explicit CCustomizingMenu(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingMenu();

public:
	HRESULT				Ready_GameObject(MENUTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	void				RenderCustomizeList(); //Ä¿½ºÅÍ¸¶ÀÌÂ¡¸ñÂ÷ ±Û¾¾ ·»´õ¸µÇÔ¼ö

private:
	MENUTYPE			m_eMenuType;
	_bool				m_bChoose;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingMenu*		Create(LPDIRECT3DDEVICE9 pGraphicDev, MENUTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void					Free();
};

#endif