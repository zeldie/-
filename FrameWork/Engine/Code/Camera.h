#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera(void);

public:
	virtual HRESULT		Ready_GameObject(void);
	virtual _int		Update_GameObject(const _double& dTimeDelta);
	_vec3&		Get_Eye() { return m_vEye; }
	_vec3&		Get_At() { return m_vAt; }
	_float		Get_Far() { return m_fFar; }
	void		Set_MouseFix(_bool bBool) { m_bMouseFix = bBool; } //»Ò¡§∏∏µÎ
	void		Set_CamMove(_bool bBool) { m_bMoving = bBool; } //»Ò¡§∏∏µÎ
protected:
	_vec3			m_vEye, m_vAt, m_vUp;
	_float			m_fFovY, m_fAspect, m_fNear, m_fFar;
	_matrix			m_matView, m_matProj, m_matOrthoView;
	_bool					m_bMouseFix;
	_bool					m_bMoving;		//UI ƒ—¡Æ¿÷¿ª§® §¿æ»øÚ¡˜¿Ã∞‘ «“∂Û∞Ì-> »Ò¡§ √ﬂ∞°
protected:
	virtual void	Free(void);

};

END
#endif // Camera_h__
