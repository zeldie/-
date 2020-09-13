#pragma once

#include "DynamicMeshObject.h"

class CTSDecoration;
class CTwohandSword : public CDynamicMeshObject
{
public:
	enum COMBAT_STATE { STATE_WAIT, STATE_COMBAT, STATE_END };
	enum ULTIMATE_STATE{ ULTIMATE_ON, ULTIMATE_OFF, ULTIMATE_END}; //�����߰�

private:
	explicit CTwohandSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTwohandSword();

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
	ULTIMATE_STATE& Get_UltimateState() { return m_eUltimateState; }

	// Set
	void			Set_Name(wstring wstrPlayerName, wstring wstrWeaponName) { m_wstrPlayerName = wstrPlayerName;  m_wstrWeaponName = wstrWeaponName; }
	void			Set_CombatState(COMBAT_STATE eValue) { m_eCombatState = eValue; Change_WeaponState(m_eCombatState); } //���� �߰� - ���ҵ� ������
	void			Set_TSDecoRender(_bool bBool = true);
	void			Set_UltimateState(ULTIMATE_STATE eState) { m_eUltimateState = eState; }
	void			Set_UltimateNoDead();
	void			Set_Pass(_uint iPass) { m_iPass = iPass; }
	void			Set_Dissolve(_bool bDissolve) { m_bDissolve = bDissolve; }
	void			Reset_Dissolve();
private:

	_vec3 m_vEditPos;					// Weapon ������ �չٴ����� �̵���Ű�� ���� ������

	const _matrix*						m_pParentBoneMatrix;
	const _matrix*						m_pParentWorldMatrix;

	COMBAT_STATE						m_eCombatState;
	ULTIMATE_STATE						m_eUltimateState; //����

	wstring								m_wstrPlayerName;
	wstring								m_wstrWeaponName;

	CTSDecoration*						m_pTSDecoration; //����

	//������
	_bool								m_bDissolve;
	_double								m_dDissolveCountDown;
	_uint								m_iPass;

public:
	static CTwohandSword* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPlayerName, wstring wstrWeaponName);
	virtual void Free() override;
};