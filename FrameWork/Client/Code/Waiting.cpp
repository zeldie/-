#include "stdafx.h"
#include "Waiting.h"
#include "RcTex.h"
#include "BossStage.h"

CWaiting::CWaiting(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CNormalObject(pGraphicDev),
		m_fFrame(0.f),
		m_fRotAngle(0.f),
		m_bRenderWaiting(false),
		m_dTime(0.f),
		m_bSceneChange(false)
{
}

CWaiting::~CWaiting()
{
}

HRESULT CWaiting::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(50.f, 50.f, 1.f);
	return S_OK;
}

_int CWaiting::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CNormalObject::Update_GameObject(dTimeDelta);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f); //->직교투영

	if (m_bRenderWaiting)
	{
		// 5초 흐르면 다음 스테이지로 넘어가게!
		m_dTime += dTimeDelta;
		if (5.f <= m_dTime)
		{		
			m_bSceneChange = true;
		}

	}
	return Engine::NO_EVENT;
}

_int CWaiting::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);
	
	if (m_bRenderWaiting)
	{
		m_fFrame += (_float)dTimeDelta;
		m_fRotAngle += (_float)dTimeDelta;

		if (3.f <= m_fFrame)
		{
			m_fFrame = 0.f;
		}

		m_pTransformCom->Rotation(Engine::ROT_Z, (_float)dTimeDelta*200.f);

		////위치 조절 test
		//_vec3 vPos;
		//m_pTransformCom->Get_Info(Engine::INFO_POS,&vPos);

		//if(Engine::KeyDown(DIK_DOWN))
		//	m_pTransformCom->Set_Pos(&_vec3(vPos.x+10.f, vPos.y, vPos.z));
		
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	}

	return Engine::NO_EVENT;

}

void CWaiting::Render_Geometry(const _double & dTimeDelta)
{
	if (m_bRenderWaiting)
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

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

HRESULT CWaiting::Clone_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Waiting"));
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

HRESULT CWaiting::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	//CBaseObject::Set_ShaderMatrix(pEffect);
	_matrix		matWorld, matView;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixIdentity(&matView); //직교투영

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	pEffect->SetVector("g_vRGBA", &_vec4(0.3f, 0.59f, 1.f, 0.81f));
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture",(int)m_fFrame);

	return S_OK;
}

CWaiting * CWaiting::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _fX, _float _fY)
{
	CWaiting* pInstance = new CWaiting(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	pInstance->m_pTransformCom->Set_Pos(_fX - (WINCX >> 1), -_fY + (WINCY >> 1), 0.f);
	
	return pInstance;
}

void CWaiting::Free()
{
	CNormalObject::Free();
}
