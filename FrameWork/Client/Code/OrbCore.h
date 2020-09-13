#pragma once

#include "BaseObject.h"
#include "RcTex.h"
#include "Texture.h"
#include "OrbTrail.h"
#include "OrbWing.h"
class COrbCore : public CBaseObject
{
public:
	enum ORBTYPE
	{
		ORB_NORMAL,
		ORB_SPIN_FRONT_RIGHT,
		ORB_SPIN_FRONT_LEFT,
		ORB_SPIN_UP_RIGHT,
		ORB_SPIN_UP_LEFT,
		ORB_BIG,
		ORB_END
	};
private:
	explicit COrbCore(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COrbCore();

public:
	virtual HRESULT Ready_GameObject(ORBTYPE eOrbType, _vec3* pPos, _vec3* pAngle, _vec3* pDir, _double dCountDown, CONTROLTYPE eControlType);
	virtual _int	Update_GameObject(const _double& dTimeDelta) override;
	virtual	_int	LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void	Render_Geometry(const _double& dTimeDelta) override;
private:
	HRESULT			Clone_Component();
	void			Move(const _double& dTimeDelta);
private:
	ORBTYPE				m_eOrbType;
	_vec3				m_vDir;
	COrbTrail*			m_pOrbCenterTrail;
	_double				m_dAccTime;
	COrbWing*			m_pOrbWing;
	_matrix				m_matParent; // 회전할떄 공전할 중심
	_double				m_dCreateEffectTime;
	_double				m_dCountDown;
	CONTROLTYPE			m_eControlType;
public:
	static COrbCore* Create(LPDIRECT3DDEVICE9 pGraphicDev, ORBTYPE eOrbType, _vec3* pPos, _vec3* pAngle, _vec3* pDir, _double dCountDown, CONTROLTYPE eControlType);
	virtual void Free() override;
};

