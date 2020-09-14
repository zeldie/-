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

	Game_Result();	// 게임이 끝났는지 확인

	m_dTime -= dTimeDelta;
	_double	dAddDelay = (rand() % 4) + 2.;
	////cout << "턴 : 0 == 플레이어 , 1 == AI    :   " << m_eTurn << endl;
	// 게임시작시 카드 보여주는 시간 끝나고 초기세팅하는부분
	if (m_eGameState == STATE_START && m_dDelay <= g_dAccumulatedTime)
	{
		m_eGameState = STATE_ING;
		InVisible_Card();

		for (auto iter : m_vecCard)
		{
			iter->Set_Delay(false);
		}
		m_dTime = 10.;

		//UI- Start 문구 띄움
		Engine::CGameObject* pGameObject = CMatchingResult::Create(m_pGraphicDev, CMatchingResult::START, true, 477.7f, 323.7f, 322.f, 80.f, 0.f);
		if (pGameObject == nullptr)
			return;
		Engine::Add_GameObject(Engine::UI, L"MatchingResult", pGameObject);

		//플레이어 턴 띄움
		CUIMgr::GetInstance()->TurnCheck(m_eTurn);
		CSoundMgr::Get_Instance()->HoSoundOn(45, 1.f);
		m_iCombo = 0;
		m_bCreateButton = true;

	}
	
	if (m_eGameState != STATE_ING)
		return;

	if (7.f >= m_dTime) //희정 추가
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

	// 시간 초과로 턴이 상대방에게 넘어가는 부분
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
	// =========== 플레이어 차례 관련함수 ============
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
	// =========== AI 차례 관련함수 ============
	else if (m_eTurn == TURN_AI)
	{
		// AI가 틀렸다 -> 플레이어 턴으로 변경
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
			// AI가 아는 카드가 있는지, 있다면 찾아낸다
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

			// AI가 아는카드는 모두 발견했다. 이제 모르는 카드를 확인해봐야한다.
			for (int i = 0; i < 40; i++)
			{
				for (int j = i + 1; j < 40; j++)
				{
					// AI가 모르는카드가 몇장남았는지 카운트하는 변수 추가해서 조건으로 설정하자 그게 1장이하라면, 아는거+ 모르는거 하면됨, 그게 마지막 한장
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

							// 모르는 두 카드를 오픈.
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
					// AI가 모르는카드가 몇장남았는지 카운트하는 변수 추가해서 조건으로 설정하자 그게 1장이하라면, 아는거+ 모르는거 하면됨, 그게 마지막 한장
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

							// 모르는 두 카드를 오픈.
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

	

	// ===================치트=====================
	if (Engine::KeyPressing(DIK_0))
		Visible_Card();
	else
		InVisible_Card();
	// ===================치트=====================


	// ===================테스트====================
	if (Engine::KeyPressing(DIK_7))
	{
		//cout << "턴 : 0 == 플레이어 , 1 == AI    :   " << m_eTurn << endl;
		//cout << "플레이어 점수 : " << m_iPlayerPoint << endl;
		//cout << "AI 점수 : " << m_iAIPoint << endl;
		//cout << "남은시간 : " << m_dTime << endl;
	}
	if (Engine::KeyPressing(DIK_8))
	{
		m_eTurn = TURN_PLAYER;
	}
	//if (Engine::KeyPressing(DIK_9))
	//{
	//	m_eTurn = TURN_AI;
	//}
	if (Engine::KeyPressing(DIK_9)) //희정
	{
		//MissionUI 의 락을 풀기위해서!
		CUIMgr::GetInstance()->Set_MissionFlagLock(false);

	}
	

	// ===================테스트=====================

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

	////UI -버튼 지워줘야해
	CUIMgr::GetInstance()->EraseMatchingButton();

	CMatchingCard* pMatchingCard = nullptr;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			pMatchingCard = CMatchingCard::Create(m_pGraphicDev, 229.5f + 82.6f * j, 85.3f + 138.6f * i, 77.6f, 133.6f, 0.1f); // 40장
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
	// 카드번호 생성
	CMatchingCard::CARDNUMBER eNum[40];
	//for (int i = 0; i < 20; i++)
	//{
	//	eNum[i] = (CMatchingCard::CARDNUMBER)(2 + i);
	//	eNum[i + 20] = (CMatchingCard::CARDNUMBER)(2 + i);
	//}

	//// 카드 섞기
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




	// 카드번호 설정
	int i = 0;
	for (auto iter : m_vecCard)
	{
		iter->Set_Index(i);
		iter->Set_CardNum(eNum[i++]);
	}

	// AI 설정
	for (auto& iter : m_tAIData)
	{
		iter.eAIData = DATA_UNKNOWN;
		iter.eCardNum = CMatchingCard::CARDNUMBER::NUM_END;
	}

	// AI 최초 알고있는거 설정
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
	if (0 != iTens) //십의자리가 0이 아닐때 -> 2자릿수
	{
		wstrCntDown = to_wstring(iTens) + to_wstring(iUnits);
		Engine::Render_Font(L"Font_GODIC3", wstrCntDown, &_vec2(638.f, 2.7f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
	else //십의자리가 0일때 -> 1자릿수
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
			//start . end 버튼 생성
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
			//플레이어 승리
			dynamic_cast<CMatchingPlayer*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingPlayer"))->Set_MatchingState(WIN);
			dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(LOSE);


			pGameObject = CMatchingResult::Create(m_pGraphicDev, CMatchingResult::WIN, true, 444.1f, 323.7f, 414.f, 78.7f, 0.f); 
			if (pGameObject == nullptr)
				return;
			Engine::Add_GameObject(Engine::UI, L"MatchingResult", pGameObject);

			//Red의 첫번째 퀘스트를 성공했음
			CQuestMgr::GetInstance()->Set_MissionCompleted(CQuestMgr::RED,true);

		}
		else if (m_iPlayerPoint < m_iAIPoint)
		{
			//AI승리
			dynamic_cast<CMatchingPlayer*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingPlayer"))->Set_MatchingState(LOSE);
			dynamic_cast<CMatchingAI*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"MatchingAI"))->Set_MatchingState(WIN);

			pGameObject = CMatchingResult::Create(m_pGraphicDev, CMatchingResult::LOSE, true, 444.1f, 323.7f, 414.f, 78.7f, 0.f);
			if (pGameObject == nullptr)
				return;
			Engine::Add_GameObject(Engine::UI, L"MatchingResult", pGameObject);

			//Red의 첫번째 퀘스트를 실패했음
			CQuestMgr::GetInstance()->Set_MissionCompleted(CQuestMgr::RED, false);

		}
		else
		{
			//비김

			pGameObject = CMatchingResult::Create(m_pGraphicDev, CMatchingResult::DRAW, true, 454.8f, 323.7f, 414.f, 78.7f, 0.f);
			if (pGameObject == nullptr)
				return;
			Engine::Add_GameObject(Engine::UI, L"MatchingResult", pGameObject);

			//Red의 첫번째 퀘스트를 성공했음
			CQuestMgr::GetInstance()->Set_MissionCompleted(CQuestMgr::RED, true);

		}
	}


}
