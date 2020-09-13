#pragma once
#ifndef __SuperArmor_h__
#define __SuperArmor_h__

#include "UIObject.h"

class CPlayerObserver;
class CSuperArmor : public CUIObject
{
public:
	enum SUPERARMORTYPE { FULL, EMPTY, SUPERARMORTYPE_END };

public:
	explicit CSuperArmor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSuperArmor();

public:
	HRESULT				Ready_GameObject(SUPERARMORTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	//옵저버
	CPlayerObserver*	m_pPlayerObserver;

	//
	SUPERARMORTYPE		m_eSuperArmor;
	_float				m_fResultValue; //감소된 최종 값 -> 쉐이더에 계산해서 보낼거	

	_uint				m_iMaxSuperArmor;
	_bool				m_bLateInit;

	//_float				m_fTest = 100.f;
private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CSuperArmor*	Create(LPDIRECT3DDEVICE9 pGraphicDev, SUPERARMORTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif