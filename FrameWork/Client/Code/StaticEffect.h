#pragma once
#ifndef __STATICEFFECT_H__
#define __STATICEFFECT_H__

#include "BaseEffect.h"
#include "StaticMesh.h"

class CStaticEffect : public CBaseEffect
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
		_vec2 vMainUVOffset = _vec2(0.f, 0.f); // 트레일 쓸거면 -1.f / m_tEffectProps.vMainUV_U.y 트레일 안쓰면 그대로 0,0
		_vec2 vMask1UVOffset = _vec2(0.f, 0.f);// 트레일 쓸거면 -1.f / m_tEffectProps.vMainUV_U.y 트레일 안쓰면 그대로 0,0
		_vec2 vMask2UVOffset = _vec2(0.f, 0.f);// 트레일 쓸거면 -1.f / m_tEffectProps.vMainUV_U.y 트레일 안쓰면 그대로 0,0
		_vec2 vMainOffsetSpeed = _vec2(0.f, 0.f);// 트레일 쓸거면 2.f / m_tEffectInfo.fDeadTime 트레일 안쓰면 그대로 0,0
		_vec2 vMask1OffsetSpeed = _vec2(0.f, 0.f);// 트레일 쓸거면 2.f / m_tEffectInfo.fDeadTime 트레일 안쓰면 그대로 0,0
		_vec2 vMask2OffsetSpeed = _vec2(0.f, 0.f);// 트레일 쓸거면 2.f / m_tEffectInfo.fDeadTime, 트레일 안쓰면 그대로 0,0

		//speed = 2 / D
		_vec2 vUVMovePosSum = _vec2(0.f, 0.f);
		_vec2 vUVMovePosSum2 = _vec2(0.f, 0.f);
		_vec2 vUVMovePosSum3 = _vec2(0.f, 0.f);

		//이펙트가 점점 옅어지게 하는것
		_vec4 vDiminishRate = _vec4(1.f, 1.f, 1.f, 1.f); // 만약에 옅어지게 할거라면 switch에서 x,y,z에 dTimeDelta 빼면 됨

		//텍스쳐를 더해야할때
		_vec4 vUseAdd = _vec4(0.f, 0.f, 0.f, 0.f); //쓸거면 1110 안쓸거면 0000
	};
private:
	explicit CStaticEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticEffect();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) ;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) ;
	virtual void		Render_Geometry(const _double& dTimeDelta) ;

public:
	void		Set_Info(EFFECT_INFO tEffectInfo, const _matrix* matWorld, _vec3 * pAngle, BASE_INFO tBaseInfo);
	void		Set_ParentTransformCom(Engine::CTransform* pParentTransformCom) { m_pParentTransformCom = pParentTransformCom; }
private:
	HRESULT		Clone_Component();
	_vec3		Create_Angle();
	//Shader
	HRESULT		Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	HRESULT		Clone_StaticCom();

	HRESULT		Add_Collider(); //여기에 EFFECT_PROPS도 추가
	void		Reset_EffectProps();
	void		Transform(const _double& dTimeDelta);

	void HoEffect();
private:
	Engine::CStaticMesh*			m_pStaticMeshCom;
	EFFECT_PROPS					m_tEffectProps;
	_bool							m_bwstrMask;
	Engine::CTransform*				m_pParentTransformCom; // 만약 어떤 이펙트가 어떤 객체를 그대로 따라가야할때 씀(ex) 활 - 아우라), 쓸려면 위에 Set_ParentTransformCom() 쓰면됨
	Engine::CShader*				m_pMeshShaderCom;
	_double							m_dDissolveCountDown;
	_bool							m_bUseHDR;
public:
	static	CStaticEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free();

};

#endif // !__CPARTICLE_H__
