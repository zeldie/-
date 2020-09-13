#include "stdafx.h"
#include "QuestMgr.h"

IMPLEMENT_SINGLETON(CQuestMgr)

CQuestMgr::CQuestMgr()
	:	//m_eNPCType(NPCTYPE_END),
		m_bNPCRed_MissionPossible(false),
		m_bNPCYellow_MissionPossible(false), //이전:true
		m_bNPCRed_MissionCompleted(false),
		m_bNPCYellow_MissionCompleted(false),
		m_bNPCYellow_PotalQCompleted(false),
		m_bNPCRed_SecondMissionCompleted(false),
		m_iRedQuestCnt(6), //이전:6
		m_iYellowQuestCnt(4), //이전:3
		m_bNPCRedAllClear(false),
		m_bOnly(true)
{
	m_vecQuest.reserve(2);
}


CQuestMgr::~CQuestMgr()
{
	m_mapQuestNarration.clear();
	m_vecQuest.clear();
}

void CQuestMgr::AddQuestNarration()
{
	m_mapQuestNarration.emplace(L"RedCanGive", L"무기있는 장소를 알려달라고?\n내가 그렇게 쉽게 알려줄 것 같아?\n나를 이기면 알려주지!");
	m_mapQuestNarration.emplace(L"RedCantGive",L"이렇게 좋은 날씨에 대회 준비라니..\n좋구만......");
	m_mapQuestNarration.emplace(L"RedCompleted", L"오~ 좀 하는데?\n근처에 있는 몬스터한테 그 무기가 있어\n포탈을 통해서 몬스터가 있는 곳으로 이동할 수 있어!");
	m_mapQuestNarration.emplace(L"RedSecondCompleted", L"진짜 구해왔네?\n이제 대회 참가가 가능하겠는데!");
	m_mapQuestNarration.emplace(L"RedAllClear", L"너는 할 수 있어!!");

	m_mapQuestNarration.emplace(L"YellowAfterPotal", L"무술대회 준비때문에 너무 바쁘네요..");
	m_mapQuestNarration.emplace(L"YellowBeforePortal", L"무술대회 준비때문에 너무 바쁘네요..");
	m_mapQuestNarration.emplace(L"YellowCompleted", L"구하기 힘드셨을텐데.. 대단해요!\n무기가 한개 더 필요한거죠?\n그럼 빨간머리 친구를 찾아가보세요~");
	m_mapQuestNarration.emplace(L"YellowPortalCompleted", L"무기가 필요하다구요?\n무기는 마을 끝 화산섬에 있는데..거긴 사람이 못가요!\n근처에 변신능력이 있는 몬스터가 있다고하던데\n그 몬스터를 잡으면 변신해서 갈 수 있을거에요");

}

void CQuestMgr::AddQuest(QUEST_INFO QuestInfo)
{
	m_vecQuest.emplace_back(QuestInfo);
}

void CQuestMgr::EraseQuest()
{
	for(_uint i = 0; i < m_vecQuest.size(); ++i)
	{
		if (m_vecQuest[i].bClear)
		{
			m_vecQuest.erase(m_vecQuest.begin() + i);
			break;
		}
	}
}

void CQuestMgr::EraseAllQuest()
{
	m_vecQuest.clear();
	CUIMgr::GetInstance()->SetRender_QuestCheck();
}

void CQuestMgr::SetQuestClear()
{
	if (0.f >= m_vecQuest.size())
		return;

	//그냥..무조건 앞에있는걸로 일단..
	m_vecQuest.front().bClear = true;
}

void CQuestMgr::SetQuestAllClear()
{
	//모든 퀘스트 클리어
	for (auto& pVec : m_vecQuest)
	{
		pVec.bClear = true;
	}
}

void CQuestMgr::MakeFirstQuest()
{
	if (m_bOnly)
	{
		//첫 미션 
		QUEST_INFO tQuest;
		tQuest.wstrQuest = L"검투대회 참가하기\n(포탈로 이동)";
		tQuest.bClear = false;
		CQuestMgr::GetInstance()->AddQuest(tQuest);

		//////////////test//////////////////////////////////////
		//tQuest.wstrQuest = L"테스트미션";
		//tQuest.bClear = false;

		//CQuestMgr::GetInstance()->AddQuest(tQuest);
		////////////////////////////////////////////////////////

		m_bOnly = false;
	}
}

void CQuestMgr::Set_MissionPossible(NPCTYPE eNPCType, _bool bBool)
{
	if (RED == eNPCType)
		m_bNPCRed_MissionPossible = bBool;
	else if(YELLOW == eNPCType)
		m_bNPCYellow_MissionPossible = bBool;

}

void CQuestMgr::Set_MissionCompleted(NPCTYPE eNPCType, _bool bBool)
{
	if (RED == eNPCType)
		m_bNPCRed_MissionCompleted = bBool;
	else if (YELLOW == eNPCType)
		m_bNPCYellow_MissionCompleted = bBool;

}

void CQuestMgr::Set_SecondMissionCompleted(NPCTYPE eNPCType, _bool bBool)
{
	if (RED == eNPCType)
		m_bNPCRed_SecondMissionCompleted = bBool;
}

wstring CQuestMgr::Get_QuestNarration(wstring wstrKey)
{
	return m_mapQuestNarration[wstrKey];
}

_bool CQuestMgr::Get_MissionPossible(NPCTYPE eNPCType)
{
	if (RED == eNPCType)
		return m_bNPCRed_MissionPossible;
	else if (YELLOW == eNPCType)
		return m_bNPCYellow_MissionPossible;
}

_bool CQuestMgr::Get_MissionCompleted(NPCTYPE eNPCType)
{
	if (RED == eNPCType)
		return m_bNPCRed_MissionCompleted;
	else if (YELLOW == eNPCType)
		return m_bNPCYellow_MissionCompleted;

}

_bool CQuestMgr::Get_SecondMissionCompleted(NPCTYPE eNPCType)
{
	if (RED == eNPCType)
		return m_bNPCRed_SecondMissionCompleted;
}
