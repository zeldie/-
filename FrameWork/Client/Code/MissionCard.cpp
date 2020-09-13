#include "stdafx.h"
#include "MissionCard.h"
#include "ScreenTex.h"
//#include "UIObject.h"

CMissionCard::CMissionCard(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eMissionCard(MISSIONCARD_END),
	m_bChoose(false),
	m_bSceneChange(false)
	//m_bCreateCartel(false)
{
}


CMissionCard::~CMissionCard()
{
}

HRESULT CMissionCard::Ready_GameObject(MISSIONCARD eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eMissionCard = eType;
	m_vSize = _vec2(1.2f,1.2f);

	return S_OK;
}

_int CMissionCard::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);

	_vec3 vBufferVtx = m_pScreenTexBufferCom->Get_vStartPos();
	_vec3 vBufferSize = m_pScreenTexBufferCom->Get_vSize();	
	_vec3 vSizeRatio = _vec3(vBufferSize.x * (m_vSize.x - 1.f) * 0.5f , vBufferSize.y * (m_vSize.y - 1.f) * 0.5f,0.f);

	_vec3 vMousePos;
	if (m_pScreenTexBufferCom->CheckAroundMouse(g_hWnd, &vMousePos, vSizeRatio))
	{
		if (RAID == m_eMissionCard || RAID_INV == m_eMissionCard)
		{
			if (!CUIMgr::GetInstance()->Get_MissionBossLock())
			{
				m_bChoose = true;
				m_pScreenTexBufferCom->VertexXYControl(m_vSize.x, m_vSize.y);
				CheckMouseClick();
			}

		}
		else if (CTF == m_eMissionCard || CTF_INV == m_eMissionCard)
		{
			if (!CUIMgr::GetInstance()->Get_MissionFlagLock())
			{
				m_bChoose = true;
				m_pScreenTexBufferCom->VertexXYControl(m_vSize.x, m_vSize.y);
				CheckMouseClick();
			}
		}
		else if (AI == m_eMissionCard || AI_INV == m_eMissionCard)
		{
			if (!CUIMgr::GetInstance()->Get_MissionApostleLock())
			{
				m_bChoose = true;
				m_pScreenTexBufferCom->VertexXYControl(m_vSize.x, m_vSize.y);
				CheckMouseClick();
			}
		}
	}
	else
	{
		m_bChoose = false;
		m_pScreenTexBufferCom->VertexOriginControl();
	}

	return Engine::NO_EVENT;
}

_int CMissionCard::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CMissionCard::Render_Geometry(const _double & dTimeDelta)
{
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(1);

	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

//_bool CMissionCard::CheckAroundMouse(HWND hWnd)
//{
//	//POINT	ptMouse{};
//
//	//GetCursorPos(&ptMouse);
//	//ScreenToClient(hWnd, &ptMouse);
//
//	//_vec2 vMousePos = _vec2((_float)ptMouse.x , (_float)ptMouse.y);
//	//_vec3 vBufferVtx = m_pScreenTexBufferCom->Get_vStartPos();
//	//_vec3 vBufferSize = m_pScreenTexBufferCom->Get_vSize();
//	//
//	//_vec2 vSizeRatio = _vec2(vBufferSize.x * (m_vSize.x - 1.f) * 0.5f , vBufferSize.y * (m_vSize.y - 1.f) * 0.5f);
//	//_vec2 vVertex[4];
//	//vVertex[0] = _vec2(vBufferVtx.x - vSizeRatio.x, vBufferVtx.y - vSizeRatio.y);
//	//vVertex[1] = _vec2(vBufferVtx.x + vBufferSize.x + vSizeRatio.x, vBufferVtx.y - vSizeRatio.y);
//	//vVertex[2] = _vec2(vBufferVtx.x + vBufferSize.x + vSizeRatio.x, vBufferVtx.y + vBufferSize.y + vSizeRatio.y);
//	//vVertex[3] = _vec2(vBufferVtx.x - vSizeRatio.x, vBufferVtx.y + vBufferSize.y + vSizeRatio.y);
//
//	//_vec2 vMouseDir[4] = {
//	//	vMousePos - vVertex[0],
//	//	vMousePos - vVertex[1],
//	//	vMousePos - vVertex[2],
//	//	vMousePos- vVertex[3]
//	//	};
//
//	//_vec2 vLine[4] = {
//	//	vVertex[1] - vVertex[0],
//	//	vVertex[2] - vVertex[1],
//	//	vVertex[3] - vVertex[2],
//	//	vVertex[0] - vVertex[3]
//	//	};
//	//
//	//_vec2 vNormal[4] = {};
//	//for (_uint i = 0; i < 4; ++i)
//	//{
//	//	vNormal[i] = _vec2(-vLine[i].y , vLine[i].x);
//
//	//	D3DXVec2Normalize(&vNormal[i], &vNormal[i]);
//	//	D3DXVec2Normalize(&vMouseDir[i],&vMouseDir[i]);
//	//}
//
//	//for (_uint i = 0; i < 4; ++i)
//	//{
//	//	if (0 > D3DXVec2Dot(&vNormal[i], &vMouseDir[i]))
//	//		return false;
//	//}
//
//	return true;
//}

void CMissionCard::CheckMouseClick()
{
	if (Engine::MouseUp(Engine::DIM_LB))
	{
		if (RAID == m_eMissionCard)
			CUIMgr::GetInstance()->CreateReadyUI(m_pGraphicDev);
		else if (CTF == m_eMissionCard)
		{
			m_bSceneChange = true;
		}
		else if (AI == m_eMissionCard)
		{
			m_bSceneChange = true;
		}
	}
}

HRESULT CMissionCard::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	//Buffer
	pComponent = m_pScreenTexBufferCom = Engine::CScreenTex::Create(m_pGraphicDev, fX, fY, fSizeX, fSizeY, fViewZ);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_MissionCard"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CMissionCard::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (!m_bChoose)
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eMissionCard);
	else
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eMissionCard + 1);

	return S_OK;
}

CMissionCard * CMissionCard::Create(LPDIRECT3DDEVICE9 pGraphicDev, MISSIONCARD eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CMissionCard* pInstance = new CMissionCard(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType,fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMissionCard::Free()
{
	CUIObject::Free();
}
