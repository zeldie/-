#ifndef Terrain_h__
#define Terrain_h__


#include "StaticMeshObject.h"
#include "NaviMesh.h"

class CTerrain : public CStaticMeshObject
{
public:
	enum TERRAIN_TYPE
	{
		TERRAIN_LOBBY,
		TERRAIN_CARTEL,
		TERRAIN_BOSSSTAGE,
		TERRAIN_SHOOTINGSTAGE,
		TERRAIN_END
	};
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain();

public:
	HRESULT			Ready_GameObject(TERRAIN_TYPE eTerrainType);
	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual _int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Depth(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT			Clone_Component(TERRAIN_TYPE eTerrainType);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
private:
	Engine::CNaviMesh*		m_pNaviMeshCom;
	_float					m_fMipMap; //»Ò¡§
	TERRAIN_TYPE			m_eTerrainType;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TERRAIN_TYPE eTerrainType);
	virtual void			Free();


};


#endif // Terrain_h__
