#pragma once
#ifndef __Skill_h__
#define __Skill_h__

#include "UIObject.h"
class CSkill : public CUIObject
{
public:
	enum SKILLTYPE{ SKILL_BASIC, SKILL_DS, SKILL_TS,SKILL_ORB, SKILL_LB, SKILLTYPE_END}; //수정예정

public:
	explicit CSkill(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkill();

public:
	SKILLTYPE&			Get_SkillType() { return m_eSkillType; }

public:
	HRESULT				Ready_GameObject(const wstring& wstrTexture, SKILLTYPE eType, Engine::KEYGUIDE eKeyType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	SKILLTYPE			m_eSkillType;
	Engine::KEYGUIDE	m_eKeyType;
	wstring				m_wstrTextureName;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CSkill*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrTexture, SKILLTYPE eType, Engine::KEYGUIDE eKeyType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif