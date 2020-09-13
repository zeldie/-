#include "Export_Engine.h"

///////////////////////////////// GraphicDev ///////////////////////////////////////////////
// Get
void Get_CamView(_matrix * matView)
{
	CGraphicDev::GetInstance()->Get_CamView(matView);
}
void Get_CamProj(_matrix * matProj)
{
	CGraphicDev::GetInstance()->Get_CamProj(matProj);
}
// Set
// General
HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}
void			Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void			Render_End(void)
{
	CGraphicDev::GetInstance()->Render_End();
}

LPD3DXLINE Get_Line()
{
	return CGraphicDev::GetInstance()->Get_Line();
}

LPDIRECT3DDEVICE9 Get_Device()
{
	return CGraphicDev::GetInstance()->GetDevice();
}

////////////////////////////////// TimerMgr /////////////////////////////////////////////////////
// Get
_double		Get_TimeDelta(const wstring& wstrTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(wstrTimerTag);
}
// Set
void		Set_TimeDelta(const wstring& wstrTimerTag)
{
	CTimerMgr::GetInstance()->Set_TimeDelta(wstrTimerTag);
}
// General;
HRESULT	Ready_Timer(const wstring& wstrTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(wstrTimerTag);
}

////////////////////////////////////// FrameMgr///////////////////////////////////////////////////////////
// Get
_bool		IsPermit_Call(const wstring& wstrFrameTag, const _double& dTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(wstrFrameTag, dTimeDelta);
}
// Set
// General

HRESULT		Ready_Frame(const wstring& wstrFrameTag, const _double& dCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(wstrFrameTag, dCallLimit);
}

///////////////////////////////////////////// FontMgr ///////////////////////////////////////////////////
// Get
// Set
// General

HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
					const wstring& wstrFontTag,
					const wstring& pFontType,
					const _uint& iWidth,
					const _uint& iHeight,
					const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, wstrFontTag, pFontType, iWidth, iHeight, iWeight);
}

void	Render_Font(const wstring& wstrFontTag,
					const wstring& pString,
					const _vec2* pPos,
					D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(wstrFontTag, pString, pPos, Color);
}

// InputDev
// Get
_int	KeyPressing(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->KeyPressing(byKeyID);
}
_int	KeyDown(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->KeyDown(byKeyID);
}
_int	KeyUp(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->KeyUp(byKeyID);
}
_byte	MousePressing(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->MousePressing(eMouse);
}
_long	MouseMove(MOUSEMOVESTATE eMouseState)
{
	return CInputDev::GetInstance()->MouseMove(eMouseState);
}

_int	MouseDown(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->MouseDown(eMouse);
}
_int	MouseUp(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->MouseUp(eMouse);
}
// Set
// General
HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}
void	Set_InputDev(void)
{
	CInputDev::GetInstance()->Set_InputDev();
}


HRESULT Reserve_ContainerSize(const _ushort & wSize)
{
	return CComponentMgr::GetInstance()->Reserve_ContainerSize(wSize);
}

HRESULT Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, BUFFERID eID, const _ulong & dwVtxCntX, const _ulong & dwVtxCntZ, const _ulong & dwVtxItv, const _ulong & dwDetail)
{
	return CComponentMgr::GetInstance()->Ready_Buffers(pGraphicDev, wContainerIdx, wstrBufferTag, eID, dwVtxCntX, dwVtxCntZ, dwVtxItv, dwDetail);
}

HRESULT Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrTextureTag, TEXTURETYPE eType, const wstring & wstrPath, const _uint & iCnt)
{
	return CComponentMgr::GetInstance()->Ready_Texture(pGraphicDev, wContainerIdx, wstrTextureTag, eType, wstrPath, iCnt);
}

HRESULT Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, wstring wstrMeshTag, MESHTYPE eType, wstring wstrFilePath, wstring wstrFileName)
{
	return CComponentMgr::GetInstance()->Ready_Meshes(pGraphicDev, wContainerIdx, wstrMeshTag, eType, wstrFilePath, wstrFileName);
}

HRESULT Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, wstring wstrShaderTag, wstring wstrFileName)
{
	return CComponentMgr::GetInstance()->Ready_Shader(pGraphicDev, wContainerIdx, wstrShaderTag, wstrFileName);
}

void Render_Texture(const _ushort & wContainerIdx, const wstring & wstrTextureTag, const _uint & iIndex)
{
	return CComponentMgr::GetInstance()->Render_Texture(wContainerIdx, wstrTextureTag, iIndex);
}

void Render_Buffer(const _ushort & wContainerIdx, const wstring & wstrBufferTag)
{
	return CComponentMgr::GetInstance()->Render_Buffer(wContainerIdx, wstrBufferTag);
}

CComponent * Clone(const _ushort & wContainerIdx, const wstring & wstrComponentTag)
{
	return CComponentMgr::GetInstance()->Clone(wContainerIdx, wstrComponentTag);
}

void DeleteResource(const _ushort & wContainerIdx, const wstring & wstrComponentTag)
{
	CComponentMgr::GetInstance()->DeleteResource(wContainerIdx, wstrComponentTag);
}

void ClearComponent(const _ushort & wContainerIdx)
{
	CComponentMgr::GetInstance()->ClearComponent(wContainerIdx);
}

HRESULT LoadMesh(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath)
{
	return CComponentMgr::GetInstance()->LoadMesh(pGraphicDev, wstrPath);
}

HRESULT LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath)
{
	return CComponentMgr::GetInstance()->LoadTexture(pGraphicDev, wstrPath);
}

HRESULT Add_Component(CComponent * pComponent, const wstring& wstrComponentTag, const _ushort & wContainerIdx)
{
	return CComponentMgr::GetInstance()->Add_Component(pComponent, wstrComponentTag, wContainerIdx);
}

CComponent * Get_Component(LAYERTYPE eLayerType, const wstring & wstrObjTag, COMPONENTTYPE eComponentType, COMPONENTID eID, _int iIndex)
{
	return CManagement::GetInstance()->Get_Component(eLayerType, wstrObjTag, eComponentType, eID, iIndex);
}

inline list<CGameObject*> Get_GameObjectlist(LAYERTYPE eLayerType, const wstring & wstrObjTag)
{
	return CManagement::GetInstance()->Get_GameObjectlist(eLayerType, wstrObjTag);
}

inline CGameObject * Get_GameObject(LAYERTYPE eLayerType, const wstring & wstrObjTag, _int iIndex)
{
	return CManagement::GetInstance()->Get_GameObject(eLayerType, wstrObjTag, iIndex);
}

HRESULT SetUp_Scene(CScene * pScene)
{
	return CManagement::GetInstance()->SetUp_Scene(pScene);
}

HRESULT Add_GameObject(LAYERTYPE eLayerType, const wstring & wstrObjTag, CGameObject * pInstance)
{
	return CManagement::GetInstance()->Add_GameObject(eLayerType, wstrObjTag, pInstance);
}

HRESULT Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement ** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Ready_TextureMgr(pGraphicDev)))
		return E_FAIL;

	if (FAILED(pManagement->Ready_Shaders(pGraphicDev)))
		return E_FAIL;

	if (FAILED(pManagement->Ready_Renderer(pGraphicDev)))
		return E_FAIL;

	*ppManagement = pManagement;
	return S_OK;
}

HRESULT AddSubject(SUBJECTTYPE eSubjectType)
{
	return CManagement::GetInstance()->AddSubject(eSubjectType);
}

void Subscribe(SUBJECTTYPE eSubjectType, CObserver * pObserver)
{
	CManagement::GetInstance()->Subscribe(eSubjectType, pObserver);
}

void UnSubscribe(SUBJECTTYPE eSubjectType, CObserver * pObserver)
{
	CManagement::GetInstance()->UnSubscribe(eSubjectType, pObserver);
}

void Notify(SUBJECTTYPE eSubjectType, int iMessage, void * pData)
{
	CManagement::GetInstance()->Notify(eSubjectType, iMessage, pData);
}

void ClearSubject()
{
	CManagement::GetInstance()->ClearSubject();
}

void ClearRenderer()
{
	CManagement::GetInstance()->ClearRenderer();
}

HRESULT Ready_MRT(RENDERTARGETLIST eMRTlistTag, RENDERTARGET eTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Ready_MRT(eMRTlistTag, eTargetTag);
}

HRESULT Begin_MRT(RENDERTARGETLIST eMRTlistTag)
{
	return CRenderTargetMgr::GetInstance()->Begin_MRT(eMRTlistTag);
}

HRESULT End_MRT(RENDERTARGETLIST eMRTlistTag)
{
	return CRenderTargetMgr::GetInstance()->End_MRT(eMRTlistTag);
}

HRESULT Begin_RT(RENDERTARGET eTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Begin_RT(eTargetTag);
}

HRESULT End_RT(RENDERTARGET eTargetTag)
{
	return CRenderTargetMgr::GetInstance()->End_RT(eTargetTag);
}

HRESULT Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, RENDERTARGET eTargetTag, const _uint & iWidth, const _uint & iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	return CRenderTargetMgr::GetInstance()->Ready_RenderTarget(pGraphicDev, eTargetTag, iWidth, iHeight, Format, Color);
}

HRESULT Ready_RenderTargetBuffer(RENDERTARGET eTargetTag, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	return CRenderTargetMgr::GetInstance()->Ready_RenderTargetBuffer(eTargetTag, fX, fY, fSizeX, fSizeY);
}

void Render_RenderTarget(RENDERTARGETLIST eMRTlistTag)
{
	CRenderTargetMgr::GetInstance()->Render_RenderTarget(eMRTlistTag);
}

void Render_RenderTarget(RENDERTARGET eTargetTag)
{
	CRenderTargetMgr::GetInstance()->Render_RenderTarget(eTargetTag);
}

void SetUp_OnShader(LPD3DXEFFECT & pEffect, RENDERTARGET eTargetTag, const char * pConstantName)
{
	CRenderTargetMgr::GetInstance()->SetUp_OnShader(pEffect, eTargetTag, pConstantName);
}


const D3DLIGHT9 * Get_Light(const _uint & iIndex)
{
	return CLightMgr::GetInstance()->Get_Light(iIndex);
}

HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9 * pLightInfo, const _uint & iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

void Render_Light(LPD3DXEFFECT & pEffect)
{
	CLightMgr::GetInstance()->Render_Light(pEffect);
}

void Delete_Light(_uint iIndex)
{
	CLightMgr::GetInstance()->Delete_Light(iIndex);
}

inline void SetTexture(LPD3DXEFFECT & pEffect, const char * pContantName, const _uint & iIndex)
{
	CTextureMgr::GetInstance()->SetTexture(pEffect, pContantName, iIndex);
}

// Release Engine
void			Release_Engine(void)
{
	//Utility
	CTextureMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
	CComponentMgr::GetInstance()->DestroyInstance();
	CRenderTargetMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	//System
	CInputDev::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}