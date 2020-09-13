#include "stdafx.h"
#include "RunObj_Item.h"


CRunObj_Item::CRunObj_Item(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBaseObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_eSect(SECT_1)
	, m_eLineType(LINE_BOTTOM_MID)
	, m_eObjCollType(OBJ_COLL_END)
	, m_dProgress(0.)
	, m_dRadius(0.)
	, m_eItemType(RUNITEM_MONEY)
{
}

CRunObj_Item::~CRunObj_Item()
{
}

HRESULT CRunObj_Item::Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _vec3* vSelfRotate, RUN_LINE_SECT eSect,
	LINETYPE eLineType, RUNGAMEOBJECT_COLL eCollType, _double dProgress, _double dRadius, _float fUpPos, RUNGAME_ITEMTYPE eItemType)
{
	if (FAILED(Clone_Component(wstrObjectKey)))
		return E_FAIL;

	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_SelfRotAngle(vSelfRotate);
	m_pTransformCom->Update_Component(0.0);
	_vec3 vDir;
	D3DXVec3Normalize(&vDir,&m_pTransformCom->m_vInfo[Engine::INFO_UP]);
	vDir *= fUpPos;
	m_pTransformCom->Move_Pos(&vDir, 1.0, 1.0);

	m_eSect = eSect;
	m_eLineType = eLineType;
	m_eObjCollType = eCollType;
	m_dProgress = dProgress;
	m_dRadius = dRadius;
	m_eItemType = eItemType;

	return S_OK;
}

_int CRunObj_Item::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CBaseObject::Update_GameObject(dTimeDelta);


	////yÃà ºôº¸µå
	//_matrix		matWorld, matView, matBill, matProj;

	//Engine::Get_CamView(&matView);

	//D3DXMatrixIdentity(&matBill);
	//matBill._11 = matView._11;
	//matBill._13 = matView._13;
	//matBill._31 = matView._31;
	//matBill._33 = matView._33;

	//D3DXMatrixInverse(&matBill, NULL, &matBill);

	//_vec3 vPos;
	//m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	//_matrix		matScale, matTrans;
	//D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	//D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	//D3DXMatrixIdentity(&matWorld);
	//matWorld = matScale * matBill * matTrans;
	//m_pTransformCom->Set_Matrix(&matWorld);



	return Engine::NO_EVENT;
}

_int CRunObj_Item::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Compute_ViewZ(m_pTransformCom->Get_Info(Engine::INFO_POS));

	return Engine::NO_EVENT;
}

void CRunObj_Item::Render_Geometry(const _double & dTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);	
}

void CRunObj_Item::Set_Collision(RUNPLAYER_POWER eColl)
{
	m_bIsDead = true;

	// ÀÌÆåÆ®
	m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_RUNGAME_ITEM_LOOT, &(m_pTransformCom->m_vInfo[Engine::INFO_POS]), &m_pTransformCom->m_vAngle);
}

HRESULT CRunObj_Item::Clone_Component(wstring wstrObjectKey)
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrObjectKey));
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

HRESULT CRunObj_Item::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	//// °´Ã¼ÀÇ ±âº» ÅØ½ºÃÄ
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", 0);

	return S_OK;
}

CRunObj_Item * CRunObj_Item::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, _vec3* vSelfRotate, RUN_LINE_SECT eSect,
	LINETYPE eLineType, RUNGAMEOBJECT_COLL eCollType, _double dProgress, _double dRadius, _float fUpPos, RUNGAME_ITEMTYPE eItemType)
{
	CRunObj_Item* pInstance = new CRunObj_Item(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(wstrObjectKey, pPos, pAngle, pScale, vSelfRotate, eSect, eLineType, eCollType, dProgress, dRadius, fUpPos, eItemType)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRunObj_Item::Free()
{
	CBaseObject::Free();
}
