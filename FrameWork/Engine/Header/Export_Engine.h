#ifndef Export_Engine_h__
#define Export_Engine_h__

#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
#include "InputDev.h"
#include "ComponentMgr.h"
#include "Management.h"
#include "Transform.h"
#include "Renderer.h"
#include "LightMgr.h"
#include "Shader.h"
#include "RenderTargetMgr.h"
#include "TextureMgr.h"
BEGIN(Engine)

///////////////////////////////////////////// GraphicDev /////////////////////////////////////////////
// Get
inline void			Get_CamView(_matrix* matView);
inline void			Get_CamProj(_matrix* matProj);
// Set
// General;
inline HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev);
inline void			Render_Begin(D3DXCOLOR Color);
inline void			Render_End(void);
inline LPD3DXLINE	Get_Line();
inline LPDIRECT3DDEVICE9 Get_Device();

///////////////////////////////////////////// TimerMgr /////////////////////////////////////////////
// Get
inline _double		Get_TimeDelta(const wstring& wstrTimerTag);
// Set
inline void			Set_TimeDelta(const wstring& wstrTimerTag);
// General;
inline HRESULT		Ready_Timer(const wstring& wstrTimerTag);

///////////////////////////////////////////// FrameMgr /////////////////////////////////////////////
// Get
inline _bool		IsPermit_Call(const wstring& wstrFrameTag, const _double& dTimeDelta);
// Set
// General
inline 	HRESULT		Ready_Frame(const wstring& wstrFrameTag, const _double& dCallLimit);

///////////////////////////////////////////// FontMgr /////////////////////////////////////////////
// Get
// Set
// General
 inline HRESULT		Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
 							const wstring& wstrFontTag,
 							const wstring& pFontType,
 							const _uint& iWidth,
 							const _uint& iHeight,
 							const _uint& iWeight);

 inline void		Render_Font(const wstring& wstrFontTag,
 							const wstring& pString,
 							const _vec2* pPos,
 							D3DXCOLOR Color);

 /////////////////////////////////////// InputDev /////////////////////////////////////////
//Get
 inline _int		KeyPressing(_ubyte byKeyID);
 inline _int		KeyDown(_ubyte byKeyID);
 inline _int		KeyUp(_ubyte byKeyID);
 inline _byte		MousePressing(MOUSEKEYSTATE eMouse);
 inline _long		MouseMove(MOUSEMOVESTATE eMouseState);
 inline _int		MouseDown(MOUSEKEYSTATE eMouse);
 inline _int		MouseUp(MOUSEKEYSTATE eMouse);

 // Set
 // General
 inline HRESULT		Ready_InputDev(HINSTANCE hInst, HWND hWnd);
 inline void		Set_InputDev(void);

 /////////////////////////////////////////////  ComponentMgr /////////////////////////////////////////////

 //Get


 //Set

 //General
inline HRESULT		Reserve_ContainerSize(const _ushort& wSize);
inline HRESULT		Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev,
									 const _ushort& wContainerIdx,
									 const wstring& wstrBufferTag,
									 BUFFERID eID,
									 const _ulong& dwVtxCntX = 1,
									 const _ulong& dwVtxCntZ = 1,
									 const _ulong& dwVtxItv = 1,
									 const _ulong& dwDetail = 1);
inline HRESULT		Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
									const _ushort& wContainerIdx,
									const wstring& wstrTextureTag,
									TEXTURETYPE eType,
									const wstring& wstrPath,
									const _uint& iCnt = 1);

inline HRESULT		Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev,
									const _ushort& wContainerIdx,
									wstring wstrMeshTag,
									MESHTYPE eType,
									wstring wstrFilePath,
									wstring wstrFileName);

inline HRESULT		Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev,
									const _ushort& wContainerIdx,
									wstring wstrShaderTag,
									wstring wstrFileName);

inline void			Render_Texture(const _ushort& wContainerIdx,
									 const wstring& wstrTextureTag,
									 const _uint& iIndex = 0);

inline void			Render_Buffer(const _ushort& wContainerIdx,
									const wstring& wstrBufferTag);

inline CComponent*	Clone(const _ushort& wContainerIdx,
							const wstring& wstrComponentTag);

inline void			DeleteResource(const _ushort& wContainerIdx,
									const wstring& wstrComponentTag);

inline void			ClearComponent(const _ushort& wContainerIdx);

inline HRESULT		LoadMesh(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath);

inline HRESULT		LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath);

inline HRESULT		Add_Component(CComponent * pComponent, const wstring& wstrComponentTag, const _ushort & wContainerIdx);

////////////////////////////////////////////////Management //////////////////////////////////////////////////
//Get
inline CComponent*			Get_Component(LAYERTYPE eLayerType, const wstring& wstrObjTag, COMPONENTTYPE eComponentType, COMPONENTID eID, _int iIndex = 0);
inline list<CGameObject*>	Get_GameObjectlist(LAYERTYPE eLayerType, const wstring& wstrObjTag);
inline CGameObject*			Get_GameObject(LAYERTYPE eLayerType, const wstring& wstrObjTag, _int iIndex = 0);
//Set
inline HRESULT				SetUp_Scene(CScene* pScene);

//General
inline HRESULT				Add_GameObject(LAYERTYPE eLayerType, const wstring& wstrObjTag, CGameObject* pInstance);
inline HRESULT				Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement);
inline HRESULT				AddSubject(SUBJECTTYPE eSubjectType);
inline void					Subscribe(SUBJECTTYPE eSubjectType, CObserver* pObserver);
inline void					UnSubscribe(SUBJECTTYPE eSubjectType, CObserver* pObserver);
inline void					Notify(SUBJECTTYPE eSubjectType, int iMessage, void* pData);
inline void					ClearSubject();
inline void					ClearRenderer();

/////////////////////////////////////RenderTargetMgr/////////////////////////////////////////////////
inline HRESULT			Ready_MRT(RENDERTARGETLIST eMRTlistTag, RENDERTARGET eTargetTag);
inline HRESULT			Begin_MRT(RENDERTARGETLIST eMRTlistTag);
inline HRESULT			End_MRT(RENDERTARGETLIST eMRTlistTag);
inline HRESULT			Begin_RT(RENDERTARGET eTargetTag);
inline HRESULT			End_RT(RENDERTARGET eTargetTag);
inline HRESULT			Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev,
											RENDERTARGET eTargetTag,
											const _uint& iWidth,
											const _uint& iHeight,
											D3DFORMAT Format,
											D3DXCOLOR Color);
inline HRESULT			Ready_RenderTargetBuffer(RENDERTARGET eTargetTag,
												const _float& fX,
												const _float& fY,
												const _float& fSizeX,
												const _float& fSizeY);
inline void				Render_RenderTarget(RENDERTARGETLIST eMRTlistTag);
inline void				Render_RenderTarget(RENDERTARGET eTargetTag);
inline void				SetUp_OnShader(LPD3DXEFFECT& pEffect, RENDERTARGET eTargetTag, const char* pConstantName);


///////////////////////////////////////////////LightMgr//////////////////////////////////////////////////////////
inline const D3DLIGHT9*		Get_Light(const _uint& iIndex);
inline HRESULT				Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
inline void					Render_Light(LPD3DXEFFECT& pEffect);

///////////////////////////////////////////////TextureMgr//////////////////////////////////////////////////////////
inline void					SetTexture(LPD3DXEFFECT& pEffect, const char* pContantName, const _uint& iIndex);
inline void Release_Engine();

#include "Export_Engine.inl"

END


#endif // Export_System_h__
