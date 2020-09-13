#ifndef __CPARTICLETEX_H__
#define __CPARTICLETEX_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CParticleTex : public CVIBuffer
{
private:
	explicit CParticleTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticleTex(const CParticleTex& rhs);
	virtual ~CParticleTex();

public:
	virtual HRESULT		Ready_Buffer();
	void				Update_Buffer(list<PARTICLE_INFO> listParticleInfo);
	void				Render_InstanceBuffer(list<PARTICLE_INFO> listParticleInfo, _float fSize);

private:
	LPDIRECT3DVERTEXBUFFER9			m_pVBTransform;
	LPDIRECT3DVERTEXDECLARATION9	m_pDeclaration;

public:
	static CParticleTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

	virtual CComponent*		Clone();
	virtual void			Free();
};

END
#endif // TerrainTex_h__
