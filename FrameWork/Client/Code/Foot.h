#pragma once
#include "DynamicMeshObject.h"
class CFoot : public CDynamicMeshObject
{
private:
	explicit CFoot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFoot();

public:
	//Getter
	_uint			Get_FootIndex() { return m_iFootIndex; } //�����߰�

public:
	HRESULT			Change_Foot(_uint iFoot, _uint iCurrentTrack, _double dTrackTime, _uint iAnimationNum);
public:
	HRESULT			Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iFootNum);
	virtual	_int	Update_GameObject(const _double& dTimeDelta, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iAnimationNum);
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual	void	Render_Geometry(const _double& dTimeDelta) override;
	virtual void	Render_PostEffect(const _double& dTimeDelta) override;

private:
	HRESULT			Clone_Component(_uint iFootNum);
	HRESULT			Setup_ShaderProps(LPD3DXEFFECT& pEffect);
	HRESULT			Load_Foot();

private:
	vector<Engine::CDynamicMesh*> m_vecFoot;
	_uint						  m_iFootNum; //�޽� �߰��Ҷ����� ����

	_uint						  m_iFootIndex; //�����߰�

public:
	static	CFoot*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iFootNum);
	virtual void	Free();
};

