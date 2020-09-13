#include "stdafx.h"
#include "NPCButton.h"
#include "ScreenTex.h"
#include "NPC_Red.h"
#include "NPC_Yellow.h"
#include "QuestBackBar.h"

CNPCButton::CNPCButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUIObject(pGraphicDev),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_eButtonType(BUTTONTYPE_END),
	m_bChoose(false),
	m_bSceneChange(false)
{
}


CNPCButton::~CNPCButton()
{
}

HRESULT CNPCButton::Ready_GameObject(BUTTONTYPE eType, CQuestMgr::NPCTYPE eNPCType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eButtonType = eType;
	m_eNPCType = eNPCType;

	return S_OK;
}

_int CNPCButton::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);

	return Engine::NO_EVENT;
}

_int CNPCButton::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	_vec3 vMousePos;

	if (m_pScreenTexBufferCom->CheckAroundMouse(g_hWnd, &vMousePos))
		m_bChoose = true;
	else
		m_bChoose = false;


	if (m_bChoose && Engine::MouseUp(Engine::DIM_LB))
	{
		CheckNPCQuest();
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CNPCButton::Render_Geometry(const _double & dTimeDelta)
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
	pEffect->BeginPass(1);

	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CNPCButton::CheckNPCQuest()
{
	if (AGREE1 == m_eButtonType)
	{
		if (CQuestMgr::RED == m_eNPCType)
		{
			/////////////////////////////////////////////////////////////////
			//// 경우의 수1 : 짝맞추기 할려고 들어오는 경우             ////
			//// 경우의 수2 :            //// 
			/////////////////////////////////////////////////////////////////

			if (!CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
			{
				//경우의 수 1 해당

				// 이전 퀘스트 삭제 & 새로운 퀘스트 추가			
				CQuestMgr::GetInstance()->EraseAllQuest();

				QUEST_INFO tQuest;
				tQuest.wstrQuest = L"빨간머리 이기기";
				tQuest.bClear = false;
				CQuestMgr::GetInstance()->AddQuest(tQuest);

				//퀘스트 체크 박스 표시
				m_pUIMgr->SetRender_QuestCheckBox(true);

				//퀘스트 체크 표시
				m_pUIMgr->SetRender_QuestCheck();

				//짝맞추기 시작
				m_bSceneChange = true;
			}
			else
			{
				if (CQuestMgr::GetInstance()->Get_SecondMissionCompleted(m_eNPCType))
				{
					////경우의 수 2 해당

					//// 이전 퀘스트 삭제 & 새로운 퀘스트 추가			
					//CQuestMgr::GetInstance()->EraseAllQuest();

					//QUEST_INFO tQuest;
					//tQuest.wstrQuest = L"습격한 몬스터 처치하기";
					//tQuest.bClear = false;
					//CQuestMgr::GetInstance()->AddQuest(tQuest);

					////Red 모든 퀘스트 다 수락했음()
					//CQuestMgr::GetInstance()->Set_RedAllClear(true);

					////대화창 삭제->그안에서 버튼삭제(퀘스트창은 제외)
					//list<Engine::CGameObject*> pList = Engine::Get_GameObjectlist(Engine::UI, L"QuestBackBar");
					//for (auto& list : pList)
					//{
					//	if (CQuestBackBar::QUEST != dynamic_cast<CQuestBackBar*>(list)->Get_QuestBackBarType())
					//		dynamic_cast<CBaseObject*>(list)->Set_Dead();
					//}

					////npc_red 불변수 트루로 바까줘-> 다시 대화시도할수있게!
					//dynamic_cast<CNPC_Red*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_MatchingAI"))->Set_OneCreateChat(true);

					////퀘스트 체크 박스 표시
					//m_pUIMgr->SetRender_QuestCheckBox(true);

					////퀘스트 체크 표시
					//m_pUIMgr->SetRender_QuestCheck();

					////UI 상태 업데이트
					//m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);
					//m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);
				}

			}
		}
		else if (CQuestMgr::YELLOW == m_eNPCType)
		{
			/////////////////////////////////////////////
			//창이 꺼지고 돼지천사 하러갈거야
			/////////////////////////////////////////////

			// 이전 퀘스트 삭제 & 새로운 퀘스트 추가
			CQuestMgr::GetInstance()->EraseAllQuest();

			QUEST_INFO tQuest;
			tQuest.wstrQuest = L"마을 끝에서 무기 구하기";
			tQuest.bClear = false;
			CQuestMgr::GetInstance()->AddQuest(tQuest);

			//대화창 삭제->그안에서 버튼삭제(퀘스트창은 제외)
			list<Engine::CGameObject*> pList = Engine::Get_GameObjectlist(Engine::UI, L"QuestBackBar");
			for (auto& list : pList)
			{
				if (CQuestBackBar::QUEST != dynamic_cast<CQuestBackBar*>(list)->Get_QuestBackBarType())
					dynamic_cast<CBaseObject*>(list)->Set_Dead();
			}

			//npc_red 불변수 트루로 바까줘-> 다시 대화시도할수있게!
			dynamic_cast<CNPC_Red*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_MatchingAI"))->Set_OneCreateChat(true);

			////(v상호작용) 문구 그려
			//m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, true);

			//MissionUI 의 락을 풀기위해서! 
			CUIMgr::GetInstance()->Set_MissionApostleLock(false);

			//퀘스트 체크 박스 표시
			m_pUIMgr->SetRender_QuestCheckBox(true);

			//퀘스트 체크 표시
			m_pUIMgr->SetRender_QuestCheck();

			//UI 상태 업데이트
			m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);
			m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);
		}
	}
	else if (DISAGREE1 == m_eButtonType)
	{
		/////////////////////////////////////////////////////////////////
		//// 경우의 수1 : 짝맞추기 할려고 들어오는 경우             ////
		//// 경우의 수2 : 깃발전 참여 후에 들어오는 경우            //// 
		/////////////////////////////////////////////////////////////////


		//대화창 삭제->그안에서 버튼삭제(퀘스트창은 제외)
		list<Engine::CGameObject*> pList = Engine::Get_GameObjectlist(Engine::UI, L"QuestBackBar");
		for (auto& list : pList)
		{
			if (CQuestBackBar::QUEST != dynamic_cast<CQuestBackBar*>(list)->Get_QuestBackBarType())
				dynamic_cast<CBaseObject*>(list)->Set_Dead();
		}

		//
		if (CQuestMgr::RED == m_eNPCType)
		{
			if (CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
			{
				if (CQuestMgr::GetInstance()->Get_SecondMissionCompleted(m_eNPCType))
				{
					if (!CQuestMgr::GetInstance()->Get_RedAllClear())
					{
						//무기있는 장소 알려줬는데 거절 누를경우						
						CQuestMgr::GetInstance()->EraseAllQuest();

						QUEST_INFO tQuest;
						tQuest.wstrQuest = L"두번째 무기 구하기";
						tQuest.bClear = false;
						CQuestMgr::GetInstance()->AddQuest(tQuest);
					}
				}
			}
			else
			{
				//나를이기면 초대장 주지! 라고했는데 거절 누른경우
				CQuestMgr::GetInstance()->EraseAllQuest();

				QUEST_INFO tQuest;
				tQuest.wstrQuest = L"빨간머리 이기기";
				tQuest.bClear = false;
				CQuestMgr::GetInstance()->AddQuest(tQuest);
			}

			//npc_red 불변수 트루로 바까줘-> 다시 대화시도할수있게!
			dynamic_cast<CNPC_Red*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_MatchingAI"))->Set_OneCreateChat(true);
		}
		else if (CQuestMgr::YELLOW == m_eNPCType)
		{
			if (!CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
			{
				//정찰다녀와 했는데 거절한 경우				
				CQuestMgr::GetInstance()->EraseAllQuest();

				QUEST_INFO tQuest;
				tQuest.wstrQuest = L"노란머리 친구한테 말걸기";
				tQuest.bClear = false;
				CQuestMgr::GetInstance()->AddQuest(tQuest);
			}

			//npc_yellow 불변수 트루로 바까줘-> 다시 대화시도할수있게!
			dynamic_cast<CNPC_Yellow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_HFPlayer"))->Set_OneCreateChat(true);

		}

		////(v상호작용) 문구 그려
		//m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, true);

		////이전 퀘스트 전부 삭제
		//CQuestMgr::GetInstance()->EraseAllQuest();

		//퀘스트 체크 박스 표시
		m_pUIMgr->SetRender_QuestCheckBox(true);

		//퀘스트 체크 표시
		m_pUIMgr->SetRender_QuestCheck();

		//UI 상태 업데이트
		m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);
		m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);
	}
	else if (OUTBUTTON1 == m_eButtonType)
	{
		//대화창 삭제->그안에서 버튼삭제(퀘스트창은 제외)
		list<Engine::CGameObject*> pList = Engine::Get_GameObjectlist(Engine::UI, L"QuestBackBar");
		for (auto& list : pList)
		{
			if (CQuestBackBar::QUEST != dynamic_cast<CQuestBackBar*>(list)->Get_QuestBackBarType())
				dynamic_cast<CBaseObject*>(list)->Set_Dead();
		}

		//
		if (CQuestMgr::RED == m_eNPCType)
		{
			/////////////////////////////////////////////////////////////////
			//// 경우의 수1 : 처음 미션 없을 때의 경우                  ////
			//// 경우의 수2 : 미션 클리어 한 후에 이버튼을 만나는 경우  //// --> 이거 하고 깃발전갈수잇게해야겟당
			/////////////////////////////////////////////////////////////////

			if (!CQuestMgr::GetInstance()->Get_MissionPossible(m_eNPCType))
			{
				if (CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
				{
					if (CQuestMgr::GetInstance()->Get_SecondMissionCompleted(m_eNPCType))
					{
						//벨라토스 깨고옴
						//이제 깃발전으로 안내할거
						// 이전 퀘스트 삭제 & 새로운 퀘스트 추가			
						CQuestMgr::GetInstance()->EraseAllQuest();	

						QUEST_INFO tQuest;
						tQuest.wstrQuest = L"검투대회 참여하기";
						tQuest.bClear = false;
						CQuestMgr::GetInstance()->AddQuest(tQuest);

						//MissionUI 의 락을 풀기위해서! 
						CUIMgr::GetInstance()->Set_MissionFlagLock(false);

						//Red 모든 퀘스트 다 수락했음()
						CQuestMgr::GetInstance()->Set_RedAllClear(true);

					}
				}
			}
			else
			{
				if (CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
				{
					// 퀘스트(짝맞추기 )성공한 후(:경우의 수 2)
					if (!CQuestMgr::GetInstance()->Get_RedAllClear())
					{
						// 이전 퀘스트 삭제 & 새로운 퀘스트 추가			
						CQuestMgr::GetInstance()->EraseAllQuest();	// 전체 퀘스트 클리어

						QUEST_INFO tQuest;
						tQuest.wstrQuest = L"두번째 무기 구하기";
						tQuest.bClear = false;
						CQuestMgr::GetInstance()->AddQuest(tQuest);

						//MissionUI 의 락을 풀기위해서! 
						CUIMgr::GetInstance()->Set_MissionBossLock(false);

						//미션x 상태로 만들어줌 --> 두번째미션부턴 미션x 상태임.즉 , m_bMissionPossible=false
						CQuestMgr::GetInstance()->Set_MissionPossible(m_eNPCType, false);
					}
					else
					{
						// 돼지천사퀘스트까지 다 받은상황
						// 그래서 뭐 없음
					}

				}
			}

			////미션x 상태로 만들어줌 --> 두번째미션부턴 미션x 상태임.즉 , m_bMissionPossible=false
			//CQuestMgr::GetInstance()->Set_MissionPossible(m_eNPCType, false);

			//npc_red 불변수 트루로 바까줘-> 다시 대화시도할수있게!
			dynamic_cast<CNPC_Red*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_MatchingAI"))->Set_OneCreateChat(true);

			////(v상호작용) 문구 그려
			//m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);

			//퀘스트 체크박스 표시
			m_pUIMgr->SetRender_QuestCheckBox(true);

			//퀘스트 체크 표시
			m_pUIMgr->SetRender_QuestCheck();

			//UI 상태 업데이트
			m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);
			m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);


		}
		else if (CQuestMgr::YELLOW == m_eNPCType)
		{
			/////////////////////////////////////////////////////////////////
			//// 경우의 수 2: 완전 초기 ////
			//// 경우의 수 2: 미션 클리어 한 후에 이버튼을 만나는 경우 뿐 ////
			/////////////////////////////////////////////////////////////////

			if (!CQuestMgr::GetInstance()->Get_MissionPossible(CQuestMgr::YELLOW))
			{
				if (!CQuestMgr::GetInstance()->Get_YellowPotalQCompleted())
				{
					//완전 처음. 포탈도 가기전
					//npc_yellow 불변수 트루로 바까줘-> 다시 대화시도할수있게!
					dynamic_cast<CNPC_Yellow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_HFPlayer"))->Set_OneCreateChat(true);

				}
				else
				{
					// 이전 퀘스트 삭제 & 새로운 퀘스트 추가			
					CQuestMgr::GetInstance()->EraseAllQuest();	// 전체 퀘스트 클리어

					QUEST_INFO tQuest;
					tQuest.wstrQuest = L"빨간머리 친구 찾기";
					tQuest.bClear = false;
					CQuestMgr::GetInstance()->AddQuest(tQuest);


					//미션x 상태로 만들어줌
					CQuestMgr::GetInstance()->Set_MissionPossible(m_eNPCType, true);

					//npc_yellow 불변수 트루로 바까줘-> 다시 대화시도할수있게!
					dynamic_cast<CNPC_Yellow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_HFPlayer"))->Set_OneCreateChat(true);

					////(v상호작용) 문구 그려
					//m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, true);

					//퀘스트 체크박스 표시
					m_pUIMgr->SetRender_QuestCheckBox(true);

					//퀘스트 체크 표시
					m_pUIMgr->SetRender_QuestCheck();

					//빨간머리 친구한테 말걸기 가능
					CQuestMgr::GetInstance()->Set_MissionPossible(CQuestMgr::RED, true);
				}
			}
			else
			{
				//if()
				//	////////////
				//	//빨간머리 찾기 퀘스트 받은 후 여기로
				//	////////////
				//	// 이전 퀘스트 삭제 & 새로운 퀘스트 추가			
				//	CQuestMgr::GetInstance()->EraseAllQuest();	// 전체 퀘스트 클리어

				//	QUEST_INFO tQuest;
				//	tQuest.wstrQuest = L"빨간머리 친구 찾기";
				//	tQuest.bClear = false;
				//	CQuestMgr::GetInstance()->AddQuest(tQuest);
				//

				////npc_yellow 불변수 트루로 바까줘-> 다시 대화시도할수있게!
				//dynamic_cast<CNPC_Yellow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_HFPlayer"))->Set_OneCreateChat(true);

				////퀘스트 체크박스 표시
				//m_pUIMgr->SetRender_QuestCheckBox(true);

			}

			//UI 상태 업데이트
			m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);
			m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);
		}

	}
}

//_bool CButton::CheckAroundMouse(HWND hWnd)
//{
//	//POINT	ptMouse{};
//
//	//GetCursorPos(&ptMouse);
//	//ScreenToClient(hWnd, &ptMouse);
//
//	//_vec3 vMousePos = _vec3((_float)ptMouse.x , (_float)ptMouse.y, 0.f);
//
//	//_vec3 vBufferVtx = m_pScreenTexBufferCom->Get_vStartPos();
//	//_vec3 vBufferSize = m_pScreenTexBufferCom->Get_vSize();
//
//	//_vec3 vVertex[4];
//	//vVertex[0] = _vec3(vBufferVtx.x , vBufferVtx.y,0.f);
//	//vVertex[1] = _vec3(vBufferVtx.x + vBufferSize.x, vBufferVtx.y, 0.f);
//	//vVertex[2] = _vec3(vBufferVtx.x + vBufferSize.x , vBufferVtx.y + vBufferSize.y, 0.f);
//	//vVertex[3] = _vec3(vBufferVtx.x , vBufferVtx.y + vBufferSize.y, 0.f);
//
//	//_vec2 vMouseDir[4] = {
//	//	vMousePos - vVertex[0],
//	//	vMousePos - vVertex[1],
//	//	vMousePos - vVertex[2],
//	//	vMousePos - vVertex[3]
//	//};
//
//	//_vec2 vLine[4] = {
//	//	vVertex[1] - vVertex[0],
//	//	vVertex[2] - vVertex[1],
//	//	vVertex[3] - vVertex[2],
//	//	vVertex[0] - vVertex[3]
//	//};
//
//	//_vec2 vNormal[4] = {};
//	//for (_uint i = 0; i < 4; ++i)
//	//{
//	//	vNormal[i] = _vec2(-vLine[i].y, vLine[i].x);
//
//	//	D3DXVec2Normalize(&vNormal[i], &vNormal[i]);
//	//	D3DXVec2Normalize(&vMouseDir[i], &vMouseDir[i]);
//	//}
//
//	//for (_uint i = 0; i < 4; ++i)
//	//{
//	//	if (0 > D3DXVec2Dot(&vNormal[i], &vMouseDir[i]))
//	//		return false;
//	//}
//
//	return true;
//}

HRESULT CNPCButton::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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
	pComponent = m_pScreenTexBufferCom = Engine::CScreenTex::Create(m_pGraphicDev, fX, fY, fSizeX, fSizeY, fViewZ);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_NPCButton"));
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

HRESULT CNPCButton::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (!m_bChoose)
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eButtonType);
	else if (m_bChoose)
		m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eButtonType + 1);

	return S_OK;
}

CNPCButton * CNPCButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, BUTTONTYPE eType, CQuestMgr::NPCTYPE eNPCType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CNPCButton* pInstance = new CNPCButton(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, eNPCType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CNPCButton::Free()
{
	CUIObject::Free();
}
