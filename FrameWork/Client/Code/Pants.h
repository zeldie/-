#pragma once

#include "DynamicMeshObject.h"
class CPants : public CDynamicMeshObject
{
private:
	explicit CPants(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPants();

public:
	//Getter
	_uint			Get_PantsIndex() { return m_iPantsIndex; } //희정추가

public:
	HRESULT			Change_Pants(_uint iPants, _uint iCurrentTrack, _double dTrackTime, _uint iAnimationNum);
public:
	HRESULT			Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iPantsNum);
	virtual	_int	Update_GameObject(const _double& dTimeDelta, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iAnimationNum);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_PostEffect(const _double& dTimeDelta) override;
private:
	HRESULT			Clone_Component(_uint iPantsNum);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	HRESULT			Load_Pants();

private:
	vector<Engine::CDynamicMesh*> m_vecPants;
	_uint						  m_iPantsNum; //메쉬 추가할때마다 갱신

	_uint						  m_iPantsIndex; //희정추가

public:
	static	CPants*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iPantsNum);
	virtual void	Free();
};

