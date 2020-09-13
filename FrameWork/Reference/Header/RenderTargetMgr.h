#ifndef RenderTargetMgr_h__
#define RenderTargetMgr_h__

#include "RenderTarget.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTargetMgr : public CBase
{
	DECLARE_SINGLETON(CRenderTargetMgr)

private:
	explicit	CRenderTargetMgr();
	virtual		~CRenderTargetMgr();

public:
	
	HRESULT			Ready_MRT(RENDERTARGETLIST eMRTlistTag, RENDERTARGET eTargetTag);


	// 해당 MRT 그린다.
	HRESULT			Begin_MRT(RENDERTARGETLIST eMRTlistTag);
	HRESULT			End_MRT(RENDERTARGETLIST eMRTlistTag);

	//MRT가 아닐경우에 그린다
	HRESULT			Begin_RT(RENDERTARGET eTargetTag);
	HRESULT			End_RT(RENDERTARGET eTargetTag);

	//RenerTarget의 함수들을 부름
	HRESULT			Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev,
										RENDERTARGET eTargetTag,
										const _uint& iWidth,
										const _uint& iHeight,
										D3DFORMAT Format,
										D3DXCOLOR Color);


	HRESULT			Ready_RenderTargetBuffer(RENDERTARGET eTargetTag,
											const _float& fX, 
											const _float& fY,
											const _float& fSizeX,
											const _float& fSizeY);

	void			Render_RenderTarget(RENDERTARGETLIST eMRTlistTag);
	void			Render_RenderTarget(RENDERTARGET eTargetTag);
	void			SetUp_OnShader(LPD3DXEFFECT& pEffect, RENDERTARGET eTargetTag, const char* pConstantName);
		
private:
	CRenderTarget*			Find_RenderTarget(RENDERTARGET eTargetTag);
	list<CRenderTarget*>*	Find_MRT(RENDERTARGETLIST eMRTlistTag);

private:
	map<RENDERTARGET, CRenderTarget*>		m_mapRenderTarget;
	map<RENDERTARGETLIST, list<CRenderTarget*>>	m_mapMRT;


private:
	virtual void Free();
};

END
#endif // RenderTargetMgr_h__
