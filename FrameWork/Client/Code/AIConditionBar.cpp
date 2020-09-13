#include "stdafx.h"
#include "AIConditionBar.h"
#include "AIBackBar.h"
#include "AISuperArmor.h"
#include "AIHP.h"

CAIConditionBar::CAIConditionBar(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CNormalObject(pGraphicDev),
		m_vScale(0.f, 0.f, 0.f),
		m_pTargetTransformCom(nullptr),	
		m_bLateInit(true)
{
}

CAIConditionBar::~CAIConditionBar()
{
}

HRESULT CAIConditionBar::Ready_GameObject(CONTROLTYPE eAIType, Engine::CTransform* pTargetTransform)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_eAIType = eAIType;
	m_pTargetTransformCom = pTargetTransform;	

	_vec3 vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);
	vTargetPos.y += 30.f;
	m_pTransformCom->Set_Pos(&vTargetPos);


	m_vScale = _vec3(70.f, 7.f, 1.f);

	Compute_ViewZ(&vTargetPos);
	m_vScale *=  (m_fViewZ*0.001f);

	return S_OK;
}

_int CAIConditionBar::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	if (m_bLateInit)
	{
		CreateCondition(_vec3(0.f,0.f,0.f) , _vec3(1.f,1.f,1.f));
		m_bLateInit = false;
	}	

	////아이들을 총괄하는 행렬을 여기서 만들어///////////////
	//위치
	_vec3 vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);
	vTargetPos.y += 30.f;
	m_pTransformCom->Set_Pos(&vTargetPos);

	CNormalObject::Update_GameObject(dTimeDelta);
	////크기
	m_vScale = _vec3(70.f, 7.f, 1.f);

	Compute_ViewZ(&vTargetPos);
	m_vScale *= (m_fViewZ*0.001f);	

	if (100.f <= m_vScale.x && 10.f <= m_vScale.y)
	{
		m_vScale.x = 100.f;
		m_vScale.y = 10.f;
	}
	if (72.f >= m_vScale.x && 7.2f >= m_vScale.y)
	{
		m_vScale.x = 72.f;
		m_vScale.y = 7.2f ;
	}

	BillBoard(&m_vScale);


	return Engine::NO_EVENT;
}

_int CAIConditionBar::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	return Engine::NO_EVENT;
}

void CAIConditionBar::Render_Geometry(const _double & dTimeDelta)
{
}

void CAIConditionBar::CreateCondition(_vec3 vPos, _vec3 vSize)
{
	Engine::CGameObject*		pUIObject = nullptr;

	pUIObject = CAIBackBar::Create(m_pGraphicDev, CAIBackBar::ROUNDTYPE, m_pTransformCom,_vec3(vPos.x, vPos.y + 10.8f, vPos.z), _vec3(vSize.x, vSize.y, vSize.z));
	Engine::Add_GameObject(Engine::UI, L"AIBackBar", pUIObject);

	pUIObject = CAIBackBar::Create(m_pGraphicDev, CAIBackBar::RECTTYPE, m_pTransformCom, _vec3(vPos.x, vPos.y + 10.f, vPos.z), _vec3(vSize.x, vSize.y+0.5f, vSize.z));
	Engine::Add_GameObject(Engine::UI, L"AIBackBar", pUIObject);

	pUIObject = CAISuperArmor::Create(m_pGraphicDev,CAISuperArmor::FULL, m_pTransformCom, m_eAIType,_vec3(vPos.x+0.007f, vPos.y + 10.8f, vPos.z), _vec3(vSize.x-0.04f, vSize.y-0.4f, vSize.z));
	Engine::Add_GameObject(Engine::UI, L"AISuperArmor", pUIObject);
	
	//수퍼마어 empty 추가
	pUIObject = CAISuperArmor::Create(m_pGraphicDev, CAISuperArmor::EMPTY, m_pTransformCom, m_eAIType, _vec3(vPos.x + 0.007f, vPos.y + 10.8f, vPos.z), _vec3(vSize.x , vSize.y , vSize.z));
	Engine::Add_GameObject(Engine::UI, L"AISuperArmor", pUIObject);

	pUIObject = CAIHP::Create(m_pGraphicDev, CAIHP::HP, m_pTransformCom, m_eAIType, _vec3(vPos.x + 0.007f, vPos.y + 10.f, vPos.z), _vec3(vSize.x - 0.035f, vSize.y - 0.35f, vSize.z));
	Engine::Add_GameObject(Engine::UI, L"AIHp", pUIObject);

	pUIObject = CAIHP::Create(m_pGraphicDev, CAIHP::PREVHP, m_pTransformCom, m_eAIType, _vec3(vPos.x + 0.007f, vPos.y + 10.f, vPos.z), _vec3(vSize.x - 0.035f, vSize.y - 0.35f, vSize.z));
	Engine::Add_GameObject(Engine::UI, L"AIBeforeHp", pUIObject);

}

HRESULT CAIConditionBar::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	return S_OK;
}

HRESULT CAIConditionBar::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	return S_OK;
}

CAIConditionBar * CAIConditionBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, CONTROLTYPE eAIType, Engine::CTransform* pTargetTransform)
{
	CAIConditionBar* pInstance = new CAIConditionBar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eAIType, pTargetTransform)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CAIConditionBar::Free()
{
	CNormalObject::Free();
}
