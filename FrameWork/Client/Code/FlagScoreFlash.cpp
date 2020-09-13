#include "stdafx.h"
#include "FlagScoreFlash.h"
#include "RcTex.h"

CFlagScoreFlash::CFlagScoreFlash(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNormalObject(pGraphicDev),
	m_eFlashSequenceType(FLASHSEQUENCETYPE_END),
	m_dSize(0.f),
	m_dTime(0.f),
	m_bStart(false)
{
	D3DXMatrixIdentity(&m_matProj);
}

CFlagScoreFlash::~CFlagScoreFlash()
{
}

HRESULT CFlagScoreFlash::Ready_GameObject(FLASHSEQUENCETYPE eType, _float fX, _float fY)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_dSize = 0.f;

	m_pTransformCom->Set_Pos(fX - (WINCX >> 1), -fY + (WINCY >> 1), 0.f);
	m_pTransformCom->Set_Scale(_float(m_dSize), _float(m_dSize), 1.f);

	m_eFlashSequenceType = eType;
	return S_OK;
}

_int CFlagScoreFlash::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CNormalObject::Update_GameObject(dTimeDelta);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f); //->직교투영

	return Engine::NO_EVENT;
}

_int CFlagScoreFlash::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);

	if (m_bStart)
	{
		m_dTime += dTimeDelta;
		if (FIRST == m_eFlashSequenceType)
		{
			m_dSize += dTimeDelta*100.f;
			if (130.f <= m_dSize)
			{
				m_dSize = 0.f;
				m_dTime = 0.f;
				m_bStart = false;
			}
		}
		else if (SECOND == m_eFlashSequenceType)
		{
			if (1.f <= m_dTime)
			{
				m_dSize += dTimeDelta*100.f;
				if (110.f <= m_dSize)
				{
					m_dSize = 0.f;
					m_dTime = 0.f;
					CUIMgr::GetInstance()->SetStartFlagFlash();
				}
			}
		}

		m_pTransformCom->Set_Scale(_float(m_dSize), _float(m_dSize),1.f);

	}

	////위치 조절 test
	//_vec3 vPos;
	//m_pTransformCom->Get_Info(Engine::INFO_POS,&vPos);

	//if (Engine::KeyDown(DIK_DOWN))
	//{
	//	m_pTransformCom->Set_Pos(&_vec3(vPos.x , vPos.y + 1.f, vPos.z));

	//_vec3 vPos2;
	//m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos2);

	////cout << vPos2.x << '\t' << vPos2.y << endl;
	//}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CFlagScoreFlash::Render_Geometry(const _double & dTimeDelta)
{
	if (m_bStart)
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

HRESULT CFlagScoreFlash::Clone_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_FlagScore"));
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

HRESULT CFlagScoreFlash::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	//CBaseObject::Set_ShaderMatrix(pEffect);
	_matrix		matWorld, matView;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixIdentity(&matView); //직교투영

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	pEffect->SetVector("g_vRGBA", &_vec4(0.28f, 0.96f, 0.96f, 0.6f));  //0.07f, 0.98f, 0.99f, 0.2f
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", 4);

	return S_OK;
}

CFlagScoreFlash * CFlagScoreFlash::Create(LPDIRECT3DDEVICE9 pGraphicDev, FLASHSEQUENCETYPE eType, _float fX, _float fY)
{
	CFlagScoreFlash* pInstance = new CFlagScoreFlash(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFlagScoreFlash::Free()
{
	CNormalObject::Free();
}
