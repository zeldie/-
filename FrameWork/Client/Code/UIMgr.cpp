#include "stdafx.h"
#include "UIMgr.h"
#include "BackBar.h"
#include "LineBar.h"
#include "SuperArmor.h"
#include "PlayerHp.h"
#include "Stamina.h"
#include "PlayerMp.h"
#include "UltimateSlot.h"
#include "SkillSlot.h"
#include "Skill.h"
#include "KeyGuide.h"
#include "StageTimer.h"
#include "MonsterHpMgr.h"
#include "Groggy.h"
#include "BattleImg.h"
#include "MissionCard.h"
#include "NoFuncObj.h"
#include "SelectMission.h"
#include "KarmaSlot.h"
#include "Button.h"
//#include "Karma.h" ->h 
#include "ReadyBattle.h"
#include "Waiting.h"
#include "StaticCamera.h"
#include "DamageBox.h"
#include "Tired.h"
#include "UltimateRecharge.h"
#include "HpNumber.h"
#include "FlagScore.h"
#include "FlagNoFuncObj.h"
#include "Yellow.h"
#include "FlagScoreFlash.h"
#include "RevivalPopUp.h"
#include "FlagCountDown.h"
#include "AI_Karma.h"
#include "ResultBackBar.h"
#include "FlagTotalScore.h"
#include "MatchingScore.h"
#include "MatchingTurn.h"
#include "MatchingResult.h"
#include "MissionLock.h"
#include "QuestBackBar.h"
#include "NPCButton.h"
#include "QuestCheckBox.h"
#include "FlagDeathBar.h"
#include "FDBCreator.h"
#include "RunCountDown.h"
#include "RunButton.h"
//#include "MatchingButton.h" ->h
#include "Body.h"
#include "CustomizingMenu.h"
#include "CustomizingSelectBox.h"
#include "CustomizingEyeUI.h"
#include "CustomizingBrowUI.h"
#include "CustomizingColorBG.h"
#include "CustomizingFrontHairUI.h"
#include "CustomizingBackHairUI.h"
#include "CustomizingSideHairUI.h"
#include "CustomizingTailHairUI.h"
#include "CustomizingTopUI.h"
#include "CustomizingPantsUI.h"
#include "CustomizingFootUI.h"
#include "CustomizingHandsUI.h"
#include "CustomizingButton.h"
#include "RunPlayerHP.h"
#include "RunPlayerItem.h"
#include "RunPlayerItemSlot.h"

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr()
	: m_eUIType(UITYPE_END),
	m_iCurIndex(0),
	m_iNextIndex(0),
	m_dAccumulatedTime(0.f),
	m_dStageTime(0.f),
	m_bStageTimeStart(false),
	m_bStageTimeStop(false),
	m_bTimeCheck(false),
	m_bUI_Working(false),
	m_bOne(true),
	m_iAccumulatedBreak(0),
	m_iAccumulatedDamage(0),
	m_iAccumulatedDeath(0),
	m_dbAccumulatedTime(0.f),
	m_bCreateBossResultUI(false),
	m_bCreateShootingResultUI(false),
	m_iBlueTotalScore(0),
	m_iRedTotalScore(0),
	m_iScore(0),
	m_bMissionFlagLock(true),
	m_bMissionApostleLock(true),
	m_bMissionBossLock(true),
	m_ilistCurCnt(0),
	m_ilistPrevCnt(0),
	m_bCartelFinish(false),
	m_bChangeScene(false),
	m_bRunGameStart(false),
	m_bRunGameTimeStop(true), //처음엔멈춰있어야해서 true
	m_bEraseSwapKarmaUIPossible(false),
	m_pBelatosTransformCom(nullptr),
	m_pAITransfomCom1(nullptr),
	m_pAITransfomCom2(nullptr),
	m_bGetFlag(false),
	m_bReCreateFlag(false),
	m_iAccumulatedRunGamePoints(0)
{
	////플레이어 무기////////
	m_ePlayerMainWeapon = TWOHANDSWORD;
	m_ePlayerSubWeapon = DUALSWORD;
	/////////////////////////

	m_vecSkillSlot.reserve(5);
	m_vecNoFuncObj.reserve(2);
	m_vecFlagNoFuncObj.reserve(4);
	m_vecFlagBackBar.reserve(4);
	m_vecFlagPos.reserve(2);
	m_pMouse = nullptr;
	m_pUltimateRecharge = nullptr;
	m_pFlagSpaceTXT = nullptr;

	ZeroMemory(&m_tPlayer, sizeof(FLAG_RESULT));
	ZeroMemory(&m_tAlliance, sizeof(FLAG_RESULT));
	ZeroMemory(&m_tEnemy_1, sizeof(FLAG_RESULT));
	ZeroMemory(&m_tEnemy_2, sizeof(FLAG_RESULT));

	////////////커스터마이징///////////////////////
	m_vecCustomEyes.reserve(4);
	m_vecCustomEyesBack.reserve(4);
	m_vecCustomBrow.reserve(4);
	m_vecCustomBrowBack.reserve(4);
	m_pCustomizingColorBG = nullptr;
	m_vecCustomFrontHair.reserve(4);
	m_vecCustomFrontHairBack.reserve(4);
	m_vecCustomBackHair.reserve(4);
	m_vecCustomBackHairBack.reserve(4);
	m_vecCustomSideHair.reserve(4);
	m_vecCustomSideHairBack.reserve(4);
	m_vecCustomTailHair.reserve(4);
	m_vecCustomTailHairBack.reserve(4);
	m_vecCustomTop.reserve(4);
	m_vecCustomTopBack.reserve(4);
	m_vecCustomPants.reserve(4);
	m_vecCustomPantsBack.reserve(4);
	m_vecCustomFoot.reserve(4);
	m_vecCustomFootBack.reserve(4);
	m_pCustomHands = nullptr;
	m_pCustomHandsBack = nullptr;
	m_pCustomButton = nullptr;
	m_bFinishCustomize = false;
	//////////////////////////////////////////////
}


CUIMgr::~CUIMgr()
{
	//m_listFlagDeathInfo.clear();	
}

_bool CUIMgr::Get_SceneChange()
{
	if (0 == m_vecCurUI_Rc.size())
		return false;

	for (auto& pVec : m_vecCurUI_Rc)
	{
		if (dynamic_cast<CWaiting*>(pVec))
		{
			_bool bBool = dynamic_cast<CWaiting*>(pVec)->Get_SceneChange();
			if (bBool)
				m_bUI_Working = false; //씬이바뀔때 기본상태로 돌려놓는다

			return bBool;
		}
	}

	return false;
}

FLAG_RESULT & CUIMgr::Get_FlagResultInfo(OBJID eOBJ)
{
	switch (eOBJ)
	{
	case OBJECT_PLAYER:
		return m_tPlayer;
	case OBJECT_ALLIANCE:
		return m_tAlliance;
	case OBJECT_ENEMY_1:
		return m_tEnemy_1;
	case OBJECT_ENEMY_2:
		return m_tEnemy_2;
	}

}
void CUIMgr::Set_ZeroStageTime()
{
	m_dAccumulatedTime = 0.f;
	m_dStageTime = 0.f;
	m_bStageTimeStart = false;
	m_bTimeCheck = false;
	m_bStageTimeStop = false;
}
void CUIMgr::AddCurUI(Engine::CGameObject * pGameObject)
{
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
}

void CUIMgr::AddvecNPCNoFuncObj(Engine::CGameObject * pGameObject)
{
	m_listNPCNoFuncObj.emplace_back(dynamic_cast<CNPCNoFuncObj*>(pGameObject));
}

HRESULT CUIMgr::CreatePlayerInfoUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer * pLayer)
{
	////////////////////////////////////////////////////////
	//플레이어 무기정보..
	_int iMainWeapon = dynamic_cast<CPlayer*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"RealPlayer"))->Get_MainWeapon(); //0==common
	wstring wstrTexture = L"";
	if (1 == iMainWeapon)
		wstrTexture = L"Skill_DS";
	else if (2 == iMainWeapon)
		wstrTexture = L"Skill_TS";
	else if (3 == iMainWeapon)
		wstrTexture = L"Skill_ORB";
	else if (4 == iMainWeapon)
		wstrTexture = L"Skill_LB";

	////////////////////////////////////////////////////////


	Engine::CGameObject*		pGameObject = nullptr;

	//SuperArmor
	pGameObject = CBackBar::Create(pGraphicDev, CBackBar::ROUNDTYPE, 1024.6f, 72.2f, 203.f, 11.f, 0.3f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	//pLayer->Add_GameObject(L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CSuperArmor::Create(pGraphicDev, CSuperArmor::FULL, 1028.2f, 72.2f, 198.6f, 10.f, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"SuperArmor", pGameObject);
	//pLayer->Add_GameObject(L"SuperArmor", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CSuperArmor::Create(pGraphicDev, CSuperArmor::EMPTY, 1120.6f, 72.2f, 9.8f, 16.4f, 0.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"SuperArmor", pGameObject);
	//pLayer->Add_GameObject(L"SuperArmor", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CLineBar::Create(pGraphicDev, CLineBar::TENLINES, 1022.6f, 72.2f, 205.7f, 11.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"LineBar", pGameObject);
	//pLayer->Add_GameObject(L"LineBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));


	//Hp
	pGameObject = CBackBar::Create(pGraphicDev, CBackBar::RECTTYPE, 1024.6f, 83.f, 207.8f, 21.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	//pLayer->Add_GameObject(L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CPlayerHp::Create(pGraphicDev, CPlayerHp::PREVHP, 1029.5f, 89.2f, 199.f, 10.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"PlayerHp", pGameObject);
	//pLayer->Add_GameObject(L"PlayerHp", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CPlayerHp::Create(pGraphicDev, CPlayerHp::HP, 1029.5f, 89.2f, 198.f, 10.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"PlayerHp", pGameObject);
	//pLayer->Add_GameObject(L"PlayerHp", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));


	//Stamina & Mp
	pGameObject = CBackBar::Create(pGraphicDev, CBackBar::RECTTYPE, 1025.4f, 99.7f, 208.f, 25.8f, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	//pLayer->Add_GameObject(L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CStamina::Create(pGraphicDev, 1029.6f, 106.9f, 198.4f, 5.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Stamina", pGameObject);
	//pLayer->Add_GameObject(L"Stamina", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CPlayerMp::Create(pGraphicDev, 1030.f, 114.f, 198.f, 5.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"PlayerMp", pGameObject);
	//pLayer->Add_GameObject(L"PlayerMp", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CLineBar::Create(pGraphicDev, CLineBar::FIVELINES, 1026.9f, 110.f, 206.2f, 11.8f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"LineBar", pGameObject);
	//pLayer->Add_GameObject(L"LineBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	/////////////////
	//Ultimate Slot    0
	pGameObject = CUltimateSlot::Create(pGraphicDev, CUltimateSlot::BASIC, true, 43.3f, 72.6f, 111.2f, 110.7f, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"UltimateSlot", pGameObject);
	//pLayer->Add_GameObject(L"UltimateSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Ultimate Slot - RechargeInside   1
	pGameObject = CUltimateSlot::Create(pGraphicDev, CUltimateSlot::RECHARGE_INSIDE, false, 43.5f, 73.5f, 110.6f, 106.5f, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"UltimateSlotInside", pGameObject);
	//pLayer->Add_GameObject(L"UltimateSlotInside", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Ultimate Slot - Outline   2
	pGameObject = CUltimateSlot::Create(pGraphicDev, CUltimateSlot::OUTLINE, true, 33.6f, 63.7f, 130.7f, 119.5f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"UltimateSlotOutLine", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//UltimateSkill    3
	pGameObject = CSkill::Create(pGraphicDev, wstrTexture, CSkill::SKILLTYPE(iMainWeapon), Engine::KEYGUIDE_Q, 58.5f, 83.2f, 80.6f, 88.6f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Skill", pGameObject);
	m_vecSkill.emplace_back(dynamic_cast<CSkill*>(pGameObject));
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Ultimate --> 겉에 게이지라인    4
	pGameObject = m_pUltimateRecharge = CUltimateRecharge::Create(pGraphicDev, 42.7f, 71.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"UltimateRecharge", pGameObject);


	//Ultimate Slot - Recharge outline
	pGameObject = CUltimateSlot::Create(pGraphicDev, CUltimateSlot::RECHARGE_OUTLINE, false, 19.7f, 56.5f, 158.f, 143.5f); // 43.3f, 560.3f, 111.2f, 110.7f
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"UltimateSlotRecharge", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//KeyGuide  5
	pGameObject = CKeyGuide::Create(pGraphicDev, Engine::KEYGUIDE_Q, 49.5f, 55.6f, 16.f, 16.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KeyGuide", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));


	//////////////////
	//Skill Slot   678
	for (int i = 0; i < 3; ++i)
	{
		pGameObject = CSkillSlot::Create(pGraphicDev, CSkillSlot::WEAPONTYPE_END, CSkillSlot::BASIC, Engine::KEYGUIDE(i), true, 157.5f + 53.f*i, 62.5f, 57.f, 59.f, 0.2f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"SkillSlot", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	}
	//Skill Slot - Gage 1(MAIN)    9 10 11
	for (int i = 0; i < 3; ++i)
	{
		pGameObject = CSkillSlot::Create(pGraphicDev, CSkillSlot::MAIN, CSkillSlot::RECHARGE, Engine::KEYGUIDE(i), true, 157.5f + 53.f*i, 62.5f, 57.f, 59.f, 0.1f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"SkillSlotCharge", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
		m_vecSkillSlot.emplace_back(dynamic_cast<CSkillSlot*>(pGameObject));
	}

	//Skill Slot - Gage 2(SUB) 12 13 14
	for (int i = 0; i < 3; ++i)
	{
		pGameObject = CSkillSlot::Create(pGraphicDev, CSkillSlot::SUB, CSkillSlot::RECHARGE, Engine::KEYGUIDE(i), true, 157.5f + 53.f*i, 62.5f, 57.f, 59.f, 0.1f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"SkillSlotCharge", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
		m_vecSkillSlot.emplace_back(dynamic_cast<CSkillSlot*>(pGameObject));
	}

	//Skill Slot - Outline 15 16 17
	for (int i = 0; i < 3; ++i)
	{
		pGameObject = CSkillSlot::Create(pGraphicDev, CSkillSlot::WEAPONTYPE_END, CSkillSlot::OUTLINE, Engine::KEYGUIDE(i), true, 157.5f + 53.f*i, 62.5f, 57.f, 59.f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"SkillSlotOutline", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	}
	//Basic Skill Slot 18 19
	for (int i = 0; i < 2; ++i)
	{
		pGameObject = CSkillSlot::Create(pGraphicDev, CSkillSlot::WEAPONTYPE_END, CSkillSlot::BASIC, Engine::KEYGUIDE(i + 3), true, 183.f + 53.f*i, 109.8f, 57.f, 59.f, 0.2f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"SkillSlot", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	}
	//Basic Skill Slot - Gage
	for (int i = 0; i < 2; ++i)
	{
		pGameObject = CSkillSlot::Create(pGraphicDev, CSkillSlot::WEAPONTYPE_END, CSkillSlot::RECHARGE, Engine::KEYGUIDE(i + 3), true, 183.f + 53.f*i, 109.8f, 57.f, 59.f, 0.1f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"SkillSlotCharge", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
		m_vecSkillSlot.emplace_back(dynamic_cast<CSkillSlot*>(pGameObject));
	}
	//Skill Slot - Outline
	for (int i = 0; i < 2; ++i)
	{
		pGameObject = CSkillSlot::Create(pGraphicDev, CSkillSlot::WEAPONTYPE_END, CSkillSlot::OUTLINE, Engine::KEYGUIDE(i + 3), true, 183.f + 53.f*i, 109.8f, 57.f, 59.f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"SkillSlotOutline", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	}

	//KeyGuide 24 25 26
	for (int i = 0; i < 3; ++i)
	{
		pGameObject = CKeyGuide::Create(pGraphicDev, (Engine::KEYGUIDE)(i), 155.8f + 53.f*i, 45.8f, 16.f, 16.f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"KeyGuide", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	}

	pGameObject = CKeyGuide::Create(pGraphicDev, Engine::KEYGUIDE_LSHIFT, 197.6f, 171.7f, 28.f, 16.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KeyGuide", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CKeyGuide::Create(pGraphicDev, Engine::KEYGUIDE_TAB, 255.7f, 171.7f, 17.f, 16.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KeyGuide", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	////////////////////////////////////////////////
	//Skill
	for (int i = 0; i < 3; ++i)
	{
		pGameObject = CSkill::Create(pGraphicDev, wstrTexture, (CSkill::SKILLTYPE)iMainWeapon, Engine::KEYGUIDE(i), 157.5f + 53.f*i, 62.5f, 57.f, 59.f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"Skill", pGameObject);
		m_vecSkill.emplace_back(dynamic_cast<CSkill*>(pGameObject));
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); 
	}
	//Basic Skill
	for (int i = 0; i < 2; ++i)
	{
		pGameObject = CSkill::Create(pGraphicDev, L"Skill_Basic", CSkill::SKILL_BASIC, Engine::KEYGUIDE(i + 3), 183.f + 53.f*i, 109.8f, 57.f, 59.f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"Skill", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	}
	////////////////////////////////////////////////

	//NoFuncObj - On Cooldown
	pGameObject = CNoFuncObj::Create(pGraphicDev, CNoFuncObj::NOFUNCOBJ_COOLDOWNTXT, false, 1114.4f, 237.8f, 119.1f, 27.8f); //187.8f
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"NoFuncObj", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	m_vecNoFuncObj.emplace_back(dynamic_cast<CNoFuncObj*>(pGameObject));

	//NoFuncObj - 기력 부족
	pGameObject = CNoFuncObj::Create(pGraphicDev, CNoFuncObj::NOFUNCOBJ_STAMINATXT, false, 1114.4f, 187.8f, 119.1f, 27.8f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"NoFuncObj", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	m_vecNoFuncObj.emplace_back(dynamic_cast<CNoFuncObj*>(pGameObject));

	//NoFuncObj - 마나 부족
	pGameObject = CNoFuncObj::Create(pGraphicDev, CNoFuncObj::NOFUNCOBJ_DEFICIENTMPTXT, false, 1114.4f, 187.8f, 119.1f, 27.8f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"NoFuncObj", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	m_vecNoFuncObj.emplace_back(dynamic_cast<CNoFuncObj*>(pGameObject));

	return S_OK;
}

HRESULT CUIMgr::CreateBattleUI(LPDIRECT3DDEVICE9 pGraphicDev,/* Engine::CLayer* pLayer,*/ UITYPE eUIType)
{
	ClearUI();
	m_eUIType = eUIType;

	////Player
	//CreatePlayerInfoUI(pGraphicDev, pLayer);

	Engine::CGameObject*		pGameObject = nullptr;

	if (UITYPE_BOSS == m_eUIType)
	{
		//StageTimer
		pGameObject = CStageTimer::Create(pGraphicDev, CStageTimer::BOSS, 523.7f, 1.f, 220.6f, 35.5f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"StageTimer", pGameObject);
		//pLayer->Add_GameObject(L"StageTimer", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

		pGameObject = CNoFuncObj::Create(pGraphicDev, CNoFuncObj::NOFUNCOBJ_BOSSNAMETXT, true, 583.2f, 47.5f, 110.f, 18.4f);//569.9f, 47.5f, 147.4f, 15.3f
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"NoFuncObj", pGameObject);
		//pLayer->Add_GameObject(L"NoFuncObj", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	}
	else if (UITYPE_SHOOTING == m_eUIType)
	{
		//StageTimer
		pGameObject = CStageTimer::Create(pGraphicDev, CStageTimer::SHOOTING, 523.7f, 1.f, 220.6f, 35.5f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"StageTimer", pGameObject);
		//pLayer->Add_GameObject(L"StageTimer", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

		pGameObject = CNoFuncObj::Create(pGraphicDev, CNoFuncObj::NOFUNCOBJ_APOSTLENAMETXT, true, 583.2f, 47.5f, 110.f, 18.4f);//569.9f, 47.5f, 147.4f, 15.3f
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"NoFuncObj", pGameObject);
		//pLayer->Add_GameObject(L"NoFuncObj", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	}

	////Monster
	//Name
	pGameObject = CBattleImg::Create(pGraphicDev, CBattleImg::BATTLEIMGTYPE_CIRCLE, 545.8f, 45.5f, 22.6f, 19.8f); // 530.9f, 45.5f, 22.6f, 19.8f
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BattleImg", pGameObject);
	//pLayer->Add_GameObject(L"BattleImg", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CBattleImg::Create(pGraphicDev, CBattleImg::BATTLEIMGTYPE_BATTLE, 540.9f, 40.1f, 33.f, 33.f); //526.f, 39.f, 33.f, 33.f
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BattleImg", pGameObject);
	//pLayer->Add_GameObject(L"BattleImg", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));




	////Monster
	pGameObject = CBackBar::Create(pGraphicDev, CBackBar::RECTTYPE, 480.f, 69.f, 327.f, 34.8f, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	//pLayer->Add_GameObject(L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CGroggy::Create(pGraphicDev, CGroggy::FULL, 484.5f, 76.f, 317.f, 5.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Groggy", pGameObject);
	//pLayer->Add_GameObject(L"Groggy", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CGroggy::Create(pGraphicDev, CGroggy::EMPTY, 632.8f, 70.f, 10.9f, 14.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Groggy", pGameObject);
	//pLayer->Add_GameObject(L"Groggy", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CMonsterHpMgr::Create(pGraphicDev);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"MonsterHpMgr", pGameObject);
	//pLayer->Add_GameObject(L"MonsterHpMgr", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	return S_OK;
}

HRESULT CUIMgr::CreateCartelUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer * pLayer)
{
	ClearUI();
	m_eUIType = UITYPE_CARTEL;

	Engine::CGameObject*		pGameObject = nullptr;

	//StageTimer
	pGameObject = CStageTimer::Create(pGraphicDev, CStageTimer::CARTEL, 525.7f, 1.48f, 220.6f, 35.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"StageTimer", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//FlagNoFuncObj - "스페이스바)깃발획득" 문구
	pGameObject = m_pFlagSpaceTXT = CFlagNoFuncObj::Create(pGraphicDev, CFlagNoFuncObj::FLAGNOFUNCOBJ_SPACEFLAG, false, 700.f, 342.f, 135.f, 47.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagNoFuncObj", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//FlagScore - bg
	pGameObject = CFlagScore::Create(pGraphicDev, CFlagScore::BACKGROUND, CFlagScore::BLUE, 550.f, 59.f, 55.f, 65.f, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagScore", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//FlagScore -bg
	pGameObject = CFlagScore::Create(pGraphicDev, CFlagScore::BACKGROUND, CFlagScore::RED, 677.f, 59.f, 55.f, 65.f, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagScore", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//FlagScore - outline
	pGameObject = CFlagScore::Create(pGraphicDev, CFlagScore::OUTLINE, CFlagScore::BLUE, 546.7f, 57.f, 60.7f, 69.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagScore", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//FlagScore - outline
	pGameObject = CFlagScore::Create(pGraphicDev, CFlagScore::OUTLINE, CFlagScore::RED, 673.9f, 57.0f, 60.f, 69.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagScore", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//FlagScore - 게이지
	pGameObject = CFlagScore::Create(pGraphicDev, CFlagScore::RECHARGE, CFlagScore::BLUE, 545.5f, 55.4f, 63.1f, 72.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagScoreGage", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//FlagScore - 게이지
	pGameObject = CFlagScore::Create(pGraphicDev, CFlagScore::RECHARGE, CFlagScore::RED, 672.4f, 55.6f, 63.5f, 72.8f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagScoreGage", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//FlagScore - 반짝반짝
	pGameObject = CFlagScoreFlash::Create(pGraphicDev, CFlagScoreFlash::FIRST, 577.5f, 90.7f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagScore", pGameObject);
	m_vecCurUI_Rc.emplace_back(dynamic_cast<CNormalObject*>(pGameObject));

	//FlagScore - 반짝반짝
	pGameObject = CFlagScoreFlash::Create(pGraphicDev, CFlagScoreFlash::SECOND, 577.5f, 90.7f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagScore", pGameObject);
	m_vecCurUI_Rc.emplace_back(dynamic_cast<CNormalObject*>(pGameObject));


	//FlagScore - scoreback
	pGameObject = CFlagScore::Create(pGraphicDev, CFlagScore::SCOREBACK, CFlagScore::FLAGCOLOR_END, 618.2f, 75.4f, 45.f, 25.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagScore", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//FlagNoFuncObj - goal txt
	pGameObject = CFlagNoFuncObj::Create(pGraphicDev, CFlagNoFuncObj::FLAGNOFUNCOBJ_GOALTXT, true, 625.3f, 55.7f, 30.3f, 23.8f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagNoFuncObj", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//FlagNoFuncObj - 검은배경
	pGameObject = CFlagNoFuncObj::Create(pGraphicDev, CFlagNoFuncObj::FLAGNOFUNCOBJ_BG, false, 574.2f, 131.7f, 129.6f, 30.7f, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagNoFuncObj", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	m_vecFlagNoFuncObj.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	////////////////////////////////////////////////////// ->stagetimer클래스 안에서 렌더 트루로 바꿔줌
	//FlagNoFuncObj - 종료임박
	pGameObject = CFlagNoFuncObj::Create(pGraphicDev, CFlagNoFuncObj::FLAGNOFUNCOBJ_ENDTXT, false, 613.1f, 153.2f, 60.7f, 27.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"FlagNoFuncObj", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	m_vecFlagNoFuncObj.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//FlagNoFuncObj - Yellow1
	pGameObject = CYellow::Create(pGraphicDev, CYellow::GOLEFT, false, 574.2f, 131.7f, 64.8f, 30.7f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"Yellow", pGameObject);
	m_vecFlagNoFuncObj.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->움직임필요할때만 풀어서 하고 담지말기 알아서없어지게!

	//FlagNoFuncObj - Yellow2
	pGameObject = CYellow::Create(pGraphicDev, CYellow::GORIGHT, false, 639.f, 131.7f, 64.8f, 30.7f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"Yellow", pGameObject);
	m_vecFlagNoFuncObj.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->움직임필요할때만 풀어서 하고 담지말기 알아서없어지게!
	//////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CUIMgr::CreateMainUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer* pLayer)
{
	ClearUI();
	m_eUIType = UITYPE_MAIN;

	Engine::CGameObject*		pGameObject = nullptr;

	//
	pGameObject = CNoFuncObj::Create(pGraphicDev, CNoFuncObj::NOFUNCOBJ_MAPTXT, false, 545.8f, 35.f, 198.2f, 30.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"NoFuncObj", pGameObject);
	m_vecNoFuncObj.emplace_back(dynamic_cast<CNoFuncObj*>(pGameObject));

	//
	//우측상단 퀘스트창
	pGameObject = CQuestBackBar::Create(pGraphicDev, CQuestBackBar::QUEST, CQuestMgr::NPCTYPE_END, true, 1020.8f, 129.5f, 251.5f, 146.7f, 0.1f); //1020.8f, 179.5f
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"QuestBackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//first 체크박스
	pGameObject = CQuestCheckBox::Create(pGraphicDev, CQuestCheckBox::RECT, CQuestCheckBox::FIRST, true, 1041.7f, 181.3f, 16.f, 16.f, 0.1f); //1041.7f, 231.3f
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"QuestCheckBox", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CQuestCheckBox::Create(pGraphicDev, CQuestCheckBox::CHECK, CQuestCheckBox::FIRST, false, 1039.6f, 174.6f, 25.f, 18.f, 0.1f); //1039.6f, 224.6f,
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"QuestCheckBox", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//second 체크박스
	pGameObject = CQuestCheckBox::Create(pGraphicDev, CQuestCheckBox::RECT, CQuestCheckBox::SECOND, true, 1041.7f, 206.3f, 16.f, 16.f, 0.1f); //1041.7f, 256.3f
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"QuestCheckBox", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CQuestCheckBox::Create(pGraphicDev, CQuestCheckBox::CHECK, CQuestCheckBox::SECOND, false, 1039.6f, 199.6f, 25.f, 18.f, 0.1f);  //1039.6f, 249.6f
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"QuestCheckBox", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	return S_OK;
}

HRESULT CUIMgr::CreateMissionUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	ClearUI();
	m_eUIType = UITYPE_MISSION;

	Engine::CGameObject*		pGameObject = nullptr;
	Engine::CGameObject*		pSelectMission = nullptr;

	//BackBar -> 미션UI 뒷배경
	pGameObject = CBackBar::Create(pGraphicDev, CBackBar::MISSIONBGTYPE, 0.f, 0.f, WINCX, WINCY, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//-------------------------------------------------
	//MissionLock
	if (m_bMissionFlagLock)
	{
		//깃발
		pGameObject = CMissionLock::Create(pGraphicDev, CMissionLock::LOCKWITHTXT, 176.8f, 141.6f, 218.9f, 337.3f, 0.f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"MissionLock", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

		//첫번째 퀘스트 성공
		CQuestMgr::GetInstance()->Set_YellowPotalQCompleted(true);
	}

	if (m_bMissionApostleLock)
	{
		//AI ->돼지천사
		pGameObject = CMissionLock::Create(pGraphicDev, CMissionLock::LOCK, 526.8f, 141.6f, 218.9f, 337.3f, 0.f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"MissionLock", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	}

	if (m_bMissionBossLock)
	{
		//보스 ->벨라토스
		pGameObject = CMissionLock::Create(pGraphicDev, CMissionLock::LOCK, 876.8f, 141.6f, 218.9f, 337.3f, 0.f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"MissionLock", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	}

	//-------------------------------------------------
	//MissionCard
	pGameObject = CMissionCard::Create(pGraphicDev, CMissionCard::CTF, 176.8f, 141.6f, 218.9f, 337.3f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"MissionCard", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//SelectMission 
	pSelectMission = CSelectMission::Create(pGraphicDev, CSelectMission::HORIZON, pGameObject, 148.8f, 102.2f, 272.7f, 14.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"SelectMission", pSelectMission);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pSelectMission));

	//SelectMission 
	pSelectMission = CSelectMission::Create(pGraphicDev, CSelectMission::VERTICAL1, pGameObject, 148.8f, 98.4f, 14.f, 424.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"SelectMission", pSelectMission);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pSelectMission));

	//SelectMission 
	pSelectMission = CSelectMission::Create(pGraphicDev, CSelectMission::VERTICAL2, pGameObject, 410.2f, 97.4f, 14.f, 424.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"SelectMission", pSelectMission);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pSelectMission));
	//-------------------------------------------------
	//-------------------------------------------------
	//MissionCard AI->돼지천사
	pGameObject = CMissionCard::Create(pGraphicDev, CMissionCard::AI, 526.8f, 141.6f, 218.9f, 337.3f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"MissionCard", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//SelectMission 
	pSelectMission = CSelectMission::Create(pGraphicDev, CSelectMission::HORIZON, pGameObject, 497.8f, 102.f, 275.8f, 14.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"SelectMission", pSelectMission);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pSelectMission));

	//SelectMission
	pSelectMission = CSelectMission::Create(pGraphicDev, CSelectMission::VERTICAL1, pGameObject, 498.1f, 97.7f, 14.f, 424.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"SelectMission", pSelectMission);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pSelectMission));

	//SelectMission 
	pSelectMission = CSelectMission::Create(pGraphicDev, CSelectMission::VERTICAL2, pGameObject, 761.4f, 97.6f, 14.f, 424.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"SelectMission", pSelectMission);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pSelectMission));

	//-------------------------------------------------
	//-------------------------------------------------
	//MissionCard
	pGameObject = CMissionCard::Create(pGraphicDev, CMissionCard::RAID, 876.8f, 141.6f, 218.9f, 337.3f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"MissionCard", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//SelectMission 
	pSelectMission = CSelectMission::Create(pGraphicDev, CSelectMission::HORIZON, pGameObject, 848.4f, 101.4f, 274.4f, 14.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"SelectMission", pSelectMission);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pSelectMission));

	//SelectMission
	pSelectMission = CSelectMission::Create(pGraphicDev, CSelectMission::VERTICAL1, pGameObject, 849.2f, 97.5f, 14.f, 425.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"SelectMission", pSelectMission);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pSelectMission));

	//SelectMission 
	pSelectMission = CSelectMission::Create(pGraphicDev, CSelectMission::VERTICAL2, pGameObject, 1108.6f, 97.f, 14.f, 424.7f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"SelectMission", pSelectMission);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pSelectMission));

	//-------------------------------------------------
	//NoFuncObj들
	pGameObject = CNoFuncObj::Create(pGraphicDev, CNoFuncObj::NOFUNCOBJ_KEYGUIDE, true, 1081.4f, 651.f, 172.3f, 42.9f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"NoFuncObj", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));



	return S_OK;
}

HRESULT CUIMgr::CreateReadyUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	ClearUI();
	m_eUIType = UITYPE_READY;

	Engine::CGameObject*		pGameObject = nullptr;

	//BackBar -> 뒷배경
	pGameObject = CBackBar::Create(pGraphicDev, CBackBar::READYBGTYPE, 0.f, 0.f, WINCX, WINCY, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//----------------------------------------------
	//KarmaSlot
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORBASIC, CKarmaSlot::INREADYUI, 583.1f, 543.7f, 67.3f, 66.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//KarmaSlot
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORBASIC, CKarmaSlot::INREADYUI, 651.f, 542.3f, 67.3f, 66.2f); // 651.f, 580.f, 55.7f, 55.f
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Karma
	pGameObject = CKarma::Create(pGraphicDev, SELECTKARMATYPE_FORBASIC, CKarma::MAIN, CKarma::KARMA_TS, 583.1f, 543.7f, 67.3f, 66.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Karma", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	m_listKarma.emplace_back(dynamic_cast<CKarma*>(pGameObject));

	//Karma
	pGameObject = CKarma::Create(pGraphicDev, SELECTKARMATYPE_FORBASIC, CKarma::SUB, CKarma::KARMA_TS, 651.f, 542.3f, 67.3f, 66.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Karma", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	m_listKarma.emplace_back(dynamic_cast<CKarma*>(pGameObject));
	//--------------------------------------------------

	//NoFuncobj - ping
	pGameObject = CNoFuncObj::Create(pGraphicDev, CNoFuncObj::NOFUNCOBJ_PING, true, 602.1f, 230.7f, 96.7f, 19.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"NoFuncObj", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//NoFuncobj - waitingTXT
	pGameObject = CNoFuncObj::Create(pGraphicDev, CNoFuncObj::NOFUNCOBJ_WAITINGTXT, false, 621.7f, 460.7f, 116.9f, 27.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"NoFuncObj", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Button
	pGameObject = CButton::Create(pGraphicDev, 601.4f, 643.2f, 100.5f, 28.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Button", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//ReadyBattle
	pGameObject = CReadyBattle::Create(pGraphicDev, 479.5f, 260.5f, 344.2f, 185.f, 0.05f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"ReadyBattle", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Waiting -> RcTex
	pGameObject = CWaiting::Create(pGraphicDev, 594.f, 472.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Waiting", pGameObject);
	m_vecCurUI_Rc.emplace_back(dynamic_cast<CNormalObject*>(pGameObject));

	//NoFuncobj - 삼각형화살표
	pGameObject = CNoFuncObj::Create(pGraphicDev, CNoFuncObj::NOFUNCOBJ_TRIANGLE, true, 641.8f, 428.3f, 20.8f, 17.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"NoFuncObj", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	return S_OK;
}

HRESULT CUIMgr::CreateResultUI_Boss(LPDIRECT3DDEVICE9 pGraphicDev/*, Engine::CLayer* pLayer*/)
{
	// 12 개 폰트매니저

	ClearUI();
	m_eUIType = UITYPE_RESULT_Boss;

	Engine::CGameObject*		pGameObject = nullptr;

	//BackBar -> 뒷배경
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::RESULTBGTYPE, 0.f, 0.f, WINCX, WINCY, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//BackBar -> 메뉴 라인
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::RECTTYPE, 107.9f, 300.2f, 730.5f, 32.9f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//BackBar -> 남색 라인
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::INDIGOCOLORTYPE, 115.f, 332.f, 716.f, 55.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//KarmaSlot 1 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 140.f, 345.f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//KarmaSlot 2 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 170.5f, 345.f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Karma
	pGameObject = CKarma::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarma::MAIN, CKarma::KARMA_TS, 140.f, 345.f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Karma", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Karma
	pGameObject = CKarma::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarma::SUB, CKarma::KARMA_TS, 170.5f, 345.f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Karma", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//m_bChangeScene = true;

	//나가기 버튼
	//Matching Button - EXIT
	pGameObject = CMatchingButton::Create(pGraphicDev, CMatchingButton::EXIT1, 1081.4f, 651.f, 172.3f, 42.9f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"MatchingButton", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	// belatos
	CSoundMgr::Get_Instance()->SetVolumeDown(0.3f);
	CSoundMgr::Get_Instance()->HoSoundOn(16, 1.f);

	//빨간머리NPC 의 미션을 클리어함
	CQuestMgr::GetInstance()->Set_SecondMissionCompleted(CQuestMgr::RED, true);

	return S_OK;
}

HRESULT CUIMgr::CreateResultUI_Cartel(LPDIRECT3DDEVICE9 pGraphicDev)
{
	// 26개 폰트매니저

	ClearUI();
	m_eUIType = UITYPE_RESULT_Cartel;

	Engine::CGameObject*		pGameObject = nullptr;

	//BackBar -> 뒷배경
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::RESULTBGTYPE, 0.f, 0.f, WINCX, WINCY, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//BackBar -> 메뉴 라인
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::RECTTYPE, 107.9f, 300.2f, 730.5f, 32.9f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//BackBar -> 남색 라인
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::INDIGOCOLORTYPE, 115.f, 332.f, 716.f, 68.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//BackBar -> 남색 라인 ->빨간색팀꺼...
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::REDCOLORTYPE, 115.f, 400.f, 716.f, 68.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//점수동그라미
	pGameObject = CFlagTotalScore::Create(pGraphicDev, CFlagTotalScore::BG, CFlagTotalScore::BLUE, 117.5f, 216.4f, 71.f, 71.f, 0.15f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"FlagTotalScore", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CFlagTotalScore::Create(pGraphicDev, CFlagTotalScore::BG, CFlagTotalScore::RED, 301.7f, 216.4f, 71.f, 71.f, 0.15f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"FlagTotalScore", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	///////////////////////////////////////////////// 정보
	////Player
	//KarmaSlot 1 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 140.f, 335.1f, 30.f, 30.f); //140.f, 345.f, 30.f, 30.f
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//KarmaSlot 2 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 170.5f, 335.1f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Karma
	pGameObject = CKarma::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarma::MAIN, CKarma::KARMA_TS, 140.f, 335.1f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Karma", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Karma
	pGameObject = CKarma::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarma::SUB, CKarma::KARMA_TS, 170.5f, 335.1f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Karma", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//////////////////위치조정 ㄱㄱㄱㄱ
	//Alliance - OBJID:OBJECT_ALLIANCE
	//KarmaSlot 1 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 140.f, 367.7f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//KarmaSlot 2 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 170.5f, 367.7f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//AI_Karma
	pGameObject = CAI_Karma::Create(pGraphicDev, OBJECT_ALLIANCE, CAI_Karma::MAIN, 140.f, 367.7f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaAI", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//AI_Karma
	pGameObject = CAI_Karma::Create(pGraphicDev, OBJECT_ALLIANCE, CAI_Karma::SUB, 170.5f, 367.7f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaAI", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	////Enenmy_1 -OBJID:OBJECT_ENEMY_1
	//KarmaSlot 1 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 140.f, 401.8f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//KarmaSlot 2 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 170.5f, 401.8f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//AI_Karma
	pGameObject = CAI_Karma::Create(pGraphicDev, OBJECT_ENEMY_1, CAI_Karma::MAIN, 140.f, 401.8f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaAI", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//AI_Karma
	pGameObject = CAI_Karma::Create(pGraphicDev, OBJECT_ENEMY_1, CAI_Karma::SUB, 170.5f, 401.8f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaAI", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	////Enemy_2 -OBJID:OBJECT_ENEMY_2
	//KarmaSlot 1 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 140.f, 433.9f, 30.f, 30.f); //432.9f
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//KarmaSlot 2 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 170.5f, 433.9f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//AI_Karma
	pGameObject = CAI_Karma::Create(pGraphicDev, OBJECT_ENEMY_2, CAI_Karma::MAIN, 140.f, 433.9f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaAI", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//AI_Karma
	pGameObject = CAI_Karma::Create(pGraphicDev, OBJECT_ENEMY_2, CAI_Karma::SUB, 170.5f, 433.9f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaAI", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//나가기 버튼
	//Matching Button - EXIT
	pGameObject = CMatchingButton::Create(pGraphicDev, CMatchingButton::EXIT1, 1081.4f, 651.f, 172.3f, 42.9f, 0.1f); //1081.4f, 651.f, 172.3f, 42.9f //314.f, 78.f,
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"MatchingButton", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	// 2vs2
	CSoundMgr::Get_Instance()->SetVolumeDown(0.3f);
	CSoundMgr::Get_Instance()->HoSoundOn(16, 1.f);

	return S_OK;
}

HRESULT CUIMgr::CreateResultUI_ShootingStage(LPDIRECT3DDEVICE9 pGraphicDev)
{
	ClearUI();
	m_eUIType = UITYPE_RESULT_Shooting;

	Engine::CGameObject*		pGameObject = nullptr;

	//BackBar -> 뒷배경
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::RESULTBGTYPE, 0.f, 0.f, WINCX, WINCY, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//BackBar -> 메뉴 라인
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::RECTTYPE, 107.9f, 300.2f, 730.5f, 32.9f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//BackBar -> 남색 라인
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::INDIGOCOLORTYPE, 115.f, 332.f, 716.f, 55.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//KarmaSlot 1 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 140.f, 345.f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//KarmaSlot 2 
	pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarmaSlot::INRESULTUI, 170.5f, 345.f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Karma
	pGameObject = CKarma::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarma::MAIN, CKarma::KARMA_TS, 140.f, 345.f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Karma", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Karma
	pGameObject = CKarma::Create(pGraphicDev, SELECTKARMATYPE_FORRESULT, CKarma::SUB, CKarma::KARMA_TS, 170.5f, 345.f, 30.f, 30.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"Karma", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//m_bChangeScene = true;

	//나가기 버튼
	//Matching Button - EXIT
	pGameObject = CMatchingButton::Create(pGraphicDev, CMatchingButton::EXIT1, 1081.4f, 651.f, 172.3f, 42.9f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"MatchingButton", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	// shoot
	CSoundMgr::Get_Instance()->SetVolumeDown(0.3f);
	CSoundMgr::Get_Instance()->HoSoundOn(16, 1.f);

	return S_OK;
}

HRESULT CUIMgr::CreateResultUI_Run(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if(!CCameraMgr::GetInstance()->Get_ItemGetCheck())
		EraseRunButton();

	m_eUIType = UITYPE_RESULT_Run;

	Engine::CGameObject*		pGameObject = nullptr;

	//BackBar -> 뒷배경
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::RESULTBGTYPE, 0.f, 0.f, WINCX, WINCY, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//BackBar -> 메뉴 라인
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::WHITELINE, 657.9f, 300.2f, 580.f, 32.9f, 0.1f); // 107.9f, 300.2f,730.5f,32.9f
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//BackBar -> 남색 라인
	pGameObject = CResultBackBar::Create(pGraphicDev, CResultBackBar::INDIGOCOLORTYPE, 665.f, 332.f, 565.5f, 55.f, 0.1f); //115.f, 332.f,716.f, 55.f
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"BackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));




	//나가기 버튼
	//Matching Button - EXIT
	pGameObject = CMatchingButton::Create(pGraphicDev, CMatchingButton::EXIT1, 1081.4f, 651.f, 172.3f, 42.9f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"MatchingButton", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	return S_OK;
}

HRESULT CUIMgr::CreateMatchingUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer * pLayer)
{
	ClearUI();
	m_eUIType = UITYPE_MATCHING;

	Engine::CGameObject*		pGameObject = nullptr;

	//StageTimer
	pGameObject = CStageTimer::Create(pGraphicDev, CStageTimer::MATCHING, 537.2f, 1.f, 220.6f, 35.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"StageTimer", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Player 점수판 - 1의자리
	pGameObject = CMatchingScore::Create(pGraphicDev, CMatchingMgr::TURN_PLAYER, CMatchingScore::UNITS, 117.4f, 133.6f, 50.f, 65.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"MatchingScore", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Player 점수판 - 10의자리
	pGameObject = CMatchingScore::Create(pGraphicDev, CMatchingMgr::TURN_PLAYER, CMatchingScore::TENS, 73.4f, 133.6f, 50.f, 65.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"MatchingScore", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//AI 점수판 - 1의자리
	pGameObject = CMatchingScore::Create(pGraphicDev, CMatchingMgr::TURN_AI, CMatchingScore::UNITS, 1172.9f, 133.6f, 50.f, 65.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"MatchingScore", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//AI 점수판 - 10의자리
	pGameObject = CMatchingScore::Create(pGraphicDev, CMatchingMgr::TURN_AI, CMatchingScore::TENS, 1130.4f, 133.6f, 50.f, 65.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"MatchingScore", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Turn - player
	pGameObject = CMatchingTurn::Create(pGraphicDev, CMatchingMgr::TURN_PLAYER, false, -44.28f, 328.57f, 360.f, 78.2f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"MatchingTurn", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Turn - ai
	pGameObject = CMatchingTurn::Create(pGraphicDev, CMatchingMgr::TURN_AI, false, 956.2f, 328.57f, 360.f, 78.2f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"MatchingTurn", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Matching Button - START
	pGameObject = CMatchingButton::Create(pGraphicDev, CMatchingButton::START1, 351.8f, 500.f, 314.f, 78.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"MatchingButton", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//Matching Button - EXIT
	pGameObject = CMatchingButton::Create(pGraphicDev, CMatchingButton::EXIT1, 660.f, 500.f, 314.f, 78.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"MatchingButton", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));


	return S_OK;
}

HRESULT CUIMgr::CreateNPCMissionUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::CGameObject* pGameObject = nullptr;

	//우측상단 퀘스트창
	pGameObject = CQuestBackBar::Create(pGraphicDev, CQuestBackBar::QUEST, CQuestMgr::NPCTYPE_END, true, 1020.8f, 129.5f, 251.5f, 146.7f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"QuestBackBar", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//first 체크박스
	pGameObject = CQuestCheckBox::Create(pGraphicDev, CQuestCheckBox::RECT, CQuestCheckBox::FIRST, true, 1041.7f, 181.3f, 16.f, 16.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"QuestCheckBox", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CQuestCheckBox::Create(pGraphicDev, CQuestCheckBox::CHECK, CQuestCheckBox::FIRST, false, 1039.6f, 174.6f, 25.f, 18.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"QuestCheckBox", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//second 체크박스
	pGameObject = CQuestCheckBox::Create(pGraphicDev, CQuestCheckBox::RECT, CQuestCheckBox::SECOND, false, 1041.7f, 206.3f, 16.f, 16.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"QuestCheckBox", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CQuestCheckBox::Create(pGraphicDev, CQuestCheckBox::CHECK, CQuestCheckBox::SECOND, false, 1039.6f, 199.6f, 25.f, 18.f, 0.1f); //21 16
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"QuestCheckBox", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));


	return S_OK;
}

HRESULT CUIMgr::CreateRunUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_eUIType = UITYPE_RUN;
	Engine::CGameObject* pGameObject = nullptr;

	//카운트다운
	pGameObject = CRunCountDown::Create(pGraphicDev, CRunCountDown::RUNCOUNTDOWNTYPE_GAMESTART, 605.f, 290.f, 75.f, 97.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"RunCountDown", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	///////////////////
	//runplayerhp
	pGameObject = CRunPlayerHP::Create(pGraphicDev,CRunPlayerHP::HP, 400.f, 50.f, 480.f, 20.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"RunPlayerHP", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//runplayerhp
	pGameObject = CRunPlayerHP::Create(pGraphicDev, CRunPlayerHP::BACKBAR, 397.4f, 47.3f, 485.5f, 24.3f,0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"RunPlayerHP", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	///////////////////
	
	//runplayeritem
	pGameObject = CRunPlayerItem::Create(pGraphicDev, CRunPlayerItem::BIG, 1060.f, 25.4f, 82.f, 82.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"RunPlayerItem", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//runplayeritem
	pGameObject = CRunPlayerItem::Create(pGraphicDev, CRunPlayerItem::SPEEDUP, 1143.4f, 25.4f, 82.f, 82.f);
	if (pGameObject == nullptr)
		return E_FAIL;
	Engine::Add_GameObject(Engine::UI, L"RunPlayerItem", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
	/////////////////

	for(_uint i = 0; i < 2; ++i)
	{
		pGameObject = CRunPlayerItemSlot::Create(pGraphicDev, CRunPlayerItemSlot::BASIC, CRunPlayerItemSlot::ITEMTYPE(i),true, 1060.f + (i*83.4f), 25.4f, 82.f, 82.f, 0.2f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"RunPlayerItemSlot", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

		pGameObject = CRunPlayerItemSlot::Create(pGraphicDev, CRunPlayerItemSlot::RECHARGE, CRunPlayerItemSlot::ITEMTYPE(i), false, 1060.f + (i*83.4f), 25.4f, 82.f, 82.f, 0.1f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"RunPlayerItemSlot", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));


		pGameObject = CRunPlayerItemSlot::Create(pGraphicDev, CRunPlayerItemSlot::OUTLINE, CRunPlayerItemSlot::ITEMTYPE(i), true, 1060.f + (i*83.4f), 25.4f, 82.f, 82.f);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"RunPlayerItemSlot", pGameObject);
		m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	}



	//노란머리NPC 의 미션을 클리어함
	CQuestMgr::GetInstance()->Set_MissionCompleted(CQuestMgr::YELLOW, true);

	return S_OK;
}

HRESULT CUIMgr::CreateCustomizeUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer * pLayer)
{
	m_eUIType = UITYPE_CUSTOMIZE;

	Engine::CGameObject* pGameObject = nullptr;

	//메뉴
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::FACE1, 450.f, 50.f, 100.f, 40.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"CustomizingMenu", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//메뉴
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::HAIR1, 575.f, 50.f, 100.f, 40.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"CustomizingMenu", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//메뉴
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::CLOTH1, 680.f, 50.f, 100.f, 40.f, 0.1f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"CustomizingMenu", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	//오른쪽 검은배경
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::BG, 880.f, 0.f, 400.f, 720.f, 0.2f);
	if (pGameObject == nullptr)
		return E_FAIL;
	pLayer->Add_GameObject(L"CustomizingMenu", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	return S_OK;
}

void CUIMgr::ClearUI()
{
	if (0 < m_vecCurUI.size())
	{
		for (auto& pUI : m_vecCurUI)
		{
			pUI->Set_Dead();
		}

		m_vecCurUI.clear();
	}

	if (0 < m_vecCurUI_Rc.size())
	{
		for (auto& pUI : m_vecCurUI_Rc)
		{
			pUI->Set_Dead();
		}

		m_vecCurUI_Rc.clear();
	}

	if (UITYPE_BOSS == m_eUIType || UITYPE_CARTEL == m_eUIType || UITYPE_SHOOTING == m_eUIType)
	{
		if (nullptr == m_pUltimateRecharge)
			return;

		m_pUltimateRecharge->Set_Dead();//
		m_pUltimateRecharge = nullptr;//


		if (0 >= m_vecSkill.size())
			return;

		for (auto& pSkill : m_vecSkill)
		{
			pSkill->Set_Dead();
		}
		m_vecSkill.clear();
	}

	if (UITYPE_CARTEL == m_eUIType)
	{
		for (auto& pFlag : m_vecFlagNoFuncObj)
		{
			pFlag = nullptr;
		}
		m_vecFlagNoFuncObj.clear();

	}

	if (UITYPE_RESULT_Cartel == m_eUIType)
	{
		m_pFlagSpaceTXT = nullptr;
	}

	if (UITYPE_READY == m_eUIType)
	{
		for (auto& pList : m_listKarma)
		{
			pList->Set_Dead();
		}
		m_listKarma.clear();

		for (auto& pList : m_listKarmaSlot)
		{
			pList->Set_Dead();
		}
		m_listKarmaSlot.clear();
	}	
}

void CUIMgr::ClearPointerUI()
{
	m_pMouse = nullptr;

	//if (m_vecCurUI.size() > 0)
	m_vecCurUI.clear();
	m_vecCurUI.shrink_to_fit();

	m_vecCurUI_Rc.clear();
	m_vecCurUI_Rc.shrink_to_fit();

	m_vecSkill.clear();
	m_vecSkill.shrink_to_fit();

	m_vecFlagNoFuncObj.clear();
	m_vecFlagNoFuncObj.shrink_to_fit();

	m_vecSkillSlot.clear();
	m_vecSkillSlot.shrink_to_fit();

	m_vecNoFuncObj.clear();
	m_vecNoFuncObj.shrink_to_fit();

	m_vecFlagBackBar.clear();
	m_vecFlagBackBar.shrink_to_fit();

	m_vecFlagPos.clear();
	m_vecFlagPos.shrink_to_fit();


	m_pFlagSpaceTXT = nullptr;//

	m_listFlagDeathInfo.clear();

	m_listNPCNoFuncObj.clear();//

	m_listKarma.clear();//

	m_listKarmaSlot.clear();//
}

void CUIMgr::SkillCoolDownCheck(Engine::KEYGUIDE KeyType)
{
	if (Engine::KEYGUIDE_Q != KeyType) //스킬 Q를 제외한 나머지
	{
		for (_uint i = -0; i < m_vecSkillSlot.size(); ++i)
		{
			if (CSkillSlot::RECHARGE == m_vecSkillSlot[i]->Get_SkillSlotType())
			{
				//스킬슬롯-charge 애들 중에 렌더중인것들을 체크하고(main-렌더중이면 sub-렌더안하고 있으니까)
				if (m_vecSkillSlot[i]->Get_RenderUI())
				{
					if (KeyType == m_vecSkillSlot[i]->Get_KeyType())
					{
						// 렌더중인 아이들 중 스킬쿨다운을 하고 있는중인지를 체크
						if (m_vecSkillSlot[i]->Get_CoolDowning())
						{
							//// --> 용호가 플레이어 안에서 하기로함
							////쿨다운 중이라면 -> 쿨다운 중이라는 유아이 띄우고
							//for (auto & pVec : m_vecNoFuncObj)
							//{
							//	if(CNoFuncObj::NOFUNCOBJ_COOLDOWNTXT == pVec->Get_NoFuncObjType())
							//		pVec->Set_RenderUI(true);
							//}
						}
						else
						{
							//아니라면 셋 해줌
							m_vecSkillSlot[i]->Set_CoolDown();
							m_vecSkillSlot[i]->Set_CoolDowning();
						}
						break;
					}
				}
			}
		}
	}
	else//스킬 Q
	{
		for (_uint i = -0; i < m_vecCurUI.size(); ++i)
		{
			if (dynamic_cast<CUltimateSlot*>(m_vecCurUI[i]))
			{
				if (CUltimateSlot::RECHARGE_INSIDE == dynamic_cast<CUltimateSlot*>(m_vecCurUI[i])->Get_UltimateType())
				{
					m_vecCurUI[i]->Set_RenderUI(true);
				}

				if (CUltimateSlot::RECHARGE_OUTLINE == dynamic_cast<CUltimateSlot*>(m_vecCurUI[i])->Get_UltimateType())
				{
					m_vecCurUI[i]->Set_RenderUI(true);
				}
			}
		}
	}
}

_bool CUIMgr::CheckSkillCoolDowning(Engine::KEYGUIDE KeyType)
{
	for (_uint i = 0; i < m_vecSkillSlot.size(); ++i)
	{
		if (CSkillSlot::RECHARGE == m_vecSkillSlot[i]->Get_SkillSlotType())
		{
			//스킬슬롯-charge 애들 중에 렌더중인것들을 체크하고(main-렌더중이면 sub-렌더안하고 있으니까)
			if (m_vecSkillSlot[i]->Get_RenderUI())
			{
				if (KeyType == m_vecSkillSlot[i]->Get_KeyType())
				{
					if (m_vecSkillSlot[i]->Get_CoolDowning())
					{
						//OnCoolDown();
						return true;
					}
					else
						return false;
				}
			}
		}
	}
	return false;
}

void CUIMgr::OnCoolDown()
{
	for (auto & pVec : m_vecNoFuncObj)
	{
		if (CNoFuncObj::NOFUNCOBJ_COOLDOWNTXT == pVec->Get_NoFuncObjType())
		{
			pVec->Set_RenderUI(true);
			break;
		}
	}
}

void CUIMgr::UseableSkillCheck(Engine::KEYGUIDE KeyType)
{
	//인자로 받는 키만 색깔이 들어오고 나머지는 안들어옴


	for (_uint i = -0; i < m_vecSkillSlot.size(); ++i)
	{
		if (CSkillSlot::RECHARGE == m_vecSkillSlot[i]->Get_SkillSlotType())
		{
			//스킬슬롯-charge 애들 중에 렌더중인것들을 체크하고(main-렌더중이면 sub-렌더안하고 있으니까)
			if (m_vecSkillSlot[i]->Get_RenderUI())
			{
				if (KeyType != m_vecSkillSlot[i]->Get_KeyType())
				{
					m_vecSkillSlot[i]->Set_Rendering(false);
				}
			}
		}
	}
}

void CUIMgr::UseableSkillCheck_All(_bool _bBool)
{
	for (_uint i = -0; i < m_vecSkillSlot.size(); ++i)
	{
		if (CSkillSlot::RECHARGE == dynamic_cast<CSkillSlot*>(m_vecSkillSlot[i])->Get_SkillSlotType())
		{
			//스킬슬롯-charge 애들 중에 렌더중인것들을 체크하고(main-렌더중이면 sub-렌더안하고 있으니까)
			if (m_vecSkillSlot[i]->Get_RenderUI())
			{
				m_vecSkillSlot[i]->Set_Rendering(_bBool);
			}
		}
	}
}

void CUIMgr::SwitchSkill(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrTexture, CSkill::SKILLTYPE eType)
{
	//wstrtexture:::
	//TS -> Skill_TS          
	//DS - Skill_DS
	//ORB -> Skill_ORB
	//LB -> Skill_LB

	if (0 >= m_vecSkill.size())
		return;

	//원래 있던거 삭제해주고
	for (auto& pSkill : m_vecSkill)
	{
		pSkill->Set_Dead();
	}
	m_vecSkill.clear();

	//다시 생성
	//UltimateSkill - Q
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CSkill::Create(pGraphicDev, wstrTexture, eType, Engine::KEYGUIDE_Q, 58.5f, 83.2f, 80.6f, 88.6f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"Skill", pGameObject);
	m_vecSkill.emplace_back(dynamic_cast<CSkill*>(pGameObject));

	//Skill - E/R/F
	for (int i = 0; i < 3; ++i)
	{
		pGameObject = CSkill::Create(pGraphicDev, wstrTexture, eType, Engine::KEYGUIDE(i), 157.5f + 53.f*i, 62.5f, 57.f, 59.f);
		if (pGameObject == nullptr)
			return;
		Engine::Add_GameObject(Engine::UI, L"Skill", pGameObject);
		m_vecSkill.emplace_back(dynamic_cast<CSkill*>(pGameObject));
	}

	for (auto& pVec : m_vecSkillSlot)
	{
		if (CSkillSlot::RECHARGE == pVec->Get_SkillSlotType())
		{
			if (CSkillSlot::MAIN == pVec->Get_WeaponType() || CSkillSlot::SUB == pVec->Get_WeaponType())
				pVec->Set_SwitchRenderUI(true);
		}

	}
}

void CUIMgr::CreateDamageBox(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform * pTargetTransform, CDamageBox::DAMAGEBOXTYPE eType, _uint iDamage)
{
	Engine::CGameObject* pGameObject = CDamageBox::Create(pGraphicDev, pTargetTransform, eType, iDamage);
	Engine::Add_GameObject(Engine::UI, L"DamageBox", pGameObject);
}

void CUIMgr::CreateTiredUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform * pTargetTransform)
{
	Engine::CGameObject* pGameObject = nullptr;

	if (UITYPE_BOSS == m_eUIType)
		pGameObject = CTired::Create(pGraphicDev, pTargetTransform);
	else if (UITYPE_SHOOTING == m_eUIType)
		pGameObject = CTired::Create(pGraphicDev, nullptr, 990.f, 99.2f);

	Engine::Add_GameObject(Engine::UI, L"Tired", pGameObject);

	for (_uint i = 0; i < m_vecNoFuncObj.size(); ++i)
	{
		if (CNoFuncObj::NOFUNCOBJ_STAMINATXT == m_vecNoFuncObj[i]->Get_NoFuncObjType())
		{
			m_vecNoFuncObj[i]->Set_RenderUI(true);
			break;
		}
	}
}

void CUIMgr::EraseTiredUI()
{
	Engine::CGameObject* pGameObject = Engine::Get_GameObject(Engine::UI, L"Tired");
	if (nullptr == pGameObject)
		return;

	dynamic_cast<CTired*>(pGameObject)->Set_Dead();
}

void CUIMgr::CreateDeficientMP()
{
	for (_uint i = 0; i < m_vecNoFuncObj.size(); ++i)
	{
		if (CNoFuncObj::NOFUNCOBJ_DEFICIENTMPTXT == m_vecNoFuncObj[i]->Get_NoFuncObjType())
		{
			m_vecNoFuncObj[i]->Set_RenderUI(true);
			break;
		}
	}
}

void CUIMgr::Set_QSkillEnd()
{
	dynamic_cast<CBasePlayer*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"RealPlayer"))->Set_RageZero();

	if (nullptr == Engine::Get_GameObject(Engine::UI, L"UltimateRecharge"))
		return;

	dynamic_cast<CUltimateRecharge*>(Engine::Get_GameObject(Engine::UI, L"UltimateRecharge"))->Set_Full(false);
	dynamic_cast<CUltimateRecharge*>(Engine::Get_GameObject(Engine::UI, L"UltimateRecharge"))->Set_Create();

	for (_uint i = 0; i < m_vecCurUI.size(); ++i)
	{
		if (dynamic_cast<CUltimateSlot*>(m_vecCurUI[i]))
		{
			if (CUltimateSlot::RECHARGE_OUTLINE == dynamic_cast<CUltimateSlot*>(m_vecCurUI[i])->Get_UltimateType())
			{
				(m_vecCurUI[i])->Set_RenderUI(false);
			}

			if (CUltimateSlot::RECHARGE_INSIDE == dynamic_cast<CUltimateSlot*>(m_vecCurUI[i])->Get_UltimateType())
			{
				(m_vecCurUI[i])->Set_RenderUI(false);
			}
		}
	}
}

void CUIMgr::SetZeroForAccumulatedVariables()
{
	//보스
	m_iAccumulatedBreak = 0;
	m_iAccumulatedDamage = 0;
	m_iAccumulatedDeath = 0;
	m_dbAccumulatedTime = 0.f;

	//깃발
	m_iBlueTotalScore = 0;
	m_iRedTotalScore = 0;
	ZeroMemory(&m_tPlayer, sizeof(FLAG_RESULT));
	ZeroMemory(&m_tAlliance, sizeof(FLAG_RESULT));
	ZeroMemory(&m_tEnemy_1, sizeof(FLAG_RESULT));
	ZeroMemory(&m_tEnemy_2, sizeof(FLAG_RESULT));
	
	//런게임
	m_iAccumulatedRunGamePoints = 0;
}

_bool CUIMgr::SceneChangeToApostle()
{
	if (0.f >= m_vecCurUI.size())
		return false;

	for (_uint i = 0; i < m_vecCurUI.size(); ++i)
	{
		if (dynamic_cast<CMissionCard*>(m_vecCurUI[i]))
		{
			if (CMissionCard::AI == dynamic_cast<CMissionCard*>(m_vecCurUI[i])->Get_MissionCardType())
				return dynamic_cast<CMissionCard*>(m_vecCurUI[i])->Get_SceneChange();
		}
	}
	return false;

}

void CUIMgr::SetRender_NPCNoFuncObj(CNPCNoFuncObj::NPCNoFuncObjTYPE eType, CQuestMgr::NPCTYPE eNPCType, _bool bBool)
{
	list<CNPCNoFuncObj*>::iterator iter_begin = m_listNPCNoFuncObj.begin();
	list<CNPCNoFuncObj*>::iterator iter_end = m_listNPCNoFuncObj.end();

	for (; iter_begin != iter_end; ++iter_begin)
	{
		if (eType == (*iter_begin)->Get_Type() && eNPCType == (*iter_begin)->Get_NPCType())
		{
			(*iter_begin)->Set_Render(bBool);
			break;
		}
	}
}

//void CUIMgr::SetClear_NPCNoFuncObj(CNPCNoFuncObj::NPCNoFuncObjTYPE eType, CQuestMgr::NPCTYPE eNPCType)
//{
//	if (m_listNPCNoFuncObj.empty())
//		return;
//
//	list<CNPCNoFuncObj*>::iterator iter_begin = m_listNPCNoFuncObj.begin();
//	list<CNPCNoFuncObj*>::iterator iter_end = m_listNPCNoFuncObj.end();
//
//	for (; iter_begin != iter_end;++iter_begin)
//	{
//		if (eType == (*iter_begin)->Get_Type() && eNPCType == (*iter_begin)->Get_NPCType())
//		{
//			m_listNPCNoFuncObj.erase(iter_begin);
//			break;
//		}
//	}
//
//}

_bool CUIMgr::SceneChageToMatchingGame()
{
	list<Engine::CGameObject*> pList = Engine::Get_GameObjectlist(Engine::UI, L"NPCButton");
	if (pList.empty())
		return false;

	for (auto& list : pList)
	{
		if (dynamic_cast<CNPCButton*>(list))
		{
			if (CNPCButton::AGREE1 == dynamic_cast<CNPCButton*>(list)->Get_ButtonType())
				return dynamic_cast<CNPCButton*>(list)->Get_SceneChange();
		}
	}
	return false;
}

void CUIMgr::SetRender_QuestCheckBox(_bool bBool)
{
	if (0.f >= m_vecCurUI.size())
		return;

	////퀘스트창 + 글자
	//for (auto& pVec : m_vecCurUI)
	//{
	//	if (dynamic_cast<CQuestBackBar*>(pVec))
	//	{
	//		if (CQuestBackBar::QUEST == dynamic_cast<CQuestBackBar*>(pVec)->Get_QuestBackBarType())
	//		{
	//			pVec->Set_RenderUI(bBool);
	//			break;
	//		}
	//	}
	//}
	// 체크박스 네모
	for (auto& pVec : m_vecCurUI)
	{
		for (auto& pVec : m_vecCurUI)
		{
			if (dynamic_cast<CQuestCheckBox*>(pVec))
			{
				if (CQuestCheckBox::RECT == dynamic_cast<CQuestCheckBox*>(pVec)->Get_QuestCheckBox())
				{
					pVec->Set_RenderUI(bBool);
				}
			}
		}
	}
}

//void CUIMgr::SetRenderTrue_QuestCheck()
//{
//	vector<QUEST_INFO> tQuest = CQuestMgr::GetInstance()->Get_vecQuest();
//	_uint iIndex = 0;
//	for (_int i = 0; i < tQuest.size(); ++i)
//	{
//		if (tQuest[i].bClear)
//		{
//			iIndex = i;
//			///////////////////////////////////////////////
//			for (auto& pVec : m_vecCurUI)
//			{
//				if (dynamic_cast<CQuestCheckBox*>(pVec))
//				{
//					if (iIndex == _uint(dynamic_cast<CQuestCheckBox*>(pVec)->Get_Sequence()))
//					{
//						if (CQuestCheckBox::CHECK == dynamic_cast<CQuestCheckBox*>(pVec)->Get_QuestCheckBox())
//						{
//							pVec->Set_RenderUI(true);
//							break;
//						}
//					}
//				}
//			}
//			/////////////////////////////////////////////
//		}
//	}	
//}

void CUIMgr::SetRender_QuestCheck()
{
	vector<QUEST_INFO> tQuest = CQuestMgr::GetInstance()->Get_vecQuest();

	if (0.f >= tQuest.size())
	{
		//퀘스트 없을때

		for (auto& pVec : m_vecCurUI)
		{
			if (dynamic_cast<CQuestCheckBox*>(pVec))
			{
				if (CQuestCheckBox::CHECK == dynamic_cast<CQuestCheckBox*>(pVec)->Get_QuestCheckBox())
				{
					pVec->Set_RenderUI(false);
				}
			}
		}
	}
	else
	{
		//퀘스트 있을때
		for (_int i = 0; i < tQuest.size(); ++i)
		{
			for (auto& pVec : m_vecCurUI)
			{
				if (dynamic_cast<CQuestCheckBox*>(pVec))
				{
					if (i == _uint(dynamic_cast<CQuestCheckBox*>(pVec)->Get_Sequence()))
					{
						if (CQuestCheckBox::CHECK == dynamic_cast<CQuestCheckBox*>(pVec)->Get_QuestCheckBox())
						{
							pVec->Set_RenderUI(tQuest[i].bClear);
						}
					}
				}
			}
		}

	}
}

HRESULT CUIMgr::CreateSwapKarmaUI(LPDIRECT3DDEVICE9 pGraphicDev, CKarma::KARMAPOSITION eKarma)
{
	Engine::CGameObject* pGameObject = nullptr;

	if (CKarma::MAIN == eKarma)
	{
		//KarmaSlot -->2개만 생성
		for (_uint i = 0; i < 2; ++i)
		{
			//KarmaSlot
			pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORSWAP,
				CKarmaSlot::INREADYUI, 485.0f + (66.9f*i), 490.f, 67.3f, 66.2f); //418.1f
			if (pGameObject == nullptr)
				return E_FAIL;
			Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
			m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
			m_listKarmaSlot.emplace_back(dynamic_cast<CKarmaSlot*>(pGameObject));

		}
		//Karma  -->2개만 생성
		_uint iCnt = 0;
		for (_uint i = 0; i < PLAYERWEAPON_END; ++i)
		{
			if (i == 0 || i == m_ePlayerMainWeapon || i==3 ) //오브인경우도 제외(오브는 벨라토스 후에 얻는 무기니까.)
				continue;
			else
			{
				//Karma
				pGameObject = CKarma::Create(pGraphicDev, SELECTKARMATYPE_FORSWAP,
					eKarma, CKarma::KARMATYPE(i), 485.0f + (66.9f*iCnt), 490.f, 67.3f, 66.2f);

				if (pGameObject == nullptr)
					return E_FAIL;
				Engine::Add_GameObject(Engine::UI, L"Karma", pGameObject);
				m_listKarma.emplace_back(dynamic_cast<CKarma*>(pGameObject));

				++iCnt;
			}
		}
	}
	else if (CKarma::SUB == eKarma)
	{
		//KarmaSlot
		for (_uint i = 0; i < 2; ++i)
		{
			//KarmaSlot
			pGameObject = CKarmaSlot::Create(pGraphicDev, SELECTKARMATYPE_FORSWAP,
				CKarmaSlot::INREADYUI, 683.1f + (66.9f*i), 490.f, 67.3f, 66.2f);
			if (pGameObject == nullptr)
				return E_FAIL;
			Engine::Add_GameObject(Engine::UI, L"KarmaSlot", pGameObject);
			//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));
			m_listKarmaSlot.emplace_back(dynamic_cast<CKarmaSlot*>(pGameObject));
		}
		//Karma
		_uint iCnt = 0;
		for (_uint i = 0; i < PLAYERWEAPON_END; ++i)
		{
			if (i == 0 || i == m_ePlayerSubWeapon || i == 3) //오브인경우도 제외(오브는 벨라토스 후에 얻는 무기니까.)
				continue;
			else
			{
				//Karma
				pGameObject = CKarma::Create(pGraphicDev, SELECTKARMATYPE_FORSWAP,
					eKarma, CKarma::KARMATYPE(i), 683.1f + (66.9f*iCnt), 490.f, 67.3f, 66.2f);

				if (pGameObject == nullptr)
					return E_FAIL;
				Engine::Add_GameObject(Engine::UI, L"Karma", pGameObject);
				m_listKarma.emplace_back(dynamic_cast<CKarma*>(pGameObject));

				++iCnt;
			}
		}
	}
}

_bool CUIMgr::CheckMyKarmaPosition(CKarma::KARMAPOSITION eKarmaPosition, CKarma::KARMATYPE eKarmaType)
{
	if (CKarma::MAIN == eKarmaPosition)
	{
		//내가 누른애가 서브무기에 있는지 먼저 찾아야함.......
		for (auto& plist : m_listKarma)
		{
			if (SELECTKARMATYPE_FORBASIC == plist->Get_SelectKarmaType()
				&& CKarma::SUB == plist->Get_KarmaPositionType())
			{
				if (eKarmaType == plist->Get_KarmaType())
				{
					//서브 무기에 있다
					return true;
				}
				else
					//서브 무기에 없다
					return false;
			}
		}
	}
	else if (CKarma::SUB == eKarmaPosition)
	{
		//내가 누른애가 메인무기에 있는지 먼저 찾아야함.......
		for (auto& plist : m_listKarma)
		{
			if (SELECTKARMATYPE_FORBASIC == plist->Get_SelectKarmaType()
				&& CKarma::MAIN == plist->Get_KarmaPositionType())
			{
				if (eKarmaType == plist->Get_KarmaType())
				{
					//메인 무기에 있다
					return true;
				}
				else
					//메인 무기에 없다
					return false;
			}
		}
	}

	return false;
}

void CUIMgr::EraseSwapKarmaUI()
{
	if (m_listKarma.empty())
		return;

	list<CKarma*>::iterator iter_begin = m_listKarma.begin();
	list<CKarma*>::iterator iter_end = m_listKarma.end();

	for (; iter_begin != iter_end;)
	{
		if (SELECTKARMATYPE_FORSWAP == (*iter_begin)->Get_SelectKarmaType())
		{
			//if (eKarmaPosition == (*iter_begin)->Get_KarmaPositionType())
			//{
			(*iter_begin)->Set_Dead();
			iter_begin = m_listKarma.erase(iter_begin);
			//}
		}
		else
			++iter_begin;
	}

	if (m_listKarmaSlot.empty())
		return;

	for (auto& plistSlot : m_listKarmaSlot)
	{
		plistSlot->Set_Dead();
	}
	m_listKarmaSlot.clear();

}

void CUIMgr::UpdateSwapKarmaUI(CKarma::KARMAPOSITION eKarmaPosition, CKarma::KARMATYPE eKarmaType)
{
	//메인 쪽 스왑카르마인 경우, 메인에들어온 아이를 제외한 나머지카르마를 그려주면된다.
	//인자-> 메인카르마

	_uint iCnt = 0;
	for (auto& plist : m_listKarma)
	{
		if (SELECTKARMATYPE_FORSWAP == plist->Get_SelectKarmaType()
			&& eKarmaPosition == plist->Get_KarmaPositionType())
		{
			for (; iCnt < PLAYERWEAPON_END; )
			{
				if (iCnt == 0 || iCnt == PLAYERWEAPON(eKarmaType) || iCnt ==3 ) //오브인경우도 제외(오브는 벨라토스 후에 얻는 무기니까.)
				{
					++iCnt;
					continue;
				}
				else
				{
					plist->Set_KarmaType(iCnt++);
					break;
				}
			}
		}
	}
}

void CUIMgr::CheckEraseSwapPossible()
{
	if (m_listKarma.empty())
		return;

	_uint iTrueCnt = 0;
	for (auto& plist : m_listKarma)
	{
		if (plist->Get_EraseSwapUI())
			++iTrueCnt;
	}

	if (2 == iTrueCnt)
	{
		EraseSwapKarmaUI();
		m_bEraseSwapKarmaUIPossible = true;
	}
}

void CUIMgr::CreateFaceUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (0.f < m_vecCustomEyesBack.size())
		return;

	EraseHairUI();
	EraseClothesUI();
	m_eUIType = UITYPE_FACE_INCUSTOMIZE;

	Engine::CGameObject* pGameObject = nullptr;
	////////////////////////&& Eye &&/////////////////////////////////////////
	//테두리
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingSelectBox::Create(pGraphicDev, CCustomizingSelectBox::BASIC, i, 930.5f + (i*83.1f), 191.9f, 82.6f, 82.6f, 0.1f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingSelectBox", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomEyesBack.emplace_back(dynamic_cast<CCustomizingSelectBox*>(pGameObject));
	}
	//텍스처
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingEyeUI::Create(pGraphicDev, CCustomizingEyeUI::EYETYPE(i), 940.f + (i*83.0f), 200.6f, 64.f, 64.f, 0.05f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingEyeUI", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomEyes.emplace_back(dynamic_cast<CCustomizingEyeUI*>(pGameObject));
	}

	//Line UnderEye
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 181.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	//////////////////////////////////////////////////////////////////////////
	////////////////////////&& Brow &&////////////////////////////////////////
	//테두리
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingSelectBox::Create(pGraphicDev, CCustomizingSelectBox::BASIC, i, 930.5f + (i*83.1f), 321.9f, 82.6f, 82.6f, 0.1f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingSelectBox", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomBrowBack.emplace_back(dynamic_cast<CCustomizingSelectBox*>(pGameObject));
	}
	//텍스처
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingBrowUI::Create(pGraphicDev, CCustomizingBrowUI::BROWTYPE(i), 940.f + (i*83.0f), 330.6f, 64.f, 64.f, 0.05f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingBrowUI", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomBrow.emplace_back(dynamic_cast<CCustomizingBrowUI*>(pGameObject));
	}

	//Line UnderBrow
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 311.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	//////////////////////////////////////////////////////////////////////////
	////////////////////////&& BodyColor &&///////////////////////////////////
	//텍스처 
	pGameObject = m_pCustomizingColorBG = CCustomizingColorBG::Create(pGraphicDev, L"Body", 936.6f, 480.6f, 326.2f, 110.2f, 0.1f); //435.f, 147.f
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingColorBG", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용	

	//Line UnderBrow
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 461.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	//////////////////////////////////////////////////////////////////////////

	//현재착용중인거 표시.
	SetIsWearing_Face();
}

void CUIMgr::CreateHairUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (0.f < m_vecCustomFrontHairBack.size())
		return;

	EraseFaceUI();
	EraseClothesUI();
	m_eUIType = UITYPE_HAIR_INCUSTOMIZE;

	Engine::CGameObject* pGameObject = nullptr;
	//////////////////////////&& FrontHair &&////////////////////////////////////
	//테두리
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingSelectBox::Create(pGraphicDev, CCustomizingSelectBox::BASIC, i, 930.5f + (i*83.1f), 41.9f, 82.6f, 82.6f, 0.1f); //91.9f
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingSelectBox", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomFrontHairBack.emplace_back(dynamic_cast<CCustomizingSelectBox*>(pGameObject));
	}
	//텍스처
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingFrontHairUI::Create(pGraphicDev, CCustomizingFrontHairUI::FRONTHAIRTYPE(i), 940.f + (i*83.0f), 50.6f, 64.f, 64.f, 0.05f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingFrontHairUI", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomFrontHair.emplace_back(dynamic_cast<CCustomizingFrontHairUI*>(pGameObject));
	}

	//Line UnderEye
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 31.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////&& BackHair &&////////////////////////////////////
	//테두리
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingSelectBox::Create(pGraphicDev, CCustomizingSelectBox::BASIC, i, 930.5f + (i*83.1f), 171.9f, 82.6f, 82.6f, 0.1f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingSelectBox", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomBackHairBack.emplace_back(dynamic_cast<CCustomizingSelectBox*>(pGameObject));
	}
	//텍스처
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingBackHairUI::Create(pGraphicDev, CCustomizingBackHairUI::BACKHAIRTYPE(i), 940.f + (i*83.0f), 180.6f, 64.f, 64.f, 0.05f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingBackHairUI", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomBackHair.emplace_back(dynamic_cast<CCustomizingBackHairUI*>(pGameObject));
	}

	//Line UnderEye
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 161.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////&& SideHair &&////////////////////////////////////
	//테두리
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingSelectBox::Create(pGraphicDev, CCustomizingSelectBox::BASIC, i, 930.5f + (i*83.1f), 301.9f, 82.6f, 82.6f, 0.1f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingSelectBox", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomSideHairBack.emplace_back(dynamic_cast<CCustomizingSelectBox*>(pGameObject));
	}
	//텍스처
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingSideHairUI::Create(pGraphicDev, CCustomizingSideHairUI::SIDEHAIRTYPE(i), 940.f + (i*83.0f), 310.6f, 64.f, 64.f, 0.05f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingSideHairUI", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomSideHair.emplace_back(dynamic_cast<CCustomizingSideHairUI*>(pGameObject));
	}

	//Line UnderEye
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 291.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////&& TailHair &&////////////////////////////////////
	//테두리
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingSelectBox::Create(pGraphicDev, CCustomizingSelectBox::BASIC, i, 930.5f + (i*83.1f), 431.9f, 82.6f, 82.6f, 0.1f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingSelectBox", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomTailHairBack.emplace_back(dynamic_cast<CCustomizingSelectBox*>(pGameObject));
	}
	//텍스처
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingTailHairUI::Create(pGraphicDev, CCustomizingTailHairUI::TAILHAIRTYPE(i), 940.f + (i*83.0f), 440.6f, 64.f, 64.f, 0.05f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingTailHairUI", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomTailHair.emplace_back(dynamic_cast<CCustomizingTailHairUI*>(pGameObject));
	}

	//Line UnderEye
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 421.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	//////////////////////////////////////////////////////////////////////////////
	////////////////////////&& HairColor &&///////////////////////////////////
	//텍스처 
	pGameObject = m_pCustomizingColorBG = CCustomizingColorBG::Create(pGraphicDev, L"Hair", 936.6f, 570.f, 326.2f, 110.2f, 0.1f); //435.f, 147.f
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingColorBG", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용	

	//Line UnderBrow
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 551.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	//////////////////////////////////////////////////////////////////////////

	//현재착용중인거 표시.
	SetIsWearing_Hair();
}

void CUIMgr::CreateClothesUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (0.f < m_vecCustomTopBack.size())
		return;

	EraseFaceUI();
	EraseHairUI();
	m_eUIType = UITYPE_CLOTHES_INCUSTOMIZE;

	Engine::CGameObject* pGameObject = nullptr;
	////////////////////////&& Top &&/////////////////////////////////////////
	//테두리
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingSelectBox::Create(pGraphicDev, CCustomizingSelectBox::BASIC, i, 930.5f + (i*83.1f), 141.9f, 82.6f, 82.6f, 0.1f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingSelectBox", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomTopBack.emplace_back(dynamic_cast<CCustomizingSelectBox*>(pGameObject));
	}
	//텍스처
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingTopUI::Create(pGraphicDev, CCustomizingTopUI::TOPTYPE(i), 940.f + (i*83.0f), 150.6f, 64.f, 64.f, 0.05f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingTopUI", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomTop.emplace_back(dynamic_cast<CCustomizingTopUI*>(pGameObject));
	}

	//Line UnderTop
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 131.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	////////////////////////////////////////////////////////////////////////////
	////////////////////////&& Pants &&/////////////////////////////////////////
	//테두리
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingSelectBox::Create(pGraphicDev, CCustomizingSelectBox::BASIC, i, 930.5f + (i*83.1f), 271.9f, 82.6f, 82.6f, 0.1f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingSelectBox", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomPantsBack.emplace_back(dynamic_cast<CCustomizingSelectBox*>(pGameObject));
	}
	//텍스처
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingPantsUI::Create(pGraphicDev, CCustomizingPantsUI::PANTSTYPE(i), 940.f + (i*83.0f), 280.6f, 64.f, 64.f, 0.05f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingPantsUI", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomPants.emplace_back(dynamic_cast<CCustomizingPantsUI*>(pGameObject));
	}

	//Line UnderPants
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 261.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	///////////////////////////////////////////////////////////////////////////
	////////////////////////&& Foot &&/////////////////////////////////////////
	//테두리
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingSelectBox::Create(pGraphicDev, CCustomizingSelectBox::BASIC, i, 930.5f + (i*83.1f), 401.9f, 82.6f, 82.6f, 0.1f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingSelectBox", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomFootBack.emplace_back(dynamic_cast<CCustomizingSelectBox*>(pGameObject));
	}
	//텍스처
	for (_uint i = 0; i < 4; ++i)
	{
		pGameObject = CCustomizingFootUI::Create(pGraphicDev, CCustomizingFootUI::FOOTTYPE(i), 940.f + (i*83.0f), 410.6f, 64.f, 64.f, 0.05f);
		if (pGameObject == nullptr)
			return;
		Add_GameObject(Engine::UI, L"CustomizingFootUI", pGameObject);
		//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
		m_vecCustomFoot.emplace_back(dynamic_cast<CCustomizingFootUI*>(pGameObject));
	}

	//Line UnderFoot
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 391.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	//////////////////////////////////////////////////////////////////////////
	////////////////////////&& Hands &&/////////////////////////////////////////
	_uint iIndex = 0;
	//테두리
	pGameObject = m_pCustomHandsBack = CCustomizingSelectBox::Create(pGraphicDev, CCustomizingSelectBox::BASIC, iIndex, 930.5f, 531.9f, 82.6f, 82.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingSelectBox", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용

	//텍스처
	pGameObject = m_pCustomHands = CCustomizingHandsUI::Create(pGraphicDev, CCustomizingHandsUI::HANDSTYPE(iIndex), 940.f, 540.6f, 64.f, 64.f, 0.05f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingHandsUI", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용

	//Line UnderHands
	pGameObject = CCustomizingMenu::Create(pGraphicDev, CCustomizingMenu::LINE, 934.3f, 521.7f, 321.4f, 7.6f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingMenu", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용
	m_vecMenuLine.emplace_back(dynamic_cast<CCustomizingMenu*>(pGameObject));

	//////////////////////////////////////////////////////////////////////////
	////////////////////////&& Button &&//////////////////////////////////////
	pGameObject = m_pCustomButton = CCustomizingButton::Create(pGraphicDev, CCustomizingButton::CREATE1, 1012.5f, 640.2f, 188.4f, 46.8f, 0.1f); //314,78
	if (pGameObject == nullptr)
		return;
	Add_GameObject(Engine::UI, L"CustomizingButton", pGameObject);
	m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject)); //->자리이동용

	//////////////////////////////////////////////////////////////////////////

	//현재착용중인거 표시.
	SetIsWearing_Clothes();
}

void CUIMgr::EraseFaceUI()
{/////////////////////////////////////한꺼번에 나중에 clearpointui 에 넣어주기.

	m_eUIType = UITYPE_CUSTOMIZE; //글자 지워지게 할려구. 필요없어질지도.
	////Eye////
	//눈배경
	for (auto& pVec : m_vecCustomEyesBack)
		pVec->Set_Dead();
	m_vecCustomEyesBack.clear();
	//눈
	for (auto& pVec : m_vecCustomEyes)
		pVec->Set_Dead();
	m_vecCustomEyes.clear();

	////Brow////
	//눈썹배경
	for (auto& pVec : m_vecCustomBrowBack)
		pVec->Set_Dead();
	m_vecCustomBrowBack.clear();
	//눈
	for (auto& pVec : m_vecCustomBrow)
		pVec->Set_Dead();
	m_vecCustomBrow.clear();

	////몸색상////
	if (nullptr != m_pCustomizingColorBG)
	{
		m_pCustomizingColorBG->Set_Dead();
		m_pCustomizingColorBG = nullptr;
	}

	//공통
	//라인
	for (auto& pVec : m_vecMenuLine)
		pVec->Set_Dead();
	m_vecMenuLine.clear();

}

void CUIMgr::EraseHairUI()
{
	m_eUIType = UITYPE_CUSTOMIZE; //글자 지워지게 할려구. 필요없어질지도.

	////FrontHair////
	//앞머리배경
	for (auto& pVec : m_vecCustomFrontHairBack)
		pVec->Set_Dead();
	m_vecCustomFrontHairBack.clear();
	//앞머리
	for (auto& pVec : m_vecCustomFrontHair)
		pVec->Set_Dead();
	m_vecCustomFrontHair.clear();

	////BackHair////
	//뒷머리배경
	for (auto& pVec : m_vecCustomBackHairBack)
		pVec->Set_Dead();
	m_vecCustomBackHairBack.clear();
	//뒷머리
	for (auto& pVec : m_vecCustomBackHair)
		pVec->Set_Dead();
	m_vecCustomBackHair.clear();

	////SideHair////
	//옆머리배경
	for (auto& pVec : m_vecCustomSideHairBack)
		pVec->Set_Dead();
	m_vecCustomSideHairBack.clear();
	//옆머리
	for (auto& pVec : m_vecCustomSideHair)
		pVec->Set_Dead();
	m_vecCustomSideHair.clear();

	////TailHair////
	//꼬리머리배경
	for (auto& pVec : m_vecCustomTailHairBack)
		pVec->Set_Dead();
	m_vecCustomTailHairBack.clear();
	//꼬리머리
	for (auto& pVec : m_vecCustomTailHair)
		pVec->Set_Dead();
	m_vecCustomTailHair.clear();

	////헤어색상////
	if (nullptr != m_pCustomizingColorBG)
	{
		m_pCustomizingColorBG->Set_Dead();
		m_pCustomizingColorBG = nullptr;
	}

	//공통
	//라인
	for (auto& pVec : m_vecMenuLine)
		pVec->Set_Dead();
	m_vecMenuLine.clear();

}

void CUIMgr::EraseClothesUI()
{
	m_eUIType = UITYPE_CUSTOMIZE; //글자 지워지게 할려구. 필요없어질지도.							

	////Top////						 
	//상의 배경
	for (auto& pVec : m_vecCustomTopBack)
		pVec->Set_Dead();
	m_vecCustomTopBack.clear();
	//상의 
	for (auto& pVec : m_vecCustomTop)
		pVec->Set_Dead();
	m_vecCustomTop.clear();

	////Pants////						 
	//하의 배경
	for (auto& pVec : m_vecCustomPantsBack)
		pVec->Set_Dead();
	m_vecCustomPantsBack.clear();
	//하의 
	for (auto& pVec : m_vecCustomPants)
		pVec->Set_Dead();
	m_vecCustomPants.clear();

	////Foot////						 
	//신발 배경
	for (auto& pVec : m_vecCustomFootBack)
		pVec->Set_Dead();
	m_vecCustomFootBack.clear();
	//신발 
	for (auto& pVec : m_vecCustomFoot)
		pVec->Set_Dead();
	m_vecCustomFoot.clear();

	////Hands////						 
	//장갑 배경
	if (nullptr != m_pCustomHandsBack)
	{
		m_pCustomHandsBack->Set_Dead();
		m_pCustomHandsBack = nullptr;
	}

	//장갑 
	if (nullptr != m_pCustomHands)
	{
		m_pCustomHands->Set_Dead();
		m_pCustomHands = nullptr;
	}

	//공통
	//라인
	for (auto& pVec : m_vecMenuLine)
		pVec->Set_Dead();
	m_vecMenuLine.clear();

	//버튼
	if (nullptr != m_pCustomButton)
	{
		m_pCustomButton->Set_Dead();
		m_pCustomButton = nullptr;
	}

}

void CUIMgr::ClearAllCustomizingUI()
{
	m_vecCustomEyesBack.clear();
	m_vecCustomEyes.clear();

	m_vecCustomBrowBack.clear();
	m_vecCustomBrow.clear();

	m_pCustomizingColorBG = nullptr;

	m_vecCustomFrontHairBack.clear();
	m_vecCustomFrontHair.clear();

	m_vecCustomBackHairBack.clear();
	m_vecCustomBackHair.clear();

	m_vecCustomSideHairBack.clear();
	m_vecCustomSideHair.clear();

	m_vecCustomTailHairBack.clear();
	m_vecCustomTailHair.clear();


	m_vecCustomTopBack.clear();
	m_vecCustomTop.clear();

	m_vecCustomPantsBack.clear();
	m_vecCustomPants.clear();

	m_vecCustomFootBack.clear();
	m_vecCustomFoot.clear();

	m_pCustomHandsBack = nullptr;
	m_pCustomHands = nullptr;
	m_pCustomButton = nullptr;

	m_vecMenuLine.clear();
}

void CUIMgr::CheckIsWearing(wstring wstrParts, _uint iNum)
{
	if (L"Eyes" == wstrParts)
	{
		if (m_vecCustomEyesBack.empty())
			return;

		for (_uint i = 0; i < m_vecCustomEyesBack.size(); ++i)
		{
			if (iNum == i)
			{
				//얘가 입혀지는 아이가 되는거야
				m_vecCustomEyesBack[i]->Set_IsWearing(true);
			}
			else
				m_vecCustomEyesBack[i]->Set_IsWearing(false);
		}
	}
	else if (L"Brow" == wstrParts)
	{
		if (m_vecCustomBrowBack.empty())
			return;

		for (_uint i = 0; i < m_vecCustomBrowBack.size(); ++i)
		{
			if (iNum == i)
			{
				//얘가 입혀지는 아이가 되는거야
				m_vecCustomBrowBack[i]->Set_IsWearing(true);
			}
			else
				m_vecCustomBrowBack[i]->Set_IsWearing(false);
		}
	}
	else if (L"FrontHair" == wstrParts)
	{
		if (m_vecCustomFrontHairBack.empty())
			return;

		for (_uint i = 0; i < m_vecCustomFrontHairBack.size(); ++i)
		{
			if (iNum == i)
			{
				//얘가 입혀지는 아이가 되는거야
				m_vecCustomFrontHairBack[i]->Set_IsWearing(true);
			}
			else
				m_vecCustomFrontHairBack[i]->Set_IsWearing(false);
		}
	}
	else if (L"BackHair" == wstrParts)
	{
		if (m_vecCustomBackHairBack.empty())
			return;

		for (_uint i = 0; i < m_vecCustomBackHairBack.size(); ++i)
		{
			if (iNum == i)
			{
				//얘가 입혀지는 아이가 되는거야
				m_vecCustomBackHairBack[i]->Set_IsWearing(true);
			}
			else
				m_vecCustomBackHairBack[i]->Set_IsWearing(false);
		}
	}
	else if (L"SideHair" == wstrParts)
	{
		if (m_vecCustomSideHairBack.empty())
			return;

		for (_uint i = 0; i < m_vecCustomSideHairBack.size(); ++i)
		{
			if (iNum == i)
			{
				//얘가 입혀지는 아이가 되는거야
				m_vecCustomSideHairBack[i]->Set_IsWearing(true);
			}
			else
				m_vecCustomSideHairBack[i]->Set_IsWearing(false);
		}
	}
	else if (L"TailHair" == wstrParts)
	{
		if (m_vecCustomTailHairBack.empty())
			return;

		for (_uint i = 0; i < m_vecCustomTailHairBack.size(); ++i)
		{
			if (iNum == i)
			{
				//얘가 입혀지는 아이가 되는거야
				m_vecCustomTailHairBack[i]->Set_IsWearing(true);
			}
			else
				m_vecCustomTailHairBack[i]->Set_IsWearing(false);
		}
	}
	else if (L"Top" == wstrParts)
	{
		if (m_vecCustomTopBack.empty())
			return;

		for (_uint i = 0; i < m_vecCustomTopBack.size(); ++i)
		{
			if (iNum == i)
			{
				//얘가 입혀지는 아이가 되는거야
				m_vecCustomTopBack[i]->Set_IsWearing(true);
			}
			else
				m_vecCustomTopBack[i]->Set_IsWearing(false);
		}
	}
	else if (L"Pants" == wstrParts)
	{
		if (m_vecCustomPantsBack.empty())
			return;

		for (_uint i = 0; i < m_vecCustomPantsBack.size(); ++i)
		{
			if (iNum == i)
			{
				//얘가 입혀지는 아이가 되는거야
				m_vecCustomPantsBack[i]->Set_IsWearing(true);
			}
			else
				m_vecCustomPantsBack[i]->Set_IsWearing(false);
		}
	}
	else if (L"Foot" == wstrParts)
	{
		if (m_vecCustomFootBack.empty())
			return;

		for (_uint i = 0; i < m_vecCustomFootBack.size(); ++i)
		{
			if (iNum == i)
			{
				//얘가 입혀지는 아이가 되는거야
				m_vecCustomFootBack[i]->Set_IsWearing(true);
			}
			else
				m_vecCustomFootBack[i]->Set_IsWearing(false);
		}
	}
	else if (L"Hands" == wstrParts)
	{
		if (nullptr == m_pCustomHandsBack)
			return;

		if (0 == iNum)
		{
			//얘가 입혀지는 아이가 되는거야
			m_pCustomHandsBack->Set_IsWearing(true);
		}
		else
			m_pCustomHandsBack->Set_IsWearing(false);
	}
}

void CUIMgr::ChangeParts(wstring wstrParts, _uint iNum)
{
	////아직 작동안해서 주석처리
	CBody* pBody = dynamic_cast<CBody*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Body"));
	if (L"Eyes" == wstrParts)
	{
		pBody->Change_Eye(iNum);
	}
	else if (L"Brow" == wstrParts)
	{
		pBody->Change_Brow(iNum);
	}
	else if (L"FrontHair" == wstrParts)
	{
		pBody->Change_FrontHair(iNum);
	}
	else if (L"BackHair" == wstrParts)
	{
		pBody->Change_BackHair(iNum);
	}
	else if (L"SideHair" == wstrParts)
	{
		pBody->Change_SideHair(iNum);
	}
	else if (L"TailHair" == wstrParts)
	{
		pBody->Change_TailHair(iNum);
	}
	else if (L"Top" == wstrParts)
	{
		pBody->Change_Top(iNum);
	}
	else if (L"Pants" == wstrParts)
	{
		pBody->Change_Pants(iNum);
	}
	else if (L"Foot" == wstrParts)
	{
		pBody->Change_Foot(iNum);
	}
	else if (L"Hands" == wstrParts)
	{
		pBody->Change_Hands(iNum);
	}
}

void CUIMgr::SetIsWearing_Face()
{
	CBody* pBody = dynamic_cast<CBody*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Body"));

	_uint iEyeIndex = 0;

	//눈
	iEyeIndex = pBody->Get_iEyeNum();
	m_vecCustomEyesBack[iEyeIndex]->Set_IsWearing(true);

	//눈썹
	iEyeIndex = pBody->Get_iBrowNum();
	m_vecCustomBrowBack[iEyeIndex]->Set_IsWearing(true);

}

void CUIMgr::SetIsWearing_Hair()
{
	CBody* pBody = dynamic_cast<CBody*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Body"));

	_uint iHairIndex = 0;

	//앞머리
	iHairIndex = pBody->Get_FrontHairIndex();
	m_vecCustomFrontHairBack[iHairIndex]->Set_IsWearing(true);

	//뒷머리
	iHairIndex = pBody->Get_BackHairIndex();
	m_vecCustomBackHairBack[iHairIndex]->Set_IsWearing(true);

	//옆머리
	iHairIndex = pBody->Get_SideHairIndex();
	m_vecCustomSideHairBack[iHairIndex]->Set_IsWearing(true);

	//꼬리머리
	iHairIndex = pBody->Get_TailHairIndex();
	m_vecCustomTailHairBack[iHairIndex]->Set_IsWearing(true);

}

void CUIMgr::SetIsWearing_Clothes()
{
	CBody* pBody = dynamic_cast<CBody*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Body"));

	_uint iClothesIndex = 0;

	//상의
	iClothesIndex = pBody->Get_TopIndex();
	m_vecCustomTopBack[iClothesIndex]->Set_IsWearing(true);

	//하의
	iClothesIndex = pBody->Get_PantsIndex();
	m_vecCustomPantsBack[iClothesIndex]->Set_IsWearing(true);

	//신발
	iClothesIndex = pBody->Get_FootIndex();
	m_vecCustomFootBack[iClothesIndex]->Set_IsWearing(true);

	//장갑
	//iClothesIndex = pBody->Get_HandsIndex();
	m_pCustomHandsBack->Set_IsWearing(true);

}

void CUIMgr::ChangeColor(wstring wstrParts, _vec4 vColor)
{
	CBody* pBody = dynamic_cast<CBody*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Body"));

	if (L"Body" == wstrParts)
	{
		pBody->Change_BodyColor(vColor);
	}
	else if (L"Hair" == wstrParts)
	{
		pBody->Change_HairColor(vColor);
	}
}

void CUIMgr::SetAnimationForParts()
{
	CBody* pBody = dynamic_cast<CBody*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Body"));
	pBody->Set_AnimationNum(2);
}

void CUIMgr::TakeOffParts(wstring wstrParts)
{
	CBody* pBody = dynamic_cast<CBody*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"Body"));

	if (L"Pants" == wstrParts)
	{
		if (m_vecCustomPantsBack.empty())
			return;
		
		for (auto& pPantsBack : m_vecCustomPantsBack)
		{
			pPantsBack->Set_IsWearing(false);
		}

		pBody->Release_Pants();
	}
}

_bool CUIMgr::SceneChangeToCTF()
{
	if (0.f >= m_vecCurUI.size())
		return false;

	for (_uint i = 0; i < m_vecCurUI.size(); ++i)
	{
		if (dynamic_cast<CMissionCard*>(m_vecCurUI[i]))
		{
			if (CMissionCard::CTF == dynamic_cast<CMissionCard*>(m_vecCurUI[i])->Get_MissionCardType())
				return dynamic_cast<CMissionCard*>(m_vecCurUI[i])->Get_SceneChange();
		}
	}
	return false;
}

void CUIMgr::Set_FlagPlayersID(OBJID eObjID, _tchar* szID)
{
	switch (eObjID)
	{
	case OBJECT_PLAYER:
		lstrcpy(m_tPlayer.szName, szID);
		break;
	case OBJECT_ALLIANCE:
		lstrcpy(m_tAlliance.szName, szID);
		break;
	case OBJECT_ENEMY_1:
		lstrcpy(m_tEnemy_1.szName, szID);
		break;
	case OBJECT_ENEMY_2:
		lstrcpy(m_tEnemy_2.szName, szID);
		break;
	}
}

void CUIMgr::CheckFlagScore(LPDIRECT3DDEVICE9 pGraphicDev, OBJID eKiller, OBJID eDead)
{
	m_iScore = 30;

	////FLAG_DEATH Info
	FLAG_DEATH tFlagDeath;

	switch (eKiller)
	{
	case OBJECT_PLAYER:
		m_iBlueTotalScore += m_iScore;
		m_tPlayer.iKill += 1;
		CreateFlagScorePopUp(pGraphicDev, CFlagScorePopUp::BLUE);
		//FLAG_DEATH Info
		tFlagDeath.eKillerTeam = TEAM::TEAM_BLUE;
		lstrcpy(tFlagDeath.szKillerName, m_tPlayer.szName);
		break;
	case OBJECT_ALLIANCE:
		m_iBlueTotalScore += m_iScore;
		m_tAlliance.iKill += 1;
		CreateFlagScorePopUp(pGraphicDev, CFlagScorePopUp::BLUE);
		//FLAG_DEATH Info
		tFlagDeath.eKillerTeam = TEAM::TEAM_BLUE;
		lstrcpy(tFlagDeath.szKillerName, m_tAlliance.szName);
		break;
	case OBJECT_ENEMY_1:
		m_iRedTotalScore += m_iScore;
		m_tEnemy_1.iKill += 1;
		CreateFlagScorePopUp(pGraphicDev, CFlagScorePopUp::RED);
		//FLAG_DEATH Info
		tFlagDeath.eKillerTeam = TEAM::TEAM_RED;
		lstrcpy(tFlagDeath.szKillerName, m_tEnemy_1.szName);
		break;
	case OBJECT_ENEMY_2:
		m_iRedTotalScore += m_iScore;
		m_tEnemy_2.iKill += 1;
		CreateFlagScorePopUp(pGraphicDev, CFlagScorePopUp::RED);
		//FLAG_DEATH Info
		tFlagDeath.eKillerTeam = TEAM::TEAM_RED;
		lstrcpy(tFlagDeath.szKillerName, m_tEnemy_2.szName);
		break;
	}

	////누가누구죽였는지에 대한정보 여기시ㅓ 전달에정
	//ekiller id. edead id. ekiller team. edead team

	switch (eDead)
	{
	case OBJECT_PLAYER:
		tFlagDeath.eDeadTeam = TEAM::TEAM_BLUE;
		lstrcpy(tFlagDeath.szDeadName, m_tPlayer.szName);
		break;
	case OBJECT_ALLIANCE:
		tFlagDeath.eDeadTeam = TEAM::TEAM_BLUE;
		lstrcpy(tFlagDeath.szDeadName, m_tAlliance.szName);
		break;
	case OBJECT_ENEMY_1:
		tFlagDeath.eDeadTeam = TEAM::TEAM_RED;
		lstrcpy(tFlagDeath.szDeadName, m_tEnemy_1.szName);
		break;
	case OBJECT_ENEMY_2:
		tFlagDeath.eDeadTeam = TEAM::TEAM_RED;
		lstrcpy(tFlagDeath.szDeadName, m_tEnemy_2.szName);
		break;
	}

	//해당하는 UI 생성해주고
	Engine::CGameObject*	pGameObject = CFDBCreator::Create(pGraphicDev, tFlagDeath.eKillerTeam, tFlagDeath.szKillerName, tFlagDeath.szDeadName);
	if (nullptr == pGameObject)
		return;
	Engine::Add_GameObject(Engine::UI, L"FDBCreator", pGameObject);
	m_listFlagDeathInfo.push_front(dynamic_cast<CFDBCreator*>(pGameObject));

	//리스트를 업뎃하면서 UI의 포스를 정해준다
	SetDeathBarPos();
}

void CUIMgr::CheckFlagScore(OBJID eTaker)
{
	m_iScore = 100;

	switch (eTaker)
	{
	case OBJECT_PLAYER:
		m_iBlueTotalScore += m_iScore;
		m_tPlayer.iEarnedPoints += m_iScore;
		break;
	case OBJECT_ALLIANCE:
		m_iBlueTotalScore += m_iScore;
		m_tAlliance.iEarnedPoints += m_iScore;
		break;
	case OBJECT_ENEMY_1:
		m_iRedTotalScore += m_iScore;
		m_tEnemy_1.iEarnedPoints += m_iScore;
		break;
	case OBJECT_ENEMY_2:
		m_iRedTotalScore += m_iScore;
		m_tEnemy_2.iEarnedPoints += m_iScore;
		break;
	}


	//깃발 점령에 성공했다
	m_bGetFlag = true;
	//벡터 비워준다
	m_vecFlagBackBar.clear();
}

void CUIMgr::SetAccumulatedDamageForFlag(OBJID eOBJ, _uint iDamage)
{
	switch (eOBJ)
	{
	case OBJECT_PLAYER:
		m_tPlayer.iDamage += iDamage;
		break;
	case OBJECT_ALLIANCE:
		m_tAlliance.iDamage += iDamage;
		break;
	case OBJECT_ENEMY_1:
		m_tEnemy_1.iDamage += iDamage;
		break;
	case OBJECT_ENEMY_2:
		m_tEnemy_2.iDamage += iDamage;
		break;
	}
}

void CUIMgr::SetAccumulatedBreakForFlag(OBJID eOBJ, _uint iBreak)
{
	switch (eOBJ)
	{
	case OBJECT_PLAYER:
		m_tPlayer.iBreak += iBreak;
		break;
	case OBJECT_ALLIANCE:
		m_tAlliance.iBreak += iBreak;
		break;
	case OBJECT_ENEMY_1:
		m_tEnemy_1.iBreak += iBreak;
		break;
	case OBJECT_ENEMY_2:
		m_tEnemy_2.iBreak += iBreak;
		break;
	}
}

void CUIMgr::SetWeapon(OBJID eOBJ, _uint iMainWeapon, _uint iSubWeapon)
{
	switch (eOBJ)
	{
	case OBJECT_ALLIANCE:
		m_tAlliance.iMainWeapon = iMainWeapon;
		m_tAlliance.iSubWeapon = iSubWeapon;
		break;
	case OBJECT_ENEMY_1:
		m_tEnemy_1.iMainWeapon = iMainWeapon;
		m_tEnemy_1.iSubWeapon = iSubWeapon;
		break;
	case OBJECT_ENEMY_2:
		m_tEnemy_2.iMainWeapon = iMainWeapon;
		m_tEnemy_2.iSubWeapon = iSubWeapon;
		break;
	}
}

void CUIMgr::SetStartFlagFlash()
{
	if (0.f >= m_vecCurUI_Rc.size())
		return;

	for (_uint i = 0; i < m_vecCurUI_Rc.size(); ++i)
	{
		if (dynamic_cast<CFlagScoreFlash*>(m_vecCurUI_Rc[i]))
			dynamic_cast<CFlagScoreFlash*>(m_vecCurUI_Rc[i])->Set_Start(true);
	}
}

void CUIMgr::CreateFlagScorePopUp(LPDIRECT3DDEVICE9 pGraphicDev, CFlagScorePopUp::POPUPCOLOR eType)
{
	if (CFlagScorePopUp::BLUE == eType)
	{
		Engine::CGameObject* pGameObject = nullptr;

		pGameObject = CFlagScorePopUp::Create(pGraphicDev, CFlagScorePopUp::BG, CFlagScorePopUp::BLUE, 520.f, 87.f);
		if (pGameObject == nullptr)
			return;
		Engine::Add_GameObject(Engine::UI, L"FlagScorePopUp", pGameObject);

		pGameObject = CFlagScorePopUp::Create(pGraphicDev, CFlagScorePopUp::BATTLE, CFlagScorePopUp::BLUE, 530.f, 87.f);
		if (pGameObject == nullptr)
			return;
		Engine::Add_GameObject(Engine::UI, L"FlagScorePopUp", pGameObject);

	}
	else if (CFlagScorePopUp::RED == eType)
	{
		Engine::CGameObject* pGameObject = nullptr;

		pGameObject = CFlagScorePopUp::Create(pGraphicDev, CFlagScorePopUp::BG, CFlagScorePopUp::RED, 761.6F, 87.f);
		if (pGameObject == nullptr)
			return;
		Engine::Add_GameObject(Engine::UI, L"FlagScorePopUp", pGameObject);

		pGameObject = CFlagScorePopUp::Create(pGraphicDev, CFlagScorePopUp::BATTLE, CFlagScorePopUp::RED, 773.6f, 87.f);
		if (pGameObject == nullptr)
			return;
		Engine::Add_GameObject(Engine::UI, L"FlagScorePopUp", pGameObject);

	}

}

void CUIMgr::CreateRevivalPopUp(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CRevivalPopUp::Create(pGraphicDev, 1072.f, 240.5f, 162.f, 44.f); //190/f
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"RevivalPopUp", pGameObject);
}

void CUIMgr::CreateGetFlagSuccessPopUp(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CFlagNoFuncObj::Create(pGraphicDev, CFlagNoFuncObj::FLAGNOFUNCOBJ_GETFLAGTXT, true, 892.3f, 298.9f, 400.f, 55.f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"FlagNoFuncObj", pGameObject);

	pGameObject = CFlagNoFuncObj::Create(pGraphicDev, CFlagNoFuncObj::FLAGNOFUNCOBJ_FLAG, true, 1008.3f, 305.2f, 17.3f, 19.7f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"FlagNoFuncObj", pGameObject);

	//FlagCountDown
	pGameObject = CFlagCountDown::Create(pGraphicDev, CFlagCountDown::COUNTDOWN, 644.2f, 212.7f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"FlagCountDown", pGameObject);

	pGameObject = CFlagCountDown::Create(pGraphicDev, CFlagCountDown::TEXT, 642.f, 174.f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"FlagCountDown", pGameObject);

	EraseFlagPos();
}

void CUIMgr::CreateReadyFlagPopUp(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CFlagNoFuncObj::Create(pGraphicDev, CFlagNoFuncObj::FLAGNOFUNCOBJ_READYFLAGTXT, true, 892.3f, 298.9f, 400.f, 55.f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"FlagNoFuncObj", pGameObject);

	pGameObject = CFlagNoFuncObj::Create(pGraphicDev, CFlagNoFuncObj::FLAGNOFUNCOBJ_FLAG, true, 1008.3f, 305.2f, 17.3f, 19.7f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"FlagNoFuncObj", pGameObject);
}

void CUIMgr::CreateReCreateFlagPopUp(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CFlagNoFuncObj::Create(pGraphicDev, CFlagNoFuncObj::FLAGNOFUNCOBJ_RECREATEFLAGTXT, true, 892.3f, 298.9f, 400.f, 55.f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"FlagNoFuncObj", pGameObject);

	pGameObject = CFlagNoFuncObj::Create(pGraphicDev, CFlagNoFuncObj::FLAGNOFUNCOBJ_FLAG, true, 1008.3f, 305.2f, 17.3f, 19.7f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"FlagNoFuncObj", pGameObject);

}

void CUIMgr::CaptureingFlag(LPDIRECT3DDEVICE9 pGraphicDev, TEAM eTeamColor, _double dTime)
{
	//깃발점령중일 때는 return
	if (m_bGetFlag)
		return;

	//깃발점령 동시에 하는걸 막기위해 추가.
	if (!m_vecFlagBackBar.empty())
		return;


	Engine::CGameObject* pGameObject = nullptr;

	if (TEAM::TEAM_BLUE == eTeamColor)
	{
		pGameObject = CFlagBackBar::Create(pGraphicDev, CFlagBackBar::BLUE, (CFlagBackBar::GAGECOLOR)eTeamColor, dTime, 1024.5f, 184.5f, 210.f, 55.f);
		if (pGameObject == nullptr)
			return;
		Engine::Add_GameObject(Engine::UI, L"FlagBackBar", pGameObject);
		m_vecFlagBackBar.emplace_back(dynamic_cast<CFlagBackBar*>(pGameObject));

		pGameObject = CFlagBackBar::Create(pGraphicDev, CFlagBackBar::GAGE, (CFlagBackBar::GAGECOLOR)eTeamColor, dTime, 1052.5f, 219.5f, 162.f, 7.f);
		if (pGameObject == nullptr)
			return;
		Engine::Add_GameObject(Engine::UI, L"FlagBackBarGage", pGameObject);
		m_vecFlagBackBar.emplace_back(dynamic_cast<CFlagBackBar*>(pGameObject));
	}
	else if (TEAM::TEAM_RED == eTeamColor)
	{
		pGameObject = CFlagBackBar::Create(pGraphicDev, CFlagBackBar::RED, (CFlagBackBar::GAGECOLOR)eTeamColor, dTime, 1024.5f, 184.5f, 210.f, 55.f);
		if (pGameObject == nullptr)
			return;
		Engine::Add_GameObject(Engine::UI, L"FlagBackBar", pGameObject);
		m_vecFlagBackBar.emplace_back(dynamic_cast<CFlagBackBar*>(pGameObject));

		pGameObject = CFlagBackBar::Create(pGraphicDev, CFlagBackBar::GAGE, (CFlagBackBar::GAGECOLOR)eTeamColor, dTime, 1052.5f, 219.5f, 162.f, 7.f);
		if (pGameObject == nullptr)
			return;
		Engine::Add_GameObject(Engine::UI, L"FlagBackBarGage", pGameObject);
		m_vecFlagBackBar.emplace_back(dynamic_cast<CFlagBackBar*>(pGameObject));
	}
}

void CUIMgr::CaptureFlagFailed()
{
	if (0 >= m_vecFlagBackBar.size())
		return;


	for (int i = 0; i < m_vecFlagBackBar.size(); ++i)
	{
		m_vecFlagBackBar[i]->Set_Dead();
	}
	m_vecFlagBackBar.clear();

}

//void CUIMgr::Clear_vecFlagBackBar()
//{
//	m_vecFlagBackBar.clear();
//}

void CUIMgr::CheckFlagPos(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform * pTargetTransform)
{
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CFlagPos::Create(pGraphicDev, CFlagPos::BG, pTargetTransform);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"FlagPos", pGameObject);
	m_vecFlagPos.emplace_back(dynamic_cast<CFlagPos*>(pGameObject));

	pGameObject = CFlagPos::Create(pGraphicDev, CFlagPos::FLAG, pTargetTransform);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"FlagPos", pGameObject);
	m_vecFlagPos.emplace_back(dynamic_cast<CFlagPos*>(pGameObject));

}

void CUIMgr::EraseFlagPos()
{
	if (0 >= m_vecFlagPos.size())
		return;

	for (auto& pFlagPos : m_vecFlagPos)
	{
		pFlagPos->Set_Dead();
	}
	m_vecFlagPos.clear();
}

void CUIMgr::ClearvecFlagBackBar()
{
	m_vecFlagBackBar.clear();
}

void CUIMgr::SetDeathBarPos()
{
	if (m_listFlagDeathInfo.empty())
		return;

	list<CFDBCreator*>::iterator iter_begin = m_listFlagDeathInfo.begin();
	list<CFDBCreator*>::iterator iter_end = m_listFlagDeathInfo.end();

	_uint iCnt = 0;
	for (; iter_begin != iter_end; ++iter_begin)
	{
		(*iter_begin)->Set_ListIndex(iCnt);
		++iCnt;
	}
}

void CUIMgr::DeathBarTEST(LPDIRECT3DDEVICE9 pGraphicDev)
{
	FLAG_DEATH tFlagDeath;
	tFlagDeath.eDeadTeam = TEAM::TEAM_BLUE;
	tFlagDeath.eKillerTeam = TEAM::TEAM_RED;
	lstrcpy(tFlagDeath.szDeadName, L"1죽음");
	lstrcpy(tFlagDeath.szKillerName, L"1Killer_r");


	Engine::CGameObject*	pGameObject = CFDBCreator::Create(pGraphicDev, tFlagDeath.eKillerTeam, tFlagDeath.szKillerName, tFlagDeath.szDeadName);
	if (nullptr == pGameObject)
		return;
	Engine::Add_GameObject(Engine::UI, L"FDBCreator", pGameObject);
	m_listFlagDeathInfo.push_front(dynamic_cast<CFDBCreator*>(pGameObject));

	//리스트를 업뎃하면서 UI의 포스를 정해준다
	SetDeathBarPos();
}

void CUIMgr::AddDeathBarTEST(LPDIRECT3DDEVICE9 pGraphicDev)
{
	FLAG_DEATH tFlagDeath;
	tFlagDeath.eDeadTeam = TEAM::TEAM_RED;
	tFlagDeath.eKillerTeam = TEAM::TEAM_BLUE;
	lstrcpy(tFlagDeath.szDeadName, L"2죽음");
	lstrcpy(tFlagDeath.szKillerName, L"2Killer_b");


	Engine::CGameObject*	pGameObject = CFDBCreator::Create(pGraphicDev, tFlagDeath.eKillerTeam, tFlagDeath.szKillerName, tFlagDeath.szDeadName);
	if (nullptr == pGameObject)
		return;
	Engine::Add_GameObject(Engine::UI, L"FDBCreator", pGameObject);
	m_listFlagDeathInfo.push_front(dynamic_cast<CFDBCreator*>(pGameObject));

	//리스트를 업뎃하면서 UI의 포스를 정해준다
	SetDeathBarPos();

}

void CUIMgr::EraseFlagDeathInfoList()
{
	//뒤에꺼 지움. 항상 뒤에부터 지워지기때문에.
	m_listFlagDeathInfo.pop_back();
}

void CUIMgr::SetRender_SpaceBar(_bool bBool)
{
	//for (auto& pVec : m_vecCurUI)
	//{
	//	if (dynamic_cast<CFlagNoFuncObj*>(pVec))
	//	{
	//		if(dynamic_cast<CFlagNoFuncObj*>(pVec))
	//	}
	//}
	if (nullptr == m_pFlagSpaceTXT)
		return;

	m_pFlagSpaceTXT->Set_RenderUI(bBool);
}

void CUIMgr::TurnCheck(CMatchingMgr::MATCHINGTURN eType)
{
	for (_uint i = 0; i < m_vecCurUI.size(); ++i)
	{
		if (dynamic_cast<CMatchingTurn*>(m_vecCurUI[i]))
		{
			if (eType == dynamic_cast<CMatchingTurn*>(m_vecCurUI[i])->Get_Turn())
			{
				m_vecCurUI[i]->Set_RenderUI(true);
				break;
			}
		}
	}
}

void CUIMgr::EraseMatchingButton(/*CMatchingButton::BUTTONTYPE eType*/)
{
	list<Engine::CGameObject*> listButton = Engine::Get_GameObjectlist(Engine::UI, L"MatchingButton");
	list<Engine::CGameObject*>::iterator iter_begin = listButton.begin();
	list<Engine::CGameObject*>::iterator iter_end = listButton.end();

	for (; iter_begin != iter_end; ++iter_begin)
	{
		dynamic_cast<CBaseObject*>(*iter_begin)->Set_Dead();
	}
}

_bool CUIMgr::CheckClickEXITButton()
{
	list<Engine::CGameObject*> listButton = Engine::Get_GameObjectlist(Engine::UI, L"MatchingButton");
	if (listButton.empty())
		return false;

	list<Engine::CGameObject*>::iterator iter_begin = listButton.begin();
	list<Engine::CGameObject*>::iterator iter_end = listButton.end();

	for (; iter_begin != iter_end; ++iter_begin)
	{
		if (CMatchingButton::EXIT1 == dynamic_cast<CMatchingButton*>(*iter_begin)->Get_ButtonType())
		{
			return dynamic_cast<CMatchingButton*>(*iter_begin)->Get_GoLobby();
		}
	}

	return false;
}

void CUIMgr::CreateRunPlayer_RevivalUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	ChangeMouseType(CMouse::MOUSE_PICKING);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CRunCountDown::Create(pGraphicDev, CRunCountDown::RUNCOUNTDOWNTYPE_REVIVAL, 605.f, 290.f, 75.f, 97.5f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"RunCountDownRevival", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CRunButton::Create(pGraphicDev, CRunButton::START1, 331.0f, 411.6f, 251.2f, 62.4f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"RunButton", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

	pGameObject = CRunButton::Create(pGraphicDev, CRunButton::EXIT1, 687.0f, 411.6f, 251.2f, 62.4f, 0.1f);
	if (pGameObject == nullptr)
		return;
	Engine::Add_GameObject(Engine::UI, L"RunButton", pGameObject);
	//m_vecCurUI.emplace_back(dynamic_cast<CUIObject*>(pGameObject));

}

void CUIMgr::EraseRunButton()
{
	list<Engine::CGameObject*> listButton = Engine::Get_GameObjectlist(Engine::UI, L"RunButton");
	if (listButton.empty())
		return;

	list<Engine::CGameObject*>::iterator iter_begin = listButton.begin();
	list<Engine::CGameObject*>::iterator iter_end = listButton.end();

	for (; iter_begin != iter_end; ++iter_begin)
	{
		dynamic_cast<CBaseObject*>(*iter_begin)->Set_Dead();
	}

}

void CUIMgr::EraseRunCountDown()
{
	list<Engine::CGameObject*> List = Engine::Get_GameObjectlist(Engine::UI, L"RunCountDownRevival");
	if (List.empty())
		return;

	list<Engine::CGameObject*>::iterator iter_begin = List.begin();
	list<Engine::CGameObject*>::iterator iter_end = List.end();

	for (; iter_begin != iter_end; ++iter_begin)
	{
		dynamic_cast<CBaseObject*>(*iter_begin)->Set_Dead();
	}

}

void CUIMgr::SetTimeOver_RunCountDown()
{
	list<Engine::CGameObject*> List = Engine::Get_GameObjectlist(Engine::UI, L"RunCountDownRevival");
	if (List.empty())
		return;

	list<Engine::CGameObject*>::iterator iter_begin = List.begin();
	list<Engine::CGameObject*>::iterator iter_end = List.end();

	for (; iter_begin != iter_end; ++iter_begin)
	{
		dynamic_cast<CRunCountDown*>(*iter_begin)->Set_TimeOver(true);
	}
}

void CUIMgr::SetAccumulatedRunGamePoints(_uint iPoints)
{
	m_iAccumulatedRunGamePoints += iPoints;
}

void CUIMgr::CreateMouse(LPDIRECT3DDEVICE9 pGraphicDev, CMouse::MOUSETYPE eMouseType)
{
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = m_pMouse = CMouse::Create(pGraphicDev, eMouseType);
	if (nullptr == m_pMouse)
		return;
	Engine::Add_GameObject(Engine::UI, L"Mouse", pGameObject);
}

void CUIMgr::EraseMouse()
{
	m_pMouse = nullptr;
}

void CUIMgr::StageTimeCheck()
{
	//런게임 시간은 런게임 클래스안에서 ! 
	//나머지는 이걸로 시간 계산!


	if (!m_bStageTimeStop) //각자 스테이지 안에서 게임끝났을때 얘를 트루로 해줌.
	{
		if (m_bStageTimeStart)
		{
			m_dAccumulatedTime = g_dAccumulatedTime;
			m_bStageTimeStart = false;
		}

		if (m_bTimeCheck)
			m_dStageTime = (g_dAccumulatedTime - m_dAccumulatedTime);
	}
}

void CUIMgr::UI_OnCheck()
{
	if (UITYPE_MAIN == m_eUIType
		|| UITYPE_BOSS == m_eUIType
		|| UITYPE_CARTEL == m_eUIType
		|| UITYPE_SHOOTING == m_eUIType
		|| UITYPE_RUN == m_eUIType)
	{
		m_bUI_Working = false; // 플레이어 움직임
		//CCameraMgr::GetInstance()->Set_MouseFix(true);

	}
	else if (UITYPE_MISSION == m_eUIType || UITYPE_READY == m_eUIType
		|| UITYPE_RESULT_Boss == m_eUIType || UITYPE_RESULT_Cartel == m_eUIType
		|| UITYPE_RESULT_Shooting == m_eUIType || UITYPE_RESULT_Run == m_eUIType
		|| UITYPE_NPC == m_eUIType)
	{
		m_bUI_Working = true; // 플레이어 안움직임
		m_pMouse->Change_MouseType(CMouse::MOUSE_PICKING);
		CCameraMgr::GetInstance()->Set_MouseFix(false);
	}
}

void CUIMgr::ChangeRenderState(const wstring & _wstrTag, _bool bBool)
{
	if (!lstrcmp(L"NoFuncObj", _wstrTag.c_str()))
	{
		if (0 >= m_vecNoFuncObj.size())
			return;

		for (_uint i = 0; i < m_vecNoFuncObj.size(); ++i)
		{
			if (CNoFuncObj::NOFUNCOBJ_MAPTXT == m_vecNoFuncObj[i]->Get_NoFuncObjType())
			{
				m_vecNoFuncObj[i]->Set_RenderUI(bBool);
				break;
			}
		}
	}
	else if (!lstrcmp(L"FlagNoFuncObj", _wstrTag.c_str()))
	{
		if (0 >= m_vecFlagNoFuncObj.size())
			return;

		for (_uint i = 0; i < m_vecFlagNoFuncObj.size(); ++i)
		{
			m_vecFlagNoFuncObj[i]->Set_RenderUI(bBool);
		}
	}

}

void CUIMgr::KeyInput(const _double & dTimeDelta)
{
	if (Engine::KeyDown(DIK_INSERT))
	{
		m_iNextIndex = m_iCurIndex + 1;
		m_iCurIndex = m_iNextIndex;

		if (m_vecCurUI.size() <= m_iNextIndex)
		{
			m_iNextIndex = 0;
			m_iCurIndex = 0;
		}

	}

	if (!dynamic_cast<CUIObject*>(m_vecCurUI[m_iNextIndex]))
		return;

	Engine::CScreenTex* pBuffer = dynamic_cast<Engine::CScreenTex*>(m_vecCurUI[m_iNextIndex]->Get_Component(Engine::BUFFER, Engine::ID_STATIC));

	_float fSpeed = 10.f;

	if (Engine::KeyPressing(DIK_LEFT)) //X 감소
	{
		dynamic_cast<Engine::CScreenTex*>(pBuffer)->VertexCustomizeTest(dTimeDelta, -fSpeed, Engine::CScreenTex::POSX);
	}
	else if (Engine::KeyPressing(DIK_RIGHT)) //X 증가
	{
		dynamic_cast<Engine::CScreenTex*>(pBuffer)->VertexCustomizeTest(dTimeDelta, fSpeed, Engine::CScreenTex::POSX);
	}
	else if (Engine::KeyPressing(DIK_UP)) //Y 감소 ->좌표반대
	{
		dynamic_cast<Engine::CScreenTex*>(pBuffer)->VertexCustomizeTest(dTimeDelta, -fSpeed, Engine::CScreenTex::POSY);
	}
	else if (Engine::KeyPressing(DIK_DOWN)) //Y 증가
	{
		dynamic_cast<Engine::CScreenTex*>(pBuffer)->VertexCustomizeTest(dTimeDelta, fSpeed, Engine::CScreenTex::POSY);
	}
	else if (Engine::KeyPressing(DIK_1)) //X 사이즈 증가
	{
		dynamic_cast<Engine::CScreenTex*>(pBuffer)->VertexCustomizeTest(dTimeDelta, fSpeed, Engine::CScreenTex::SIZEX);
	}
	else if (Engine::KeyPressing(DIK_2)) //X 사이즈 감소
	{
		dynamic_cast<Engine::CScreenTex*>(pBuffer)->VertexCustomizeTest(dTimeDelta, -fSpeed, Engine::CScreenTex::SIZEX);
	}
	else if (Engine::KeyPressing(DIK_3)) //Y 사이즈 증가
	{
		dynamic_cast<Engine::CScreenTex*>(pBuffer)->VertexCustomizeTest(dTimeDelta, fSpeed, Engine::CScreenTex::SIZEY);
	}
	else if (Engine::KeyPressing(DIK_4)) //Y 사이즈 감소
	{
		dynamic_cast<Engine::CScreenTex*>(pBuffer)->VertexCustomizeTest(dTimeDelta, -fSpeed, Engine::CScreenTex::SIZEY);
	}


	////위치테스트-> 희정
	_vec3 vPos = dynamic_cast<Engine::CScreenTex*>(pBuffer)->Get_vStartPos();
	_vec2 vSize = dynamic_cast<Engine::CScreenTex*>(pBuffer)->Get_vSize();

	cout << m_iNextIndex << '\t' << vPos.x << '\t' << vPos.y << '\t' << vSize.x << '\t' << vSize.y << endl;
}
