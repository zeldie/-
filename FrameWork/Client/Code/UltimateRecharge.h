#pragma once
#ifndef __UltimateRecharge_h__
#define __UltimateRecharge_h__

#include "BaseObject.h"
#include "UltimateBuffer.h"
#include "Texture.h"

class CPlayerObserver;
class CUltimateRecharge : public CBaseObject
{
public:
	explicit CUltimateRecharge(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUltimateRecharge();

public:
	//Setter
	void				Set_Full(_bool bBool);
	void				Set_TextureRage(_uint iRage) { m_iTextureRage = iRage; }
	void				Set_Create() { m_bCreate = true; }

	//Getter
	_bool				Get_Full() { return m_bFull; }
	_bool				Get_Create() { return m_bCreate; }

public:
	HRESULT				Ready_GameObject(_float fX, _float fY);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	//옵저버
	CPlayerObserver*			m_pPlayerObserver;

	Engine::CUltimateBuffer*	m_pUltimateBuffer;
	Engine::CTexture*			m_pTextureCom;
	_matrix						m_matProj; //직교투영행렬

	_double						m_dbMaxRage;
	_bool						m_bLateInit;
	_uint						m_iTextureRage;
	_bool						m_bFull;
	_bool						m_bCreate;

	CUIMgr*						m_pUIMgr;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CUltimateRecharge*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY);
	virtual void				Free();
};

#endif