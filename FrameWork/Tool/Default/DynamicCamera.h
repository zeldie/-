#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

BEGIN(Engine)
class CCalculator;
END
class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

public:
	HRESULT			Ready_GameObject(const _vec3* pEye,
									const _vec3* pAt,
									const _vec3* pUp,
									const _float& fFovY,
									const _float& fAspect,
									const _float& fNear,
									const _float& fFar);

	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual _int	LateUpdate_GameObject(const _double& dTimeDelta) override;

private:
	HRESULT			Clone_Component();
private:
	void			Key_Input(const _double& dTimeDelta);
	void			Mouse_Move(const _double& dTimeDelta);
	void			Fix_Mouse();

private:
	_float						m_fSpeed;
	_bool						m_bClick;
	_bool						m_bFix;

	Engine::CCalculator*		m_pCalculatorCom;
public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										const _float& fFovY,
										const _float& fAspect,
										const _float& fNear,
										const _float& fFar);

private:
	virtual void Free() override;

};

#endif // DynamicCamera_h__
