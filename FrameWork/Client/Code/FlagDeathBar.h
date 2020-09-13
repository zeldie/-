#pragma once
#ifndef __FlagDeathBar_h__
#define __FlagDeathBar_h__

#include "UIObject.h"
class CFlagDeathBar : public CUIObject
{	
public:
	enum FLAGDEATHBARTYPE{BG, ARROW , EMOTICON, FLAGDEATHBARTYPE_END};
public:
	explicit CFlagDeathBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlagDeathBar();

public:
	//Setter
	void Set_RealPos(_vec2 vPos) { m_vRealPos = vPos; }
	void Set_ListIndex(_uint iIndex) { m_iListIndex = iIndex; }

public:
	HRESULT				Ready_GameObject(TEAM eType, FLAGDEATHBARTYPE eFlagType, _tchar* tKiller, _tchar* tDead, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	TEAM				m_eTeam;
	FLAGDEATHBARTYPE	m_eFlagDeathBarType;
	_tchar				m_szKillerName[MIN_STR]; 
	_tchar				m_szDeadName[MIN_STR];
	_uint				m_iListIndex; //리스트 인덱스

	_float				m_fAlpha;
	_double				m_dTime;

	_vec2				m_vRealPos;
	_float				m_fSpeed;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CFlagDeathBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TEAM eType, FLAGDEATHBARTYPE eFlagType, _tchar* tKiller, _tchar* tDead, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif