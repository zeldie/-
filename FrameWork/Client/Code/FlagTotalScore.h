#pragma once
#ifndef __FlagTotalScore_h__
#define __FlagTotalScore_h__

#include "UIObject.h"

class CFlagTotalScore : public CUIObject
{
public:
	enum FLAGTOTALSCORETYPE{BG, OUTLINE, FLAGTOTALSCORETYPE_END};
	enum TEAMCOLOR{BLUE, RED, TEAMCOLOR_END};

public:
	explicit CFlagTotalScore(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlagTotalScore();
	
public:
	HRESULT				Ready_GameObject(FLAGTOTALSCORETYPE eType, TEAMCOLOR eColor,_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	FLAGTOTALSCORETYPE	m_eFlagScoreType;
	TEAMCOLOR			m_eTeamColor;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CFlagTotalScore*		Create(LPDIRECT3DDEVICE9 pGraphicDev, FLAGTOTALSCORETYPE eType, TEAMCOLOR eColor, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif