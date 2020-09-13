#pragma once

#include "BaseObject.h"
#include "OrbTrail.h"
class COrbWing : public CBaseObject
{
private:
	explicit COrbWing(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COrbWing();

public:
	virtual HRESULT Ready_GameObject(_vec3* pPos, _vec3* pAngle, Engine::CTransform* pCore);
	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;
private:
	HRESULT			Clone_Component();

private:
	COrbTrail*			m_pOrbWingTrail;
	_double				m_dAccTime;
	Engine::CTransform*	m_pCoreTransformCom;

	Engine::CRcTex*		m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;
public:
	static COrbWing* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, Engine::CTransform* pCore);
	virtual void Free() override;
};

