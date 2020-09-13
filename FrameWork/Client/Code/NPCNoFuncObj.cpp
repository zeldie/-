#include "stdafx.h"
#include "NPCNoFuncObj.h"
#include "RcTex.h"

CNPCNoFuncObj::CNPCNoFuncObj(LPDIRECT3DDEVICE9 pGraphicDev)
	:CNormalObject(pGraphicDev),
	m_eNPCNoFuncObjType(NPCNoFuncObjTYPE_END),
	m_pTargetTransformCom(nullptr)
{
	D3DXMatrixIdentity(&m_matProj);
	//fx = 0.f;
	//fy= 0.f;
}


CNPCNoFuncObj::~CNPCNoFuncObj()
{
}

HRESULT CNPCNoFuncObj::Ready_GameObject(NPCNoFuncObjTYPE eType, CQuestMgr::NPCTYPE eNPCType, _bool bRender, _vec3 vPos, _vec3 vSize, Engine::CTransform* pTargetTransform)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_eNPCNoFuncObjType = eType;
	m_pTargetTransformCom = pTargetTransform;
	m_eNPCType = eNPCType;

	if (nullptr == m_pTargetTransformCom) //직교투영
	{
		m_pTransformCom->Set_Pos(vPos.x - (WINCX >> 1), -vPos.y + (WINCY >> 1), 0.f);
		m_pTransformCom->Set_Scale(vSize.x, vSize.y, vSize.z);
	}
	else// y축빌보드--> 느낌표
	{
		_vec3 vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);
		vTargetPos.y += 130.f;

		m_pTransformCom->Set_Pos(&vTargetPos);		
		m_vScale = vSize;
	}

	m_bRender = bRender;

	return S_OK;
}

_int CNPCNoFuncObj::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CNormalObject::Update_GameObject(dTimeDelta);
	
	if (nullptr == m_pTargetTransformCom)
		D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f); //->직교투영
	else
	{
		//y축 빌보드
		_matrix		matWorld, matView, matBill, matProj;		

		Engine::Get_CamView(&matView);

		D3DXMatrixIdentity(&matBill);
		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		D3DXMatrixInverse(&matBill, NULL, &matBill);

		_vec3 vPos;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

		_matrix		matScale, matTrans;
		D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y , vPos.z);

		D3DXMatrixIdentity(&matWorld);
		matWorld = matScale * matBill * matTrans;
		m_pTransformCom->Set_Matrix(&matWorld);

	}

	return Engine::NO_EVENT;
}

_int CNPCNoFuncObj::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CNPCNoFuncObj::Render_Geometry(const _double & dTimeDelta)
{
	if (m_bRender)
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

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

HRESULT CNPCNoFuncObj::Clone_Component()
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_NPCNoFuncObj"));
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

HRESULT CNPCNoFuncObj::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	//CBaseObject::Set_ShaderMatrix(pEffect);

	if (nullptr == m_pTargetTransformCom)
	{
		_matrix		matWorld, matView;

		m_pTransformCom->Get_WorldMatrix(&matWorld);
		D3DXMatrixIdentity(&matView); //직교투영

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &m_matProj);
	}
	else
	{
		CBaseObject::Set_ShaderMatrix(pEffect);
	}

	//pEffect->set
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eNPCNoFuncObjType);

	return S_OK;
}

CNPCNoFuncObj * CNPCNoFuncObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, NPCNoFuncObjTYPE eType, CQuestMgr::NPCTYPE eNPCType, _bool bRender, _vec3 vPos, _vec3 vSize, Engine::CTransform* pTargetTransform)
{
	CNPCNoFuncObj* pInstance = new CNPCNoFuncObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, eNPCType, bRender, vPos, vSize, pTargetTransform)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CNPCNoFuncObj::Free()
{
	CNormalObject::Free();
}
