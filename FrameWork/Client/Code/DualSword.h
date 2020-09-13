#pragma once

#include "DynamicMeshObject.h"

class CDualSword : public CDynamicMeshObject
{
public:
	enum COMBAT_STATE { STATE_WAIT, STATE_COMBAT, STATE_END };
private:
	explicit CDualSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDualSword();

public:
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_PostEffect(const _double& dTimeDelta) override;
	virtual void	Render_Shadow(const _double& dTimeDelta) override;
	// Getter
	_bool			Get_isHide() { return m_bHide; }
	wstring			Get_WeaponName() { return m_wstrWeaponName; }

	// Setter
	void			Set_Hand(Engine::HAND eHand) { m_eHand = eHand; }
	void			Set_Name(wstring wstrPlayerName, wstring wstrWeaponName) { m_wstrPlayerName = wstrPlayerName;  m_wstrWeaponName = wstrWeaponName; }
	void			Set_Pass(_uint iPass) { m_iPass = iPass; }
	void			Set_Dissolve(_bool bDissolve) { m_bDissolve = bDissolve; }

	void			Reset_Dissolve();
private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	__forceinline	void	Set_CombatState(COMBAT_STATE eValue) { m_eNextCombat = eValue; }
	void					Set_Hide(_bool bValue) { m_bHide = bValue; }

private:
	COMBAT_STATE						m_eCurCombat;		// �������̸� DualSword_L�� ��ġ�� Weapon_L�� ���� �ٿ����Ѵ�.
	COMBAT_STATE						m_eNextCombat;		// �������� �ƴϸ� DualSword_L�� ��ġ�� Weapon_Spine_L�� ���� �ٿ����Ѵ�.
	Engine::HAND						m_eHand;
	//_bool								m_bIsCombat;
	_bool								m_bHide;			// E ��ų �����߰��� ������ٰ� �ٽ� ���δ�.

	wstring								m_wstrPlayerName;
	wstring								m_wstrWeaponName;

	_vec3								m_vEditPos;						// Weapon ������ �չٴ����� �̵���Ű�� ���� ������

	const _matrix*						m_pParentBoneMatrix;
	const _matrix*						m_pParentWorldMatrix;

	//������
	_bool								m_bDissolve;
	_double								m_dDissolveCountDown;
	_uint								m_iPass;
public:
	static CDualSword* Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::HAND eHand, wstring wstrPlayerName, wstring wstrWeaponName);
	virtual void Free() override;
};