#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr();
	virtual ~CLightMgr();

public:
	const D3DLIGHT9*		Get_Light(const _uint& iIndex);

	HRESULT					Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	void					Render_Light(LPD3DXEFFECT& pEffect);
private:
	list<CLight*>			m_LightList;

private:
	virtual void Free();

};

END
#endif // LightMgr_h__
