#pragma once

#include "GameObject.h"
#include "Renderer.h"
#include "Transform.h"
#include "Shader.h"
class CToolGameObject : public Engine::CGameObject
{
protected:
	explicit CToolGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolGameObject();

public:
	virtual HRESULT		Ready_GameObject() override;
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	_bool				IsDead() { return m_bIsDead; }

	void				Set_Dead() { m_bIsDead = true; }
	void				Set_WireFrame(_bool bWireFrame) { m_bIsWireFrame = bWireFrame; }

	Engine::CTransform*	Get_Transform() { return m_pTransformCom; }
	HRESULT				Set_ShaderMatrix(LPD3DXEFFECT & pEffect);

	void				Add_TextureInfo(Engine::EFFECT_UV_INFO eInfo);
	void				Delete_TextureInfo(Engine::EFFECT_UV_INFO eInfo);
	void				Delete_TextureInfoIdx(_uint iIdx);
	Engine::EFFECT_UV_INFO*		Get_TextureInfo(_uint iIdx);
	void				Set_TextureInfo(_uint iIdx, Engine::EFFECT_UV_INFO eInfo);
protected:
	Engine::CRenderer*					m_pRendererCom;
	Engine::CTransform*					m_pTransformCom;
	Engine::CShader*					m_pShaderCom;
	Engine::CTexture*					m_pTextureCom;
	_bool								m_bIsDead;
	_bool								m_bIsWireFrame;
	//¿Ã∆Â∆Æ≈¯
public:
	_vec3								m_vOrginPos;
	_vec3								m_vOrginRot;
	_vec3								m_vOrginScale;

	_vec3								m_vMovePos;
	_vec3								m_vMoveRot;
	_vec3								m_vMoveScale;
	_float								m_fDeadTime;
	_int								m_iAlpha;
	_float								m_fSpeed;
	_vec3								m_vDir;

	_float								m_fFrame;
	vector<Engine::EFFECT_UV_INFO>		m_vecTextureInfo;
protected:
	virtual void	Free(void);
};

