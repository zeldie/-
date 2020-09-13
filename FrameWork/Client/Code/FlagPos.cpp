#include "stdafx.h"
#include "FlagPos.h"
#include "RcTex.h"
#include "Player.h"

CFlagPos::CFlagPos(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNormalObject(pGraphicDev),
	m_eFlagPosType(FLAGPOSTYPE_END),
	m_pTargetTransformCom(nullptr),
	m_vScale(0.f, 0.f, 0.f),
	m_bFixLine(false)
{
	D3DXMatrixIdentity(&m_matProj);
}

CFlagPos::~CFlagPos()
{
}

HRESULT CFlagPos::Ready_GameObject(FLAGPOSTYPE eType, Engine::CTransform* pTargetTransform)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_eFlagPosType = eType;
	m_pTargetTransformCom = pTargetTransform;

	//if (BG == m_eFlagPosType)
	//	m_vScale = _vec3(50.f, 50.f, 1.f);
	//else if (FLAG == m_eFlagPosType)
	//	m_vScale = _vec3(20.f, 20.f, 1.f);

	//투영행렬로 좌표설정해줄거라 이미지 크기 더 작게!
	if (BG == m_eFlagPosType)
		m_vScale = _vec3(0.08f, 0.128f, 1.f); //0.1f, 0.15f
	else if (FLAG == m_eFlagPosType)
		m_vScale = _vec3(0.032f, 0.048f, 1.f); //0.04f, 0.06f,

	m_pTransformCom->Set_Scale(m_vScale.x, m_vScale.y, 1.f);

	return S_OK;
}

_int CFlagPos::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CheckFlagPos();

	CNormalObject::Update_GameObject(dTimeDelta);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f); //->직교투영 --> 이거 안쓰는중

	return Engine::NO_EVENT;
}

_int CFlagPos::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CFlagPos::Render_Geometry(const _double & dTimeDelta)
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

	pEffect->BeginPass(10);//5

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CFlagPos::CheckFlagPos()
{
	//월드 -> 투영 
	_vec3 vTargetPos;
	m_pTargetTransformCom->Get_Info(Engine::INFO_POS, &vTargetPos);
	vTargetPos.y += 250.f;

	_matrix matView, matProj;
	Engine::Get_CamView(&matView);
	Engine::Get_CamProj(&matProj);

	D3DXVec3TransformCoord(&vTargetPos, &vTargetPos, &matView);

	if (0.f >= vTargetPos.z)
		m_bFixLine = true;
	else
		m_bFixLine = false;

	D3DXVec3TransformCoord(&vTargetPos, &vTargetPos, &matProj);


	if (!m_bFixLine)
	{
		vTargetPos.x = Engine::Clamp(vTargetPos.x, -1.f, 1.f);
		vTargetPos.y = Engine::Clamp(vTargetPos.y, -1.f, 1.f);
		vTargetPos.z = Engine::Clamp(vTargetPos.z, 0.f, 1.f);
	}
	else
	{
		vTargetPos.x = Engine::Clamp(vTargetPos.x, -1.f, 1.f);
		vTargetPos.y = -1.f;
		vTargetPos.z = 0.3f;
	}

	_float fScaleX = 0.f;
	_float fScaleY = 0.f;
	if (BG == m_eFlagPosType)
	{
		fScaleX = m_vScale.x * 0.5f;
		fScaleY = m_vScale.y * 0.5f;
	}
	else if (FLAG == m_eFlagPosType)
	{
		fScaleX = m_vScale.x * 0.5f + 0.024f; //0.02f->깃발모양&네모 크기차이 (0.048f>>1)
		fScaleY = m_vScale.y * 0.5f + 0.04f; //0.04f->깃발모양&네모 크기차이 (0.08f>>1)

	}

	if (-1.f >= vTargetPos.x - fScaleX)
	{
		vTargetPos.x = -1.f + fScaleX;
	}
	
	if (1.f <= vTargetPos.x + fScaleX)
	{
		vTargetPos.x = 1.f - fScaleX;
	}

	if (1.f <= vTargetPos.y + fScaleY)
	{
		vTargetPos.y = 1.f - fScaleY;
	}

	if (-1.f >= vTargetPos.y - fScaleY)
	{
		vTargetPos.y = -1.f + fScaleY;
	}

	m_pTransformCom->Set_Pos(vTargetPos.x/* - (WINCX >> 1)*/, vTargetPos.y /*+ (WINCY >> 1)*/,vTargetPos.z);
}

HRESULT CFlagPos::Clone_Component()
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

	//Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_FlagPos"));
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

HRESULT CFlagPos::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	//CBaseObject::Set_ShaderMatrix(pEffect);
	//_matrix		matWorld, matView;

	//m_pTransformCom->Get_WorldMatrix(&matWorld);
	//D3DXMatrixIdentity(&matView); //직교투영

	//pEffect->SetMatrix("g_matWorld", &matWorld);
	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &m_matProj);

	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixIdentity(&matView); //직교투영

	//업데이트에서 투영좌표로 set_pos 해주니까 투영행렬은 항등행렬로!
	D3DXMatrixIdentity(&matProj); 

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vRGBA", &_vec4(1.f, 0.92f, 0.41f, 1.f));
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eFlagPosType);
	return S_OK;
}

CFlagPos * CFlagPos::Create(LPDIRECT3DDEVICE9 pGraphicDev, FLAGPOSTYPE eType, Engine::CTransform* pTargetTransform)
{
	CFlagPos* pInstance = new CFlagPos(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, pTargetTransform)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFlagPos::Free()
{
	CNormalObject::Free();
}
