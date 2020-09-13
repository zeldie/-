#include "stdafx.h"
#include "Ai_BasicCombo.h"
#include "OrbCore.h"
CAi_BasicCombo::CAi_BasicCombo()
	:m_bEnter(false),
	m_bIsSkill(false),
	m_bNextAtk(false),
	m_dbMotionCancel(0),
	//m_dbCharging(0),
	m_eEndType(TYPE_END)
{
}

CAi_BasicCombo::~CAi_BasicCombo()
{
}

void CAi_BasicCombo::Enter(CAi_Player * pPlayer)
{
}

void CAi_BasicCombo::Update(CAi_Player * pPlayer, const _double dTimeDelta)
{
}

void CAi_BasicCombo::ChangeState(CAi_PlayerState * pState)
{
}

void CAi_BasicCombo::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float * fCrossy, _vec3 * vDir, _float * fDegree)
{
}

void CAi_BasicCombo::Enter_State(CBasePlayer* pPlayer)
{
	CAi_PlayerState::Enter_State(pPlayer);
}

void CAi_BasicCombo::Update_State(const _double dTimeDelta)
{
	Update_PlayerPattern(dTimeDelta);
}

CAi_Player::STATE_ID CAi_BasicCombo::LateUpadte_State(const _double dTimeDelta)
{
	if (m_pPlayer->Get_isDead())
		return CAi_Player::ID_COMMON;

	if (m_pPlayer->Get_TargetPlayer() == nullptr)
	{
		m_eEndType = TYPE_END;
		m_pPlayer->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
		m_pPlayer->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
		//m_pPlayer->Set_StateState(STATETYPE::STATE_COMMON);
		return CAi_Player::ID_COMMON;
	}

	if (m_eDown_Key == KEY_SHIFT)
	{
		m_pPlayer->Set_DashReserve();
	}


	m_dbMotionCancel += dTimeDelta;

	switch (m_eEndType)
	{
	case CAi_BasicCombo::TYPE_RESERVE:
	{
		//if (m_pMeshCom->Is_AnimationSetFinish(0.2))

		if (!m_bEnter)
			m_bEnter = true;
		if (m_eWeapon == CAi_Player::TWOHANDSWORD)
		{
			if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.21)
			{
				Set_Effect();
				//m_bDoubleEffect = true;
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				m_pPlayer->Set_State(m_eReserveState);
				m_dbMotionCancel = 0;
				m_eEndType = TYPE_END;
			}
		}

		else if (m_eWeapon == CAi_Player::ORB)
		{
			if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.3)
			{
				Set_Effect();
				//m_bDoubleEffect = true;
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				m_pPlayer->Set_State(m_eReserveState);
				m_dbMotionCancel = 0;
				m_eEndType = TYPE_END;
			}
		}

		else if (m_eWeapon == CAi_Player::DUALSWORD)
		{
			if (m_pPlayer->Get_CurState() == CAi_Player::DS_BASICCOMBO_04)
			{
				if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.7)
				{
					Set_Effect();
					m_bDoubleEffect = true;
					m_pMeshCom->Set_AnimationSet(m_eReserveState);
					m_pPlayer->Set_State(m_eReserveState);
					m_dbMotionCancel = 0;
					m_eEndType = TYPE_END;
				}

			}
			else if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.3)
			{
				Set_Effect();
				m_bDoubleEffect = true;
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				m_pPlayer->Set_State(m_eReserveState);
				m_dbMotionCancel = 0;
				m_eEndType = TYPE_END;
			}
		}

		else
		{
			if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.5)
			{
				Set_Effect();
				m_bDoubleEffect = true;
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				m_pPlayer->Set_State(m_eReserveState);
				m_dbMotionCancel = 0;
				m_eEndType = TYPE_END;
			}
		}
		//else
		//{
		//	if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		//	{
		//		m_pMeshCom->Set_AnimationSet(m_eReserveState);
		//		m_pPlayer->Set_State(m_eReserveState);
		//		m_dbMotionCancel = 0;
		//		m_eEndType = TYPE_END;
		//	}
		//}
	}
	break;
	case CAi_BasicCombo::TYPE_CHANGE:
	{
		if (m_eWeapon == CAi_Player::TWOHANDSWORD)
		{
			if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.21)
			{
				if (!m_bIsSkill)
					return CAi_Player::ID_BASICCOMBOEX;
			}
		}

			else if (m_eWeapon == CAi_Player::DUALSWORD)
			{
				if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.5)
				{
					if (!m_bIsSkill)
						return CAi_Player::ID_BASICCOMBOEX;
				}
			}
			else if (m_eWeapon == CAi_Player::ORB)
			{
				if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.5)
				{
					if (!m_bIsSkill)
						return CAi_Player::ID_BASICCOMBOEX;
				}
			}
			else if (m_eWeapon == CAi_Player::LONGBOW)
			{
				if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.5)
				{
					if (!m_bIsSkill)
						return CAi_Player::ID_BASICCOMBOEX;
				}
			}
			else
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					if (!m_bIsSkill)
						return CAi_Player::ID_BASICCOMBOEX;
				}
			}
		}
		break;
		case CAi_BasicCombo::TYPE_END:
		{
			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_pPlayer->Set_AnimationSpeed(1);
				WeaponDissolve();
				return CAi_Player::ID_COMMON;
			}
		}
		break;
		default:
			break;
		}

	return CAi_Player::ID_BASICCOMBO;
}


void CAi_BasicCombo::Update_PlayerPattern(const _double dTimeDelta)
{
	if (m_pPlayer->Get_isDead())
		return;

	if (m_pPlayer->Get_TargetPlayer() == nullptr)
	{
		m_eEndType = TYPE_END;
		m_pPlayer->Get_DyanmicMesh()->Set_AnimationSet(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
		m_pPlayer->Set_CurState(CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT);
		m_pPlayer->Set_StateState(STATETYPE::STATE_COMMON);
		return;
	}

	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);
	_vec3 vUp = *m_pTransform->Get_Info(Engine::INFO_UP);
	D3DXVec3Normalize(&vUp, &vUp);
	_vec3 vPos = m_pPlayer->GetPlayerPos();

	if (!m_bEnter) // √π ∞¯∞›
	{
		switch (m_eWeapon)
		{
		case CAi_Player::COMMON:
			//ø©±‚ ø¿∏È ∏¡«‘
			break;
		case CAi_Player::DUALSWORD:
		{
			Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_pPlayer->Get_NameMainWeapon(), Engine::MESH, Engine::ID_STATIC));
			pWeaponCom->Set_AnimationSet(0);

			pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_pPlayer->Get_NameDualSword_L(), Engine::MESH, Engine::ID_STATIC));
			pWeaponCom->Set_AnimationSet(0);

			CDualSword* pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_pPlayer->Get_NameDualSword_L()));
			pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

			m_pMeshCom->Set_AnimationSet(CAi_Player::DS_BASICCOMBO_01);
			m_pPlayer->Set_State(CAi_Player::DS_BASICCOMBO_01);

			if (m_bEffect)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL1, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bEffect = false;
			}

			if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
			{
				if (m_bDoubleEffect)
				{
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bDoubleEffect = false;
				}

				_vec3 vPos = m_pPlayer->GetPlayerPos();
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
			}

			if (m_eDown_Key == KEY_LB)
			{
				m_bDoubleEffect = true;
				m_eEndType = TYPE_RESERVE;
				m_eReserveState = CAi_Player::DS_BASICCOMBO_02;
			}
		}
		break;
		case CAi_Player::TWOHANDSWORD:
		{
			//»Ò¡§ √ﬂ∞° - 2¡Ÿ
			CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, m_pPlayer->Get_NameMainWeapon()));
			pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);

			m_pMeshCom->Set_AnimationSet(CAi_Player::TS_BASICCOMBO_01);
			m_pPlayer->Set_State(CAi_Player::TS_BASICCOMBO_01);

			////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
			if (m_bEffect)
			{
				//π´±‚- ±√±ÿ±‚ off
				if(CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				//π´±‚- ±√±ÿ±‚ on
				else if(CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bEffect = false;
			}
			/////////////////////////////

			if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
			{
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
			}

			if (m_eDown_Key == KEY_LB)
			{
				Set_Effect();
				m_eEndType = TYPE_RESERVE;
				m_eReserveState = CAi_Player::TS_BASICCOMBO_02;
			}
		}
		break;
		case CAi_Player::ORB:
		{
			if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.21)
			{
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
			}
			if (m_bEffect)
			{
				_vec3 vOffSet = _vec3(0.f, 50.f, 0.f);
				vPos += vOffSet;
				Engine::CGameObject* pGameObject = nullptr;
				pGameObject = COrbCore::Create(Engine::Get_Device(), COrbCore::ORB_NORMAL, &vPos,m_pPlayer->Get_TransformCom()->Get_Angle(), &vLook, 1, m_pPlayer->Get_ControlType());
				Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);
				m_bEffect = false;
			}
			m_pMeshCom->Set_AnimationSet(CAi_Player::OB_BASICCOMBO_01);
			m_pPlayer->Set_State(CAi_Player::OB_BASICCOMBO_01);
			m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBO01);
			{
				if (m_eDown_Key == KEY_LB)
				{
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CAi_Player::OB_BASICCOMBO_02;
				}
			}
		}
		break;
		case CAi_Player::LONGBOW:
		{
			m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBO_01);
			m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBO_01);
			m_pPlayer->Set_AnimationSpeed(1.5);
			if (m_bEffect)
			{
				Engine::CGameObject* pGameObject = nullptr;
				pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
				if (pGameObject == nullptr)
					return;
				Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bEffect = false;
			}

			{
				if (m_eDown_Key == KEY_LB)
				{
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CAi_Player::LB_BASICCOMBO_02;
				}

				if (m_eDown_Key == KEY_RB)
				{
					m_bIsSkill = false;
					//m_bNextAtk = true;

					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CAi_Player::LEVEL1);
				}
			}
		}
		break;
		default:
			break;
		}
	}



	else
	{
		switch (m_eWeapon)
		{
		case CAi_Player::COMMON:
			//ø©±‚ ø¿∏È ∏¡«‘
			break;
		case CAi_Player::DUALSWORD:
		{
			if (CAi_Player::DS_BASICCOMBO_02 == m_pPlayer->Get_CurState())
			{
				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.2)
				{
					if (m_bDoubleEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL4, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bDoubleEffect = false;
					}

					/*_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));*/
				}
				//m_eEndType = TYPE_RESERVE;
				if (m_bEffect)
				{
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL3, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bEffect = false;
				}

				if (m_eDown_Key == KEY_LB)
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CAi_Player::DS_BASICCOMBO_03;
				}
				if (!m_bNextAtk)
					m_eEndType = TYPE_END;
			}

			if (CAi_Player::DS_BASICCOMBO_03 == m_pPlayer->Get_CurState())
			{
				if (m_bEffect)
				{
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_SPEAR, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bEffect = false;
				}

				if (m_pMeshCom->Get_TrackTime() > m_pMeshCom->Get_Period()*0.01)
				{
					if (m_bDoubleEffect)
					{
						m_bDoubleEffect = false;
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_SPEAR2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					}
				}
				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
				}
				//m_eEndType = TYPE_RESERVE;

				if (m_eDown_Key == KEY_LB)
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CAi_Player::DS_BASICCOMBO_04;
				}
				if (!m_bNextAtk)
					m_eEndType = TYPE_END;
			}

			if (CAi_Player::DS_BASICCOMBO_04 == m_pPlayer->Get_CurState())
			{
				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.2)
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));

					if (m_bDoubleEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL6, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bDoubleEffect = false;
					}

					if (m_bEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL5, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bEffect = false;
					}
				}
				//m_eEndType = TYPE_RESERVE;

				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.41)
				{
					if (!m_bDoubleEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL7, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bDoubleEffect = true;
					}

					if (!m_bEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL8, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bEffect = true;
					}
				}

				if (m_eDown_Key == KEY_LB)
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CAi_Player::DS_BASICCOMBOEX2_START;
				}

				if (m_ePress_Key == KEY_LB)
				{
					m_bIsSkill = false;
					m_bNextAtk = true;
					//m_dbCharging += dTimeDelta;
					//if (0.5f < m_dbCharging)
					//{
						m_eEndType = TYPE_CHANGE;
						m_pPlayer->Set_AtkLevel(CAi_Player::LEVEL2);
					//}
				}

				if (m_eUp_Key == KEY_RB)
				{
					m_bNextAtk = true;
					m_bIsSkill = false;
					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CAi_Player::LEVEL1);
					//m_eReserveState = CAi_Player::TS_BASICCOMBOEX01;
				}

				if (!m_bNextAtk)
					m_eEndType = TYPE_END;
			}

			if (CAi_Player::DS_BASICCOMBOEX2_START == m_pPlayer->Get_CurState())
			{
				m_eEndType = TYPE_RESERVE;
				m_eReserveState = CAi_Player::DS_BASICCOMBOEX2_END;
			}

			if (CAi_Player::DS_BASICCOMBOEX2_END == m_pPlayer->Get_CurState())
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.1)
				{
					if (m_bEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TEST, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bEffect = false;
					}

				}
				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
				}
			}
		}
		break;
		case CAi_Player::TWOHANDSWORD:
		{
			if (CAi_Player::TS_BASICCOMBO_02 == m_pPlayer->Get_CurState())
			{
				//»Ò¡§ √ﬂ∞° - 2¡Ÿ
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, m_pPlayer->Get_NameMainWeapon()));
				pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);

				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));

					////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
					if (m_bEffect)
					{
						//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_SECOND, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						
						
						//π´±‚- ±√±ÿ±‚ off
						if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
							m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_SECOND, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						//π´±‚- ±√±ÿ±‚ on
						else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
							m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_SECOND_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						
						m_bEffect = false;
					}
					/////////////////////////////
				}

				if (m_ePress_Key == KEY_LB)
				{
					//Set_Effect();
					m_bIsSkill = false;
					m_bNextAtk = true;
					//m_dbCharging += dTimeDelta;
					//if (0.5f < m_dbCharging)
					//{
						m_eEndType = TYPE_CHANGE;
						m_pPlayer->Set_AtkLevel(CAi_Player::LEVEL2);
					//}
				}

				if (m_eDown_Key == KEY_LB)
				{
					//Set_Effect();
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CAi_Player::TS_BASICCOMBO_03;
				}

				if (m_eUp_Key == KEY_RB)
				{
					m_bNextAtk = true;
					m_bIsSkill = false;
					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CAi_Player::LEVEL1);
					//m_eReserveState = CAi_Player::TS_BASICCOMBOEX01;
				}

				if (!m_bNextAtk)
					m_eEndType = TYPE_END;

			}

			if (CAi_Player::TS_BASICCOMBO_03 == m_pPlayer->Get_CurState())
			{
				//»Ò¡§ √ﬂ∞° - 2¡Ÿ
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, m_pPlayer->Get_NameMainWeapon()));
				pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);

				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));

					////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
					if (m_bEffect)
					{
						//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_THIRD1, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						
						//π´±‚- ±√±ÿ±‚ off
						if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
							m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_THIRD1, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						//π´±‚- ±√±ÿ±‚ on
						else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
							m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_THIRD1_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

						m_bEffect = false;
					}
					/////////////////////////////

				}

				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.3)
				{
					////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
					if (m_bDoubleEffect)
					{
						//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_THIRD2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						
						//π´±‚- ±√±ÿ±‚ off
						if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
							m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_THIRD2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						//π´±‚- ±√±ÿ±‚ on
						else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
							m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_THIRD2_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

						m_bDoubleEffect = false;
					}
					/////////////////////////////


					m_eEndType = TYPE_RESERVE;

					if (m_pMeshCom->Is_AnimationSetFinish(0.2))
					{
						m_eEndType = TYPE_END;
					}

				}
			}
			break;
		}
		case CAi_Player::ORB:
		{
			if (CAi_Player::OB_BASICCOMBO_02 == m_pPlayer->Get_CurState())
			{
				if (m_bEffect)
				{
					_vec3 vOffSet = _vec3(0.f, 50.f, 0.f);
					vPos += vOffSet;
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = COrbCore::Create(Engine::Get_Device(), COrbCore::ORB_NORMAL, &vPos, m_pPlayer->Get_TransformCom()->Get_Angle(), &vLook, 1, m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);
					m_bEffect = false;
				}
				m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBO02);
				if (m_eDown_Key == KEY_LB)
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CAi_Player::OB_BASICCOMBO_03;
				}
			}

			if (CAi_Player::OB_BASICCOMBO_03 == m_pPlayer->Get_CurState())
			{
				m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBO03);
				if (m_bEffect)
				{
					_vec3 vOffSet = _vec3(0.f, 50.f, 0.f);
					_vec3 vLeftOrbPos = vPos + vOffSet - vUp * 20.f;
					_vec3 vRightOrbPos = vPos + vOffSet + vUp * 20.f;
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = COrbCore::Create(Engine::Get_Device(), COrbCore::ORB_SPIN_FRONT_RIGHT, &vLeftOrbPos, m_pPlayer->Get_TransformCom()->Get_Angle(), &vLook, 1, m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);
					pGameObject = COrbCore::Create(Engine::Get_Device(), COrbCore::ORB_SPIN_FRONT_LEFT, &vRightOrbPos, m_pPlayer->Get_TransformCom()->Get_Angle(), &vLook, 1, m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);
					m_bEffect = false;
				}
				if (m_eDown_Key == KEY_LB)
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CAi_Player::OB_BASICCOMBO_04;
				}
				if (m_eDown_Key == KEY_RB)
				{
					m_bNextAtk = true;
					m_bIsSkill = false;
					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CAi_Player::LEVEL3);
				}
			}

			if (CAi_Player::OB_BASICCOMBO_04 == m_pPlayer->Get_CurState())
			{
				if (m_bEffect)
				{
					_vec3 vOffSet = _vec3(0.f, 50.f, 0.f);
					_vec3 vLeftOrbPos = vPos + vOffSet - vUp * 20.f;
					_vec3 vRightOrbPos = vPos + vOffSet + vUp * 20.f;
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = COrbCore::Create(Engine::Get_Device(), COrbCore::ORB_SPIN_FRONT_RIGHT, &vLeftOrbPos, m_pPlayer->Get_TransformCom()->Get_Angle(), &vLook, 1, m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);
					pGameObject = COrbCore::Create(Engine::Get_Device(), COrbCore::ORB_SPIN_FRONT_LEFT, &vRightOrbPos, m_pPlayer->Get_TransformCom()->Get_Angle(), &vLook, 1, m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);
					m_bEffect = false;
				}
				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 500 * (_float)dTimeDelta), m_dwNaviIndex)));
				}

				m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBO04);
				if (m_eDown_Key == KEY_LB)
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CAi_Player::OB_BASICCOMBO_05;
				}

				if (m_eDown_Key == KEY_RB)
				{
					m_bNextAtk = true;
					m_bIsSkill = false;
					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CAi_Player::LEVEL4);
				}
			}
			if (CAi_Player::OB_BASICCOMBO_05 == m_pPlayer->Get_CurState())
			{
				if (m_bEffect)
				{
					_vec3 vOffSet = _vec3(0.f, 50.f, 0.f);
					vPos += vOffSet;
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = COrbCore::Create(Engine::Get_Device(), COrbCore::ORB_BIG, &vPos, m_pPlayer->Get_TransformCom()->Get_Angle(), &vLook, 5, m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);

					m_bEffect = false;
				}
				if (m_pMeshCom->Get_TrackTime() > m_pMeshCom->Get_Period()*0.1 &&m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vLook * 750 * (_float)dTimeDelta), m_dwNaviIndex)));
				}
				m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBO05);
			}
		}
		break;
		case CAi_Player::LONGBOW:
		{
			if (CAi_Player::LB_BASICCOMBO_02 == m_pPlayer->Get_CurState())
			{
				m_pPlayer->Set_AnimationSpeed(1.5);
				if (m_bEffect)
				{
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
					if (pGameObject == nullptr)
						return;
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bEffect = false;
				}
				
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.2)
				{
					if (m_eUp_Key == KEY_LB)
					{
						m_bNextAtk = true;
						m_eEndType = TYPE_RESERVE;
						m_eReserveState = CAi_Player::LB_BASICCOMBO_03;

					}
				}
				if (m_ePress_Key == KEY_LB)
				{
					m_bIsSkill = false;
					m_bNextAtk = true;
					//m_dbCharging += dTimeDelta;
					//if (0.5f < m_dbCharging)
					//{
						m_eEndType = TYPE_CHANGE;
						m_pPlayer->Set_AtkLevel(CAi_Player::LEVEL2);
					//}
				}

				if (m_eDown_Key == KEY_RB)
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CAi_Player::LEVEL3);
				}

				if (!m_bNextAtk)
					m_eEndType = TYPE_END;

			}

			if (CAi_Player::LB_BASICCOMBO_03 == m_pPlayer->Get_CurState())
			{
				m_pPlayer->Set_AnimationSpeed(1.5);
				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.75)
				{
					if (m_bEffect)
					{
						Engine::CGameObject* pGameObject = nullptr;
						pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
						if (pGameObject == nullptr)
							return;
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bEffect = false;
					}
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vLook * 350 * (_float)dTimeDelta), m_dwNaviIndex)));
				}
			}
		}
		break;
		default:
			break;
		}
	}
}
