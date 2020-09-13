#pragma once
#ifndef __DYNAMICEFFECT_H__
#define __DYNAMICEFFECT_H__

#include "BaseEffect.h"
#include "DynamicMesh.h"

class CDynamicEffect : public CBaseEffect
{
public:
	struct EFFECT_PROPS
	{
		//X : 시작 위치, Y :  아틀라스 개수, Z : 트레일이 아니면서 아틀라스를 안쓰는, UV가 흐를때의 속도
		_vec3 vMainUV_U = _vec3(0.f, 1.f, 0.f);
		_vec3 vMainUV_V = _vec3(0.f, 1.f, 0.f);
		_vec3 vMask1UV_U = _vec3(0.f, 1.f, 0.f);
		_vec3 vMask1UV_V = _vec3(0.f, 1.f, 0.f);
		_vec3 vMask2UV_U = _vec3(0.f, 1.f, 0.f);
		_vec3 vMask2UV_V = _vec3(0.f, 1.f, 0.f);
		_vec4 vUseMask1 = _vec4(1.f, 1.f, 1.f, 1.f); //쓸거면 0001 안쓸거면 1111
		_vec4 vUseMask2 = _vec4(1.f, 1.f, 1.f, 1.f);//쓸거면 0001 안쓸거면 1111
		_vec4 vUseDistortion = _vec4(1.f, 1.f, 1.f, 1.f);;//쓸거면 0001 안쓸거면 1111
		 //아래 6개 트레일용
		_vec2 vMainUVOffset = _vec2(0.f, 0.f); // 트레일 쓸거면 -1, -1, 트레일 안쓰면 그대로 0,0
		_vec2 vMask1UVOffset = _vec2(0.f, 0.f);// 트레일 쓸거면 -1, -1, 트레일 안쓰면 그대로 0,0
		_vec2 vMask2UVOffset = _vec2(0.f, 0.f);// 트레일 쓸거면 -1, -1, 트레일 안쓰면 그대로 0,0
		_vec2 vMainOffsetSpeed = _vec2(0.f, 0.f);// 트레일 쓸거면 1, 1, 트레일 안쓰면 그대로 0,0
		_vec2 vMask1OffsetSpeed = _vec2(0.f, 0.f);// 트레일 쓸거면 1, 1, 트레일 안쓰면 그대로 0,0
		_vec2 vMask2OffsetSpeed = _vec2(0.f, 0.f);// 트레일 쓸거면 1, 1, 트레일 안쓰면 그대로 0,0
		 //speed = 2 / D
		_vec2 vUVMovePosSum = _vec2(0.f, 0.f);
	};
private:
	explicit CDynamicEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicEffect();

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
	HRESULT		Clone_DynamicCom();

	HRESULT		Add_Collider();

	HRESULT		LoadOuterCollider(WCHAR* wstrPath);
	HRESULT		LoadAtkCollider(WCHAR* wstrPath, _vec3* pPos);

private:
	Engine::CDynamicMesh*			m_pDynamicMeshCom;
	EFFECT_PROPS					m_tEffectProps;
	_double							m_dDissolveCountDown;
public:
	static	CDynamicEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free();

};

#endif // !__CPARTICLE_H__
