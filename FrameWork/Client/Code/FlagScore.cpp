#include "stdafx.h"
#include "FlagScore.h"
#include "ScreenTex.h"

CFlagScore::CFlagScore(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CUIObject(pGraphicDev),
		m_pUIMgr(CUIMgr::GetInstance()),
		m_eFlagType(FLAGTYPE_END),
		m_eFlagColor(FLAGCOLOR_END),
		m_fResultValue(1.f),
		m_iGoal(0),
		m_wstrGoal(L""),
		m_iBlueScore(0),
		m_wstrBlueScore(L""),
		m_iRedScore(0),
		m_wstrRedScore(L""),
		m_iDigitCnt(1),
		m_bOne(true)
{
	//fx = 0.f;
	//fy= 0.f;
}


CFlagScore::~CFlagScore()
{
}

void CFlagScore::SetScore(FLAGCOLOR eColorType, _uint iScore)
{
	//점수 누적
	if (BLUE == eColorType)
		m_iBlueScore += iScore;
	else if(RED == eColorType)
		m_iRedScore += iScore;

}

HRESULT CFlagScore::Ready_GameObject(FLAGTYPE eType, FLAGCOLOR eColorType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eFlagType = eType;
	m_eFlagColor = eColorType;

	m_iGoal = 500;
	m_wstrGoal = to_wstring(m_iGoal);
	return S_OK;
}

_int CFlagScore::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);

	if (RECHARGE == m_eFlagType)
	{
		m_iBlueScore = m_pUIMgr->Get_BlueScore();
		if (m_bOne)
		{
			if (0 < m_iBlueScore)
			{
				m_pUIMgr->SetStartFlagFlash();
				m_bOne = false;
			}
		}

		if (m_iGoal <= m_iBlueScore)
			m_iBlueScore = m_iGoal;

		m_iRedScore = m_pUIMgr->Get_RedScore();
		if (m_iGoal <= m_iRedScore)
			m_iRedScore = m_iGoal;

	}

	return Engine::NO_EVENT;
}

_int CFlagScore::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (RECHARGE == m_eFlagType)
	{
		if (BLUE == m_eFlagColor)
		{
			if( 3 != m_iDigitCnt) //3이 아닐때만 자릿수 계산 ㄱㄱ
				CalculateDigitCnt(m_iBlueScore);

			m_fResultValue = 1.f - (_float(m_iBlueScore) / _float(m_iGoal));
			m_wstrBlueScore = to_wstring(m_iBlueScore);
		}
		else if (RED == m_eFlagColor)
		{
			if (3 != m_iDigitCnt) //3이 아닐때만 자릿수 계산 ㄱㄱ
				CalculateDigitCnt(m_iRedScore);

			m_fResultValue = 1.f - (_float(m_iRedScore) / _float(m_iGoal));
			m_wstrRedScore = to_wstring(m_iRedScore);
		}

		if (0.f >= m_fResultValue)
		{
			m_fResultValue = 0.f;
			m_pUIMgr->Set_CartelFinish(true);
		}

		m_pScreenTexBufferCom->VertexYControl_UpDir(m_fResultValue);
	}
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CFlagScore::Render_Geometry(const _double & dTimeDelta)
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

	pEffect->BeginPass(5);

	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

	//if (Engine::KeyDown(DIK_RIGHT))		
	//	fx += 1.f;
	//if (Engine::KeyDown(DIK_LEFT))
	//	fx -= 1.f;
	//if (Engine::KeyDown(DIK_UP))		
	//	fy += 1.f;
	//if (Engine::KeyDown(DIK_DOWN))
	//	fy -= 1.f;

	if (RECHARGE == m_eFlagType)
	{
		Engine::Render_Font(L"Font_GODIC2", m_wstrGoal, &_vec2(628.f, 78.4f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		if (1 == m_iDigitCnt)
		{
			Engine::Render_Font(L"Font_GODIC2", m_wstrBlueScore, &_vec2(574.5f, 78.4f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC2", m_wstrRedScore, &_vec2(700.4f, 78.4f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
		else if (2 == m_iDigitCnt)
		{
			Engine::Render_Font(L"Font_GODIC2", m_wstrBlueScore, &_vec2(568.5f, 78.4f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC2", m_wstrRedScore, &_vec2(695.4f, 78.4f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
		else if (3 == m_iDigitCnt)
		{
			Engine::Render_Font(L"Font_GODIC2", m_wstrBlueScore, &_vec2(563.5f , 78.4f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC2", m_wstrRedScore, &_vec2(691.4f, 78.4f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}

		if (3 != m_iDigitCnt)
			m_iDigitCnt = 1;
	}
}

void CFlagScore::CalculateDigitCnt(_uint iScore)
{
	int iQuotient = iScore / 10;
	if (0 != iQuotient)
	{
		CalculateDigitCnt(iQuotient);
		++m_iDigitCnt;
	}
}

HRESULT CFlagScore::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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
	pComponent = m_pScreenTexBufferCom = Engine::CScreenTex::Create(m_pGraphicDev,fX, fY, fSizeX, fSizeY, fViewZ);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_FlagScore"));
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

HRESULT CFlagScore::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);	

	if (BACKGROUND == m_eFlagType)
	{
		pEffect->SetVector("g_vRGBA", &_vec4(0.f, 0.f, 0.f, 0.6f));
	}
	else if (OUTLINE == m_eFlagType)
	{
		if (BLUE == m_eFlagColor)
			pEffect->SetVector("g_vRGBA",&_vec4(0.06f, 0.83f, 1.f,1.f));
		else if(RED == m_eFlagColor)
			pEffect->SetVector("g_vRGBA", &_vec4(0.98f, 0.23f, 0.31f, 1.f)); 
	}
	else if (RECHARGE == m_eFlagType)
	{
		if (BLUE == m_eFlagColor)
			pEffect->SetVector("g_vRGBA", &_vec4(0.28f, 0.96f, 0.96f, 0.8f)); 
		else if (RED == m_eFlagColor)
			pEffect->SetVector("g_vRGBA", &_vec4(0.99f, 0.35f, 0.34f, 0.8f)); 

	}
	else if (SCOREBACK == m_eFlagType)
	{
		pEffect->SetVector("g_vRGBA", &_vec4(0.f, 0.f, 0.f, 0.6f));
	}


	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture",m_eFlagType); 
	return S_OK;
}

CFlagScore * CFlagScore::Create(LPDIRECT3DDEVICE9 pGraphicDev, FLAGTYPE eType, FLAGCOLOR eColorType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CFlagScore* pInstance = new CFlagScore(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, eColorType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFlagScore::Free()
{
	CUIObject::Free();
}
