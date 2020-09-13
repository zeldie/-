#include "stdafx.h"
#include "PlayerHp.h"
#include "ScreenTex.h"
#include "PlayerObserver.h"

CPlayerHp::CPlayerHp(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_pPlayerObserver(nullptr),
	m_ePlayerHpType(PLAYERHPTYPE_END),
	m_iPlayerMaxHp(0),
	m_fResultValue(1.f),
	m_iHp_Before(0),
	m_bLateInit(true)
{
}


CPlayerHp::~CPlayerHp()
{
}

HRESULT CPlayerHp::Ready_GameObject(PLAYERHPTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_ePlayerHpType = eType;

	//옵저버 신청
	m_pPlayerObserver = CPlayerObserver::Create();
	if (nullptr == m_pPlayerObserver)
		return E_FAIL;
	Engine::Subscribe(Engine::PLAYER, m_pPlayerObserver);

	return S_OK;
}

_int CPlayerHp::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	
	if (m_bLateInit)
	{
		m_iPlayerMaxHp= m_pPlayerObserver->Get_MaxHp();
		m_bLateInit = false;
	}

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CPlayerHp::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	int iHp = 0;	
	if (HP == m_ePlayerHpType)
	{
		//옵저버에서 정보받아야함
		iHp = m_pPlayerObserver->Get_Hp();
		if (m_iPlayerMaxHp <= iHp)
			iHp = m_iPlayerMaxHp;

		//if (m_iHp_Before != iHp)
		//{
			m_fResultValue = (float)iHp / (float)m_iPlayerMaxHp;
			m_pScreenTexBufferCom->VertexXControl_LeftDir(m_fResultValue);
		//	m_iHp_Before = iHp;
		//}
	}
	else if (PREVHP == m_ePlayerHpType)
	{
		iHp = m_pPlayerObserver->Get_PrevHp();
		if (m_iPlayerMaxHp <= iHp)
			iHp = m_iPlayerMaxHp;

		//if (m_iHp_Before != iHp)
		//{
			m_fResultValue = (float)iHp / (float)m_iPlayerMaxHp;
			m_pScreenTexBufferCom->VertexXControl_LeftDir(m_fResultValue);
		//	m_iHp_Before = iHp;
		//}
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CPlayerHp::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CPlayerHp::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_PlayerHp"));
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

HRESULT CPlayerHp::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect,"g_DiffuseTexture", m_ePlayerHpType);

	return S_OK;
}

CPlayerHp * CPlayerHp::Create(LPDIRECT3DDEVICE9 pGraphicDev, PLAYERHPTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CPlayerHp* pInstance = new CPlayerHp(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);
	
	return pInstance;
}

void CPlayerHp::Free()
{
	CUIObject::Free();

	Engine::UnSubscribe(Engine::PLAYER, m_pPlayerObserver);
	Engine::Safe_Release(m_pPlayerObserver);
}
