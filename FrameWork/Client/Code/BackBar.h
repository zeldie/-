#pragma once
#ifndef __BackBar_h__
#define __BackBar_h__

#include "UIObject.h"
class CBackBar : public CUIObject
{
public:
	enum BACKBARTYPE {ROUNDTYPE, RECTTYPE, MISSIONBGTYPE, READYBGTYPE, NONE0, NONE1, NONE2, MATCHINGBGTYPE,BACKBARTYPE_END};

public:
	explicit CBackBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackBar();

public:
	HRESULT				Ready_GameObject(BACKBARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;


private:
	BACKBARTYPE			m_eBackBarType;
	CUIMgr*				m_pUIMgr;

	////자리 이동 변수
	//_float fx;
	//_float fy;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CBackBar*	Create(LPDIRECT3DDEVICE9 pGraphicDev, BACKBARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif