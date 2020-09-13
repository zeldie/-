#pragma once

#ifndef __Tired_h__
#define __Tired_h__

#include "NormalObject.h"

class CTired : public CNormalObject
{

public:
	explicit CTired(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTired();

public:
	HRESULT					Ready_GameObject(Engine::CTransform* pTargetTransform, _float fX, _float fY);
	virtual _int			Update_GameObject(const _double& dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void			Render_Geometry(const _double& dTimeDelta) override;

public:
	Engine::CTransform*		m_pPlayerTransformCom;
	_vec3					m_vScale;

	_matrix					m_matBill;
	_bool					m_bRender; //UI �������� �Ⱥ��̰� �Ҷ��

	CUIMgr*					m_pUIMgr;

	//�̴ϰ��� - ź��
	_matrix					m_matProj; //�����������

private:
	HRESULT					Clone_Component();
	HRESULT					Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CTired*			Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTargetTransform = nullptr, _float fX=0.f, _float fY=0.f);
	virtual void			Free();
};

#endif