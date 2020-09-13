#pragma once
#ifndef __CPuzzleMgr_h__
#define __CPuzzleMgr_h__

#include "PuzzleObj.h"
#include "PuzzlePreview.h"

class CPuzzleMgr
{
	DECLARE_SINGLETON(CPuzzleMgr)
public:
	enum PUZZLETYPE { PUZZLE_APOSTLE, PUZZLE_BELATOS, PUZZLE_END };
private:
	CPuzzleMgr();
	~CPuzzleMgr();

public:
	//Getter
	PUZZLETYPE	Get_PuzzleType() { return m_ePuzzleType; }
	_bool		Get_ChangeScene() { return m_bChangeScene; }

	//Set
	void		Set_SceneChange(_bool bSceneChange) { m_bChangeScene = bSceneChange; }
public:
	HRESULT		Set_Device(LPDIRECT3DDEVICE9 pGraphicDev);
	void		Update_PuzzleMgr(const _double& dTimeDelta);
	void		Create_Puzzle_Apostle();
	void		Create_Puzzle_Belatos();
	void		Reset_Puzzle_Apostle();
	void		Reset_Puzzle_Belatos();
	void		Delete_Puzzle();
	void		Clicked_Puzzle(CPuzzleObj* pPuzzleObj);

private:
	void		LateInit();
	void		Change_Puzzle(_uint iIndex);
	_bool		Success_Check();
	void		Success_Update_Apostle(const _double& dTimeDelta);
	void		Success_Update_Belatos(const _double& dTimeDelta);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	vector<CPuzzleObj*>			m_vecPuzzleObj;
	_uint						m_iNull_Index;	// ∫§≈Õ¿« ∫ÒæÓ¿÷¥¬ ¿Œµ¶Ω∫,
	_bool						m_bInit;
	_bool						m_bStart;
	CPuzzlePreview*				m_pPreview;
	CPuzzlePreview*				m_pSuccessView;
	_bool						m_bSuccess;
	_double						m_dSuccessDelay;
	PUZZLETYPE					m_ePuzzleType;

	//¿Œ±’
	_bool						m_bChangeScene;

};

#endif // !__CPuzzleMgr_h__
