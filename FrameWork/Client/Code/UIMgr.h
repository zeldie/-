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

//////&& Ŀ���� ����¡
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
	_bool			Get_SceneChange(); // : �κ�->�����佺��
	UITYPE&			Get_UIType() { return m_eUIType; }
	_uint			Get_AccumulatedDeath() { return m_iAccumulatedDeath; } //������)
	_uint			Get_AccumulatedBreak() { return m_iAccumulatedBreak; } //������)
	_uint			Get_AccumulatedDamage() { return m_iAccumulatedDamage; } //������)
	_double			Get_AccumulatedTime() { return m_dbAccumulatedTime; } //������)
	_uint			Get_BlueScore() { return m_iBlueTotalScore; } //�����)
	_uint			Get_RedScore() { return m_iRedTotalScore; } //�����)
	_bool			Get_CartelFinish() { return m_bCartelFinish; } //�����)
	FLAG_RESULT&	Get_FlagResultInfo(OBJID eOBJ); //�����)
	_bool			Get_MissionFlagLock() { return m_bMissionFlagLock; }
	_bool			Get_MissionApostleLock() { return m_bMissionApostleLock; }
	_bool			Get_MissionBossLock() { return m_bMissionBossLock; }
	_bool			Get_ChangeScene() { return m_bChangeScene; } //���� ������ �Ѿ�� �ϱ� ���� ������ ����(�α�)
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
	void		ChangeMouseType(CMouse::MOUSETYPE eMouseType) { m_pMouse->Change_MouseType(eMouseType); } //���콺Ÿ�Թٲٴ� �Լ�
	void		Set_ChangeScene(_bool bChangeScene) { m_bChangeScene = bChangeScene; } //�α�
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
	//vecCurUI�� add �ϱ� ���� �Լ�
	void		AddCurUI(Engine::CGameObject* pGameObject);

	//vecNPCNOFuncObj�� add �ϱ� ���� �Լ�
	void		AddvecNPCNoFuncObj(Engine::CGameObject* pGameObject);

	//UI �� �÷��̾������� �ش��ϴ� ���̵� �����ϴ� �Լ� -->�ʹ��� ���� ����
	HRESULT		CreatePlayerInfoUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer* pLayer);

	//�ʿ��� �����̸� ���� ������ �Լ�
	HRESULT		CreateBattleUI(LPDIRECT3DDEVICE9 pGraphicDev,/* Engine::CLayer* pLayer,*/ UITYPE eUIType); // Boss
	HRESULT		CreateCartelUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer* pLayer); // ����� 
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
	
	//������ ������ �Լ�(set dead)
	void		ClearUI();
	void		ClearPointerUI();

	//skillslot ��ٿ� ���� �Լ�
	void		SkillCoolDownCheck(Engine::KEYGUIDE KeyType);

	//Skill ��ٿ� ������ �����ϴ� �Լ�
	_bool		CheckSkillCoolDowning(Engine::KEYGUIDE KeyType); 
	
	//��ٿ� ���϶� ���� Ű ������ "���� �����" �۾� �߰� �ϴ� �Լ�
	void		OnCoolDown();

	//�����ִ� ��ų�� ǥ�����ְ�
	void		UseableSkillCheck(Engine::KEYGUIDE KeyType); //���ڷ� �޴� Ű�� ������ ������ �������� �ȵ���
	void		UseableSkillCheck_All(_bool _bBool);	// ��� ������ �����ų�, �ȵ����ų�.

	//TabŰ ������ �� ����ٲ�鼭 ��ų�� �ٲ�
	void		SwitchSkill(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrTexture, CSkill::SKILLTYPE eType);

	//DamageFont ���� �Լ�
	void		CreateDamageBox(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTargetTransform, CDamageBox::DAMAGEBOXTYPE eType, _uint iDamage);

	//UI->Tired ���� & �Ҹ� �Լ�
	void		CreateTiredUI(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTargetTransform);
	void		EraseTiredUI();

	//"���� ����" ���� ����& �Ҹ� �Լ�
	void		CreateDeficientMP();
	
	// Q��ų �����Լ�
	void		Set_QSkillEnd();

	//���� ���â -> ������ ������ų �Լ�
	void		SetAccumulatedDeath(_uint iDeath) { m_iAccumulatedDeath += iDeath; }
	void		SetAccumulatedDamage(_uint iDamage) { m_iAccumulatedDamage += iDamage; }
	void		SetAccumulatedBreak(_uint iBreak) { m_iAccumulatedBreak += iBreak; }
	void		SetAccumulatedTime(_double dbTime) { m_dbAccumulatedTime = dbTime; }
	
	//���� ���â ���� 0���� �ʱ�ȭ�ϴ� �Լ�
	void		SetZeroForAccumulatedVariables();

	//���� õ��� �Ѿ�� �����ִ� �Լ�
	_bool		SceneChangeToApostle();

	//NPC ���� UI
	void		SetRender_NPCNoFuncObj(CNPCNoFuncObj::NPCNoFuncObjTYPE eType, CQuestMgr::NPCTYPE eNPCType, _bool bBool);
	//void		SetClear_NPCNoFuncObj(CNPCNoFuncObj::NPCNoFuncObjTYPE eType, CQuestMgr::NPCTYPE eNPCType); //���ٲ� ���ͺ��� Ŭ�������ֱ����ؼ� ���� �Լ�
	_bool		SceneChageToMatchingGame();
	void		SetRender_QuestCheckBox(_bool bBool); //üũ�ڽ�-�׸�
	//void		SetRenderTrue_QuestCheck();
	void		SetRender_QuestCheck(); // üũ�ڽ� - üũ

	//readyUI ���� ���� ü���� ���� UI �Լ���
	HRESULT		CreateSwapKarmaUI(LPDIRECT3DDEVICE9 pGraphicDev, CKarma::KARMAPOSITION eKarma);
	_bool		CheckMyKarmaPosition(CKarma::KARMAPOSITION eKarmaPosition, CKarma::KARMATYPE eKarmaType);
	void		EraseSwapKarmaUI();
	void		UpdateSwapKarmaUI(CKarma::KARMAPOSITION eKarmaPosition, CKarma::KARMATYPE eKarmaType);
	void		CheckEraseSwapPossible();

public:
	////Ŀ���͸���¡ �Լ�
	void		CreateFaceUI(LPDIRECT3DDEVICE9 pGraphicDev);
	void		CreateHairUI(LPDIRECT3DDEVICE9 pGraphicDev);
	void		CreateClothesUI(LPDIRECT3DDEVICE9 pGraphicDev);
	void		EraseFaceUI();
	void		EraseHairUI();
	void		EraseClothesUI();
	void		ClearAllCustomizingUI();

	//�����Ҷ�����ϴ��Լ�
	void		CheckIsWearing(wstring wstrParts, _uint iNum);
	void		ChangeParts(wstring wstrParts, _uint iNum);
	void		SetIsWearing_Face();
	void		SetIsWearing_Hair();
	void		SetIsWearing_Clothes();
	void		ChangeColor(wstring wstrParts, _vec4 vColor);
	void		SetAnimationForParts(); //Ŀ�������� �������� �ִϸ��̼ǹ�ȣ �����ٶ� ���� �Լ�
	void		TakeOffParts(wstring wstrParts);

public:
	//////����� �Լ�
	////��������� �Ѿ�� �����ִ� �Լ�
	_bool		SceneChangeToCTF();

	////���â�� �ʿ��� ������ ���� �Լ�
	void		Set_FlagPlayersID(OBJID eObjID, _tchar* szID); //���̵� id �������ִ� �Լ�
	void		CheckFlagScore(LPDIRECT3DDEVICE9 pGraphicDev, OBJID eKiller, OBJID eDead); //ų���� �� �������, ų�������
	void		CheckFlagScore(OBJID eTaker); //���ȹ�� �� �������,���ڴ������
	void		SetAccumulatedDamageForFlag(OBJID eOBJ, _uint iDamage); //���� ������ ���� - �´¾ְ� ȣ��
	void		SetAccumulatedBreakForFlag(OBJID eOBJ, _uint iBreak); //���� �극��ũ ���� - �´¾ְ� ȣ��
	void		SetWeapon(OBJID eOBJ, _uint iMainWeapon, _uint iSubWeapon);
	//
	void		SetStartFlagFlash();
	void		CreateFlagScorePopUp(LPDIRECT3DDEVICE9 pGraphicDev, CFlagScorePopUp::POPUPCOLOR eType); //30�� ���� �� �ߴ� ������
	void		CreateRevivalPopUp(LPDIRECT3DDEVICE9 pGraphicDev); // "�� ���� �ڵ��һ� " ���� ����
	void		CreateGetFlagSuccessPopUp(LPDIRECT3DDEVICE9 pGraphicDev); // "��� ȹ�� ����!" ���� ���� & ��߻��� ī��Ʈ�ٿ�
	void		CreateReadyFlagPopUp(LPDIRECT3DDEVICE9 pGraphicDev); //"��� ����� ����" ���� ����
	void		CreateReCreateFlagPopUp(LPDIRECT3DDEVICE9 pGraphicDev); //"��� ����� �Ϸ�" ���� ����

	void		CaptureingFlag(LPDIRECT3DDEVICE9 pGraphicDev, TEAM eTeamColor, _double dTime); //��� ȹ�� �߿� ���. �������� �̹��� ����
	void		CaptureFlagFailed(); // ���ȹ�� �� ���� �����ϸ� Ǯ��. �������� �̹��� �����
	//void		Clear_vecFlagBackBar(); //��� ȹ��â �µ���ɶ����� ȣ�⿹�� . �� ���ͺ����� Ŭ���� ���־���.

	void		CheckFlagPos(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTargetTransform); //����� �����Ǹ� �Լ� ȣ���ؾ���
	void		EraseFlagPos(); //��� ȹ���ؼ� ������� �������� ->CreateGetFlagSuccessPopUp �ȿ��� ȣ����..
	


	void		ClearvecFlagBackBar(); //������ɵ��ÿ��ϴ°Ÿ������ �����Լ� 

	////���� ���� �׿������� ���õ� UI �Լ�
	//(�ʿ��� ����) - ���λ�����̵�, ���λ�� ��, ����������̵�, ������� �� 
	void		SetDeathBarPos();
	void		DeathBarTEST(LPDIRECT3DDEVICE9 pGraphicDev);
	void		AddDeathBarTEST(LPDIRECT3DDEVICE9 pGraphicDev);
	void		EraseFlagDeathInfoList();

	////��߰�������� �����̽���UI �߰��ϴ� �Լ�
	void		SetRender_SpaceBar(_bool bBool);

public:
	//MatchingGame �Լ�
	void		TurnCheck(CMatchingMgr::MATCHINGTURN eType); //�ϳѾ �� �ߴ� ����
	void		EraseMatchingButton(/*CMatchingButton::BUTTONTYPE eType*/); //��Ī��ư ����� �Լ�
	_bool		CheckClickEXITButton();

public:
	//RunGame �Լ�
	void		CreateRunPlayer_RevivalUI(LPDIRECT3DDEVICE9 pGraphicDev);
	void		EraseRunButton(); //����ư ����� �Լ�
	void		EraseRunCountDown();
	void		SetTimeOver_RunCountDown();
	void		SetAccumulatedRunGamePoints(_uint iPoints); //���� ����
	_uint		GetAccumulatedRunGamePoints() { return m_iAccumulatedRunGamePoints; }

public:
	////��� üũ �Լ�
	//���콺 ���� -> ���X, �ѹ���
	void		CreateMouse(LPDIRECT3DDEVICE9 pGraphicDev, CMouse::MOUSETYPE eMouseType);
	void		EraseMouse();

	//stage���� �ð� �� �Լ�
	void		StageTimeCheck();

	//UI �ִ��� ������ üũ�ϴ� �Լ�
	void		UI_OnCheck();

	//UI Render ���� �Լ�
	void		ChangeRenderState(const wstring& _wstrTag, _bool bBool);

public:
	//������ ��ġ ���� �Լ� ->�ӽ�
	void		KeyInput(const _double & dTimeDelta);

public:
	//�α� - �Ʒ� �ڵ�� UI�� ������ �ȵǰ� AIMgr�� �־���ϴµ� �������� �Ϸ� �ű�
	void					Set_BelatosTransformCom(Engine::CTransform* pTrans) { m_pBelatosTransformCom = pTrans; }
	void					Set_AITransformCom1(Engine::CTransform* pTrans) { m_pAITransfomCom1 = pTrans; }
	void					Set_AITransformCom2(Engine::CTransform* pTrans) { m_pAITransfomCom2 = pTrans; }
	Engine::CTransform*		Get_BelatosTransformCom() { return m_pBelatosTransformCom; }
	Engine::CTransform*		Get_AI1TransformCom() { return m_pAITransfomCom1; }
	Engine::CTransform*		Get_AI2TransformCom() { return m_pAITransfomCom2; }

private:
	//�����̵� �����ͷ�
	//��ų ->����, Ű->����, 
	vector<CSkillSlot*>		m_vecSkillSlot; 
	vector<CNoFuncObj*>		m_vecNoFuncObj;
	vector<CSkill*>			m_vecSkill; //��� vecCurUI�� ���� �ʾ���. ������ ������� �ݺ��ҰŶ�
	vector<CUIObject*>		m_vecFlagNoFuncObj; //�����) �������¹ٲٱ����� ���
	vector<CFlagBackBar*>	m_vecFlagBackBar; //�����) ���ȹ�� ���н� ������ �������ֱ� ����->vecCurUI�� �� ����.
	vector<CFlagPos*>		m_vecFlagPos; //�����) ������� ���� ��ġ�˷��ִ� �ֵ�. �̰ɷ� setdead �ҿ���->vecCurUI�� �� ����.
	CMouse*					m_pMouse;
	list<CNPCNoFuncObj*>	m_listNPCNoFuncObj; //�κ�) ��� vecCurUI�� ���� �ʾ���.
	CFlagNoFuncObj*			m_pFlagSpaceTXT;
	CUltimateRecharge*		m_pUltimateRecharge;
	list <CKarma*>			m_listKarma; //�κ�-ReadyUI) ī�������Ҷ����� ���⼭ �����Ұž�.vecCurUI�� ���� �ʾ���.. ���â���� �ȳ����ž�
	list<CKarmaSlot*>		m_listKarmaSlot;//�κ�-ReadyUI) ī�������Ҷ����� ���⼭ �����Ұž�.vecCurUI�� ���� �ʾ���.. ���â���� �ȳ����ž�
	list<CUIObject*>		m_listRunGameUI;//Run) ���â�㶧 �������־��ϴϱ� ���⼭ �µ������ٶ��.->vecCurUI�� �� ����(ī��Ʈ�ٿ����ȳ־���)
	///////////////////////Ŀ���͸���¡ ����////////////////////////////////
	vector<CCustomizingMenu*>			m_vecMenuLine;				//��ɾ��� ���ڹؿ� ��
	vector<CCustomizingEyeUI*>			m_vecCustomEyes;			//�� �ؽ�ó
	vector<CCustomizingSelectBox*>		m_vecCustomEyesBack;		//�� �ؽ�ó �ֺ� ����
	vector<CCustomizingBrowUI*>			m_vecCustomBrow;			//���� �ؽ�ó 
	vector<CCustomizingSelectBox*>		m_vecCustomBrowBack;		//���� �ؽ�ó �ֺ� ����
	CCustomizingColorBG*				m_pCustomizingColorBG;
	vector<CCustomizingFrontHairUI*>	m_vecCustomFrontHair;		//�ոӸ� �ؽ�ó
	vector<CCustomizingSelectBox*>		m_vecCustomFrontHairBack;	//�ոӸ� �ؽ�ó �ֺ� ����
	vector<CCustomizingBackHairUI*>		m_vecCustomBackHair;		//�޸Ӹ� �ؽ�ó
	vector<CCustomizingSelectBox*>		m_vecCustomBackHairBack;	//�޸Ӹ� �ؽ�ó �ֺ� ����
	vector<CCustomizingSideHairUI*>		m_vecCustomSideHair;		//���Ӹ� �ؽ�ó
	vector<CCustomizingSelectBox*>		m_vecCustomSideHairBack;	//���Ӹ� �ؽ�ó �ֺ� ����
	vector<CCustomizingTailHairUI*>		m_vecCustomTailHair;		//�����Ӹ� �ؽ�ó
	vector<CCustomizingSelectBox*>		m_vecCustomTailHairBack;	//�����Ӹ� �ؽ�ó �ֺ� ����
	vector<CCustomizingTopUI*>			m_vecCustomTop;				//���� �ؽ�ó
	vector<CCustomizingSelectBox*>		m_vecCustomTopBack;			//���� �ؽ�ó �ֺ� ����
	vector<CCustomizingPantsUI*>		m_vecCustomPants;			//���� �ؽ�ó
	vector<CCustomizingSelectBox*>		m_vecCustomPantsBack;		//���� �ؽ�ó �ֺ� ����
	vector<CCustomizingFootUI*>			m_vecCustomFoot;			//�Ź� �ؽ�ó
	vector<CCustomizingSelectBox*>		m_vecCustomFootBack;		//�Ź� �ؽ�ó �ֺ� ����
	CCustomizingHandsUI*				m_pCustomHands;				//�尩 �ؽ�ó----------------->���� �ƴ�. ����1������.
	CCustomizingSelectBox*				m_pCustomHandsBack;			//�尩 �ؽ�ó �ֺ� ����------->���� �ƴ�. ����1������.
	CCustomizingButton*					m_pCustomButton;			//"ĳ���� ����"��ư

	_bool								m_bFinishCustomize;			//���� �� �Ѿ������ ����
	///////////////////////////////////////////////////////////////////////

	//����
	UITYPE					m_eUIType;

	//���ݻ������ UI����Ʈ 
	vector<CUIObject*>		m_vecCurUI; 
	vector<CNormalObject*>	m_vecCurUI_Rc;

	_uint					m_iCurIndex;
	_uint					m_iNextIndex;

	//�������� �ð� ��� ���� ->���߿� ���â���� ���ž�
	_double					m_dAccumulatedTime;
	_double					m_dStageTime;
	_bool					m_bStageTimeStart;
	_bool					m_bStageTimeStop; //��������Ǹ� �ð�üũ�� ����(����: 2�� �� ���â. ���: 2�� �� ���â. )
	_bool					m_bTimeCheck;

	//UI ���� �ִ��� �������� ���� üũ ����
	_bool					m_bUI_Working;

	//���콺 �Ƚ� �¿���->�ѹ���
	_bool					m_bOne;

	//Tab->��ų �ٲ�� �� ���� ����
	_uint					m_iCurWeapon;
	_uint					m_iPrevWeapon;

	//MissionUI���� Lock/Unlock ǥ�ø� �����ִ� ����
	_bool					m_bMissionFlagLock; //True ->������(�����Ұž� �̼Ƕ�Ŭ����) False ->�������(�������Ұž� �̼Ƕ�Ŭ����)
	_bool					m_bMissionApostleLock; //True ->������(�����Ұž� �̼Ƕ�Ŭ����) False ->�������(�������Ұž� �̼Ƕ�Ŭ����)
	_bool					m_bMissionBossLock; //True ->������(�����Ұž� �̼Ƕ�Ŭ����) False ->�������(�������Ұž� �̼Ƕ�Ŭ����)

	//�����
	_bool					m_bGetFlag; //-> ��� ����üũ �Һ��� / ���ɵǾ����� true.
	_bool					m_bReCreateFlag; //-> ��� ����� �Һ���

	//������
	_uint					m_iAccumulatedRunGamePoints;

	//UI ���ο��� �����ؾ��� ����
	// ���� ������
	// ���� �ð� ->�� ���⺯���� �ϸ�ɵ� stagetime
	// ���� ų
	// ���� ����
	// ���� ..

	//������ --> ī����2��, ���ط�(break), ���ط�, ����, Ŭ���� Ÿ��
	//_uint m_iMainWeapon;
	//_uint m_iSubWeapon;
	_uint					m_iAccumulatedBreak;
	_uint					m_iAccumulatedDamage;
	_uint					m_iAccumulatedDeath;
	_double					m_dbAccumulatedTime;
	_bool					m_bCreateBossResultUI; //�����佺 ������ ���âUI �߰� �����ִ� ����
	_bool					m_bCreateShootingResultUI; //Apostle ������ ���âUI �߰� �����ִ� ����

	//����� --> (���� )ī����2��, ���� ����, ����Ƚ�� , ���ط�, ���ط�
	//           (�� ��)������
	FLAG_RESULT				m_tPlayer;
	FLAG_RESULT				m_tAlliance;
	FLAG_RESULT				m_tEnemy_1;
	FLAG_RESULT				m_tEnemy_2;
	_uint					m_iBlueTotalScore;
	_uint					m_iRedTotalScore;

	_uint					m_iScore; //�Ű����� ���� 
	_bool					m_bCartelFinish; // ����� �����°� üũ�ϴ� ����
	
	list<CFDBCreator*>		m_listFlagDeathInfo; //�����������̴��� ���� �����ϴ� ����
	_uint					m_ilistPrevCnt;
	_uint					m_ilistCurCnt;

	//������
	_bool					m_bRunGameStart; //��ŸƮ ���� ������ �� �Ŀ� ������ �����ϰ� �ҷ��� ���� ����
	_bool					m_bRunGameTimeStop; //������ �ð���� �� ������ ���� �����ϴº���

	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	//��� �������� �÷��̾��� ����
	PLAYERWEAPON m_ePlayerMainWeapon;
	PLAYERWEAPON m_ePlayerSubWeapon;
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	//���⽺��
	_bool					m_bEraseSwapKarmaUIPossible;

	//�α�
	_bool					m_bChangeScene; //�� �ٲٴ� ����
	Engine::CTransform*		m_pBelatosTransformCom; // �����佺
	Engine::CTransform*		m_pAITransfomCom1; // AI1
	Engine::CTransform*		m_pAITransfomCom2; // AI2
};

#endif
