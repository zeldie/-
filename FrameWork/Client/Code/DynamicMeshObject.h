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

	// Test End - 이펙트에 충돌체를 붙이는 작업이 끝날 경우 삭제 또는 그에 맞게 변경 필요

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
	//네비매쉬 인덱스
	_ulong								m_dwNaviIndex;
	//_bool								m_bIsDead;
	_bool								m_bNoRender;
	//충돌처리용 콜라이더 모음
public:
	virtual void	Free() override;
};

