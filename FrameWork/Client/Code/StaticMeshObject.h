#pragma once

#include "BaseObject.h"
#include "StaticMesh.h"


class CStaticMeshObject : public CBaseObject
{
protected:
	explicit CStaticMeshObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticMeshObject();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Depth(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;
	virtual void		Render_PostEffect(const _double& dTimeDelta) override;

protected:
	Engine::CStaticMesh*				m_pStaticMeshCom;
public:
	virtual void	Free() override;
};

