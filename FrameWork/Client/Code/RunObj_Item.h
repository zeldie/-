#pragma once
#include "BaseObject.h"
class CRunObj_Item : public CBaseObject
{
private:
	explicit CRunObj_Item(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRunObj_Item();

public:
	virtual HRESULT		Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _vec3* vSelfRotate, RUN_LINE_SECT eSect,
		LINETYPE eLineType, RUNGAMEOBJECT_COLL eCollType, _double dProgress, _double dRadius, _float fUpPos, RUNGAME_ITEMTYPE eItemType);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

public:
	RUN_LINE_SECT			Get_LineSect() { return m_eSect; }
	LINETYPE				Get_LineType() { return m_eLineType; }
	RUNGAMEOBJECT_COLL		Get_CollType() { return m_eObjCollType; }
	_double					Get_Progress() { return m_dProgress; }
	_double					Get_Radius() { return m_dRadius; }
	RUNGAME_ITEMTYPE		Get_ItemType() { return m_eItemType; }
	void					Set_Collision(RUNPLAYER_POWER eColl);

private:
	HRESULT				Clone_Component(wstring wstrObjectKey);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	//_matrix					m_matProj;
	_vec3					m_vScale;

	RUN_LINE_SECT			m_eSect;	// ����
	LINETYPE				m_eLineType;	// ���� ��ġ
	RUNGAMEOBJECT_COLL		m_eObjCollType;	// �浹 Ÿ��
	_double					m_dProgress;	// ���൵
	_double					m_dRadius;		// ���൵ ������ (+- �浹����)
	RUNGAME_ITEMTYPE		m_eItemType;	// ������ ����

public:
	static CRunObj_Item*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _vec3* vSelfRotate, RUN_LINE_SECT eSect,
		LINETYPE eLineType, RUNGAMEOBJECT_COLL eCollType, _double dProgress, _double dRadius, _float fUpPos, RUNGAME_ITEMTYPE eItemType);
	virtual void			Free();


};

