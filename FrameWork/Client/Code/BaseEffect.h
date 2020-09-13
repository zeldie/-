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
	// ����������
	_double				m_dFrame;
	// �ؽ��� ������ ����
	_double				m_dMaxFrame;
	// ������ ����
	_vec3				m_vDir;
	//��Ʋ�� �̹����� ���� ������
	_bool				m_bLoop;
	//��Ʋ�� �󸶳� ���� ������
	_float				m_fFrameSpeed;
	//�״� �ð�
	_double				m_dbTime;
	//�н�
	_uint				m_iPass;
};

#endif // !__CBASEEFFECT_H__
