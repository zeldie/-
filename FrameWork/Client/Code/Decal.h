#pragma once

#include "BaseObject.h"
#include "CubeTex.h"
#include "Texture.h"
class CDecal : public CBaseObject
{
public:
	enum DECAL_TYPE
	{
		DECAL_SKILLTARGET, //���� 4�� �� ���ǵ� ��ų ���� ���ٴڿ� Ÿ���� �Ǵ°�
		DECAL_ORBF, // ���� F ��ų ���� ���� ���� , ������ �ڱ�
		DECAL_ORBR, //���� R ��ų ���� ���� ����
		DECAL_TS_F, // ��� F ��ų
		DECAL_BOW_FALL, //Ȱ E ��ų ����������  �Ʒ� ������
		DECAL_ORB_RIGHT, // ���� �������Ҷ� ���ۺ��� ���ư��� ���� �����ϰ� ��
		DECAL_ORB_E_BEAM, // ���� E��ų ���� ���� �������ϴ� ��Į
		DECAL_ORB_F, // ���� F ��ų ���� �ΰ��� �ٶ� ����Ʈ�� ��������
		DECAL_ORB_ULT, //�þ��� �Ʒ��� ����
		DECAL_ORB_FLYING, //���갡 ��� ��ﶧ
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
	HRESULT			Create_RainArrow(const _double& dTimeDelta); // Ȱ �������°�
	HRESULT			Create_Orb_Right(const _double& dTimeDelta); // ���� ��Ŭ��
	HRESULT			Create_Orb_Beam(const _double& dTimeDelta); //���� �������������� ��
	HRESULT			Create_Orb_Wind(const _double& dTimeDelta); // ���� �ٶ� �ֵ��°�
	HRESULT			Create_Orb_Ult(const _double& dTimeDelta); //���� �� ����
	HRESULT			Create_Orb_Flying(const _double& dTimeDelta); //���� ������ �������� ���ε� �ѹ߸�, ��ﶧ ��
	void			Move(const _double& dTimeDelta);


private:
	Engine::CCubeTex* m_pCubeCom;
	Engine::CTexture* m_pTexCom;
	DECAL_TYPE		  m_eDecalType;
	_float			  m_fDiminishRate; // ��Į ������� ���� ����
	_uint			  m_iPass; // �н�
	_vec4			  m_vUseMasking; // ����ŷ ���ų�
	_vec3*			  m_pDecalPos; //�� ��Į�� �� ��Į������ ����
	_double			  m_dCountDown; // ������� �ð�
	_double			  m_dEffectTerm; // ȭ��� ������ִ� �ð�, ���굵 ��
	_bool			  m_bCreateEffect;
	_vec3			  m_vPlayerAngle; // �� ��Į�� ���»���� ����
	CONTROLTYPE		  m_eControlType; // ���� �̰� ����
	_bool			  m_bNeedToReleaseDecal; //������ �ٷ� �������� AI�� Ȯ���ϰ� �װ� �ؾ���
public:
	static	CDecal*	Create(LPDIRECT3DDEVICE9 pGraphicDev, DECAL_TYPE eDecalType, _vec3* pPos, _double dCountDown, _vec3 vPlayerAngle, CONTROLTYPE eControlType);
	virtual void	Free();
};

