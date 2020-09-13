#pragma once

#include "StaticMeshObject.h"
class CWater : public CStaticMeshObject
{
	struct Mtrl //물의 표면 재질
	{
		D3DXCOLOR	vambient;
		D3DXCOLOR	vdiffuse;
		D3DXCOLOR	vspec;
		_float  fspecPower;
	};

	struct DirLight //방향성 광원 구조체
	{
		D3DXCOLOR vambient;
		D3DXCOLOR vdiffuse;
		D3DXCOLOR vspec;
		_vec3 vdirW;
	};
private:
	explicit CWater(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWater();

public:
	HRESULT			Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual	void	Render_Geometry(const _double& dTimeDelta)override;

private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
public:
	static	CWater*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void	Free();

private:
	_vec2 m_vWaveVelocity0;
	_vec2 m_vWaveVelocity1;
	_float m_fRefractBias;
	_float m_fRefractPower;
	_vec2 m_vRippleScale;
	_vec2 m_vWaveOffset0;
	_vec2 m_vWaveOffset1;

};

