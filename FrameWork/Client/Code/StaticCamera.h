#pragma once

#include "Camera.h"
#include "Player.h"

BEGIN(Engine)
class CTransform;
END

class CPlayer;
class CStaticCamera : public Engine::CCamera
{
	enum STEP { STEP1, STEP2, STEP3, STEP4, STEP5, STEP6, STEP7, STEP8, STEP9 };
	enum SHAKING { LEVEL1, LEVEL2, LEVEL3, LEVEL4, LEVEL5, LEVEL6, LEVEL7, LEVEL8, LEVEL9, LEVEL_END };
private:
	explicit	CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CStaticCamera();

public:
	//Getter
	_vec3		Get_Pos() { return m_vEye; }

	//Setter
	//void		Set_MouseFix(_bool bBool) { m_bMouseFix = bBool; } //»Ò¡§∏∏µÎ
	//void		Set_CamMove(_bool bBool) { m_bMoving = bBool; } //»Ò¡§∏∏µÎ
	void		LateInit();

	void		Set_ActionZoom() { m_bActionZoom = true; }
	void		Set_BelatosCutscene() { m_bCutScene = true; }
	void		Set_ApostleCutscene() { m_bCutScene = true; }
	void		Set_FlagCutscene() { m_bFlag = true; }
	void		Set_Shaking() { m_bShaking = true; }
	void		ReSet_ActionZoom() { m_bActionZoom = false; }
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
	void Shaking_Camera(const _double & dTimeDelta);
	void Normal_Update(const _double & dTimeDelta);
	void Action_Zoom(const _double & dTimeDelta);
	void Reset_Camera(const _double & dTimeDelta);
	void Rage_Action(const _double & dTimeDelta);
	void Set_RageCam(CPlayer::PLAYER_WEAPONTYPE eWeapon);

	void TS_Rage(const _double & dTimeDelta);
	void DS_Rage(const _double & dTimeDelta);
	void LB_Rage(const _double & dTimeDelta);
	void OB_Rage(const _double & dTimeDelta);

	void CutScene_Belatos(const _double & dTimeDelta);
	void CutScene_Apostle(const _double & dTimeDelta);
	void CutScene_Flag(const _double & dTimeDelta);

	void Shooting_Camera(const _double& dTimeDelta);
	void EndCutScene_APostle(const _double & dTimeDelta);
public:
	_bool					m_bClick;
	_bool					m_bActionZoom;
	_bool					m_bRecov;
	
	_bool					m_bRage;
	_bool					m_bCutScene;
	_bool					m_bShaking;
	_bool					m_bFlag;
	_bool					m_bTopView;
	_bool					m_bEndCutScene;

	_float					m_fCamAngle;
	_float					m_fRatio;
	_float					m_fUpDown;

	//Player
	Engine::CTransform*		pPlayerTransform;
	Engine::CTransform*		m_pTargetTransform;

	_vec3					m_vNextEye;
	_vec3					m_vNextAt;
	_vec3					m_vLook;
	_vec3					m_vAtUp;

	_float					m_fDist;
	_float					m_fRotX;
	_float					m_fRotY;
								   
	_double					m_dTime;
	_double					m_dbShakingTime;

	_int					m_iShakingCnt;

	_bool					m_bLateInit;
	Engine::CNaviMesh*		m_pNaviMeshCom;
	_ulong					m_dwNaviIndex;

	// It's Mine
	_bool m_bSetComplet = false;
	_bool m_bRageCam = false;
	

	_float m_fEyeDist = 0.f;
	_float m_fAtDist = 0.f;

	_double m_dbRecovTime=0.0;

	STEP m_eStep = STEP1;
	SHAKING m_eLevel;
	CPlayer::PLAYER_WEAPONTYPE	m_eWeapon;
	_vec3 vTest;

	_vec3 vDir = {};

	_vec3 vAostlePos;

	//_vec3 v1 = { -1638.f, 398.f, -1070.f };
	////_vec3 v1 = { -545.f, 603.f, 1639.f };
	//_vec3 v2 = { -1913.f, 429.f, -553.f };
	//_vec3 v3 = { 1838.f, 505.f, -517.f };
	////_vec3 v4 = { 607.f, 397.f, -2030.f };
	//_vec3 v4 = { -1638.f, 398.f, -1070.f };
	//_float t = 0.f;

	_bool bOnoff=false;

	_float fOldDist;
	_float fCurDist;

public:
	static CStaticCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
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