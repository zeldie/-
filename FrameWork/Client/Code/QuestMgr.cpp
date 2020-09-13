#include "stdafx.h"
#include "QuestMgr.h"

IMPLEMENT_SINGLETON(CQuestMgr)

CQuestMgr::CQuestMgr()
	:	//m_eNPCType(NPCTYPE_END),
		m_bNPCRed_MissionPossible(false),
		m_bNPCYellow_MissionPossible(false), //����:true
		m_bNPCRed_MissionCompleted(false),
		m_bNPCYellow_MissionCompleted(false),
		m_bNPCYellow_PotalQCompleted(false),
		m_bNPCRed_SecondMissionCompleted(false),
		m_iRedQuestCnt(6), //����:6
		m_iYellowQuestCnt(4), //����:3
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
	m_mapQuestNarration.emplace(L"RedCanGive", L"�����ִ� ��Ҹ� �˷��޶��?\n���� �׷��� ���� �˷��� �� ����?\n���� �̱�� �˷�����!");
	m_mapQuestNarration.emplace(L"RedCantGive",L"�̷��� ���� ������ ��ȸ �غ���..\n������......");
	m_mapQuestNarration.emplace(L"RedCompleted", L"��~ �� �ϴµ�?\n��ó�� �ִ� �������� �� ���Ⱑ �־�\n��Ż�� ���ؼ� ���Ͱ� �ִ� ������ �̵��� �� �־�!");
	m_mapQuestNarration.emplace(L"RedSecondCompleted", L"��¥ ���ؿԳ�?\n���� ��ȸ ������ �����ϰڴµ�!");
	m_mapQuestNarration.emplace(L"RedAllClear", L"�ʴ� �� �� �־�!!");

	m_mapQuestNarration.emplace(L"YellowAfterPotal", L"������ȸ �غ񶧹��� �ʹ� �ٻڳ׿�..");
	m_mapQuestNarration.emplace(L"YellowBeforePortal", L"������ȸ �غ񶧹��� �ʹ� �ٻڳ׿�..");
	m_mapQuestNarration.emplace(L"YellowCompleted", L"���ϱ� ��������ٵ�.. ����ؿ�!\n���Ⱑ �Ѱ� �� �ʿ��Ѱ���?\n�׷� �����Ӹ� ģ���� ã�ư�������~");
	m_mapQuestNarration.emplace(L"YellowPortalCompleted", L"���Ⱑ �ʿ��ϴٱ���?\n����� ���� �� ȭ�꼶�� �ִµ�..�ű� ����� ������!\n��ó�� ���Ŵɷ��� �ִ� ���Ͱ� �ִٰ��ϴ���\n�� ���͸� ������ �����ؼ� �� �� �����ſ���");

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

	//�׳�..������ �տ��ִ°ɷ� �ϴ�..
	m_vecQuest.front().bClear = true;
}

void CQuestMgr::SetQuestAllClear()
{
	//��� ����Ʈ Ŭ����
	for (auto& pVec : m_vecQuest)
	{
		pVec.bClear = true;
	}
}

void CQuestMgr::MakeFirstQuest()
{
	if (m_bOnly)
	{
		//ù �̼� 
		QUEST_INFO tQuest;
		tQuest.wstrQuest = L"������ȸ �����ϱ�\n(��Ż�� �̵�)";
		tQuest.bClear = false;
		CQuestMgr::GetInstance()->AddQuest(tQuest);

		//////////////test//////////////////////////////////////
		//tQuest.wstrQuest = L"�׽�Ʈ�̼�";
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
