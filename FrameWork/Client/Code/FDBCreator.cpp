#include "stdafx.h"
#include "FDBCreator.h"
//#include "ScreenTex.h"
#include "FlagDeathBar.h"

CFDBCreator::CFDBCreator(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUIObject(pGraphicDev),
	m_eTeam(TEAM_END),
	m_bLateInit(true),
	m_iListIndex(0)
{
	m_vecFlagDeathBar.reserve(3);
}


CFDBCreator::~CFDBCreator()
{
}

HRESULT CFDBCreator::Ready_GameObject(TEAM eType, _tchar* tKiller, _tchar* tDead)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_eTeam = eType;

	lstrcpy(m_szKillerName, tKiller);
	lstrcpy(m_szDeadName, tDead);

	return S_OK;
}

_int CFDBCreator::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{
		CUIMgr::GetInstance()->EraseFlagDeathInfoList();
		return Engine::OBJ_DEAD;
	}

	CUIObject::Update_GameObject(dTimeDelta);

	//한번만 생성
	if (m_bLateInit)
	{
		_uint j = 0;

		Engine::CGameObject*		pGameObject = nullptr;
		pGameObject = CFlagDeathBar::Create(m_pGraphicDev, m_eTeam,
			CFlagDeathBar::BG, m_szKillerName, m_szDeadName,
			0.f, 0.f,  170.4f, 19.8f, 0.1f);
		Engine::Add_GameObject(Engine::UI, L"FlagDeathBar", pGameObject);
		m_vecFlagDeathBar.emplace_back(dynamic_cast<CFlagDeathBar*>(pGameObject));

		pGameObject = CFlagDeathBar::Create(m_pGraphicDev, m_eTeam,
			CFlagDeathBar::ARROW, m_szKillerName, m_szDeadName,
			0.f, 0.f, 30.f, 15.f, 0.1f);
		Engine::Add_GameObject(Engine::UI, L"FlagDeathBar", pGameObject);
		m_vecFlagDeathBar.emplace_back(dynamic_cast<CFlagDeathBar*>(pGameObject));

		pGameObject = CFlagDeathBar::Create(m_pGraphicDev, m_eTeam,
			CFlagDeathBar::EMOTICON, m_szKillerName, m_szDeadName,
			0.f, 0.f, 15.f, 15.f, 0.1f);
		Engine::Add_GameObject(Engine::UI, L"FlagDeathBar", pGameObject);
		m_vecFlagDeathBar.emplace_back(dynamic_cast<CFlagDeathBar*>(pGameObject));
		m_bLateInit = false;
	}

	//위치 정해주는거야(puimgr->m_listflagdeathinfo의 정보업뎃에 따라 위치가 바뀌어야 하므로)
	for (_uint i = 0; i<m_vecFlagDeathBar.size(); ++i)
	{
		if (0 == i)
		{
			m_vecFlagDeathBar[i]->Set_RealPos(_vec2(-0.8f, 244.5f + (m_iListIndex* 24.8f))); //19.8f
		}
		else if (1 == i)
		{
			m_vecFlagDeathBar[i]->Set_RealPos(_vec2(46.9f, 248.5f + (m_iListIndex* 24.8f)));
		}
		else if (2 == i)
		{
			m_vecFlagDeathBar[i]->Set_RealPos(_vec2(123.7f, 246.5f + (m_iListIndex* 24.8f)));
		}

		m_vecFlagDeathBar[i]->Set_ListIndex(m_iListIndex);
	}

	return Engine::NO_EVENT;
}

_int CFDBCreator::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	_uint iDeadCnt = 0;
	for (auto& pVec : m_vecFlagDeathBar)
	{
		if (pVec->Get_isDead())
			++iDeadCnt;
		else
			break;
	}

	if (3 == iDeadCnt)
	{
		m_vecFlagDeathBar.clear();
		m_bIsDead = true;
	}

	return Engine::NO_EVENT;
}

void CFDBCreator::Render_Geometry(const _double & dTimeDelta)
{
}

HRESULT CFDBCreator::Clone_Component()
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

	//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_FlagDeathBar"));
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

HRESULT CFDBCreator::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	return S_OK;
}

CFDBCreator * CFDBCreator::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEAM eType, _tchar* tKiller, _tchar* tDead)
{
	CFDBCreator* pInstance = new CFDBCreator(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, tKiller, tDead)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFDBCreator::Free()
{
	CUIObject::Free();
}
