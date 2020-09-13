#include "stdafx.h"
#include "AiMgr.h"

#include "Player.h"
#include "Ai_Player.h"
#include "BaseEffect.h"
#include "Ai_PlayerState.h"
#include "AIConditionBar.h"

IMPLEMENT_SINGLETON(CAiMgr)

CAiMgr::CAiMgr()
	:m_pManagement(Engine::CManagement::GetInstance())
	, m_pAi_Alliance(nullptr)
	, m_pAi_Enermy_1(nullptr)
	, m_pAi_Enermy_2(nullptr)
	, m_eStageType(STAGE_END)
{
	m_wstrNameAliiance = L"Alliance";
	m_wstrNameEnermy_1 = L"Enermy_1";
	m_wstrNameEnermy_2 = L"Enermy_2";
	//m_vFlagPos = { 430.22f, 115.f, 118.8f };

	m_vInitialPos = { 0.f, -3000.f, 0.f };

	for (int i = OBJID::OBJECT_PLAYER; i < OBJID::OBJECT_MONSTER; ++i)
		m_vFirstPos[i] = m_vInitialPos;
	for (int i = OBJID::OBJECT_PLAYER; i < OBJID::OBJECT_MONSTER; ++i)
		m_vSecondPos[i] = m_vInitialPos;
	for (int i = OBJID::OBJECT_PLAYER; i < OBJID::OBJECT_MONSTER; ++i)
		m_vAvoidDir[i] = m_vInitialPos;

	CSoundMgr::Get_Instance()->SetAiVolumeDown(0);
}


CAiMgr::~CAiMgr()
{
	Release();
}

Engine::CGameObject* CAiMgr::Create_Alliance()
{
	m_pAi_Alliance = CAi_Player::Create(m_pGraphicDev, &_vec3(862.f, 56.f, 1066.f), &_vec3(0.f, -130.f, 0.f), &_vec3(0.5f, 0.5f, 0.5f),
		CBasePlayer::PLAYER_ID::ID_AI, CONTROLTYPE::CTRL_AI_ALLIANCE, m_wstrNameAliiance);
	if (m_pAi_Alliance == nullptr)
		return nullptr;

	return m_pAi_Alliance;
}

Engine::CGameObject * CAiMgr::Create_Enermy_1()
{
	m_pAi_Enermy_1 = CAi_Player::Create(m_pGraphicDev, &_vec3(-1480.f, 53.f, -991.f), &_vec3(0.f, 30.f, 0.f), &_vec3(0.5f, 0.5f, 0.5f),
		CBasePlayer::PLAYER_ID::ID_AI, CONTROLTYPE::CTRL_AI_ENERMY_1, m_wstrNameEnermy_1);
	if (m_pAi_Enermy_1 == nullptr)
		return nullptr;

	return m_pAi_Enermy_1;
}

Engine::CGameObject * CAiMgr::Create_Enermy_2()
{
	m_pAi_Enermy_2 = CAi_Player::Create(m_pGraphicDev, &_vec3(-1300.f, 53.5f, -1144.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.5f, 0.5f, 0.5f),
		CBasePlayer::PLAYER_ID::ID_AI, CONTROLTYPE::CTRL_AI_ENERMY_2, m_wstrNameEnermy_2);
	if (m_pAi_Enermy_2 == nullptr)
		return nullptr;

	return m_pAi_Enermy_2;
}

void CAiMgr::Update(const _double & dTimeDelta)
{
	//m_bCartelStart = true;

	if (m_bCartelReady == true)
	{
		if (m_pPlayer->Get_CartelBlueStart() == false)
		{
			m_pPlayer->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_WAIT_HABIT);
			m_pAi_Alliance->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_START);
			m_pPlayer->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_WAIT_HABIT);
			m_pAi_Alliance->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_START);
		}
		if (m_pAi_Alliance->Get_CartelBlueStart() == true)
		{
			m_pPlayer->Set_CartelBlueStart(true);

			m_pPlayer->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
			m_pAi_Alliance->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
			m_pPlayer->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
			m_pAi_Alliance->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);

			m_pAi_Enermy_1->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_START);
			m_pAi_Enermy_2->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_WAIT_HABIT);
			m_pAi_Enermy_1->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_START);
			m_pAi_Enermy_2->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_WAIT_HABIT);
		}
		if (m_pAi_Enermy_1->Get_CartelRedStart() == true)
		{
			m_pAi_Enermy_2->Set_CartelRedStart(true);

			m_pAi_Enermy_1->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
			m_pAi_Enermy_2->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
			m_pAi_Enermy_1->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
			m_pAi_Enermy_2->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
			m_bCartelReady = false;
			m_bCartelStart = true;
			

			//////////// 희정 추가 ////////////////////
			Engine::CGameObject*		pUIObject = nullptr;

			pUIObject = CAIConditionBar::Create(m_pGraphicDev, m_pAi_Alliance->Get_ControlType(), m_pAi_Alliance->Get_TransformCom());
			Engine::Add_GameObject(Engine::UI, L"AIAllCondition", pUIObject);

			pUIObject = CAIConditionBar::Create(m_pGraphicDev, m_pAi_Enermy_1->Get_ControlType(), m_pAi_Enermy_1->Get_TransformCom());
			Engine::Add_GameObject(Engine::UI, L"AIAllCondition", pUIObject);

			pUIObject = CAIConditionBar::Create(m_pGraphicDev, m_pAi_Enermy_2->Get_ControlType(), m_pAi_Enermy_2->Get_TransformCom());
			Engine::Add_GameObject(Engine::UI, L"AIAllCondition", pUIObject);

			///////////////////////////////////////////

		}
	}

	Reset_Key();
	Update_FlagPos();

	Ai_ControlCenter(dTimeDelta);
}

void CAiMgr::LateUpdate(const _double & dTimeDelta)
{
	//float fDist = D3DXVec3Length(&(m_vFlagPos - m_pPlayer->GetPlayerPos()));
	////cout << "플레이어와 깃발거리 : " << fDist << endl;

	if (Engine::KeyDown(DIK_NUMPAD0))
	{
		for (auto& rObj : m_vecAiCharacter)
		{
			switch (rObj->Get_ControlType())
			{
			case CTRL_AI_ALLIANCE:
				//cout << "Alliance의 위치는 : " << rObj->Get_TransformCom()->Get_Info(Engine::INFO_POS)->x << " / " << rObj->Get_TransformCom()->Get_Info(Engine::INFO_POS)->y << " / " << rObj->Get_TransformCom()->Get_Info(Engine::INFO_POS)->z << endl;
				break;
			case CTRL_AI_ENERMY_1:
				//cout << "Enermy_1의 위치는 : " << rObj->Get_TransformCom()->Get_Info(Engine::INFO_POS)->x << " / " << rObj->Get_TransformCom()->Get_Info(Engine::INFO_POS)->y << " / " << rObj->Get_TransformCom()->Get_Info(Engine::INFO_POS)->z << endl;
				break;
			case CTRL_AI_ENERMY_2:
				//cout << "Enermy_2의 위치는 : " << rObj->Get_TransformCom()->Get_Info(Engine::INFO_POS)->x << " / " << rObj->Get_TransformCom()->Get_Info(Engine::INFO_POS)->y << " / " << rObj->Get_TransformCom()->Get_Info(Engine::INFO_POS)->z << endl;
				break;
			}
		}
	}

	Debug();
	Cheat();

	//Check_Animation();
	// 점령중인 플레이어를 찾아서 점령완료하면 점령완료신호를 준다.
	Check_CapturePlayer();
	// 스킬 쿨타임을 돌린다.
	Update_Skill_Cooldown(dTimeDelta);
}

void CAiMgr::Release()
{
	Engine::Safe_Release(m_pGraphicDev);
}

void CAiMgr::Set_AttackType(CBasePlayer * pAi, ATTACKTYPE eType)
{
	AI_TYPE eAi = Get_AiType(pAi);

	m_eAttackType[eAi] = eType;
}

void CAiMgr::Set_StartSkillCoolDown(CBasePlayer * pAi, AI_SKILL eType)
{
	AI_TYPE eAi = Get_AiType(pAi);

	m_map_Skill_Cooldown[eAi].emplace(eType, 0);
}

void CAiMgr::Add_Effect(OBJID eType, Engine::CTransform * pEffectTrans)
{
	if (eType > OBJID::OBJECT_MONSTER)
		return;
	if (pEffectTrans == nullptr)
		return;
	m_list_Effect_Storage[eType].emplace_back(pEffectTrans);
}

void CAiMgr::Delete_Effect(OBJID eID, Engine::CTransform * pTrans)
{
	if (eID > OBJID::OBJECT_MONSTER)
		return;
	if (pTrans == nullptr)
		return;

	list<Engine::CTransform*>::iterator iter_begin_list = m_list_Effect_Storage[eID].begin();
	list<Engine::CTransform*>::iterator iter_end_list = m_list_Effect_Storage[eID].end();

	for (auto iter = iter_begin_list; iter != iter_end_list; )
	{
		if (*iter == pTrans)
		{
			iter = m_list_Effect_Storage[eID].erase(iter);
		}
		else
			++iter;
	}
}

HRESULT CAiMgr::Team_Setting()
{
	if (m_pPlayer != nullptr)
	{
		m_vecAllCharacter.push_back(m_pPlayer);
	}
	if (m_pAi_Alliance != nullptr)
	{
		m_vecAiCharacter.push_back(m_pAi_Alliance);
		m_vecAllCharacter.push_back(m_pAi_Alliance);
	}
	if (m_pAi_Enermy_1 != nullptr)
	{
		m_vecAiCharacter.push_back(m_pAi_Enermy_1);
		m_vecAllCharacter.push_back(m_pAi_Enermy_1);
	}
	if (m_pAi_Enermy_2 != nullptr)
	{
		m_vecAiCharacter.push_back(m_pAi_Enermy_2);
		m_vecAllCharacter.push_back(m_pAi_Enermy_2);
	}


	for (auto& pAi : m_vecAllCharacter)
	{
		if (pAi->Get_Team() == TEAM::TEAM_BLUE)
			m_listTeamBlue.push_back(pAi);
		else
			m_listTeamRed.push_back(pAi);
	}
	return S_OK;
}

void CAiMgr::Ai_ControlCenter(const _double & dTimeDelta)
{
	// 무기체크해서 해당 무기 ai로 !
	for (auto& pAi : m_vecAiCharacter)
	{
		if (Check_Dead(pAi))
			continue;

		Check_TargetDead(pAi);
		pAi->Set_CaptureFlag(false);


		// 깃발에서 너무 멀어지면 근접무기로 바꿈
		if (pAi->Get_ControlType() == CONTROLTYPE::CTRL_AI_ALLIANCE)
		{
			if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
			{
				if (pAi->Get_MainWeapon() == CBasePlayer::PLAYER_WEAPONTYPE::LONGBOW)
				{
					if (pAi->GetDist_ToFlag() > 1500)
						dynamic_cast<CAi_Player*>(pAi)->Change_Weapon();
				}
			}
			////cout << pAi->GetDist_ToFlag() << endl;
		}

		// 깃발과 가까워지면 다시 원거리로 바꿈
		if (pAi->Get_ControlType() == CONTROLTYPE::CTRL_AI_ALLIANCE)
		{
			if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
			{
				if (pAi->Get_MainWeapon() == CBasePlayer::PLAYER_WEAPONTYPE::DUALSWORD)
				{
					if (pAi->GetDist_ToFlag() < 1200)
						dynamic_cast<CAi_Player*>(pAi)->Change_Weapon();
				}
			}
		}

		switch (pAi->Get_MainWeapon())
		{
		case CPlayer::PLAYER_WEAPONTYPE::DUALSWORD:
			Ai_DualSword(pAi, dTimeDelta);
			break;
		case CPlayer::PLAYER_WEAPONTYPE::TWOHANDSWORD:
			Ai_TwoHandSword(pAi, dTimeDelta);
			break;
		case CPlayer::PLAYER_WEAPONTYPE::LONGBOW:
			Ai_LongBow(pAi, dTimeDelta);
			break;
		case CPlayer::PLAYER_WEAPONTYPE::ORB:
			Ai_Orb(pAi, dTimeDelta);
			break;
		}

		// 상태체크
		if (Engine::KeyDown(DIK_NUMPAD1))
		{
			for (auto& rObj : m_vecAiCharacter)
			{
				if (rObj->Get_ControlType() == CONTROLTYPE::CTRL_AI_ALLIANCE)
				{
					switch (rObj->Get_StateState())
					{
					case STATETYPE::STATE_COMMON:
						cout << "ALLIANCE is Common" << endl;
						break;
					case STATETYPE::STATE_AVOID:
						cout << "ALLIANCE is Avoid" << endl;
						break;
					case STATETYPE::STATE_ATTACK:
						cout << "ALLIANCE is Attack" << endl;
						break;
					case STATETYPE::STATE_AIR_DASH:
						cout << "ALLIANCE is Dash" << endl;
						break;
					default:
						cout << "ALLIANCE is ???" << endl;
						break;
					}
				}
				if (rObj->Get_ControlType() == CONTROLTYPE::CTRL_AI_ENERMY_1)
				{
					switch (rObj->Get_StateState())
					{
					case STATETYPE::STATE_COMMON:
						cout << "ENERMY_1 is Common" << endl;
						break;
					case STATETYPE::STATE_AVOID:
						cout << "ENERMY_1 is Avoid" << endl;
						break;
					case STATETYPE::STATE_ATTACK:
						cout << "ENERMY_1 is Attack" << endl;
						break;
					case STATETYPE::STATE_AIR_DASH:
						cout << "ENERMY_1 is Dash" << endl;
						break;
					default:
						cout << "ENERMY_1 is ???" << endl;
						break;
					}
				}
				if (rObj->Get_ControlType() == CONTROLTYPE::CTRL_AI_ENERMY_2)
				{
					switch (rObj->Get_StateState())
					{
					case STATETYPE::STATE_COMMON:
						cout << "ENERMY_2 is Common" << endl;
						break;
					case STATETYPE::STATE_AVOID:
						cout << "ENERMY_2 is Avoid" << endl;
						break;
					case STATETYPE::STATE_ATTACK:
						cout << "ENERMY_2 is Attack" << endl;
						break;
					case STATETYPE::STATE_AIR_DASH:
						cout << "ENERMY_2 is Dash" << endl;
						break;
					default:
						cout << "ENERMY_2 is ???" << endl;
						break;
					}
				}
			}
		}
	}
}

void CAiMgr::Ai_Common_ShortRange(CBasePlayer * pAi, const _double & dTimeDelta)
{
	//pAi->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);

	// 최우선순위로 깃발점령
	// 깃발 범위내에 적이 있을경우 해당 적을 타겟

	pAi->Set_Timer(L"Check_Target");

	if (pAi->Get_Timer(L"Check_Target") == 0.f)
	{
		pAi->Start_Timer(L"Check_Target");

		Check_FlagArea_RedTeam(pAi);
		Check_FlagArea_BlueTeam(pAi);
	}
	else
	{
		pAi->Start_Timer(L"Check_Target");

		if (pAi->Get_Timer(L"Check_Target") > 1.f)	// 타겟설정하는 타이머
		{
			pAi->Delete_Timer(L"Check_Target");
		}
	}



	// 점령 관련
	if (pAi->Get_StateState() == STATETYPE::STATE_COMMON)
	{
		if (pAi->Get_TargetPlayer() != nullptr)
		{
			// 타겟한테 접근

			CBasePlayer* pTarget = pAi->Get_TargetPlayer();
			_vec3 vPos = pAi->Get_TargetPos();

			pAi->Set_DownKey(KEY_W);
			pAi->Set_PressKey(KEY_W);
		}
		else if (pAi->Get_TargetPlayer() == nullptr)	// 타겟이 없다면 깃발점령한다.
		{
			Capture(pAi, dTimeDelta);
		}
	}



	// 타겟과의 거리가 가까우면 움직이지 않고 공격한다.
	if (pAi->Get_TargetPlayer() != nullptr)
	{
		// 가까워 지면 공격한다.
		if (Engine::GetDistNoY(pAi->Get_TargetPos(), pAi->GetPlayerPos()) < 250)
		{
			// 공격 상태로 전환
			// 어떤 공격할것인지 정해서 상태를 셋해줌
			pAi->Set_StateState(STATETYPE::STATE_ATTACK);
			return;
		}
	}
}


void CAiMgr::Ai_Common_LongRange(CBasePlayer * pAi, const _double & dTimeDelta)
{
	//pAi->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);

	// 최우선순위로 깃발점령
	// 깃발 범위내에 적이 있을경우 해당 적을 타겟

	pAi->Set_Timer(L"Check_Target");

	if (pAi->Get_Timer(L"Check_Target") == 0.f)
	{
		pAi->Start_Timer(L"Check_Target");

		Check_FlagArea_RedTeam(pAi);
		Check_FlagArea_BlueTeam(pAi);

	}
	else
	{
		pAi->Start_Timer(L"Check_Target");

		if (pAi->Get_Timer(L"Check_Target") > 1.f)	// 타겟설정하는 타이머
		{
			pAi->Delete_Timer(L"Check_Target");
		}
	}


	// 점령 관련
	if (pAi->Get_StateState() == STATETYPE::STATE_COMMON)
	{
		if (pAi->Get_TargetPlayer() != nullptr)
		{
			// 타겟한테 접근
			pAi->Set_DownKey(KEY_W);
			pAi->Set_PressKey(KEY_W);
			////cout << "KEY_W" << endl;
		}
		else if (pAi->Get_TargetPlayer() == nullptr)	// 타겟이 없다면 깃발점령한다.
		{
			Capture(pAi, dTimeDelta);
			////cout << "KEY_A" << endl;
			// Target이 없는데 뛰는현상 강제수정
		}
	}


	// 타겟과의 거리가 가까우면 움직이지 않고 공격한다.
	if (pAi->Get_TargetPlayer() != nullptr)
	{
		// 가까워 지면 공격한다.
		switch (pAi->Get_MainWeapon())
		{
		case CBasePlayer::PLAYER_WEAPONTYPE::LONGBOW:
			if (Engine::GetDistNoY(pAi->Get_TargetPos(), pAi->GetPlayerPos()) < 1000)
			{
				// 공격 상태로 전환
				// 어떤 공격할것인지 정해서 상태를 셋해줌
				pAi->Set_StateState(STATETYPE::STATE_ATTACK);
				return;
			}
			break;
		case CBasePlayer::PLAYER_WEAPONTYPE::ORB:
			if (Engine::GetDistNoY(pAi->Get_TargetPos(), pAi->GetPlayerPos()) < 800)
			{
				// 공격 상태로 전환
				// 어떤 공격할것인지 정해서 상태를 셋해줌
				pAi->Set_StateState(STATETYPE::STATE_ATTACK);
				return;
			}
			break;
		}
	}
}

void CAiMgr::Ai_Attack_ShortRange(CBasePlayer * pAi, const _double & dTimeDelta)
{
	if (pAi->Get_TargetPlayer() == nullptr)
	{
		pAi->Set_StateState(STATETYPE::STATE_COMMON);
		return;
	}

	// 거리가 멀어지면 다시 COMMON상태로 만들고 return
	if (pAi->GetDist_ToEnermy() > 300)
	{
		pAi->Set_StateState(STATETYPE::STATE_COMMON);
		return;
	}

	pAi->Set_DownKey(KEY_W);
	pAi->Set_PressKey(KEY_W);

	//TS_R(pAi, dTimeDelta);

	Run_AttackType(pAi, dTimeDelta);
}

void CAiMgr::Ai_Attack_LongRange(CBasePlayer * pAi, const _double & dTimeDelta)
{
	if (pAi->Get_TargetPlayer() == nullptr)
	{
		pAi->Set_StateState(STATETYPE::STATE_COMMON);
		return;
	}

	// 거리가 멀어지면 다시 COMMON상태로 만들고 return
	switch (pAi->Get_MainWeapon())
	{
	case CBasePlayer::PLAYER_WEAPONTYPE::LONGBOW:
		if (pAi->GetDist_ToEnermy() > 1000)
		{
			pAi->Set_StateState(STATETYPE::STATE_COMMON);
			return;
		}
		break;
	case CBasePlayer::PLAYER_WEAPONTYPE::ORB:
		if (pAi->GetDist_ToEnermy() > 800)
		{
			pAi->Set_StateState(STATETYPE::STATE_COMMON);
			return;
		}
		break;
	}

	pAi->Set_DownKey(KEY_W);
	pAi->Set_PressKey(KEY_W);


	Run_AttackType(pAi, dTimeDelta);
}

AI_TYPE CAiMgr::Get_AiType(CBasePlayer * pAi)
{
	switch (pAi->Get_ControlType())
	{
	case CONTROLTYPE::CTRL_AI_ALLIANCE:
		return AI_TYPE::AI_ALLIANCE;
	case CONTROLTYPE::CTRL_AI_ENERMY_1:
		return AI_TYPE::AI_ENERMY_1;
	case CONTROLTYPE::CTRL_AI_ENERMY_2:
		return AI_TYPE::AI_ENERMY_2;
	}

	return AI_TYPE::AI_END;
}

OBJID CAiMgr::Get_OBJID(CBasePlayer * pAi)
{
	return pAi->Get_BaseInfo()->eObjectID;
}

void CAiMgr::Select_AttackType(CBasePlayer * pAi, const _double & dTimeDelta)
{
	AI_TYPE eAi_Type = Get_AiType(pAi);

	AI_SKILL eSkill = Get_SkillType(pAi);

	// 일반 스킬이면
	if (eSkill != AI_SKILL::SKILL_END)
	{
		switch (eSkill)
		{
		case SKILL_Q:
			m_eAttackType[eAi_Type] = ATTACK_Q;
			////cout << "Q" << endl;
			break;
		case SKILL_E:
			m_eAttackType[eAi_Type] = ATTACK_E;
			////cout << "E" << endl;
			break;
		case SKILL_R:
			m_eAttackType[eAi_Type] = ATTACK_R;
			////cout << "R" << endl;
			break;
		case SKILL_F:
			m_eAttackType[eAi_Type] = ATTACK_F;
			////cout << "F" << endl;
			break;
		}
	}
	// End면 평타사용
	else
	{
		int iNum = Engine::RandomNumber(4);

		if (pAi->Get_Decal() != nullptr)
		{
			if (pAi->Get_MainWeapon() == CBasePlayer::PLAYER_WEAPONTYPE::ORB)
			{
				while (true)
				{
					if (iNum != 3 && iNum != 1)
						break;

					iNum = Engine::RandomNumber(4);
				}
			}
		}

		switch (iNum)
		{
		case 0:
			m_eAttackType[eAi_Type] = ATTACK_LB1;
			////cout << "LB1" << endl;
			break;
		case 1:
			m_eAttackType[eAi_Type] = ATTACK_LB2;
			////cout << "LB2" << endl;
			break;
		case 2:
			m_eAttackType[eAi_Type] = ATTACK_LB3;
			////cout << "LB3" << endl;
			break;
		case 3:
			m_eAttackType[eAi_Type] = ATTACK_RB;
			////cout << "RB" << endl;
			break;
		}
	}
}

AI_SKILL CAiMgr::Get_SkillType(CBasePlayer * pAi)
{
	AI_TYPE eAi_Type = Get_AiType(pAi);

	AI_SKILL eSkill[7] = { SKILL_Q, SKILL_END, SKILL_E, SKILL_R, SKILL_END, SKILL_F, SKILL_END };

	// 데칼스킬이 초기화가 안되있을때 다른 데칼스킬쓰면 사용중이던 스킬의 위치가 바뀌기 때문에 막음
	if (pAi->Get_Decal() != nullptr)
	{
		////cout << "Decal is Not nullptr" << endl;
		if (pAi->Get_MainWeapon() == CBasePlayer::PLAYER_WEAPONTYPE::LONGBOW)
		{
			// E스킬 -> R스킬
			eSkill[2] = SKILL_R;
		}
		else if (pAi->Get_MainWeapon() == CBasePlayer::PLAYER_WEAPONTYPE::ORB)
		{
			// E스킬 -> 평타
			eSkill[2] = SKILL_END;
			// F스킬 -> R스킬
			eSkill[5] = SKILL_R;
			// Q스킬 -> 평타
			eSkill[0] = SKILL_END;
		}
	}
	else
		////cout << "Decal is nullptr" << endl;

		if (pAi->Get_MainWeapon() == CBasePlayer::PLAYER_WEAPONTYPE::TWOHANDSWORD)
		{
			// 평타 -> R스킬
			eSkill[1] = SKILL_R;
			// 평타 -> R스킬
			eSkill[6] = SKILL_R;
		}

	while (true)
	{
		for (int i = 0; i <= 30; i++)
		{
			int iRand1 = Engine::RandomNumber(7);
			int iRand2 = Engine::RandomNumber(7);

			if (iRand1 == iRand2)
				continue;

			swap(eSkill[iRand1], eSkill[iRand2]);
		}
		break;
	}

	////cout << "순서 : " << eSkill[0] << "/" << eSkill[1] << "/" << eSkill[2] << "/" << eSkill[3] << "/" << eSkill[4] << endl;

	if (m_map_Skill_Cooldown[eAi_Type].end() == m_map_Skill_Cooldown[eAi_Type].find(eSkill[0]))
		return eSkill[0];
	else if (m_map_Skill_Cooldown[eAi_Type].end() == m_map_Skill_Cooldown[eAi_Type].find(eSkill[1]))
		return eSkill[1];
	else if (m_map_Skill_Cooldown[eAi_Type].end() == m_map_Skill_Cooldown[eAi_Type].find(eSkill[2]))
		return eSkill[2];
	else if (m_map_Skill_Cooldown[eAi_Type].end() == m_map_Skill_Cooldown[eAi_Type].find(eSkill[3]))
		return eSkill[3];
	else if (m_map_Skill_Cooldown[eAi_Type].end() == m_map_Skill_Cooldown[eAi_Type].find(eSkill[4]))
		return eSkill[4];

	return AI_SKILL::SKILL_END;
}

void CAiMgr::Run_AttackType(CBasePlayer * pAi, const _double & dTimeDelta)
{
	AI_TYPE eAi = Get_AiType(pAi);

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		if (m_eAttackType[eAi] == ATTACKTYPE::ATTACK_END)
			Select_AttackType(pAi, dTimeDelta);
	}

	switch (pAi->Get_MainWeapon())
	{
	case CBasePlayer::PLAYER_WEAPONTYPE::TWOHANDSWORD:
		switch (m_eAttackType[eAi])
		{
		case ATTACKTYPE::ATTACK_Q:
			TS_Q(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_E:
			TS_E(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_R:
			TS_R(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_F:
			TS_F(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB1:
			TS_BasicCombo(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB2:
			TS_BasicComboEx01(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB3:
			TS_BasicComboEx02(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_RB:
			TS_MouseRight(pAi, dTimeDelta);
			break;
		}
		break;
	case CBasePlayer::PLAYER_WEAPONTYPE::DUALSWORD:
		switch (m_eAttackType[eAi])
		{
		case ATTACKTYPE::ATTACK_Q:
			DS_Q(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_E:
			DS_E(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_R:
			DS_R(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_F:
			DS_F(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB1:
			DS_BasicCombo(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB2:
			DS_BasicComboEx01(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB3:
			DS_BasicComboEx02(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_RB:
			DS_MouseRight(pAi, dTimeDelta);
			break;
		}
		break;
	case CBasePlayer::PLAYER_WEAPONTYPE::LONGBOW:
		switch (m_eAttackType[eAi])
		{
		case ATTACKTYPE::ATTACK_Q:
			LB_Q(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_E:
			LB_E(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_R:
			LB_R(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_F:
			LB_F(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB1:
			LB_BasicCombo(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB2:
			LB_BasicComboEx(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB3:
			LB_BasicCombo_PiercingArrow(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_RB:
			LB_LBRB(pAi, dTimeDelta);
			break;
		}
		break;
	case CBasePlayer::PLAYER_WEAPONTYPE::ORB:
		switch (m_eAttackType[eAi])
		{
		case ATTACKTYPE::ATTACK_Q:
			OB_Q(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_E:
			OB_E(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_R:
			OB_R(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_F:
			OB_F(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB1:
			OB_BasicCombo(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB2:
			OB_BasicComboEx01(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_LB3:
			OB_BasicComboEx02(pAi, dTimeDelta);
			break;
		case ATTACKTYPE::ATTACK_RB:
			OB_MouseRight(pAi, dTimeDelta);
			break;
		}
		break;
	}

}

void CAiMgr::Ai_DualSword(CBasePlayer* pAi, const _double & dTimeDelta)
{
	// 해당 Ai와 주변 이펙트 검사
	if (Check_Effect(pAi))
	{
		if (Avoid(pAi, dTimeDelta))
			return;
	}



	// 적에게 다가가고 도망가는건 여기서
	// 공격상태인 경우 스킵
	if (pAi->Get_StateState() == STATETYPE::STATE_COMMON)
		Ai_Common_ShortRange(pAi, dTimeDelta);


	// 너무 가까우면 뒤로 물러선다
	Move_ToBackward(pAi, dTimeDelta);


	// 공격할때 쿨타임 확인하고 패턴 돌리는건 여기서?
	if (pAi->Get_StateState() == STATETYPE::STATE_ATTACK)
		Ai_Attack_ShortRange(pAi, dTimeDelta);

}

void CAiMgr::Ai_TwoHandSword(CBasePlayer* pAi, const _double & dTimeDelta)
{
	// 해당 Ai와 주변 이펙트 검사
	if (Check_Effect(pAi))
	{
		if (Avoid(pAi, dTimeDelta))
			return;
	}

	STATETYPE eType = pAi->Get_StateState();

	// 적에게 다가가고 도망가는건 여기서
	// 공격상태인 경우 스킵
	if (pAi->Get_StateState() == STATETYPE::STATE_COMMON)
		Ai_Common_ShortRange(pAi, dTimeDelta);


	// 너무 가까우면 뒤로 물러선다
	Move_ToBackward(pAi, dTimeDelta);


	// 공격할때 쿨타임 확인하고 패턴 돌리는건 여기서?
	if (pAi->Get_StateState() == STATETYPE::STATE_ATTACK)
		Ai_Attack_ShortRange(pAi, dTimeDelta);
}

void CAiMgr::Ai_LongBow(CBasePlayer* pAi, const _double & dTimeDelta)
{
	// 해당 Ai와 주변 이펙트 검사
	if (Check_Effect(pAi))
	{
		if (Avoid(pAi, dTimeDelta))
			return;
	}


	// 적에게 다가가고 도망가는건 여기서
	// 공격상태인 경우 스킵
	if (pAi->Get_StateState() == STATETYPE::STATE_COMMON)
		Ai_Common_LongRange(pAi, dTimeDelta);


	// 너무 가까우면 뒤로 물러선다
	Move_ToBackward(pAi, dTimeDelta);


	// 공격할때 쿨타임 확인하고 패턴 돌리는건 여기서?
	if (pAi->Get_StateState() == STATETYPE::STATE_ATTACK)
		Ai_Attack_LongRange(pAi, dTimeDelta);
}

void CAiMgr::Ai_Orb(CBasePlayer* pAi, const _double & dTimeDelta)
{
	// 해당 Ai와 주변 이펙트 검사
	if (Check_Effect(pAi))
	{
		if (Avoid(pAi, dTimeDelta))
			return;
	}



	// 적에게 다가가고 도망가는건 여기서
	// 공격상태인 경우 스킵
	if (pAi->Get_StateState() == STATETYPE::STATE_COMMON)
		Ai_Common_LongRange(pAi, dTimeDelta);


	// 너무 가까우면 뒤로 물러선다
	Move_ToBackward(pAi, dTimeDelta);


	// 공격할때 쿨타임 확인하고 패턴 돌리는건 여기서?
	if (pAi->Get_StateState() == STATETYPE::STATE_ATTACK)
		Ai_Attack_LongRange(pAi, dTimeDelta);
}

void CAiMgr::Move_ToBackward(CBasePlayer * pAi, const _double & dTimeDelta)
{
	if (pAi->Get_TargetPlayer() == nullptr)
	{
		if (pAi->Get_StateState() == STATETYPE::STATE_MOVE_TO_BACKWARD)
		{
			pAi->Set_StateState(STATETYPE::STATE_COMMON);
		}
		return;
	}

	if (pAi->GetDist_ToEnermy() < 100)
		pAi->Set_StateState(STATETYPE::STATE_MOVE_TO_BACKWARD);

	if (pAi->Get_StateState() == STATETYPE::STATE_MOVE_TO_BACKWARD)
	{
		pAi->Set_DownKey(KEY_S);
		pAi->Set_PressKey(KEY_S);

		if (pAi->GetDist_ToEnermy() > 300)
			pAi->Set_StateState(STATETYPE::STATE_COMMON);
	}
}

_bool CAiMgr::Avoid(CBasePlayer * pAi, const _double & dTimeDelta)
{
	AI_TYPE eAi = Get_AiType(pAi);
	OBJID eID = Get_OBJID(pAi);

	// 쿨타임중인지 검색해서 쿨타임중이면 리턴
	if (m_map_Skill_Cooldown[eAi].end() != m_map_Skill_Cooldown[eAi].find(SKILL_DASH))
		return false;

	// 맞는중이면 리턴
	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_HEART)
		return false;

	if (pAi->Get_StateState() == STATETYPE::STATE_ATTACK)
		return false;

	_vec3 vDir = m_vAvoidDir[eID];
	_vec3 vLook = *pAi->Get_TransformCom()->Get_Info(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);

	float fAngle = Engine::GetAngleY(vLook, vDir);
	if (!isnan(fAngle))
		pAi->Get_TransformCom()->Rotation(Engine::ROTATION::ROT_Y, fAngle);

	pAi->Set_DownKey(INPUT_KEY::KEY_SHIFT);
	pAi->Set_DashDir(vDir);
	CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_DASH);

	m_vFirstPos[eID] = m_vInitialPos;
	m_vSecondPos[eID] = m_vInitialPos;
	m_vAvoidDir[eID] = m_vInitialPos;

	pAi->Set_StateState(STATETYPE::STATE_COMMON);
}

void CAiMgr::Capture(CBasePlayer * pAi, const _double & dTimeDelta)
{
	CONTROLTYPE eType = pAi->Get_ControlType();

	// 깃발에 다가가면 점령애니메이션
	if (CUIMgr::GetInstance()->Get_GetFlag() == false)
	{
		if (pAi->GetDist_ToFlag() < 150)
		{
			if (eType == CONTROLTYPE::CTRL_AI_ALLIANCE)
			{
				if (m_pPlayer->Get_CurState() == CBasePlayer::PLAYER_STATE::COMMON_CAPTUREFLAG_START || m_pPlayer->Get_CurState() == CBasePlayer::PLAYER_STATE::COMMON_CAPTUREFLAG_LOOP
					|| m_pPlayer->Get_CurState() == CBasePlayer::PLAYER_STATE::COMMON_CAPTUREFLAG_END)
				{
					//pAi->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
					pAi->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
					pAi->Set_StateState(STATETYPE::STATE_COMMON);
					return;
				}
			}

			dynamic_cast<CAi_Player*>(pAi)->Set_AnimationSpeed(1);
			pAi->Set_DownKey(KEY_Capture_Flag);
		}
		else // 깃발에서 좀 떨어져있을때
		{
			if (eType == CONTROLTYPE::CTRL_AI_ENERMY_1 || eType == CONTROLTYPE::CTRL_AI_ENERMY_2)
			{
				for (auto& rObj : m_listTeamRed)
				{
					// 자신이 아닌, 자기팀원일때
					if (rObj->Get_ControlType() != pAi->Get_ControlType())
					{
						if (rObj->Get_CurState() == CBasePlayer::PLAYER_STATE::COMMON_CAPTUREFLAG_START || rObj->Get_CurState() == CBasePlayer::PLAYER_STATE::COMMON_CAPTUREFLAG_LOOP
							|| rObj->Get_CurState() == CBasePlayer::PLAYER_STATE::COMMON_CAPTUREFLAG_END)
						{
							//pAi->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
							pAi->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
							pAi->Set_StateState(STATETYPE::STATE_COMMON);
						}
						else
						{
							pAi->Set_DownKey(KEY_A);
							pAi->Set_PressKey(KEY_A);
						}
					}
				}
			}
			else if (eType == CONTROLTYPE::CTRL_AI_ALLIANCE)
			{
				if (m_pPlayer->Get_CurState() == CBasePlayer::PLAYER_STATE::COMMON_CAPTUREFLAG_START || m_pPlayer->Get_CurState() == CBasePlayer::PLAYER_STATE::COMMON_CAPTUREFLAG_LOOP
					|| m_pPlayer->Get_CurState() == CBasePlayer::PLAYER_STATE::COMMON_CAPTUREFLAG_END)
				{
					//pAi->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
					pAi->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
					pAi->Set_StateState(STATETYPE::STATE_COMMON);
					return;
				}
				else
				{
					pAi->Set_DownKey(KEY_A);
					pAi->Set_PressKey(KEY_A);
				}
			}
		}
	}
}

void CAiMgr::Check_FlagArea_RedTeam(CBasePlayer* pAi)
{
	if (pAi->Get_Team() == TEAM::TEAM_RED)
	{
		// 주변안에 있는지 검색하고 적 둘다 깃발 주변에 있다면 그 중에 자신과 가까운 적으로 설정	
		int iCloseToFlagPlayer = 0;

		// 나중에 iCloseToFlagPlayer검사해서 2면 깃발 근처에 적이 2명있다는 의미니까 둘중에 가까운적을 찾아서 타겟으로 삼는 작업을 해야함
		for (auto& pBlue : m_listTeamBlue)
		{
			float fDist = Engine::GetDistNoY(pBlue->GetPlayerPos(), m_vFlagPos);
			if (fDist < 2000)
			{
				if (pBlue->Get_isDead() == true)
					continue;
				else
				{
					pAi->Set_TargetPlayer(pBlue);
					iCloseToFlagPlayer++;
				}
			}
		}

		// 적이 깃발 근처에 없음
		if (iCloseToFlagPlayer == 0)
		{
			pAi->Set_TargetPlayer(nullptr);
			return;
		}

		// 적 한명만 깃발 근처에 있으면 리턴
		if (iCloseToFlagPlayer == 1)
			return;

		CBasePlayer* pTarget1 = nullptr;
		CBasePlayer* pTarget2 = nullptr;
		float fDist1 = 0.f;
		float fDist2 = 0.f;

		// 적 두명이 다 깃발 근처에 있을 때 자신과 가까운 적을 타겟으로 삼음
		if (iCloseToFlagPlayer >= 2)
		{
			// %%%%%%%%%%%%%%%%% 가까운 적 검사 %%%%%%%%%%%%%%%%%%%%
			for (auto& pBlue : m_listTeamBlue)
			{
				if (fDist1 == 0)	// 첫번째 검사일때
				{
					fDist1 = Engine::GetDistNoY(pBlue->GetPlayerPos(), m_vFlagPos);
					pTarget1 = pBlue->Get_PlayerPointer();
				}
				else if (fDist2 == 0)				// 두번째 검사
				{
					fDist2 = Engine::GetDistNoY(pBlue->GetPlayerPos(), m_vFlagPos);
					pTarget2 = pBlue->Get_PlayerPointer();
				}
			}
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

			//%%%%%%%%%%%%%%% 가까운 적 타겟설정 %%%%%%%%%%%%%%%%%
			if (fDist1 < fDist2)
				pAi->Set_TargetPlayer(pTarget1);
			else if (fDist2 < fDist1)
				pAi->Set_TargetPlayer(pTarget2);
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

			// %%%%%%%%%%%% 팀과 같은 적을 타겟으로 삼는지 검사 및 대상변경 %%%%%%%%%
			for (auto& Team : m_listTeamRed)
			{
				// 현재 Ai와 다른 Ai일 경우
				if (pAi->Get_ControlType() != Team->Get_ControlType())
				{
					// 팀원과 같은 타겟일때
					if (pAi->Get_TargetPlayer() == Team->Get_TargetPlayer())
					{
						// 다른 타겟으로 바꿈
						if (pAi->Get_TargetPlayer() == pTarget1)
						{
							pAi->Set_TargetPlayer(pTarget2);

							//// %%%%%%%%%%%%%%%%%%%%% 디버깅 코드 %%%%%%%%%%%%%%%%%%%%%%%%%%
							//switch (pAi->Get_ControlType())
							//{
							//case CONTROLTYPE::CTRL_AI_ENERMY_1:
							//	switch (pTarget2->Get_ControlType())
							//	{
							//	case CONTROLTYPE::CTRL_AI_ALLIANCE:
							//		//cout << "[Enermy_1]은 Target을 Player에서 Alliance로 바꿨다" << endl;
							//		break;
							//	case CONTROLTYPE::CTRL_PLAYER:
							//		//cout << "[Enermy_1]은 Target을 Alliance에서 Player로 바꿨다" << endl;
							//		break;
							//	default:
							//		MessageBoxA(g_hWnd, "[Enermy_1]의 타겟이 default가 뜸", 0, MB_OK);
							//		break;
							//	}
							//	break;
							//case CONTROLTYPE::CTRL_AI_ENERMY_2:
							//	switch (pTarget2->Get_ControlType())
							//	{
							//	case CONTROLTYPE::CTRL_AI_ALLIANCE:
							//		//cout << "[Enermy_2]는 Target을 Player에서 Alliance로 바꿨다" << endl;
							//		break;
							//	case CONTROLTYPE::CTRL_PLAYER:
							//		//cout << "[Enermy_2]는 Target을 Alliance에서 Player로 바꿨다" << endl;
							//		break;
							//	default:
							//		MessageBoxA(g_hWnd, "[Enermy_2]의 타겟이 default가 뜸", 0, MB_OK);
							//		break;
							//	}
							//	break;
							//}
							//// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
						}
						else if (pAi->Get_TargetPlayer() == pTarget2)
						{
							pAi->Set_TargetPlayer(pTarget1);

							//// %%%%%%%%%%%%%%%%%%%%% 디버깅 코드 %%%%%%%%%%%%%%%%%%%%%%%%%%
							//switch (pAi->Get_ControlType())
							//{
							//case CONTROLTYPE::CTRL_AI_ENERMY_1:
							//	switch (pTarget1->Get_ControlType())
							//	{
							//	case CONTROLTYPE::CTRL_AI_ALLIANCE:
							//		//cout << "[Enermy_1]은 Target을 Player에서 Alliance로 바꿨다" << endl;
							//		break;
							//	case CONTROLTYPE::CTRL_PLAYER:
							//		//cout << "[Enermy_1]은 Target을 Alliance에서 Player로 바꿨다" << endl;
							//		break;
							//	default:
							//		MessageBoxA(g_hWnd, "[Enermy_1]의 타겟이 default가 뜸", 0, MB_OK);
							//		break;
							//	}
							//	break;
							//case CONTROLTYPE::CTRL_AI_ENERMY_2:
							//	switch (pTarget1->Get_ControlType())
							//	{
							//	case CONTROLTYPE::CTRL_AI_ALLIANCE:
							//		//cout << "[Enermy_2]는 Target을 Player에서 Alliance로 바꿨다" << endl;
							//		break;
							//	case CONTROLTYPE::CTRL_PLAYER:
							//		//cout << "[Enermy_2]는 Target을 Alliance에서 Player로 바꿨다" << endl;
							//		break;
							//	default:
							//		MessageBoxA(g_hWnd, "[Enermy_2]의 타겟이 default가 뜸", 0, MB_OK);
							//		break;
							//	}
							//	break;
							//}
							//// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
						}
					}
				}
			}
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
		}
	}
}

void CAiMgr::Check_FlagArea_BlueTeam(CBasePlayer * pAi)
{
	if (pAi->Get_Team() == TEAM::TEAM_BLUE)
	{
		// 주변안에 있는지 검색하고 적 둘다 깃발 주변에 있다면 그 중에 자신과 가까운 적으로 설정	
		int iCloseToFlagPlayer = 0;

		// 나중에 iCloseToFlagPlayer검사해서 2면 깃발 근처에 적이 2명있다는 의미니까 둘중에 가까운적을 찾아서 타겟으로 삼는 작업을 해야함

		for (auto& pRed : m_listTeamRed)
		{
			float fDist = Engine::GetDistNoY(pRed->GetPlayerPos(), m_vFlagPos);
			if (fDist < 2000)
			{
				if (pRed->Get_isDead() == true)
					continue;
				else
				{
					iCloseToFlagPlayer++;
					pAi->Set_TargetPlayer(pRed);
				}
			}
		}

		// 적이 깃발 근처에 없음
		if (iCloseToFlagPlayer == 0)
		{
			pAi->Set_TargetPlayer(nullptr);
			return;
		}

		// 적 한명만 깃발 근처에 있으면 리턴
		if (iCloseToFlagPlayer == 1)
			return;

		CBasePlayer* pTarget1 = nullptr;
		CBasePlayer* pTarget2 = nullptr;
		float fDist1 = 0.f;
		float fDist2 = 0.f;

		int iCheck = 0;

		// 적 두명이 다 깃발 근처에 있을 때
		if (iCloseToFlagPlayer >= 2)
		{
			// 적이 Alliance를 타겟으로  삼았을 경우
			for (auto& pRed : m_listTeamRed)
			{
				if (pAi->Get_PlayerPointer() == pRed->Get_TargetPlayer())
				{
					++iCheck;
					pAi->Set_TargetPlayer(pRed->Get_PlayerPointer());
				}
			}

			// 체크. 적 두명 다 Alliance를 타겟으로 삼았다
			//if (iCheck == 2)
			//	MessageBoxA(g_hWnd, "iCheck == 2", 0, MB_OK);


			// Alliance를 타겟으로 삼은 적이 없을경우 깃발점령
			if (iCheck == 0)
			{
				pAi->Set_TargetPlayer(nullptr);
			}
		}
	}
}

void CAiMgr::Check_CapturePlayer()
{
	for (auto& rAi : m_vecAllCharacter)
	{
		if (rAi->Get_CurState() == CBasePlayer::COMMON_CAPTUREFLAG_LOOP)
		{
			if (!rAi->IsReady_Timer(L"Captureing"))
			{
				rAi->Set_Timer(L"Captureing");
				// Ui 점령 표시 // 
				float fFlagPeriod = rAi->Get_DyanmicMesh()->Get_Period();
				CUIMgr::GetInstance()->CaptureingFlag(m_pGraphicDev, rAi->Get_Team(), (_double)fFlagPeriod);

				////cout << "점령중" << endl;
			}

			rAi->Start_Timer(L"Captureing");

			float fPeriod = rAi->Get_DyanmicMesh()->Get_Period();


			////cout << rAi->Get_DyanmicMesh()->Get_TrackTime() << endl;

			if (rAi->Get_DyanmicMesh()->Get_TrackTime() > fPeriod * 0.9)
			{
				rAi->Set_CaptureFlag(true);
				rAi->Delete_Timer(L"Captureing");

				////cout << "점령완료" << endl;

				switch (rAi->Get_ControlType())
				{
				case CONTROLTYPE::CTRL_PLAYER:
					CUIMgr::GetInstance()->CheckFlagScore(OBJID::OBJECT_PLAYER);
					break;
				case CONTROLTYPE::CTRL_AI_ALLIANCE:
					CUIMgr::GetInstance()->CheckFlagScore(OBJID::OBJECT_ALLIANCE);
					break;
				case CONTROLTYPE::CTRL_AI_ENERMY_1:
					CUIMgr::GetInstance()->CheckFlagScore(OBJID::OBJECT_ENEMY_1);
					break;
				case CONTROLTYPE::CTRL_AI_ENERMY_2:
					CUIMgr::GetInstance()->CheckFlagScore(OBJID::OBJECT_ENEMY_2);
					break;
				}
			}
		}
		else
		{
			rAi->Set_CaptureFlag(false);

			// 점령중이었으면 타이머 삭제하면서 점령취소 UI생성 
			if (rAi->Get_Timer(L"Captureing") > 0)
			{
				rAi->Delete_Timer(L"Captureing");
				CUIMgr::GetInstance()->CaptureFlagFailed();
			}
		}
	}
}

void CAiMgr::Check_TargetDead(CBasePlayer * pAi)
{
	if (pAi->Get_TargetPlayer() != nullptr)
	{
		if (pAi->Get_TargetPlayer()->Get_isDead())
			pAi->Set_TargetPlayer(nullptr);
	}
}

_bool CAiMgr::Check_Dead(CBasePlayer * pAi)
{
	if (pAi->Get_isDead())
	{
		pAi->Set_TargetPlayer(nullptr);
		pAi->Set_isRunToFlag(false);
		pAi->Clear_Timer();
		return true;
	}

	return false;
}

void CAiMgr::Check_Animation()
{
	for (auto& rObj : m_vecAiCharacter)
	{
		if (rObj->Get_MoveKey() != true)
		{
			if (rObj->Get_TargetPlayer() == nullptr)
			{
				if (rObj->Get_CurState() != CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT)
				{
					rObj->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
					rObj->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
					rObj->Set_StateState(STATETYPE::STATE_COMMON);
				}
			}
		}
	}
}

void CAiMgr::Debug()
{
	if (Engine::KeyDown(DIK_NUMPAD2))
	{
		for (auto& rObj : m_vecAiCharacter)
		{
			if (rObj->Get_ControlType() == CONTROLTYPE::CTRL_AI_ALLIANCE)
			{
				CBasePlayer* pTarget = rObj->Get_TargetPlayer();

				//cout << "CTRL_AI_ALLIANCE의 타겟주소 : " << pTarget << endl;

				if (pTarget == nullptr)
				{
					//cout << "CTRL_AI_ALLIANCE의 타겟 : nullptr" << endl;
				}
				else
				{
					switch (pTarget->Get_ControlType())
					{
					case CONTROLTYPE::CTRL_PLAYER:
						//cout << "CTRL_AI_ALLIANCE의 타겟 : PLAYER " << endl;
						break;
					case CONTROLTYPE::CTRL_AI_ALLIANCE:
						//cout << "CTRL_AI_ALLIANCE의 타겟 : ALLIANCE " << endl;
						break;
					case CONTROLTYPE::CTRL_AI_ENERMY_1:
						//cout << "CTRL_AI_ALLIANCE의 타겟 : ENEMY_1 " << endl;
						break;
					case CONTROLTYPE::CTRL_AI_ENERMY_2:
						//cout << "CTRL_AI_ALLIANCE의 타겟 : ENEMY_2 " << endl;
						break;
					default:
						//cout << "CTRL_AI_ALLIANCE의 타겟 : ??? " << endl;
						break;
					}
				}

				switch (rObj->Get_StateState())
				{
				case STATETYPE::STATE_COMMON:
					//cout << "CTRL_AI_ALLIANCE의 상태 : Common" << endl;
					break;
				case STATETYPE::STATE_ATTACK:
					//cout << "CTRL_AI_ALLIANCE의 상태 : Attack" << endl;
					break;
				case STATETYPE::STATE_CAPTURE_FLAG:
					//cout << "CTRL_AI_ALLIANCE의 상태 : capture_flag" << endl;
					break;
				case STATETYPE::STATE_AVOID:
					//cout << "CTRL_AI_ALLIANCE의 상태 : avoid" << endl;
					break;
				case STATETYPE::STATE_HIT:
					//cout << "CTRL_AI_ALLIANCE의 상태 : hit" << endl;
					break;
				case STATETYPE::STATE_MOVE_TO_BACKWARD:
					//cout << "CTRL_AI_ALLIANCE의 상태 : move to backward" << endl;
					break;
				default:
					//cout << "CTRL_AI_ALLIANCE의 상태 : ???" << endl;
					break;
				}
			}
			else if (rObj->Get_ControlType() == CONTROLTYPE::CTRL_AI_ENERMY_1)
			{
				CBasePlayer* pTarget = rObj->Get_TargetPlayer();

				//cout << "CTRL_AI_ENERMY_1의 타겟주소 : " << pTarget << endl;

				if (pTarget == nullptr)
				{
					//cout << "CTRL_AI_ENERMY_1의 타겟 : nullptr" << endl;
				}
				else
				{
					switch (pTarget->Get_ControlType())
					{
					case CONTROLTYPE::CTRL_PLAYER:
						//cout << "CTRL_AI_ENERMY_1의 타겟 : PLAYER " << endl;
						break;
					case CONTROLTYPE::CTRL_AI_ALLIANCE:
						//cout << "CTRL_AI_ENERMY_1의 타겟 : ALLIANCE " << endl;
						break;
					case CONTROLTYPE::CTRL_AI_ENERMY_1:
						//cout << "CTRL_AI_ENERMY_1의 타겟 : ENEMY_1 " << endl;
						break;
					case CONTROLTYPE::CTRL_AI_ENERMY_2:
						//cout << "CTRL_AI_ENERMY_1의 타겟 : ENEMY_2 " << endl;
						break;
					default:
						//cout << "CTRL_AI_ENERMY_1의 타겟 : ??? " << endl;
						break;
					}
				}

				switch (rObj->Get_StateState())
				{
				case STATETYPE::STATE_COMMON:
					//cout << "CTRL_AI_ENERMY_1의 상태 : Common" << endl;
					break;
				case STATETYPE::STATE_ATTACK:
					//cout << "CTRL_AI_ENERMY_1의 상태 : Attack" << endl;
					break;
				case STATETYPE::STATE_CAPTURE_FLAG:
					//cout << "CTRL_AI_ENERMY_1의 상태 : capture_flag" << endl;
					break;
				case STATETYPE::STATE_AVOID:
					//cout << "CTRL_AI_ENERMY_1의 상태 : avoid" << endl;
					break;
				case STATETYPE::STATE_HIT:
					//cout << "CTRL_AI_ENERMY_1의 상태 : hit" << endl;
					break;
				case STATETYPE::STATE_MOVE_TO_BACKWARD:
					//cout << "CTRL_AI_ENERMY_1의 상태 : move to backward" << endl;
					break;
				default:
					//cout << "CTRL_AI_ENERMY_1의 상태 : ???" << endl;
					break;
				}
			}
			else if (rObj->Get_ControlType() == CONTROLTYPE::CTRL_AI_ENERMY_2)
			{
				CBasePlayer* pTarget = rObj->Get_TargetPlayer();

				//cout << "CTRL_AI_ENERMY_2의 타겟주소 : " << pTarget << endl;

				if (pTarget == nullptr)
				{
					//cout << "CTRL_AI_ENERMY_2의 타겟 : nullptr" << endl;
				}
				else
				{
					switch (pTarget->Get_ControlType())
					{
					case CONTROLTYPE::CTRL_PLAYER:
						//cout << "CTRL_AI_ENERMY_2의 타겟 : PLAYER " << endl;
						break;
					case CONTROLTYPE::CTRL_AI_ALLIANCE:
						//cout << "CTRL_AI_ENERMY_2의 타겟 : ALLIANCE " << endl;
						break;
					case CONTROLTYPE::CTRL_AI_ENERMY_1:
						//cout << "CTRL_AI_ENERMY_2의 타겟 : ENEMY_1 " << endl;
						break;
					case CONTROLTYPE::CTRL_AI_ENERMY_2:
						//cout << "CTRL_AI_ENERMY_2의 타겟 : ENEMY_2 " << endl;
						break;
					default:
						//cout << "CTRL_AI_ENERMY_2의 타겟 : ??? " << endl;
						break;
					}
				}


				switch (rObj->Get_StateState())
				{
				case STATETYPE::STATE_COMMON:
					//cout << "CTRL_AI_ENERMY_2의 상태 : Common" << endl;
					break;
				case STATETYPE::STATE_ATTACK:
					//cout << "CTRL_AI_ENERMY_2의 상태 : Attack" << endl;
					break;
				case STATETYPE::STATE_CAPTURE_FLAG:
					//cout << "CTRL_AI_ENERMY_2의 상태 : capture_flag" << endl;
					break;
				case STATETYPE::STATE_AVOID:
					//cout << "CTRL_AI_ENERMY_2의 상태 : avoid" << endl;
					break;
				case STATETYPE::STATE_HIT:
					//cout << "CTRL_AI_ENERMY_2의 상태 : hit" << endl;
					break;
				case STATETYPE::STATE_MOVE_TO_BACKWARD:
					//cout << "CTRL_AI_ENERMY_2의 상태 : move to backward" << endl;
					break;
				default:
					//cout << "CTRL_AI_ENERMY_2의 상태 : ???" << endl;
					break;
				}
			}
		}
	}

	if (Engine::KeyDown(DIK_NUMPAD3))
		system("pause");
}

void CAiMgr::Cheat()
{
	if (Engine::KeyDown(DIK_NUMPAD4))
	{
		m_pAi_Enermy_1->Set_Dead(true);
		m_pAi_Enermy_1->Set_Hp(0);
		m_pAi_Enermy_1->Set_PrevHp(0);
		m_pAi_Enermy_2->Set_Dead(true);
		m_pAi_Enermy_2->Set_Hp(0);
		m_pAi_Enermy_2->Set_PrevHp(0);
	}

	if (Engine::KeyDown(DIK_NUMPAD5))
	{
		m_pAi_Alliance->Set_Dead(true);
		m_pAi_Alliance->Set_Hp(0);
		m_pAi_Alliance->Set_PrevHp(0);
	}
}

void CAiMgr::DS_BasicCombo(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_02)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_03)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_04)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBOEX2_END)
	{

	}
}

void CAiMgr::DS_BasicComboEx01(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_02)
	{
		pAi->Set_UpKey(KEY_RB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_03)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_04)
	{
		pAi->Set_UpKey(KEY_RB);
	}
}

void CAiMgr::DS_BasicComboEx02(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_02)
	{
		pAi->Set_UpKey(KEY_RB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_03)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_BASICCOMBO_04)
	{
		pAi->Set_PressKey(KEY_LB);

	}
}

void CAiMgr::DS_MouseRight(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_RB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::DS_SPECIALCOMBO_CHARGE)
	{
		pAi->Set_PressKey(KEY_RB);
	}
}

void CAiMgr::DS_Q(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_Q);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_Q);
	}
}

void CAiMgr::DS_E(CBasePlayer * pAi, const _double & dTimeDelta)
{
	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_E);
	}
	CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_E);
}

void CAiMgr::DS_R(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_R);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_R);
	}
}

void CAiMgr::DS_F(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_F);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_F);
	}
}

void CAiMgr::TS_BasicCombo(CBasePlayer* pAi, const _double& dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	// TwohandSword의 BasicCombo는 3까지있음
	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		////cout << "시작 TS_BasicCombo" << endl;
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_BASICCOMBO_02)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_BASICCOMBO_03)
	{
		float fPeriod = pAi->Get_DyanmicMesh()->Get_Period();

		if (pAi->Get_DyanmicMesh()->Get_TrackTime() > fPeriod * 0.5)
		{
			pAi->Set_StateState(STATETYPE::STATE_COMMON);
		}
	}
}

void CAiMgr::TS_BasicComboEx01(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_BASICCOMBO_02)
	{
		pAi->Set_UpKey(KEY_RB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_BASICCOMBOEX01)
	{
		float fPeriod = pAi->Get_DyanmicMesh()->Get_Period();

		if (pAi->Get_DyanmicMesh()->Get_TrackTime() > fPeriod * 0.5)
		{
			pAi->Set_StateState(STATETYPE::STATE_COMMON);
		}
	}
}

void CAiMgr::TS_BasicComboEx02(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		////cout << "시작 TS_BasicComboEx02" << endl;
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_BASICCOMBO_02)
	{
		pAi->Set_PressKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_BASICCOMBOEX02_START)
	{
		pAi->Set_PressKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_BASICCOMBOEX02_LOOP)
	{
		pAi->Set_PressKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_BASICCOMBOEX02_END)
	{
		float fPeriod = pAi->Get_DyanmicMesh()->Get_Period();

		if (pAi->Get_DyanmicMesh()->Get_TrackTime() > fPeriod * 0.5)
		{
			pAi->Set_StateState(STATETYPE::STATE_COMMON);
		}
	}
}

void CAiMgr::TS_MouseRight(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_RB);
	}
}

void CAiMgr::TS_Q(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_Q);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_Q);
	}
	//if (pAi->Get_CurState() == CBasePlayer::TS_RAGESKILL_ARMAGEDDONBLADE)
	//{
	//	float fPeriod = pAi->Get_DyanmicMesh()->Get_Period();

	//	if (pAi->Get_DyanmicMesh()->Get_TrackTime() > fPeriod * 0.7)
	//	{
	//		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_Q);
	//	}
	//}
}

void CAiMgr::TS_E(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	// 거리 체크
	if (pAi->GetDist_ToEnermy() > 200)
	{
		pAi->Set_PressKey(KEY_W);
		pAi->Set_DownKey(KEY_W);
		return;
	}

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_E);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_E);
	}
	//if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_OUTRAGE_START)
	//{
	//	float fPeriod = pAi->Get_DyanmicMesh()->Get_Period();

	//	if (pAi->Get_DyanmicMesh()->Get_TrackTime() > fPeriod * 0.7)
	//	{
	//		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_E);
	//	}
	//}
}

void CAiMgr::TS_R(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_R);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_LEAP_ATTACK_READY)
	{
		pAi->Set_DownKey(KEY_LB);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_R);
		switch (pAi->Get_ControlType())
		{
		case CONTROLTYPE::CTRL_AI_ALLIANCE:
			CSoundMgr::Get_Instance()->AiSoundOn(59);
			break;
		case CONTROLTYPE::CTRL_AI_ENERMY_1:
			CSoundMgr::Get_Instance()->AiSoundOn(60);
			break;
		case CONTROLTYPE::CTRL_AI_ENERMY_2:
			CSoundMgr::Get_Instance()->AiSoundOn(61);
			break;
		}
	}
}

void CAiMgr::TS_F(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_F);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_F);
	}
	//if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::TS_ROCKSHOT)
	//{
	//	float fPeriod = pAi->Get_DyanmicMesh()->Get_Period();

	//	if (pAi->Get_DyanmicMesh()->Get_TrackTime() > fPeriod * 0.7)
	//	{
	//		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_F);
	//	}
	//}
}

void CAiMgr::LB_BasicCombo(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	pAi->Set_LookDir_ToTarget(dTimeDelta);
	pAi->Set_isSetLookDir(false);

	// LongBow의 BasicCombo는 3까지있음
	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::LB_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::LB_BASICCOMBO_02)
	{
		pAi->Set_UpKey(KEY_LB);

	}
}

void CAiMgr::LB_BasicComboEx(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	pAi->Set_LookDir_ToTarget(dTimeDelta);
	pAi->Set_isSetLookDir(false);

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::LB_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::LB_BASICCOMBO_02)
	{
		pAi->Set_PressKey(KEY_LB);
	}
	//	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::LB_BASICCOMBOEX_START)
	{

	}
}

void CAiMgr::LB_BasicCombo_PiercingArrow(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	pAi->Set_LookDir_ToTarget(dTimeDelta);
	pAi->Set_isSetLookDir(false);

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::LB_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::LB_BASICCOMBO_02)
	{
		pAi->Set_DownKey(KEY_RB);
	}
	//if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::LB_SKILL_PIERCINGARROW_SHOT)
	//{

	//}
}

void CAiMgr::LB_LBRB(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::LB_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_RB);
	}
}

void CAiMgr::LB_Q(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_Q);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_Q);
	}
}

void CAiMgr::LB_E(CBasePlayer * pAi, const _double & dTimeDelta)
{
	//// 방향 검사
	//if (false == pAi->CheckLookDir(dTimeDelta))
	//	return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_E);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::LB_SKILL_BINDINGSHOT_READY)
	{
		pAi->Set_DownKey(KEY_RB);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_E);
	}
}

void CAiMgr::LB_R(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_R);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_R);
		switch (pAi->Get_ControlType())
		{
		case CONTROLTYPE::CTRL_AI_ALLIANCE:
			CSoundMgr::Get_Instance()->AiSoundOn(59);
			break;
		case CONTROLTYPE::CTRL_AI_ENERMY_1:
			CSoundMgr::Get_Instance()->AiSoundOn(60);
			break;
		case CONTROLTYPE::CTRL_AI_ENERMY_2:
			CSoundMgr::Get_Instance()->AiSoundOn(61);
			break;
		}
	}
}

void CAiMgr::LB_F(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_F);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_F);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::LB_SKILL_SIEGEPOSITION_LOOP)
	{
		if (pAi->GetDist_ToEnermy() > 1200)
		{
			pAi->Set_ActionEnd(true);
			pAi->Set_StateState(STATE_COMMON);
			pAi->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
			pAi->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
			return;
		}

		pAi->Set_LookDir_ToTarget(dTimeDelta);
		pAi->Set_DownKey(KEY_LB);
		pAi->Set_isCheckLookDir(false);
		pAi->Set_isSetLookDir(false);
	}
}

void CAiMgr::OB_BasicCombo(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	pAi->Set_LookDir_ToTarget(dTimeDelta);
	pAi->Set_isSetLookDir(false);

	// Orb의 BasicCombo는 5까지있음
	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_02)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_03)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_04)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_05)
	{

	}
}

void CAiMgr::OB_BasicComboEx01(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	pAi->Set_LookDir_ToTarget(dTimeDelta);
	pAi->Set_isSetLookDir(false);


	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{

		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_02)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_03)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_04)
	{
		pAi->Set_DownKey(KEY_RB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBOEX1)
	{

	}
}

void CAiMgr::OB_BasicComboEx02(CBasePlayer * pAi, const _double & dTimeDelta)
{
	// 방향 검사
	if (false == pAi->CheckLookDir(dTimeDelta))
		return;

	pAi->Set_LookDir_ToTarget(dTimeDelta);
	pAi->Set_isSetLookDir(false);


	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_01)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_02)
	{
		pAi->Set_DownKey(KEY_LB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBO_03)
	{
		pAi->Set_DownKey(KEY_RB);
	}
	if (pAi->Get_CurState() == CBasePlayer::PLAYER_STATE::OB_BASICCOMBOEX2_01_F)
	{

	}
}

void CAiMgr::OB_MouseRight(CBasePlayer * pAi, const _double & dTimeDelta)
{
	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_RB);
	}
}

void CAiMgr::OB_Q(CBasePlayer * pAi, const _double & dTimeDelta)
{
	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_Q);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_Q);
	}
}

void CAiMgr::OB_E(CBasePlayer * pAi, const _double & dTimeDelta)
{
	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_E);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_E);
	}
}

void CAiMgr::OB_R(CBasePlayer * pAi, const _double & dTimeDelta)
{
	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_R);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_R);
	}
}

void CAiMgr::OB_F(CBasePlayer * pAi, const _double & dTimeDelta)
{
	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_COMMON)
	{
		pAi->Set_DownKey(KEY_F);
		CAiMgr::GetInstance()->Set_StartSkillCoolDown(pAi, SKILL_F);
	}
}

void CAiMgr::View_State(CBasePlayer* pAi, const _double& dTimeDelta)
{
	switch (pAi->Get_ControlType())
	{
	case CONTROLTYPE::CTRL_AI_ALLIANCE:
		if (pAi->Get_TargetPlayer() == nullptr)
			cout << "[Alliance]는 타겟이 없습니다." << endl;
		else
		{
			switch (pAi->Get_TargetPlayer()->Get_ControlType())
			{
			case CONTROLTYPE::CTRL_AI_ENERMY_1:
				//cout << " [Alliance]의 타겟은 Enermy_1 " << endl;
				break;
			case CONTROLTYPE::CTRL_AI_ENERMY_2:
				//cout << " [Alliance]의 타겟은 Enermy_2 " << endl;
				break;
			default:
				//cout << " [Alliance]의 타겟 ?? " << endl;
				break;
			}
		}
		break;
	case CONTROLTYPE::CTRL_AI_ENERMY_1:
		if (pAi->Get_TargetPlayer() == nullptr)
			cout << "[Enermy_1]은 타겟이 없습니다." << endl;
		else
		{
			switch (pAi->Get_TargetPlayer()->Get_ControlType())
			{
			case CONTROLTYPE::CTRL_AI_ALLIANCE:
				//cout << " [Enermy_1]의 타겟은 Alliance " << endl;
				break;
			case CONTROLTYPE::CTRL_PLAYER:
				//cout << " [Enermy_1]의 타겟은 Player " << endl;
				break;
			default:
				//cout << " [Enermy_1]의 타겟 ?? " << endl;
				break;
			}
		}
		break;
	case CONTROLTYPE::CTRL_AI_ENERMY_2:
		if (pAi->Get_TargetPlayer() == nullptr)
			cout << "[Enermy_2]는 타겟이 없습니다." << endl;
		else
		{
			switch (pAi->Get_TargetPlayer()->Get_ControlType())
			{
			case CONTROLTYPE::CTRL_AI_ALLIANCE:
				//cout << " [Enermy_2]의 타겟은 Alliance " << endl;
				break;
			case CONTROLTYPE::CTRL_PLAYER:
				//cout << " [Enermy_2]의 타겟은 Player " << endl;
				break;
			default:
				//cout << " [Enermy_2]의 타겟 ?? " << endl;
				break;
			}
		}
		break;
	}
}

void CAiMgr::Update_Skill_Cooldown(const _double & dTimeDelta)
{
	for (int i = 0; i < AI_TYPE::AI_END; i++)
	{
		auto iter = m_map_Skill_Cooldown[i].begin();

		for (; iter != m_map_Skill_Cooldown[i].end(); )
		{
			iter->second += dTimeDelta;

			switch (iter->first)
			{
			case AI_SKILL::SKILL_Q:
				if (iter->second > 60)
				{
					iter = m_map_Skill_Cooldown[i].erase(iter);
					////cout << "Q스킬 쿨타임 완료" << endl;
				}
				else
					++iter;
				break;
			case AI_SKILL::SKILL_E:
				if (iter->second > 20)
				{
					iter = m_map_Skill_Cooldown[i].erase(iter);
					////cout << "E스킬 쿨타임 완료" << endl;
				}
				else
					++iter;
				break;
			case AI_SKILL::SKILL_R:
				if (iter->second > 20)
				{
					iter = m_map_Skill_Cooldown[i].erase(iter);
					////cout << "R스킬 쿨타임 완료" << endl;
				}
				else
					++iter;
				break;
			case AI_SKILL::SKILL_F:
				if (iter->second > 20)
				{
					iter = m_map_Skill_Cooldown[i].erase(iter);
					////cout << "F스킬 쿨타임 완료" << endl;
				}
				else
					++iter;
				break;
			case AI_SKILL::SKILL_DASH:
				if (iter->second > 0.5)
				{
					iter = m_map_Skill_Cooldown[i].erase(iter);
				}
				else
					++iter;
				break;
			}
		}

		//for (auto& pAi : m_map_Skill_Cooldown[i])
		//{
		//	pAi.second += dTimeDelta;

		//	switch (pAi.first)
		//	{
		//	case AI_SKILL::SKILL_Q:
		//		if (pAi.second > 40)
		//		{
		//			auto& rSkill = m_map_Skill_Cooldown[i].find(pAi.first);
		//			m_map_Skill_Cooldown[i].erase(rSkill);
		//		}
		//		break;
		//	case AI_SKILL::SKILL_E:
		//		if (pAi.second > 15)
		//		{
		//			auto& rSkill = m_map_Skill_Cooldown[i].find(pAi.first);
		//			m_map_Skill_Cooldown[i].erase(rSkill);
		//		}
		//		break;
		//	case AI_SKILL::SKILL_R:
		//		if (pAi.second > 15)
		//		{
		//			auto& rSkill = m_map_Skill_Cooldown[i].find(pAi.first);
		//			m_map_Skill_Cooldown[i].erase(rSkill);
		//		}
		//		break;
		//	case AI_SKILL::SKILL_F:
		//		if (pAi.second > 15)
		//		{
		//			auto& rSkill = m_map_Skill_Cooldown[i].find(pAi.first);
		//			m_map_Skill_Cooldown[i].erase(rSkill);
		//		}
		//		break;
		//	case AI_SKILL::SKILL_DASH:
		//		if (pAi.second > 3)
		//		{
		//			auto& rSkill = m_map_Skill_Cooldown[i].find(pAi.first);
		//			m_map_Skill_Cooldown[i].erase(rSkill);
		//		}
		//		break;
		//	}
		//}
	}
}

_bool CAiMgr::Check_Effect(CBasePlayer* pAi)
{
	AI_TYPE eType = Get_AiType(pAi);
	OBJID	eID = Get_OBJID(pAi);

	// 맞는중이면 리턴
	if (pAi->Get_CurStateID() == CBasePlayer::STATE_ID::ID_HEART)
		return false;

	if (pAi->Get_StateState() == STATETYPE::STATE_ATTACK)
		return false;

	if (m_map_Skill_Cooldown[eType].end() == m_map_Skill_Cooldown[eType].find(AI_SKILL::SKILL_DASH))
	{
		for (_uint i = OBJID::OBJECT_PLAYER; i < OBJID::OBJECT_MONSTER; ++i)
		{
			for (auto& rObj : m_list_Effect_Storage[i])
			{
				if (eID == OBJID::OBJECT_ALLIANCE)
				{
					if (i == OBJID::OBJECT_PLAYER || i == OBJID::OBJECT_ALLIANCE)
						continue;
				}
				else if (eID == OBJID::OBJECT_ENEMY_1 || eID == OBJID::OBJECT_ENEMY_2)
				{
					if (i == OBJID::OBJECT_ENEMY_1 || i == OBJID::OBJECT_ENEMY_2)
						continue;
				}

				float fDis = Engine::GetDistNoY(rObj->Get_Info_byWorld(Engine::INFO_POS), pAi->GetPlayerPos());

				if (fDis < 300)
				{
					// Collider와 거리가 가까우면 Dir을 구하기 위해 두개의 Pos를 저장한다.
					if (m_vFirstPos[eID] == m_vInitialPos)
						m_vFirstPos[eID] = rObj->Get_Info_byWorld(Engine::INFO_POS);
					else if (m_vSecondPos[eID] == m_vInitialPos)
						m_vSecondPos[eID] = rObj->Get_Info_byWorld(Engine::INFO_POS);

					if ((m_vFirstPos[eID] != m_vInitialPos) && (m_vSecondPos[eID] != m_vInitialPos))
					{
						// Dir이 없는 공격일때
						if (m_vFirstPos[eID] == m_vSecondPos[eID])
						{
							_vec3 vTargetPos;

							//float fDist = Engine::GetDistNoY(m_vFirstPos[eID], pAi->GetPlayerPos());
							////cout << "Dist : " << fDist << endl;
							////cout << "First : " << m_vFirstPos[eID].x << "/" << m_vFirstPos[eID].y << "/" << m_vFirstPos[eID] << endl;
							////cout << "Second : " << m_vSecondPos[eID].x << "/" << m_vSecondPos[eID].y << "/" << m_vSecondPos[eID] << endl;
							////cout << " %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << endl;

							switch (i)
							{
							case OBJID::OBJECT_PLAYER:
								vTargetPos = m_pPlayer->GetPlayerPos();
								break;
							case OBJID::OBJECT_ALLIANCE:
								vTargetPos = m_pAi_Alliance->GetPlayerPos();
								break;
							case OBJID::OBJECT_ENEMY_1:
								vTargetPos = m_pAi_Enermy_1->GetPlayerPos();
								break;
							case OBJID::OBJECT_ENEMY_2:
								vTargetPos = m_pAi_Enermy_2->GetPlayerPos();
								break;
							}

							_vec3 vAiToEnemy = Engine::GetDirNoY(vTargetPos, pAi->GetPlayerPos());
							D3DXVec3Normalize(&vAiToEnemy, &vAiToEnemy);
							vAiToEnemy *= -1.f;

							// 뒤로 이동한다.
							m_vAvoidDir[eID] = vAiToEnemy;

							return true;
						}
						// Dir이 있는 공격일때
						else if (m_vFirstPos[eID] != m_vSecondPos[eID])
						{
							_vec3 vTargetPos;
							switch (i)
							{
							case OBJID::OBJECT_PLAYER:
								vTargetPos = m_pPlayer->GetPlayerPos();
								break;
							case OBJID::OBJECT_ALLIANCE:
								vTargetPos = m_pAi_Alliance->GetPlayerPos();
								break;
							case OBJID::OBJECT_ENEMY_1:
								vTargetPos = m_pAi_Enermy_1->GetPlayerPos();
								break;
							case OBJID::OBJECT_ENEMY_2:
								vTargetPos = m_pAi_Enermy_2->GetPlayerPos();
								break;
							}
							_vec3 vColliderDir = Engine::GetDirNoY(m_vSecondPos[eID], m_vFirstPos[eID]);
							_vec3 vAiToEnemy = Engine::GetDirNoY(vTargetPos, pAi->GetPlayerPos());
							D3DXVec3Normalize(&vColliderDir, &vColliderDir);
							D3DXVec3Normalize(&vAiToEnemy, &vAiToEnemy);

							// 법선벡터 1과 2를 생성
							_vec3 vNormal_1 = { -vAiToEnemy.z, 0, vAiToEnemy.x };
							_vec3 vNormal_2 = vNormal_1 * -1.f;
							D3DXVec3Normalize(&vNormal_1, &vNormal_1);
							D3DXVec3Normalize(&vNormal_2, &vNormal_2);

							// 두 법선벡터와 Collider의 진행방향과 내적한다.
							float fAngle_1 = Engine::GetAngleY(vColliderDir, vNormal_1);
							float fAngle_2 = Engine::GetAngleY(vColliderDir, vNormal_2);

							fAngle_1 = fabsf(fAngle_1);
							fAngle_2 = fabsf(fAngle_2);

							//float fTest_Angle1 = Engine::GetAngleY(vObjToCollider, vNormal_1);
							//float fTest_Angle2 = Engine::GetAngleY(vObjToCollider, vNormal_2);

							//// 법선벡터 확인
							////cout << fTest_Angle1 << endl;
							////cout << fTest_Angle2 << endl;


							// 큰값으로 이동한다.
							if (fAngle_1 > fAngle_2)
							{
								m_vAvoidDir[eID] = vNormal_1;
								////cout << "fAngle1 : " << fAngle_1 << "/ fAngle2 : " << fAngle_2 << endl;
								////cout << "fAngle1" << endl;
							}
							else
							{
								m_vAvoidDir[eID] = vNormal_2;
								////cout << "fAngle2 : " << fAngle_2 << "/ fAngle1 : " << fAngle_1 << endl;
								////cout << "fAngle2" << endl;
							}
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void CAiMgr::Update_FlagPos()
{
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Set_FlagPos(m_vFlagPos);
	}
	if (m_pAi_Alliance != nullptr)
	{
		m_pAi_Alliance->Set_FlagPos(m_vFlagPos);
	}
	if (m_pAi_Enermy_1 != nullptr)
	{
		m_pAi_Enermy_1->Set_FlagPos(m_vFlagPos);
	}
	if (m_pAi_Enermy_2 != nullptr)
	{
		m_pAi_Enermy_2->Set_FlagPos(m_vFlagPos);
	}
}

void CAiMgr::Reset_Key()
{
	if (m_pAi_Alliance != nullptr)
	{
		dynamic_cast<CAi_Player*>(m_pAi_Alliance)->Get_AiState()->Set_DownKey(KEY_END);
		dynamic_cast<CAi_Player*>(m_pAi_Alliance)->Get_AiState()->Set_UpKey(KEY_END);
		dynamic_cast<CAi_Player*>(m_pAi_Alliance)->Get_AiState()->Set_PressKey(KEY_END);
	}
	if (m_pAi_Enermy_1 != nullptr)
	{
		dynamic_cast<CAi_Player*>(m_pAi_Enermy_1)->Get_AiState()->Set_DownKey(KEY_END);
		dynamic_cast<CAi_Player*>(m_pAi_Enermy_1)->Get_AiState()->Set_UpKey(KEY_END);
		dynamic_cast<CAi_Player*>(m_pAi_Enermy_1)->Get_AiState()->Set_PressKey(KEY_END);
	}
	if (m_pAi_Enermy_2 != nullptr)
	{
		dynamic_cast<CAi_Player*>(m_pAi_Enermy_2)->Get_AiState()->Set_DownKey(KEY_END);
		dynamic_cast<CAi_Player*>(m_pAi_Enermy_2)->Get_AiState()->Set_UpKey(KEY_END);
		dynamic_cast<CAi_Player*>(m_pAi_Enermy_2)->Get_AiState()->Set_PressKey(KEY_END);
	}
}

void CAiMgr::Clear_All()
{
	m_mapTimer.clear();

	for (auto iter = m_vecAiCharacter.begin(); iter != m_vecAiCharacter.end(); ++iter)
	{
		Engine::Safe_Release(*iter);
	}

	m_vecAiCharacter.clear();
	m_vecAllCharacter.clear();
	m_listTeamBlue.clear();
	m_listTeamRed.clear();
	for (_uint i = 0; i < OBJID::OBJECT_MONSTER; ++i)
		m_list_Effect_Storage[i].clear();

	m_pPlayer = nullptr;
	m_pAi_Alliance = nullptr;
	m_pAi_Enermy_1 = nullptr;
	m_pAi_Enermy_2 = nullptr;
}
