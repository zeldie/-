#pragma once

#ifndef __Uimgr_h__
#define __Uimgr_h__

#include "DamageBox.h"
#include "Skill.h"
#include "FlagScorePopUp.h"
#include "FlagBackBar.h"
#include "FlagPos.h"
#include "Mouse.h"
#include "MatchingMgr.h"
#include "MatchingButton.h"
#include "NPCNoFuncObj.h"
#include "Karma.h"

class CUIObject;
class CNormalObject;
class CSkillSlot;
class CNoFuncObj;
class CFDBCreator;
class CFlagNoFuncObj;
class CUltimateRecharge;
class CKarmaSlot;

//////&& 커스터 마이징
class CCustomizingEyeUI;
class CCustomizingMenu;
class CCustomizingSelectBox;
class CCustomizingBrowUI;
class CCustomizingColorBG;
class CCustomizingFrontHairUI;
class CCustomizingBackHairUI;
class CCustomizingSideHairUI;
class CCustomizingTailHairUI;
class CCustomizingTopUI;
class CCustomizingPantsUI;
class CCustomizingFootUI;
class CCustomizingHandsUI;
class CCustomizingButton;
//////&&

class CUIMgr
{
	DECLARE_SINGLETON(CUIMgr)

public:
	enum UITYPE {UITYPE_BOSS, UITYPE_CARTEL,
		UITYPE_SHOOTING, UITYPE_MATCHING, UITYPE_PUZZLE, UITYPE_RUN,
		UITYPE_MAIN, UITYPE_NPC,  
		UITYPE_MISSION, UITYPE_READY, 
		UITYPE_RESULT_Boss , UITYPE_RESULT_Cartel, UITYPE_RESULT_Shooting, UITYPE_RESULT_Run, 
		UITYPE_CUSTOMIZE, UITYPE_FACE_INCUSTOMIZE, UITYPE_HAIR_INCUSTOMIZE, UITYPE_CLOTHES_INCUSTOMIZE, UITYPE_END};

public:
	enum PLAYERWEAPON{ COMMON, DUALSWORD, TWOHANDSWORD, ORB, LONGBOW, PLAYERWEAPON_END };

private:
	explicit CUIMgr();
	virtual ~CUIMgr();

public:
	//Getter
	_double			Get_StageTime() { return m_dStageTime; }
	_bool			Get_UI_Working() { return m_bUI_Working; }
	_bool			Get_SceneChange(); // : 로비->벨라토스로
	UITYPE&			Get_UIType() { return m_eUIType; }
	_uint			Get_AccumulatedDeath() { return m_iAccumulatedDeath; } //보스전)
	_uint			Get_AccumulatedBreak() { return m_iAccumulatedBreak; } //보스전)
	_uint			Get_AccumulatedDamage() { return m_iAccumulatedDamage; } //보스전)
	_double			Get_AccumulatedTime() { return m_dbAccumulatedTime; } //보스전)
	_uint			Get_BlueScore() { return m_iBlueTotalScore; } //깃발전)
	_uint			Get_RedScore() { return m_iRedTotalScore; } //깃발전)
	_bool			Get_CartelFinish() { return m_bCartelFinish; } //깃발전)
	FLAG_RESULT&	Get_FlagResultInfo(OBJID eOBJ); //깃발전)
	_bool			Get_MissionFlagLock() { return m_bMissionFlagLock; }
	_bool			Get_MissionApostleLock() { return m_bMissionApostleLock; }
	_bool			Get_MissionBossLock() { return m_bMissionBossLock; }
	_bool			Get_ChangeScene() { return m_bChangeScene; } //다음 씬으로 넘어가게 하기 위한 변수를 받음(인균)
	_bool			Get_CreateBossResultUI() { return m_bCreateBossResultUI; }
	_bool			Get_CreateShootingResultUI() { return m_bCreateShootingResultUI; }
	_bool			Get_RunGameStart() { return m_bRunGameStart; }
	_bool			Get_RunGameTimeStop() { return m_bRunGameTimeStop; }
	
	_uint			Get_PlayerMainWeaponType() { return m_ePlayerMainWeapon; }
	_uint			Get_PlayerSubWeaponType() { return m_ePlayerSubWeapon; }
	_bool			Get_EraseSwapKarmaUIPossible() { return m_bEraseSwapKarmaUIPossible; }
	_bool			Get_GetFlag() { return m_bGetFlag; }
	_bool			Get_ReCreateFlag() { return m_bReCreateFlag; }
	_bool			Get_FinishCustomize() { return m_bFinishCustomize; }

	//Setter
	void		Set_CartelFinish(_bool bBool=true) { m_bCartelFinish = bBool; }
	void		Set_ZeroStageTime();
	void		Set_UIType(UITYPE eType) { m_eUIType = eType; }
	void		Set_MissionFlagLock(_bool bBool = true) { m_bMissionFlagLock = bBool; }
	void		Set_MissionApostleLock(_bool bBool = true) { m_bMissionApostleLock = bBool; }
	void		Set_MissionBossLock(_bool bBool = true) { m_bMissionBossLock = bBool; }
	void		ChangeMouseType(CMouse::MOUSETYPE eMouseType) { m_pMouse->Change_MouseType(eMouseType); } //마우스타입바꾸는 함수
	void		Set_ChangeScene(_bool bChangeScene) { m_bChangeScene = bChangeScene; } //인균
	void		Set_CreateBossResultUI(_bool bBool = true) { m_bCreateBossResultUI = bBool; }
	void		Set_CreateShootingResultUI(_bool bBool=true) { m_bCreateShootingResultUI = bBool; }
	void		Set_StageTimeStart() { m_bStageTimeStart = true; m_bTimeCheck = true; }
	void		Set_StageTimeStop() { m_bStageTimeStop = true; }
	void		Set_RunGameStart(_bool bBool) { m_bRunGameStart = bBool; }
	void		Set_RunGameTimeStop(_bool bBool) { m_bRunGameTimeStop = bBool; }
	void		Set_PlayerMainWeapon(_uint iWeapon) { m_ePlayerMainWeapon = (PLAYERWEAPON)iWeapon; }
	void		Set_PlayerSubWeapon(_uint iWeapon) {m_ePlayerSubWeapon = (PLAYERWEAPON)iWeapon;}
	void		Set_EraseSwapKarmaUIPossible(_bool bBool) { m_bEraseSwapKarmaUIPossible = bBool; }
	void		Set_GetFlag(_bool bBool) { m_bGetFlag = bBool; }
	void		Set_ReCreateFlag(_bool bBool) { m_bReCreateFlag = bBool; }
	void		Set_FinishCustomize() { m_bFinishCustomize = true; }

public:
	//vecCurUI에 add 하기 위한 함수
	void		AddCurUI(Engine::CGameObject* pGameObject);

	//vecNPCNOFuncObj에 add 하기 위한 함수
	void		AddvecNPCNoFuncObj(Engine::CGameObject* pGameObject);

	//UI 중 플레이어정보에 해당하는 아이들 생성하는 함수 -->너무길어서 따로 만듬
	HRESULT		CreatePlayerInfoUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer* pLayer);

	//필요한 유아이를 따로 생성할 함수
	HRESULT		CreateBattleUI(LPDIRECT3DDEVICE9 pGraphicDev,/* Engine::CLayer* pLayer,*/ UITYPE eUIType); // Boss
	HRESULT		CreateCartelUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer* pLayer); // 깃발전 
	HRESULT		CreateMainUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer* pLayer);
	HRESULT		CreateMissionUI(LPDIRECT3DDEVICE9 pGraphicDev/*, Engine::CLayer* pLayer*/);
	HRESULT		CreateReadyUI(LPDIRECT3DDEVICE9 pGraphicDev/*, Engine::CLayer* pLayer*/);
	HRESULT		CreateResultUI_Boss(LPDIRECT3DDEVICE9 pGraphicDev/*, Engine::CLayer* pLayer*/);
	HRESULT		CreateResultUI_Cartel(LPDIRECT3DDEVICE9 pGraphicDev/*, Engine::CLayer* pLayer*/);
	HRESULT		CreateResultUI_ShootingStage(LPDIRECT3DDEVICE9 pGraphicDev/*, Engine::CLayer* pLayer*/);
	HRESULT		CreateResultUI_Run(LPDIRECT3DDEVICE9 pGraphicDev/*, Engine::CLayer* pLayer*/);
	HRESULT		CreateMatchingUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer* pLayer);
	HRESULT		CreateNPCMissionUI(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT		CreateRunUI(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT		CreateCustomizeUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer* pLayer);
	
	//유아이 지워줄 함수(set dead)
	void		ClearUI();
	void		ClearPointerUI();

	//skillslot 쿨다운 시작 함수
	void		SkillCoolDownCheck(Engine::KEYGUIDE KeyType);

	//Skill 쿨다운 중인지 구분하는 함수
	_bool		CheckSkillCoolDowning(Engine::KEYGUIDE KeyType); 
	
	//쿨다운 중일때 같은 키 눌리면 "재사용 대기중" 글씨 뜨게 하는 함수
	void		OnCoolDown();

	//쓸수있는 스킬만 표시해주게
	void		UseableSkillCheck(Engine::KEYGUIDE KeyType); //인자로 받는 키만 색깔이 들어오고 나머지는 안들어옴
	void		UseableSkillCheck_All(_bool _bBool);	// 모두 색깔이 들어오거나, 안들어오거나.

	//Tab키 눌렀을 시 무기바뀌면서 스킬이 바뀜
	void		SwitchSkill(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrTexture, CSkill::SKILLTYPE eType);

	//DamageFont 생성 함수
	void		CreateDamageBox(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTargetTransform, CDamageBox::DAMAGEBOXTYPE eType, _uint iDamage);

	//UI->Tired 생성 & 소멸 함수
	void		CreateTiredUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTargetTransform);
	void		EraseTiredUI();

	//"마나 부족" 문구 생성& 소멸 함수
	void		CreateDeficientMP();
	
	// Q스킬 관련함수
	void		Set_QSkillEnd();

	//보스 결과창 -> 변수들 누적시킬 함수
	void		SetAccumulatedDeath(_uint iDeath) { m_iAccumulatedDeath += iDeath; }
	void		SetAccumulatedDamage(_uint iDamage) { m_iAccumulatedDamage += iDamage; }
	void		SetAccumulatedBreak(_uint iBreak) { m_iAccumulatedBreak += iBreak; }
	void		SetAccumulatedTime(_double dbTime) { m_dbAccumulatedTime = dbTime; }
	
	//보스 결과창 변수 0으로 초기화하는 함수
	void		SetZeroForAccumulatedVariables();

	//돼지 천사로 넘어가게 도와주는 함수
	_bool		SceneChangeToApostle();

	//NPC 관련 UI
	void		SetRender_NPCNoFuncObj(CNPCNoFuncObj::NPCNoFuncObjTYPE eType, CQuestMgr::NPCTYPE eNPCType, _bool bBool);
	//void		SetClear_NPCNoFuncObj(CNPCNoFuncObj::NPCNoFuncObjTYPE eType, CQuestMgr::NPCTYPE eNPCType); //씬바뀔때 벡터변수 클리어해주기위해서 만든 함수
	_bool		SceneChageToMatchingGame();
	void		SetRender_QuestCheckBox(_bool bBool); //체크박스-네모
	//void		SetRenderTrue_QuestCheck();
	void		SetRender_QuestCheck(); // 체크박스 - 체크

	//readyUI 에서 무기 체인지 관련 UI 함수들
	HRESULT		CreateSwapKarmaUI(LPDIRECT3DDEVICE9 pGraphicDev, CKarma::KARMAPOSITION eKarma);
	_bool		CheckMyKarmaPosition(CKarma::KARMAPOSITION eKarmaPosition, CKarma::KARMATYPE eKarmaType);
	void		EraseSwapKarmaUI();
	void		UpdateSwapKarmaUI(CKarma::KARMAPOSITION eKarmaPosition, CKarma::KARMATYPE eKarmaType);
	void		CheckEraseSwapPossible();

public:
	////커스터마이징 함수
	void		CreateFaceUI(LPDIRECT3DDEVICE9 pGraphicDev);
	void		CreateHairUI(LPDIRECT3DDEVICE9 pGraphicDev);
	void		CreateClothesUI(LPDIRECT3DDEVICE9 pGraphicDev);
	void		EraseFaceUI();
	void		EraseHairUI();
	void		EraseClothesUI();
	void		ClearAllCustomizingUI();

	//변경할때사용하는함수
	void		CheckIsWearing(wstring wstrParts, _uint iNum);
	void		ChangeParts(wstring wstrParts, _uint iNum);
	void		SetIsWearing_Face();
	void		SetIsWearing_Hair();
	void		SetIsWearing_Clothes();
	void		ChangeColor(wstring wstrParts, _vec4 vColor);
	void		SetAnimationForParts(); //커마끝나고 마지막에 애니메이션번호 셋해줄때 쓰는 함수
	void		TakeOffParts(wstring wstrParts);

public:
	//////깃발전 함수
	////깃발전으로 넘어가게 도와주는 함수
	_bool		SceneChangeToCTF();

	////결과창에 필요한 정보들 관련 함수
	void		Set_FlagPlayersID(OBJID eObjID, _tchar* szID); //아이들 id 설정해주는 함수
	void		CheckFlagScore(LPDIRECT3DDEVICE9 pGraphicDev, OBJID eKiller, OBJID eDead); //킬했을 시 점수계산, 킬수도계산
	void		CheckFlagScore(OBJID eTaker); //깃발획득 시 점수계산,각자누적계산
	void		SetAccumulatedDamageForFlag(OBJID eOBJ, _uint iDamage); //입힌 데미지 누적 - 맞는애가 호출
	void		SetAccumulatedBreakForFlag(OBJID eOBJ, _uint iBreak); //입힌 브레이크 누적 - 맞는애가 호출
	void		SetWeapon(OBJID eOBJ, _uint iMainWeapon, _uint iSubWeapon);
	//
	void		SetStartFlagFlash();
	void		CreateFlagScorePopUp(LPDIRECT3DDEVICE9 pGraphicDev, CFlagScorePopUp::POPUPCOLOR eType); //30점 땄을 때 뜨는 점수판
	void		CreateRevivalPopUp(LPDIRECT3DDEVICE9 pGraphicDev); // "몇 초후 자동소생 " 문구 생성
	void		CreateGetFlagSuccessPopUp(LPDIRECT3DDEVICE9 pGraphicDev); // "깃발 획등 성공!" 문구 생성 & 깃발생성 카운트다운
	void		CreateReadyFlagPopUp(LPDIRECT3DDEVICE9 pGraphicDev); //"깃발 재생성 시작" 문구 생성
	void		CreateReCreateFlagPopUp(LPDIRECT3DDEVICE9 pGraphicDev); //"깃발 재생성 완료" 문구 생성

	void		CaptureingFlag(LPDIRECT3DDEVICE9 pGraphicDev, TEAM eTeamColor, _double dTime); //깃발 획득 중에 사용. 게이지바 이미지 생성
	void		CaptureFlagFailed(); // 깃발획득 중 누가 공격하면 풀림. 게이지바 이미지 사라짐
	//void		Clear_vecFlagBackBar(); //깃발 획득창 셋데드될때마다 호출예정 . 이 벡터변수도 클리어 해주야해.

	void		CheckFlagPos(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTargetTransform); //깃발이 생성되면 함수 호출해야해
	void		EraseFlagPos(); //깃발 획득해서 사라질때 지워야해 ->CreateGetFlagSuccessPopUp 안에서 호출중..
	


	void		ClearvecFlagBackBar(); //깃발점령동시에하는거막을라고 쓰는함수 

	////누가 누구 죽였는지에 관련된 UI 함수
	//(필요한 정보) - 죽인사람아이디, 죽인사람 팀, 죽은사람아이디, 죽은사람 팀 
	void		SetDeathBarPos();
	void		DeathBarTEST(LPDIRECT3DDEVICE9 pGraphicDev);
	void		AddDeathBarTEST(LPDIRECT3DDEVICE9 pGraphicDev);
	void		EraseFlagDeathInfoList();

	////깃발가까워지면 스페이스바UI 뜨게하는 함수
	void		SetRender_SpaceBar(_bool bBool);

public:
	//MatchingGame 함수
	void		TurnCheck(CMatchingMgr::MATCHINGTURN eType); //턴넘어갈 때 뜨는 문구
	void		EraseMatchingButton(/*CMatchingButton::BUTTONTYPE eType*/); //매칭버튼 지우기 함수
	_bool		CheckClickEXITButton();

public:
	//RunGame 함수
	void		CreateRunPlayer_RevivalUI(LPDIRECT3DDEVICE9 pGraphicDev);
	void		EraseRunButton(); //런버튼 지우기 함수
	void		EraseRunCountDown();
	void		SetTimeOver_RunCountDown();
	void		SetAccumulatedRunGamePoints(_uint iPoints); //점수 누적
	_uint		GetAccumulatedRunGamePoints() { return m_iAccumulatedRunGamePoints; }

public:
	////상시 체크 함수
	//마우스 생성 -> 상시X, 한번만
	void		CreateMouse(LPDIRECT3DDEVICE9 pGraphicDev, CMouse::MOUSETYPE eMouseType);
	void		EraseMouse();

	//stage에서 시간 잴 함수
	void		StageTimeCheck();

	//UI 있는지 없는지 체크하는 함수
	void		UI_OnCheck();

	//UI Render 조절 함수
	void		ChangeRenderState(const wstring& _wstrTag, _bool bBool);

public:
	//유아이 위치 조정 함수 ->임시
	void		KeyInput(const _double & dTimeDelta);

public:
	//인균 - 아래 코드는 UI에 있으면 안되고 AIMgr에 있어야하는데 누수나서 일로 옮김
	void					Set_BelatosTransformCom(Engine::CTransform* pTrans) { m_pBelatosTransformCom = pTrans; }
	void					Set_AITransformCom1(Engine::CTransform* pTrans) { m_pAITransfomCom1 = pTrans; }
	void					Set_AITransformCom2(Engine::CTransform* pTrans) { m_pAITransfomCom2 = pTrans; }
	Engine::CTransform*		Get_BelatosTransformCom() { return m_pBelatosTransformCom; }
	Engine::CTransform*		Get_AI1TransformCom() { return m_pAITransfomCom1; }
	Engine::CTransform*		Get_AI2TransformCom() { return m_pAITransfomCom2; }

private:
	//유아이들 포인터로
	//스킬 ->벡터, 키->벡터, 
	vector<CSkillSlot*>		m_vecSkillSlot; 
	vector<CNoFuncObj*>		m_vecNoFuncObj;
	vector<CSkill*>			m_vecSkill; //얘는 vecCurUI에 넣지 않았음. 지웠다 만들었다 반복할거라서
	vector<CUIObject*>		m_vecFlagNoFuncObj; //깃발전) 렌더상태바꾸기위해 사용
	vector<CFlagBackBar*>	m_vecFlagBackBar; //깃발전) 깃발획득 실패시 설정을 변경해주기 위해->vecCurUI에 안 넣음.
	vector<CFlagPos*>		m_vecFlagPos; //깃발전) 깃발있을 동안 위치알려주는 애들. 이걸로 setdead 할에정->vecCurUI에 안 넣음.
	CMouse*					m_pMouse;
	list<CNPCNoFuncObj*>	m_listNPCNoFuncObj; //로비) 얘는 vecCurUI에 넣지 않았음.
	CFlagNoFuncObj*			m_pFlagSpaceTXT;
	CUltimateRecharge*		m_pUltimateRecharge;
	list <CKarma*>			m_listKarma; //로비-ReadyUI) 카르마스왑때문에 여기서 관리할거야.vecCurUI에 넣지 않았음.. 결과창꺼는 안넣을거야
	list<CKarmaSlot*>		m_listKarmaSlot;//로비-ReadyUI) 카르마스왑때문에 여기서 관리할거야.vecCurUI에 넣지 않았음.. 결과창꺼는 안넣을거야
	list<CUIObject*>		m_listRunGameUI;//Run) 결과창뜰때 다지워주야하니까 여기서 셋데드해줄라고.->vecCurUI에 안 넣음(카운트다운은안넣었음)
	///////////////////////커스터마이징 변수////////////////////////////////
	vector<CCustomizingMenu*>			m_vecMenuLine;				//기능없는 글자밑에 선
	vector<CCustomizingEyeUI*>			m_vecCustomEyes;			//눈 텍스처
	vector<CCustomizingSelectBox*>		m_vecCustomEyesBack;		//눈 텍스처 주변 라인
	vector<CCustomizingBrowUI*>			m_vecCustomBrow;			//눈썹 텍스처 
	vector<CCustomizingSelectBox*>		m_vecCustomBrowBack;		//눈썹 텍스처 주변 라인
	CCustomizingColorBG*				m_pCustomizingColorBG;
	vector<CCustomizingFrontHairUI*>	m_vecCustomFrontHair;		//앞머리 텍스처
	vector<CCustomizingSelectBox*>		m_vecCustomFrontHairBack;	//앞머리 텍스처 주변 라인
	vector<CCustomizingBackHairUI*>		m_vecCustomBackHair;		//뒷머리 텍스처
	vector<CCustomizingSelectBox*>		m_vecCustomBackHairBack;	//뒷머리 텍스처 주변 라인
	vector<CCustomizingSideHairUI*>		m_vecCustomSideHair;		//옆머리 텍스처
	vector<CCustomizingSelectBox*>		m_vecCustomSideHairBack;	//옆머리 텍스처 주변 라인
	vector<CCustomizingTailHairUI*>		m_vecCustomTailHair;		//꼬리머리 텍스처
	vector<CCustomizingSelectBox*>		m_vecCustomTailHairBack;	//꼬리머리 텍스처 주변 라인
	vector<CCustomizingTopUI*>			m_vecCustomTop;				//상의 텍스처
	vector<CCustomizingSelectBox*>		m_vecCustomTopBack;			//상의 텍스처 주변 라인
	vector<CCustomizingPantsUI*>		m_vecCustomPants;			//하의 텍스처
	vector<CCustomizingSelectBox*>		m_vecCustomPantsBack;		//하의 텍스처 주변 라인
	vector<CCustomizingFootUI*>			m_vecCustomFoot;			//신발 텍스처
	vector<CCustomizingSelectBox*>		m_vecCustomFootBack;		//신발 텍스처 주변 라인
	CCustomizingHandsUI*				m_pCustomHands;				//장갑 텍스처----------------->벡터 아님. 종류1개여서.
	CCustomizingSelectBox*				m_pCustomHandsBack;			//장갑 텍스처 주변 라인------->벡터 아님. 종류1개여서.
	CCustomizingButton*					m_pCustomButton;			//"캐릭터 생성"버튼

	_bool								m_bFinishCustomize;			//다음 씬 넘어가기위한 변수
	///////////////////////////////////////////////////////////////////////

	//관리
	UITYPE					m_eUIType;

	//지금사용중인 UI리스트 
	vector<CUIObject*>		m_vecCurUI; 
	vector<CNormalObject*>	m_vecCurUI_Rc;

	_uint					m_iCurIndex;
	_uint					m_iNextIndex;

	//스테이지 시간 재는 변수 ->나중에 결과창에도 띄울거야
	_double					m_dAccumulatedTime;
	_double					m_dStageTime;
	_bool					m_bStageTimeStart;
	_bool					m_bStageTimeStop; //게임종료되면 시간체크도 종료(보스: 2초 후 결과창. 깃발: 2초 후 결과창. )
	_bool					m_bTimeCheck;

	//UI 켜져 있는지 없는지에 대한 체크 변수
	_bool					m_bUI_Working;

	//마우스 픽스 온오프->한번만
	_bool					m_bOne;

	//Tab->스킬 바뀌는 걸 위한 변수
	_uint					m_iCurWeapon;
	_uint					m_iPrevWeapon;

	//MissionUI에서 Lock/Unlock 표시를 도와주는 변수
	_bool					m_bMissionFlagLock; //True ->락상태(생성할거야 미션락클래스) False ->언락상태(생성안할거야 미션락클래스)
	_bool					m_bMissionApostleLock; //True ->락상태(생성할거야 미션락클래스) False ->언락상태(생성안할거야 미션락클래스)
	_bool					m_bMissionBossLock; //True ->락상태(생성할거야 미션락클래스) False ->언락상태(생성안할거야 미션락클래스)

	//깃발전
	_bool					m_bGetFlag; //-> 깃발 점령체크 불변수 / 점령되었으면 true.
	_bool					m_bReCreateFlag; //-> 깃발 재생성 불변수

	//런게임
	_uint					m_iAccumulatedRunGamePoints;

	//UI 내부에서 저장해야할 정보
	// 누적 데미지
	// 누적 시간 ->얜 저기변수로 하면될듯 stagetime
	// 누적 킬
	// 누적 데스
	// 누적 ..

	//보스전 --> 카르마2개, 방해량(break), 피해량, 데스, 클리어 타임
	//_uint m_iMainWeapon;
	//_uint m_iSubWeapon;
	_uint					m_iAccumulatedBreak;
	_uint					m_iAccumulatedDamage;
	_uint					m_iAccumulatedDeath;
	_double					m_dbAccumulatedTime;
	_bool					m_bCreateBossResultUI; //벨라토스 죽으면 결과창UI 뜨게 도와주는 변수
	_bool					m_bCreateShootingResultUI; //Apostle 죽으면 결과창UI 뜨게 도와주는 변수

	//깃발전 --> (개인 )카르마2개, 얻은 점수, 죽인횟수 , 피해량, 방해량
	//           (팀 별)총점수
	FLAG_RESULT				m_tPlayer;
	FLAG_RESULT				m_tAlliance;
	FLAG_RESULT				m_tEnemy_1;
	FLAG_RESULT				m_tEnemy_2;
	_uint					m_iBlueTotalScore;
	_uint					m_iRedTotalScore;

	_uint					m_iScore; //매겨지는 점수 
	_bool					m_bCartelFinish; // 깃발전 끝나는걸 체크하는 변수
	
	list<CFDBCreator*>		m_listFlagDeathInfo; //누가누구죽이는지 정보 보관하는 변수
	_uint					m_ilistPrevCnt;
	_uint					m_ilistCurCnt;

	//런게임
	_bool					m_bRunGameStart; //스타트 문구 나오고 난 후에 런게임 시작하게 할려고 만든 변수
	_bool					m_bRunGameTimeStop; //런게임 시간재는 걸 막을지 말지 결정하는변수

	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	//모든 씬에서의 플레이어의 무기
	PLAYERWEAPON m_ePlayerMainWeapon;
	PLAYERWEAPON m_ePlayerSubWeapon;
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	//무기스왑
	_bool					m_bEraseSwapKarmaUIPossible;

	//인균
	_bool					m_bChangeScene; //씬 바꾸는 변수
	Engine::CTransform*		m_pBelatosTransformCom; // 벨라토스
	Engine::CTransform*		m_pAITransfomCom1; // AI1
	Engine::CTransform*		m_pAITransfomCom2; // AI2
};

#endif
