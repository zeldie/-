#pragma once

#include "Camera.h"
#include "Transform.h"
class CLightCamera : public Engine::CCamera
{
private:
	explicit CLightCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightCamera();

public:
	_matrix				Get_LightView() { return m_matView; }
	_matrix				Get_LightProj() { return m_matProj; }
public:
	virtual HRESULT		Ready_GameObject(const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										const _float& fFovY,
										const _float& fAspect,
										const _float& fNear,
										const _float& fFar);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	void	Set_PlayerTransformCom(Engine::CTransform* pTransformCom);
	void	Release_TransformCom() { m_pPlayerTransformCom = nullptr; }
private:
	Engine::CTransform*			m_pPlayerTransformCom;
public:
	static CLightCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
								const _vec3* pEye,
								const _vec3* pAt,
								const _vec3* pUp,
								const _float& fFovY,
								const _float& fAspect,
								const _float& fNear,
								const _float& fFar);
	virtual void Free();
};

