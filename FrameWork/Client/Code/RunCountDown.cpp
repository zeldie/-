#include "stdafx.h"
#include "RunCountDown.h"
#include "ScreenTex.h"
#include "MatchingResult.h"

CRunCountDown::CRunCountDown(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUIObject(pGraphicDev),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_iNumber(0),
	m_dTime(0.f),
	m_bTimeOver(false)
{
}


CRunCountDown::~CRunCountDown()
{
}

HRESULT CRunCountDown::Ready_GameObject(RunCountDownType eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eRunCountDownType = eType;

	if (RUNCOUNTDOWNTYPE_GAMESTART == m_eRunCountDownType)
		m_dTime = 4.f;
	else if (RUNCOUNTDOWNTYPE_REVIVAL == m_eRunCountDownType)
		m_dTime = 6.f;

	return S_OK;
}

_int CRunCountDown::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{
		if (RUNCOUNTDOWNTYPE_GAMESTART == m_eRunCountDownType)
		{
			//Start문구 생성
			Engine::CGameObject* pGameObject = nullptr;
			pGameObject = CMatchingResult::Create(m_pGraphicDev, CMatchingResult::START, true, 477.7f, 323.7f, 323.f, 80.f);
			if (pGameObject == nullptr)
				return Engine::NO_EVENT;
			Engine::Add_GameObject(Engine::UI, L"MatchingResult", pGameObject);

			m_pUIMgr->Set_RunGameStart(true);
		}
		else if (RUNCOUNTDOWNTYPE_REVIVAL == m_eRunCountDownType)
		{
			if (!m_bTimeOver)
			{
				//런유아이 생성
				m_pUIMgr->CreateRunUI(m_pGraphicDev);

				//마우스 바꿔주고
				m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);
			}
			else
			{
				//타임오버
				//이땐 그냥 결과창
				//시간멈추고
				m_pUIMgr->Set_RunGameTimeStop(true);
				//나가기 -> 결과창 뚜둔
				m_pUIMgr->CreateResultUI_Run(m_pGraphicDev);

			}
		}

		return Engine::OBJ_DEAD;
	}

	CUIObject::Update_GameObject(dTimeDelta);

	m_dTime -= dTimeDelta;

	return Engine::NO_EVENT;
}

_int CRunCountDown::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	m_iNumber = (_uint)m_dTime;
	if (1.f >= m_dTime)
	{
		m_dTime = 0.f;
		m_iNumber = 1;
		m_bIsDead = true;
		m_bTimeOver = true;
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CRunCountDown::Render_Geometry(const _double & dTimeDelta)
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
	pEffect->BeginPass(2);

	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CRunCountDown::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_DamageBox"));
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

HRESULT CRunCountDown::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if(RUNCOUNTDOWNTYPE_GAMESTART == m_eRunCountDownType)
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_iNumber);
	else if(RUNCOUNTDOWNTYPE_REVIVAL == m_eRunCountDownType)
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_iNumber + 10 );


	return S_OK;
}

CRunCountDown * CRunCountDown::Create(LPDIRECT3DDEVICE9 pGraphicDev, RunCountDownType eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CRunCountDown* pInstance = new CRunCountDown(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunCountDown::Free()
{
	CUIObject::Free();
}
