#pragma once
#ifndef __CPuzzleObj_h__
#define __CPuzzleObj_h__

#include "BaseObject.h"

#include "BaseObject.h"
#include "ScreenTex.h"
#include "Texture.h"

class CPuzzleObj : public CBaseObject
{
public:
	explicit CPuzzleObj(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPuzzleObj();

public:
	void		Set_Index(_uint iIndex) { m_iIndex = iIndex; }
	void		Set_TextureNum(_uint iTextureNum) { m_iTextureNum = iTextureNum; }

	_uint		Get_Index() { return m_iIndex; }
	_uint		Get_TextureNum() { return m_iTextureNum; }
	Engine::CScreenTex*		Get_ScreenTex() { return m_pScreenTexBufferCom; }
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
	_bool					m_bChoose;		//지금 마우스가 올라가있는지
	_uint					m_iIndex;	// 매니저에서 해당 클래스의 인덱스
	_uint					m_iTextureNum; // 텍스쳐 번호

public:
	static CPuzzleObj*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ, wstring wstrKey);
	virtual void		Free();
};

#endif // !__CPuzzleObj_h__
