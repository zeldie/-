#pragma once

#include "Component.h"

BEGIN(Engine)

class CGameObject;
class CTerrainTex;
class CTransform;
class ENGINE_DLL CCalculator : public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator();

public:
	void		TranslateLocalSpace(HWND hWnd, const _matrix& WorldMatrix);
	void		TranslateWorldSpace(HWND hWnd);
	void		TranslateWorldSpaceDecal(HWND hWnd);

	_float		Compute_HeightOnTerrain(const _vec3* pPos,
										const _vec3* pTerrainVtxPos,
										const _ulong& dwCntX,
										const _ulong& dwCntZ,
										const _ulong& dwItv);

	_bool		PickNaviMesh(HWND hWnd, CGameObject* pNaviMeshObject, LPD3DXBASEMESH Mesh, _vec3 * vecOutPos);
	_bool		PickMesh(HWND hWnd, CGameObject* pMeshObject, _vec3* vecOutPos, _float* pOldDist);
	_bool		PickStaticMesh(HWND hWnd, CGameObject* pMeshObject, _vec3* vecOutPos, _float* pOldDist);
	_bool		PickDynamicMesh(HWND hWnd, CGameObject* pDynamicMesh, LPD3DXBASEMESH Mesh, _vec3 * vecOutPos, _float* pOldDist);
	_bool		PickDecalPos(HWND hWnd, CGameObject* pMeshObject, _vec3* vecOutPos);
private:
	D3DXVECTOR3						m_vRayPos;
	D3DXVECTOR3						m_vRayDir;
	CGameObject*					m_pPickTarget;

public:
	static CCalculator*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	CComponent*		Clone() override;
	virtual void			Free() override;
};

END