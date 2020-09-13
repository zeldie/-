#include "stdafx.h"
#include "Karma.h"
#include "ScreenTex.h"
#include "PlayerObserver.h"

CKarma::CKarma(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eKarmaType(KARMA_END),
	m_eKarmaPositionType(KARMAPOSITION_END),
	m_bChoose(false),
	m_bEraseSwapUI(false)
{
}


CKarma::~CKarma()
{
}

HRESULT CKarma::Ready_GameObject(SELECTKARMATYPE eSelectType, KARMAPOSITION eType, KARMATYPE eKarmaType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eSelectKarmaType = eSelectType;
	m_eKarmaPositionType = eType;
	m_eKarmaType = eKarmaType;

	//��������û
	m_pPlayerObserver = CPlayerObserver::Create();
	if (nullptr == m_pPlayerObserver)
		return E_FAIL;
	Engine::Subscribe(Engine::PLAYER, m_pPlayerObserver);

	return S_OK;
}

_int CKarma::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	
	if (SELECTKARMATYPE_FORSWAP != m_eSelectKarmaType)
	{
		if (MAIN == m_eKarmaPositionType)
		{
			m_eKarmaType = (KARMATYPE)(m_pPlayerObserver->Get_MainWeapon()); //baseplayer���� �̳Ѱ� 0���� common �̹Ƿ� 1���־� �߸���
		}
		else if (SUB == m_eKarmaPositionType)
		{
			m_eKarmaType = (KARMATYPE)(m_pPlayerObserver->Get_SubWeapon());//baseplayer���� �̳Ѱ� 0���� common �̹Ƿ� 1���־� �߸���
		}
	}	

	return Engine::NO_EVENT;
}

_int CKarma::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	PickingKarma();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CKarma::Render_Geometry(const _double & dTimeDelta)
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

	if(SELECTKARMATYPE_FORRESULT ==  m_eSelectKarmaType)
		pEffect->BeginPass(1);
	else
	{
		if (m_bChoose)
		{
			pEffect->BeginPass(5);
		}
		else
		{
			pEffect->BeginPass(1);
		}
	}
	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CKarma::PickingKarma()
{	
	if (SELECTKARMATYPE_FORBASIC == m_eSelectKarmaType)
	{
		_vec3 vMousePos;
		if (m_pScreenTexBufferCom->CheckAroundMouse(g_hWnd, &vMousePos))
			m_bChoose = true;
		else
			m_bChoose = false;

		if (m_bChoose && Engine::MouseUp(Engine::DIM_LB))
		{
			CUIMgr::GetInstance()->CreateSwapKarmaUI(m_pGraphicDev, m_eKarmaPositionType);			
		}
	}
	else if (SELECTKARMATYPE_FORSWAP == m_eSelectKarmaType)
	{
		_vec3 vMousePos;
		if (m_pScreenTexBufferCom->CheckAroundMouse(g_hWnd, &vMousePos))
		{
			m_bChoose = true;
			m_bEraseSwapUI = false;
		}
		else
			m_bChoose = false;

		if (m_bChoose && Engine::MouseUp(Engine::DIM_LB))
		{
			CheckSwapKarma();			
		}
		else if (!m_bChoose && Engine::MouseDown(Engine::DIM_LB))
		{
			m_bEraseSwapUI = true;
		}
	}
}

void CKarma::CheckSwapKarma()
{
	CBasePlayer* pPlayer = dynamic_cast<CBasePlayer*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"RealPlayer"));
	
	if (MAIN == m_eKarmaPositionType)
	{
		//üũ�� �ϰ���? (�������� �����̶��) -> ���� ������ ���꿡 �ִ°�
		//���꿡 �ִ� -> ���긦 ��������, ������ �����
		//���꿡 ���� -> ���� ���� ��������, ����� �״��

		//1.
		if (CUIMgr::GetInstance()->CheckMyKarmaPosition(m_eKarmaPositionType,m_eKarmaType))
		{
			KARMATYPE eBuffKarma = KARMATYPE(m_pPlayerObserver->Get_MainWeapon());
			//���꿡 �ִ�

			//����->����
			pPlayer->Set_SubWeapon(eBuffKarma);

			//����->����
			pPlayer->Set_MainWeapon(m_eKarmaType);

			//
			CUIMgr::GetInstance()->Set_PlayerMainWeapon(m_eKarmaType);
			CUIMgr::GetInstance()->Set_PlayerSubWeapon(eBuffKarma);
	
			//���Ҹ���Ʈ �ٽñ׸���
			CUIMgr::GetInstance()->UpdateSwapKarmaUI(m_eKarmaPositionType, m_eKarmaType);
			CUIMgr::GetInstance()->UpdateSwapKarmaUI(SUB, eBuffKarma);


		}
		else
		{			
			//3. ���꿡 ����
			//3-1. ���� ���� ����. ����� �״��.			
			pPlayer->Set_MainWeapon(m_eKarmaType);

			CUIMgr::GetInstance()->Set_PlayerMainWeapon(m_eKarmaType);
	
			//���Ҹ���Ʈ �ٽñ׸���
			CUIMgr::GetInstance()->UpdateSwapKarmaUI(m_eKarmaPositionType, m_eKarmaType);
			//
		}

	}
	else if (SUB == m_eKarmaPositionType)
	{
		//üũ�� �ϰ���? (�������� ������) -> ���� ������ ���ο� �ִ°�
		//���ο� �ִ� ->	���θ� ��������, ������ ���η�
		//���ο� ���� ->	���� ���� ��������, ���δ� �״��
		//1.
		if (CUIMgr::GetInstance()->CheckMyKarmaPosition(m_eKarmaPositionType,m_eKarmaType))
		{
			KARMATYPE eBuffKarma = KARMATYPE(m_pPlayerObserver->Get_SubWeapon());

			//2. ���ο� �ִ�
			//2-2. ����->����
			pPlayer->Set_MainWeapon(eBuffKarma);
			//2-1. ����->����
			pPlayer->Set_SubWeapon(m_eKarmaType);
			//
			CUIMgr::GetInstance()->Set_PlayerMainWeapon(eBuffKarma);
			CUIMgr::GetInstance()->Set_PlayerSubWeapon(m_eKarmaType);

			//���Ҹ���Ʈ �ٽñ׸���
			CUIMgr::GetInstance()->UpdateSwapKarmaUI(m_eKarmaPositionType, m_eKarmaType);
			CUIMgr::GetInstance()->UpdateSwapKarmaUI(MAIN, eBuffKarma);
			

		}
		else
		{
			//3. ���ο� ����
			//3-1. ���� ���� ��������, ���δ� �״��
			pPlayer->Set_SubWeapon(m_eKarmaType);
			CUIMgr::GetInstance()->Set_PlayerSubWeapon(m_eKarmaType);

			//���Ҹ���Ʈ �ٽñ׸���
			CUIMgr::GetInstance()->UpdateSwapKarmaUI(m_eKarmaPositionType, m_eKarmaType);

			//
		}

	}

}

HRESULT CKarma::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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
	pComponent = m_pScreenTexBufferCom = Engine::CScreenTex::Create(m_pGraphicDev,fX, fY, fSizeX, fSizeY, fViewZ);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Karma"));
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

HRESULT CKarma::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);		
	
	if (SELECTKARMATYPE_FORRESULT != m_eSelectKarmaType && m_bChoose) //���콺��ŷ�˻��߿� ����ٲ��..
	{
		switch (m_eKarmaType)
		{
		case CKarma::KARMA_DS:
			pEffect->SetVector("g_vRGBA", &_vec4(0.52f,0.31f,1.f,1.f));
			break;
		case CKarma::KARMA_TS:
			pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 0.f, 1.f));
			break;
		case CKarma::KARMA_ORB:
			pEffect->SetVector("g_vRGBA", &_vec4(0.039f,1.f,0.67f,1.f));
			break;
		case CKarma::KARMA_LB:
			pEffect->SetVector("g_vRGBA", &_vec4(0.30f,1.f,0.15f,1.f));
			break;
		}
	}

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eKarmaType);


	return S_OK;
}

CKarma * CKarma::Create(LPDIRECT3DDEVICE9 pGraphicDev, SELECTKARMATYPE eSelectType, KARMAPOSITION eType, KARMATYPE eKarmaType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CKarma* pInstance = new CKarma(pGraphicDev);	

	if (FAILED(pInstance->Ready_GameObject(eSelectType, eType, eKarmaType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CKarma::Free()
{
	CUIObject::Free();
	
	Engine::UnSubscribe(Engine::PLAYER, m_pPlayerObserver);
	Engine::Safe_Release(m_pPlayerObserver);

}
