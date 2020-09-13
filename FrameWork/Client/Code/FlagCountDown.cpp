#include "stdafx.h"
#include "FlagCountDown.h"
#include "RcTex.h"

CFlagCountDown::CFlagCountDown(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CNormalObject(pGraphicDev),
		m_dTime(0.f)
{
	D3DXMatrixIdentity(&m_matProj);
}

CFlagCountDown::~CFlagCountDown()
{
}

HRESULT CFlagCountDown::Ready_GameObject(TEXTYPE eType, _float _fX, _float _fY)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_dTime = 6.f;
	
	m_eTexType = eType;
	m_pTransformCom->Set_Pos(_fX - (WINCX >> 1), -_fY + (WINCY >> 1), 0.f);

	if (COUNTDOWN == m_eTexType)
		m_pTransformCom->Set_Scale(57.f, 57.f, 1.f);
	else if(TEXT == m_eTexType)
		m_pTransformCom->Set_Scale(95.f, 20.f, 1.f);

	return S_OK;
}

_int CFlagCountDown::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CNormalObject::Update_GameObject(dTimeDelta);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f); //->직교투영

	m_dTime -= dTimeDelta;
	return Engine::NO_EVENT;
}

_int CFlagCountDown::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CNormalObject::LateUpdate_GameObject(dTimeDelta);


	if (COUNTDOWN == m_eTexType)
	{		
		if (0.f >= m_dTime)
		{
			//깃발재생성시작 문구 ㄱㄱ 
			CUIMgr::GetInstance()->CreateReadyFlagPopUp(m_pGraphicDev);
			m_bIsDead = true;
			//m_dTime = 6.f;
		}

		wsprintf(m_szCountDown, L"%d", (_int)m_dTime);


		m_fRotAngle += (_float)dTimeDelta;

		m_pTransformCom->Rotation(Engine::ROT_Z, (_float)dTimeDelta*100.f);
	}
	else if (TEXT == m_eTexType)
	{
		if (0.f >= m_dTime)
		{
			m_bIsDead = true;
		}

		//위치 조절 test
		_vec3 vPos;

		m_pTransformCom->Get_Info(Engine::INFO_POS,&vPos);

		if(Engine::KeyDown(DIK_DOWN))
			m_pTransformCom->Set_Pos(&_vec3(vPos.x, vPos.y , vPos.z));
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;

}

void CFlagCountDown::Render_Geometry(const _double & dTimeDelta)
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
	if (COUNTDOWN == m_eTexType)
		pEffect->BeginPass(5);
	else
		pEffect->BeginPass(1);


	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);


	if (COUNTDOWN == m_eTexType)
		Engine::Render_Font(L"Font_GODIC3", m_szCountDown, &_vec2(636.f, 198.f), D3DXCOLOR(1.f, 0.92f, 0.41f, 1.f));
	
	
	////Engine::Render_Font(L"Font_GODIC2", L"깃발 재생성 준비", &_vec2(583.f, 162.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CFlagCountDown::Clone_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_FlagCountDown"));
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

HRESULT CFlagCountDown::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	//CBaseObject::Set_ShaderMatrix(pEffect);
	_matrix		matWorld, matView;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixIdentity(&matView); //직교투영

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	if (COUNTDOWN == m_eTexType)
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 0.92f, 0.41f, 1.f));

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture",m_eTexType);

	return S_OK;
}

CFlagCountDown * CFlagCountDown::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTYPE eType, _float _fX, _float _fY)
{
	CFlagCountDown* pInstance = new CFlagCountDown(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, _fX, _fY)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFlagCountDown::Free()
{
	CNormalObject::Free();
}
