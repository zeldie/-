#pragma once

#ifndef __QuestMgr_h__
#define __QuestMgr_h__

class CQuestMgr
{
	DECLARE_SINGLETON(CQuestMgr)

public:
	enum NPCTYPE { RED, YELLOW, NPCTYPE_END };

private:
	explicit CQuestMgr();
	virtual ~CQuestMgr();

public:
	//Getter
	vector<QUEST_INFO>& Get_vecQuest() { return m_vecQuest; };
	wstring				Get_QuestNarration(wstring wstrKey);
	_bool				Get_MissionPossible(NPCTYPE eNPCType);
	_bool				Get_MissionCompleted(NPCTYPE eNPCType);
	_bool				Get_YellowPotalQCompleted() { return m_bNPCYellow_PotalQCompleted; }//YELLOW만 해당
	_bool				Get_SecondMissionCompleted(NPCTYPE eNPCType); //RED만 해당
	_uint				Get_RedQuestCnt() { return m_iRedQuestCnt; }
	_uint				Get_YellowQuestCnt() { return m_iYellowQuestCnt; }
	_bool				Get_RedAllClear() { return m_bNPCRedAllClear; }

	//Setter
	void				Set_MissionPossible(NPCTYPE eNPCType, _bool bBool);
	void				Set_MissionCompleted(NPCTYPE eNPCType, _bool bBool);
	void				Set_YellowPotalQCompleted(_bool bBool) { m_bNPCYellow_PotalQCompleted = bBool; }; //YELLOW만 해당
	void				Set_SecondMissionCompleted(NPCTYPE eNPCType, _bool bBool); //RED만 해당
	void				Set_RedQuestCnt(_int iCnt) { m_iRedQuestCnt = iCnt; }
	void				Set_YellowQuestCnt(_int iCnt) { m_iYellowQuestCnt = iCnt; }
	void				Set_RedAllClear(_bool bBool) { m_bNPCRedAllClear = bBool; }

public:
	void				AddQuestNarration();
	void				AddQuest(QUEST_INFO QuestInfo);

	void				EraseQuest();
	void				EraseAllQuest();
	void				SetQuestClear();
	void				SetQuestAllClear();

public:
	//정말 처음시작할때 딱한번 만들어질 퀘스트
	void				MakeFirstQuest();

private:
	map<wstring, wstring>		m_mapQuestNarration;
	vector<QUEST_INFO>			m_vecQuest;

	//미션이 있다 없다 
	_bool					m_bNPCRed_MissionPossible;
	_bool					m_bNPCYellow_MissionPossible;

	//미션을 클리어 했다 안했다
	_bool					m_bNPCRed_MissionCompleted;
	_bool					m_bNPCYellow_MissionCompleted;

	//YELLOW의 왕첫번째(포탈)미션 클리어 햇다안햇다
	_bool					m_bNPCYellow_PotalQCompleted;

	//RED의 두번째 미션 클리어  했다 안했다
	_bool					m_bNPCRed_SecondMissionCompleted;

	//RED
	_bool					m_bNPCRedAllClear; //RED의 모든 퀘스트 클리어하면 true

	//미션 카운트
	_uint					m_iRedQuestCnt;
	_uint					m_iYellowQuestCnt;

	//
	_bool					m_bOnly;


};

#endif
