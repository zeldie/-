#include "stdafx.h"
#include "QuestBackBar.h"
#include "ScreenTex.h"
#include "NPCButton.h"

CQuestBackBar::CQuestBackBar(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_pQuestMgr(CQuestMgr::GetInstance()),
	m_eQuestBackBarType(QUESTBACKBAR_END),
	m_bLateInit(true),
	m_eNPCType(CQuestMgr::NPCTYPE_END),
	m_wstrQuestNarration(L""),
	m_bTwoLine(false)
{
	//fx = 0.f;
	//fy= 0.f;
}


CQuestBackBar::~CQuestBackBar()
{
}

HRESULT CQuestBackBar::Ready_GameObject(QUESTBACKBARTYPE eType, CQuestMgr::NPCTYPE eNPCType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eQuestBackBarType = eType;
	m_eNPCType = eNPCType;
	m_bRenderUI = bBool;

	return S_OK;
}

_int CQuestBackBar::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{
		if (CHAT == m_eQuestBackBarType)
		{
			list<Engine::CGameObject*> pList = Engine::Get_GameObjectlist(Engine::UI, L"NPCButton");
			for (auto& list : pList)
			{
				dynamic_cast<CBaseObject*>(list)->Set_Dead();
			}
		}
		return Engine::OBJ_DEAD;
	}

	if (m_bLateInit)
	{
		if (CHAT == m_eQuestBackBarType)
		{
			Engine::CGameObject* pGameObject = nullptr;

			if (CQuestMgr::GetInstance()->Get_MissionPossible(m_eNPCType))
			{
				//미션 ㅇ				
				if (CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
				{
					if (CQuestMgr::RED == m_eNPCType)
					{
						if (CQuestMgr::GetInstance()->Get_SecondMissionCompleted(m_eNPCType))
						{
							//모든 퀘스트 수락한경우
							if (CQuestMgr::GetInstance()->Get_RedAllClear())
							{
								//버튼 - 나가기
								pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::OUTBUTTON1, m_eNPCType, 1144.4f, 430.3f, 99.2f, 41.1f, 0.1f); //1144.4f, 630.3f
								if (pGameObject == nullptr)
									return E_FAIL;
								Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);
							}
							//아닌경우
							else
							{
								//버튼 - 수락/거절
								pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::AGREE1, m_eNPCType, 1040.9f, 430.3f, 99.2f, 41.1f, 0.1f); //1040.9f, 630.3f
								if (pGameObject == nullptr)
									return E_FAIL;
								Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);

								pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::DISAGREE1, m_eNPCType, 1144.4f, 430.3f, 99.2f, 41.1f, 0.1f); //1144.4f, 630.3f
								if (pGameObject == nullptr)
									return E_FAIL;
								Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);
							}
						}
						else
						{
							//버튼 - 나가기
							pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::OUTBUTTON1, m_eNPCType, 1144.4f, 430.3f, 99.2f, 41.1f, 0.1f); //1144.4f, 630.3f
							if (pGameObject == nullptr)
								return E_FAIL;
							Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);

						}

					}
					else if (CQuestMgr::YELLOW == m_eNPCType)
					{
						//버튼 - 나가기
						pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::OUTBUTTON1, m_eNPCType, 1144.4f, 430.3f, 99.2f, 41.1f, 0.1f); //1144.4f, 630.3f
						if (pGameObject == nullptr)
							return E_FAIL;
						Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);

					}
				}
				else
				{
					//미션 클리어 x

					//버튼 - 수락/거절
					pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::AGREE1, m_eNPCType, 1040.9f, 430.3f, 99.2f, 41.1f, 0.1f); // 1040.9f, 630.3f
					if (pGameObject == nullptr)
						return E_FAIL;
					Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);

					pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::DISAGREE1, m_eNPCType, 1144.4f, 430.3f, 99.2f, 41.1f, 0.1f); //1144.4f, 630.3f
					if (pGameObject == nullptr)
						return E_FAIL;
					Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);
				}


			}
			else
			{
				//미션 x
				if (CQuestMgr::RED == m_eNPCType)
				{
					//퀘스트(벨라토스) 성공 후 
					if (CQuestMgr::GetInstance()->Get_SecondMissionCompleted(m_eNPCType))
					{
						//	//모든 퀘스트 수락한경우
						if (CQuestMgr::GetInstance()->Get_RedAllClear())
						{
							//버튼 - 나가기
							pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::OUTBUTTON1, m_eNPCType, 1144.4f, 430.3f, 99.2f, 41.1f, 0.1f); //1144.4f, 630.3f
							if (pGameObject == nullptr)
								return E_FAIL;
							Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);
						}
						//아닌경우
						else //여기서 깃발전참여할수잇다고 알려줄거야.
						{
							//버튼 - 나가기
							pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::OUTBUTTON1, m_eNPCType, 1144.4f, 430.3f, 99.2f, 41.1f, 0.1f); //1144.4f, 630.3f
							if (pGameObject == nullptr)
								return E_FAIL;
							Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);
						}

					}
					//퀘스트(벨라토스) 성공 전
					else
					{
						//버튼 - 나가기
						pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::OUTBUTTON1, m_eNPCType, 1144.4f, 430.3f, 99.2f, 41.1f, 0.1f);  //1144.4f, 630.3f
						if (pGameObject == nullptr)
							return E_FAIL;
						Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);
					}
				}
				else if (CQuestMgr::YELLOW == m_eNPCType)
				{
					if (CQuestMgr::GetInstance()->Get_YellowPotalQCompleted())
					{
						if (CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
						{
							//버튼 - 나가기
							pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::OUTBUTTON1, m_eNPCType, 1144.4f, 430.3f, 99.2f, 41.1f, 0.1f);  //1144.4f, 630.3f
							if (pGameObject == nullptr)
								return E_FAIL;
							Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);
						}
						else
						{
							//버튼 - 수락/거절
							pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::AGREE1, m_eNPCType, 1040.9f, 430.3f, 99.2f, 41.1f, 0.1f); // 1040.9f, 630.3f
							if (pGameObject == nullptr)
								return E_FAIL;
							Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);

							pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::DISAGREE1, m_eNPCType, 1144.4f, 430.3f, 99.2f, 41.1f, 0.1f); //1144.4f, 630.3f
							if (pGameObject == nullptr)
								return E_FAIL;
							Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);

						}
					}
					else
					{
						//버튼 - 나가기
						pGameObject = CNPCButton::Create(m_pGraphicDev, CNPCButton::OUTBUTTON1, m_eNPCType, 1144.4f, 430.3f, 99.2f, 41.1f, 0.1f);  //1144.4f, 630.3f
						if (pGameObject == nullptr)
							return E_FAIL;
						Engine::Add_GameObject(Engine::UI, L"NPCButton", pGameObject);

					}
				}

			}
		}
		else if (QUEST == m_eQuestBackBarType)
		{

		}
		m_bLateInit = false;
	}

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CQuestBackBar::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (CHAT == m_eQuestBackBarType)
	{
		CheckQuestNarration();
	}
	else if (QUEST == m_eQuestBackBarType)
	{
		//렌더에서
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CQuestBackBar::Render_Geometry(const _double & dTimeDelta)
{
	if (m_bRenderUI) //->quest창 껏다켯다 하고싶어서 추가했음
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

		pEffect->BeginPass(5);

		m_pScreenTexBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);

		//대사
		if (CHAT == m_eQuestBackBarType)
		{
			Engine::Render_Font(L"Font_GODIC3", m_wstrQuestNarration, &_vec2(574.5f, 300.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //574.5f,500.f
		}
		else if (QUEST == m_eQuestBackBarType)
		{
			CheckQuest();
		}
		//
	}
}

void CQuestBackBar::CheckQuestNarration()
{
	if (CQuestMgr::RED == m_eNPCType)
	{
		if (CQuestMgr::GetInstance()->Get_MissionPossible(m_eNPCType))
		{
			if (CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType)) //짝맞추기 깨고옴
			{
				if (CQuestMgr::GetInstance()->Get_SecondMissionCompleted(m_eNPCType)) //깃발전 깨고옴
				{
					if (CQuestMgr::GetInstance()->Get_RedAllClear()) //모든 퀘스트 다 수락
						m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"RedAllClear");
					else //퀘스트 남아있음(돼지천사)
						m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"RedSecondCompleted");
				}
				else
					m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"RedCompleted");

			}
			else //미션ㅇ -> 짝맞추기 아직안함
				m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"RedCanGive");
		}
		else // 미션 x
		{
			if (CQuestMgr::GetInstance()->Get_SecondMissionCompleted(m_eNPCType)) //깃발전 깨고옴
			{
				if (CQuestMgr::GetInstance()->Get_RedAllClear()) //모든 퀘스트 다 수락
					m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"RedAllClear");
				else //퀘스트 남아있음(돼지천사)
					m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"RedSecondCompleted");

			}
			else
				m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"RedCantGive");
		}
	}
	else if (CQuestMgr::YELLOW == m_eNPCType)
	{
		if (CQuestMgr::GetInstance()->Get_MissionPossible(m_eNPCType))
		{
			//if(CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
			//	m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"YellowCompleted");
			//else //미션 ㅇ -> 
			m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"YellowAfterPotal");
		}
		else // 미션 x
		{
			if (CQuestMgr::GetInstance()->Get_YellowPotalQCompleted())
			{
				if (CQuestMgr::GetInstance()->Get_MissionCompleted(m_eNPCType))
				{
					m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"YellowCompleted");
				}
				else
				{
					m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"YellowPortalCompleted");
					m_bTwoLine = true;
				}
			}
			else
			{
				m_wstrQuestNarration = m_pQuestMgr->Get_QuestNarration(L"YellowBeforePortal");
			}

		}
	}
}

void CQuestBackBar::CheckQuest()
{
	vector<QUEST_INFO> vecQ = m_pQuestMgr->Get_vecQuest();
	wstring wstrQ;

	if (m_bTwoLine)
	{
		for (_uint i = 0; i < vecQ.size(); ++i)
		{
			wstrQ = vecQ[i].wstrQuest;
			Engine::Render_Font(L"Font_GODIC2", wstrQ, &_vec2(1070.f, 180.f + (i*85.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //1070.f, 230.f + (i*25.f)
		}
		m_bTwoLine = false;
	}
	else
	{
		for (_uint i = 0; i < vecQ.size(); ++i)
		{
			wstrQ = vecQ[i].wstrQuest;
			Engine::Render_Font(L"Font_GODIC2", wstrQ, &_vec2(1070.f, 180.f + (i*25.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //1070.f, 230.f + (i*25.f)
		}
	}

}

HRESULT CQuestBackBar::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Quest"));
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

HRESULT CQuestBackBar::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (CHAT == m_eQuestBackBarType)
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 1.f, 1.f));
	else if (NPCPICTURE == m_eQuestBackBarType
		|| NPCPICTURE_YELLOW == m_eQuestBackBarType)
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 1.f, 1.f));
	else if (QUEST == m_eQuestBackBarType)
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 1.f, 0.8f));

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eQuestBackBarType);

	return S_OK;
}

CQuestBackBar * CQuestBackBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, QUESTBACKBARTYPE eType, CQuestMgr::NPCTYPE eNPCType, _bool bBool, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CQuestBackBar* pInstance = new CQuestBackBar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, eNPCType, bBool, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CQuestBackBar::Free()
{
	CUIObject::Free();
}
