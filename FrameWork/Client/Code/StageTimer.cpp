#include "stdafx.h"
#include "StageTimer.h"
#include "ScreenTex.h"

CStageTimer::CStageTimer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUIObject(pGraphicDev),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_eStageTimerType(STAGETIMERTYPE_END),
	m_dStageTime(0.f),
	m_dMaxStageTime(0.f),
	m_wstrStageTime(L""),
	m_iSecondUnits(0),
	m_iSecondTens(0),
	m_iMinuteUnits(0),
	m_iMinuteTens(0),
	m_bLateInit(true),
	m_bCountDownStart(false)
{
}


CStageTimer::~CStageTimer()
{
}

HRESULT CStageTimer::Ready_GameObject(STAGETIMERTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eStageTimerType = eType;

	return S_OK;
}

_int CStageTimer::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{
		return Engine::OBJ_DEAD;
	}

	if (m_bLateInit)
	{
		if (CARTEL == m_eStageTimerType)
			m_dMaxStageTime = 300.f; //8분 

		m_bLateInit = false;
	}

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CStageTimer::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	SetTime();

	if (BOSS == m_eStageTimerType || SHOOTING == m_eStageTimerType)
		CUIMgr::GetInstance()->SetAccumulatedTime(m_dStageTime);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CStageTimer::Render_Geometry(const _double & dTimeDelta)
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

	if (MATCHING != m_eStageTimerType)
	{
		if (!m_bCountDownStart)
			Engine::Render_Font(L"Font_GODO", m_wstrStageTime, &_vec2(606.f, 10.7f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		else
		{
			if (0 != m_iSecondTens) //십의자리가 0이 아닐때 -> 2자릿수
				Engine::Render_Font(L"Font_GODIC3", m_wstrStageTime, &_vec2(620.5f, 2.7f), D3DXCOLOR(0.83f, 0.15f, 0.015f, 1.f));
			else //십의자리가 0일때 -> 1자릿수
				Engine::Render_Font(L"Font_GODIC3", m_wstrStageTime, &_vec2(627.f, 2.7f), D3DXCOLOR(0.83f, 0.15f, 0.015f, 1.f));
		}
	}
}

void CStageTimer::SetTime()
{
	if (BOSS == m_eStageTimerType || SHOOTING ==m_eStageTimerType)
	{
		m_dStageTime = m_pUIMgr->Get_StageTime();
	}
	else if (CARTEL == m_eStageTimerType)
	{
		m_dStageTime = m_dMaxStageTime - m_pUIMgr->Get_StageTime();
	}

	if (CARTEL == m_eStageTimerType)
	{
		if (30.9f >= m_dStageTime)
		{
			m_bCountDownStart = true;
			m_pUIMgr->ChangeRenderState(L"FlagNoFuncObj", true);
		}

		if (0.f >= m_dStageTime)
		{
			m_dStageTime = 0.f;
			m_pUIMgr->Set_CartelFinish(true);
			//m_bIsDead = true;
		}
	}

	_uint iQuotient = (_ulong)m_dStageTime / 60;
	_uint iRemainder = (_ulong)m_dStageTime % 60;

	m_iMinuteTens = iQuotient / 10;
	m_iMinuteUnits = iQuotient % 10;

	m_iSecondTens = iRemainder / 10;
	m_iSecondUnits = iRemainder % 10;

	if (!m_bCountDownStart)
	{
		m_wstrStageTime = to_wstring(m_iMinuteTens) + to_wstring(m_iMinuteUnits) + L":" + to_wstring(m_iSecondTens) + to_wstring(m_iSecondUnits);
	}
	else
	{
		if (0 != m_iSecondTens) //십의자리가 0이 아닐때 -> 2자릿수
			m_wstrStageTime = to_wstring(m_iSecondTens) + to_wstring(m_iSecondUnits);
		else //십의자리가 0일때 -> 1자릿수
			m_wstrStageTime = to_wstring(m_iSecondUnits);
	}

}

HRESULT CStageTimer::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_StageTimer"));
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

HRESULT CStageTimer::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");
	return S_OK;
}

CStageTimer * CStageTimer::Create(LPDIRECT3DDEVICE9 pGraphicDev, STAGETIMERTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CStageTimer* pInstance = new CStageTimer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStageTimer::Free()
{
	CUIObject::Free();
}
