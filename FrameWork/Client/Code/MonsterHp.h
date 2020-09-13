#pragma once
#ifndef __MonsterHp_h__
#define __MonsterHp_h__

#include "UIObject.h"

class CMonsterHp : public CUIObject
{
public:
	enum MONSTERHPTYPE{ FIRSTHP, SECONDHP, MONSTERHPTYPE_END};

public:
	explicit CMonsterHp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterHp();

public:
	//Getter
	_bool				Get_Working() { return  m_bWorking; }
	_bool				Get_Swap() { return m_bSwap; }
	_float				Get_MaxHpPerLine() { return m_fMaxHpPerLine; }

	//Setter
	void				Set_Working(_bool bBool) { m_bWorking = bBool; }
	void				Set_MonsterDamage(_float fDamage) { m_fDamage = fDamage; }
	void				Set_HpIsZero(bool bBool = true) { m_bHpIsZero = bBool; }
	void				Set_Swap(bool bBool = true) { m_bSwap = bBool; }
	void				Set_VertexZ();
	void				Set_ChageColor(bool bBool = true) { m_bChangeColor = bBool; };

public:
	HRESULT				Ready_GameObject(MONSTERHPTYPE eType, _bool bBool, _float MaxHpPerLine, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	MONSTERHPTYPE			m_eMonsterHpType;

	//int						m_iMonsterMaxHp;
	_float					m_fMaxHpPerLine; //한 줄 당 최고치
	_float					m_fDamage; //깍인양 (데미지)
	_float					m_fRemainHp; //줄 당 현재 깍이기 가능한 피 
	_float					m_fGiveNextHp;

	_float					m_fResultValue;

	_bool					m_bLateInit;
	_bool					m_bWorking; //피가 깍이고 있는 상태를 구분하기 위한 변수
	_bool					m_bSwap;
	_bool					m_bHpIsZero;
	_bool					m_bCheck; //다음에 넘겨줄 양을 한번만 계산하기 위해 있는 변수
	_bool					m_bChangeColor;

	_float m_fR;
	_float m_fG;
	_float m_fB;
	

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CMonsterHp*	Create(LPDIRECT3DDEVICE9 pGraphicDev, MONSTERHPTYPE eType, _bool bBool, _float MaxHpPerLine, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif