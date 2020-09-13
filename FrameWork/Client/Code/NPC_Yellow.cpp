#include "stdafx.h"
#include "NPC_Yellow.h"
#include "NPCNoFuncObj.h"
#include "QuestBackBar.h"

CNPC_Yellow::CNPC_Yellow(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_pQuestMgr(CQuestMgr::GetInstance()),
	m_bLateInit(true),
	m_bOne(true),
	m_bMissionPossible(false),
	m_bOneCreateChat(true),
	m_bMissionCompleted(false),
	m_iQuestCnt(0),
	m_bPotalQCompleted(false),
	m_bOnePotalQ(true)
{
	//ù��° ����Ʈ : ����Ӹ� ģ�� ã�� --> ������ȸ �����ϱ�\n(��Ż�� �̵�) m_bPotalQCompleted=false/
	//�ι�° ����Ʈ : ���� �ٳ���� --> ����Ӹ� ����� ã��
	//����° ����Ʈ : �ٽ� ����Ӹ����� ���� --> ���� ���ϱ�
	//�׹�° ����Ʈ :                        --> �ٽ� ����Ӹ����� ����
}


CNPC_Yellow::~CNPC_Yellow()
{
}

HRESULT CNPC_Yellow::Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale)
{
	if (FAILED(Clone_Component(wstrObjectKey)))
		return E_FAIL;

	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);

	m_pDynamicMeshCom->Set_AnimationSet(13); //COMMON_COMBATWAIT

	m_eNPCType = CQuestMgr::YELLOW;
	return S_OK;
}

_int CNPC_Yellow::Update_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::Update_GameObject(dTimeDelta);

	if (m_bLateInit)
	{
		Engine::CGameObject* pGameObject = nullptr;

		//�Ӹ��� !
		pGameObject = CNPCNoFuncObj::Create(m_pGraphicDev, CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, false, _vec3(0.f, 0.f, 0.f), _vec3(30.f, 30.f, 1.f), m_pTransformCom);
		if (nullptr == pGameObject)
			return Engine::NO_EVENT;
		Engine::Add_GameObject(Engine::UI, L"NPCNoFuncObj", pGameObject);
		m_pUIMgr->AddvecNPCNoFuncObj(pGameObject);

		//�Ӹ�����ȭâ
		pGameObject = CNPCNoFuncObj::Create(m_pGraphicDev, CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, false, _vec3(0.f, 0.f, 0.f), _vec3(20.f, 20.f, 1.f), m_pTransformCom);
		if (nullptr == pGameObject)
			return Engine::NO_EVENT;
		Engine::Add_GameObject(Engine::UI, L"NPCNoFuncObj", pGameObject);
		m_pUIMgr->AddvecNPCNoFuncObj(pGameObject);

		//�Ӹ��� ?
		pGameObject = CNPCNoFuncObj::Create(m_pGraphicDev, CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, true, _vec3(0.f, 0.f, 0.f), _vec3(10.f, 20.f, 1.f), m_pTransformCom);
		if (nullptr == pGameObject)
			return Engine::NO_EVENT;
		Engine::Add_GameObject(Engine::UI, L"NPCNoFuncObj", pGameObject);
		m_pUIMgr->AddvecNPCNoFuncObj(pGameObject);

		//��ȣ�ۿ� txt ����
		pGameObject = CNPCNoFuncObj::Create(m_pGraphicDev, CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false, _vec3(WINCX >> 1, (WINCY >> 1) + 100.f, 0.f), _vec3(135.f, 47.f, 1.f));
		if (nullptr == pGameObject)
			return Engine::NO_EVENT;
		Engine::Add_GameObject(Engine::UI, L"NPCNoFuncObj", pGameObject);
		m_pUIMgr->AddvecNPCNoFuncObj(pGameObject);

		m_bLateInit = false;
	}

	m_iQuestCnt = m_pQuestMgr->Get_YellowQuestCnt();

	return Engine::NO_EVENT;
}

_int CNPC_Yellow::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);

	//�̼����� üũ
	if (m_pQuestMgr->Get_MissionPossible(m_eNPCType))
		m_bMissionPossible = true;
	else
		m_bMissionPossible = false;

	//ù�̼� Ŭ���� üũ
	if (m_pQuestMgr->Get_YellowPotalQCompleted())
	{
		//��Ż���ٿ°ž�
		m_bPotalQCompleted = true;
		//m_bOneCreateChat = true;

		if (m_bOnePotalQ)
		{
			if (4 == m_iQuestCnt)
			{
				--m_iQuestCnt;
				m_pQuestMgr->Set_YellowQuestCnt(m_iQuestCnt);

				m_pQuestMgr->SetQuestClear();//Ŭ�����ߴٰ� �˷��ְ�		
				m_pQuestMgr->EraseAllQuest(); 
				//m_pUIMgr->SetRender_QuestCheck();//Ŭ���������� üũǥ�����ְ�.
												 //// & ���ο� ����Ʈ �߰�(��� �տ� ����Ʈ ���ΰ� �߰��Ұ�-�̾����� ����Ʈ��.)	
				
				QUEST_INFO tQuest;
				tQuest.wstrQuest = L"����Ӹ� ����� ã��";
				tQuest.bClear = false;
				m_pQuestMgr->AddQuest(tQuest);
			}
			m_bOnePotalQ = false;

		}
	}
	else
		m_bPotalQCompleted = false;



	//�̼� ���� üũ
	if (m_pQuestMgr->Get_MissionCompleted(m_eNPCType))
	{
		if (m_bOne)
		{
			////����õ�� �����°�
			m_bMissionCompleted = true;
			m_bOneCreateChat = true;

			////�����佺 ����Ʈ ���� üũ ǥ��	
			if (2 == m_iQuestCnt)
			{
				--m_iQuestCnt;
				m_pQuestMgr->Set_YellowQuestCnt(m_iQuestCnt);

				m_pQuestMgr->SetQuestClear();//Ŭ�����ߴٰ� �˷��ְ�		
				m_pUIMgr->SetRender_QuestCheck();//Ŭ���������� üũǥ�����ְ�.
												 //// & ���ο� ����Ʈ �߰�(��� �տ� ����Ʈ ���ΰ� �߰��Ұ�-�̾����� ����Ʈ��.)	
				QUEST_INFO tQuest;
				tQuest.wstrQuest = L"�ٽ� ����Ӹ� ģ������ ����";
				tQuest.bClear = false;
				m_pQuestMgr->AddQuest(tQuest);
			}
			m_bOne = false;
		}
	}
	else
	{
		m_bMissionCompleted = false;
	}

	CheckPlayerDist();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CNPC_Yellow::Render_Geometry(const _double & dTimeDelta)
{
	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta);

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;


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
			pEffect->CommitChanges();
			pEffect->Begin(&iPassMax, 0);
			pEffect->BeginPass(0);
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
			pEffect->End();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}

	Engine::Safe_Release(pEffect);

	CDynamicMeshObject::Render_Geometry(dTimeDelta);
}

void CNPC_Yellow::Render_PostEffect(const _double & dTimeDelta)
{
	//Shader
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	/*if (FAILED(CBaseObject::Set_ShaderShadow(pEffect)))
		return;*/
	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(1);
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->SetTexture("g_DiffuseTexture", nullptr);
			pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->EndPass();
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CNPC_Yellow::Render_Shadow(const _double & dTimeDelta)
{
	////Shader
	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//if (pEffect == nullptr)
	//	return;
	//Engine::Safe_AddRef(pEffect);
	//if (FAILED(Setup_ShaderProps(pEffect)))
	//	return;
	////if (FAILED(CBaseObject::Set_ShaderShadow(pEffect)))
	////	return;
	//_uint iPassMax = 0;

	//list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	//for (auto& iter : *plistMeshContainer)
	//{
	//	_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
	//	m_pDynamicMeshCom->Render_Meshes_Begin(iter);
	//	for (_ulong i = 0; i < dwSubsetNum; ++i)
	//	{
	//		pEffect->CommitChanges();
	//		pEffect->Begin(&iPassMax, 0);
	//		pEffect->BeginPass(6);
	//		m_pDynamicMeshCom->Render_Meshes(iter, i);
	//		pEffect->EndPass();
	//		pEffect->End();
	//	}
	//	m_pDynamicMeshCom->Render_Meshes_End(iter);
	//}


	//Engine::Safe_Release(pEffect);
}

void CNPC_Yellow::CheckPlayerDist()
{
	_vec3 vPlayerPos;
	dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"RealPlayer", Engine::TRANSFORM, Engine::ID_DYNAMIC))->Get_Info(Engine::INFO_POS, &vPlayerPos);

	_float fDist = Engine::GetDistNoY(*m_pTransformCom->Get_Info(Engine::INFO_POS), vPlayerPos);
	if (90.f >= fDist)
	{
		m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, false);
		m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, false);
		m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, false);

		if (m_bOneCreateChat)
			m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, true);

		if (Engine::KeyDown(DIK_V))
		{
			if (m_bOneCreateChat)
			{
				//m_bOne = true;
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);

				Engine::CGameObject* pGameObject = nullptr;
				//�ϴ� ���â ����			
				pGameObject = CQuestBackBar::Create(m_pGraphicDev, CQuestBackBar::CHAT, m_eNPCType, true, 32.9f, 288.f, 1232.3f, 200.f, 0.2f); //32.9f, 488.0f, 1232.3f, 200.f, 0.2f
				if (pGameObject == nullptr)
					return;
				Engine::Add_GameObject(Engine::UI, L"QuestBackBar", pGameObject);

				//�ϴ� NPC �׸�
				pGameObject = CQuestBackBar::Create(m_pGraphicDev, CQuestBackBar::NPCPICTURE_YELLOW, m_eNPCType, true, 4.f, 211.3f, 440.f, 600.f, 0.f);
				if (pGameObject == nullptr)
					return;
				Engine::Add_GameObject(Engine::UI, L"QuestBackBar", pGameObject);

				m_pUIMgr->Set_UIType(CUIMgr::UITYPE_NPC);

				////����Ʈâ �׸��� �ʾ�
				//m_pUIMgr->SetRender_Quest(false);

				//����Ʈ ī��Ʈ üũ 
				CheckQuestCnt();

				m_bOneCreateChat = false;
			}
		}
	}
	else
	{
		if (!m_bMissionPossible) //�̼��� �Ұ����� ����
		{
			if (!m_bPotalQCompleted)
			{
				//��Ż������
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, false);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, false);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, true);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);

			}
			else
			{
				if (!m_bMissionCompleted)
				{					
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, false);
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, true);
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, false);
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);
				}
				else
				{
					//����õ�������
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, false);
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, true);
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, false);
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);
				}
			}

		}
		else // �̼��� ������ ����
		{
			if (!m_bMissionCompleted)
			{
				//�̼��� ���� ���� ���� ����
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, true); //����ǥ ����
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, false);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, false);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);

			}
			else
			{
				//�̼��� �����»���
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, false);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, false); //����ǥ����
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, true);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);
			}
		}

		////UI ���� ������Ʈ �ѹ���!
		//if (m_bOne)
		//{
		//	//m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);
		//	//m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);
		//
		//	m_bOne = false;
		//}

	}
}

void CNPC_Yellow::CheckQuestCnt()
{
	if (0 >= m_iQuestCnt)
		return;
	if (4 == m_iQuestCnt) //ù��° �� : ��Ż�̵�
	{
		///////////////////
		//���� �������
		///////////////////
		if (!m_bMissionPossible) //�̼��� �Ұ����� ����
		{
			if (m_bPotalQCompleted)
			{
				m_pQuestMgr->SetQuestClear();
				m_pUIMgr->SetRender_QuestCheck();
			}
		}
		else // �̼��� ������ ����
		{
		}
	}
	else if (3 == m_iQuestCnt) //ù��° ����Ʈ : ����Ӹ� ����� ã��
	{
		if (!m_bMissionPossible) //�̼��� �Ұ����� ����
		{
			if (m_bPotalQCompleted)
			{
				//ī��Ʈ �Ѱ� ���
				--m_iQuestCnt;
				m_pQuestMgr->Set_YellowQuestCnt(m_iQuestCnt);

				//Ŭ�����ߴٰ� �˷��ְ�
				m_pQuestMgr->SetQuestClear();

				//Ŭ���������� üũǥ�����ְ�.
				m_pUIMgr->SetRender_QuestCheck();
			}
		}
		else // �̼��� ������ ����
		{
		}
	}
	else if (2 == m_iQuestCnt) 
	{		
		///////////////////////////////
		//������������� �Ϸε���
		//////////////////////////////

		if (!m_bMissionPossible) //�̼��� �Ұ����� ����
		{
		}
		else // �̼��� ������ ����
		{
			if (!m_bMissionCompleted)
			{
				if (m_bPotalQCompleted)
				{
					//m_pQuestMgr->SetQuestClear();
					//m_pUIMgr->SetRender_QuestCheck();
					CQuestMgr::GetInstance()->EraseAllQuest();
				}
			}
		}

	}
	//else if (1 == m_iQuestCnt) //����° ����Ʈ : �ٽ� ����Ӹ����� ����
	//{
	//	if (!m_bMissionPossible) //�̼��� �Ұ����� ����
	//	{
	//	}
	//	else // �̼��� ������ ����
	//	{
	//		if (!m_bMissionCompleted)
	//		{
	//			//�̼��� ���� ���� ���� ����
	//		}
	//		else
	//		{
	//			//ī��Ʈ �Ѱ� ���
	//			--m_iQuestCnt;
	//			m_pQuestMgr->Set_YellowQuestCnt(m_iQuestCnt);

	//			////Ŭ�����ߴٰ� �˷��ְ�
	//			//m_pQuestMgr->SetQuestAllClear();

	//			////Ŭ���������� üũǥ�����ְ�.
	//			//m_pUIMgr->SetRender_QuestCheck();
	//		}
	//	}
	//}
}

HRESULT CNPC_Yellow::Clone_Component(wstring wstrObjectKey)
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

	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrObjectKey));
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

HRESULT CNPC_Yellow::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	pEffect->SetVector("vChangeColor", &_vec4(0.7f, 0.7f, 0.7f, 1.f));
	return S_OK;
}

CNPC_Yellow * CNPC_Yellow::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CNPC_Yellow* pInstance = new CNPC_Yellow(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(wstrObjectKey, pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CNPC_Yellow::Free()
{
	CDynamicMeshObject::Free();
}