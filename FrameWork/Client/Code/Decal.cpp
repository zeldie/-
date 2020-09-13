#include "stdafx.h"
#include "Decal.h"
#include "Arrow.h"
#include "OrbCore.h"
#include "OrbBeam.h"
CDecal::CDecal(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseObject(pGraphicDev),
	m_eDecalType(DECAL_END),
	m_fDiminishRate(1.f),
	m_iPass(10),
	m_vUseMasking(1.f, 1.f,1.f,1.f),
	m_dCountDown(0.),
	m_dEffectTerm(0.05),
	m_bCreateEffect(false),
	m_bNeedToReleaseDecal(false)
{
}


CDecal::~CDecal()
{
}

HRESULT CDecal::Ready_GameObject(DECAL_TYPE eDecalType, _vec3 * pPos, _double dCountDown, _vec3 vPlayerAngle, CONTROLTYPE eControlType)
{
	if (FAILED(Clone_Component(eDecalType)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	switch (eDecalType)
	{
	case CDecal::DECAL_SKILLTARGET:
		m_pTransformCom->Set_Scale(1000.f, 1000.f, 1000.f);
		break;
	case CDecal::DECAL_ORBF:
		break;
	case CDecal::DECAL_ORBR:
		break;
	case CDecal::DECAL_TS_F:
		break;
	case CDecal::DECAL_BOW_FALL:
		m_pTransformCom->Set_Scale(1000.f, 200.f, 1000.f);
		break;
	case CDecal::DECAL_ORB_RIGHT:
		m_pTransformCom->Set_Scale(1000.f, 200.f, 1000.f);
		break;
	case CDecal::DECAL_ORB_E_BEAM:
		m_pTransformCom->Set_Scale(1000.f, 200.f, 1000.f);
		break;
	case CDecal::DECAL_ORB_F:
		m_pTransformCom->Set_Scale(1000.f, 200.f, 1000.f);
		break;
	case CDecal::DECAL_ORB_ULT:
		m_pTransformCom->Set_Scale(1000.f, 200.f, 1000.f);
		break;
	case CDecal::DECAL_ORB_FLYING:
		m_pTransformCom->Set_Scale(1000.f, 200.f, 1000.f);
		break;
	case CDecal::DECAL_END:
		break;
	default:
		break;
	}
	
	m_eDecalType = eDecalType;
	m_pDecalPos = pPos;
	m_dCountDown = dCountDown;
	m_vPlayerAngle = vPlayerAngle;
	m_eControlType = eControlType;
	return S_OK;
}

_int CDecal::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	if (m_dCountDown <= 0)
		m_bNeedToReleaseDecal = true;
	Move(dTimeDelta);
	Create_Effect(dTimeDelta);
	CBaseObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CDecal::LateUpdate_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	m_dCountDown -= dTimeDelta;

	if (m_dCountDown <= 0)
		m_bNeedToReleaseDecal = true;

	CBaseObject::LateUpdate_GameObject(dTimeDelta);
	Compute_ViewZ(m_pTransformCom->Get_Info(Engine::INFO_POS));
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	return Engine::NO_EVENT;
}

void CDecal::Render_Geometry(const _double & dTimeDelta)
{
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect, dTimeDelta)))
		return;
	_uint iPassMax = 0;
	Engine::SetUp_OnShader(pEffect, Engine::PRE_DEPTH, "g_PreDepthTexture");
	pEffect->CommitChanges();
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(m_iPass);
	m_pCubeCom->Render_Buffer();
	pEffect->EndPass();
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

HRESULT CDecal::Clone_Component(DECAL_TYPE eDecalType)
{
	Engine::CComponent* pComponent = nullptr;

	wstring wstrDecalID = L"";
	switch (eDecalType)
	{
	case CDecal::DECAL_SKILLTARGET:
		wstrDecalID = L"Decal_SkillTarget";
		break;
	case CDecal::DECAL_ORBF:
	case CDecal::DECAL_ORB_ULT:
	case CDecal::DECAL_ORB_FLYING:
		wstrDecalID = L"Decal_OrbF";
		break;
	case CDecal::DECAL_ORBR:
		wstrDecalID = L"Decal_OrbR";
		m_iPass = 12;
		break;
	case CDecal::DECAL_TS_F:
		wstrDecalID = L"Decal_TS_F";
		m_iPass = 13;
		break;
	case CDecal::DECAL_BOW_FALL:
	case CDecal::DECAL_ORB_RIGHT:
	case CDecal::DECAL_ORB_E_BEAM:
	case CDecal::DECAL_ORB_F:
		wstrDecalID = L"Decal_Bow_E_Fall";
		m_iPass = 13;
		break;
	case CDecal::DECAL_END:
		break;
	default:
		break;
	}
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	pComponent = m_pCubeCom = dynamic_cast<Engine::CCubeTex*>(Engine::CCubeTex::Create(m_pGraphicDev));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTexCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrDecalID));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);
	return S_OK;
}

HRESULT CDecal::Setup_ShaderProps(LPD3DXEFFECT & pEffect, const _double& dTimeDelta)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	_matrix	matWorld, matView, matProj;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	Engine::Get_CamView(&matView);
	Engine::Get_CamProj(&matProj);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	pEffect->SetMatrix("g_matProjInv", &matProj);
	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matWorldInv", &matWorld);
	m_pTexCom->SetTexture(pEffect, "g_CubeTexture");
	
	switch (m_eDecalType)
	{
	case CDecal::DECAL_SKILLTARGET:
		break;
	case CDecal::DECAL_ORBF:
		break;
	case CDecal::DECAL_ORBR:
		m_fDiminishRate -= _float(dTimeDelta * 0.5f);
		pEffect->SetFloat("fDiminishColor", m_fDiminishRate);
		break;
	case CDecal::DECAL_TS_F:
		//m_fDiminishRate -= _float(dTimeDelta * 0.5f);
		m_vUseMasking = _vec4(0.f, 0.f, 0.f, 1.f);
		Engine::SetTexture(pEffect, "g_MaskingTexture", 6);
		//pEffect->SetFloat("fDiminishColor", m_fDiminishRate);
		break;
	case CDecal::DECAL_BOW_FALL:
		break;
	case CDecal::DECAL_END:
		break;
	default:
		break;
	}
	pEffect->SetVector("vUseMasking", &m_vUseMasking);
	return S_OK;
}

HRESULT CDecal::Create_Effect(const _double& dTimeDelta)
{
	switch (m_eDecalType)
	{
	case CDecal::DECAL_SKILLTARGET:
		break;
	case CDecal::DECAL_ORBF:
		break;
	case CDecal::DECAL_ORBR:
		break;
	case CDecal::DECAL_TS_F:
		break;
	case CDecal::DECAL_BOW_FALL:
		Create_RainArrow(dTimeDelta);
		break;
	case CDecal::DECAL_ORB_RIGHT:
		Create_Orb_Right(dTimeDelta);
		break;
	case CDecal::DECAL_ORB_E_BEAM:
		Create_Orb_Beam(dTimeDelta);
		break;
	case CDecal::DECAL_ORB_F:
		Create_Orb_Wind(dTimeDelta);
		break;
	case CDecal::DECAL_ORB_ULT:
		Create_Orb_Ult(dTimeDelta);
		break;
	case CDecal::DECAL_ORB_FLYING:
		Create_Orb_Flying(dTimeDelta);
		break;
	case CDecal::DECAL_END:
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CDecal::Create_RainArrow(const _double& dTimeDelta)
{
	m_dEffectTerm -= dTimeDelta;
	if (m_dEffectTerm < 0.)
	{
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vCreatePos = _vec3(Engine::RandomFloatRange(-200.f, 200.f), 500.f, Engine::RandomFloatRange(-200.f, 200.f)) + *m_pDecalPos;
		pGameObject = CArrow::Create(m_pGraphicDev, CArrow::RAIN_ARROW, &vCreatePos, &_vec3(0.f, 0.f, 0.f), m_pDecalPos->y, _vec3(0.f, 0.f, 0.f), m_eControlType);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::GAMEOBJECT, L"Rain_Arrow", pGameObject);
		m_dEffectTerm = 0.05;
	}
	
	return S_OK;
}

HRESULT CDecal::Create_Orb_Right(const _double & dTimeDelta)
{
	m_dEffectTerm += dTimeDelta;
	Engine::CGameObject* pGameObject = nullptr;
	if (0 < m_dEffectTerm && m_dEffectTerm < 1.)
	{
		if (!m_bCreateEffect)
		{
			m_pTransformCom->Set_Pos(m_pDecalPos);
			m_pTransformCom->Update_Component(dTimeDelta);
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_TRAIL, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_TRAIL2, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
			pGameObject = COrbCore::Create(m_pGraphicDev, COrbCore::ORB_SPIN_UP_RIGHT, m_pDecalPos, m_pTransformCom->Get_Angle(), &_vec3(0.f, 0.f, 0.f), 2.2, m_eControlType);
			Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);
			pGameObject = COrbCore::Create(m_pGraphicDev, COrbCore::ORB_SPIN_UP_LEFT, m_pDecalPos, m_pTransformCom->Get_Angle(), &_vec3(0.f, 0.f, 0.f), 2.2, m_eControlType);
			Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);
			m_bCreateEffect = true;
		}
	}
	else if (1.< m_dEffectTerm && m_dEffectTerm < 2.)
	{
		if (m_bCreateEffect)
		{
			_vec3 vSecondEffectPos = *m_pDecalPos + _vec3(0.f, 100.f, 0.f);
			m_pTransformCom->Set_Pos(&vSecondEffectPos);
			m_pTransformCom->Update_Component(dTimeDelta);
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_TRAIL, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_TRAIL2, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
			m_pTransformCom->Set_Pos(m_pDecalPos);
			m_pTransformCom->Update_Component(dTimeDelta);
			m_bCreateEffect = false;
		}
	}
	else if (2.3 < m_dEffectTerm && m_dEffectTerm < 3.)
	{
		if (!m_bCreateEffect)
		{
			_vec3 vFlarePos = *m_pDecalPos + _vec3(0.f, 200.f, 0.f);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE, &vFlarePos);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE2, &vFlarePos);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE3, &vFlarePos);
			m_bCreateEffect = true;
		}
	}
	return S_OK;
}

HRESULT CDecal::Create_Orb_Beam(const _double & dTimeDelta)
{
	m_dEffectTerm -= dTimeDelta;
	if (m_dEffectTerm < 0.)
	{
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vCreatePos = _vec3(Engine::RandomFloatRange(-200.f, 200.f), 500.f, Engine::RandomFloatRange(-200.f, 200.f)) + *m_pDecalPos;
		pGameObject = COrbBeam::Create(m_pGraphicDev, &vCreatePos, m_pDecalPos->y, m_eControlType);
		Engine::Add_GameObject(Engine::GAMEOBJECT, L"Orb_Beam", pGameObject);
		m_dEffectTerm = 0.1;
	}
	return S_OK;
}

HRESULT CDecal::Create_Orb_Wind(const _double & dTimeDelta)
{
	m_dEffectTerm += dTimeDelta;
	if (!m_bCreateEffect && m_dEffectTerm < 1)
	{
		m_pTransformCom->Set_Pos(m_pDecalPos);
		m_pTransformCom->Update_Component(dTimeDelta);
		m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND2, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
		m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
		m_bCreateEffect = true;
	}
	if (m_dEffectTerm > 2)
	{
		if (m_bCreateEffect)
		{
			m_pTransformCom->Set_Angle(&m_vPlayerAngle);
			m_pTransformCom->Update_Component(dTimeDelta);
			_vec3 vRight = m_pTransformCom->Get_Info_byWorld(Engine::INFO_RIGHT);
			D3DXVec3Normalize(&vRight, &vRight);
			
			_vec3 vOrbCreaterPos1 = *m_pDecalPos + _vec3(0.f, 100.f, 0.f) - vRight * 200.f;
			_vec3 vOrbCreaterPos2 = *m_pDecalPos + _vec3(0.f, 120.f, 0.f) - vRight * 100.f;
			_vec3 vOrbCreaterPos3 = *m_pDecalPos + _vec3(0.f, 140.f, 0.f);
			_vec3 vOrbCreaterPos4 = *m_pDecalPos + _vec3(0.f, 120.f, 0.f) + vRight * 100.f;
			_vec3 vOrbCreaterPos5 = *m_pDecalPos + _vec3(0.f, 100.f, 0.f) + vRight * 200.f;
			_vec3 vOrbCreatorAngle = m_vPlayerAngle;
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR1, &vOrbCreaterPos1, &vOrbCreatorAngle, nullptr, m_eControlType);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR2, &vOrbCreaterPos1, &vOrbCreatorAngle, nullptr, m_eControlType);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR1, &vOrbCreaterPos2, &vOrbCreatorAngle, nullptr, m_eControlType);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR2, &vOrbCreaterPos2, &vOrbCreatorAngle, nullptr, m_eControlType);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR1, &vOrbCreaterPos3, &vOrbCreatorAngle, nullptr, m_eControlType);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR2, &vOrbCreaterPos3, &vOrbCreatorAngle, nullptr, m_eControlType);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR1, &vOrbCreaterPos4, &vOrbCreatorAngle, nullptr, m_eControlType);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR2, &vOrbCreaterPos4, &vOrbCreatorAngle, nullptr, m_eControlType);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR1, &vOrbCreaterPos5, &vOrbCreatorAngle, nullptr, m_eControlType);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR2, &vOrbCreaterPos5, &vOrbCreatorAngle, nullptr, m_eControlType);
			m_bCreateEffect = false;
		}
		
	}
	return S_OK;
}

HRESULT CDecal::Create_Orb_Ult(const _double & dTimeDelta)
{
	m_dEffectTerm += dTimeDelta;
	if (!m_bCreateEffect)
	{
		if (m_dEffectTerm > 2 && m_dEffectTerm <3)
		{
			_vec3 vPos = *m_pDecalPos + _vec3(0.f, 50.f, 0.f);
			m_pTransformCom->Set_Pos(m_pDecalPos);
			m_pTransformCom->Update_Component(dTimeDelta);
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND3, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND4, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
			_vec3 vUltPos = *m_pDecalPos + _vec3(0.f, 150.f, 0.f);
			//Engine::CGameObject* pGameObject = nullptr;
			//pGameObject = COrbCore::Create(m_pGraphicDev, COrbCore::ORB_BIG, &vPos, m_pTransformCom->Get_Angle(), &_vec3(0.f, 0.f, 0.f), 1, m_eControlType);
			//Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);
			//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORBULT1, &vUltPos);
			//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORBULT2, &vUltPos);
			//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORBULT3, &vUltPos);
			m_bCreateEffect = true;
		}
	}
	else
	{
		if (m_dEffectTerm > 4)
		{
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_ULT_WIND, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
			_vec3 vUltPos = *m_pDecalPos + _vec3(0.f, 150.f, 0.f);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE, &vUltPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_eControlType);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE2, &vUltPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_eControlType);
			m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE3, &vUltPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_eControlType);
			m_bCreateEffect = false;
		}
	}
	
	return S_OK;
}

HRESULT CDecal::Create_Orb_Flying(const _double & dTimeDelta)
{
	m_dEffectTerm += dTimeDelta;
	if (!m_bCreateEffect)
	{
		if (m_dEffectTerm > 1.5)
		{
			Engine::CGameObject* pGameObject = nullptr;
			// 이거 위치는 상대가 없으면 내 위치고 상대가 있으면 상대 위치임
			_vec3 vCreatePos = *m_pDecalPos;
			pGameObject = COrbBeam::Create(m_pGraphicDev, &vCreatePos, m_pDecalPos->y, m_eControlType);
			Engine::Add_GameObject(Engine::GAMEOBJECT, L"Orb_Beam", pGameObject);
			m_bCreateEffect = true;
		}
	}
	return S_OK;
}

void CDecal::Move(const _double & dTimeDelta)
{
	switch (m_eDecalType)
	{
	case CDecal::DECAL_SKILLTARGET:
		break;
	case CDecal::DECAL_ORBF:
		break;
	case CDecal::DECAL_ORBR:
		break;
	case CDecal::DECAL_TS_F:
		break;
	case CDecal::DECAL_BOW_FALL:
		m_pTransformCom->Rotation(Engine::ROT_Y, _float(10.f * dTimeDelta));
		break;
	case CDecal::DECAL_END:
		break;
	default:
		break;
	}
	m_pTransformCom->Set_Info(m_pDecalPos, Engine::INFO_POS);
}

CDecal * CDecal::Create(LPDIRECT3DDEVICE9 pGraphicDev, DECAL_TYPE eDecalType, _vec3 * pPos, _double dCountDown, _vec3 vPlayerAngle, CONTROLTYPE eControlType)
{
	CDecal* pInstance = new CDecal(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eDecalType, pPos, dCountDown, vPlayerAngle, eControlType)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDecal::Free()
{
	CBaseObject::Free();
}
