#pragma once

#include "StaticMeshObject.h"
class CTSDecoration : public CStaticMeshObject
{
public:
	enum COMBAT_STATE { STATE_WAIT, STATE_COMBAT, STATE_END };

private:
	explicit CTSDecoration(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTSDecoration();

public:
	//Setter
	void Set_CombatState(COMBAT_STATE eType) {	m_eCombatState = eType;	}
	void Set_UltimateRender(_bool bBool = true) { m_bUltimateRender = bBool; }
	void Set_NoDead(_bool bBool) { m_bNoDead = bBool; }

	//Getter
	_bool Get_NoDead() { return m_bNoDead; }

public:
	virtual HRESULT Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, wstring wstrPlayerName, wstring wstrWeaponName);
	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_Shadow(const _double& dTimeDelta) override;
private:
	void			Change_WeaponState(COMBAT_STATE eState);

private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	_vec3	m_vEditPos;						// Weapon 뼈에서 손바닥으로 이동시키기 위한 보정값

	wstring								m_wstrPlayerName;
	wstring								m_wstrWeaponName;

	const _matrix*						m_pParentBoneMatrix;
	const _matrix*						m_pParentWorldMatrix;
	
	COMBAT_STATE						m_eCombatState;
	_bool								m_bUltimateRender; //q스킬 발동 시에만 그려진다
	_double								m_dUltimateTime;
	
	_bool								m_bNoDead;//무적 치트

public:
	static CTSDecoration* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, wstring wstrPlayerName,wstring wstrWeaponName);
	virtual void Free() override;
};

