#pragma once
#ifndef __FlagScore_h__
#define __FlagScore_h__

#include "UIObject.h"

class CFlagScore : public CUIObject
{
public:
	enum FLAGTYPE{BACKGROUND, OUTLINE, RECHARGE, SCOREBACK, FLASH, FLAGTYPE_END};  
	enum FLAGCOLOR{BLUE, RED, FLAGCOLOR_END};

public:
	explicit CFlagScore(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlagScore();
	
public:
	//Setter
	void				SetScore(FLAGCOLOR eColorType, _uint iScore);

public:
	HRESULT				Ready_GameObject(FLAGTYPE eType, FLAGCOLOR eColorType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	void				CalculateDigitCnt(_uint iScore);

private:
	CUIMgr*				m_pUIMgr;
	FLAGTYPE			m_eFlagType;
	FLAGCOLOR			m_eFlagColor;

	_float				m_fResultValue;
	_uint				m_iGoal;
	wstring				m_wstrGoal;
	_uint				m_iBlueScore;
	wstring				m_wstrBlueScore;
	_uint				m_iRedScore;
	wstring				m_wstrRedScore;
	_uint				m_iDigitCnt; //자릿수에 따라서 점수숫자 자리를 바꿀거야 그래야이쁨
	_bool				m_bOne;


								   ////자리 이동 변수
								   //_float fx;
								   //_float fy;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CFlagScore*		Create(LPDIRECT3DDEVICE9 pGraphicDev, FLAGTYPE eType, FLAGCOLOR eColorType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void			Free();
};

#endif