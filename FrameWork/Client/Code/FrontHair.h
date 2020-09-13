#pragma once

#include "DynamicMeshObject.h"
class CFrontHair final : public CDynamicMeshObject
{
private:
	explicit CFrontHair(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFrontHair();

public:
	//Getter
	_uint			Get_FrontHairIndex() { return m_iFrontHairIndex; } //희정 추가

public:
	HRESULT			Change_FrontHair(_uint iFrontHair, _uint iCurrentTrack, _double dTrackTime, _uint iAnimationNum, _double dAccTime);
	void			Change_HairColor(_vec4 vHairColor) { m_vHairColor = vHairColor; }
public:
	HRESULT			Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iFronHairNum);
	virtual	_int	Update_GameObject(const _double& dTimeDelta, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iAnimationNum);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_PostEffect(const _double& dTimeDelta) override;
private:
	HRESULT			Clone_Component(_uint iFrontHairNum);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	HRESULT			Load_FrontHair();


private:
	vector<Engine::CDynamicMesh*>	m_vecFrontHair;
	_uint							m_iFrontHairNum; //메쉬 추가할때마다 갱신
	_uint							m_iAnimationNum;
	_vec4							m_vHairColor;

	_uint							m_iFrontHairIndex; //희정 추가 -> UI에서 착용중인거 표시할때 필요한 변수.

public:
	static	CFrontHair*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iFrontHairNum);
	virtual void	Free();
};

