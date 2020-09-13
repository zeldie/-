#pragma once

#include "DynamicMeshObject.h"

class COrb : public CDynamicMeshObject
{
public:
	//enum WEAPONS_STATE {COMBAT, };
	enum ORB_STATE {
		BASICCOMBO01, BASICCOMBO02, BASICCOMBO03, BASICCOMBO04, BASICCOMBO05,
		BASICCOMBOEX01, BASICCOMBOEX02, JUMP_DOWN, JUMP_LANDING, JUMP_UP, RUN, RUN_END,
		SPECIALMOVE, WAIT
	};
private:
	explicit COrb(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COrb();

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
	//__forceinline	void	Change_WeaponState() { m_bIsCombat = !m_bIsCombat; }

	// Get
	wstring			Get_WeaponName() { return m_wstrWeaponName; }

	// Set
	void			Set_Name(wstring wstrPlayerName, wstring wstrWeaponName) { m_wstrPlayerName = wstrPlayerName;  m_wstrWeaponName = wstrWeaponName; }
	void			Set_OB_Animation(ORB_STATE eState);
	void			Set_Pass(_uint iPass) { m_iPass = iPass; }
	void			Set_Dissolve(_bool bDissolve) { m_bDissolve = bDissolve; }
	void			Reset_Dissolve();

	void			Set_Item(_bool bValue) { m_bItem = bValue; }
	void			Set_ItemPos(_vec3 vPos) { m_vItemPos = vPos; }

private:
	_bool m_bItem;
	_vec3 m_vItemPos;

	_bool m_bIsCombat;

	_vec3 m_vEditPos;						// Weapon 뼈에서 손바닥으로 이동시키기 위한 보정값

	wstring								m_wstrPlayerName;
	wstring								m_wstrWeaponName;

	const _matrix* m_pParentBoneMatrix;
	const _matrix* m_pParentWorldMatrix;
	
	ORB_STATE m_eState;

	//디졸브
	_bool								m_bDissolve;
	_double								m_dDissolveCountDown;
	_uint								m_iPass;
public:
	static COrb* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPlayerName, wstring wstrWeaponName);
	static COrb* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free() override;
};