#pragma once

#include "NormalObject.h"
class CMouse : public CNormalObject
{
public:
	enum MOUSETYPE
	{
		MOUSE_PICKING, // �Ϲ� ���콺 ����, ���� ���� ��
		MOUSE_CROSSHAIR, // �����ÿ� ���콺(����� ����)
		MOUSE_LOCK, // �����ÿ� ��� ĳ���Ϳ� ���콺 ����(�Ϲ������� ��밡 �� ��Ÿ� �Ǵ� �þ� ���� ������ �̰� ��, ��Ÿ� �ۿ� ������ MOUSE_CROSSHAIR)
		MOUSE_INVISIBLE,
		MOUSE_ZOOM, //Ȱ ���ݽÿ� �ܶ������� ������ �Ҷ��� �̰� ��
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

