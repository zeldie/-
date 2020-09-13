#pragma once

#include "StaticMeshObject.h"
class COrbBeam : public CStaticMeshObject
{
public:
	explicit COrbBeam(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COrbBeam();

public:
	HRESULT			Ready_GameObject(_vec3* pPos, _float fCreateEffectPosY, CONTROLTYPE eControlType);
	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;
private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	void			Move(const _double& dTimeDelta);
private:
	_float			m_fCreateEffectPosY;
	_bool			m_bCreateEffect;
	_double			m_dCountDown;
	CONTROLTYPE		m_eControlType;
public:
	static COrbBeam* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _float fCreateEffectPosY, CONTROLTYPE eControlType);
	virtual void Free() override;
};

