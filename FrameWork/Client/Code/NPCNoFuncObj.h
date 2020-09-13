#pragma once
#ifndef __NPCNoFuncObj_h__
#define __NPCNoFuncObj_h__

#include "NormalObject.h"
#include "QuestMgr.h" //?

class CNPCNoFuncObj : public CNormalObject
{
public:
	enum NPCNoFuncObjTYPE{EXCLAMATIONMARK, VKEY_TXT, SPEECHBUBBLE, QUESTIONMARK, NPCNoFuncObjTYPE_END};

public:
	explicit CNPCNoFuncObj(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNPCNoFuncObj();

public:
	//Getter
	NPCNoFuncObjTYPE&		Get_Type() { return m_eNPCNoFuncObjType; }
	CQuestMgr::NPCTYPE&		Get_NPCType() { return m_eNPCType; }

	//Setter
	void					Set_Render(_bool bBool = true) { m_bRender = bBool; }

public:
	HRESULT				Ready_GameObject(NPCNoFuncObjTYPE eType, CQuestMgr::NPCTYPE eNPCType, _bool bRender, _vec3 vPos, _vec3 vSize, Engine::CTransform* pTargetTransform);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;


private:
	NPCNoFuncObjTYPE			m_eNPCNoFuncObjType;
	Engine::CTransform*			m_pTargetTransformCom;
	_matrix						m_matProj;
	_bool						m_bRender;
	_vec3						m_vScale;
	CQuestMgr::NPCTYPE			m_eNPCType;
	////자리 이동 변수
	//_float fx;
	//_float fy;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	static CNPCNoFuncObj*	Create(LPDIRECT3DDEVICE9 pGraphicDev, NPCNoFuncObjTYPE eType, CQuestMgr::NPCTYPE eNPCType, _bool bRender, _vec3 vPos=_vec3(0.f,0.f,0.f), _vec3 vSize=_vec3(1.f,1.f,1.f), Engine::CTransform* pTargetTransform=nullptr);
	virtual void			Free();
};

#endif