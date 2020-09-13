#pragma once

#include "StaticMeshObject.h"
class CLBDecoration : public CStaticMeshObject
{
private:
	explicit CLBDecoration(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLBDecoration();

public:
	virtual HRESULT Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, wstring wstrPlayerName);
	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;

public:
	void			Set_Dissolve(_bool bDissolve) { m_bDissolve = bDissolve; }
	void			Reset_Dissolve();
private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	_vec3 m_vEditPos;						// Weapon ������ �չٴ����� �̵���Ű�� ���� ������

	wstring								m_wstrPlayerName;
	wstring								m_wstrWeaponName;

	const _matrix* m_pParentBoneMatrix;
	const _matrix* m_pParentWorldMatrix;

	//������
	_bool								m_bDissolve;
	_double								m_dDissolveCountDown;
	_uint								m_iPass;
public:
	static CLBDecoration* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, wstring wstrPlayerName);
	virtual void Free() override;
};

