#pragma once
#ifndef __CustomizingSelectBox_h__
#define __CustomizingSelectBox_h__

#include "UIObject.h"
class CCustomizingSelectBox : public CUIObject
{
public:
	enum SELECTTYPE{BASIC, BEFORESELECT, AFTERSELECT, SELECTTYPE_END};

public:
	explicit CCustomizingSelectBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizingSelectBox();

public:
	//Setter
	void Set_IsWearing(_bool bBool) { m_bIsWearing = bBool; }

public:
	HRESULT				Ready_GameObject(SELECTTYPE eType, _uint iIndex, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	SELECTTYPE			m_eSelectBoxType;
	_bool				m_bChoose;
	_uint				m_iIndex;

	_bool				m_bIsWearing; //내가 누른걸 착용 하고있는지 판별하는 변수

	

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CCustomizingSelectBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev, SELECTTYPE eType, _uint iIndex, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void					Free();
};

#endif