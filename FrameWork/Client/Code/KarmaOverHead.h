#pragma once
#ifndef __KarmaOverHead_h__
#define __KarmaOverHead_h__

#include "NormalObject.h"

class CKarmaOverHead : public CNormalObject
{
public:
	enum KARMATYPE{KARMA_DS, KARMA_TS, KARMA_ORB, KARMA_LB, KARMA_END};

public:
	explicit CKarmaOverHead(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKarmaOverHead();

public:
	HRESULT				Ready_GameObject(CBasePlayer::PLAYER_WEAPONTYPE eTYpe, Engine::CTransform* pTargetTransform);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	KARMATYPE			m_eKarmaType;
	Engine::CTransform* m_pTargetTransformCom;
	_vec3				m_vScale;
	_double				m_dTime;
	_float				m_fAlpha;
	_bool				m_bTimeStart;
	_bool				m_bAlphaMinus;
	_bool				m_bCreateEffect;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CKarmaOverHead*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CBasePlayer::PLAYER_WEAPONTYPE eTYpe, Engine::CTransform* pTargetTransform);
	virtual void		Free();
};

#endif