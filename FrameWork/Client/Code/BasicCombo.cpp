#include "stdafx.h"
#include "BasicCombo.h"
#include "OrbCore.h"
#include "BasePlayer.h"
CBasicCombo::CBasicCombo()
	:m_bEnter(false),
	m_bIsSkill(false),
	m_bNextAtk(false),
	m_dbMotionCancel(0),
	m_dbCharging(0),
	m_eEndType(TYPE_END)
{
}

CBasicCombo::~CBasicCombo()
{
}

void CBasicCombo::Enter(CPlayer * pPlayer)
{
}

void CBasicCombo::Update(CPlayer * pPlayer, const _double dTimeDelta)
{
}

void CBasicCombo::ChangeState(CPlayerState * pState)
{
}

void CBasicCombo::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float * fCrossy, _vec3 * vDir, _float * fDegree)
{
}

void CBasicCombo::Enter_State(CBasePlayer* pPlayer)
{
	CPlayerState::Enter_State(pPlayer);
	m_pPlayer->Set_BattleType(CPlayer::TYPE_BATTLE);
	m_pPlayer->Get_Renderer()->Set_RadialBlur(false);
	switch (m_eWeapon)
	{
	case CBasePlayer::ORB:
	{
		if (m_bRotation)
		{
			_int iEnemyType = m_pPlayer->Check_Enemy();
			if (iEnemyType == 0)
				RotationCam();
			else
				RotationTarget_Range(iEnemyType);
			m_bRotation = false;
		}
	}
	break;
	case CBasePlayer::LONGBOW:
	{
		if (m_bRotation)
		{
			_int iEnemyType = m_pPlayer->Check_Enemy();
			if (iEnemyType == 0)
				RotationCam();
			else
				RotationTarget_Range(iEnemyType);
			m_bRotation = false;
		}
	}
	break;
	default:
		break;
	}
}

void CBasicCombo::Update_State(const _double dTimeDelta)
{
	Update_PlayerPattern(dTimeDelta);
}

CPlayer::STATE_ID CBasicCombo::LateUpadte_State(const _double dTimeDelta)
{
	if (Engine::KeyDown(DIK_LSHIFT))
	{
		m_pPlayer->Set_DashReserve();
		return CPlayer::ID_COMMON;
	}

	/*if (m_pPlayer->Get_Dash())
	{
		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
			
	}*/

	m_dbMotionCancel += dTimeDelta;

	switch (m_eEndType)
	{
	case CBasicCombo::TYPE_RESERVE:
	{
		//if (m_pMeshCom->Is_AnimationSetFinish(0.2))

		if (!m_bEnter)
			m_bEnter = true;
		if (m_eWeapon == CPlayer::TWOHANDSWORD)
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

		else if (m_eWeapon == CPlayer::ORB)
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

		else if (m_eWeapon == CPlayer::DUALSWORD)
		{
			if (m_pPlayer->Get_CurState() == CPlayer::DS_BASICCOMBO_04)
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
	case CBasicCombo::TYPE_CHANGE:
	{
		if (m_eWeapon == CPlayer::TWOHANDSWORD)
		{
			if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.21)
			{
				if (!m_bIsSkill)
					return CPlayer::ID_BASICCOMBOEX;
			}
		}

			else if (m_eWeapon == CPlayer::DUALSWORD)
			{
				if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.5)
				{
					if (!m_bIsSkill)
						return CPlayer::ID_BASICCOMBOEX;
				}
			}
			else if (m_eWeapon == CPlayer::ORB)
			{
				if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.5)
				{
					if (!m_bIsSkill)
						return CPlayer::ID_BASICCOMBOEX;
				}
			}
			else if (m_eWeapon == CPlayer::LONGBOW)
			{
				if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.5)
				{
					if (!m_bIsSkill)
						return CPlayer::ID_BASICCOMBOEX;
				}
			}
			else
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					if (!m_bIsSkill)
						return CPlayer::ID_BASICCOMBOEX;
				}
			}
		}
		break;
		case CBasicCombo::TYPE_END:
		{
			if (m_pPlayer->Get_CurState() == CPlayer::DS_BASICCOMBO_01)
			{
				if (m_bEffect)
				{
					OBJID eID = m_pPlayer->Get_BaseInfo()->eObjectID;
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL1, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bEffect = false;
					CSoundMgr::Get_Instance()->SoundOn(43);
				}

				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
				{
					if (m_bDoubleEffect)
					{
						OBJID eID = m_pPlayer->Get_BaseInfo()->eObjectID;
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bDoubleEffect = false;
					}

					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
				}
			}
			else if (m_pPlayer->Get_CurState() == CPlayer::OB_BASICCOMBO_01)
			{
				if (m_bEffect)
				{
					_vec3 vOffSet = _vec3(0.f, 50.f, 0.f);
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					D3DXVec3Normalize(&vLook, &vLook);

					vPos += vOffSet;
					Engine::CGameObject* pGameObject = nullptr;
					if (Get_FindTarget())
					{
						_vec3 vTargetLook = Engine::GetDirNoY(m_vTargetPos, vPos);
						D3DXVec3Normalize(&vTargetLook, &vTargetLook);
						pGameObject = COrbCore::Create(Engine::Get_Device(), COrbCore::ORB_NORMAL, &vPos, m_pPlayer->Get_TransformCom()->Get_Angle(), &vLook, 1, m_pPlayer->Get_ControlType());
					}
					else
						pGameObject = COrbCore::Create(Engine::Get_Device(), COrbCore::ORB_NORMAL, &vPos, m_pPlayer->Get_TransformCom()->Get_Angle(), &vLook, 1, m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);
					m_bEffect = false;

					CSoundMgr::Get_Instance()->SoundOn(65);
				}
			}
			else if (m_pPlayer->Get_CurState() == CPlayer::LB_BASICCOMBO_01)
			{
				if (m_bEffect)
				{
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					_vec3 vPos = m_pPlayer->GetPlayerPos();

					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
					if (pGameObject == nullptr)
						return CPlayer::ID_COMMON;
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bEffect = false;

					_uint iSound = rand() % 6 + 57;
					CSoundMgr::Get_Instance()->SoundOn(iSound);

				}
			}
			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_pPlayer->Set_AnimationSpeed(1);
				WeaponDissolve();
				return CPlayer::ID_COMMON;
			}
		}
		break;
		default:
			break;
		}

	return CPlayer::ID_BASICCOMBO;
}


void CBasicCombo::Update_PlayerPattern(const _double dTimeDelta)
{
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
		case CBasePlayer::COMMON:
			break;
		case CBasePlayer::DUALSWORD:
		{
			if (m_bRotation)
			{
				RotationTarget();
				m_bRotation = false;
			}
		}
		break;
		case CBasePlayer::TWOHANDSWORD:
		{
			if (m_bRotation)
			{
				RotationTarget();
				m_bRotation = false;
			}
		}
		break;
		default:
			break;
		}

		switch (m_eWeapon)
		{
		case CPlayer::COMMON:
			//ø©±‚ ø¿∏È ∏¡«‘
			break;
		case CPlayer::DUALSWORD:
		{			
			Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_R", Engine::MESH, Engine::ID_STATIC));
			pWeaponCom->Set_AnimationSet(0);

			pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_L", Engine::MESH, Engine::ID_STATIC));
			pWeaponCom->Set_AnimationSet(0);

			CDualSword* pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_L"));
			pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

			pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_R"));
			pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

			m_pMeshCom->Set_AnimationSet(CPlayer::DS_BASICCOMBO_01);
			m_pPlayer->Set_State(CPlayer::DS_BASICCOMBO_01);

			/*if (m_bEffect)
			{
				OBJID eID = m_pPlayer->Get_BaseInfo()->eObjectID;
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL1, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bEffect = false;
			}

			if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
			{
				if (m_bDoubleEffect)
				{
					OBJID eID = m_pPlayer->Get_BaseInfo()->eObjectID;
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bDoubleEffect = false;
				}

				_vec3 vPos = m_pPlayer->GetPlayerPos();
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
			}*/

			if (Engine::MouseDown(Engine::DIM_LB))
			{
				m_bDoubleEffect = true;
				m_eEndType = TYPE_RESERVE;
				m_eReserveState = CPlayer::DS_BASICCOMBO_02;
			}
		}
		break;
		case CPlayer::TWOHANDSWORD:
		{
			//»Ò¡§ √ﬂ∞° - 2¡Ÿ
			CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));
			pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);

			m_pMeshCom->Set_AnimationSet(CPlayer::TS_BASICCOMBO_01);
			m_pPlayer->Set_State(CPlayer::TS_BASICCOMBO_01);

			////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
			if (m_bEffect)
			{
				//π´±‚- ±√±ÿ±‚ off
				if(CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				//π´±‚- ±√±ÿ±‚ on
				else if(CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

				CSoundMgr::Get_Instance()->SoundOn(50);

				m_bEffect = false;
			}
			/////////////////////////////

			if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
			{
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
			}

			if (Engine::MouseDown(Engine::DIM_LB))
			{
				Set_Effect();
				m_eEndType = TYPE_RESERVE;
				m_eReserveState = CPlayer::TS_BASICCOMBO_02;
			}
		}
		break;
		case CPlayer::ORB:
		{
			if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.21)
			{
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex)));
			}
			
			m_pMeshCom->Set_AnimationSet(CPlayer::OB_BASICCOMBO_01);
			m_pPlayer->Set_State(CPlayer::OB_BASICCOMBO_01);
			m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBO01);
			{
				if (Engine::MouseDown(Engine::DIM_LB))
				{
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CPlayer::OB_BASICCOMBO_02;
				}
			}
		}
		break;
		case CPlayer::LONGBOW:
		{
			Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"LB", Engine::MESH, Engine::ID_STATIC));
			pWeaponCom->Set_AnimationSet(1);

			CLongBow* pWeapon = dynamic_cast<CLongBow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"LB"));
			pWeapon->Set_CombatState(CLongBow::STATE_COMBAT);

			m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBO_01);
			m_pPlayer->Set_State(CPlayer::LB_BASICCOMBO_01);
			m_pPlayer->Set_AnimationSpeed(1.5);			

			{
				if (Engine::MouseDown(Engine::DIM_LB))
				{
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CPlayer::LB_BASICCOMBO_02;
				}

				if (Engine::MouseDown(Engine::DIM_RB))
				{
					m_bIsSkill = false;
					//m_bNextAtk = true;

					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CPlayer::LEVEL1);
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
		case CPlayer::COMMON:
			//ø©±‚ ø¿∏È ∏¡«‘
			break;
		case CPlayer::DUALSWORD:
		{
			if (CPlayer::DS_BASICCOMBO_02 == m_pPlayer->Get_CurState())
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
					CSoundMgr::Get_Instance()->SoundOn(44);
				}

				if (Engine::MouseDown(Engine::DIM_LB))
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CPlayer::DS_BASICCOMBO_03;
				}
				if (!m_bNextAtk)
					m_eEndType = TYPE_END;
			}

			if (CPlayer::DS_BASICCOMBO_03 == m_pPlayer->Get_CurState())
			{
				if (m_bEffect)
				{
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_SPEAR, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bEffect = false;
					CSoundMgr::Get_Instance()->SoundOn(45);
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

				if (Engine::MouseDown(Engine::DIM_LB))
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CPlayer::DS_BASICCOMBO_04;
				}
				if (!m_bNextAtk)
					m_eEndType = TYPE_END;
			}

			if (CPlayer::DS_BASICCOMBO_04 == m_pPlayer->Get_CurState())
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
						CSoundMgr::Get_Instance()->SoundOn(46);
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
						CSoundMgr::Get_Instance()->SoundOn(47);

					}

					if (!m_bEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL8, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bEffect = true;
					}
				}

				if (Engine::MouseDown(Engine::DIM_LB))
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CPlayer::DS_BASICCOMBOEX2_START;
				}

				if (Engine::MousePressing(Engine::DIM_LB))
				{
					m_bIsSkill = false;
					m_bNextAtk = true;
					m_dbCharging += dTimeDelta;
					if (0.5f < m_dbCharging)
					{
						m_eEndType = TYPE_CHANGE;
						m_pPlayer->Set_AtkLevel(CPlayer::LEVEL2);
					}
				}

				if (Engine::MouseUp(Engine::DIM_RB))
				{
					m_bNextAtk = true;
					m_bIsSkill = false;
					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CPlayer::LEVEL1);
					CSoundMgr::Get_Instance()->SoundOn(48);
					//m_eReserveState = CPlayer::TS_BASICCOMBOEX01;
				}

				if (!m_bNextAtk)
					m_eEndType = TYPE_END;
			}

			if (CPlayer::DS_BASICCOMBOEX2_START == m_pPlayer->Get_CurState())
			{
				m_eEndType = TYPE_RESERVE;
				m_eReserveState = CPlayer::DS_BASICCOMBOEX2_END;
			}

			if (CPlayer::DS_BASICCOMBOEX2_END == m_pPlayer->Get_CurState())
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.1)
				{
					if (m_bEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TEST, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bEffect = false;
						CSoundMgr::Get_Instance()->SoundOn(49);
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
		case CPlayer::TWOHANDSWORD:
		{
			if (CPlayer::TS_BASICCOMBO_02 == m_pPlayer->Get_CurState())
			{
				//»Ò¡§ √ﬂ∞° - 2¡Ÿ
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));
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
						CSoundMgr::Get_Instance()->SoundOn(51);
					}
					/////////////////////////////
				}

				if (Engine::MousePressing(Engine::DIM_LB))
				{
					//Set_Effect();
					m_bIsSkill = false;
					m_bNextAtk = true;
					m_dbCharging += dTimeDelta;
					if (0.5f < m_dbCharging)
					{
						m_eEndType = TYPE_CHANGE;
						m_pPlayer->Set_AtkLevel(CPlayer::LEVEL2);
					}
				}

				if (Engine::MouseDown(Engine::DIM_LB))
				{
					//Set_Effect();
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CPlayer::TS_BASICCOMBO_03;
				}

				if (Engine::MouseUp(Engine::DIM_RB))
				{
					m_bNextAtk = true;
					m_bIsSkill = false;
					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CPlayer::LEVEL1);
					//m_eReserveState = CPlayer::TS_BASICCOMBOEX01;
				}

				if (!m_bNextAtk)
					m_eEndType = TYPE_END;

			}

			if (CPlayer::TS_BASICCOMBO_03 == m_pPlayer->Get_CurState())
			{
				//»Ò¡§ √ﬂ∞° - 2¡Ÿ
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));
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
						CSoundMgr::Get_Instance()->SoundOn(52);
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
		case CPlayer::ORB:
		{
			if (CPlayer::OB_BASICCOMBO_02 == m_pPlayer->Get_CurState())
			{
				if (m_bEffect)
				{
					_vec3 vOffSet = _vec3(0.f, 50.f, 0.f);
					vPos += vOffSet;
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = COrbCore::Create(Engine::Get_Device(), COrbCore::ORB_NORMAL, &vPos, m_pPlayer->Get_TransformCom()->Get_Angle(), &vLook, 1, m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);
					m_bEffect = false;
					CSoundMgr::Get_Instance()->SoundOn(66);
				}
				m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBO02);
				if (Engine::MouseDown(Engine::DIM_LB))
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CPlayer::OB_BASICCOMBO_03;
				}
			}

			if (CPlayer::OB_BASICCOMBO_03 == m_pPlayer->Get_CurState())
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
					CSoundMgr::Get_Instance()->SoundOn(67);
				}
				if (Engine::MouseDown(Engine::DIM_LB))
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CPlayer::OB_BASICCOMBO_04;
				}
				if (Engine::MouseDown(Engine::DIM_RB))
				{
					m_bNextAtk = true;
					m_bIsSkill = false;
					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CPlayer::LEVEL3);
				}
			}

			if (CPlayer::OB_BASICCOMBO_04 == m_pPlayer->Get_CurState())
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
					CSoundMgr::Get_Instance()->SoundOn(68);
				}
				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 500 * (_float)dTimeDelta), m_dwNaviIndex)));
				}

				m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBO04);
				if (Engine::MouseDown(Engine::DIM_LB))
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_RESERVE;
					m_eReserveState = CPlayer::OB_BASICCOMBO_05;
				}

				if (Engine::MouseDown(Engine::DIM_RB))
				{
					m_bNextAtk = true;
					m_bIsSkill = false;
					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CPlayer::LEVEL4);
				}
			}
			if (CPlayer::OB_BASICCOMBO_05 == m_pPlayer->Get_CurState())
			{
				if (m_bEffect)
				{
					_vec3 vOffSet = _vec3(0.f, 50.f, 0.f);
					vPos += vOffSet;
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = COrbCore::Create(Engine::Get_Device(), COrbCore::ORB_BIG, &vPos, m_pPlayer->Get_TransformCom()->Get_Angle(), &vLook, 5, m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);

					m_bEffect = false;
					CSoundMgr::Get_Instance()->SoundOn(71);
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
		case CPlayer::LONGBOW:
		{
			if (CPlayer::LB_BASICCOMBO_02 == m_pPlayer->Get_CurState())
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
					_uint iSound = rand() % 6 + 57;
					CSoundMgr::Get_Instance()->SoundOn(iSound);
				}
				
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.2)
				{
					if (Engine::MouseUp(Engine::DIM_LB))
					{
						m_bNextAtk = true;
						m_eEndType = TYPE_RESERVE;
						m_eReserveState = CPlayer::LB_BASICCOMBO_03;

					}
				}
				if (Engine::MousePressing(Engine::DIM_LB))
				{
					m_bIsSkill = false;
					m_bNextAtk = true;
					m_dbCharging += dTimeDelta;
					if (0.5f < m_dbCharging)
					{
						m_eEndType = TYPE_CHANGE;
						m_pPlayer->Set_AtkLevel(CPlayer::LEVEL2);
					}
				}

				if (Engine::MouseDown(Engine::DIM_RB))
				{
					m_bNextAtk = true;
					m_eEndType = TYPE_CHANGE;
					m_pPlayer->Set_AtkLevel(CPlayer::LEVEL3);
				}

				if (!m_bNextAtk)
					m_eEndType = TYPE_END;

			}

			if (CPlayer::LB_BASICCOMBO_03 == m_pPlayer->Get_CurState())
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

						_uint iSound = rand() % 6 + 57;
						CSoundMgr::Get_Instance()->SoundOn(iSound);
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
