#pragma once

#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include "Optimization.h"
#include "Shader.h"
#include "SphereCollider.h"
#include "CubeCollider.h"

class CCollisionMgr;
class CBaseObject : public Engine::CGameObject
{
protected:
	explicit CBaseObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBaseObject();

public:
	_bool										Get_isDead() { return m_bIsDead; }
	Engine::CTransform*							Get_TransformCom() { return m_pTransformCom; }
	Engine::CRenderer*							Get_Renderer() { return m_pRendererCom; }
	virtual vector<Engine::CSphereCollider*>*	Get_CollSpherVector() { return m_vecCollSphere; }
	virtual vector<Engine::CCubeCollider*>*		Get_CollBoxVector() { return m_vecCollBox; }
	virtual	const BASE_INFO*					Get_BaseInfo() { return &m_tBaseInfo; }

	void										Set_Dead(_bool bDead=true) { m_bIsDead = bDead; }
	HRESULT										Set_ShaderMatrix(LPD3DXEFFECT & pEffect); // Shader_base에 필요한것들을 한다.
	HRESULT										Set_ShaderShadow(LPD3DXEFFECT & pEffect); // For Shadow
	virtual	void								Set_Collision(CBaseObject* pObj) {};
	virtual	void								Set_Collision(CBaseObject* pObj, COLLIDER eColliderType) {};

	//컬링쓰고 싶으면 if(Culling())해서 True면 Add_RenderGroup 하게 만들면됨
	_bool										Culling();
	//빌보드의 스케일은 m_ptrasformcom의 scale과 각각 곱해져서 최종 스케일을 내보냄
	void										BillBoard(_vec3* _vScale = &_vec3(1.f, 1.f, 1.f));
public:
	virtual HRESULT								Ready_GameObject();
	virtual _int								Update_GameObject(const _double& dTimeDelta) override;
	virtual _int								LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void								Render_Depth(const _double& dTimeDelta) override;
	virtual void								Render_Geometry(const _double& dTimeDelta) override;
	virtual void								Render_PostEffect(const _double& dTimeDelta) override;
	virtual void								Render_Shadow(const _double& dTimeDelta) override;

protected:
	virtual _vec3			Create_Pos(const _matrix * pMatWorld, _vec3* pPos);





protected:
	Engine::CTransform*					m_pTransformCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::COptimization*				m_pOptimizationCom;
	Engine::CShader*					m_pShaderCom;

	CEffectMgr*							m_pEffectMgr;
	CCollisionMgr*						m_pCollisionMgr;
	//충돌 구체
	vector<Engine::CSphereCollider*>	m_vecCollSphere[COLLSPHERETYPE::SPHERE_END];
	vector<Engine::CCubeCollider*>		m_vecCollBox[COLLBOXTYPE::BOX_END];
	BASE_INFO							m_tBaseInfo;

	_bool								m_bIsDead;

public:
	virtual void								Free();
};

