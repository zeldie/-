#pragma once

#include "BaseObject.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"

class CDynamicMeshObject : public CBaseObject
{
protected:
	explicit CDynamicMeshObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicMeshObject();

public:
	HRESULT				Link_NaviMesh();
	HRESULT				LoadOuterCollider(WCHAR* wstrPath);
	HRESULT				LoadHitCollider(WCHAR* wstrPath);
	void				Calculate_Collision();
	// Test Atk Collider

	HRESULT LoadAtkCollider(WCHAR* wstrPath, _vec3* pPos);

	// Test End - ����Ʈ�� �浹ü�� ���̴� �۾��� ���� ��� ���� �Ǵ� �׿� �°� ���� �ʿ�

	// Getter
	//_bool				Get_isDead() { return m_bIsDead; }
	_vec3				GetDir(Engine::INFO eInfo);
	_ulong&				Get_NaviIndex() { return m_dwNaviIndex; }
	// Setter
	void				Set_Dead(_bool bDead) { m_bIsDead = bDead; }
	void				Set_NoRender(_bool bRender) { m_bNoRender = bRender; }
public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;
	virtual void		Render_PostEffect(const _double& dTimeDelta) override;
	virtual void		Render_Shadow(const _double& dTimeDelta) override;
protected:
	Engine::CDynamicMesh*				m_pDynamicMeshCom;
	Engine::CNaviMesh*					m_pNaviMeshCom;
	//�׺�Ž� �ε���
	_ulong								m_dwNaviIndex;
	//_bool								m_bIsDead;
	_bool								m_bNoRender;
	//�浹ó���� �ݶ��̴� ����
public:
	virtual void	Free() override;
};

