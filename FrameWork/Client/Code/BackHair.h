#pragma once

#include "DynamicMeshObject.h"
class CBackHair final : public CDynamicMeshObject
{
private:
	explicit CBackHair(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackHair();

public:
	//Getter
	_uint			Get_BackHairIndex() { return m_iBackHairIndex; } //���� �߰�

public:
	HRESULT			Change_BackHair(_uint iBackHair, _uint iCurrentTrack, _double dTrackTime, _uint iAnimationNum, _double dAccTime);
	void			Change_HairColor(_vec4 vHairColor) { m_vHairColor = vHairColor; }
public:
	HRESULT			Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iBackHairNum);
	virtual	_int	Update_GameObject(const _double& dTimeDelta, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iAnimationNum);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_PostEffect(const _double& dTimeDelta) override;
private:
	HRESULT			Clone_Component(_uint iBackHairNum);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	HRESULT			Load_BackHair();


private:
	vector<Engine::CDynamicMesh*>	m_vecBackHair;
	_uint							m_iBackHairNum; //�޽� �߰��Ҷ����� ����
	_uint							m_iAnimationNum;
	_vec4							m_vHairColor;

	_uint							m_iBackHairIndex; //���� �߰� -> UI���� �������ΰ� ǥ���Ҷ� �ʿ��� ����.

public:
	static	CBackHair*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iBackHairNum);
	virtual void	Free();
};

