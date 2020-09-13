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
			//// ����� ��1 : ¦���߱� �ҷ��� ������ ���             ////
			//// ����� ��2 :            //// 
			/////////////////////////////////////////////////////////////////

			if (!CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
			{
				//����� �� 1 �ش�

				// ���� ����Ʈ ���� & ���ο� ����Ʈ �߰�			
				CQuestMgr::GetInstance()->EraseAllQuest();

				QUEST_INFO tQuest;
				tQuest.wstrQuest = L"�����Ӹ� �̱��";
				tQuest.bClear = false;
				CQuestMgr::GetInstance()->AddQuest(tQuest);

				//����Ʈ üũ �ڽ� ǥ��
				m_pUIMgr->SetRender_QuestCheckBox(true);

				//����Ʈ üũ ǥ��
				m_pUIMgr->SetRender_QuestCheck();

				//¦���߱� ����
				m_bSceneChange = true;
			}
			else
			{
				if (CQuestMgr::GetInstance()->Get_SecondMissionCompleted(m_eNPCType))
				{
					////����� �� 2 �ش�

					//// ���� ����Ʈ ���� & ���ο� ����Ʈ �߰�			
					//CQuestMgr::GetInstance()->EraseAllQuest();

					//QUEST_INFO tQuest;
					//tQuest.wstrQuest = L"������ ���� óġ�ϱ�";
					//tQuest.bClear = false;
					//CQuestMgr::GetInstance()->AddQuest(tQuest);

					////Red ��� ����Ʈ �� ��������()
					//CQuestMgr::GetInstance()->Set_RedAllClear(true);

					////��ȭâ ����->�׾ȿ��� ��ư����(����Ʈâ�� ����)
					//list<Engine::CGameObject*> pList = Engine::Get_GameObjectlist(Engine::UI, L"QuestBackBar");
					//for (auto& list : pList)
					//{
					//	if (CQuestBackBar::QUEST != dynamic_cast<CQuestBackBar*>(list)->Get_QuestBackBarType())
					//		dynamic_cast<CBaseObject*>(list)->Set_Dead();
					//}

					////npc_red �Һ��� Ʈ��� �ٱ���-> �ٽ� ��ȭ�õ��Ҽ��ְ�!
					//dynamic_cast<CNPC_Red*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_MatchingAI"))->Set_OneCreateChat(true);

					////����Ʈ üũ �ڽ� ǥ��
					//m_pUIMgr->SetRender_QuestCheckBox(true);

					////����Ʈ üũ ǥ��
					//m_pUIMgr->SetRender_QuestCheck();

					////UI ���� ������Ʈ
					//m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);
					//m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);
				}

			}
		}
		else if (CQuestMgr::YELLOW == m_eNPCType)
		{
			/////////////////////////////////////////////
			//â�� ������ ����õ�� �Ϸ����ž�
			/////////////////////////////////////////////

			// ���� ����Ʈ ���� & ���ο� ����Ʈ �߰�
			CQuestMgr::GetInstance()->EraseAllQuest();

			QUEST_INFO tQuest;
			tQuest.wstrQuest = L"���� ������ ���� ���ϱ�";
			tQuest.bClear = false;
			CQuestMgr::GetInstance()->AddQuest(tQuest);

			//��ȭâ ����->�׾ȿ��� ��ư����(����Ʈâ�� ����)
			list<Engine::CGameObject*> pList = Engine::Get_GameObjectlist(Engine::UI, L"QuestBackBar");
			for (auto& list : pList)
			{
				if (CQuestBackBar::QUEST != dynamic_cast<CQuestBackBar*>(list)->Get_QuestBackBarType())
					dynamic_cast<CBaseObject*>(list)->Set_Dead();
			}

			//npc_red �Һ��� Ʈ��� �ٱ���-> �ٽ� ��ȭ�õ��Ҽ��ְ�!
			dynamic_cast<CNPC_Red*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_MatchingAI"))->Set_OneCreateChat(true);

			////(v��ȣ�ۿ�) ���� �׷�
			//m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, true);

			//MissionUI �� ���� Ǯ�����ؼ�! 
			CUIMgr::GetInstance()->Set_MissionApostleLock(false);

			//����Ʈ üũ �ڽ� ǥ��
			m_pUIMgr->SetRender_QuestCheckBox(true);

			//����Ʈ üũ ǥ��
			m_pUIMgr->SetRender_QuestCheck();

			//UI ���� ������Ʈ
			m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);
			m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);
		}
	}
	else if (DISAGREE1 == m_eButtonType)
	{
		/////////////////////////////////////////////////////////////////
		//// ����� ��1 : ¦���߱� �ҷ��� ������ ���             ////
		//// ����� ��2 : ����� ���� �Ŀ� ������ ���            //// 
		/////////////////////////////////////////////////////////////////


		//��ȭâ ����->�׾ȿ��� ��ư����(����Ʈâ�� ����)
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
						//�����ִ� ��� �˷���µ� ���� �������						
						CQuestMgr::GetInstance()->EraseAllQuest();

						QUEST_INFO tQuest;
						tQuest.wstrQuest = L"�ι�° ���� ���ϱ�";
						tQuest.bClear = false;
						CQuestMgr::GetInstance()->AddQuest(tQuest);
					}
				}
			}
			else
			{
				//�����̱�� �ʴ��� ����! ����ߴµ� ���� �������
				CQuestMgr::GetInstance()->EraseAllQuest();

				QUEST_INFO tQuest;
				tQuest.wstrQuest = L"�����Ӹ� �̱��";
				tQuest.bClear = false;
				CQuestMgr::GetInstance()->AddQuest(tQuest);
			}

			//npc_red �Һ��� Ʈ��� �ٱ���-> �ٽ� ��ȭ�õ��Ҽ��ְ�!
			dynamic_cast<CNPC_Red*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_MatchingAI"))->Set_OneCreateChat(true);
		}
		else if (CQuestMgr::YELLOW == m_eNPCType)
		{
			if (!CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
			{
				//�����ٳ�� �ߴµ� ������ ���				
				CQuestMgr::GetInstance()->EraseAllQuest();

				QUEST_INFO tQuest;
				tQuest.wstrQuest = L"����Ӹ� ģ������ ���ɱ�";
				tQuest.bClear = false;
				CQuestMgr::GetInstance()->AddQuest(tQuest);
			}

			//npc_yellow �Һ��� Ʈ��� �ٱ���-> �ٽ� ��ȭ�õ��Ҽ��ְ�!
			dynamic_cast<CNPC_Yellow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_HFPlayer"))->Set_OneCreateChat(true);

		}

		////(v��ȣ�ۿ�) ���� �׷�
		//m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, true);

		////���� ����Ʈ ���� ����
		//CQuestMgr::GetInstance()->EraseAllQuest();

		//����Ʈ üũ �ڽ� ǥ��
		m_pUIMgr->SetRender_QuestCheckBox(true);

		//����Ʈ üũ ǥ��
		m_pUIMgr->SetRender_QuestCheck();

		//UI ���� ������Ʈ
		m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);
		m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);
	}
	else if (OUTBUTTON1 == m_eButtonType)
	{
		//��ȭâ ����->�׾ȿ��� ��ư����(����Ʈâ�� ����)
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
			//// ����� ��1 : ó�� �̼� ���� ���� ���                  ////
			//// ����� ��2 : �̼� Ŭ���� �� �Ŀ� �̹�ư�� ������ ���  //// --> �̰� �ϰ� ����������հ��ؾ߰ٴ�
			/////////////////////////////////////////////////////////////////

			if (!CQuestMgr::GetInstance()->Get_MissionPossible(m_eNPCType))
			{
				if (CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
				{
					if (CQuestMgr::GetInstance()->Get_SecondMissionCompleted(m_eNPCType))
					{
						//�����佺 �����
						//���� ��������� �ȳ��Ұ�
						// ���� ����Ʈ ���� & ���ο� ����Ʈ �߰�			
						CQuestMgr::GetInstance()->EraseAllQuest();	

						QUEST_INFO tQuest;
						tQuest.wstrQuest = L"������ȸ �����ϱ�";
						tQuest.bClear = false;
						CQuestMgr::GetInstance()->AddQuest(tQuest);

						//MissionUI �� ���� Ǯ�����ؼ�! 
						CUIMgr::GetInstance()->Set_MissionFlagLock(false);

						//Red ��� ����Ʈ �� ��������()
						CQuestMgr::GetInstance()->Set_RedAllClear(true);

					}
				}
			}
			else
			{
				if (CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
				{
					// ����Ʈ(¦���߱� )������ ��(:����� �� 2)
					if (!CQuestMgr::GetInstance()->Get_RedAllClear())
					{
						// ���� ����Ʈ ���� & ���ο� ����Ʈ �߰�			
						CQuestMgr::GetInstance()->EraseAllQuest();	// ��ü ����Ʈ Ŭ����

						QUEST_INFO tQuest;
						tQuest.wstrQuest = L"�ι�° ���� ���ϱ�";
						tQuest.bClear = false;
						CQuestMgr::GetInstance()->AddQuest(tQuest);

						//MissionUI �� ���� Ǯ�����ؼ�! 
						CUIMgr::GetInstance()->Set_MissionBossLock(false);

						//�̼�x ���·� ������� --> �ι�°�̼Ǻ��� �̼�x ������.�� , m_bMissionPossible=false
						CQuestMgr::GetInstance()->Set_MissionPossible(m_eNPCType, false);
					}
					else
					{
						// ����õ������Ʈ���� �� ������Ȳ
						// �׷��� �� ����
					}

				}
			}

			////�̼�x ���·� ������� --> �ι�°�̼Ǻ��� �̼�x ������.�� , m_bMissionPossible=false
			//CQuestMgr::GetInstance()->Set_MissionPossible(m_eNPCType, false);

			//npc_red �Һ��� Ʈ��� �ٱ���-> �ٽ� ��ȭ�õ��Ҽ��ְ�!
			dynamic_cast<CNPC_Red*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_MatchingAI"))->Set_OneCreateChat(true);

			////(v��ȣ�ۿ�) ���� �׷�
			//m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, false);

			//����Ʈ üũ�ڽ� ǥ��
			m_pUIMgr->SetRender_QuestCheckBox(true);

			//����Ʈ üũ ǥ��
			m_pUIMgr->SetRender_QuestCheck();

			//UI ���� ������Ʈ
			m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);
			m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);


		}
		else if (CQuestMgr::YELLOW == m_eNPCType)
		{
			/////////////////////////////////////////////////////////////////
			//// ����� �� 2: ���� �ʱ� ////
			//// ����� �� 2: �̼� Ŭ���� �� �Ŀ� �̹�ư�� ������ ��� �� ////
			/////////////////////////////////////////////////////////////////

			if (!CQuestMgr::GetInstance()->Get_MissionPossible(CQuestMgr::YELLOW))
			{
				if (!CQuestMgr::GetInstance()->Get_YellowPotalQCompleted())
				{
					//���� ó��. ��Ż�� ������
					//npc_yellow �Һ��� Ʈ��� �ٱ���-> �ٽ� ��ȭ�õ��Ҽ��ְ�!
					dynamic_cast<CNPC_Yellow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_HFPlayer"))->Set_OneCreateChat(true);

				}
				else
				{
					// ���� ����Ʈ ���� & ���ο� ����Ʈ �߰�			
					CQuestMgr::GetInstance()->EraseAllQuest();	// ��ü ����Ʈ Ŭ����

					QUEST_INFO tQuest;
					tQuest.wstrQuest = L"�����Ӹ� ģ�� ã��";
					tQuest.bClear = false;
					CQuestMgr::GetInstance()->AddQuest(tQuest);


					//�̼�x ���·� �������
					CQuestMgr::GetInstance()->Set_MissionPossible(m_eNPCType, true);

					//npc_yellow �Һ��� Ʈ��� �ٱ���-> �ٽ� ��ȭ�õ��Ҽ��ְ�!
					dynamic_cast<CNPC_Yellow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_HFPlayer"))->Set_OneCreateChat(true);

					////(v��ȣ�ۿ�) ���� �׷�
					//m_pUIMgr->SetRender_NPCNoFuncObj(CNPCNoFuncObj::VKEY_TXT, m_eNPCType, true);

					//����Ʈ üũ�ڽ� ǥ��
					m_pUIMgr->SetRender_QuestCheckBox(true);

					//����Ʈ üũ ǥ��
					m_pUIMgr->SetRender_QuestCheck();

					//�����Ӹ� ģ������ ���ɱ� ����
					CQuestMgr::GetInstance()->Set_MissionPossible(CQuestMgr::RED, true);
				}
			}
			else
			{
				//if()
				//	////////////
				//	//�����Ӹ� ã�� ����Ʈ ���� �� �����
				//	////////////
				//	// ���� ����Ʈ ���� & ���ο� ����Ʈ �߰�			
				//	CQuestMgr::GetInstance()->EraseAllQuest();	// ��ü ����Ʈ Ŭ����

				//	QUEST_INFO tQuest;
				//	tQuest.wstrQuest = L"�����Ӹ� ģ�� ã��";
				//	tQuest.bClear = false;
				//	CQuestMgr::GetInstance()->AddQuest(tQuest);
				//

				////npc_yellow �Һ��� Ʈ��� �ٱ���-> �ٽ� ��ȭ�õ��Ҽ��ְ�!
				//dynamic_cast<CNPC_Yellow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Mesh_HFPlayer"))->Set_OneCreateChat(true);

				////����Ʈ üũ�ڽ� ǥ��
				//m_pUIMgr->SetRender_QuestCheckBox(true);

			}

			//UI ���� ������Ʈ
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
