#include "stdafx.h"
#include "ResultBackBar.h"
#include "ScreenTex.h"

CResultBackBar::CResultBackBar(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUIObject(pGraphicDev),
	m_eBackBarType(BACKBARTYPE_END),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_bOne(true),
	m_bBlueWIN(false),
	m_dbTotalTime(0.f),
	m_wstrTotalTime(L""),
	m_iTotalDamage(0),
	m_szTotalDamage(L""),
	//m_wstrTotalDamage(L""),
	m_iTotalBreak(0),
	m_szTotalBreak(L""),
	//m_wstrTotalBreak(L""),
	m_iTotalDeath(0),
	m_wstrTotalDeath(L"")
{
	//fx = 0.f;
	//fy= 0.f;
}


CResultBackBar::~CResultBackBar()
{
}

HRESULT CResultBackBar::Ready_GameObject(BACKBARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eBackBarType = eType;

	for (_uint i = 0; i < FLAGRESULTINFO_END; ++i)
	{
		ZeroMemory(&m_szFlagResult[i], sizeof(TCHAR_FLAGRESULT));
	}

	return S_OK;
}

_int CResultBackBar::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;

	CUIObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CResultBackBar::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (RESULTBGTYPE == m_eBackBarType)
	{
		if (m_bOne)
		{
			if (CUIMgr::UITYPE_RESULT_Boss == m_pUIMgr->Get_UIType()
				|| CUIMgr::UITYPE_RESULT_Shooting == m_pUIMgr->Get_UIType())
				//보스결과창 & 슈팅게임 결과창 ->클리어타임체크
				GetTotalTime();
			else if (CUIMgr::UITYPE_RESULT_Run == m_pUIMgr->Get_UIType())
				//런게임 결과창 -> 클리어타임체크
				GetTotalTimeInRunGame();

			GetResult(); //보스결과창 & 깃발전결과창 &슈팅게임
			m_bOne = false;
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CResultBackBar::Render_Geometry(const _double & dTimeDelta)
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

	pEffect->BeginPass(1);

	m_pScreenTexBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

	//if (Engine::KeyDown(DIK_RIGHT))		
	//	fx += 1.f;
	//if (Engine::KeyDown(DIK_LEFT))
	//	fx -= 1.f;
	//if (Engine::KeyDown(DIK_UP))		
	//	fy += 1.f;
	//if (Engine::KeyDown(DIK_DOWN))
	//	fy -= 1.f;

	if (RESULTBGTYPE == m_eBackBarType)
	{
		if (CUIMgr::UITYPE_RESULT_Boss == m_pUIMgr->Get_UIType())
		{
			Engine::Render_Font(L"Font_GODICHEAVY", L"임무완료", &_vec2(126.f, 270.f), D3DXCOLOR(0.08f, 0.71f, 0.95f, 1.f));

			Engine::Render_Font(L"Font_GODIC", L"카르마", &_vec2(156.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"이름", &_vec2(265.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"방해량", &_vec2(381.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"피해량", &_vec2(501.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"데스", &_vec2(631.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"클리어 타임", &_vec2(745.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));

			Engine::Render_Font(L"Font_GODIC", L"JUSIN103", &_vec2(251.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szTotalBreak/*m_wstrTotalBreak*/, &_vec2(384.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szTotalDamage/* m_wstrTotalDamage*/, &_vec2(504.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_wstrTotalDeath, &_vec2(634.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_wstrTotalTime, &_vec2(750.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
		else if (CUIMgr::UITYPE_RESULT_Cartel == m_pUIMgr->Get_UIType())
		{
			//메뉴이름
			Engine::Render_Font(L"Font_GODIC", L"카르마", &_vec2(156.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"이름", &_vec2(265.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"획득 점수", &_vec2(381.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"킬", &_vec2(501.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"피해량", &_vec2(631.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"방해량", &_vec2(745.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));

			// 점수
			Engine::Render_Font(L"Font_GODIC3", m_szBlueTotalScore, &_vec2(128.f, 238.4f), D3DXCOLOR(0.28f, 0.96f, 0.96f, 1.f));
			Engine::Render_Font(L"Font_GODIC3", m_szRedTotalScore, &_vec2(311.2f, 238.4f), D3DXCOLOR(0.99f, 0.35f, 0.34f, 1.f));

			//승리 유무 text
			if (m_bBlueWIN) //파란팀승
				Engine::Render_Font(L"Font_GODICHEAVY", L"아군 승리", &_vec2(203.f, 238.4f), D3DXCOLOR(0.28f, 0.96f, 0.96f, 1.f));
			else //레드팀승
				Engine::Render_Font(L"Font_GODICHEAVY", L"적군 승리", &_vec2(203.f, 238.4f), D3DXCOLOR(0.99f, 0.35f, 0.34f, 1.f));


			//Player
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[PLAYER].szName, &_vec2(251.f, 340.1f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[PLAYER].szEarnedPoints, &_vec2(384.f, 340.1f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[PLAYER].szKill, &_vec2(504.f, 340.1f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[PLAYER].szDamage, &_vec2(632.f, 340.1f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[PLAYER].szBreak, &_vec2(748.f, 340.1f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			//Alliance
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ALLIANCE].szName, &_vec2(251.f, 372.7f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ALLIANCE].szEarnedPoints, &_vec2(384.f, 372.7f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ALLIANCE].szKill, &_vec2(504.f, 372.7f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ALLIANCE].szDamage, &_vec2(632.f, 372.7f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ALLIANCE].szBreak, &_vec2(748.f, 372.7f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			//Enemy_1
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ENEMY_1].szName, &_vec2(251.f, 407.8f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ENEMY_1].szEarnedPoints, &_vec2(384.f, 407.8f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ENEMY_1].szKill, &_vec2(504.f, 407.8f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ENEMY_1].szDamage, &_vec2(632.f, 407.8f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ENEMY_1].szBreak, &_vec2(748.f, 407.8f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			//Enemy_2
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ENEMY_2].szName, &_vec2(251.f, 439.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ENEMY_2].szEarnedPoints, &_vec2(384.f, 439.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ENEMY_2].szKill, &_vec2(504.f, 439.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ENEMY_2].szDamage, &_vec2(632.f, 439.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szFlagResult[ENEMY_2].szBreak, &_vec2(748.f, 439.9f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		}
		else if (CUIMgr::UITYPE_RESULT_Shooting == m_pUIMgr->Get_UIType())
		{
			Engine::Render_Font(L"Font_GODICHEAVY", L"임무완료", &_vec2(126.f, 270.f), D3DXCOLOR(0.08f, 0.71f, 0.95f, 1.f));

			Engine::Render_Font(L"Font_GODIC", L"카르마", &_vec2(156.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"이름", &_vec2(265.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"방해량", &_vec2(381.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"피해량", &_vec2(501.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"데스", &_vec2(631.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"클리어 타임", &_vec2(745.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));

			Engine::Render_Font(L"Font_GODIC", L"JUSIN103", &_vec2(251.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szTotalBreak/*m_wstrTotalBreak*/, &_vec2(384.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_szTotalDamage/* m_wstrTotalDamage*/, &_vec2(504.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_wstrTotalDeath, &_vec2(634.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC", m_wstrTotalTime, &_vec2(750.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		}
		else if (CUIMgr::UITYPE_RESULT_Run == m_pUIMgr->Get_UIType())
		{
			//Engine::Render_Font(L"Font_GODICHEAVY", L"임무완료", &_vec2(126.f, 270.f), D3DXCOLOR(0.08f, 0.71f, 0.95f, 1.f));

			//Engine::Render_Font(L"Font_GODIC", L"카르마", &_vec2(156.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"이름", &_vec2(915.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			//Engine::Render_Font(L"Font_GODIC", L"방해량", &_vec2(381.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			//Engine::Render_Font(L"Font_GODIC", L"피해량", &_vec2(501.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"데스", &_vec2(1031.f, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));
			Engine::Render_Font(L"Font_GODIC", L"클리어 타임", &_vec2(1151, 310.f), D3DXCOLOR(0.49f, 0.48f, 0.52f, 1.f));

			Engine::Render_Font(L"Font_GODIC2", L"탐욕의 사도", &_vec2(901.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			//Engine::Render_Font(L"Font_GODIC", m_szTotalBreak/*m_wstrTotalBreak*/, &_vec2(384.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			//Engine::Render_Font(L"Font_GODIC", m_szTotalDamage/* m_wstrTotalDamage*/, &_vec2(504.f, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC2", m_wstrTotalDeath, &_vec2(1034, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Font_GODIC2", m_wstrTotalTime, &_vec2(1154, 350.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		}
	}
}

void CResultBackBar::GetTotalTime()
{
	m_dbTotalTime = m_pUIMgr->Get_AccumulatedTime();

	_uint iQuotient = (_ulong)m_dbTotalTime / 60;
	_uint iRemainder = (_ulong)m_dbTotalTime % 60;

	int iMiniuteTens, iMinuteUnits, iSecondTens, iSecondUnits = 0;
	iMiniuteTens = iQuotient / 10;
	iMinuteUnits = iQuotient % 10;

	iSecondTens = iRemainder / 10;
	iSecondUnits = iRemainder % 10;

	m_wstrTotalTime = to_wstring(iMiniuteTens) + to_wstring(iMinuteUnits) + L":" + to_wstring(iSecondTens) + to_wstring(iSecondUnits);
}

void CResultBackBar::GetTotalTimeInRunGame()
{
	m_dbTotalTime = m_pUIMgr->Get_AccumulatedTime();
	//앞에 초
	wstring wstrSecond=L"";
	wstrSecond = to_wstring((_uint)m_dbTotalTime);

	//소수점자리 초
	_double dSemiSecondTime = (m_dbTotalTime - _uint(m_dbTotalTime)) * 100.f;
	wstring wstrSs = L"." + to_wstring(_uint(dSemiSecondTime)) + L"s";

	m_wstrTotalTime = wstrSecond + wstrSs;
}

void CResultBackBar::GetResult()
{
	if (CUIMgr::UITYPE_RESULT_Boss == m_pUIMgr->Get_UIType())
	{
		m_iTotalDamage = m_pUIMgr->Get_AccumulatedDamage();
		MakeComma(m_iTotalDamage, m_szTotalDamage);
		//m_wstrTotalDamage = to_wstring(m_iTotalDamage);

		m_iTotalBreak = m_pUIMgr->Get_AccumulatedBreak();
		MakeComma(m_iTotalBreak, m_szTotalBreak);
		//m_wstrTotalBreak = to_wstring(m_iTotalBreak);

		m_iTotalDeath = m_pUIMgr->Get_AccumulatedDeath();
		m_wstrTotalDeath = to_wstring(m_iTotalDeath);
	}
	else if (CUIMgr::UITYPE_RESULT_Cartel == m_pUIMgr->Get_UIType())
	{
		//////점수
		_uint iBlueTeam = m_pUIMgr->Get_BlueScore();
		_uint iRedTeam = m_pUIMgr->Get_RedScore();

		////test/////////////////
		//iBlueTeam = 150;
		//iRedTeam = 100;
		///////////////////////
		wsprintf(m_szBlueTotalScore, L"%d", iBlueTeam);
		wsprintf(m_szRedTotalScore, L"%d", iRedTeam);


		if (iBlueTeam > iRedTeam)
		{
			m_bBlueWIN = true;
			CheckWinner(CFlagTotalScore::BLUE);
		}
		else
		{
			m_bBlueWIN = false;
			CheckWinner(CFlagTotalScore::RED);
		}

		//////정보
		//PLAYER
		FLAG_RESULT tPlayer = m_pUIMgr->Get_FlagResultInfo(OBJECT_PLAYER);

		////test//////////////////////////////
		//tPlayer.iEarnedPoints = 100;
		//tPlayer.iKill = 5;
		//tPlayer.iDamage = 25683;
		//tPlayer.iBreak = 456821542;
		/////////////////////////////////////////


		lstrcpy(m_szFlagResult[PLAYER].szName, tPlayer.szName);
		wsprintf(m_szFlagResult[PLAYER].szEarnedPoints, L"%d", tPlayer.iEarnedPoints);
		wsprintf(m_szFlagResult[PLAYER].szKill, L"%d", tPlayer.iKill);
		MakeComma(tPlayer.iDamage, m_szFlagResult[PLAYER].szDamage);
		MakeComma(tPlayer.iBreak, m_szFlagResult[PLAYER].szBreak);



		//Alliance
		FLAG_RESULT tAlliance = m_pUIMgr->Get_FlagResultInfo(OBJECT_ALLIANCE);

		lstrcpy(m_szFlagResult[ALLIANCE].szName, tAlliance.szName);
		wsprintf(m_szFlagResult[ALLIANCE].szEarnedPoints, L"%d", tAlliance.iEarnedPoints);
		wsprintf(m_szFlagResult[ALLIANCE].szKill, L"%d", tAlliance.iKill);
		MakeComma(tAlliance.iDamage, m_szFlagResult[ALLIANCE].szDamage);
		MakeComma(tAlliance.iBreak, m_szFlagResult[ALLIANCE].szBreak);


		//Enemy_1
		FLAG_RESULT tEnemy1 = m_pUIMgr->Get_FlagResultInfo(OBJECT_ENEMY_1);

		lstrcpy(m_szFlagResult[ENEMY_1].szName, tEnemy1.szName);
		wsprintf(m_szFlagResult[ENEMY_1].szEarnedPoints, L"%d", tEnemy1.iEarnedPoints);
		wsprintf(m_szFlagResult[ENEMY_1].szKill, L"%d", tEnemy1.iKill);
		MakeComma(tEnemy1.iDamage, m_szFlagResult[ENEMY_1].szDamage);
		MakeComma(tEnemy1.iBreak, m_szFlagResult[ENEMY_1].szBreak);

		//Enemy_2
		FLAG_RESULT tEnemy2 = m_pUIMgr->Get_FlagResultInfo(OBJECT_ENEMY_2);

		lstrcpy(m_szFlagResult[ENEMY_2].szName, tEnemy2.szName);
		wsprintf(m_szFlagResult[ENEMY_2].szEarnedPoints, L"%d", tEnemy2.iEarnedPoints);
		wsprintf(m_szFlagResult[ENEMY_2].szKill, L"%d", tEnemy2.iKill);
		MakeComma(tEnemy2.iDamage, m_szFlagResult[ENEMY_2].szDamage);
		MakeComma(tEnemy2.iBreak, m_szFlagResult[ENEMY_2].szBreak);

	}
	else if (CUIMgr::UITYPE_RESULT_Shooting == m_pUIMgr->Get_UIType())
	{
		m_iTotalDamage = m_pUIMgr->Get_AccumulatedDamage();
		MakeComma(m_iTotalDamage, m_szTotalDamage);
		//m_wstrTotalDamage = to_wstring(m_iTotalDamage);

		m_iTotalBreak = m_pUIMgr->Get_AccumulatedBreak();
		MakeComma(m_iTotalBreak, m_szTotalBreak);
		//m_wstrTotalBreak = to_wstring(m_iTotalBreak);

		m_iTotalDeath = m_pUIMgr->Get_AccumulatedDeath();
		m_wstrTotalDeath = to_wstring(m_iTotalDeath);
	}
	else if (CUIMgr::UITYPE_RESULT_Run == m_pUIMgr->Get_UIType())
	{
		m_iTotalDeath = m_pUIMgr->Get_AccumulatedDeath();
		m_wstrTotalDeath = to_wstring(m_iTotalDeath);
	}
}

void CResultBackBar::MakeComma(_uint iData, _tchar * szResult)
{
	_tchar szBuff[MAX_STR], *szCopy;

	_uint iLen = wsprintf(szBuff, L"%d", iData);
	//음수 조건 제외

	szCopy = szBuff;
	//자릿수가 3자리 이상이어야 ',' 사용 -> 자릿수 체크
	_uint AddCnt = iLen / 3;
	_uint SkipCnt = iLen % 3;

	if (AddCnt > 0 && !SkipCnt)
		AddCnt--;

	//자릿수가 3자리 이상이면 ',' 찍는다
	if (AddCnt > 0)
	{
		//선두에 있는 ','앞에 추가될 숫자복사
		if (SkipCnt > 0)
		{
			//memcpy(szResult, szCopy, SkipCnt);
			lstrcpy(&szResult[0], &szBuff[0]);

			szResult += SkipCnt;
			*szResult++ = ',';

			szCopy += SkipCnt;
			iLen -= SkipCnt;
		}

		for (_uint i = 0; i < iLen; i++)
		{
			//3의 배수마다 ',' 추가
			if (i && !(i % 3))
				*szResult++ = ',';

			*szResult++ = *szCopy++;
		}

		*szResult = 0;
	}
	else
	{
		//자릿수가 3자리가 안되면 원본복사
		memcpy(szResult, szCopy, iLen + 1);
	}

}

void CResultBackBar::CheckWinner(CFlagTotalScore::TEAMCOLOR eTeam)
{
	Engine::CGameObject* pGameObject = nullptr;

	if(CFlagTotalScore::BLUE == eTeam)
		pGameObject = CFlagTotalScore::Create(m_pGraphicDev, CFlagTotalScore::OUTLINE, CFlagTotalScore::BLUE, 117.5f, 216.4f, 71.f, 71.f, 0.15f);
	else if(CFlagTotalScore::RED == eTeam)
		pGameObject = CFlagTotalScore::Create(m_pGraphicDev, CFlagTotalScore::OUTLINE, CFlagTotalScore::RED, 301.7f, 216.4f, 71.f, 71.f, 0.15f);


	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"FlagTotalScore", pGameObject);
	m_pUIMgr->AddCurUI(pGameObject);
}

HRESULT CResultBackBar::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_BackBar"));
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

HRESULT CResultBackBar::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eBackBarType);

	return S_OK;
}

CResultBackBar * CResultBackBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, BACKBARTYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CResultBackBar* pInstance = new CResultBackBar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CResultBackBar::Free()
{
	CUIObject::Free();
}
