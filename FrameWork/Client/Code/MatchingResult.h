#pragma once
#ifndef __MatchingResult_h__
#define __MatchingResult_h__

#include "UIObject.h"

class CMatchingResult : public CUIObject
{
public:
	enum RESULT{WIN, LOSE, DRAW, START, RESULT_END};

public:
	explicit CMatchingResult(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMatchingResult();

public:
	//Setter
	void Set_ResultType(RESULT eType) {	m_eResultType = eType; }

public:
	HRESULT						Ready_GameObject(RESULT eType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int				Update_GameObject(const _double& dTimeDelta) override;
	virtual _int				LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void				Render_Geometry(const _double& dTimeDelta) override;

private:
	RESULT						m_eResultType;
	_double						m_dTime;

private:
	HRESULT						Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT						Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CMatchingResult*		Create(LPDIRECT3DDEVICE9 pGraphicDev, RESULT eType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif