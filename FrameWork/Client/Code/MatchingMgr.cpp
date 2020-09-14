#include "stdafx.h"
#include "MatchingMgr.h"
#include "MatchingGameObserver.h"
#include "MatchingAI.h"
#include "MatchingPlayer.h"
#include "MatchingResult.h"
#include "MatchingButton.h"

IMPLEMENT_SINGLETON(CMatchingMgr)

CMatchingMgr::CMatchingMgr()
	: m_pGraphicDev(nullptr)
	, m_pManagement(Engine::CManagement::GetInstance())
	, m_dDelay(0.)
	, m_bFailed(false)
	, m_eTurn(TURN_PLAYER)
	, m_eGameState(STATE_REALEND)
	, m_iPlayerPoint(0)
	, m_iAIPoint(0)
	, m_bInit(false)
	, m_bEndGame(false)
	, m_dTime(0.)
	, m_iCombo(0)
	, m_bWaitAnim(true)
	, m_bCreateButton(true)
{
	m_vecCard.reserve(40);
	m_vecSelectedCard.reserve(2);
}


CMatchingMgr::~CMatchingMgr()
{
	CUIMgr::GetInstance()->EraseMouse();
	Engine::Safe_Release(m_pGraphicDev);
}

void CMatchingMgr::Update_MatchingMgr(const _double & dTimeDelta)
{
	LateInit();

	ObserverNotify();

	Game_Result();	// ������ �������� Ȯ��

	m_dTime -= dTimeDelta;
	_double	dAddDelay = (rand() % 4) + 2.;
	////cout << "�� : 0 == �÷��̾� , 1 == AI    :   " << m_eTurn << endl;
	// ���ӽ��۽� ī�� �����ִ� �ð� ������ �ʱ⼼���ϴºκ�
	if (m_eGameState == STATE_START && m_dDelay <= g_dAccumulatedTime)
	{
		m_eGameState = STATE_ING;
		InVisible_Card();

		for (auto iter : m_vecCard)
		{
			iter->Set_Delay(false);
		}
		m_dTime = 10.;

		//UI- Start ���� ���
		Engine::CGameObject* pGameObject = CMatchingResult::Create(m_pGraphicDev, CMatchingResult::START, true, 477.7f, 323.7f, 322.f, 80.f, 0.f);
		if (pGameObject == nullptr)
			return;
		Engine::Add_GameObject(Engine::UI, L"MatchingResult", pGameObject);

		//�÷��̾� �� ���
		CUIMgr::GetInstance()->TurnCheck(m_eTurn);
		CSoundMgr::Get_Instance()->HoSoundOn(45, 1.f);
		m_iCombo = 0;
		m_bCreateButton = true;

	}
	
	if (m_eGameState != STATE_ING)
		return;

	if (7.f >= m_dTime) //���� �߰�
	{
		if (m_bWaitAnim)
		{
			if (TURN_PLAYER == m_eTurn)
			{
				dynamic_cast<CMatchingPlayer*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingPlayer"))->Set_MatchingState(WAIT);
			}
			else if (TURN_AI == m_eTurn)
			{
				dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(WAIT);
			}


			m_bWaitAnim = false;
		}
	}

	// �ð� �ʰ��� ���� ���濡�� �Ѿ�� �κ�
	if (m_dTime <= 0.)
	{
		if (m_eTurn == TURN_PLAYER)
		{
			m_eTurn = TURN_AI;

			m_dDelay = g_dAccumulatedTime + dAddDelay;
			for (auto iter : m_vecCard)
			{
				iter->Set_Delay(true);
			}
			if (m_vecSelectedCard.size() > 0)
			{
				m_vecSelectedCard[0]->Set_Open(false);
				m_vecSelectedCard.clear();
			}		
		}
		else if (m_eTurn == TURN_AI)
		{
			m_eTurn = TURN_PLAYER;
		}

		m_dTime = 10.;

		CUIMgr::GetInstance()->TurnCheck(m_eTurn);
		
		m_iCombo = 0;
		m_bWaitAnim = true;
	}
	// =========== �÷��̾� ���� �����Լ� ============
	if (m_eTurn == TURN_PLAYER)
	{
		if (m_bFailed && m_dDelay <= g_dAccumulatedTime)
		{
			m_bFailed = false;
			m_vecSelectedCard[0]->Set_Open(false);
			m_vecSelectedCard[1]->Set_Open(false);
			m_vecSelectedCard[0]->Set_CardState(CMatchingCard::CARDNUMBER::NUM_BACK);
			m_vecSelectedCard[1]->Set_CardState(CMatchingCard::CARDNUMBER::NUM_BACK);

			m_vecSelectedCard.clear();
			m_eTurn = TURN_AI;
			m_dTime = 10.;
			m_dDelay += dAddDelay;

			CUIMgr::GetInstance()->TurnCheck(m_eTurn);
			m_iCombo = 0;
			m_bWaitAnim = true;
		}

		if (m_vecSelectedCard.size() >= 2)
		{
			if (m_vecSelectedCard[0]->Get_CardNum() == m_vecSelectedCard[1]->Get_CardNum())
			{
				CSoundMgr::Get_Instance()->SiwonSoundOn(15);

				++m_iPlayerPoint;
				++m_iCombo;

				m_tAIData[m_vecSelectedCard[0]->Get_Index()].eAIData = DATA_OPEN;
				m_tAIData[m_vecSelectedCard[1]->Get_Index()].eAIData = DATA_OPEN;

				m_vecSelectedCard.clear();
				m_dTime += 3.;

				if(0 == m_iCombo%2)
					dynamic_cast<CMatchingPlayer*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingPlayer"))->Set_MatchingState(SUCCESS_1);
				else if(1 == m_iCombo % 2)
					dynamic_cast<CMatchingPlayer*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingPlayer"))->Set_MatchingState(SUCCESS_2);

			}
			else
			{
				if (!m_bFailed)
				{
					CSoundMgr::Get_Instance()->SiwonSoundOn(16);

					m_dDelay = g_dAccumulatedTime + 0.5;
					m_bFailed = true;

					for (auto iter : m_vecCard)
					{
						iter->Set_Delay(true);
					}

					m_tAIData[m_vecSelectedCard[0]->Get_Index()].eAIData = DATA_KNOWN;
					m_tAIData[m_vecSelectedCard[1]->Get_Index()].eAIData = DATA_KNOWN;
					m_tAIData[m_vecSelectedCard[0]->Get_Index()].eCardNum = m_vecSelectedCard[0]->Get_CardNum();
					m_tAIData[m_vecSelectedCard[1]->Get_Index()].eCardNum = m_vecSelectedCard[1]->Get_CardNum();

					dynamic_cast<CMatchingPlayer*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingPlayer"))->Set_MatchingState(FAIL);

				}
			}
		}
	}
	// =========== AI ���� �����Լ� ============
	else if (m_eTurn == TURN_AI)
	{
		// AI�� Ʋ�ȴ� -> �÷��̾� ������ ����
		if (m_bFailed && m_dDelay <= g_dAccumulatedTime)
		{
			m_bFailed = false;
			m_vecCard[m_iAICard[0]]->Set_Open(false);
			m_vecCard[m_iAICard[1]]->Set_Open(false);

			for (auto iter : m_vecCard)
			{
				iter->Set_Delay(false);
			}
			m_eTurn = TURN_PLAYER;
			m_dTime = 10.;

			CUIMgr::GetInstance()->TurnCheck(m_eTurn);
			CSoundMgr::Get_Instance()->HoSoundOn(45, 1.f);
			m_iCombo = 0;
			m_bWaitAnim = true;
			return;
		}

		if (!m_bFailed && m_dDelay <= g_dAccumulatedTime)
		{
			// AI�� �ƴ� ī�尡 �ִ���, �ִٸ� ã�Ƴ���
			for (int i = 0; i < 40; i++)
			{
				for (int j = i + 1; j < 40; j++)
				{
					if (m_tAIData[i].eAIData == DATA_KNOWN  && m_tAIData[j].eAIData == DATA_KNOWN)
					{
						if ((m_tAIData[i].eCardNum == m_tAIData[j].eCardNum) &&
							((m_tAIData[i].eCardNum != CMatchingCard::CARDNUMBER::NUM_END) && (m_tAIData[j].eCardNum != CMatchingCard::CARDNUMBER::NUM_END)))
						{
							CSoundMgr::Get_Instance()->SiwonSoundOn(15);

							++m_iAIPoint;
							++m_iCombo;

							m_dDelay = g_dAccumulatedTime + dAddDelay;

							m_vecCard[i]->Set_Open(true);
							m_vecCard[j]->Set_Open(true);
							m_tAIData[i].eAIData = DATA_OPEN;
							m_tAIData[j].eAIData = DATA_OPEN;
							m_dTime += 3.;

							if (0 == m_iCombo % 2)
								dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(SUCCESS_1);
							else if (1 == m_iCombo % 2)
								dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(SUCCESS_2);

							return;
						}
					}
				}
			}

			// AI�� �ƴ�ī��� ��� �߰��ߴ�. ���� �𸣴� ī�带 Ȯ���غ����Ѵ�.
			for (int i = 0; i < 40; i++)
			{
				for (int j = i + 1; j < 40; j++)
				{
					// AI�� �𸣴�ī�尡 ���峲�Ҵ��� ī��Ʈ�ϴ� ���� �߰��ؼ� �������� �������� �װ� 1�����϶��, �ƴ°�+ �𸣴°� �ϸ��, �װ� ������ ����
					if (m_tAIData[i].eAIData == DATA_UNKNOWN  && m_tAIData[j].eAIData == DATA_UNKNOWN)
					{
						if (m_vecCard[i]->Get_CardNum() == m_vecCard[j]->Get_CardNum())
						{
							CSoundMgr::Get_Instance()->SiwonSoundOn(15);

							++m_iAIPoint;
							++m_iCombo;

							m_dDelay = g_dAccumulatedTime + dAddDelay;

							m_vecCard[i]->Set_Open(true);
							m_vecCard[j]->Set_Open(true);
							m_tAIData[i].eAIData = DATA_OPEN;
							m_tAIData[j].eAIData = DATA_OPEN;
							m_dTime += 3.;

							if (0 == m_iCombo % 2)
								dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(SUCCESS_1);
							else if (1 == m_iCombo % 2)
								dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(SUCCESS_2);



							return;
						}
						else
						{
							CSoundMgr::Get_Instance()->SiwonSoundOn(16);

							m_tAIData[i].eAIData = DATA_KNOWN;
							m_tAIData[j].eAIData = DATA_KNOWN;
							m_tAIData[i].eCardNum = m_vecCard[i]->Get_CardNum();
							m_tAIData[j].eCardNum = m_vecCard[j]->Get_CardNum();

							// �𸣴� �� ī�带 ����.
							m_iAICard[0] = i;
							m_iAICard[1] = j;
							m_vecCard[i]->Set_Open(true);
							m_vecCard[j]->Set_Open(true);

							m_dDelay = g_dAccumulatedTime + 0.5;
							m_bFailed = true;

							dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(FAIL);
							return;
						}
					}
				}
			}
			for (int i = 0; i < 40; i++)
			{
				for (int j = i + 1; j < 40; j++)
				{
					// AI�� �𸣴�ī�尡 ���峲�Ҵ��� ī��Ʈ�ϴ� ���� �߰��ؼ� �������� �������� �װ� 1�����϶��, �ƴ°�+ �𸣴°� �ϸ��, �װ� ������ ����
					if (((m_tAIData[i].eAIData == DATA_UNKNOWN) || (m_tAIData[i].eAIData == DATA_KNOWN)) && ((m_tAIData[j].eAIData == DATA_UNKNOWN) || (m_tAIData[j].eAIData == DATA_KNOWN)))
					{
						if (m_vecCard[i]->Get_CardNum() == m_vecCard[j]->Get_CardNum())
						{
							CSoundMgr::Get_Instance()->SiwonSoundOn(15);

							++m_iAIPoint;
							++m_iCombo;

							m_dDelay = g_dAccumulatedTime + dAddDelay;

							m_vecCard[i]->Set_Open(true);
							m_vecCard[j]->Set_Open(true);
							m_tAIData[i].eAIData = DATA_OPEN;
							m_tAIData[j].eAIData = DATA_OPEN;
							m_dTime += 3.;

							if (0 == m_iCombo % 2)
								dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(SUCCESS_1);
							else if (1 == m_iCombo % 2)
								dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(SUCCESS_2);

							return;
						}
						else
						{
							CSoundMgr::Get_Instance()->SiwonSoundOn(16);

							m_tAIData[i].eAIData = DATA_KNOWN;
							m_tAIData[j].eAIData = DATA_KNOWN;
							m_tAIData[i].eCardNum = m_vecCard[i]->Get_CardNum();
							m_tAIData[j].eCardNum = m_vecCard[j]->Get_CardNum();

							// �𸣴� �� ī�带 ����.
							m_iAICard[0] = i;
							m_iAICard[1] = j;
							m_vecCard[i]->Set_Open(true);
							m_vecCard[j]->Set_Open(true);

							m_dDelay = g_dAccumulatedTime + 0.5;
							m_bFailed = true;

							dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(FAIL);
							return;
						}
					}
				}
			}
		}
	}

	

	// ===================ġƮ=====================
	if (Engine::KeyPressing(DIK_0))
		Visible_Card();
	else
		InVisible_Card();
	// ===================ġƮ=====================


	// ===================�׽�Ʈ====================
	if (Engine::KeyPressing(DIK_7))
	{
		//cout << "�� : 0 == �÷��̾� , 1 == AI    :   " << m_eTurn << endl;
		//cout << "�÷��̾� ���� : " << m_iPlayerPoint << endl;
		//cout << "AI ���� : " << m_iAIPoint << endl;
		//cout << "�����ð� : " << m_dTime << endl;
	}
	if (Engine::KeyPressing(DIK_8))
	{
		m_eTurn = TURN_PLAYER;
	}
	//if (Engine::KeyPressing(DIK_9))
	//{
	//	m_eTurn = TURN_AI;
	//}
	if (Engine::KeyPressing(DIK_9)) //����
	{
		//MissionUI �� ���� Ǯ�����ؼ�!
		CUIMgr::GetInstance()->Set_MissionFlagLock(false);

	}
	

	// ===================�׽�Ʈ=====================

	return;
}

HRESULT CMatchingMgr::Set_Device(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (pGraphicDev == nullptr)
		return E_FAIL;

	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	return S_OK;
}

void CMatchingMgr::Create_Card()
{
	Delete_Card();

	////UI -��ư ���������
	CUIMgr::GetInstance()->EraseMatchingButton();

	CMatchingCard* pMatchingCard = nullptr;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			pMatchingCard = CMatchingCard::Create(m_pGraphicDev, 229.5f + 82.6f * j, 85.3f + 138.6f * i, 77.6f, 133.6f, 0.1f); // 40��
			Engine::Add_GameObject(Engine::UI, L"MatchingCard", pMatchingCard);
			m_vecCard.push_back(pMatchingCard);
		}
	}
	Shuffle_Card();
	m_iAIPoint = 0;
	m_iPlayerPoint = 0;
	m_eGameState = STATE_START;
	m_dDelay = g_dAccumulatedTime + 5.0;
	Visible_Card();
	for (auto iter : m_vecCard)
	{
		iter->Set_Delay(true);
	}
}

void CMatchingMgr::Shuffle_Card()
{
	// ī���ȣ ����
	CMatchingCard::CARDNUMBER eNum[40];
	//for (int i = 0; i < 20; i++)
	//{
	//	eNum[i] = (CMatchingCard::CARDNUMBER)(2 + i);
	//	eNum[i + 20] = (CMatchingCard::CARDNUMBER)(2 + i);
	//}

	//// ī�� ����
	//for (int i = 0; i < 40; i++)
	//{
	//	int iRan = rand() % 40;
	//	CMatchingCard::CARDNUMBER eTemp;
	//	eTemp = eNum[i];
	//	eNum[i] = eNum[iRan];
	//	eNum[iRan] = eTemp;
	//}

	eNum[0] = CMatchingCard::CARDNUMBER::NUM_1_A;
	eNum[1] = CMatchingCard::CARDNUMBER::NUM_1_B;
	eNum[2] = CMatchingCard::CARDNUMBER::NUM_2_A;
	eNum[3] = CMatchingCard::CARDNUMBER::NUM_2_B;
	eNum[4] = CMatchingCard::CARDNUMBER::NUM_1_B;

	eNum[5] = CMatchingCard::CARDNUMBER::NUM_3_A;
	eNum[6] = CMatchingCard::CARDNUMBER::NUM_3_B;
	eNum[7] = CMatchingCard::CARDNUMBER::NUM_4_A;
	eNum[8] = CMatchingCard::CARDNUMBER::NUM_2_B;
	eNum[9] = CMatchingCard::CARDNUMBER::NUM_4_B;


	eNum[10] = CMatchingCard::CARDNUMBER::NUM_1_A;
	eNum[11] = CMatchingCard::CARDNUMBER::NUM_3_B;
	eNum[12] = CMatchingCard::CARDNUMBER::NUM_5_A;
	eNum[13] = CMatchingCard::CARDNUMBER::NUM_5_A;
	eNum[14] = CMatchingCard::CARDNUMBER::NUM_5_B;

	eNum[15] = CMatchingCard::CARDNUMBER::NUM_6_A;
	eNum[16] = CMatchingCard::CARDNUMBER::NUM_6_B;
	eNum[17] = CMatchingCard::CARDNUMBER::NUM_7_A;
	eNum[18] = CMatchingCard::CARDNUMBER::NUM_4_B;
	eNum[19] = CMatchingCard::CARDNUMBER::NUM_7_A;


	eNum[20] = CMatchingCard::CARDNUMBER::NUM_5_B;
	eNum[21] = CMatchingCard::CARDNUMBER::NUM_7_B;
	eNum[22] = CMatchingCard::CARDNUMBER::NUM_8_A;
	eNum[23] = CMatchingCard::CARDNUMBER::NUM_2_A;
	eNum[24] = CMatchingCard::CARDNUMBER::NUM_7_B;

	eNum[25] = CMatchingCard::CARDNUMBER::NUM_8_B;
	eNum[26] = CMatchingCard::CARDNUMBER::NUM_9_A;
	eNum[27] = CMatchingCard::CARDNUMBER::NUM_8_A;
	eNum[28] = CMatchingCard::CARDNUMBER::NUM_9_B;
	eNum[29] = CMatchingCard::CARDNUMBER::NUM_8_B;


	eNum[30] = CMatchingCard::CARDNUMBER::NUM_6_A;
	eNum[31] = CMatchingCard::CARDNUMBER::NUM_10_A;
	eNum[32] = CMatchingCard::CARDNUMBER::NUM_10_B;
	eNum[33] = CMatchingCard::CARDNUMBER::NUM_10_A;
	eNum[34] = CMatchingCard::CARDNUMBER::NUM_9_A;

	eNum[35] = CMatchingCard::CARDNUMBER::NUM_9_B;
	eNum[36] = CMatchingCard::CARDNUMBER::NUM_10_B;
	eNum[37] = CMatchingCard::CARDNUMBER::NUM_6_B;
	eNum[38] = CMatchingCard::CARDNUMBER::NUM_4_A;
	eNum[39] = CMatchingCard::CARDNUMBER::NUM_3_A;




	// ī���ȣ ����
	int i = 0;
	for (auto iter : m_vecCard)
	{
		iter->Set_Index(i);
		iter->Set_CardNum(eNum[i++]);
	}

	// AI ����
	for (auto& iter : m_tAIData)
	{
		iter.eAIData = DATA_UNKNOWN;
		iter.eCardNum = CMatchingCard::CARDNUMBER::NUM_END;
	}

	// AI ���� �˰��ִ°� ����
	for (int i = 0; i < 10; i++)
	{
		while (true)
		{
			int iRan = rand() % 40;
			if (m_tAIData[iRan].eAIData == DATA_UNKNOWN)
			{
				m_tAIData[iRan].eAIData = DATA_KNOWN;
				m_tAIData[iRan].eCardNum = m_vecCard[iRan]->Get_CardNum();
				break;
			}
		}
	}
}

void CMatchingMgr::Delete_Card()
{
	for (auto iter : m_vecCard)
	{
		iter->Set_Dead();
	}
	m_vecCard.clear();
	//m_eGameState = STATE_END;
}

void CMatchingMgr::Add_SelectCard(CMatchingCard * pCard)
{
	if (pCard == nullptr)
		return;

	m_vecSelectedCard.push_back(pCard);
}

void CMatchingMgr::Visible_Card()
{
	for (auto iter : m_vecCard)
	{
		iter->Set_Cheat(true);
	}
}

void CMatchingMgr::InVisible_Card()
{
	for (auto iter : m_vecCard)
	{
		iter->Set_Cheat(false);
	}
}

void CMatchingMgr::RenderTime()
{
	if (0.f >= m_dTime)
		return;

	_uint iTens, iUnits = 0 ;
	iTens = (_ulong)m_dTime / 10;
	iUnits = (_ulong)m_dTime % 10;

	wstring wstrCntDown = L"";
	if (0 != iTens) //�����ڸ��� 0�� �ƴҶ� -> 2�ڸ���
	{
		wstrCntDown = to_wstring(iTens) + to_wstring(iUnits);
		Engine::Render_Font(L"Font_GODIC3", wstrCntDown, &_vec2(638.f, 2.7f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
	else //�����ڸ��� 0�϶� -> 1�ڸ���
	{
		wstrCntDown = to_wstring(iUnits);
		Engine::Render_Font(L"Font_GODIC3", wstrCntDown, &_vec2(640.f, 2.7f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}

}

void CMatchingMgr::LateInit()
{
	if (m_bInit)
		return;
	m_bInit = true;
}

void CMatchingMgr::ObserverNotify()
{
	Engine::Notify(Engine::MATCHING_GMAE, CMatchingGameObserver::PLAYER_POINT, (void*)(&m_iPlayerPoint));
	Engine::Notify(Engine::MATCHING_GMAE, CMatchingGameObserver::AI_POINT, (void*)(&m_iAIPoint));
	Engine::Notify(Engine::MATCHING_GMAE, CMatchingGameObserver::TURN, (void*)(&m_eTurn));
}

void CMatchingMgr::Game_Result()
{
	Engine::CGameObject* pGameObject = nullptr;

	if (m_eGameState == STATE_END && m_dDelay <= g_dAccumulatedTime)
	{
		Delete_Card();

		if (m_bCreateButton)
		{
			//start . end ��ư ����
			//Matching Button - START
			pGameObject = CMatchingButton::Create(m_pGraphicDev, CMatchingButton::START1, 351.8f, 500.f, 314.f, 78.f, 0.1f);
			if (pGameObject == nullptr)
				return;
			Engine::Add_GameObject(Engine::UI, L"MatchingButton", pGameObject);

			//Matching Button - EXIT
			pGameObject = CMatchingButton::Create(m_pGraphicDev, CMatchingButton::EXIT1, 660.f, 500.f, 314.f, 78.f, 0.1f);
			if (pGameObject == nullptr)
				return;
			Engine::Add_GameObject(Engine::UI, L"MatchingButton", pGameObject);

			m_bCreateButton = false;
		}

	}

	if (m_eGameState == STATE_ING &&m_iPlayerPoint + m_iAIPoint >= 20)
	{
		m_eGameState = STATE_END;
		m_dDelay = g_dAccumulatedTime + 3.f;

		if (m_iPlayerPoint > m_iAIPoint)
		{
			//�÷��̾� �¸�
			dynamic_cast<CMatchingPlayer*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingPlayer"))->Set_MatchingState(WIN);
			dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(LOSE);


			pGameObject = CMatchingResult::Create(m_pGraphicDev, CMatchingResult::WIN, true, 444.1f, 323.7f, 414.f, 78.7f, 0.f); 
			if (pGameObject == nullptr)
				return;
			Engine::Add_GameObject(Engine::UI, L"MatchingResult", pGameObject);

			//Red�� ù��° ����Ʈ�� ��������
			CQuestMgr::GetInstance()->Set_MissionCompleted(CQuestMgr::RED,true);

		}
		else if (m_iPlayerPoint < m_iAIPoint)
		{
			//AI�¸�
			dynamic_cast<CMatchingPlayer*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingPlayer"))->Set_MatchingState(LOSE);
			dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(WIN);

			pGameObject = CMatchingResult::Create(m_pGraphicDev, CMatchingResult::LOSE, true, 444.1f, 323.7f, 414.f, 78.7f, 0.f);
			if (pGameObject == nullptr)
				return;
			Engine::Add_GameObject(Engine::UI, L"MatchingResult", pGameObject);

			//Red�� ù��° ����Ʈ�� ��������
			CQuestMgr::GetInstance()->Set_MissionCompleted(CQuestMgr::RED, false);

		}
		else
		{
			//���

			pGameObject = CMatchingResult::Create(m_pGraphicDev, CMatchingResult::DRAW, true, 454.8f, 323.7f, 414.f, 78.7f, 0.f);
			if (pGameObject == nullptr)
				return;
			Engine::Add_GameObject(Engine::UI, L"MatchingResult", pGameObject);

			//Red�� ù��° ����Ʈ�� ��������
			CQuestMgr::GetInstance()->Set_MissionCompleted(CQuestMgr::RED, true);

		}
	}


}
