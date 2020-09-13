#pragma once
#ifndef __KarmaSlot_h__
#define __KarmaSlot_h__

#include "UIObject.h"
class CKarmaSlot : public CUIObject
{
public:
	enum KARMASLOTTYPE{INREADYUI, INRESULTUI, INREADYBGUI, KARMASLOTTYPE_END};

public:
	explicit CKarmaSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKarmaSlot();

public:
	//Getter
	SELECTKARMATYPE&	Get_SelectKarmaSlotType() { return m_eSelectKarmaSlotType; }

public:
	HRESULT				Ready_GameObject(SELECTKARMATYPE eSelectSlotType, KARMASLOTTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	KARMASLOTTYPE		m_eKarmaSlotType;
	SELECTKARMATYPE		m_eSelectKarmaSlotType;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ=0.f);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CKarmaSlot*	Create(LPDIRECT3DDEVICE9 pGraphicDev, SELECTKARMATYPE eSelectSlotType, KARMASLOTTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif