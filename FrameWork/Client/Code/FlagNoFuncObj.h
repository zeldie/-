#pragma once
#ifndef __FlagNoFuncObj_h__
#define __FlagNoFuncObj_h__

#include "UIObject.h"
class CFlagNoFuncObj : public CUIObject
{
public:
	enum FLAGNOFUNCOBJ{ FLAGNOFUNCOBJ_GOALTXT, FLAGNOFUNCOBJ_BG, FLAGNOFUNCOBJ_ENDTXT, NONE , 
		FLAGNOFUNCOBJ_GETFLAGTXT, FLAGNOFUNCOBJ_FLAG, FLAGNOFUNCOBJ_READYFLAGTXT, 
		FLAGNOFUNCOBJ_RECREATEFLAGTXT, 
		FLAGNOFUNCOBJ_SPACEFLAG, /*깃발획득할 수 있다는 표시나타내는 문구 -> m_brenderui 를 트루펄스 바꿔주야함 깃발 가까워지면*/
		FLAGNOFUNCOBJ_END};

public:
	explicit CFlagNoFuncObj(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlagNoFuncObj();

public:
	//Getter
	FLAGNOFUNCOBJ&			Get_NoFuncObjType() { return m_eFlagNoFuncObjType; }

public:
	HRESULT				Ready_GameObject(FLAGNOFUNCOBJ eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	FLAGNOFUNCOBJ			m_eFlagNoFuncObjType;
	_float					m_fPosY;
	_double					m_dGetFlagTxtTime; //GETFLAGTXT->얘 살아있는 시간 정해줄라고

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CFlagNoFuncObj*		Create(LPDIRECT3DDEVICE9 pGraphicDev, FLAGNOFUNCOBJ eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void				Free();
};

#endif