#pragma once

#include "DynamicMeshObject.h"
class CTop : public CDynamicMeshObject
{
private:
	explicit CTop(LPDIRECT3DDEVICE9 pGarphicDev);
	virtual ~CTop();

public:
	//Getter
	_uint			Get_TopIndex() { return m_iTopIndex; } //희정추가

public:
	HRESULT			Change_Top(_uint iTop, _uint iCurrentTrack, _double dTrackTime, _uint iAnimationNum, _double dAccTime);
public:
	HRESULT			Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iTopNum);
	virtual	_int	Update_GameObject(const _double& dTimeDelta, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iAnimationNum);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_PostEffect(const _double& dTimeDelta) override;
private:
	HRESULT			Clone_Component(_uint iTopNum);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	HRESULT			Load_Top();


private:
	vector<Engine::CDynamicMesh*>	m_vecTop;
	_uint							m_iTopNum; //메쉬 추가할때마다 갱신
	_uint							m_iAnimationNum;

	_uint							m_iTopIndex; //희정추가

public:
	static	CTop*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iTopNum);
	virtual void	Free();
};

