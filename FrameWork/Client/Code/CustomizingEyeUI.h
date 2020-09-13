#pragma once
#ifndef __CustomizingEyeUI_h__
#define __CustomizingEyeUI_h__

#include "UIObject.h"
class CCustomizingEyeUI : public CUIObject
{
public:
	enum EYETYPE{EYE0, EYE1, EYE2, EYE3, EYETYPE_END};

public:
	explicit CCustomizingEyeUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingEyeUI();

public:
	HRESULT				Ready_GameObject(EYETYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	EYETYPE				m_eEyeType; //CustomizingSelectBox의 인덱스번호와 같을예정
	_bool				m_bChoose;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingEyeUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, EYETYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void					Free();
};

#endif