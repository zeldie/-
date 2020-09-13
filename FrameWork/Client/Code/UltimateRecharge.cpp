#include "stdafx.h"
#include "UltimateRecharge.h"
#include "PlayerObserver.h"
#include "UltimateSlot.h"

CUltimateRecharge::CUltimateRecharge(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CBaseObject(pGraphicDev),
		m_pUltimateBuffer(nullptr),
		m_pTextureCom(nullptr),
		m_pPlayerObserver(nullptr),
		m_bLateInit(true),
		m_iTextureRage(0),
		m_bFull(false),
		m_bCreate(true),
		m_pUIMgr(CUIMgr::GetInstance())
{
	D3DXMatrixIdentity(&m_matProj);
}


CUltimateRecharge::~CUltimateRecharge()
{
}

void CUltimateRecharge::Set_Full(_bool bBool)
{
	m_bFull = bBool;
	m_iTextureRage = 0;
}

HRESULT CUltimateRecharge::Ready_GameObject(_float fX, _float fY)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(112.f, 113.f, 1.f);
	m_pTransformCom->Set_Pos(fX - (WINCX >> 1), -fY + (WINCY >> 1), 0.f);

	//옵저버신청
	m_pPlayerObserver = CPlayerObserver::Create();
	if (nullptr == m_pPlayerObserver)
		return E_FAIL;
	Engine::Subscribe(Engine::PLAYER, m_pPlayerObserver);

	return S_OK;
}

_int CUltimateRecharge::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	if (m_bLateInit)
	{
		m_dbMaxRage = m_pPlayerObserver->Get_MaxRage();
		m_bLateInit = false;
	}

	CBaseObject::Update_GameObject(dTimeDelta);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f); //->직교투영

	return Engine::NO_EVENT;
}

_int CUltimateRecharge::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseObject::LateUpdate_GameObject(dTimeDelta);

	if (!m_bFull)
	{
		_double dbRage = m_pPlayerObserver->Get_Rage();
		m_iTextureRage = dbRage / m_dbMaxRage* 34;

		if (34 <= m_iTextureRage)
		{			
			m_bFull = true;		

			if (m_bCreate)
			{
				m_pUIMgr->SkillCoolDownCheck(Engine::KEYGUIDE_Q);
				m_bCreate = false;
			}

		}

	}
	else
	{
		m_iTextureRage = 34;
	}



	/////////////////////////////////////////
	////위치 조절 test
	//_vec3 vPos;
	//m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	//float itest = 1.f;

	//if (Engine::KeyDown(DIK_RIGHT))
	//	m_pTransformCom->Set_Pos(&_vec3(vPos.x + 1.f, vPos.y, vPos.z));

	//if (Engine::KeyDown(DIK_LEFT))
	//	m_pTransformCom->Set_Pos(&_vec3(vPos.x - 1.f, vPos.y, vPos.z));

	//if (Engine::KeyDown(DIK_UP))
	//	m_pTransformCom->Set_Pos(&_vec3(vPos.x , vPos.y + 1.f, vPos.z));

	//if (Engine::KeyDown(DIK_DOWN))
	//	m_pTransformCom->Set_Pos(&_vec3(vPos.x , vPos.y - 1.f, vPos.z));

	//if (Engine::KeyDown(DIK_F5))
	//	m_pTransformCom->Set_Scale(m_pTransformCom->m_vScale.x + -itest, m_pTransformCom->m_vScale.x + -itest, 1.f);
	//if (Engine::KeyDown(DIK_F6))
	//	m_pTransformCom->Set_Scale(m_pTransformCom->m_vScale.x + itest, m_pTransformCom->m_vScale.x + itest, 1.f);

	//_vec3 vPos2;
	//m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos2);
	//_vec3 vScale = *m_pTransformCom->Get_Scale();
	////cout << vPos2.x << '\t' << vPos2.y << '\t' << vScale.x << '\t' << endl;
	/////////////////////////////////////////////

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CUltimateRecharge::Render_Geometry(const _double & dTimeDelta)
{
	////////////////SHADER/////////////
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(12); //

	m_pUltimateBuffer->Render_Buffer(m_iTextureRage);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CUltimateRecharge::Clone_Component()
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

	pComponent = m_pUltimateBuffer = Engine::CUltimateBuffer::Create(m_pGraphicDev);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_UltimateRecharge"));
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

HRESULT CUltimateRecharge::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	//CBaseObject::Set_ShaderMatrix(pEffect);	
	_matrix		matWorld, matView;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixIdentity(&matView); //직교투영	

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");

	return S_OK;
}

CUltimateRecharge * CUltimateRecharge::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY)
{
	CUltimateRecharge* pInstance = new CUltimateRecharge(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(fX, fY)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CUltimateRecharge::Free()
{
	CBaseObject::Free();

	Engine::UnSubscribe(Engine::PLAYER, m_pPlayerObserver);
	Engine::Safe_Release(m_pPlayerObserver);
}
