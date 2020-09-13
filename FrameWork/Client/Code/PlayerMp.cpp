#include "stdafx.h"
#include "PlayerMp.h"
#include "ScreenTex.h"
#include "PlayerObserver.h"

CPlayerMp::CPlayerMp(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CUIObject(pGraphicDev),
		m_pPlayerObserver(nullptr),
		m_fMaxMp(0.f),
		m_fResultValue(0.f),
		m_bLateInit(true)
{
}


CPlayerMp::~CPlayerMp()
{
}

HRESULT CPlayerMp::Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	//옵저버 신청
	m_pPlayerObserver = CPlayerObserver::Create();
	if (nullptr == m_pPlayerObserver)
		return E_FAIL;
	Engine::Subscribe(Engine::PLAYER, m_pPlayerObserver);

	return S_OK;
}

_int CPlayerMp::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	if (m_bLateInit)
	{
		m_fMaxMp = m_pPlayerObserver->Get_MaxMp();
		m_bLateInit = false;
	}

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CPlayerMp::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	//스킬안쓸땐 -->플레이어 안에서 구분예정

	//스킬쓸때만 -->플레이어 안에서 구분예정
	_float fMp = m_pPlayerObserver->Get_Mp();
	if (m_fMaxMp <= fMp)
		fMp = m_fMaxMp;

	m_fResultValue = fMp / m_fMaxMp;
	m_pScreenTexBufferCom->VertexXControl_LeftDir(m_fResultValue);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CPlayerMp::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CPlayerMp::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_PlayerMp"));
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

HRESULT CPlayerMp::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect,"g_DiffuseTexture");

	return S_OK;
}

CPlayerMp * CPlayerMp::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CPlayerMp* pInstance = new CPlayerMp(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CPlayerMp::Free()
{
	CUIObject::Free();

	Engine::UnSubscribe(Engine::PLAYER, m_pPlayerObserver);
	Engine::Safe_Release(m_pPlayerObserver);
}
