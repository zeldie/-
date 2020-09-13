#pragma once

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CCollider : public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();

public:
	_bool			Get_IsDead() { return m_bIsDead; }
	MESHTYPE		Get_ParentMeshType() { return m_eParentMeshType; }
	
	void			Set_ColType(COLLTYPE eCollType) { m_eCollType = eCollType; }
	void			Set_Collision(_bool bCollision) { m_bCollision = bCollision; }
	void			Set_Dead(_bool bDead) { m_bIsDead = bDead; }
public:
	virtual _int	Update_Component(const _double& dTimeDelta) override;
	
protected:
	_matrix						m_matParent; // �Ʒ� �ΰ� ��� ���� ���� ��� or �θ� ��� ����ü
	const	_matrix*			m_pmatParentBone;
	const	_matrix*			m_pmatParentWorld;

	COLLTYPE					m_eCollType; //���򶫿�
	_bool						m_bCollision;
	MESHTYPE					m_eParentMeshType; // �� �ݶ��̴��� �������ִ� ������Ʈ�� �޽� Ÿ��
	_bool						m_bIsDead;
#ifdef _DEBUG
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];
#endif

public:
	virtual	CComponent*		Clone() override;
	virtual void			Free();

};

END