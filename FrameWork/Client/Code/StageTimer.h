#pragma once
#ifndef __StageTimer_h__
#define __StageTimer_h__

#include "UIObject.h"
class CStageTimer : public CUIObject
{
public:
	enum STAGETIMERTYPE{BOSS, CARTEL, MATCHING, SHOOTING, STAGETIMERTYPE_END};

public:
	explicit CStageTimer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStageTimer();

public:
	HRESULT				Ready_GameObject(STAGETIMERTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	void				SetTime();

private:
	CUIMgr*				m_pUIMgr;
	STAGETIMERTYPE		m_eStageTimerType;

	//시간재는 변수가 필요할듯
	_double				m_dStageTime;	//(BOSS )UIMgr 에서 받아오는 스테이지 누적 시간   (CARTEL)  이걸 mattime에서 뺄거야
	_double				m_dMaxStageTime; //(CARTEL)
	_uint				m_iSecondUnits; //초 -1의자리
	_uint				m_iSecondTens;  //초 -10의자리
	_uint				m_iMinuteUnits;   //분 -1의자리
	_uint				m_iMinuteTens;    //분 -10의자리

	wstring				m_wstrStageTime;
	_bool				m_bLateInit;
	_bool				m_bCountDownStart; //(CARTEL)30초 카운트 다운 체크 변수

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CStageTimer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, STAGETIMERTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void			Free();
};

#endif