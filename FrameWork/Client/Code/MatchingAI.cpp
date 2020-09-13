#include "stdafx.h"
#include "MatchingAI.h"

CMatchingAI::CMatchingAI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev)
	, m_eCurMatchingState(MATCHINGSTATE_END)
	, m_eNextMatchingState(MATCHINGSTATE_END)
{
}


CMatchingAI::~CMatchingAI()
{
}

HRESULT CMatchingAI::Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);

	m_pDynamicMeshCom->Set_AnimationSet(0);
	return S_OK;
}

HRESULT CMatchingAI::Ready_GameObject(_float fX, _float fY)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(fX - (WINCX >> 1), -fY + (WINCY >> 1), 300.f);
	m_pTransformCom->Set_Scale(3.f, 3.f, 3.f);
	m_pTransformCom->Rotation(Engine::ROT_Y,190.f);

	m_pDynamicMeshCom->Set_AnimationSet(0);

	return S_OK;
}

_int CMatchingAI::Update_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::Update_GameObject(dTimeDelta);
	

	return Engine::NO_EVENT;
}

_int CMatchingAI::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);

	CheckState();
	Is_AnimationSetFinish();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CMatchingAI::Render_Geometry(const _double & dTimeDelta)
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
			pEffect->BeginPass(17); //�⺻ + �ø�� ��
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();

		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CMatchingAI::Render_PostEffect(const _double & dTimeDelta)
{
	////Shader
	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//if (pEffect == nullptr)
	//	return;
	//Engine::Safe_AddRef(pEffect);
	//if (FAILED(Setup_ShaderProps(pEffect)))
	//	return;
	//_uint iPassMax = 0;

	//pEffect->Begin(&iPassMax, 0);

	//list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	//for (auto& iter : *plistMeshContainer)
	//{
	//	_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
	//	m_pDynamicMeshCom->Render_Meshes_Begin(iter);
	//	for (_ulong i = 0; i < dwSubsetNum; ++i)
	//	{
	//		pEffect->SetTexture("g_DiffuseTexture", nullptr);
	//		pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
	//		pEffect->BeginPass(1);
	//		pEffect->CommitChanges();
	//		m_pDynamicMeshCom->Render_Meshes(iter, i);
	//		pEffect->EndPass();
	//	}
	//	m_pDynamicMeshCom->Render_Meshes_End(iter);
	//}
	//pEffect->End();
	//Engine::Safe_Release(pEffect);
}


void CMatchingAI::CheckState()
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
			iSound = rand() % 4 + 9;
			CSoundMgr::Get_Instance()->HoSoundOn(iSound, 1.f);

			break;
		case SUCCESS_2:
			m_pDynamicMeshCom->Set_AnimationSet(HF_EMLikeIt);
			iSound = rand() % 4 + 9;
			CSoundMgr::Get_Instance()->HoSoundOn(iSound, 1.f);

			break;
		case FAIL:
			m_pDynamicMeshCom->Set_AnimationSet(HF_EMSadness_Start);
			iSound = rand() % 3 + 13;
			CSoundMgr::Get_Instance()->HoSoundOn(iSound, 1.f);

			break;
		case WIN:
			m_pDynamicMeshCom->Set_AnimationSet(HF_EMVictory_1);

			break;
		case LOSE:
			m_pDynamicMeshCom->Set_AnimationSet(HF_EMAngry);
			/*iSound = rand() % 3 + 114;
			CSoundMgr::Get_Instance()->SoundOn(iSound);*/

			break;
		}
		m_eCurMatchingState = m_eNextMatchingState;
	}
}

void CMatchingAI::Is_AnimationSetFinish()
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


HRESULT CMatchingAI::Clone_Component()
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

	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_MatchingAI"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CMatchingAI::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	_matrix		matWorld, matView;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixIdentity(&matView); //��������
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.7f, 7000.f); //->��������
	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);
	pEffect->SetVector("vChangeColor", &_vec4(1.f, 1.f, 1.f, 1.f));
	return S_OK;
}

CMatchingAI * CMatchingAI::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CMatchingAI* pInstance = new CMatchingAI(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CMatchingAI * CMatchingAI::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY)
{
	CMatchingAI* pInstance = new CMatchingAI(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(fX, fY)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMatchingAI::Free()
{
	CDynamicMeshObject::Free();
}
