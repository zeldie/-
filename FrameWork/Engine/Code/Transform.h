#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	explicit CTransform(LPDIRECT3DDEVICE9 pgraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	_vec3*			Get_Info(INFO eType);
	void			Get_Info(INFO eType, _vec3* pVector);
	void			Get_Info_byWorld(INFO eType, _vec3* pVector);
	_vec3			Get_Info_byWorld(INFO eType);
	_vec3*			Get_Scale();
	_vec3*			Get_Angle();
	_vec3*			Get_SelfRotAngle();
	const _vec3*	Get_Revolution();
	_float			Get_Speed();
	void			Get_WorldMatrix(_matrix* pWorld)const;
	const _matrix*	Get_WorldMatrix() const;
	const _matrix*	Get_NRotWorldMatrix(_matrix* pWorld) const;
	_matrix*		Get_WorldMatrix();

	void			Set_Info(_float fX, _float fY, _float fZ, INFO eType);
	void			Set_Info(_vec3* pInfo, INFO eType);
	void			Set_Scale(_float fX, _float fY, _float fZ);
	void			Set_Scale(_vec3* pScale);
	void			Set_Angle(_float fX, _float fY, _float fZ);
	void			Set_Angle(_vec3* pAngle);
	void			Set_Revolution(_float fX, _float fY, _float fZ);
	void			Set_Revolution(_vec3* pRevolution);
	void			Set_Speed(_float fSpeed);
	void			Set_Matrix(const _matrix* pMatrix);
	void			Set_ParentMatrix(const _matrix* pParent);
	void			Set_Pos(_float fX, _float fY, _float fZ);
	void			Set_Pos(_vec3* pPos);
	void			Set_RevParentMatrix(_matrix* pParent); //°øÀüÇÒ¶§ ¾¸
	void			Set_ParentPos(_float fX, _float fY, _float fZ);
	void			Set_ParentPos(_vec3* pPos);
	void			Set_SelfRotAngle(_vec3* vSelfRotAngle);

	float			Calculate_Distance(_vec3 vPos);

	void			Move_Pos(const _vec3* pDir, _double dSpeed, const _double& dTimeDelta);
	void			Move_Parent(const _vec3* pDir, _double dSpeed, const _double& dTimeDelta);
	void			Rotation(ROTATION eType, const _float& fAngle);
	void			Revolution(ROTATION eType, const _float& fAngle);
	const _matrix*	Compute_LookAtTarget(const _vec3* pTargetPos);
	void			Chase_Target(const _vec3* pTargetPos, const _float& fSpeed);
	void			Look_Target(_vec3 pTargetPos);
	_bool			Change_Scale(_vec3* vScale);
	//Orb Move
	void			RotationSelf(ROTATION eType, const _float& fAngle);
	void			Revolution_XZ(_double dAccTime, const _float& fAngle, _bool bRight);
	void			RotationParent(ROTATION eType, const _float& fAngle);
public:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_Transform();

	virtual _int	Update_Component(const _double& dTimeDelta);

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_float			m_fSpeed;
	_matrix			m_matWorld;
	_matrix			m_matNRotWorld;
	_matrix*		m_pParent;
	_vec3			m_vRevAngle;
	_vec3			m_vSelfRotAngle;
public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone() override;
	virtual void			Free();
};

END