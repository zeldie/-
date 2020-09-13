#pragma once

#include "ToolGameObject.h"
#include "RcTex.h"
#include "Texture.h"

class CToolEffectTextureObject : public CToolGameObject
{
private:
	explicit CToolEffectTextureObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolEffectTextureObject();

private:
	// CToolGameObject을(를) 통해 상속됨
	virtual HRESULT		Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component(wstring wstrObjectKey);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	Engine::CRcTex*				m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;
	_int						m_iTextureNum;

public:
	static CToolEffectTextureObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _int iTextureNum);
	virtual void	Free();
};

