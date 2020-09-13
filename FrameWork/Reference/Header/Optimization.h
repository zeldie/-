#ifndef Optimization_h__
#define Optimization_h__

#include "Component.h"
#include "Frustum.h"

BEGIN(Engine)

class ENGINE_DLL COptimization : public CComponent
{
private:
	explicit	COptimization(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	COptimization(const COptimization& rhs);
	virtual		~COptimization();

public:
	HRESULT					Ready_Optimization(USEAGE eType, const _ulong& dwCntX, const _ulong& dwCntZ);
	_bool					IsIn_Frustum_ForObject(const _vec3* pPosWorld, const _float& fRadius);

private:
	CFrustum*			m_pFrustum;

public:
	static COptimization*		Create(LPDIRECT3DDEVICE9 pGraphicDev, USEAGE eType, const _ulong& dwCntX, const _ulong& dwCntZ);
	virtual CComponent*			Clone();
	virtual void				Free();

};


END
#endif // Optimization_h__
