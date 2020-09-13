#ifndef RenderTarget_h__
#define RenderTarget_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CScreenTex;
class ENGINE_DLL CRenderTarget : public CBase
{
private:
	explicit CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRenderTarget();
	
public:
	void			SetUp_OnGraphicDev(const _uint& iIndex);
	void			SetUp_OnShader(LPD3DXEFFECT& pEffect, const char* pConstantName);
	void			Clear_RenderTarget();
	void			Release_OnGraphicDev(const _uint& iIndex);

	//�Ʒ� �ΰ��� �� ��Ʈ
	//Ÿ�� �ؽ��Ŀ� ���۸� ���� �غ��Ѵ�.(�ʿ伺�� ����)
	HRESULT			Ready_RenderTarget(const _uint& iWidth, 
										const _uint& iHeight,
										D3DFORMAT Format,
										D3DXCOLOR Color);


	HRESULT			Ready_RenderTargetBuffer(const _float& fX,
											const _float& fY,
											const _float& fSizeX,
											const _float& fSizeY);

	//����Ÿ�� �׷���
	void			Render_RenderTarget();
private:
	LPDIRECT3DDEVICE9			m_pGraphicDev; 
	LPDIRECT3DTEXTURE9			m_pTargetTexture;
	LPDIRECT3DSURFACE9			m_pTargetSurface;
	LPDIRECT3DSURFACE9			m_pOldTargetSurface;
	D3DXCOLOR					m_ClearColor;
	CScreenTex*					m_pScreenBuffer;
public:
	static CRenderTarget*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										const _uint& iWidth, 
										const _uint& iHeight,
										D3DFORMAT Format,
										D3DXCOLOR Color);

	virtual void Free();

};
END
#endif // RenderTarget_h__
