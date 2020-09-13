#pragma once
#ifndef __MonsterHpMgr_h__
#define __MonsterHpMgr_h__

#include "UIObject.h"

class CBelatos;
class CMonsterHp;
class CHpNumber;
class CApostleObserver;

class CMonsterHpMgr : public CUIObject
{	
public:
	explicit CMonsterHpMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterHpMgr();

public:
	//Setter
	void				Set_GiveNextHp(_float fNextHp) { m_fGiveNextHp = fNextHp; }
	void				Set_Swap(_bool bBool) { m_bSwap = bBool; }

	//Getter


public:
	HRESULT				Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	void				CreateMonsterHp();

private:
	void				CreateHpNumber(_uint iLineCnt);
	void				CalculateDigitCnt(_uint iLineCnt);

private:
	CBelatos*				m_pMonster;

	_bool					m_bLateInit;
	vector<CMonsterHp*>		m_vecMonsterHp;

	_float					m_fMaxMonsterHp; 
	_float					m_fMonsterHp;
	_float					m_fGiveNextHp;
	_uint					m_iLineCnt;

	_bool					m_bSwap;
	_uint					m_iSwapCnt;

	_uint					m_iAccumulatedDamage; //몬스터의 누적데미지 여기서할까 uimagr에서할까

	//숫자 표시 변수
	vector<CHpNumber*>		m_vecHpNumber;
	_uint					m_iDigitCnt;

	//미니게임 - 탄막
	CApostleObserver*		m_pApostleObserver;
	
	CUIMgr::UITYPE			m_eUIType;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CMonsterHpMgr*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free();
};

#endif