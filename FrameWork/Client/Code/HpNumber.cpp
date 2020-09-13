#include "stdafx.h"
#include "HpNumber.h"
#include "ScreenTex.h"

CHpNumber::CHpNumber(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CUIObject(pGraphicDev),
		m_iNumber(0),
		m_iLineCnt(0),
		m_iDigitCnt(1)
{
}


CHpNumber::~CHpNumber()
{
}


HRESULT CHpNumber::Ready_GameObject(DIGITTYPE eType, _uint iLineCnt, _uint iNumber/*, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ*/)
{
	m_eDigitType = eType;

	_float fX, fY, fSizeX, fSizeY, fViewZ = 0;
	switch (m_eDigitType)
	{
	case CHpNumber::UNITS:
		fX = 793.5f;
		break;
	case CHpNumber::TENS:
		fX = 778.5f;
		break;
	case CHpNumber::HUNDREDS:
		fX = 763.5f;
		break;
	case CHpNumber::THOUSANDS:
		fX = 748.5f;
		break;
	}

	fY = 87.9f;
	fSizeX = 20.f;
	fSizeY = 20.f;
	fViewZ = 0.f;


	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;
	
	m_iLineCnt = iLineCnt;
	m_iNumber = iNumber;

	return S_OK;
}

_int CHpNumber::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;	

	if (0 >= m_iLineCnt)
		m_iLineCnt = 0;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CHpNumber::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	CalculateDigitCnt(m_iLineCnt);
	CheckNumber();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CHpNumber::Render_Geometry(const _double & dTimeDelta)
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

void CHpNumber::CalculateDigitCnt(_uint iLineCnt)
{
	int iQuotient = iLineCnt / 10;
	if (0 != iQuotient)
	{
		CalculateDigitCnt(iQuotient);
		++m_iDigitCnt;
	}
}

void CHpNumber::CheckNumber()
{
	if (0 == m_iDigitCnt)
	{
		switch (m_eDigitType)
		{
		case CHpNumber::UNITS:
			m_iNumber = 11;
			break;
		case CHpNumber::TENS:
			m_iNumber = 11;
			break;
		case CHpNumber::HUNDREDS:
			m_iNumber = 11;
			break;
		case CHpNumber::THOUSANDS:
			m_iNumber = 11;
			break;
		}
	}
	else if (1 == m_iDigitCnt)
	{
		_uint iUnits = 0;
		iUnits = m_iLineCnt % 10;

		switch (m_eDigitType)
		{
		case CHpNumber::UNITS:
			m_iNumber = iUnits;
			break;
		case CHpNumber::TENS:
			m_iNumber = 10;
			break;
		case CHpNumber::HUNDREDS:
			m_iNumber = 11;
			break;
		case CHpNumber::THOUSANDS:
			m_iNumber = 11;
			break;
		}

	}
	else if (2 == m_iDigitCnt)
	{
		_uint iUnits, iTens = 0;
		iUnits = m_iLineCnt % 10;
		iTens = m_iLineCnt / 10;

		switch (m_eDigitType)
		{
		case CHpNumber::UNITS:
			m_iNumber = iUnits;
			break;
		case CHpNumber::TENS:
			m_iNumber = iTens;
			break;
		case CHpNumber::HUNDREDS:
			m_iNumber = 10;
			break;
		case CHpNumber::THOUSANDS:
			m_iNumber = 11;
			break;
		}

	}
	else if (3 == m_iDigitCnt)
	{
		_uint iUnits, iTens, iHundreds = 0;

		iUnits = m_iLineCnt % 10;
		iTens = (m_iLineCnt / 10) % 10;
		iHundreds = (_uint)floor(m_iLineCnt*0.01f);

		switch (m_eDigitType)
		{
		case CHpNumber::UNITS:
			m_iNumber = iUnits;
			break;
		case CHpNumber::TENS:
			m_iNumber = iTens;
			break;
		case CHpNumber::HUNDREDS:
			m_iNumber = iHundreds;
			break;
		case CHpNumber::THOUSANDS:
			m_iNumber = 10;
			break;
		}
	}

	m_iDigitCnt = 1;

}

HRESULT CHpNumber::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_HpNumber"));
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

HRESULT CHpNumber::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_iNumber);
	return S_OK;
}

CHpNumber * CHpNumber::Create(LPDIRECT3DDEVICE9 pGraphicDev, DIGITTYPE eType, _uint iLineCnt, _uint iNumber/*, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ*/)
{
	CHpNumber* pInstance = new CHpNumber(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, iLineCnt, iNumber/*, fX, fY, fSizeX, fSizeY, fViewZ*/)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CHpNumber::Free()
{
	CUIObject::Free();
}
