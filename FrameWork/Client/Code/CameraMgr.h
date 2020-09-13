#pragma once
#include "Camera.h"

#include "Player.h"
#include "Renderer.h"
class CCameraMgr : public CBase
{
	DECLARE_SINGLETON(CCameraMgr)

public:
	enum MODE { STATIC, DYNAMIC, RUNGAME, ACTION, CUSTOM, MODE_END };

private:
	explicit CCameraMgr();
	virtual ~CCameraMgr();

public:
	void Ready_Camera(LPDIRECT3DDEVICE9 pGraphicDev, MODE eMode);

public:
	_vec3& Get_Eye();
	_vec3& Get_At();
public:
	void Update_Camera(const _double& dTimeDelta);
	void LateUpdate_Camera(const _double& dTimeDelta);
	void Change_Camera(MODE eMode);
	void Set_MouseFix(_bool bBool); 
	void Set_CamMove(_bool bBool);  
	void Set_RageCam(CPlayer::PLAYER_WEAPONTYPE eWeapon);
	void Set_BelatosCutscene();
	void Set_ApostleCutscene();
	void Set_ShakingCamera();
	void Set_FlagCutscene();
	void LateInit();

	void Set_ItemGetComplete(_bool bComplete) { m_bBelatosComplete = bComplete; }
	_bool Get_ItemGetCheck() { return m_bItemGetComplete; }
	//¿Œ±’
public:
	void	Set_RendererCom();
	void	SettingCamView(); //∑ª¥ı∑Øø°º≠ æ∏
	_float	Get_CamFar();
public: // Test
	void Set_ActionZoom();
	void ReSet_ActionZoom();
	void Set_ItemGet();
		//Test End
public:
	_vec3 Get_Pos();
	_vec3 Get_PosRun();
	_bool Get_Cerate() { return m_bCreate; }
private:
	Engine::CCamera*			 m_pCam;
	MODE						 m_eMode;
	vector<Engine::CCamera*>	 m_vecCam[MODE_END];
	_bool						 m_bCreate;
	_bool						 m_bBelatosComplete;
	_bool						 m_bApostleComplete;
	_bool						 m_bFlagComplete;
	_bool						 m_bItemGetComplete;
	CPlayer::PLAYER_WEAPONTYPE	 m_eWeapon;
	Engine::CRenderer*			 m_pRendererCom;
public:
	// CBase¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual void Free(void) override;
};
