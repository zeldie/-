#include "stdafx.h"
#include "DamageBox.h"
#include "Number.h"

CDamageBox::CDamageBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CNormalObject(pGraphicDev),
		m_eDamageBoxType(DAMAGEBOX_END),
		m_iDamage(0),
		m_iDigitCnt(1),
		m_vScale(0.f, 0.f, 0.f),
		m_pTargetTransformCom(nullptr),	
		m_bLateInit(true),
		m_dTime(0.f)
{
	D3DXMatrixIdentity(&m_matCamWorld);
}

CDamageBox::~CDamageBox()
{
}

HRESULT CDamageBox::Ready_GameObject(Engine::CTransform* pTargetTransform, DAMAGEBOXTYPE eType, _uint iDamage)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_eDamageBoxType = eType;
	m_iDamage = iDamage;
	m_pTargetTransformCom = pTargetTransform;


	if (BASIC == m_eDamageBoxType)
		m_vScale = _vec3(60.f, 60.f, 1.f);
	else if(CRITICAL == m_eDamageBoxType)
		m_vScale = _vec3(80.f, 80.f, 1.f);
	
	//랜덤위치 생성
	_vec3 vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);

	_int iRandom = Engine::RandomNumber(2);
	if (0 == iRandom)
		iRandom = 1;
	else
		iRandom = -1;

	vTargetPos.x += Engine::RandomNumber(50) * iRandom;
	vTargetPos.y += Engine::RandomNumber(50) * 0.7f;

	m_pTransformCom->Set_Pos(&vTargetPos);
	Compute_ViewZ(&vTargetPos);

	m_vScale *=  (m_fViewZ*0.001f);

	return S_OK;
}

_int CDamageBox::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	if (m_bLateInit)
	{
		CreateNumber(m_eDamageBoxType, _vec3(0.f, 0.5f, 0.f), _vec3(1.f,1.f,1.f), m_iDamage);
		m_bLateInit = false;
	}	

	////넘버들을 총괄하는 행렬을 여기서 만들어///////////////
	CNormalObject::Update_GameObject(dTimeDelta);
	BillBoard(&m_vScale);
	if (BASIC == m_eDamageBoxType)
	{
		m_vScale -= _vec3((_float)dTimeDelta*200.f, (_float)dTimeDelta*200.f, 1.f);
		if (10.f >= m_vScale.x)
			m_vScale = _vec3(10.f, 10.f, 1.f);
	}
	else if (CRITICAL == m_eDamageBoxType)
	{
		m_vScale -= _vec3((_float)dTimeDelta*300.f, (_float)dTimeDelta*300.f, 1.f);
		if (20.f >= m_vScale.x)
			m_vScale = _vec3(20.f, 20.f, 1.f);

	}

	/////////////////////////////////////////////////////////

	m_dTime += dTimeDelta*10.f;
	

	return Engine::NO_EVENT;
}

_int CDamageBox::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	//이동
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &m_matCamWorld);
	D3DXMatrixInverse(&m_matCamWorld, nullptr, &m_matCamWorld);

	_vec3 vRight;
	memcpy(&vRight, &m_matCamWorld.m[0], sizeof(_vec3));
	D3DXVec3Normalize(&vRight, &vRight);
	

	_vec3 vUp;
	if (BASIC == m_eDamageBoxType)
	{
		m_pTransformCom->Move_Pos(&vRight, dTimeDelta*150.f, dTimeDelta);
		
		if (2.0f <= m_dTime)
		{
			vUp = _vec3(0.f, -1.f, 0.f);
			m_pTransformCom->Move_Pos(&vUp, 200.f, dTimeDelta);
		}
		else
		{
			vUp = _vec3(0.f, 1.f, 0.f);
			m_pTransformCom->Move_Pos(&vUp, 400.f, dTimeDelta);
		}
	}		
	else if (CRITICAL == m_eDamageBoxType)
	{
		m_pTransformCom->Move_Pos(&vRight, dTimeDelta*150.f, dTimeDelta);

		if (2.f <= m_dTime)
		{
			vUp = _vec3(0.f, -1.f, 0.f);
			m_pTransformCom->Move_Pos(&vUp, 300.f, dTimeDelta);
		}
		else
		{
			vUp = _vec3(0.f, 1.f, 0.f);
			m_pTransformCom->Move_Pos(&vUp, 600.f, dTimeDelta);
		}
	}

	return Engine::NO_EVENT;
}

void CDamageBox::Render_Geometry(const _double & dTimeDelta)
{
}

void CDamageBox::CreateNumber(DAMAGEBOXTYPE eType, _vec3 vPos, _vec3 vSize, _uint iDamage)
{
	CalculateDigitCnt(iDamage);

	if (2 == m_iDigitCnt)
	{
		_uint iUnits, iTens = 0;
		iUnits = iDamage % 10;
		iTens = (iDamage / 10);	

		CGameObject* pGameObject = nullptr;
		pGameObject = CNumber::Create(m_pGraphicDev, this, m_pTransformCom, (CNumber::NUMBERTYPE)eType, _vec3(vPos.x + 0.3f, vPos.y, vPos.z), vSize, iUnits);
		Engine::Add_GameObject(Engine::UI, L"Number", pGameObject);

		pGameObject = CNumber::Create(m_pGraphicDev, this, m_pTransformCom, (CNumber::NUMBERTYPE)eType,  _vec3(vPos.x-0.3f, vPos.y, vPos.z), vSize, iTens);
		Engine::Add_GameObject(Engine::UI, L"Number", pGameObject);

	}
	else if (3 == m_iDigitCnt)
	{
		_uint iUnits, iTens, iHundreds = 0;

		iUnits = iDamage % 10;
		iTens = (iDamage/10) % 10;
		iHundreds = (_uint)floor(iDamage*0.01f);

		CGameObject* pGameObject = nullptr;
		pGameObject = CNumber::Create(m_pGraphicDev, this, m_pTransformCom, (CNumber::NUMBERTYPE)eType,  _vec3(vPos.x+ 0.7f, vPos.y, vPos.z), vSize,iUnits);
		Engine::Add_GameObject(Engine::UI, L"Number", pGameObject);

		pGameObject = CNumber::Create(m_pGraphicDev, this, m_pTransformCom, (CNumber::NUMBERTYPE)eType,  _vec3(vPos.x , vPos.y, vPos.z), vSize, iTens);
		Engine::Add_GameObject(Engine::UI, L"Number", pGameObject);

		pGameObject = CNumber::Create(m_pGraphicDev, this, m_pTransformCom, (CNumber::NUMBERTYPE)eType,  _vec3(vPos.x- 0.7f, vPos.y, vPos.z), vSize,iHundreds);
		Engine::Add_GameObject(Engine::UI, L"Number", pGameObject);

	}
	else if (4 == m_iDigitCnt)
	{
		_uint iUnits, iTens, iHundreds, iThousands = 0;

		iUnits = iDamage % 10;
		iTens = (iDamage/10) % 10;
		iHundreds = (iDamage / 100) % 10;
		iThousands = (_uint)floor(iDamage*0.001f);

		CGameObject* pGameObject = nullptr;
		pGameObject = CNumber::Create(m_pGraphicDev, this, m_pTransformCom, (CNumber::NUMBERTYPE)eType,  _vec3(vPos.x + 1.1f, vPos.y, vPos.z), vSize, iUnits);
		Engine::Add_GameObject(Engine::UI, L"Number", pGameObject);

		pGameObject = CNumber::Create(m_pGraphicDev, this, m_pTransformCom, (CNumber::NUMBERTYPE)eType,  _vec3(vPos.x+0.35f, vPos.y, vPos.z), vSize, iTens);
		Engine::Add_GameObject(Engine::UI, L"Number", pGameObject);

		pGameObject = CNumber::Create(m_pGraphicDev, this, m_pTransformCom, (CNumber::NUMBERTYPE)eType,  _vec3(vPos.x - 0.35f, vPos.y, vPos.z), vSize, iHundreds);
		Engine::Add_GameObject(Engine::UI, L"Number", pGameObject);

		pGameObject = CNumber::Create(m_pGraphicDev, this, m_pTransformCom, (CNumber::NUMBERTYPE)eType,  _vec3(vPos.x - 1.1f, vPos.y, vPos.z), vSize, iThousands);
		Engine::Add_GameObject(Engine::UI, L"Number", pGameObject);

	}

}

void CDamageBox::CalculateDigitCnt(_uint iDamage)
{
	int iQuotient = iDamage / 10;
	if (0 != iQuotient)
	{
		CalculateDigitCnt(iQuotient);
		++m_iDigitCnt;
	}
}

HRESULT CDamageBox::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	return S_OK;
}

HRESULT CDamageBox::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	return S_OK;
}

CDamageBox * CDamageBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTargetTransform, DAMAGEBOXTYPE eType,_uint iDamage)
{
	CDamageBox* pInstance = new CDamageBox(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pTargetTransform, eType, iDamage)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDamageBox::Free()
{
	CNormalObject::Free();
}
