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
		STATE_REALEND // 있어야하지만 안씀
	};
private:
	CMatchingMgr();
	~CMatchingMgr();

public:
	//Getter
	GAMESTATE	Get_GameState() { return m_eGameState; }

	void		Update_MatchingMgr(const _double& dTimeDelta);
	HRESULT		Set_Device(LPDIRECT3DDEVICE9 pGraphicDev);
	void		Create_Card();	// 카드 생성
	void		Shuffle_Card();	// 카드 섞기
	void		Delete_Card(); // 카드 삭제
	void		Add_SelectCard(CMatchingCard* pCard);

	// 치트
	void		Visible_Card();
	void		InVisible_Card();

public:
	void		RenderTime(); //희정

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

	//희정 추가
	_bool						m_bWaitAnim; //고민하는 애니매이션 한번만 나오게 하고싶어서
	_bool						m_bCreateButton;
};


#endif // !__CMatchingMgr_h__
