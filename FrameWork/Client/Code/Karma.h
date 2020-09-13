#pragma once
#ifndef __Karma_h__
#define __Karma_h__

#include "UIObject.h"

class CPlayerObserver;
class CKarma : public CUIObject
{
public:
	enum KARMATYPE{COMMON, KARMA_DS, KARMA_TS, KARMA_ORB, KARMA_LB, KARMA_END};
	enum KARMAPOSITION{MAIN, SUB, KARMAPOSITION_END};

public:
	explicit CKarma(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKarma();

public:
	//Getter
	KARMAPOSITION&		Get_KarmaPositionType() { return m_eKarmaPositionType; }
	KARMATYPE&			Get_KarmaType() { return m_eKarmaType; }
	SELECTKARMATYPE&	Get_SelectKarmaType() { return m_eSelectKarmaType; }
	_bool				Get_EraseSwapUI() { return m_bEraseSwapUI; }
	
	
	//Setter
	void				Set_KarmaType(_uint iWeaponType) { m_eKarmaType = (KARMATYPE)iWeaponType; }

public:
	HRESULT				Ready_GameObject(SELECTKARMATYPE eSelectType, KARMAPOSITION eType, KARMATYPE eKarmaType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	void				PickingKarma();
	void				CheckSwapKarma();

private:
	//옵저버
	CPlayerObserver*	m_pPlayerObserver;

	KARMATYPE			m_eKarmaType;
	KARMAPOSITION		m_eKarmaPositionType;
	SELECTKARMATYPE		m_eSelectKarmaType; //기본ReadyUI용, 무기스왑용, 결과창UI용으로 나눔

	_bool				m_bChoose;
	_bool				m_bEraseSwapUI;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CKarma*		Create(LPDIRECT3DDEVICE9 pGraphicDev, SELECTKARMATYPE eSelectType, KARMAPOSITION eType, KARMATYPE eKarmaType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif