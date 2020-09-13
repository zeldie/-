#ifndef Terrain_h__
#define Terrain_h__


#include "ToolGameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTexture;
END
class CTerrain : public CToolGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain(void);

public:
	virtual HRESULT	Ready_GameObject() override;
	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual _int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT			Clone_Component();
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
private:
	Engine::CStaticMesh*	m_pStaticMeshCom;
	Engine::CTexture*		m_pTextureCom;

	_float					m_fMipMap; //»Ò¡§

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free();


};


#endif // Terrain_h__
