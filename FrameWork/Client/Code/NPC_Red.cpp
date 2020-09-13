#include "stdafx.h"
#include "NPC_Red.h"
#include "NPCNoFuncObj.h"
#include "QuestBackBar.h"

CNPC_Red::CNPC_Red(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_pQuestMgr(CQuestMgr::GetInstance()),
	m_bLateInit(true),
	m_bOne(true),
	m_bTwo(true),
	m_bMissionPossible(false),
	m_bOneCreateChat(true),
	m_bMissionCompleted(false),
	m_bSecondMissionCompleted(false),
	m_iQuestCnt(0),
	m_bAllClear(false)
{
	//첫번째 퀘스트 : 빨간머리 친구 찾기
	//두번째 퀘스트 : 빨간머리 이기기      -> 짝맞추기
	//세번째 퀘스트 : 다시 빨간머리 친구에게 가기
	//네번째 퀘스트 : 깃발전 참여하기                       -----> 무기구하기
	//다섯번째 퀘스트 : 다시 빨간머리 친구에게 가기
	//여섯번째 퀘스트 : 습격한 몬스터 처치하기              -----> 검투대회 참여하기
}

CNPC_Red::~CNPC_Red()
{
}

HRESULT CNPC_Red::Ready_GameObject(wstring wstrObjectKey, _vec3* pPos, _vec3* pAngle, _vec3* pScale)
{
	if (FAILED(Clone_Component(wstrObjectKey)))
		return E_FAIL;

	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);

	m_pDynamicMeshCom->Set_AnimationSet(6);

	m_eNPCType = CQuestMgr::RED;
	return S_OK;
}

_int CNPC_Red::Update_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::Update_GameObject(dTimeDelta);

	if (m_bLateInit)
	{
		Engine::CGameObject* pGameObject = nullptr;

		//머리위 !
		pGameObject = CNPCNoFuncObj::Create(m_pGraphicDev, CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, false, _vec3(0.f, 0.f, 0.f), _vec3(30.f, 30.f, 1.f), m_pTransformCom);
		if (nullptr == pGameObject)
			return Engine::NO_EVENT;
		Engine::Add_GameObject(Engine::UI, L"NPCNoFuncObj", pGameObject);
		m_pUIMgr->AddvecNPCNoFuncObj(pGameObject);

		//머리위대화창
		pGameObject = CNPCNoFuncObj::Create(m_pGraphicDev, CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, true, _vec3(0.f, 0.f, 0.f), _vec3(20.f, 20.f, 1.f), m_pTransformCom);
		if (nullptr == pGameObject)
			return Engine::NO_EVENT;
		Engine::Add_GameObject(Engine::UI, L"NPCNoFuncObj", pGameObject);
		m_pUIMgr->AddvecNPCNoFuncObj(pGameObject);

		//머리위 ?
		pGameObject = CNPCNoFuncObj::Create(m_pGraphicDev, CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, false, _vec3(0.f, 0.f, 0.f), _vec3(10.f, 20.f, 1.f), m_pTransformCom);
		if (nullptr == pGameObject)
			return Engine::NO_EVENT;
		Engine::Add_GameObject(Engine::UI, L"NPCNoFuncObj", pGameObject);
		m_pUIMgr->AddvecNPCNoFuncObj(pGameObject);

		//상호작용 txt 생성
		pGameObject = CNPCNoFuncObj::Create(m_pGraphicDev, CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false, _vec3(WINCX >> 1, (WINCY >> 1) + 100.f, 0.f), _vec3(135.f, 47.f, 1.f));
		if (nullptr == pGameObject)
			return Engine::NO_EVENT;
		Engine::Add_GameObject(Engine::UI, L"NPCNoFuncObj", pGameObject);
		m_pUIMgr->AddvecNPCNoFuncObj(pGameObject);

		m_bLateInit = false;
	}

	m_iQuestCnt = m_pQuestMgr->Get_RedQuestCnt();
	m_bAllClear = m_pQuestMgr->Get_RedAllClear();

	return Engine::NO_EVENT;
}

_int CNPC_Red::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);

	//미션유무 체크
	if (m_pQuestMgr->Get_MissionPossible(m_eNPCType))
		m_bMissionPossible = true;
	else
		m_bMissionPossible = false;

	//미션 성공 체크
	if (m_pQuestMgr->Get_MissionCompleted(m_eNPCType))
	{
		if (m_bOne)
		{
			////짝맞추기 깨고 온거임
			m_bMissionCompleted = true;
			m_bOneCreateChat = true;

			////짝맞추기 퀘스트 성공 체크 표시	
			if (5 == m_iQuestCnt)
			{
				--m_iQuestCnt;
				m_pQuestMgr->Set_RedQuestCnt(m_iQuestCnt);
				m_pQuestMgr->SetQuestClear();//클리어했다고 알려주고		
				m_pUIMgr->SetRender_QuestCheck();//클리어했으니 체크표시해주고.
												 //// & 새로운 퀘스트 추가(얘는 앞에 퀘스트 냅두고 추가할것-이어지는 퀘스트라.)	
				QUEST_INFO tQuest;
				tQuest.wstrQuest = L"다시 빨간머리 친구에게 가기";
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

	//두번쨰 미션  성공 체크
	if (m_pQuestMgr->Get_SecondMissionCompleted(m_eNPCType))
	{
		if (m_bTwo)
		{
			//깃발전끝내고온거야
			m_bSecondMissionCompleted = true; //(이경우는 m_bMissionPossible= false, m_bSecondMissionCompleted = true 임)

			////깃발전 퀘스트 성공 체크 표시	
			if (3 == m_iQuestCnt)
			{
				--m_iQuestCnt;
				m_pQuestMgr->Set_RedQuestCnt(m_iQuestCnt);
				m_pQuestMgr->SetQuestClear();//클리어했다고 알려주고		
				m_pUIMgr->SetRender_QuestCheck();//클리어했으니 체크표시해주고.
												 //// & 새로운 퀘스트 추가(얘는 앞에 퀘스트 냅두고 추가할것-이어지는 퀘스트라.)	

				QUEST_INFO tQuest;
				tQuest.wstrQuest = L"다시 빨간머리 친구에게 가기";
				tQuest.bClear = false;
				m_pQuestMgr->AddQuest(tQuest);

			}

			m_bTwo = false;
		}
	}
	else
	{
		m_bSecondMissionCompleted = false;
	}

	CheckPlayerDist();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return Engine::NO_EVENT;
}

void CNPC_Red::Render_Geometry(const _double & dTimeDelta)
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

void CNPC_Red::Render_PostEffect(const _double & dTimeDelta)
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

void CNPC_Red::Render_Shadow(const _double & dTimeDelta)
{
	//Shader
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	//if (FAILED(CBaseObject::Set_ShaderShadow(pEffect)))
	//	return;
	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->BeginPass(6);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();

		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CNPC_Red::CheckPlayerDist()
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
				//하단 대사창 생성			
				pGameObject = CQuestBackBar::Create(m_pGraphicDev, CQuestBackBar::CHAT, m_eNPCType, true, 32.9f, 288.f, 1232.3f, 200.f, 0.2f);
				if (pGameObject == nullptr)
					return;
				Engine::Add_GameObject(Engine::UI, L"QuestBackBar", pGameObject);

				//하단 NPC 그림
				pGameObject = CQuestBackBar::Create(m_pGraphicDev, CQuestBackBar::NPCPICTURE, m_eNPCType, true, 4.f, 211.3f, 440.f, 600.f, 0.f);
				if (pGameObject == nullptr)
					return;
				Engine::Add_GameObject(Engine::UI, L"QuestBackBar", pGameObject);

				m_pUIMgr->Set_UIType(CUIMgr::UITYPE_NPC);

				//퀘스트창 그리지 않아
				//m_pUIMgr->SetRender_Quest(false);

				//퀘스트 카운트 체크 
				CheckQuestCnt();

				m_bOneCreateChat = false;
			}
		}
	}
	else
	{
		m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);

		if (!m_bMissionPossible) //미션이 불가능한 상태
		{
			if (m_bMissionCompleted)
			{
				if (m_bSecondMissionCompleted)
				{
					if (!m_bAllClear)
					{
						//두번째 미션(벨라토스)을 깨고 온 상태
						//아직 깃발전 퀘스트 수락전 
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, false);
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, true);
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, false);
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);
					}
					else
					{
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, false);
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, false);
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, true);
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);						
					}
				}

			}
			else
			{
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, false);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, false);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, true);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);
			}
		}
		else // 미션이 가능한 상태
		{

			if (!m_bMissionCompleted)
			{
				//미션을 깨고 오지 않은 상태
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, true);//물음표 상태
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, false);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, false);
				m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);
			}
			else
			{
				if (!m_bSecondMissionCompleted)
				{
					//미션을 깨고온상태 (짝맞추기 깨고옴)
					//벨라토스 전
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, false);
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, true); //느낌표상태
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, false);
					m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);

				}
				else
				{
					//깃발전 참여 후
					if (!m_bAllClear)
					{

					}
					else
					{
						//검투대회 수락해야함
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::QUESTIONMARK, m_eNPCType, true);
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::EXCLAMATIONMARK, m_eNPCType, false);
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::SPEECHBUBBLE, m_eNPCType, false);
						m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);

					}
				}
			}
		}

		////UI 상태 업데이트 한번만!
		//if (m_bOne)
		//{
		//	//m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);
		//	//m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);
		//
		//	m_bOne = false;
		//}

	}
}

void CNPC_Red::CheckQuestCnt()
{
	if (0.f >= m_iQuestCnt)
		return;

	if (6 == m_iQuestCnt) //첫번째 퀘스트 : 빨간머리 친구 찾기
	{
		if (!m_bMissionPossible) //미션이 불가능한 상태
		{
		}
		else // 미션이 가능한 상태
		{
			if (!m_bMissionCompleted)
			{
				//미션을 깨고 오지 않은 상태
				//카운트 한개 까고
				--m_iQuestCnt;
				m_pQuestMgr->Set_RedQuestCnt(m_iQuestCnt);

				//클리어했다고 알려주고
				m_pQuestMgr->SetQuestClear();

				//클리어했으니 체크표시해주고.
				m_pUIMgr->SetRender_QuestCheck();

			}
			else
			{
				//미션을 깨고온상태 (노란머리 찾은상태)
			}
		}
	}
	else if (5 == m_iQuestCnt) //두번째 퀘스트 : 빨간머리 이기기
	{
		//if (!m_bMissionPossible) //미션이 불가능한 상태
		//{
		//}
		//else // 미션이 가능한 상태
		//{
		//	if (!m_bMissionCompleted)
		//	{
		//		//미션을 깨고 오지 않은 상태
		//	}
		//	else
		//	{
		//		//미션을 깨고온상태 (벨라토스 처치하고옴)
		//		
		//		//카운트 한개 까고
		//		--m_iQuestCnt;

		//		//클리어했다고 알려주고
		//		CQuestMgr::GetInstance()->SetQuestClear();

		//		//클리어했으니 체크표시해주고.
		//		m_pUIMgr->SetRender_QuestCheck();
		//	}
		//}
	}
	else if (4 == m_iQuestCnt) //세번째 퀘스트 : 다시 빨간머리 친구에게 가기
	{
		if (!m_bMissionPossible) //미션이 불가능한 상태
		{
		}
		else // 미션이 가능한 상태
		{
			if (!m_bMissionCompleted)
			{
				//미션을 깨고 오지 않은 상태
			}
			else
			{
				//미션을 깨고온상태 (짝맞추기 하고옴)

				//카운트 한개 까고
				--m_iQuestCnt;
				m_pQuestMgr->Set_RedQuestCnt(m_iQuestCnt);

				//클리어했다고 알려주고
				m_pQuestMgr->SetQuestAllClear();

				//클리어했으니 체크표시해주고.
				m_pUIMgr->SetRender_QuestCheck();
			}
		}
	}
	else if (3 == m_iQuestCnt) //네번째 퀘스트 : 벨라토스
	{
		//--
	}
	else if (2 == m_iQuestCnt) //다섯번째 퀘스트 : 다시 빨간머리 친구에게 가기
	{
		if (!m_bMissionPossible) //미션이 불가능한 상태
		{
			if (!m_bSecondMissionCompleted)
			{
			}
			else
			{
				if (!m_bAllClear)
				{
					//두번째 미션 깨고 온 상태(벨라토스 하고옴)
					//카운트 한개 까고
					--m_iQuestCnt;
					m_pQuestMgr->Set_RedQuestCnt(m_iQuestCnt);

					//클리어했다고 알려주고
					m_pQuestMgr->SetQuestAllClear();

					//클리어했으니 체크표시해주고.
					m_pUIMgr->SetRender_QuestCheck();
				}
			}
		}
		else // 미션이 가능한 상태
		{
			if (!m_bMissionCompleted)
			{
				//미션을 깨고 오지 않은 상태
			}
			else
			{
				//if (m_bSecondMissionCompleted)
				//{
					////두번째 미션 깨고 온 상태(깃발전 하고옴)
					////카운트 한개 까고
					//--m_iQuestCnt;
					//m_pQuestMgr->Set_RedQuestCnt(m_iQuestCnt);

					////클리어했다고 알려주고
					//m_pQuestMgr->SetQuestAllClear();

					////클리어했으니 체크표시해주고.
					//m_pUIMgr->SetRender_QuestCheck();
				//}
			}
		}
	}
	else if (1 == m_iQuestCnt) //여섯번째 퀘스트 : 무술 대회 참여하기
	{
		//여기는 하게된다면 무술 대회 참여하기 완료한 후가 된다

		//if (!m_bMissionPossible) //미션이 불가능한 상태
		//{
		//	if (!m_bSecondMissionCompleted)
		//	{
		//		//두번째 미션 깨고 오지 않은 상태
		//	}
		//	else
		//	{
		//		//두번째 미션 깨고 온 상태(깃발전 하고옴)
		//		//카운트 한개 까고
		//		--m_iQuestCnt;
		//		m_pQuestMgr->Set_RedQuestCnt(m_iQuestCnt);

		//		//클리어했다고 알려주고
		//		m_pQuestMgr->SetQuestAllClear();

		//		//클리어했으니 체크표시해주고.
		//		m_pUIMgr->SetRender_QuestCheck();
		//	}
		//}
		//else // 미션이 가능한 상태
		//{
		//	if (!m_bMissionCompleted)
		//	{
		//		//미션을 깨고 오지 않은 상태
		//	}
		//	else
		//	{
		//		//미션을 깨고온상태 (짝맞추기 하고옴)
		//	}
		//}
	}

}

HRESULT CNPC_Red::Clone_Component(wstring wstrObjectKey)
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

HRESULT CNPC_Red::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	pEffect->SetVector("vChangeColor", &_vec4(0.7f, 0.7f, 0.7f, 1.f));
	return S_OK;
}

CNPC_Red * CNPC_Red::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjectKey, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CNPC_Red* pInstance = new CNPC_Red(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(wstrObjectKey, pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CNPC_Red::Free()
{
	CDynamicMeshObject::Free();
}
