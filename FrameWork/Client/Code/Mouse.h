#pragma once

#include "NormalObject.h"
class CMouse : public CNormalObject
{
public:
	enum MOUSETYPE
	{
		MOUSE_PICKING, // 일반 마우스 상태, 고를때 보통 씀
		MOUSE_CROSSHAIR, // 전투시에 마우스(가운데에 고정)
		MOUSE_LOCK, // 전투시에 상대 캐릭터에 마우스 고정(일반적으로 상대가 내 사거리 또는 시야 내에 있으면 이걸 씀, 사거리 밖에 있으면 MOUSE_CROSSHAIR)
		MOUSE_INVISIBLE,
		MOUSE_ZOOM, //활 공격시에 줌땡겨지는 공격을 할때는 이게 뜸
		MOUSE_END
	};
private:
	explicit CMouse(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMouse();

public:
	void				Change_MouseType(MOUSETYPE eMouseType);

public:
	virtual HRESULT		Ready_GameObject(MOUSETYPE eMouseType);
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component();
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	MOUSETYPE			m_eMouseType;
	_uint				m_ePass;
	_float				m_fRotateAngle;
public:
	static CMouse*		Create(LPDIRECT3DDEVICE9 pGraphicDev, MOUSETYPE eMouseType);
	virtual void		Free();

};

