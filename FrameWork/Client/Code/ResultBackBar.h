#pragma once
#ifndef __ResultBackBar_h__
#define __ResultBackBar_h__

#include "FlagTotalScore.h"

#include "UIObject.h"
class CResultBackBar : public CUIObject
{
public:
	enum BACKBARTYPE {NONE0, RECTTYPE, NONE1, NONE2, RESULTBGTYPE, INDIGOCOLORTYPE, REDCOLORTYPE, NONE3, WHITELINE, BACKBARTYPE_END};
	enum FLAGRESULTINFO {PLAYER, ALLIANCE, ENEMY_1, ENEMY_2, FLAGRESULTINFO_END}; //깃발전 결과창에서 사용

public:
	explicit CResultBackBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CResultBackBar();

public:
	HRESULT				Ready_GameObject(BACKBARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	void				GetTotalTime();
	void				GetTotalTimeInRunGame();
	void				GetResult();
	void				MakeComma(_uint iData, _tchar* szResult);
	void				CheckWinner(CFlagTotalScore::TEAMCOLOR eTeam);
	
	

private:
	BACKBARTYPE			m_eBackBarType;
	CUIMgr*				m_pUIMgr;
	_bool				m_bOne;
	_bool				m_bBlueWIN;

	////자리 이동 변수
	//_float fx;
	//_float fy;

	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	//보스 결과창
	_double				m_dbTotalTime;
	wstring				m_wstrTotalTime;
	_uint				m_iTotalDamage;
	//wstring				m_wstrTotalDamage;
	_uint				m_iTotalBreak;
	//wstring				m_wstrTotalBreak;
	_uint				m_iTotalDeath;
	wstring				m_wstrTotalDeath;

	_tchar m_szTotalDamage[MIN_STR];
	_tchar m_szTotalBreak[MIN_STR];
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	//깃발전 결과창
	_tchar				m_szBlueTotalScore[4];
	_tchar				m_szRedTotalScore[4];
	TCHAR_FLAGRESULT	m_szFlagResult[FLAGRESULTINFO_END];

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CResultBackBar*	Create(LPDIRECT3DDEVICE9 pGraphicDev, BACKBARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void			Free();
};

#endif