#include "stdafx.h"
#include "KarmaOverHead.h"
#include "RcTex.h"

CKarmaOverHead::CKarmaOverHead(LPDIRECT3DDEVICE9 pGraphicDev)
	:CNormalObject(pGraphicDev),
	m_eKarmaType(KARMA_END),
	m_pTargetTransformCom(nullptr),
	m_vScale(0.f,0.f,0.f),
	m_dTime(0.5f),
	m_fAlpha(0.f),
	m_bTimeStart(false),
	m_bAlphaMinus(false),
	m_bCreateEffect(true)
{
}

CKarmaOverHead::~CKarmaOverHead()
{
}

HRESULT CKarmaOverHead::Ready_GameObject(CBasePlayer::PLAYER_WEAPONTYPE eTYpe, Engine::CTransform* pTargetTransform)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_eKarmaType = (KARMATYPE)(eTYpe - 1);
	m_pTargetTransformCom = pTargetTransform;


	m_vScale = _vec3(80.f, 80.f,1.f);

	_vec3 vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);
	vTargetPos.y += 170.f;

	m_pTransformCom->Set_Pos(&vTargetPos);
	Compute_ViewZ(&vTargetPos);
	
	m_vScale *= (m_fViewZ*0.001f);

	return S_OK;
}

_int CKarmaOverHead::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	//크기, 위치
	_vec3 vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);
	vTargetPos.y += 150.f;

	m_pTransformCom->Set_Pos(&vTargetPos);
	CNormalObject::Update_GameObject(dTimeDelta);
	
	BillBoard(&m_vScale);

	if (m_bTimeStart)
	{
		m_dTime -= dTimeDelta;
	}
	else
	{
		if (!m_bAlphaMinus)
		{
			m_fAlpha += (_float)dTimeDelta;

			//////effect ...안할래...... 
			//if (0.7f <= m_fAlpha)
			//{
			//	switch (m_eKarmaType)
			//	{
			//	case CKarmaOverHead::KARMA_DS:
			//		if (m_bCreateEffect)
			//		{
			//			CEffectMgr::GetInstance()->Create_TextureEffect(TEXTURE_WEAPONCHANGE_DS, &m_pTransformCom->m_vInfo[Engine::INFO_POS]);
			//			m_bCreateEffect = false;
			//			//Engine::CGameObject* pGameObject = COverHeadEffect::Create(m_pGraphicDev, COverHeadEffect::KARMA_DS,m_pTransformCom);
			//			//if (nullptr == pGameObject)
			//			//	break;
			//			//Engine::Add_GameObject(Engine::UI,L"OverHeadEffect",pGameObject);
			//		}
			//		break;
			//	case CKarmaOverHead::KARMA_TS:
			//		if (m_bCreateEffect)
			//		{
			//			CEffectMgr::GetInstance()->Create_TextureEffect(TEXTURE_WEAPONCHANGE_TS, &m_pTransformCom->m_vInfo[Engine::INFO_POS]);
			//			m_bCreateEffect = false;
			//		}
			//		break;
			//	case CKarmaOverHead::KARMA_ORB:
			//		if (m_bCreateEffect)
			//		{
			//			CEffectMgr::GetInstance()->Create_TextureEffect(TEXTURE_WEAPONCHANGE_ORB, &m_pTransformCom->m_vInfo[Engine::INFO_POS]);
			//			m_bCreateEffect = false;
			//		}
			//		break;
			//	case CKarmaOverHead::KARMA_LB:
			//		if (m_bCreateEffect)
			//		{
			//			CEffectMgr::GetInstance()->Create_TextureEffect(TEXTURE_WEAPONCHANGE_LB, &m_pTransformCom->m_vInfo[Engine::INFO_POS]);
			//			m_bCreateEffect = false;
			//		}
			//		break;
			//	}
			//}
			//////
		}
		else
			m_fAlpha -= (_float)dTimeDelta;

	}

	return Engine::NO_EVENT;
}

_int CKarmaOverHead::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	if (!m_bAlphaMinus)
	{
		if (1.f <= m_fAlpha)
		{
			m_fAlpha = 1.f;
			m_bTimeStart = true;
		}
	}
	else
	{
		if (0.f>= m_fAlpha)
		{
			m_fAlpha = 0.f;
			m_bTimeStart = true;
			m_bIsDead = true;
		}

	}


	if (0.f >= m_dTime)
	{
		m_dTime = 0.5f;
		m_bTimeStart = false;
		m_bAlphaMinus = true;
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CKarmaOverHead::Render_Geometry(const _double & dTimeDelta)
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

	pEffect->BeginPass(23);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CKarmaOverHead::Clone_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_KarmaOverHead"));
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

HRESULT CKarmaOverHead::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);	

	pEffect->SetVector("g_vRGBA",&_vec4(1.f,1.f,1.f,m_fAlpha));
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eKarmaType); //baseplayer에서 이넘값 0번이 common 이므로 1빼주야 잘맞음
	return S_OK;
}

CKarmaOverHead * CKarmaOverHead::Create(LPDIRECT3DDEVICE9 pGraphicDev, CBasePlayer::PLAYER_WEAPONTYPE eTYpe, Engine::CTransform* pTargetTransform)
{
	CKarmaOverHead* pInstance = new CKarmaOverHead(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eTYpe, pTargetTransform)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CKarmaOverHead::Free()
{
	CNormalObject::Free();
}
