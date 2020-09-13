#pragma once

#include "BaseObject.h"
#include "CubeTex.h"
#include "Texture.h"
class CDecal : public CBaseObject
{
public:
	enum DECAL_TYPE
	{
		DECAL_SKILLTARGET, //무기 4개 다 쓸건데 스킬 쓰면 땅바닥에 타겟팅 되는거
		DECAL_ORBF, // 오브 F 스킬 쓰면 땅에 생김 , 검은색 자국
		DECAL_ORBR, //오브 R 스킬 쓰면 땅에 생김
		DECAL_TS_F, // 대검 F 스킬
		DECAL_BOW_FALL, //활 E 스킬 떨어졌을때  아래 마법진
		DECAL_ORB_RIGHT, // 오브 강공격할때 빙글빙글 돌아가는 오브 생성하게 함
		DECAL_ORB_E_BEAM, // 오브 E스킬 쓰면 빔을 떨구게하는 데칼
		DECAL_ORB_F, // 오브 F 스킬 쓰면 두개의 바람 이펙트가 나오게함
		DECAL_ORB_ULT, //궁쓰면 아래에 생김
		DECAL_ORB_FLYING, //오브가 상대 띄울때
		DECAL_END
	};
private:
	explicit CDecal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDecal();
public:
	HRESULT			Ready_GameObject(DECAL_TYPE eDecalType, _vec3* pPos, _double dCountDown, _vec3 vPlayerAngle, CONTROLTYPE eControlType);
	virtual	_int	Update_GameObject(const _double& dTimeDelta);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta);
	virtual	void	Render_Geometry(const _double& dTimeDelta);

	_bool			Get_NeedToReleaseDecal() { return m_bNeedToReleaseDecal; }
private:
	HRESULT			Clone_Component(DECAL_TYPE eDecalType);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect, const _double& dTimeDelta);
	HRESULT			Create_Effect(const _double& dTimeDelta);
private:
	HRESULT			Create_RainArrow(const _double& dTimeDelta); // 활 떨어지는거
	HRESULT			Create_Orb_Right(const _double& dTimeDelta); // 오브 우클릭
	HRESULT			Create_Orb_Beam(const _double& dTimeDelta); //오브 위에서떨어지는 빔
	HRESULT			Create_Orb_Wind(const _double& dTimeDelta); // 오브 바람 휘도는거
	HRESULT			Create_Orb_Ult(const _double& dTimeDelta); //오브 궁 생성
	HRESULT			Create_Orb_Flying(const _double& dTimeDelta); //오브 위에서 떨어지는 빔인데 한발만, 띄울때 씀
	void			Move(const _double& dTimeDelta);


private:
	Engine::CCubeTex* m_pCubeCom;
	Engine::CTexture* m_pTexCom;
	DECAL_TYPE		  m_eDecalType;
	_float			  m_fDiminishRate; // 데칼 사라지는 정도 조절
	_uint			  m_iPass; // 패스
	_vec4			  m_vUseMasking; // 마스킹 쓸거냐
	_vec3*			  m_pDecalPos; //이 데칼은 이 데칼포스를 따라감
	_double			  m_dCountDown; // 사라지는 시간
	_double			  m_dEffectTerm; // 화살비 만들어주는 시간, 오브도 씀
	_bool			  m_bCreateEffect;
	_vec3			  m_vPlayerAngle; // 이 데칼을 쓰는사람의 각도
	CONTROLTYPE		  m_eControlType; // 누가 이걸 쓰냐
	_bool			  m_bNeedToReleaseDecal; //죽을때 바로 죽지말고 AI가 확인하고 죽게 해야함
public:
	static	CDecal*	Create(LPDIRECT3DDEVICE9 pGraphicDev, DECAL_TYPE eDecalType, _vec3* pPos, _double dCountDown, _vec3 vPlayerAngle, CONTROLTYPE eControlType);
	virtual void	Free();
};

