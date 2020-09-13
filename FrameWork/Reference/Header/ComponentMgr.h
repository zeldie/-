#pragma once

#include "Engine_Defines.h"
#include "Base.h"
#include "Component.h"
#include "TriCol.h"
#include "RcTex.h"
#include "CubeCol.h"
#include "CubeTex.h"
#include "TerrainTex.h"
#include "ScreenTex.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Texture.h"
#include "NaviMesh.h"
#include "ParticleTex.h"
#include "Shader.h"
#include "DynamicBuffer.h"
#include "NaviMeshRun.h"
BEGIN(Engine)

class ENGINE_DLL CComponentMgr : public CBase
{
	DECLARE_SINGLETON(CComponentMgr);
private:
	explicit CComponentMgr();
	virtual ~CComponentMgr();

public:
	HRESULT		Reserve_ContainerSize(const _ushort& wSize);

	HRESULT		Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev,
							const _ushort& wContainerIdx,
							const wstring& wstrBufferTag,
							BUFFERID eID,
							const _ulong& dwVtxCntX = 1,
							const _ulong& dwVtxCntZ = 1,
							const _ulong& dwVtxItv = 1,
							const _ulong& dwDetail = 1);

	HRESULT		Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ushort& wContainerIdx,
								const wstring& wstrTextureTag,
								TEXTURETYPE eType,
								const wstring& wstrPath,
								const _uint& iCnt = 1);

	HRESULT		Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ushort& wContainerIdx,
								wstring wstrMeshTag,
								MESHTYPE eType,
								wstring wstrFilePath,
								wstring wstrFileName);

	HRESULT		Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ushort& wContainerIdx,
								wstring wstrShaderTag,
								wstring wstrFileName);

	void		Render_Texture(const _ushort& wContainerIdx,
								const wstring& wstrTextureTag,
								const _uint& iIndex = 0);

	void		Render_Buffer(const _ushort& wContainerIdx,
								const wstring& wstrBufferTag);

	CComponent*	Clone(const _ushort& wContainerIdx,
							const wstring& wstrComponentTag);

	void		DeleteResource(const _ushort& wContainerIdx,
								const wstring& wstrComponentTag);

	void		ClearComponent(const _ushort& wContainerIdx);

	HRESULT		LoadMesh(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath);

	HRESULT		LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath);

	HRESULT		Add_Component(CComponent* pComponent, const wstring& wstrComponentTag, const _ushort& wContainerIdx);
private:
	CComponent*	Find_Component(const _ushort& wContainerIdx,
								const wstring& wstrComponentTag);

private:
	map<wstring, CComponent*>*					m_pmapComponent;
	_ushort										m_wContainerSize;

public:
	virtual void Free();
};

END