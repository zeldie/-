#pragma once

#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

class CRunPlayer;
class CRunGameCamera : public Engine::CCamera
{
public:
	enum STEP {STEP1,STEP2,STEP3,STEP4,STEP5};
private:
	explicit	CRunGameCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CRunGameCamera();

public:
	//Getter
	//_vec3		Get_Eye() { return m_vEye; }
	//_vec3		Get_At() { return m_vAt; }

	//Setter
	void		LateInit();

private:
	void		Key_Input(const _double& dTimeDelta);
	void		Mouse_Input(const _double& dTimeDelta);
	void		Mouse_Fix();
	void		LookAtPlayer(const _double & dTimeDelta);

	void		Item_Get(const _double & dTimeDelta);
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
	void	Set_ItemGet(_bool bResult) { m_bGetItem = bResult; }
public:
	_bool					m_bClick;
	_bool					m_bMouseFix;
	_float					m_fCamAngle;

	//Player
	Engine::CTransform*		pPlayerTransform;

	_vec3					m_vNextEye;
	_vec3					m_vNextAt;
	_vec3					m_vLook;

	_float					m_fDist;
	_float					m_fRotX;
	_float					m_fRotY;
								   
	_double					m_dTime;

	_bool					m_bLateInit;

	STEP m_eStep;
	Engine::CTransform*		m_pItemTransfrom;
	_vec3 vLastEye;
	_float m_fRatio;
	_bool					m_bGetItem;
public:
	static CRunGameCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
								const _vec3* pEye,
								const _vec3* pAt,
								const _vec3* pUp,
								const _float& fFovY,
								const _float& fAspect,
								const _float& fNear,
								const _float& fFar);

private:
	virtual void			Free() override;
};