#include "stdafx.h"
#include "MonsterHpMgr.h"
#include "ScreenTex.h"
#include "Belatos.h"
#include "MonsterHp.h"
#include "HpNumber.h"
#include "ApostleObserver.h"

CMonsterHpMgr::CMonsterHpMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUIObject(pGraphicDev),
	m_pMonster(nullptr),
	m_bLateInit(true),
	m_fMaxMonsterHp(0.f),
	m_fMonsterHp(0.f),
	m_fGiveNextHp(0.f),
	m_iLineCnt(0),
	m_bSwap(false),
	m_iSwapCnt(0),
	m_iDigitCnt(1),
	m_pApostleObserver(nullptr)
{
	m_vecMonsterHp.reserve(2);
	m_vecHpNumber.reserve(4);
}


CMonsterHpMgr::~CMonsterHpMgr()
{
	//충돌용
}

HRESULT CMonsterHpMgr::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	//보스, 탄막 공동 
	m_iLineCnt = 100;
	m_eUIType = CUIMgr::GetInstance()->Get_UIType();

	if (CUIMgr::UITYPE_SHOOTING == m_eUIType)
	{
		//탄막 옵저버 신청
		m_pApostleObserver = CApostleObserver::Create();
		if (nullptr == m_pApostleObserver)
			return E_FAIL;
		Engine::Subscribe(Engine::APOSTLE, m_pApostleObserver);
	}

	return S_OK;
}

_int CMonsterHpMgr::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	if (m_bLateInit)
	{ //스테이지별로 나눠서 초기화(UIType별)
		if (CUIMgr::UITYPE_SHOOTING == m_eUIType)
		{
			m_pMonster = nullptr;

			m_fMaxMonsterHp = (_float)m_pApostleObserver->Get_MaxHp();
		}
		else if (CUIMgr::UITYPE_BOSS == m_eUIType)
		{
			m_pMonster = dynamic_cast<CBelatos*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Belatos"));

			m_pApostleObserver = nullptr;

			m_fMaxMonsterHp = (_float)m_pMonster->Get_BaseInfo()->iMaxHp;
		}

		CreateMonsterHp();
		CreateHpNumber(m_iLineCnt);

		m_bLateInit = false;
	}

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CMonsterHpMgr::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	//monsterhp에 값을 전달
	if (CUIMgr::UITYPE_SHOOTING == m_eUIType)
	{
		m_fMonsterHp = (_float)m_pApostleObserver->Get_Hp();
	}
	else if (CUIMgr::UITYPE_BOSS == m_eUIType)
	{
		if (nullptr == m_pMonster)
			return Engine::NO_EVENT;

		m_fMonsterHp = (_float)m_pMonster->Get_BaseInfo()->iHp;
	}


	if (m_fMaxMonsterHp <= m_fMonsterHp)
		m_fMonsterHp = m_fMaxMonsterHp;

	if (0.f >= m_fMonsterHp)
	{
		m_fMonsterHp = 0.f;
		for (auto& pMonsterHp : m_vecMonsterHp)
		{
			pMonsterHp->Set_HpIsZero();
		}

		for (auto& pHpNum : m_vecHpNumber)
		{
			pHpNum->Set_LineCnt(0);
		}

		return Engine::NO_EVENT;
	}



	_float fCnt = 0.f;
	for (auto& pMonsterHp : m_vecMonsterHp)
	{
		if (m_bSwap)
		{
			if (!pMonsterHp->Get_Swap() && !pMonsterHp->Get_Working())
			{
				++m_iSwapCnt;
				////숫자 뜨는거 숫자 조정////
				m_iLineCnt -= 1;
				if (0 >= m_iLineCnt)
					m_iLineCnt = 0;

				for (auto& pHpNum : m_vecHpNumber)
				{
					pHpNum->Set_LineCnt(m_iLineCnt);
				}
				///////////////////////////////
				pMonsterHp->Set_Working(true);
				fCnt = m_iSwapCnt * pMonsterHp->Get_MaxHpPerLine();
				pMonsterHp->Set_MonsterDamage(m_fMaxMonsterHp - m_fMonsterHp - (fCnt/*스왑횟수 * m_fMaxHpPerLine */)+m_fGiveNextHp);
				pMonsterHp->Set_VertexZ();
				pMonsterHp->Set_ChageColor();
				m_bSwap = false;
			}
		}
		else
		{
			if (pMonsterHp->Get_Working())
			{
				fCnt = m_iSwapCnt * pMonsterHp->Get_MaxHpPerLine();
				pMonsterHp->Set_MonsterDamage(m_fMaxMonsterHp - m_fMonsterHp - (fCnt/*스왑횟수 * m_fMaxHpPerLine */)); //깍인 양(데미지)을 전달
			}
			else
			{
				pMonsterHp->Set_Swap(false);
			}
		}
	}

	//if (Engine::KeyDown(DIK_0))
	//{
	//	//if (m_bSwap)
	//		////cout << "Swap True" << endl;

	//	////cout << m_fMonsterHp << '\t' << " 다음 넘어갈 양 : " << m_fGiveNextHp << endl;
	//}

	return Engine::NO_EVENT;
}

void CMonsterHpMgr::Render_Geometry(const _double & dTimeDelta)
{
	////SHADER
	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//if (pEffect == nullptr)
	//	return;
	//Engine::Safe_AddRef(pEffect);

	//if (FAILED(Setup_ShaderProps(pEffect)))
	//	return;

	//_uint iPassMax = 0;

	//pEffect->Begin(&iPassMax, 0);
	//pEffect->BeginPass(1);

	//m_pScreenTexBufferCom->Render_Buffer();

	//pEffect->EndPass();
	//pEffect->End();

	//Engine::Safe_Release(pEffect);
}

void CMonsterHpMgr::CreateMonsterHp()
{
	Engine::CGameObject* pGameObject = nullptr;

	_float fHpPerLine = m_fMaxMonsterHp / m_iLineCnt;
	pGameObject = CMonsterHp::Create(m_pGraphicDev, CMonsterHp::FIRSTHP, true, fHpPerLine, 484.f, 83.9f, 317.f, 12.f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"MonsterHp", pGameObject);
	CUIMgr::GetInstance()->AddCurUI(pGameObject);
	m_vecMonsterHp.emplace_back(dynamic_cast<CMonsterHp*>(pGameObject));

	pGameObject = CMonsterHp::Create(m_pGraphicDev, CMonsterHp::SECONDHP, false, fHpPerLine, 484.f, 83.9f, 317.f, 12.f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"MonsterHp", pGameObject);
	CUIMgr::GetInstance()->AddCurUI(pGameObject);
	m_vecMonsterHp.emplace_back(dynamic_cast<CMonsterHp*>(pGameObject));

}

void CMonsterHpMgr::CreateHpNumber(_uint iLineCnt)
{
	// 끝숫자 위치 :  793.5f, 87.9f, 20.f, 20.f 
	CalculateDigitCnt(iLineCnt);

	if (2 == m_iDigitCnt)
	{
		_uint iUnits, iTens = 0;
		iUnits = (_uint)iLineCnt % 10;
		iTens = (_uint)iLineCnt / 10;

		CGameObject* pGameObject = nullptr;
		pGameObject = CHpNumber::Create(m_pGraphicDev, CHpNumber::UNITS, m_iLineCnt, iUnits);
		Engine::Add_GameObject(Engine::UI, L"MonsterHpNumber", pGameObject);
		CUIMgr::GetInstance()->AddCurUI(pGameObject);
		m_vecHpNumber.emplace_back(dynamic_cast<CHpNumber*>(pGameObject));

		pGameObject = CHpNumber::Create(m_pGraphicDev, CHpNumber::TENS, m_iLineCnt, iTens);
		Engine::Add_GameObject(Engine::UI, L"MonsterHpNumber", pGameObject);
		m_vecHpNumber.emplace_back(dynamic_cast<CHpNumber*>(pGameObject));
		CUIMgr::GetInstance()->AddCurUI(pGameObject);

		pGameObject = CHpNumber::Create(m_pGraphicDev, CHpNumber::HUNDREDS, m_iLineCnt, 10);
		Engine::Add_GameObject(Engine::UI, L"MonsterHpNumber", pGameObject);
		m_vecHpNumber.emplace_back(dynamic_cast<CHpNumber*>(pGameObject));
		CUIMgr::GetInstance()->AddCurUI(pGameObject);

	}
	else if (3 == m_iDigitCnt)
	{
		_uint iUnits, iTens, iHundreds = 0;

		iUnits = (_uint)iLineCnt % 10;
		iTens = ((_uint)iLineCnt / 10) % 10;
		iHundreds = (_uint)floor(iLineCnt*0.01f);

		CGameObject* pGameObject = nullptr;
		pGameObject = CHpNumber::Create(m_pGraphicDev, CHpNumber::UNITS, m_iLineCnt, iUnits);
		Engine::Add_GameObject(Engine::UI, L"MonsterHpNumber", pGameObject);
		m_vecHpNumber.emplace_back(dynamic_cast<CHpNumber*>(pGameObject));
		CUIMgr::GetInstance()->AddCurUI(pGameObject);

		pGameObject = CHpNumber::Create(m_pGraphicDev, CHpNumber::TENS, m_iLineCnt, iTens);
		Engine::Add_GameObject(Engine::UI, L"MonsterHpNumber", pGameObject);
		m_vecHpNumber.emplace_back(dynamic_cast<CHpNumber*>(pGameObject));
		CUIMgr::GetInstance()->AddCurUI(pGameObject);

		pGameObject = CHpNumber::Create(m_pGraphicDev, CHpNumber::HUNDREDS, m_iLineCnt, iHundreds);
		Engine::Add_GameObject(Engine::UI, L"MonsterHpNumber", pGameObject);
		m_vecHpNumber.emplace_back(dynamic_cast<CHpNumber*>(pGameObject));
		CUIMgr::GetInstance()->AddCurUI(pGameObject);

		pGameObject = CHpNumber::Create(m_pGraphicDev, CHpNumber::THOUSANDS, m_iLineCnt, 10);
		Engine::Add_GameObject(Engine::UI, L"MonsterHpNumber", pGameObject);
		m_vecHpNumber.emplace_back(dynamic_cast<CHpNumber*>(pGameObject));
		CUIMgr::GetInstance()->AddCurUI(pGameObject);

	}
	//else if (4 == m_iDigitCnt) /////얘는 안고침 
	//{
	//	_uint iUnits, iTens, iHundreds, iThousands = 0;

	//	iUnits = iDamage % 10;
	//	iTens = (iDamage / 10) % 10;
	//	iHundreds = (iDamage / 100) % 10;
	//	iThousands = (_uint)floor(iDamage*0.001f);

	//}

}

void CMonsterHpMgr::CalculateDigitCnt(_uint iLineCnt)
{
	int iQuotient = iLineCnt / 10;
	if (0 != iQuotient)
	{
		CalculateDigitCnt(iQuotient);
		++m_iDigitCnt;
	}
}



HRESULT CMonsterHpMgr::Clone_Component()
{
	//Engine::CComponent* pComponent = nullptr;

	//pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	//if (pComponent == nullptr)
	//	return E_FAIL;
	//m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	//pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	//if (pComponent == nullptr)
	//	return E_FAIL;
	//m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	////Buffer
	//pComponent = m_pScreenTexBufferCom = Engine::CScreenTex::Create(m_pGraphicDev, fX, fY, fSizeX, fSizeY, fViewZ);
	//if (pComponent == nullptr)
	//	return E_FAIL;
	//m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	//// Texture
	//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_MonsterHp"));
	//if (pComponent == nullptr)
	//	return E_FAIL;
	//m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	////Shader
	//pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	//if (pComponent == nullptr)
	//	return E_FAIL;
	//m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CMonsterHpMgr::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	//CBaseObject::Set_ShaderMatrix(pEffect);
	//m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eMonsterHpType);
	return S_OK;
}

CMonsterHpMgr * CMonsterHpMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonsterHpMgr* pInstance = new CMonsterHpMgr(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMonsterHpMgr::Free()
{
	CUIObject::Free();
	
	if (CUIMgr::UITYPE_SHOOTING == m_eUIType)
	{
		Engine::UnSubscribe(Engine::APOSTLE, m_pApostleObserver);
		Engine::Safe_Release(m_pApostleObserver);
	}

	m_vecMonsterHp.clear();
	m_vecHpNumber.clear();
}
