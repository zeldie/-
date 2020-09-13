#pragma once
#ifndef __CPuzzlePreview_h__
#define __CPuzzlePreview_h__

#include "BaseObject.h"

#include "BaseObject.h"
#include "ScreenTex.h"
#include "Texture.h"

class CPuzzlePreview : public CBaseObject
{
public:
	explicit CPuzzlePreview(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPuzzlePreview();

public:
	HRESULT				Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ, wstring wstrKey);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ, wstring wstrKey);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	Engine::CScreenTex*		m_pScreenTexBufferCom;
	Engine::CTexture*		m_pTextureCom;

public:
	static CPuzzlePreview*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ, wstring wstrKey);
	virtual void		Free();
};

#endif // !__CPuzzleObj_h__
