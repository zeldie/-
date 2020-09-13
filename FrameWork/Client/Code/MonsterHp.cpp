#include "stdafx.h"
#include "MonsterHp.h"
#include "ScreenTex.h"
#include "Belatos.h"
#include "MonsterHpMgr.h"

CMonsterHp::CMonsterHp(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eMonsterHpType(MONSTERHPTYPE_END),
	//m_iMonsterMaxHp(0),
	m_fMaxHpPerLine(0.f),
	m_fDamage(0.f),
	m_fRemainHp(0.f),
	m_fGiveNextHp(0.f),
	m_fResultValue(1.f),
	m_bLateInit(true),
	m_bSwap(false),
	m_bHpIsZero(false),
	m_bCheck(true),
	m_bChangeColor(false),
	m_fR(1.f),
	m_fG(1.f),
	m_fB(1.f)
{
}


CMonsterHp::~CMonsterHp()
{
}

void CMonsterHp::Set_VertexZ()
{
	m_pScreenTexBufferCom->VertexZControl();
}

HRESULT CMonsterHp::Ready_GameObject(MONSTERHPTYPE eType, _bool bBool, _float MaxHpPerLine, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eMonsterHpType = eType;
	m_bWorking = bBool;
	m_fMaxHpPerLine = MaxHpPerLine;

	m_fRemainHp = m_fMaxHpPerLine;

	return S_OK;
}

_int CMonsterHp::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;	

	if (m_bHpIsZero)
	{
		m_fRemainHp = 0.f;
		m_pScreenTexBufferCom->VertexXControl_LeftDir(0.f);
		return Engine::NO_EVENT;
	}

	if (m_fMaxHpPerLine <= m_fRemainHp)
		m_fRemainHp = m_fMaxHpPerLine;


	if (m_bWorking)
	{
		if (m_fDamage >= m_fRemainHp) //남아있는양보다 데미지가 더클때
		{
			if (m_bCheck)
			{
				m_fGiveNextHp = m_fDamage - m_fRemainHp;//깍을양 - 남아있는양 (다음에 넘겨줄때 써먹어야하니까)			
				m_bCheck = false;
			}

			if (0 >= m_fRemainHp)
			{
				//  워킹 중단 후 z값 뒤로 보내고
				m_bWorking = false;
				m_pScreenTexBufferCom->VertexZControl();

				//	다른애를 워킹 시키고 z값 앞으로 보내고 ->다른애 워킹 여기서, 레이트업뎃 앞으로보내기
				m_bSwap = true;

				//  남아있는 양 다시 풀로 채우고->레이트업뎃 / 데미지도 0으로 				
				m_fDamage = 0;
				
				//  다음애가 깍이기 시작해야할값을 여기서 계산해서 mgr 통해서 전달해야할듯
				dynamic_cast<CMonsterHpMgr*>(Engine::Get_GameObject(Engine::UI, L"MonsterHpMgr"))->Set_GiveNextHp(m_fGiveNextHp);
				dynamic_cast<CMonsterHpMgr*>(Engine::Get_GameObject(Engine::UI, L"MonsterHpMgr"))->Set_Swap(true);
			}
			else
			{
				m_fRemainHp = m_fMaxHpPerLine - m_fDamage;			
			}

		}
		else //일반상황
		{
			m_fRemainHp = m_fMaxHpPerLine - m_fDamage;			
		}
	}
	else
	{
		m_pScreenTexBufferCom->VertexXControl_LeftDir(1.f);
	}

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CMonsterHp::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (m_bHpIsZero)
	{
		m_fRemainHp = 0.f;
		m_pScreenTexBufferCom->VertexXControl_LeftDir(0.f);
		return Engine::NO_EVENT;
	}


	if (m_bWorking)
	{
		m_fResultValue = m_fRemainHp / m_fMaxHpPerLine;
		m_pScreenTexBufferCom->VertexXControl_LeftDir(m_fResultValue);
	}
	else
	{
		if (0 >= m_fRemainHp)
		{
			m_bCheck = true;
			m_fRemainHp = m_fMaxHpPerLine;
			
			dynamic_cast<CMonsterHpMgr*>(Engine::Get_GameObject(Engine::UI, L"MonsterHpMgr"))->Set_GiveNextHp(0.f);
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CMonsterHp::Render_Geometry(const _double & dTimeDelta)
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

HRESULT CMonsterHp::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_MonsterHp"));
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

HRESULT CMonsterHp::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (m_bChangeColor)
	{
		m_fR = Engine::RandomNumber(100) * 0.01f;
		m_fG = Engine::RandomNumber(100) * 0.01f;
		m_fB = Engine::RandomNumber(100) * 0.01f;

		if (0.f >= m_fR)
			m_fR = 1.f;
		
		if (0.f >= m_fG)
			m_fG = 1.f;

		if (0.f >= m_fB)
			m_fB = 1.f;

		m_bChangeColor = false;
	}


	pEffect->SetVector("g_vRGBA", &_vec4(m_fR, m_fG, m_fB, 1.f));
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eMonsterHpType);
	return S_OK;
}

CMonsterHp * CMonsterHp::Create(LPDIRECT3DDEVICE9 pGraphicDev, MONSTERHPTYPE eType, _bool bBool, _float MaxHpPerLine, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CMonsterHp* pInstance = new CMonsterHp(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, bBool, MaxHpPerLine, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMonsterHp::Free()
{
	CUIObject::Free();
}
