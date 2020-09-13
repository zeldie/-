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
	_bool				Get_YellowPotalQCompleted() { return m_bNPCYellow_PotalQCompleted; }//YELLOW�� �ش�
	_bool				Get_SecondMissionCompleted(NPCTYPE eNPCType); //RED�� �ش�
	_uint				Get_RedQuestCnt() { return m_iRedQuestCnt; }
	_uint				Get_YellowQuestCnt() { return m_iYellowQuestCnt; }
	_bool				Get_RedAllClear() { return m_bNPCRedAllClear; }

	//Setter
	void				Set_MissionPossible(NPCTYPE eNPCType, _bool bBool);
	void				Set_MissionCompleted(NPCTYPE eNPCType, _bool bBool);
	void				Set_YellowPotalQCompleted(_bool bBool) { m_bNPCYellow_PotalQCompleted = bBool; }; //YELLOW�� �ش�
	void				Set_SecondMissionCompleted(NPCTYPE eNPCType, _bool bBool); //RED�� �ش�
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
	//���� ó�������Ҷ� ���ѹ� ������� ����Ʈ
	void				MakeFirstQuest();

private:
	map<wstring, wstring>		m_mapQuestNarration;
	vector<QUEST_INFO>			m_vecQuest;

	//�̼��� �ִ� ���� 
	_bool					m_bNPCRed_MissionPossible;
	_bool					m_bNPCYellow_MissionPossible;

	//�̼��� Ŭ���� �ߴ� ���ߴ�
	_bool					m_bNPCRed_MissionCompleted;
	_bool					m_bNPCYellow_MissionCompleted;

	//YELLOW�� ��ù��°(��Ż)�̼� Ŭ���� �޴پ��޴�
	_bool					m_bNPCYellow_PotalQCompleted;

	//RED�� �ι�° �̼� Ŭ����  �ߴ� ���ߴ�
	_bool					m_bNPCRed_SecondMissionCompleted;

	//RED
	_bool					m_bNPCRedAllClear; //RED�� ��� ����Ʈ Ŭ�����ϸ� true

	//�̼� ī��Ʈ
	_uint					m_iRedQuestCnt;
	_uint					m_iYellowQuestCnt;

	//
	_bool					m_bOnly;


};

#endif
