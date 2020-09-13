#pragma once
#ifndef __AI_Karma_h__
#define __AI_Karma_h__

#include "UIObject.h"

class CAI_Karma : public CUIObject
{
public:
	enum KARMATYPE{COMMON, KARMA_DS, KARMA_TS, KARMA_ORB, KARMA_LB, KARMA_END};
	enum KARMAPOSITION{MAIN, SUB, KARMAPOSITION_END};

public:
	explicit CAI_Karma(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAI_Karma();

public:
	HRESULT				Ready_GameObject(OBJID eAIType, KARMAPOSITION eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	CUIMgr*				m_pUIMgr;
	OBJID				m_eAIType;
	KARMATYPE			m_eKarmaType;
	KARMAPOSITION		m_eKarmaPositionType;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CAI_Karma*	Create(LPDIRECT3DDEVICE9 pGraphicDev, OBJID eAIType, KARMAPOSITION eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif