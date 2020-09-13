#include "stdafx.h"
#include "RunPlayerHp.h"
#include "ScreenTex.h"
#include "RunPlayerObserver.h"

CRunPlayerHP::CRunPlayerHP(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_pRunPlayerObserver(nullptr),
	m_iRunPlayerMaxHp(0),
	m_fResultValue(1.f),
	m_bLateInit(true)
{
}


CRunPlayerHP::~CRunPlayerHP()
{
}

HRESULT CRunPlayerHP::Ready_GameObject(BARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eBarType = eType;
	if (HP == m_eBarType)
	{
		//옵저버 신청
		m_pRunPlayerObserver = CRunPlayerObserver::Create();
		if (nullptr == m_pRunPlayerObserver)
			return E_FAIL;
		Engine::Subscribe(Engine::RUNPLAYER, m_pRunPlayerObserver);
	}

	return S_OK;
}

_int CRunPlayerHP::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	if (HP == m_eBarType)
	{
		if (m_bLateInit)
		{
			m_iRunPlayerMaxHp = m_pRunPlayerObserver->Get_MaxHp();
			m_bLateInit = false;
		}
	}

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CRunPlayerHP::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (HP == m_eBarType)
	{
		int iHp = 0;

		//옵저버에서 정보받아야함
		iHp = m_pRunPlayerObserver->Get_Hp();
		if (m_iRunPlayerMaxHp <= iHp)
			iHp = m_iRunPlayerMaxHp;

		m_fResultValue = (float)iHp / (float)m_iRunPlayerMaxHp;
		m_pScreenTexBufferCom->VertexXControl_LeftDir(m_fResultValue);
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CRunPlayerHP::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CRunPlayerHP::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_RunPlayerHP"));
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

HRESULT CRunPlayerHP::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture",m_eBarType);

	return S_OK;
}

CRunPlayerHP * CRunPlayerHP::Create(LPDIRECT3DDEVICE9 pGraphicDev, BARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CRunPlayerHP* pInstance = new CRunPlayerHP(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunPlayerHP::Free()
{
	CUIObject::Free();

	if (HP == m_eBarType)
	{
		Engine::UnSubscribe(Engine::RUNPLAYER, m_pRunPlayerObserver);
		Engine::Safe_Release(m_pRunPlayerObserver);
	}
}