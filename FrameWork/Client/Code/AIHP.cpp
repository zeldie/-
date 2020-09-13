#include "stdafx.h"
#include "AIHP.h"
#include "RcTex.h"
#include "AI_Alliance_Observer.h"
#include "AI_Enemy1_Observer.h"
#include "AI_Enemy2_Observer.h"

CAIHP::CAIHP(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNormalObject(pGraphicDev),
	m_pAllianceObserver(nullptr),
	m_pEnemy1_Observer(nullptr),
	m_pEnemy2_Observer(nullptr),
	m_pParentTransformCom(nullptr),
	m_fResultValue(1.f),
	m_iMaxHp(0)
	//m_bLateInit(true)
{
}


CAIHP::~CAIHP()
{
}

HRESULT CAIHP::Ready_GameObject(HPTYPE eType, Engine::CTransform* pParentTransform, CONTROLTYPE eAIType, _vec3 vPos, _vec3 vSize)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_eAIHPType = eType;
	m_pParentTransformCom = pParentTransform;
	m_eAIType = eAIType;

	m_pTransformCom->Set_Pos(&vPos);
	m_pTransformCom->Set_Scale(&vSize);

	////옵저버신청
	switch (m_eAIType)
	{
	case CTRL_AI_ALLIANCE:
	{
		m_pAllianceObserver = CAI_Alliance_Observer::Create();
		if (nullptr == m_pAllianceObserver)
			return E_FAIL;
		Engine::Subscribe(Engine::AI_ALLIANCE, m_pAllianceObserver);
		break;
	}
	case CTRL_AI_ENERMY_1:
	{
		m_pEnemy1_Observer = CAI_Enemy1_Observer::Create();
		if (nullptr == m_pEnemy1_Observer)
			return E_FAIL;
		Engine::Subscribe(Engine::AI_ENEMY_1, m_pEnemy1_Observer);
		break;
	}
	case CTRL_AI_ENERMY_2:
	{
		m_pEnemy2_Observer = CAI_Enemy2_Observer::Create();
		if (nullptr == m_pEnemy2_Observer)
			return E_FAIL;
		Engine::Subscribe(Engine::AI_ENEMY_2, m_pEnemy2_Observer);
		break;
	}
	}


	return S_OK;
}

_int CAIHP::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	//if (m_bLateInit)
	//{
	switch (m_eAIType)
	{
	case CTRL_AI_ALLIANCE:
		m_iMaxHp = m_pAllianceObserver->Get_MaxHp();
		break;
	case CTRL_AI_ENERMY_1:
		m_iMaxHp = m_pEnemy1_Observer->Get_MaxHp();
		break;
	case CTRL_AI_ENERMY_2:
		m_iMaxHp = m_pEnemy2_Observer->Get_MaxHp();
		break;
	}
	//m_bLateInit = false;
//}

	CNormalObject::Update_GameObject(dTimeDelta);
	m_pTransformCom->Set_ParentMatrix(m_pParentTransformCom->Get_WorldMatrix()); //부모행렬->빌보드적용되어있음

	return Engine::NO_EVENT;
}

_int CAIHP::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	int iHp = 0;
	if (HP == m_eAIHPType)
	{
		switch (m_eAIType)
		{
		case CTRL_AI_ALLIANCE:
			iHp = m_pAllianceObserver->Get_Hp();
			break;
		case CTRL_AI_ENERMY_1:
			iHp = m_pEnemy1_Observer->Get_Hp();
			break;
		case CTRL_AI_ENERMY_2:
			iHp = m_pEnemy2_Observer->Get_Hp();
			break;
		}

		if (m_iMaxHp <= iHp)
			iHp = m_iMaxHp;

		m_fResultValue = (float)iHp / (float)m_iMaxHp;
		m_pBufferCom->VertexXControl_LeftDir(m_fResultValue);
	}
	else if (PREVHP == m_eAIHPType)
	{
		switch (m_eAIType)
		{
		case CTRL_AI_ALLIANCE:
			iHp = m_pAllianceObserver->Get_PrevHp();
			break;
		case CTRL_AI_ENERMY_1:
			iHp = m_pEnemy1_Observer->Get_PrevHp();
			break;
		case CTRL_AI_ENERMY_2:
			iHp = m_pEnemy2_Observer->Get_PrevHp();
			break;
		}

		if (m_iMaxHp <= iHp)
			iHp = m_iMaxHp;

		m_fResultValue = (float)iHp / (float)m_iMaxHp;
		m_pBufferCom->VertexXControl_LeftDir(m_fResultValue);
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CAIHP::Render_Geometry(const _double & dTimeDelta)
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
	pEffect->BeginPass(10);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CAIHP::Clone_Component()
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

HRESULT CAIHP::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	// 아군-> 파. 적군->빨. prevhp->노.
	if (HP == m_eAIHPType)
	{
		switch (m_eAIType)
		{
		case CTRL_AI_ALLIANCE:
			pEffect->SetVector("g_vRGBA", &_vec4(0.06f, 0.83f, 1.f, 1.f));
			break;
		case CTRL_AI_ENERMY_1:
			pEffect->SetVector("g_vRGBA", &_vec4(0.98f, 0.23f, 0.31f, 1.f));
			break;
		case CTRL_AI_ENERMY_2:
			pEffect->SetVector("g_vRGBA", &_vec4(0.98f, 0.23f, 0.31f, 1.f));
			break;
		}
	}
	else if (PREVHP == m_eAIHPType)
	{
		pEffect->SetVector("g_vRGBA",&_vec4(0.99f,0.82f,0.15f,1.f));
	}

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");

	return S_OK;
}

CAIHP * CAIHP::Create(LPDIRECT3DDEVICE9 pGraphicDev, HPTYPE eType, Engine::CTransform * pParentTransform, CONTROLTYPE eAIType, _vec3 vPos, _vec3 vSize)
{
	CAIHP* pInstance = new CAIHP(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, pParentTransform, eAIType, vPos, vSize)))
		Engine::Safe_Release(pInstance);

	return pInstance;

}

void CAIHP::Free()
{
	CNormalObject::Free();

	switch (m_eAIType)
	{
	case CTRL_AI_ALLIANCE:
	{
		Engine::UnSubscribe(Engine::AI_ALLIANCE, m_pAllianceObserver);
		Engine::Safe_Release(m_pAllianceObserver);
		break;
	}
	case CTRL_AI_ENERMY_1:
	{
		Engine::UnSubscribe(Engine::AI_ENEMY_1, m_pEnemy1_Observer);
		Engine::Safe_Release(m_pEnemy1_Observer);
		break;
	}
	case CTRL_AI_ENERMY_2:
	{
		Engine::UnSubscribe(Engine::AI_ENEMY_2, m_pEnemy2_Observer);
		Engine::Safe_Release(m_pEnemy2_Observer);
		break;
	}
	}
}
