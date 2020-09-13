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
		//X : ���� ��ġ, Y :  ��Ʋ�� ����, Z : Ʈ������ �ƴϸ鼭 ��Ʋ�󽺸� �Ⱦ���, UV�� �带���� �ӵ�
		_vec3 vMainUV_U = _vec3(0.f, 1.f, 0.f);
		_vec3 vMainUV_V = _vec3(0.f, 1.f, 0.f);

		_vec3 vMask1UV_U = _vec3(0.f, 1.f, 0.f);
		_vec3 vMask1UV_V = _vec3(0.f, 1.f, 0.f);
		_vec3 vMask2UV_U = _vec3(0.f, 1.f, 0.f);
		_vec3 vMask2UV_V = _vec3(0.f, 1.f, 0.f);

		_vec4 vUseMask1 = _vec4(1.f, 1.f, 1.f, 1.f); //���Ÿ� 0001 �Ⱦ��Ÿ� 1111
		_vec4 vUseMask2 = _vec4(1.f, 1.f, 1.f, 1.f);//���Ÿ� 0001 �Ⱦ��Ÿ� 1111
		_vec4 vUseDistortion = _vec4(1.f, 1.f, 1.f, 1.f);;//���Ÿ� 0001 �Ⱦ��Ÿ� 1111

		//�Ʒ� 6�� Ʈ���Ͽ�
		_vec2 vMainUVOffset = _vec2(0.f, 0.f); // Ʈ���� ���Ÿ� -1.f / m_tEffectProps.vMainUV_U.y Ʈ���� �Ⱦ��� �״�� 0,0
		_vec2 vMask1UVOffset = _vec2(0.f, 0.f);// Ʈ���� ���Ÿ� -1.f / m_tEffectProps.vMainUV_U.y Ʈ���� �Ⱦ��� �״�� 0,0
		_vec2 vMask2UVOffset = _vec2(0.f, 0.f);// Ʈ���� ���Ÿ� -1.f / m_tEffectProps.vMainUV_U.y Ʈ���� �Ⱦ��� �״�� 0,0
		_vec2 vMainOffsetSpeed = _vec2(0.f, 0.f);// Ʈ���� ���Ÿ� 2.f / m_tEffectInfo.fDeadTime Ʈ���� �Ⱦ��� �״�� 0,0
		_vec2 vMask1OffsetSpeed = _vec2(0.f, 0.f);// Ʈ���� ���Ÿ� 2.f / m_tEffectInfo.fDeadTime Ʈ���� �Ⱦ��� �״�� 0,0
		_vec2 vMask2OffsetSpeed = _vec2(0.f, 0.f);// Ʈ���� ���Ÿ� 2.f / m_tEffectInfo.fDeadTime, Ʈ���� �Ⱦ��� �״�� 0,0

		//speed = 2 / D
		_vec2 vUVMovePosSum = _vec2(0.f, 0.f);
		_vec2 vUVMovePosSum2 = _vec2(0.f, 0.f);
		_vec2 vUVMovePosSum3 = _vec2(0.f, 0.f);

		//����Ʈ�� ���� �������� �ϴ°�
		_vec4 vDiminishRate = _vec4(1.f, 1.f, 1.f, 1.f); // ���࿡ �������� �ҰŶ�� switch���� x,y,z�� dTimeDelta ���� ��

		//�ؽ��ĸ� ���ؾ��Ҷ�
		_vec4 vUseAdd = _vec4(0.f, 0.f, 0.f, 0.f); //���Ÿ� 1110 �Ⱦ��Ÿ� 0000
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

	HRESULT		Add_Collider(); //���⿡ EFFECT_PROPS�� �߰�
	void		Reset_EffectProps();
	void		Transform(const _double& dTimeDelta);

	void HoEffect();
private:
	Engine::CStaticMesh*			m_pStaticMeshCom;
	EFFECT_PROPS					m_tEffectProps;
	_bool							m_bwstrMask;
	Engine::CTransform*				m_pParentTransformCom; // ���� � ����Ʈ�� � ��ü�� �״�� ���󰡾��Ҷ� ��(ex) Ȱ - �ƿ��), ������ ���� Set_ParentTransformCom() �����
	Engine::CShader*				m_pMeshShaderCom;
	_double							m_dDissolveCountDown;
	_bool							m_bUseHDR;
public:
	static	CStaticEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free();

};

#endif // !__CPARTICLE_H__
