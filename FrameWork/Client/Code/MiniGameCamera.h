#pragma once

#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

class CPlayer;
class CMiniGameCamera : public Engine::CCamera
{
	//enum STEP {STEP1,STEP2,STEP3,STEP4};
private:
	explicit	CMiniGameCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CMiniGameCamera();

public:
	//Getter
	_vec3		Get_Pos() { return m_vEye; }

	//Setter
	void		Set_MouseFix(_bool bBool) { m_bMouseFix = bBool; } //»Ò¡§∏∏µÎ
	void		Set_CamMove(_bool bBool) { m_bMoving = bBool; } //»Ò¡§∏∏µÎ
	void		LateInit();


private:
	void		Key_Input(const _double& dTimeDelta);
	void		Mouse_Input(const _double& dTimeDelta);
	void		Mouse_Fix();
	void		LookAtPlayer(const _double & dTimeDelta);

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
	_bool					m_bMoving; //UI ƒ—¡Æ¿÷¿ª§® §¿æ»øÚ¡˜¿Ã∞‘ «“∂Û∞Ì-> »Ò¡§ √ﬂ∞°
	Engine::CNaviMesh*		m_pNaviMeshCom;
	_ulong					m_dwNaviIndex;

	// It's Mine
	//_bool TestLock = false;
	//STEP m_eStep = STEP1;
	//_vec3 vTest;

	_vec3 vDir = {};

	//_vec3 v1 = { -1638.f, 398.f, -1070.f };
	////_vec3 v1 = { -545.f, 603.f, 1639.f };
	//_vec3 v2 = { -1913.f, 429.f, -553.f };
	//_vec3 v3 = { 1838.f, 505.f, -517.f };
	////_vec3 v4 = { 607.f, 397.f, -2030.f };
	//_vec3 v4 = { -1638.f, 398.f, -1070.f };
	//_float t = 0.f;

	//_bool bOnoff=false;

	//_float fOldDist;
	//_float fCurDist;
public:
	static CMiniGameCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
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