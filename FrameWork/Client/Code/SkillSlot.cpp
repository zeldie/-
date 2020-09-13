#include "stdafx.h"
#include "SkillSlot.h"
#include "ScreenTex.h"

CSkillSlot::CSkillSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eWeaponType(WEAPONTYPE_END),
	m_eSkillSlotType(SKILLSLOT_END),
	m_eKeyType(Engine::KEYGUIDE_END),
	m_fResultValue(1.f),
	m_bCoolDown(false),
	m_fCoolDownSpeed(1.f),
	m_dTime(0.f),
	m_bCoolDowning(false),
	m_bSwitchRenderUI(false)
{
	m_bRendering = false;
}


CSkillSlot::~CSkillSlot()
{
}

HRESULT CSkillSlot::Ready_GameObject(WEAPONTYPE eWeaponType, SKILLSLOT eType, Engine::KEYGUIDE eKeyType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_bRenderUI = bRender;
	m_eSkillSlotType = eType;
	m_eWeaponType = eWeaponType;
	m_eKeyType = eKeyType;

	if (RECHARGE == m_eSkillSlotType)
	{
		if (MAIN == m_eWeaponType || WEAPONTYPE_END == m_eWeaponType/*basic skill 경우*/)
		{
			m_bRenderUI = true;
			m_bRendering = true;
		}
		else if (SUB == m_eWeaponType)
		{
			m_bRenderUI = false;
			m_bRendering = true;
		}
	}

	//m_bRenderUI -> 그려라 그리지마라 (main /sub 구분용)
	//m_bRendering -> 그려지고 있는 것들 중에서 (그리는 중이다 안그리는중이다)를 판단
	//m_bSwitchRenderUI -> 무기 바꼈을때 무기 별 쿨타임 보존을 위한 변수

	return S_OK;
}

_int CSkillSlot::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

		if (m_bSwitchRenderUI)
		{// 무기가 바꼈으니 그리던걸 그리지말고, 그리지 않던걸 그려라
			if (m_bRenderUI)
				m_bRenderUI = false;
			else
				m_bRenderUI = true;


			m_bSwitchRenderUI = false;
		}


	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CSkillSlot::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (RECHARGE == m_eSkillSlotType)
	{
		if (m_bRendering)
		{
			//
			if (m_bCoolDown)
			{
				//쿨다운 중일때는 같은 거 못써 그걸 구분해줄거야 아래변수로
				//m_bCoolDowning = true;

				switch (m_eKeyType)
				{
				case Engine::KEYGUIDE_E:
					m_fCoolDownSpeed = 0.2f; //5초
					break;
				case Engine::KEYGUIDE_R:
					m_fCoolDownSpeed = 0.125f; //8초
					break;
				case Engine::KEYGUIDE_F:
					m_fCoolDownSpeed = 0.08f; //12초
					break;
				case Engine::KEYGUIDE_LSHIFT:
					m_fCoolDownSpeed = 1.2f; //1초
					break;
				case Engine::KEYGUIDE_TAB:
					m_fCoolDownSpeed = 0.5f; //2초
					break;
				default:
					break;
				}

				//정점 조절 -> 스킬마다 차오르는 시간이 다를예정
				m_fResultValue -= (_float)dTimeDelta * m_fCoolDownSpeed;
				m_pScreenTexBufferCom->VertexYControl_UpDir(m_fResultValue);
				if (0.f >= m_fResultValue)
				{
					m_fResultValue = 1.f;
					m_bCoolDown = false;
					m_bCoolDowning = false;
				}
			}
			else
			{
				// 예시) E스킬 쓰는중이면 모두가 색이 없어
				//       E스킬 애니매이션 끝나면 E 스킬은 쿨시작, 나머지는 색이 있어
				//      """ 색이 있는 """"경우를 이때 만들어준당
				m_pScreenTexBufferCom->VertexYControl_UpDir(0.f);

			}
		}
		else
		{
			// 예시) E스킬 쓰는중이면 모두가 색이 없어
			//       E스킬 애니매이션 끝나면 E 스킬은 쿨시작, 나머지는 색이 있어
			//      """ 색이 없는 """경우를 이때 만들어준당
			m_pScreenTexBufferCom->VertexYControl_UpDir(1.f);
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CSkillSlot::Render_Geometry(const _double & dTimeDelta)
{
	if (m_bRenderUI)
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
	}
}

HRESULT CSkillSlot::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_SkillSlot"));
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

HRESULT CSkillSlot::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (BASIC == m_eSkillSlotType)
	{
		pEffect->SetVector("g_vRGBA", &_vec4(0.3f, 0.2f, 0.06f, 1.f));
	}
	else if (RECHARGE == m_eSkillSlotType)
	{
		switch (m_eKeyType)
		{
		case Engine::KEYGUIDE_E:
			pEffect->SetVector("g_vRGBA", &_vec4(0.45f, 0.56f, 0.78f, 0.8f));
			break;
		case Engine::KEYGUIDE_R:
			pEffect->SetVector("g_vRGBA", &_vec4(0.45f, 0.56f, 0.78f, 0.8f));
			break;
		case Engine::KEYGUIDE_F:
			pEffect->SetVector("g_vRGBA", &_vec4(0.45f, 0.56f, 0.78f, 0.8f));
			break;
		case Engine::KEYGUIDE_LSHIFT:
			pEffect->SetVector("g_vRGBA", &_vec4(1.f, 0.45f, 0.17f, 0.8f));
			break;
		case Engine::KEYGUIDE_TAB:
			pEffect->SetVector("g_vRGBA", &_vec4(0.49f, 0.57f, 0.24f, 0.8f));
			break;
		default:
			break;
		}
	}
	else if (OUTLINE == m_eSkillSlotType)
	{
		pEffect->SetVector("g_vRGBA", &_vec4(0.f, 0.f, 0.f, 1.f));
	}

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eSkillSlotType);
	return S_OK;
}

CSkillSlot * CSkillSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev, WEAPONTYPE eWeaponType, SKILLSLOT eType, Engine::KEYGUIDE eKeyType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CSkillSlot* pInstance = new CSkillSlot(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eWeaponType, eType, eKeyType, bRender, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSkillSlot::Free()
{
	CUIObject::Free();
}
