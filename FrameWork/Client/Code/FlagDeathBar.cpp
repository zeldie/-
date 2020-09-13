#include "stdafx.h"
#include "FlagDeathBar.h"
#include "ScreenTex.h"

CFlagDeathBar::CFlagDeathBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUIObject(pGraphicDev),
	m_eTeam(TEAM_END),
	m_eFlagDeathBarType(FLAGDEATHBARTYPE_END),
	m_dTime(0.f),
	m_fAlpha(1.f),
	m_iListIndex(0),
	m_fSpeed(0.f)
{
}


CFlagDeathBar::~CFlagDeathBar()
{
}

HRESULT CFlagDeathBar::Ready_GameObject(TEAM eType, FLAGDEATHBARTYPE eFlagType, _tchar* tKiller, _tchar* tDead, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eTeam = eType;
	m_eFlagDeathBarType = eFlagType;

	lstrcpy(m_szKillerName, tKiller);
	lstrcpy(m_szDeadName, tDead);

	return S_OK;
}

_int CFlagDeathBar::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{	
		return Engine::OBJ_DEAD;
	}

	CUIObject::Update_GameObject(dTimeDelta);

	m_dTime += dTimeDelta;
	
	if(3.f >= m_dTime)
		m_pScreenTexBufferCom->VertexPositionXYControl2(m_vRealPos);
	
	return Engine::NO_EVENT;
}

_int CFlagDeathBar::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (3.f <= m_dTime)
	{
		m_fAlpha -= (_float)dTimeDelta*0.5f;
		m_fSpeed += (_float)dTimeDelta*-200.f;
		m_pScreenTexBufferCom->VertexPositionXControl(m_fSpeed);

		if (0.f >= m_fAlpha)
		{
			m_fAlpha = 0.f;
			m_bIsDead = true;
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CFlagDeathBar::Render_Geometry(const _double & dTimeDelta)
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

		if(BG == m_eFlagDeathBarType)
			pEffect->BeginPass(25);
		else
			pEffect->BeginPass(5);
		
		m_pScreenTexBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);


		//////////font 출력
		if (BG == m_eFlagDeathBarType)
		{
			_vec2 vKillerFontPos = {};
			_vec2 vDeadFontPos = {};

			vKillerFontPos = _vec2(5.f + m_fSpeed, 248.5f + (m_iListIndex*24.8f));//19.8f
			vDeadFontPos = _vec2(85.f + m_fSpeed, 248.5f + (m_iListIndex*24.8f));


			//아이디를 출력해주야함
			if (TEAM::TEAM_BLUE == m_eTeam) //killer가 블루팀일때
			{
				Engine::Render_Font(L"Font_GODIC", m_szKillerName, &vKillerFontPos, D3DXCOLOR(0.11f, 0.79f, 0.9f, m_fAlpha));
				Engine::Render_Font(L"Font_GODIC", m_szDeadName, &vDeadFontPos, D3DXCOLOR(0.9f, 0.19f, 0.04f, m_fAlpha));
			}
			else if (TEAM::TEAM_RED == m_eTeam) // killer가 레드팀일때
			{
				Engine::Render_Font(L"Font_GODIC", m_szKillerName, &vKillerFontPos, D3DXCOLOR(0.9f, 0.19f, 0.04f, m_fAlpha));
				Engine::Render_Font(L"Font_GODIC", m_szDeadName, &vDeadFontPos, D3DXCOLOR(0.11f, 0.79f, 0.9f, m_fAlpha));
			}
		}
}

HRESULT CFlagDeathBar::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_FlagDeathBar"));
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

HRESULT CFlagDeathBar::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (TEAM_BLUE == m_eTeam)
	{
		if (BG == m_eFlagDeathBarType)
		{
			pEffect->SetVector("g_vRGBA", &_vec4(0.03f, 0.09f, 0.15f, m_fAlpha));
		}
		else if (ARROW == m_eFlagDeathBarType)
		{
			pEffect->SetVector("g_vRGBA", &_vec4(0.11f, 0.79f, 0.9f, m_fAlpha));
		}
		else if (EMOTICON == m_eFlagDeathBarType)
		{
			pEffect->SetVector("g_vRGBA", &_vec4(0.11f, 0.79f, 0.9f, m_fAlpha));
		}
	}
	else if (TEAM_RED == m_eTeam)
	{
		if (BG == m_eFlagDeathBarType)
		{
			pEffect->SetVector("g_vRGBA", &_vec4(0.56f, 0.14f, 0.14f, m_fAlpha));
		}
		else if (ARROW == m_eFlagDeathBarType)
		{
			pEffect->SetVector("g_vRGBA", &_vec4(0.9f, 0.19f, 0.04f, m_fAlpha));
		}
		else if (EMOTICON == m_eFlagDeathBarType)
		{
			pEffect->SetVector("g_vRGBA", &_vec4(0.9f, 0.19f, 0.04f, m_fAlpha));
		}
	}

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eFlagDeathBarType);
	return S_OK;
}

CFlagDeathBar * CFlagDeathBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEAM eType, FLAGDEATHBARTYPE eFlagType, _tchar* tKiller, _tchar* tDead, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CFlagDeathBar* pInstance = new CFlagDeathBar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, eFlagType, tKiller, tDead, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFlagDeathBar::Free()
{
	CUIObject::Free();
}
