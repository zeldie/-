#pragma once
#ifndef __FDBCreator_h__
#define __FDBCreator_h__

#include "UIObject.h"

class CFlagDeathBar;
class CFDBCreator : public CUIObject
{	
public:
	explicit CFDBCreator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFDBCreator();

public:
	//Setter
	void Set_ListIndex(_uint iIndex) { m_iListIndex = iIndex; }

public:
	HRESULT				Ready_GameObject(TEAM eType, _tchar* tKiller, _tchar* tDead );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	TEAM				m_eTeam;
	_tchar				m_szKillerName[MIN_STR]; 
	_tchar				m_szDeadName[MIN_STR];

	_bool				m_bLateInit;

	vector<CFlagDeathBar*> m_vecFlagDeathBar;
	_uint				   m_iListIndex;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CFDBCreator*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TEAM eType, _tchar* tKiller, _tchar* tDead);
	virtual void				Free();
};

#endif