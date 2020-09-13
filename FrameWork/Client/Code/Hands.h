#pragma once
#include "DynamicMeshObject.h"
class CHands : public CDynamicMeshObject
{
private:
	explicit CHands(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHands();

public:
	//Getter
	_uint			Get_HandsIndex() { return m_iHandsIndex; } //�����߰�

public:
	HRESULT			Change_Hands(_uint iHands, _uint iCurrentTrack, _double dTrackTime, _uint iAnimationNum);
public:
	HRESULT			Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iHandsNum);
	virtual	_int	Update_GameObject(const _double& dTimeDelta, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iAnimationNum);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_PostEffect(const _double& dTimeDelta) override;

private:
	HRESULT			Clone_Component(_uint iHandsNum);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	HRESULT			Load_Hands();

private:
	vector<Engine::CDynamicMesh*> m_vecHands;
	_uint						  m_iHandsNum; //�޽� �߰��Ҷ����� ����

	_uint						  m_iHandsIndex; //�����߰�

public:
	static	CHands*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iHandsNum);
	virtual void	Free();
};

