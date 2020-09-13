#pragma once
#ifndef __CMatchingMgr_h__
#define __CMatchingMgr_h__

#include "MatchingCard.h"

class CMatchingMgr
{
	DECLARE_SINGLETON(CMatchingMgr)
public:
	enum MATCHINGTURN
	{
		TURN_PLAYER,
		TURN_AI,
		TURN_END
	};
	enum AIDATA
	{
		DATA_OPEN,
		DATA_KNOWN,
		DATA_UNKNOWN,
		DATA_END
	};
	struct tagAidata
	{
		AIDATA						eAIData;
		CMatchingCard::CARDNUMBER	eCardNum;
	};
	enum GAMESTATE
	{
		STATE_START,
		STATE_ING,
		STATE_END,
		STATE_REALEND // �־�������� �Ⱦ�
	};
private:
	CMatchingMgr();
	~CMatchingMgr();

public:
	//Getter
	GAMESTATE	Get_GameState() { return m_eGameState; }

	void		Update_MatchingMgr(const _double& dTimeDelta);
	HRESULT		Set_Device(LPDIRECT3DDEVICE9 pGraphicDev);
	void		Create_Card();	// ī�� ����
	void		Shuffle_Card();	// ī�� ����
	void		Delete_Card(); // ī�� ����
	void		Add_SelectCard(CMatchingCard* pCard);

	// ġƮ
	void		Visible_Card();
	void		InVisible_Card();

public:
	void		RenderTime(); //����

private:
	void		LateInit();
	void		ObserverNotify();
	void		Game_Result();

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	Engine::CManagement*		m_pManagement;
	vector<CMatchingCard*>		m_vecCard;
	vector<CMatchingCard*>		m_vecSelectedCard;
	_double						m_dDelay;
	_bool						m_bFailed;
	MATCHINGTURN				m_eTurn;
	tagAidata					m_tAIData[40];
	_uint						m_iAICard[2];
	GAMESTATE					m_eGameState;
	_uint						m_iPlayerPoint;
	_uint						m_iAIPoint;
	_bool						m_bInit;
	_bool						m_bEndGame;
	_double						m_dTime;
	_uint						m_iCombo;

	//���� �߰�
	_bool						m_bWaitAnim; //����ϴ� �ִϸ��̼� �ѹ��� ������ �ϰ�;
	_bool						m_bCreateButton;
};


#endif // !__CMatchingMgr_h__
