#ifndef Renderer_h__
#define Renderer_h__

#include "Component.h"
#include "GameObject.h"
#include "Texture.h"
BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRenderer();

public:
	void	Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void	Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev, const _double& dTimeDelta);
	void	Clear_RenderGroup();
	void	Set_DrawRenderTarget();

public:
	HRESULT Ready_Prototype();
	HRESULT Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev);
	HRESULT	Resize_Buffer(_float fSizeX, _float fSizeY);

private:
	void	Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev, const _double& dTimeDelta);
	void	Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev, const _double& dTimeDelta);
	void	Render_NonAlpha_Cartoon(LPDIRECT3DDEVICE9& pGraphicDev, const _double& dTimeDelta);
	void	Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev, const _double& dTimeDelta);
	void	Render_UI(LPDIRECT3DDEVICE9& pGraphicDev, const _double& dTimeDelta);

	void	Render_Shadow(LPDIRECT3DDEVICE9& pGraphicDev, const _double& dTimeDelta);
	void	Render_Depth(LPDIRECT3DDEVICE9& pGraphicDev, const _double& dTimeDelta);
	void	Render_Geometry(LPDIRECT3DDEVICE9& pGraphicDev, const _double& dTimeDelta);
	void	Render_Cartoon(LPDIRECT3DDEVICE9& pGraphicDev, const _double& dTimeDelta); //위랑해서 2번 그림
	void	Render_Edge(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_Occlusion(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_HDR_Alpha(LPDIRECT3DDEVICE9& pGraphicDev, const _double dTimeDelta);
	void	Render_LightAcc(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_HDRBase(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_Luminance(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_HDR(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_LightShaft(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_RadialBlur(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_Final(LPDIRECT3DDEVICE9& pGraphicDev);

	void	Render_DOF(LPDIRECT3DDEVICE9& pGraphicDev);

	void	Render_Fade(LPDIRECT3DDEVICE9& pGraphicDev, const _double& dTimeDelta);
	//가우시안
	_float	Compute_Gaussian(_float fX, _float fMean, _float std_Deviation);

public:
	void	Setting_Sun(_vec3 vPos);
	_bool	Get_StartFade() { return m_bStartFade; }
	_bool	Get_EndFade() { return m_bEndFade; }
	_int	Get_NonAlpha() { return m_iRenderNonAlpha; }
	_int	Get_Alpha() { return m_iRenderAlpha; }
	_int	Get_UI() { return m_iRenderUI; }
	void	Set_StartFade(_bool bStartFade) { m_bStartFade = bStartFade; }
	void	Set_EndFade(_bool bEndFade) { m_bEndFade = bEndFade; }
	void	Set_CamFar(_float fFar) { m_fCamFar = fFar; }
	void	Change_Gamma(_float fGamma) { m_fGamma += fGamma; }

	//아래는 다른 클래스들이 쓰게끔
public:
	void	Set_Monochrome(_bool bFilterMonochrome) { m_bFilterMonoChrome = bFilterMonochrome; }
	void	Set_RadialBlur(_bool bRadialBlur) { m_bUseRadialBlur = bRadialBlur; }
private:
	list<CGameObject*>			m_RenderGroup[RENDER_END];
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	_bool						m_bDrawRenderTarget;

	//가우시안
	_float						m_fBrightThreshold;
	_float						m_fGaussMultiplier;
	_float						m_fGaussMean;
	_float						m_fGaussStdDev;
	_float                      m_fExposure;

	//For Occulusion
	_float						m_fRadius;

	//For MonoChrome
	_bool						m_bFilterMonoChrome;

	//LightShaft
	_vec2						m_vLightPosition;

	//화면 전환
	_bool						m_bStartFade;
	_bool						m_bEndFade;
	_vec4						m_vFadeColor;


	_int						m_iRenderNonAlpha;
	_int						m_iRenderAlpha;
	_int						m_iRenderUI;

	_float						m_fCamFar;

	//방사블러 쓸지말지
	_bool						m_bUseRadialBlur;

	LPDIRECT3DSURFACE9			m_pOriginal_DS_Surface;
	LPDIRECT3DSURFACE9			m_pShadow_DS_Surface;
	_float						m_fGamma;
public:
	static		CRenderer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		CComponent*		Clone();
	virtual		void			Free();
};

END
#endif // Renderer_h__
