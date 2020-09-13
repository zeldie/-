#pragma once
#ifndef __CBASEEFFECT_H__
#define __CBASEEFFECT_H__

#include "BaseObject.h"

class CBaseEffect : public CBaseObject
{
protected:
	explicit CBaseEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBaseEffect();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

protected:
	void		Delete_Collider();
	void		Set_ColliderScaling(const _double& dTimeDelta);

protected:
	void		ResetEffectInfo();

public:
	virtual void	Free();

protected:
	Engine::CTexture*	m_pTextureCom;
	EFFECT_INFO			m_tEffectInfo;
	// 시작프레임
	_double				m_dFrame;
	// 텍스쳐 프레임 개수
	_double				m_dMaxFrame;
	// 움직일 방향
	_vec3				m_vDir;
	//아틀라스 이미지들 루프 돌건지
	_bool				m_bLoop;
	//아틀라스 얼마나 빨리 돌건지
	_float				m_fFrameSpeed;
	//죽는 시간
	_double				m_dbTime;
	//패스
	_uint				m_iPass;
};

#endif // !__CBASEEFFECT_H__
