#include "stdafx.h"
#include "Ai_BasicComboEx.h"

CAi_BasicComboEx::CAi_BasicComboEx()
	:m_bEnter(false),
	m_eEndType(TYPE_END),
	m_eLevel(CAi_Player::LEVEL_END),
	m_bReserveFinish(false),
	m_bPass(false),
	m_dbHoldingTime(0),
	m_dbCharge(0),
	m_dbAcc(0),
	m_iCount(0),
	m_eStep(STEP1)
{
}

CAi_BasicComboEx::~CAi_BasicComboEx()
{
}

void CAi_BasicComboEx::Enter(CAi_Player * pPlayer)
{
}

void CAi_BasicComboEx::Update(CAi_Player * pPlayer, const _double dTimeDelta)
{
}

void CAi_BasicComboEx::ChangeState(CAi_PlayerState * pState)
{
}

void CAi_BasicComboEx::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float * fCrossy, _vec3 * vDir, _float * fDegree)
{
}

void CAi_BasicComboEx::Enter_State(CBasePlayer * pPlayer)
{
	CAi_PlayerState::Enter_State(pPlayer);
	m_eLevel = m_pPlayer->Get_AtkLevel();
}

void CAi_BasicComboEx::Update_State(const _double dTimeDelta)
{
	Update_PlayerPattern(dTimeDelta);
}

CAi_Player::STATE_ID CAi_BasicComboEx::LateUpadte_State(const _double dTimeDelta)
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

	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);
	_vec3 vPos = m_pPlayer->GetPlayerPos();
	
	switch (m_eEndType)
	{
	case CAi_BasicComboEx::TYPE_LOOP:
	{
		return CAi_Player::ID_BASICCOMBOEX;
	}
	break;
	case CAi_BasicComboEx::TYPE_RESERVE:
	{
		if (!m_bEnter)
			m_bEnter = true;

		if (m_eWeapon == CAi_Player::TWOHANDSWORD)
		{
			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				m_pPlayer->Set_State(m_eReserveState);
				m_eEndType = TYPE_END;
			}
		}
		else if (m_eWeapon == CAi_Player::DUALSWORD)
		{
			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				m_pPlayer->Set_State(m_eReserveState);
				m_eEndType = TYPE_END;
			}
		}
		else if (m_eWeapon == CAi_Player::LONGBOW)
		{
			/*if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				switch (m_eStep)
				{
				case CAi_BasicComboEx::STEP1:
					break;
				case CAi_BasicComboEx::STEP2:
				{
					m_eEndType = TYPE_RESERVE;
					m_eStep = STEP3;
				}
				break;
				case CAi_BasicComboEx::STEP3:
				{
					m_eEndType = TYPE_RESERVE;
					m_eStep = STEP4;
				}
				break;
				case CAi_BasicComboEx::STEP4:
				{
					m_eEndType = TYPE_RESERVE;
					m_eStep = STEP_END;
				}
				break;
				case CAi_BasicComboEx::STEP_END:
					m_eEndType = TYPE_END;
					break;
				default:
					break;
				}
			}*/
		}

		else if (m_eWeapon == CAi_Player::ORB)
		{

			if (m_pPlayer->Get_CurState() == CAi_Player::COMMON_COMBATWAIT)
			{
				////cout << m_pMeshCom->Get_Period() << endl;
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.1)
				{
					WeaponDissolve();
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::OB_BASICCOMBOEX2_01_F)
			{
				//∆Ú∆Ú∆Ú øÏ≈¨∏Ø
				_matrix matView = m_pPlayer->Get_ViewMatrix();
				_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
				_vec3 vUp = *m_pTransform->Get_Info(Engine::INFO_UP);
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				_vec3 vAngle = *m_pTransform->Get_Angle();
				D3DXMatrixInverse(&matView, 0, &matView);
				D3DXVec3Normalize(&vLook, &vLook);
				D3DXVec3Normalize(&vRight, &vRight);
				D3DXVec3Normalize(&vUp, &vUp);

				_float fCrossy, fDegree;

				vPurpose = { matView.m[2][0],matView.m[2][1] ,matView.m[2][2] };

				CAi_PlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

				if (!isnan(fDegree))
				{
					if (fCrossy > 0.f)
						m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
				}

				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.7)
				{
					if (m_bEffect && m_pMeshCom->Get_TrackTime() > m_pMeshCom->Get_Period()*0.4)
					{
						_vec3 vOrbCreatorPos1 = vPos + vRight * 100.f + vUp * 50.f;
						_vec3 vOrbCreatorPos2 = vPos - vRight * 100.f + vUp * 50.f;
						_vec3 vOrbCreatorAngle1 = vAngle - _vec3(0.f, 10.f, 0.f);
						_vec3 vOrbCreatorAngle2 = vAngle + _vec3(0.f, 10.f, 0.f);
						m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR1, &vOrbCreatorPos1, &vOrbCreatorAngle1);
						m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR2, &vOrbCreatorPos1, &vOrbCreatorAngle1);
						m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR1, &vOrbCreatorPos2, &vOrbCreatorAngle2);
						m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR2, &vOrbCreatorPos2, &vOrbCreatorAngle2);
						m_bEffect = false;
					}
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vPurpose * 500 * (_float)dTimeDelta), m_dwNaviIndex)));
				}


				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pMeshCom->Set_AnimationSet(m_eReserveState);
					m_pPlayer->Set_State(m_eReserveState);
					m_eEndType = TYPE_RESERVE;
					return CAi_Player::ID_BASICCOMBOEX;
				}
			}

			else  if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				m_pPlayer->Set_State(m_eReserveState);
				m_eEndType = TYPE_END;
			}
		}
	}
	break;
	case CAi_BasicComboEx::TYPE_END:
	{
		if (m_eWeapon == CAi_Player::TWOHANDSWORD)
		{
			CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, m_pPlayer->Get_NameMainWeapon()));

			if (m_pPlayer->Get_CurState() == CAi_Player::TS_SPECIALCOMBO_CRASH)
			{
				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.5)
				{
					if (m_bEffect)
					{
						//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_AIRLB_FOURTH, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						
						//π´±‚- ±√±ÿ±‚ off
						if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
							m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_AIRLB_FOURTH, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						//π´±‚- ±√±ÿ±‚ on
						else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
							m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_AIRLB_FOURTH_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

						m_bEffect = false;
					}

					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250.f * (_float)dTimeDelta), m_dwNaviIndex)));
				}

				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.25f)
				{
					if (m_bDoubleEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_RIGHT_MIN, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_RIGHT_MID, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

						m_bDoubleEffect = false;
					}
				}

			}
			else if (m_pPlayer->Get_CurState() == CAi_Player::TS_SPECIALCOMBO_READY)
			{
				//»Ò¡§ √ﬂ∞° - 2¡Ÿ
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, m_pPlayer->Get_NameMainWeapon()));
				pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);

				//if (m_ePress_Key == KEY_RB)
				//	m_dbCharge += dTimeDelta;

				//if (m_eUp_Key == KEY_RB)
				//{
					//if (m_dbCharge >= m_pMeshCom->Get_Period()*0.5)
					//{
					//	int i = 0;
					//	// full charge
					//}


					m_pPlayer->Set_State(CAi_Player::TS_SPECIALCOMBO_CRASH);
					m_pMeshCom->Set_AnimationSet(CAi_Player::TS_SPECIALCOMBO_CRASH);
					return CAi_Player::ID_BASICCOMBOEX;
				//}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_State(CAi_Player::TS_SPECIALCOMBO_CRASH);
					m_pMeshCom->Set_AnimationSet(CAi_Player::TS_SPECIALCOMBO_CRASH);
					return CAi_Player::ID_BASICCOMBOEX;
				}
			}

		}

		else if (m_eWeapon == CAi_Player::DUALSWORD)
		{
			if (m_pPlayer->Get_CurState() == CAi_Player::DS_SPECIALCOMBO_END)
			{
				m_dbAcc -= dTimeDelta * 2;
				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.25)
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 750.f * (_float)(dTimeDelta)*(_float)(m_dbAcc)), m_dwNaviIndex)));
				}
			}

			// Charge_Loop
			if (m_pPlayer->Get_CurState() == CAi_Player::DS_SPECIALCOMBO_CHARGE_LOOP)
			{
				if (m_ePress_Key == KEY_RB)
					m_dbCharge += dTimeDelta;

				if (m_eUp_Key == KEY_RB)
				{
					if (m_dbCharge >= 3)
					{
						int i = 0;
						// full charge
					}

					m_pPlayer->Set_State(CAi_Player::DS_SPECIALCOMBO_END);
					m_pMeshCom->Set_AnimationSet(CAi_Player::DS_SPECIALCOMBO_END);
					m_dbAcc = 1;

					if (m_bDoubleEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL7, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bDoubleEffect = true;
					}

					if (m_bEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL8, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bEffect = true;
					}
					return CAi_Player::ID_BASICCOMBOEX;
				}

				if (m_dbCharge >= 5)
				{
					m_pPlayer->Set_State(CAi_Player::DS_SPECIALCOMBO_END);
					m_pMeshCom->Set_AnimationSet(CAi_Player::DS_SPECIALCOMBO_END);
					m_dbAcc = 1;

					if (m_bDoubleEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL7, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bDoubleEffect = true;
					}

					if (m_bEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL8, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bEffect = true;
					}
					return CAi_Player::ID_BASICCOMBOEX;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pMeshCom->Set_AnimationSet(CAi_Player::DS_SPECIALCOMBO_END);
					m_pMeshCom->Set_AnimationSet(CAi_Player::DS_SPECIALCOMBO_CHARGE_LOOP);
				}
			}

			if (m_pPlayer->Get_CurState() == CAi_Player::DS_SPECIALCOMBO_CHARGE)
			{
				//if (m_ePress_Key == KEY_RB)
				//	m_dbCharge += dTimeDelta;

				//if (m_eUp_Key == KEY_RB)
				//{
					//if (m_dbCharge >= 3)
					//{
					//	int i = 0;
					//	// full charge
					//}
					m_pPlayer->Set_State(CAi_Player::DS_SPECIALCOMBO_END);
					m_pMeshCom->Set_AnimationSet(CAi_Player::DS_SPECIALCOMBO_END);
					m_dbAcc = 1;//m_pMeshCom->Get_Period()*0.75;
					if (m_bDoubleEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL7, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bDoubleEffect = true;
					}

					if (m_bEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL8, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bEffect = true;
					}
					return CAi_Player::ID_BASICCOMBOEX;
				//}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					// full charge
					m_pPlayer->Set_State(CAi_Player::DS_SPECIALCOMBO_CHARGE_LOOP);
					m_pMeshCom->Set_AnimationSet(CAi_Player::DS_SPECIALCOMBO_CHARGE_LOOP);
					return CAi_Player::ID_BASICCOMBOEX;
				}
			}
		}

		else if (m_eWeapon == CAi_Player::ORB)
		{
			if (m_pPlayer->Get_CurState() == CAi_Player::OB_SPECIALCOMBO_READY)
			{
				//if (m_ePress_Key == KEY_RB)
				//{
				//	Engine::CGameObject* pTerrain = nullptr;
				//	pTerrain = Engine::Get_GameObject(Engine::GAMEOBJECT, L"Terrain");
				//	m_pCalculatorCom->PickDecalPos(g_hWnd, pTerrain, m_pPlayer->Get_DecalPos());
				//	if (m_bEffect)
				//	{
				//		m_pPlayer->Create_Decal(CDecal::DECAL_SKILLTARGET, 100);
				//		m_bEffect = false;
				//	}
				//	m_dbCharge += dTimeDelta;
				//}
					

				//if (m_eUp_Key == KEY_RB)
				//{
					//m_pPlayer->Release_Decal();
					if (m_bDecalCreateOnce == false)
					{
						_uint iTargetNaviIndex = m_pPlayer->Get_TargetPlayer()->Get_NaviIndex();
						_vec3 vTargetPos = m_pPlayer->Get_TargetPos();
						vTargetPos.y += 500.f;
						m_pPlayer->Get_TargetPlayer()->Get_NaviMesh()->Find_PosY(&vTargetPos, iTargetNaviIndex, vTargetPos);

						m_pPlayer->Set_DecalPos(&vTargetPos);
						m_pPlayer->Create_Decal(CDecal::DECAL_ORB_RIGHT, 2.7);
						m_bDecalCreateOnce = true;
					}
					//if (m_dbCharge >= m_pMeshCom->Get_Period()*0.5);
					//{
					//	int i = 0;
					//	// full charge
					//}

					m_pPlayer->Set_State(CAi_Player::OB_DASHCOMBO);
					m_pMeshCom->Set_AnimationSet(CAi_Player::OB_DASHCOMBO);
					return CAi_Player::ID_BASICCOMBOEX;
				//}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					// full charge
					m_pPlayer->Set_State(CAi_Player::OB_DASHCOMBO);
					m_pMeshCom->Set_AnimationSet(CAi_Player::OB_DASHCOMBO);
					return CAi_Player::ID_BASICCOMBOEX;
				}
			}
			else if (m_pPlayer->Get_CurState() == CAi_Player::OB_BASICCOMBOEX1)
			{
				//∆Ú∆Ú∆Ú∆Ú øÏ≈¨∏Ø
				if (m_bEffect)
				{
					_uint iTargetNaviIndex = m_pPlayer->Get_TargetPlayer()->Get_NaviIndex();
					_vec3 vTargetPos = m_pPlayer->Get_TargetPos();
					vTargetPos.y += 500.f;
					m_pPlayer->Get_TargetPlayer()->Get_NaviMesh()->Find_PosY(&vTargetPos, iTargetNaviIndex, vTargetPos);

					m_pPlayer->Set_DecalPos(&vTargetPos);
					m_pPlayer->Create_Decal(CDecal::DECAL_ORB_FLYING, 1.5);
					m_bEffect = false;
				}

			}
		}

		else if (m_eWeapon == CAi_Player::LONGBOW)
		{
			if (m_pPlayer->Get_CurState() == CAi_Player::LB_BASICCOMBOEX1_END)
			{
				if (m_pMeshCom->Get_Period()*0.5 > m_pMeshCom->Get_TrackTime())

				{
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					_vec3 vPos = m_pPlayer->GetPlayerPos();

					m_dbAcc += dTimeDelta * 2;
					vPos = m_pPlayer->GetPlayerPos();
					vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK) * -1.f;

					D3DXVec3Normalize(&vLook, &vLook);

					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250.f*(_float)dTimeDelta * (_float)(1.5f - m_dbAcc)), m_dwNaviIndex, true)));
				}
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					WeaponDissolve();
					return CAi_Player::ID_COMMON;
				}
				else
					return CAi_Player::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_BASICCOMBOEX1_START)
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBOEX1_LOOP);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX1_LOOP);
					return CAi_Player::ID_BASICCOMBOEX;
				}
			}
			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_BASICCOMBOEX1_LOOP)
			{
				m_pPlayer->Set_AnimationSpeed(0.75);

				if (m_eUp_Key == KEY_LB)
				{
					m_pPlayer->Set_AnimationSpeed(1);
					m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBOEX1_END);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX1_END);
					return CAi_Player::ID_BASICCOMBOEX;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.015))
				{
					if (5 < m_iCount)
					{
						m_pPlayer->Set_AnimationSpeed(1);
						m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBOEX1_END);
						m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX1_END);
						Engine::CGameObject* pGameObject = nullptr;
						pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
						pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + (vLook + vRight) * 0.5f + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
						pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + (vLook - vRight) * 0.5f + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						return CAi_Player::ID_BASICCOMBOEX;
					}
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					// full charge
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX1_END);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX1_LOOP);
					++m_iCount;
					return CAi_Player::ID_BASICCOMBOEX;
				}
				return CAi_Player::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_BASICCOMBOEX3_02_DOWN)
			{
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				m_dbAcc += dTimeDelta;
				D3DXVec3Normalize(&vLook, &vLook);

				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.7)
				{
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vLook * 250 * (_float)((1.2f - m_dbAcc) * dTimeDelta)), m_dwNaviIndex)));
				}
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AnimationSpeed(1);
					WeaponDissolve();
					return CAi_Player::ID_COMMON;
				}
				else
					return CAi_Player::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_BASICCOMBOEX3_02_CHARGESHOT)
			{
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				m_dbAcc += dTimeDelta;
				D3DXVec3Normalize(&vLook, &vLook);

				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.7)
				{
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vLook * 250 * (_float)((1.2f - m_dbAcc) * dTimeDelta)), m_dwNaviIndex)));
				}
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AnimationSpeed(1);
					WeaponDissolve();
					return CAi_Player::ID_COMMON;
				}
				else
					return CAi_Player::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_BASICCOMBOEX3_02_CHARGE)
			{
				//m_pPlayer->Set_AnimationSpeed(0.75);
				m_dbCharge += dTimeDelta;

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					// full charge
					m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBOEX3_02_CHARGESHOT);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX3_02_CHARGESHOT);
					return CAi_Player::ID_BASICCOMBOEX;
				}

				if (m_eUp_Key == KEY_RB)
				{
					if (m_dbCharge >= m_pMeshCom->Get_Period()*0.5)
						int i = 0; // full charge

					m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBOEX3_02_CHARGESHOT);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX3_02_CHARGESHOT);

					return CAi_Player::ID_BASICCOMBOEX;
				}

				else
					return CAi_Player::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_BASICCOMBOEX3_01)
			{
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				m_dbAcc += dTimeDelta;
				D3DXVec3Normalize(&vLook, &vLook);

				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.7)
				{
					if (m_pMeshCom->Get_TrackTime() > m_pMeshCom->Get_Period()*0.4 && m_bEffect)
					{
						Engine::CGameObject* pGameObject = nullptr;
						pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bEffect = false;
					}
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vLook * 500 * (_float)((1.2f - m_dbAcc) * dTimeDelta)), m_dwNaviIndex)));
				}


				if (m_eDown_Key == KEY_RB)
				{
					m_bReserveFinish = true;
				}

				if (m_bReserveFinish && m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					m_dbAcc = 0;

					/*_matrix matView = m_pPlayer->Get_ViewMatrix();
					_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					_vec3 vPos = m_pPlayer->GetPlayerPos();

					D3DXMatrixInverse(&matView, 0, &matView);
					D3DXVec3Normalize(&vLook, &vLook);

					_float fCrossy, fDegree;

					vPurpose = { matView.m[2][0],matView.m[2][1] ,matView.m[2][2] };

					CAi_PlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

					if (!isnan(fDegree))
					{
						if (fCrossy > 0.f)
							m_pTransform->Rotation(Engine::ROT_Y, fDegree);
						else
							m_pTransform->Rotation(Engine::ROT_Y, -fDegree);
					}*/

					if (m_ePress_Key == KEY_RB)
					{
						m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBOEX3_02_CHARGE);
						m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX3_02_CHARGE);
						return CAi_Player::ID_BASICCOMBOEX;
					}

					m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBOEX3_02);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX3_02);
					return CAi_Player::ID_BASICCOMBOEX;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					WeaponDissolve();
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_BASICCOMBOEX2_END)
			{
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				m_dbAcc += dTimeDelta;
				D3DXVec3Normalize(&vLook, &vLook);

				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.1)
				{
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vLook * 250 * (_float)((1.2f - m_dbAcc) * dTimeDelta)), m_dwNaviIndex)));
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					WeaponDissolve();
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_BASICCOMBOEX2_START)
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_dbAcc = 0;
					m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBOEX2_END);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX2_END);
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					return CAi_Player::ID_BASICCOMBOEX;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_SPECIALCOMBO_CHARGESHOT)
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					if (m_ePress_Key == KEY_RB)
					{
						m_pPlayer->Set_State(CAi_Player::LB_SPECIALCOMBO_WAIT);
						m_pMeshCom->Set_AnimationSet(CAi_Player::LB_SPECIALCOMBO_WAIT);
						return CAi_Player::ID_BASICCOMBOEX;
					}
					else
					{
						WeaponDissolve();
						return CAi_Player::ID_COMMON;
					}
				}
				return CAi_Player::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_SPECIALCOMBO_CHARGE)
			{
				m_dbCharge += dTimeDelta;

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					// full charge
					m_pPlayer->Set_State(CAi_Player::LB_SPECIALCOMBO_CHARGESHOT);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_SPECIALCOMBO_CHARGESHOT);
					return CAi_Player::ID_BASICCOMBOEX;
				}

				if (!m_ePress_Key == KEY_LB)
				{
					if (m_dbCharge >= m_pMeshCom->Get_Period()*0.5)
						int i = 0; // full charge
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_pPlayer->Set_State(CAi_Player::LB_SPECIALCOMBO_CHARGESHOT);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_SPECIALCOMBO_CHARGESHOT);
					return CAi_Player::ID_BASICCOMBOEX;
				}

				return CAi_Player::ID_BASICCOMBOEX;
			}
			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_SPECIALCOMBO_WAIT)
			{
				if (!m_ePress_Key == KEY_RB)
				{
					m_pPlayer->Set_State(CAi_Player::LB_SPECIALCOMBO_END);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_SPECIALCOMBO_END);
					return CAi_Player::ID_BASICCOMBOEX;
				}

				if (m_ePress_Key == KEY_LB)
				{
					m_pPlayer->Set_State(CAi_Player::LB_SPECIALCOMBO_CHARGE);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_SPECIALCOMBO_CHARGE);
					return CAi_Player::ID_BASICCOMBOEX;
				}

				return CAi_Player::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_SPECIALCOMBO_READY)
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_State(CAi_Player::LB_SPECIALCOMBO_WAIT);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_SPECIALCOMBO_WAIT);
					return CAi_Player::ID_BASICCOMBOEX;
				}
				return CAi_Player::ID_BASICCOMBOEX;
			}

		}

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			WeaponDissolve();
			return CAi_Player::ID_COMMON;
		}
	}
	break;
	default:
		break;
	}
	return CAi_Player::ID_BASICCOMBOEX;
}

void CAi_BasicComboEx::Update_PlayerPattern(const _double dTimeDelta)
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

	if (m_bPass)
	{
		m_eEndType = TYPE_END;
		return;
	}

	if (!m_bEnter)
	{
		//m_eEndType = TYPE_RESERVE;
		switch (m_eLevel)
		{
		case CBasePlayer::LEVEL1:
		{
			if (m_eWeapon == CAi_Player::TWOHANDSWORD)
			{
				Set_Effect();
				m_bDoubleEffect = true;
				m_pMeshCom->Set_AnimationSet(CAi_Player::TS_BASICCOMBOEX01);
				m_pPlayer->Set_State(CAi_Player::TS_BASICCOMBOEX01);
				m_eEndType = TYPE_RESERVE;
			}
			else if (m_eWeapon == CAi_Player::DUALSWORD)
			{
				if (m_bEffect)
				{
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_AIRUP, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bEffect = false;
				}

				if (m_pMeshCom->Get_TrackTime() > m_pMeshCom->Get_Period()*0.2)
				{
					if (m_bDoubleEffect)
					{
						m_bDoubleEffect = false;
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_AIRUP2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					}
				}

				m_pMeshCom->Set_AnimationSet(CAi_Player::DS_BASICCOMBOEX1);
				m_pPlayer->Set_State(CAi_Player::DS_BASICCOMBOEX1);
				m_eEndType = TYPE_RESERVE;
			}
			else if (m_eWeapon == CAi_Player::LONGBOW)
			{
				//ø©±‚∞° µ⁄∑Œ π–∑¡≥™∞®
				_matrix matTarget = m_pPlayer->Get_Target_WorldMatrix();
				_vec3 vTargetPos = m_pPlayer->Get_TargetPos();
				_vec3 vDir = { 0.f,0.f,0.f };
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				_vec3 vPurpose = vTargetPos - vPos;

				//D3DXMatrixInverse(&matTarget, 0, &matTarget);
				D3DXVec3Normalize(&vLook, &vLook);
				D3DXVec3Normalize(&vPurpose, &vPurpose);

				_float fCrossy, fDegree;

				//vPurpose = { matTarget.m[2][0],matTarget.m[2][1] ,matTarget.m[2][2] };

				CAi_PlayerState::Rotation_Direction(matTarget, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

				if (!isnan(fDegree))
				{
					if (fCrossy > 0.f)
						m_pTransform->Rotation(Engine::ROT_Y, fDegree);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree);
				}

				m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX3_01);
				m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBOEX3_01);
				m_eEndType = TYPE_END;
				m_bPass = true;
			}

		}
		break;
		case CBasePlayer::LEVEL2:
		{
			if (m_eWeapon == CAi_Player::TWOHANDSWORD)
			{
				Set_Effect();
				m_bDoubleEffect = true;
				m_eEndType = TYPE_RESERVE;
				m_pMeshCom->Set_AnimationSet(CAi_Player::TS_BASICCOMBOEX02_START);
				m_pPlayer->Set_State(CAi_Player::TS_BASICCOMBOEX02_START);
				m_pPlayer->Set_AnimationSpeed(2);
			}

			else if (m_eWeapon == CAi_Player::DUALSWORD)
			{
				m_eEndType = TYPE_RESERVE;
				m_pMeshCom->Set_AnimationSet(CAi_Player::DS_BASICCOMBOEX2_START);
				m_pPlayer->Set_State(CAi_Player::DS_BASICCOMBOEX2_START);
			}

			else if (m_eWeapon == CAi_Player::LONGBOW)
			{
				//ø©±‚∞° ø¨πﬂ Ω√¿€
				m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX1_START);
				m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBOEX1_START);
				m_eEndType = TYPE_RESERVE;
				m_bPass = true;
			}

			else if (m_eWeapon == CAi_Player::ORB)
			{
				//ø©±‚∞° ∞≠∞¯∞›

				m_eEndType = TYPE_END;
				m_bPass = true;
				m_pMeshCom->Set_AnimationSet(CAi_Player::OB_SPECIALCOMBO_READY);
				m_pPlayer->Set_State(CAi_Player::OB_SPECIALCOMBO_READY);
			}
		}
		break;
		case CBasePlayer::LEVEL3:
		{
			if (m_eWeapon == CAi_Player::ORB)
			{
				m_eEndType = TYPE_RESERVE;
				m_pMeshCom->Set_AnimationSet(CAi_Player::OB_BASICCOMBOEX2_01_F);
				m_pPlayer->Set_State(CAi_Player::OB_BASICCOMBOEX2_01_F);
				m_eReserveState = CAi_Player::COMMON_COMBATWAIT;
				m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBOEX02);
			}

			else if (m_eWeapon == CAi_Player::LONGBOW)
			{
				//ø©±‚∞° ∞¯¡ﬂ
				//_matrix matView = m_pPlayer->Get_ViewMatrix();

				_matrix matTarget = m_pPlayer->Get_Target_WorldMatrix();
				_vec3 vTargetPos = m_pPlayer->Get_TargetPos();
				_vec3 vDir = { 0.f,0.f,0.f };
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				_vec3 vPurpose = vTargetPos - vPos;

				//D3DXMatrixInverse(&matTarget, 0, &matTarget);
				D3DXVec3Normalize(&vLook, &vLook);
				D3DXVec3Normalize(&vPurpose, &vPurpose);

				_float fCrossy, fDegree;

				//vPurpose = { matTarget.m[2][0],matTarget.m[2][1] ,matTarget.m[2][2] };

				CAi_PlayerState::Rotation_Direction(matTarget, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

				if (!isnan(fDegree))
				{
					if (fCrossy > 0.f)
						m_pTransform->Rotation(Engine::ROT_Y, fDegree);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree);
				}

				m_pMeshCom->Set_AnimationSet(CAi_Player::LB_BASICCOMBOEX2_START);
				m_pPlayer->Set_State(CAi_Player::LB_BASICCOMBOEX2_START);
				m_eEndType = TYPE_END;
				m_bPass = true;
			}
		}
		break;
		case CBasePlayer::LEVEL4:
		{
			if (m_eWeapon == CAi_Player::ORB)
			{
				m_eEndType = TYPE_END;
				m_pMeshCom->Set_AnimationSet(CAi_Player::OB_BASICCOMBOEX1);
				m_pPlayer->Set_State(CAi_Player::OB_BASICCOMBOEX1);
				m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBOEX01);
			}

			else if (m_eWeapon == CAi_Player::TWOHANDSWORD)
			{
				Set_Effect();
				m_bDoubleEffect = true;
				m_eEndType = TYPE_END;
				m_bPass = true;
				m_pMeshCom->Set_AnimationSet(CAi_Player::TS_SPECIALCOMBO_READY);
				m_pPlayer->Set_State(CAi_Player::TS_SPECIALCOMBO_READY);
			}

			else if (m_eWeapon == CAi_Player::DUALSWORD)
			{
				m_eEndType = TYPE_END;
				m_bPass = true;
				m_pMeshCom->Set_AnimationSet(CAi_Player::DS_SPECIALCOMBO_CHARGE);
				m_pPlayer->Set_State(CAi_Player::DS_SPECIALCOMBO_CHARGE);
			}

			else if (m_eWeapon == CAi_Player::LONGBOW)
			{
				//ø©±‚∞° ¡‹
				m_eEndType = TYPE_END;
				m_bPass = true;
				m_pMeshCom->Set_AnimationSet(CAi_Player::LB_SPECIALCOMBO_READY);
				m_pPlayer->Set_State(CAi_Player::LB_SPECIALCOMBO_READY);
			}
		}
		break;
		case CBasePlayer::LEVEL_END:
			break;
		default:
			break;
		}

	}
	else
	{
		switch (m_eLevel)
		{
		case CBasePlayer::LEVEL1:
		{
			if (m_eWeapon == CAi_Player::TWOHANDSWORD)
			{
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, m_pPlayer->Get_NameMainWeapon()));

				////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
				if (m_bEffect)
				{
					//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_AIRLB_THIRD, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					
					//π´±‚- ±√±ÿ±‚ off
					if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_AIRLB_THIRD, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					//π´±‚- ±√±ÿ±‚ on
					else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_AIRLB_THIRD_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

					m_bEffect = false;
				}
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.2f)
				{
					if (m_bDoubleEffect)
					{
						//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_AIRUP, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						
						//π´±‚- ±√±ÿ±‚ off
						if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
							m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_AIRUP, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						//π´±‚- ±√±ÿ±‚ on
						else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
							m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_AIRUP_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

						
						m_bDoubleEffect = false;
					}
				}
				/////////////////////////////

				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 500.f * (_float)dTimeDelta), m_dwNaviIndex)));
					m_eEndType = TYPE_END;
				}

				//if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				//{
				//	m_eEndType = TYPE_END;
				//}
			}
			else if (m_eWeapon == CAi_Player::DUALSWORD)
			{

				if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 500 * (_float)dTimeDelta), m_dwNaviIndex)));
					m_eEndType = TYPE_END;
				}
			}
		}
		break;
		case CBasePlayer::LEVEL2: // BasicComboEx02
		{
			if (m_eWeapon == CAi_Player::TWOHANDSWORD)
			{
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, m_pPlayer->Get_NameMainWeapon()));

				if (CAi_Player::TS_BASICCOMBOEX02_START == m_pPlayer->Get_CurState())
				{
					m_eReserveState = CAi_Player::TS_BASICCOMBOEX02_LOOP;
					m_eEndType = TYPE_RESERVE;

					////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
					if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.3f)
					{
						if (m_bEffect)
						{
							//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_PRESSING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
							
							//π´±‚- ±√±ÿ±‚ off
							if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_PRESSING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
							//π´±‚- ±√±ÿ±‚ on
							else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_PRESSING_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

							m_bEffect = false;
						}
					}
					///////////////////////////////

				}

				else if (CAi_Player::TS_BASICCOMBOEX02_LOOP == m_pPlayer->Get_CurState())
				{
					m_pPlayer->Set_AnimationSpeed(2);
					m_dbHoldingTime += dTimeDelta;
					//m_eEndType = TYPE_RESERVE;

					////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
					if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.3f)
					{
						if (m_bEffect)
						{
							//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_PRESSING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
							
							//π´±‚- ±√±ÿ±‚ off
							if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_PRESSING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
							//π´±‚- ±√±ÿ±‚ on
							else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_PRESSING_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

							m_bEffect = false;
						}
					}
					///////////////////////////////

					if (2 > m_dbHoldingTime)
					{
						m_eEndType = TYPE_LOOP;
					}
					else
					{
						m_eEndType = TYPE_RESERVE;
						m_eReserveState = CAi_Player::TS_BASICCOMBOEX02_END;

						////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
						if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.25f)
						{
							if (m_bDoubleEffect)
							{
								//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_PRESSING2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
								
								//π´±‚- ±√±ÿ±‚ off
								if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
									m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_PRESSING2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
								//π´±‚- ±√±ÿ±‚ on
								else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
									m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_PRESSING2_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

								m_bDoubleEffect = false;
							}
						}
						//////////////////////////////
					}

					if (m_ePress_Key == KEY_LB)
					{
						if (5 <= m_dbHoldingTime)
						{
							m_eEndType = TYPE_RESERVE;
							m_eReserveState = CAi_Player::TS_BASICCOMBOEX02_END;
						}
						else
						{
							m_eEndType = TYPE_LOOP;
							m_eReserveState = CAi_Player::TS_BASICCOMBOEX02_LOOP;
						}
					}

					if (m_eUp_Key == KEY_LB)
					{
						if (2 <= m_dbHoldingTime)
						{
							m_eEndType = TYPE_RESERVE;
							m_eReserveState = CAi_Player::TS_BASICCOMBOEX02_END;
						}
						else
						{
							m_bReserveFinish = true;
							m_eEndType = TYPE_LOOP;
							m_eReserveState = CAi_Player::TS_BASICCOMBOEX02_LOOP;
						}
					}

					if (m_bReserveFinish && 2 <= m_dbHoldingTime)
					{
						m_eEndType = TYPE_RESERVE;
						m_eReserveState = CAi_Player::TS_BASICCOMBOEX02_END;
					}
				}
			}

			else if (m_eWeapon == CAi_Player::DUALSWORD)
			{
				if (CAi_Player::DS_BASICCOMBOEX2_START == m_pPlayer->Get_CurState())
				{
					m_eReserveState = CAi_Player::DS_BASICCOMBOEX2_LOOP;
					m_eEndType = TYPE_RESERVE;
				}

				else if (CAi_Player::DS_BASICCOMBOEX2_LOOP == m_pPlayer->Get_CurState())
				{
					m_dbHoldingTime += dTimeDelta;
					m_dbAcc += dTimeDelta;
					//m_eEndType = TYPE_RESERVE;
					if (2 > m_dbHoldingTime)
					{
						m_eEndType = TYPE_LOOP;
					}
					else
					{
						m_eEndType = TYPE_RESERVE;
						m_eReserveState = CAi_Player::DS_BASICCOMBOEX2_END;

					}

					if (m_ePress_Key == KEY_LB)
					{
						if (0.15 <= m_dbAcc)
						{
							m_dbAcc = 0.0;
							if (3 == m_iCount)
							{
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICSP4, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
								++m_iCount;
							}
							else if (2 == m_iCount)
							{
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICSP3, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
								++m_iCount;
							}
							else if (1 == m_iCount)
							{
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICSP2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
								++m_iCount;
							}
							else if (0 == m_iCount)
							{
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICSP1, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
								++m_iCount;
							}
							else if (4 == m_iCount)
							{
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICSP5, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
								m_iCount = 0;
							}
						}

						if (3 <= m_dbHoldingTime)
						{
							m_eEndType = TYPE_RESERVE;
							m_eReserveState = CAi_Player::DS_BASICCOMBOEX2_END;
						}
						else
						{
							m_eEndType = TYPE_LOOP;
							m_eReserveState = CAi_Player::DS_BASICCOMBOEX2_LOOP;
						}
					}

					if (m_eUp_Key == KEY_LB)
					{
						if (2 <= m_dbHoldingTime)
						{
							m_eEndType = TYPE_RESERVE;
							m_eReserveState = CAi_Player::DS_BASICCOMBOEX2_END;
						}
						else
						{
							m_bReserveFinish = true;
							m_eEndType = TYPE_LOOP;
							m_eReserveState = CAi_Player::DS_BASICCOMBOEX2_LOOP;
						}

					}
				}

				else if (CAi_Player::DS_BASICCOMBOEX2_END == m_pPlayer->Get_CurState())
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
		}
		break;
		case CBasePlayer::LEVEL3:
			break;
		case CBasePlayer::LEVEL4:
			break;
		case CBasePlayer::LEVEL_END:
			break;
		default:
			break;
		}
	}
}
