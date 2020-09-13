#pragma once
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

class CCustomizeCamera : public Engine::CCamera
{
private:
	explicit CCustomizeCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCustomizeCamera();

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
	void		LateInit();
	void		MouseInput(const _double& dTimeDelta);
private:
	// Target
	Engine::CTransform*		m_pTargetTransform;

	_bool	m_bLateInit;
	_float	m_fUpDown;
	_float  m_fEyeY;
	_float  m_fZoom;

	_bool m_bTestHead=false;
	_bool m_bTestBody=false;
public:
	static CCustomizeCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
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