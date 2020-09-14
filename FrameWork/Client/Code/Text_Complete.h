#pragma once

#ifndef __CText_Complete_h__
#define __CText_Complete_h__

#include "NormalObject.h"

class CText_Complete : public CNormalObject
{
public:
	explicit CText_Complete(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CText_Complete();

public:
	//Setter
	void				Set_RenderWaiting(_bool _bBool=true) { m_bRenderWaiting = _bBool; }

	//Getter
	_bool				Get_RenderWaiting() { return m_bRenderWaiting; }
	_bool				Get_SceneChange() { return m_bSceneChange; }

public:
	HRESULT				Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	_matrix				m_matProj; //�����������
	_float				m_fFrame;
	_float				m_fRotAngle;

	_bool				m_bRenderWaiting;

	//������������ �Ѿ�� �ϱ����� �ð� ���� & ��ü���� ����
	_double				m_dTime;
	_bool				m_bSceneChange;

public:
	static CText_Complete*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free();
};

#endif