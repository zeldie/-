#pragma once
#ifndef __CCylinderEffect_H__
#define __CCylinderEffect_H__

#include "BaseEffect.h"
#include "StaticMesh.h"

class CCylinderEffect : public CBaseEffect
{
private:
	explicit CCylinderEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCylinderEffect();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) ;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) ;
	virtual void		Render_Geometry(const _double& dTimeDelta) ;

public:
	void		Set_Info(EFFECT_INFO tEffectInfo, const _matrix* matWorld, _vec3 * pAngle, BASE_INFO tBaseInfo);

private:
	HRESULT		Clone_Component();
	_vec3		Create_Angle();
	//Shader
	HRESULT		Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	HRESULT		Clone_StaticCom();
	HRESULT		Add_Collider();
private:
	//Engine::CStaticMesh*			m_pStaticMeshCom;
	LPD3DXMESH						m_pMesh;

public:
	static	CCylinderEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free();

};

#endif // !__CPARTICLE_H__
