#pragma once

#include "BaseObject.h"
BEGIN(Engine)
class CDynamicBuffer;
class CTexture;
END

class COrbTrail : public CBaseObject
{
public:
	enum TRAIL_TYPE
	{
		TRAIL_CENTER,
		TRAIL_WING,
		TRAIL_END
	};
private:
	explicit COrbTrail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COrbTrail();

public:
	HRESULT			Ready_GameObject(TRAIL_TYPE eTrailType, Engine::CTransform* pParentTransformCom);
	_int			Update_GameObject(const _double& dTimeDelta, _vec3 pUpPos, _vec3 pDownPos);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component(TRAIL_TYPE eTrailType);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	Engine::CDynamicBuffer*		m_pDynamicBufferCom;
	Engine::CTexture*			m_pTextureCom;
	list<_vec3>					m_listPos[2];
	_double						m_AddTrailTime;
	TRAIL_TYPE					m_eTrailType;
	_uint						m_iPass;
public:
	static COrbTrail*			Create(LPDIRECT3DDEVICE9 pGraphicDev, TRAIL_TYPE eTrailType, Engine::CTransform* pParentTransformCom);
	virtual void				Free();
};

