#include "stdafx.h"
#include "AISuperArmor.h"
#include "RcTex.h"
#include "AI_Alliance_Observer.h"
#include "AI_Enemy1_Observer.h"
#include "AI_Enemy2_Observer.h"

CAISuperArmor::CAISuperArmor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNormalObject(pGraphicDev),
	m_pAllianceObserver(nullptr),
	m_pEnemy1_Observer(nullptr),
	m_pEnemy2_Observer(nullptr),
	m_pParentTransformCom(nullptr),
	m_fResultValue(1.f),
	m_iMaxSuperArmor(0),
	//m_bLateInit(true),
	m_bAISuperArmorRender(false)
{
}


CAISuperArmor::~CAISuperArmor()
{
}

HRESULT CAISuperArmor::Ready_GameObject(SUPERARMORTYPE eType, Engine::CTransform* pParentTransform, CONTROLTYPE eAIType, _vec3 vPos, _vec3 vSize)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_eSuperArmor = eType;
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
			Engine::Subscribe(Engine::AI_ALLIANCE,m_pAllianceObserver);
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

_int CAISuperArmor::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	
	//if (m_bLateInit)
	//{
		switch (m_eAIType)
		{
			case CTRL_AI_ALLIANCE:			
				m_iMaxSuperArmor = m_pAllianceObserver->Get_MaxSuperArmor();
				break;			
			case CTRL_AI_ENERMY_1:			
				m_iMaxSuperArmor = m_pEnemy1_Observer->Get_MaxSuperArmor();
				break;			
			case CTRL_AI_ENERMY_2:			
				m_iMaxSuperArmor = m_pEnemy2_Observer->Get_MaxSuperArmor();
				break;			
		}
		//m_bLateInit = false;
	//}

	CNormalObject::Update_GameObject(dTimeDelta);
	m_pTransformCom->Set_ParentMatrix(m_pParentTransformCom->Get_WorldMatrix()); //부모행렬->빌보드적용되어있음

	if (EMPTY == m_eSuperArmor)
	{
		m_pTransformCom->Set_Scale(0.1f,1.f,1.f);
	}

	return Engine::NO_EVENT;
}

_int CAISuperArmor::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	_uint iSuperArmor = 0;
	switch (m_eAIType)
	{
	case CTRL_AI_ALLIANCE:
		iSuperArmor = m_pAllianceObserver->Get_SuperArmor();
		break;
	case CTRL_AI_ENERMY_1:
		iSuperArmor = m_pEnemy1_Observer->Get_SuperArmor();
		break;
	case CTRL_AI_ENERMY_2:
		iSuperArmor = m_pEnemy2_Observer->Get_SuperArmor();
		break;
	}

	if (m_iMaxSuperArmor <= iSuperArmor)
		iSuperArmor = m_iMaxSuperArmor;
	
	////test
	//iSuperArmor = 0;
	
	if (FULL == m_eSuperArmor)
	{
		if (0 >= iSuperArmor)
			m_bAISuperArmorRender = false;
		else
		{
			m_bAISuperArmorRender = true;
			m_fResultValue = (_float)iSuperArmor / (_float)m_iMaxSuperArmor;
			m_pBufferCom->VertexXControl_LeftDir(m_fResultValue);
		}
	}
	else if (EMPTY == m_eSuperArmor)
	{
		if (0 >= iSuperArmor)
			m_bAISuperArmorRender = true;
		else
			m_bAISuperArmorRender = false;
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CAISuperArmor::Render_Geometry(const _double & dTimeDelta)
{
	if (m_bAISuperArmorRender)
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
		pEffect->BeginPass(24);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

HRESULT CAISuperArmor::Clone_Component()
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_SuperArmor"));
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

HRESULT CAISuperArmor::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eSuperArmor);

	return S_OK;
}

CAISuperArmor * CAISuperArmor::Create(LPDIRECT3DDEVICE9 pGraphicDev, SUPERARMORTYPE eType, Engine::CTransform * pParentTransform, CONTROLTYPE eAIType, _vec3 vPos, _vec3 vSize)
{
	CAISuperArmor* pInstance = new CAISuperArmor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, pParentTransform, eAIType, vPos, vSize)))
		Engine::Safe_Release(pInstance);

	return pInstance;

}

void CAISuperArmor::Free()
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
