#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject();

public:
	CComponent*			Get_Component(COMPONENTTYPE eComponentType, COMPONENTID eID);
	_float				Get_ViewZ() { return m_fViewZ; }

	CComponent*			Find_Component(COMPONENTTYPE eComponentType, COMPONENTID eID);
	void				EraseComponent(COMPONENTTYPE eComponentType, COMPONENTID eID);
	void				Compute_ViewZ(const _vec3* pPos);
public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta);
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta);
	virtual void		Render_Depth(const _double& dTimeDelta);
	virtual void		Render_Geometry(const _double& dTimeDelta);
	virtual void		Render_PostEffect(const _double& dTimeDelta);
	virtual void		Render_Shadow(const _double& dTimeDelta);
protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<COMPONENTTYPE, CComponent*>		m_mapComponent[ID_END];
	_float								m_fViewZ;
public:
	virtual void	Free();

};

END
#endif // GameObject_h__
