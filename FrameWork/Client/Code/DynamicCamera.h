#pragma once

#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

class CPlayer;
class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

public:
	HRESULT		Ready_Object(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFovY,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_GameObject(const _double& dTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _double& dTimeDelta) override;

private:
	void Key_Input(const _float& fTimeDelta);
	void Mouse_Move(const _float& fTimeDelta);
	void Fix_Mouse(void);

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFovY,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

private:
	virtual void Free(void);
};