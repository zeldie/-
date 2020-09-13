#include "stdafx.h"
#include "MatchingPlayer.h"
//#include "MatchingGameObserver.h"

CMatchingPlayer::CMatchingPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev)
	, m_eCurMatchingState(MATCHINGSTATE_END)
	, m_eNextMatchingState(MATCHINGSTATE_END)
{
}


CMatchingPlayer::~CMatchingPlayer()
{
}

HRESULT CMatchingPlayer::Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);

	m_pDynamicMeshCom->Set_AnimationSet(0);

	////옵저버신청
	//m_pMatchingGameObserver = CMatchingGameObserver::Create();
	//if (nullptr == m_pMatchingGameObserver)
	//	return E_FAIL;
	//Engine::Subscribe(Engine::MATCHING_GMAE, m_pMatchingGameObserver);

	return S_OK;
}

HRESULT CMatchingPlayer::Ready_GameObject(_float fX, _float fY)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(fX - (WINCX >> 1), -fY + (WINCY >> 1), 300.f);
	m_pTransformCom->Set_Scale(3.f, 3.f, 3.f);
	m_pTransformCom->Rotation(Engine::ROT_Y,160.f);

	m_pDynamicMeshCom->Set_AnimationSet(0);

	////옵저버신청
	//m_pMatchingGameObserver = CMatchingGameObserver::Create();
	//if (nullptr == m_pMatchingGameObserver)
	//	return E_FAIL;
	//Engine::Subscribe(Engine::MATCHING_GMAE, m_pMatchingGameObserver);

	return S_OK;
}

_int CMatchingPlayer::Update_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::Update_GameObject(dTimeDelta);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.1f, 4000.f); //->직교투영

	return Engine::NO_EVENT;
}

_int CMatchingPlayer::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);
	
	CheckState();
	Is_AnimationSetFinish();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CMatchingPlayer::Render_Geometry(const _double & dTimeDelta)
{
	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta);

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
			pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
			pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
			pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
			pEffect->BeginPass(17); //기본 + 컬모드 넌
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CMatchingPlayer::Render_PostEffect(const _double & dTimeDelta)
{
	//Shader
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->SetTexture("g_DiffuseTexture", nullptr);
			pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
			pEffect->BeginPass(1);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}


void CMatchingPlayer::CheckState()
{
	if (m_eCurMatchingState != m_eNextMatchingState)
	{
		_int iSound;

		switch (m_eNextMatchingState)
		{
		case COMMON:
			m_pDynamicMeshCom->Set_AnimationSet(HF_ISTJ_Wait);
			break;
		case WAIT:
		{
			if (SUCCESS_1 == m_eCurMatchingState || FAIL == m_eCurMatchingState
				|| SUCCESS_2 == m_eCurMatchingState)
				return;
			else
				m_pDynamicMeshCom->Set_AnimationSet(HF_ISTJ_Wait_Habit);
		}
			break;
		case SUCCESS_1:
			m_pDynamicMeshCom->Set_AnimationSet(HF_EMLoveyou_1);
			iSound = rand() % 7;
			CSoundMgr::Get_Instance()->HoSoundOn(iSound, 1.f);
			break;
		case SUCCESS_2:
			m_pDynamicMeshCom->Set_AnimationSet(HF_EMLikeIt);
			iSound = rand() % 7;
			CSoundMgr::Get_Instance()->HoSoundOn(iSound, 1.f);
			break;
		case FAIL:
			m_pDynamicMeshCom->Set_AnimationSet(HF_EMSadness_Start);
			iSound = rand() % 2 + 7;
			CSoundMgr::Get_Instance()->HoSoundOn(iSound, 1.f);
			break;
		case WIN:
			m_pDynamicMeshCom->Set_AnimationSet(HF_EMVictory_1);
			CSoundMgr::Get_Instance()->SetVolumeDown(0.3f);
			CSoundMgr::Get_Instance()->HoSoundOn(16,1.f);
			break;
		case LOSE:
			m_pDynamicMeshCom->Set_AnimationSet(HF_EMAngry);
			break;
		}
		m_eCurMatchingState = m_eNextMatchingState;
	}
}

void CMatchingPlayer::Is_AnimationSetFinish()
{
	if (true == m_pDynamicMeshCom->Is_AnimationSetFinish(0.2f))
	{
		switch (m_eNextMatchingState)
		{
			//case COMMON:
			//	break;
		case WAIT:
		{
			if (m_pDynamicMeshCom->Get_TrackTime() >= m_pDynamicMeshCom->Get_Period()*0.8f)
			{
				m_eNextMatchingState = COMMON;
			}
		}
		break;
		case SUCCESS_1:
		{
			if (m_pDynamicMeshCom->Get_TrackTime() >= m_pDynamicMeshCom->Get_Period()*0.8f)
			{
				m_eNextMatchingState = COMMON;
			}
		}
		break;
		//case FAIL:
		//	break;
		//case WIN:
		//	break;
		//case LOSE:
		//	break;
		default:
			m_eNextMatchingState = COMMON;
			break;
		}
	}
}

HRESULT CMatchingPlayer::Clone_Component()
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

	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_MatchingPlayer"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CMatchingPlayer::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	_matrix		matWorld, matView;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixIdentity(&matView); //직교투영

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);
	pEffect->SetVector("vChangeColor", &_vec4(0.5f, 0.5f, 0.5f, 1.f));
	return S_OK;
}

CMatchingPlayer * CMatchingPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CMatchingPlayer* pInstance = new CMatchingPlayer(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CMatchingPlayer * CMatchingPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY)
{
	CMatchingPlayer* pInstance = new CMatchingPlayer(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(fX, fY)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMatchingPlayer::Free()
{
	CDynamicMeshObject::Free();

	//Engine::UnSubscribe(Engine::MATCHING_GMAE, m_pMatchingGameObserver);
	//Engine::Safe_Release(m_pMatchingGameObserver);

}
