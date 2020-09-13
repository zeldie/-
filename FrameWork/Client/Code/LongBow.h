#pragma once

#include "DynamicMeshObject.h"

class CLBDecoration;
class CLongBow final: public CDynamicMeshObject
{
public:
	enum COMBAT_STATE { STATE_WAIT, STATE_COMBAT, STATE_END };
private:
	explicit CLongBow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLongBow();

public:
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_PostEffect(const _double& dTimeDelta) override;
	virtual void	Render_Shadow(const _double& dTimeDelta) override;
private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
public:
	void			Change_WeaponState(COMBAT_STATE eState);

	// Get
	wstring			Get_WeaponName() { return m_wstrWeaponName; }

	// Set
	void			Set_Name(wstring wstrPlayerName, wstring wstrWeaponName) { m_wstrPlayerName = wstrPlayerName;  m_wstrWeaponName = wstrWeaponName; }
	void			Set_Pass(_uint iPass) { m_iPass = iPass; }
	void			Set_Dissolve(_bool bDissolve) { m_bDissolve = bDissolve; }
	void			Reset_Dissolve();
	void			Set_CombatState(COMBAT_STATE eValue) { m_eCombatState = eValue; Change_WeaponState(m_eCombatState); } //희정 추가 - 듀얼소드 참고함

	void			Set_LBDissolve(_bool bDissolve);
	void			Reset_LBDissolve();
private:
	_bool m_bIsCombat;

	_vec3 m_vEditPos;						// Weapon 뼈에서 손바닥으로 이동시키기 위한 보정값

	wstring								m_wstrPlayerName;
	wstring								m_wstrWeaponName;

	const _matrix* m_pParentBoneMatrix;
	const _matrix* m_pParentWorldMatrix;

	CLBDecoration*	m_pLBDecoration;

	//디졸브
	_bool								m_bDissolve;
	_double								m_dDissolveCountDown;
	_uint								m_iPass;
	COMBAT_STATE	m_eCombatState;
public:
	static CLongBow* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPlayerName, wstring wstrWeaponName);
	virtual void Free() override;
};