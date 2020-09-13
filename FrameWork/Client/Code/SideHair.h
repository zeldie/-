#pragma once

#include "DynamicMeshObject.h"
class CSideHair final : public CDynamicMeshObject
{
private:
	explicit CSideHair(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSideHair();

public:
	//Getter
	_uint			Get_SideHairIndex() { return m_iSideHairIndex; } //���� �߰�

public:
	HRESULT			Change_SideHair(_uint iSideHair, _uint iCurrentTrack, _double dTrackTime, _uint iAnimationNum, _double dAccTime);
	void			Change_HairColor(_vec4 vHairColor) { m_vHairColor = vHairColor; }
public:
	HRESULT			Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iSideHairNum);
	virtual	_int	Update_GameObject(const _double& dTimeDelta, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iAnimationNum);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_PostEffect(const _double& dTimeDelta) override;

private:
	HRESULT			Clone_Component(_uint iSideHairNum);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	HRESULT			Load_SideHair();


private:
	vector<Engine::CDynamicMesh*>	m_vecSideHair;
	_uint							m_iSideHairNum; //�޽� �߰��Ҷ����� ����
	_uint							m_iAnimationNum;
	_vec4							m_vHairColor;

	_uint							m_iSideHairIndex; //���� �߰� -> UI���� �������ΰ� ǥ���Ҷ� �ʿ��� ����.

public:
	static	CSideHair*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iSideHairNum);
	virtual void	Free();
};

