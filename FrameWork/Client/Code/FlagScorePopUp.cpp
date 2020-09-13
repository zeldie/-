#include "stdafx.h"
#include "FlagScorePopUp.h"
#include "RcTex.h"

CFlagScorePopUp::CFlagScorePopUp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNormalObject(pGraphicDev),
	m_ePopUpColor(POPUPCOLOR_END),
	m_eFlagScorePopUpType(FlagScorePopUpTYPE_END),
	m_fAlphaBG(0.f),
	m_fAlphaBATTLE(0.f),
	m_dTime(0.f),
	m_bTimeStart(false),
	m_bRenderTXT(false)
{
	D3DXMatrixIdentity(&m_matProj);
}

CFlagScorePopUp::~CFlagScorePopUp()
{
}

HRESULT CFlagScorePopUp::Ready_GameObject(FlagScorePopUpTYPE eType, POPUPCOLOR eColor, _float fX, _float fY)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;


	m_eFlagScorePopUpType = eType;
	m_ePopUpColor = eColor;

	m_pTransformCom->Set_Pos(fX - (WINCX >> 1), -fY + (WINCY >> 1), 0.f);

	if (BG == m_eFlagScorePopUpType)
		m_pTransformCom->Set_Scale(50.f, 28.f, 1.f);
	else if (BATTLE == m_eFlagScorePopUpType)
		m_pTransformCom->Set_Scale(28.f, 30.f, 1.f);

	return S_OK;
}

_int CFlagScorePopUp::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CNormalObject::Update_GameObject(dTimeDelta);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f); //->직교투영

	m_fAlphaBATTLE += _float(dTimeDelta);

	if (0.3f <= m_fAlphaBATTLE)
		m_bRenderTXT = true;

	if (1.f <= m_fAlphaBATTLE)
		m_fAlphaBATTLE = 1.f;
	
	m_fAlphaBG += _float(dTimeDelta);
	if (0.8f <= m_fAlphaBG)
		m_fAlphaBG = 0.8f;

	if (m_bTimeStart)
	{
		m_dTime += dTimeDelta;		
	}

	return Engine::NO_EVENT;
}

_int CFlagScorePopUp::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	//일정시간 잠깐 옆으로 움직이고
	if (!m_bTimeStart)
	{
		if (BLUE == m_ePopUpColor)
		{
			//m_pTransformCom->Move_Pos(&_vec3(1.f, 0.f, 0.f), -5.f, dTimeDelta);
			//m_fBlueFontX -= 5.f * dTimeDelta;
			if (1.f <= m_fAlphaBATTLE)
			{
				//m_pTransformCom->Set_Pos(&m_pTransformCom->m_vInfo[Engine::INFO_POS]);
				m_bTimeStart = true;
				m_fAlphaBATTLE = 1.f;
			}

		}
		else if (RED == m_ePopUpColor)
		{
			//m_pTransformCom->Move_Pos(&_vec3(1.f, 0.f, 0.f), 5.f, dTimeDelta);
			//m_fRedFontX += 5.f * dTimeDelta;
			if (1.f <= m_fAlphaBATTLE)
			{
				//m_pTransformCom->Set_Pos(&m_pTransformCom->m_vInfo[Engine::INFO_POS]);
				m_bTimeStart = true;
				m_fAlphaBATTLE = 1.f;
			}
		}
	}

	//멈춰서 3초정도
	if (2.f <= m_dTime)
	{
		m_bIsDead = true;
	}


	//if (BATTLE == m_eFlagScorePopUpType)
	//{
	//	if (RED == m_ePopUpColor)
	//	{
	//		//위치 조절 test
	//		_vec3 vPos;
	//		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	//		if (Engine::KeyDown(DIK_DOWN))
	//		{
	//			m_pTransformCom->Set_Pos(&_vec3(vPos.x + 1.f, vPos.y, vPos.z));

	//			_vec3 vPos2;
	//			m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos2);

	//			//cout << vPos2.x << '\t' << vPos2.y << endl;
	//		}
	//	}
	//}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CFlagScorePopUp::Render_Geometry(const _double & dTimeDelta)
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

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

	if (m_bRenderTXT)
	{
		if (BG == m_eFlagScorePopUpType)
		{
			if (BLUE == m_ePopUpColor)
				Engine::Render_Font(L"Font_GODIC2", L"30", &_vec2(500.f, 76.2f), D3DXCOLOR(0.06f, 0.83f, 1.f, 1.f));
			else if (RED == m_ePopUpColor)
				Engine::Render_Font(L"Font_GODIC2", L"30", &_vec2(747.f, 76.2f), D3DXCOLOR(0.98f, 0.23f, 0.31f, 1.f));
		}

	}

}

HRESULT CFlagScorePopUp::Clone_Component()
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
	pComponent = m_pBufferCom = Engine::CRcTex::Create(m_pGraphicDev);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_FlagScorePopUp"));
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

HRESULT CFlagScorePopUp::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	//CBaseObject::Set_ShaderMatrix(pEffect);
	_matrix		matWorld, matView;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixIdentity(&matView); //직교투영

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	if (BG == m_eFlagScorePopUpType)
		pEffect->SetVector("g_vRGBA", &_vec4(0.f, 0.f, 0.f, m_fAlphaBG));
	else if (BATTLE == m_eFlagScorePopUpType)
	{
		if (BLUE == m_ePopUpColor)
			pEffect->SetVector("g_vRGBA", &_vec4(0.06f, 0.83f, 1.f, m_fAlphaBATTLE));
		else if (RED == m_ePopUpColor)
			pEffect->SetVector("g_vRGBA", &_vec4(0.98f, 0.23f, 0.31f, m_fAlphaBATTLE));

	}
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eFlagScorePopUpType);

	return S_OK;
}

CFlagScorePopUp * CFlagScorePopUp::Create(LPDIRECT3DDEVICE9 pGraphicDev, FlagScorePopUpTYPE eType, POPUPCOLOR eColor, _float fX, _float fY)
{
	CFlagScorePopUp* pInstance = new CFlagScorePopUp(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, eColor, fX, fY)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFlagScorePopUp::Free()
{
	CNormalObject::Free();
}
