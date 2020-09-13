#ifndef Frustum_h__
#define Frustum_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum : public CBase
{
private:
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFrustum();

public:
	LPDIRECT3DDEVICE9		Get_GraphicDev() { return m_pGraphicDev; }

public:
	HRESULT					Ready_Frustum();
	_bool					IsIn_Frustum(const _vec3* pPos, 
										 const _float fRadius = 0.f);

	_bool					IsIn_Frustum_ForObject(const _vec3* pPosWorld, 
													const _float& fRadius);

	_bool					IsIn_Rectangle(_vec3* pMinimun, _vec3* pMaximum);
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_vec3					m_vPoint[8];
	D3DXPLANE				m_Plane[6];

public:
	static	CFrustum*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free();
};

END
#endif // Frustum_h__
