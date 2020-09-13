#include "stdafx.h"
#include "Ai_AirCombo.h"
#include "DSBullet.h"
#include "OrbCore.h"

CAi_AirCombo::CAi_AirCombo()
	:m_bEnter(false),
	m_bNextAtk(false),
	m_bFaill(false),
	m_bLanding(false),
	m_bPass(false),
	m_bChange(false),
	m_bCancel(false),
	m_eEndType(TYPE_RESERVE),
	m_dbMotionCancel(0),
	m_dbJumpTime(0),
	m_dbAcc(0),
	m_dbDashTime(0),
	m_iRepeat(0),
	m_iCount(0)
{
}

CAi_AirCombo::~CAi_AirCombo()
{
}

void CAi_AirCombo::Enter(CAi_Player * pPlayer)
{
	CAi_PlayerState::Enter_State(pPlayer);
}

void CAi_AirCombo::Update(CAi_Player * pPlayer, const _double dTimeDelta)
{
}

void CAi_AirCombo::ChangeState(CAi_PlayerState * pState)
{
}

void CAi_AirCombo::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float * fCrossy, _vec3 * vDir, _float * fDegree)
{
}

void CAi_AirCombo::Enter_State(CBasePlayer * pPlayer)
{
	CAi_PlayerState::Enter_State(pPlayer);
}

void CAi_AirCombo::Update_State(const _double dTimeDelta)
{
	Update_PlayerPattern(dTimeDelta);
}

CAi_Player::STATE_ID CAi_AirCombo::LateUpadte_State(const _double dTimeDelta)
{
	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);
	_vec3 vUp = *m_pTransform->Get_Info(Engine::INFO_UP);
	D3DXVec3Normalize(&vUp, &vUp);
	_vec3 vPos = m_pPlayer->GetPlayerPos();
	if (m_bCancel)
		return CAi_Player::ID_COMMON;
	m_dbMotionCancel += dTimeDelta;
	m_bEnter = true;
	switch (m_eEndType)
	{
	case CAi_AirCombo::TYPE_RESERVE:
	{
		//if (m_pMeshCom->Is_AnimationSetFinish(0.2))

		if (!m_bEnter)
			m_bEnter = true;

		m_bNextAtk = false;

		if (m_eReserveState == CAi_Player::STATE_END)
			return CAi_Player::ID_COMMON;

		else if (m_pPlayer->Get_CurState() == CAi_Player::OB_SPECIALMOVE)
		{
			_vec3 vPos = m_pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 750 * (_float)(dTimeDelta)), m_dwNaviIndex, true)));

			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_pMeshCom->Set_AnimationSet(CAi_Player::TS_AIR_COMBO_01);
				m_pMeshCom->Set_AnimationSet(CAi_Player::OB_SPECIALMOVE);
				if (0 > m_iRepeat)
				{
					m_pPlayer->Set_AirComboLevel(0);
					return CAi_Player::ID_COMMON;
				}
				--m_iRepeat;
				return CAi_Player::ID_AIRCOMBO;
			}
			return CAi_Player::ID_AIRCOMBO;
		}

		else if (m_eWeapon == CAi_Player::LONGBOW)
		{

			if (m_pPlayer->Get_CurState() == CAi_Player::LB_AIRCOMBO_03)
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					m_pPlayer->Set_AirComboLevel(3);
				}
			}

			if (m_pPlayer->Get_CurState() == CAi_Player::LB_AIRCOMBO_02)
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					m_pPlayer->Set_AirComboLevel(2);
				}
			}

			if (m_pPlayer->Get_CurState() == CAi_Player::LB_AIRCOMBO_01)
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					m_pPlayer->Set_AirComboLevel(1);
				}
			}

		}

		else if (m_eWeapon == CAi_Player::TWOHANDSWORD)
		{
			/*if (m_pPlayer->Get_CurState() == CAi_Player::TS_AIR_COMBO_04)
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					m_pPlayer->Set_AirComboLevel(4);
				}
			}*/

			if (m_pPlayer->Get_CurState() == CAi_Player::TS_AIR_COMBO_03)
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					Set_Effect();
					m_bDoubleEffect = true;
					//m_pPlayer->Set_AirComboLevel(3);
				}
			}

			if (m_pPlayer->Get_CurState() == CAi_Player::TS_AIR_COMBO_02)
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					Set_Effect();
					m_bDoubleEffect = true;
					m_pPlayer->Set_AirComboLevel(2);
				}
			}

			if (m_pPlayer->Get_CurState() == CAi_Player::TS_AIR_COMBO_01)
			{

				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					Set_Effect();
					m_bDoubleEffect = true;
					m_pPlayer->Set_AirComboLevel(1);
				}
			}
		}

		else if (m_eWeapon == CAi_Player::DUALSWORD)
		{
			if (m_pPlayer->Get_CurState() == CAi_Player::DS_AIRCOMBO_04)
			{
				m_bPass = true;
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					m_pPlayer->Set_AirComboLevel(3);
				}
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
					m_pPlayer->Set_AirComboLevel(0);
			}

			if (m_pPlayer->Get_CurState() == CAi_Player::DS_AIRCOMBO_03)
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					m_pPlayer->Set_AirComboLevel(3);
				}
			}

			if (m_pPlayer->Get_CurState() == CAi_Player::DS_AIRCOMBO_02)
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					m_pPlayer->Set_AirComboLevel(2);
				}
			}

			if (m_pPlayer->Get_CurState() == CAi_Player::DS_AIRCOMBO_01)
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					m_pPlayer->Set_AirComboLevel(1);
				}
			}
		}

		//else
		{
			//if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
			{
				Set_Effect();
				m_bDoubleEffect = true;
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				m_pPlayer->Set_State(m_eReserveState);
				m_eEndType = TYPE_END;
			}
		}
	}
	break;
	case CAi_AirCombo::TYPE_CHANGE:
	{
		//if (m_dbMotionCancel >= m_pMeshCom->Get_Period()*0.21)
		//{
		//	//if (!m_bIsSkill)
		//	//	return CAi_Player::ID_BASICCOMBOEX;
		//}
	}
	break;
	case CAi_AirCombo::TYPE_END:
	{
		if (m_eWeapon == CAi_Player::TWOHANDSWORD)
		{
			//»Ò¡§ √ﬂ∞° - 2¡Ÿ
			CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));
			pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);

			if (m_pPlayer->Get_CurState() == CAi_Player::TS_AIR_COMBO_01)
			{
				////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
				if (m_bEffect)
				{
					//π´±‚- ±√±ÿ±‚ off
					if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					//π´±‚- ±√±ÿ±‚ on
					else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

					m_bEffect = false;
				}
				/////////////////////////////

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AirComboLevel(1);

					m_pPlayer->Set_AnimationSpeed(1);
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::TS_AIR_COMBO_02)
			{
				//////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
				if (m_bEffect)
				{
					//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_AIRLB_SECOND, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					
					
					//π´±‚- ±√±ÿ±‚ off
					if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_AIRLB_SECOND, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					//π´±‚- ±√±ÿ±‚ on
					else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_AIRLB_SECOND_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

					
					m_bEffect = false;
				}
				///////////////////////////

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AirComboLevel(2);
					m_pPlayer->Set_AnimationSpeed(1);
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::TS_AIR_COMBO_03)
			{
				//////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
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
				///////////////////////////

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					Set_Effect();
					//m_pPlayer->Set_AirComboLevel(3);
					m_pPlayer->Set_AnimationSpeed(1);
					m_pMeshCom->Set_AnimationSet(CAi_Player::TS_AIR_COMBO_04);
					m_pPlayer->Set_State(CAi_Player::TS_AIR_COMBO_04);
					return CAi_Player::ID_AIRCOMBO;

					//return CAi_Player::ID_COMMON;
				}
			}
			if (m_pPlayer->Get_CurState() == CAi_Player::TS_AIR_COMBO_04_LANDING)
			{
				if (m_bDoubleEffect)
				{
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_AIRROCK_MIN, m_pTransform->Get_WorldMatrix(), m_pTransform->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_AIRROCK_MID, m_pTransform->Get_WorldMatrix(), m_pTransform->Get_Angle(), *m_pPlayer->Get_BaseInfo());

					m_bDoubleEffect = false;
				}
				if (m_pMeshCom->Is_AnimationSetFinish(0.15))
				{
					m_pPlayer->Set_AnimationSpeed(1);
					m_pPlayer->Set_Jump_Finish();
					m_pPlayer->Set_Fall_End();
					m_pPlayer->Set_AirComboLevel(0);
					
					return CAi_Player::ID_COMMON;
				}
				return CAi_Player::ID_AIRCOMBO;
			}
			else if (m_pPlayer->Get_CurState() == CAi_Player::TS_AIR_COMBO_04)
			{
				////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
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
				///////////////////////

				m_pPlayer->Set_AnimationSpeed(0.75);
				_vec3 vPos = m_pPlayer->GetPlayerPos();

				_vec3 vUp = (*m_pTransform->Get_Info(Engine::INFO_UP) * -1.f);
				D3DXVec3Normalize(&vUp, &vUp);

				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vUp * 600.f*(_float)(dTimeDelta)), m_dwNaviIndex, true)));

				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					m_pNaviCom->Find_PosY(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex, vPos);
					m_pTransform->Set_Pos(&vPos);
					m_pPlayer->Set_AnimationSpeed(1);

					m_pMeshCom->Set_AnimationSet(CAi_Player::TS_AIR_COMBO_04_LANDING);
					m_pPlayer->Set_State(CAi_Player::TS_AIR_COMBO_04_LANDING);
					return CAi_Player::ID_AIRCOMBO;
				}

				if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
				{
					m_pNaviCom->Find_PosY(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex, vPos);
					m_pTransform->Set_Pos(&vPos);
					m_pPlayer->Set_AnimationSpeed(1);

					m_pMeshCom->Set_AnimationSet(CAi_Player::TS_AIR_COMBO_04_LANDING);
					m_pPlayer->Set_State(CAi_Player::TS_AIR_COMBO_04_LANDING);
					return CAi_Player::ID_AIRCOMBO;

				}

				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period())
				{
					m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_AIR_JUMPLANDING);
					m_pMeshCom->Set_AnimationSet(CAi_Player::TS_AIR_COMBO_04);

					m_pPlayer->Set_AnimationSpeed(1);
					m_pPlayer->Set_AirComboLevel(0);
					return CAi_Player::ID_AIRCOMBO;
				}
				return CAi_Player::ID_AIRCOMBO;
			}
		}

		else if (m_eWeapon == CAi_Player::ORB)
		{
			if (m_pPlayer->Get_CurState() == CAi_Player::OB_AIR_COMBO_01)
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
				_matrix matView = m_pPlayer->Get_ViewMatrix();
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
						m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
				}
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AirComboLevel(1);

					m_pPlayer->Set_AnimationSpeed(1);
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::OB_AIR_COMBO_02)
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
				_matrix matView = m_pPlayer->Get_ViewMatrix();
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
						m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
				}
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AirComboLevel(2);
					m_pPlayer->Set_AnimationSpeed(1);
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::OB_AIR_COMBO_03)
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
				_matrix matView = m_pPlayer->Get_ViewMatrix();
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
						m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
				}

				m_dbAcc += dTimeDelta;
				vPos = m_pPlayer->GetPlayerPos();
				vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK) * -1.f;
				_vec3 vUp = (*m_pTransform->Get_Info(Engine::INFO_UP) * -1.f);

				D3DXVec3Normalize(&vLook, &vLook);
				D3DXVec3Normalize(&vUp, &vUp);

				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 375.f*(_float)dTimeDelta * (_float)(1.5f - m_dbAcc)), m_dwNaviIndex, true)));
				vPos = m_pPlayer->GetPlayerPos();
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vUp * 500.f*(_float)(dTimeDelta*m_dbAcc)), m_dwNaviIndex, true)));

				if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
				{
					m_pNaviCom->Find_PosY(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex, vPos);
					m_pTransform->Set_Pos(&vPos);
					m_pPlayer->Set_Fall_End();
					m_pPlayer->Set_Jump_Finish();
					m_pPlayer->Set_AirComboLevel(0);
					m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_AIR_JUMPLANDING);
					return CAi_Player::ID_COMMON;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_dbAcc = 0;
					m_pPlayer->Set_AirComboLevel(3);
					m_pPlayer->Set_AnimationSpeed(1);
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::OB_SPECIALMOVE)
			{
				_matrix matView = m_pPlayer->Get_ViewMatrix();
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
						m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
				}

				m_dbDashTime += dTimeDelta;
				vPos = m_pPlayer->GetPlayerPos();
				vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 1000.f*(_float)(dTimeDelta) * (_float)(1.5 - m_dbDashTime)), m_dwNaviIndex, true)));

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pMeshCom->Set_AnimationSet(CAi_Player::TS_AIR_COMBO_01);
					m_pMeshCom->Set_AnimationSet(CAi_Player::OB_SPECIALMOVE);
					if (0 > m_iRepeat)
					{
						m_dbAcc = 0;
						m_pPlayer->Set_AirComboLevel(0);
						m_pPlayer->Set_Jump_Start();
						m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_AIR_JUMPDOWN);
						return CAi_Player::ID_COMMON;
					}
					--m_iRepeat;
					return CAi_Player::ID_AIRCOMBO;
				}
				return CAi_Player::ID_AIRCOMBO;
			}

			else if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_pPlayer->Set_AnimationSpeed(1);
				return CAi_Player::ID_COMMON;
			}
		}

		else if (m_eWeapon == CAi_Player::DUALSWORD)
		{
			if (m_pPlayer->Get_CurState() == CAi_Player::DS_AIRCOMBO_01)
			{
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					if (m_bDoubleEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL7, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bDoubleEffect = false;
						_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
						_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
						_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
						_vec3 test = *m_pTransform->Get_Angle();
						Engine::CGameObject* pGameObject = nullptr;
						pGameObject = CDsBullet::Create(Engine::Get_Device(), &(vPos + _vec3(0.f, 50.f, 0.f)), &test, vPos + _vec3(0.f, 50.f, 0.f) + vLook*500.f, Engine::EFFECTTYPE::EFFECT_BASICTRAIL7, m_pPlayer->Get_BaseInfo()->eObjectID);
						if (pGameObject == nullptr)
							return CAi_Player::ID_SKILL;
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"DSBullet", pGameObject);
					}
				}

				if (m_bEffect)
				{
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL8, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bEffect = false;
					_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
					_vec3 test = *m_pTransform->Get_Angle();
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = CDsBullet::Create(Engine::Get_Device(), &(vPos + _vec3(0.f, 50.f, 0.f)), &test, vPos + _vec3(0.f, 50.f, 0.f) + vLook*500.f, Engine::EFFECTTYPE::EFFECT_BASICTRAIL8, m_pPlayer->Get_BaseInfo()->eObjectID);
					if (pGameObject == nullptr)
						return CAi_Player::ID_SKILL;
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"DSBullet", pGameObject);
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AirComboLevel(1);
					m_pPlayer->Set_AnimationSpeed(1);
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::DS_AIRCOMBO_02)
			{
				_matrix matView = m_pPlayer->Get_ViewMatrix();
				_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vPos = m_pPlayer->GetPlayerPos();

				D3DXMatrixInverse(&matView, 0, &matView);
				D3DXVec3Normalize(&vLook, &vLook);

				_float fCrossy, fDegree;

				vPurpose = { matView.m[2][0],matView.m[2][1] ,matView.m[2][2] };

				CAi_PlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.25)
				{
					if (m_bDoubleEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_AIRTRAIL2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bDoubleEffect = false;
						_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
						_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
						_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
						_vec3 test = *m_pTransform->Get_Angle();
						Engine::CGameObject* pGameObject = nullptr;
						pGameObject = CDsBullet::Create(Engine::Get_Device(), &(vPos + _vec3(0.f, 50.f, 0.f)), &test, vPos + _vec3(0.f, 50.f, 0.f) + vLook*500.f, Engine::EFFECTTYPE::EFFECT_AIRTRAIL2, m_pPlayer->Get_BaseInfo()->eObjectID);
						if (pGameObject == nullptr)
							return CAi_Player::ID_SKILL;
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"DSBullet", pGameObject);
					}
				}

				if (m_bEffect)
				{
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_AIRTRAIL1, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bEffect = false;
					_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
					_vec3 test = *m_pTransform->Get_Angle();
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = CDsBullet::Create(Engine::Get_Device(), &(vPos + _vec3(0.f, 50.f, 0.f)), &test, vPos + _vec3(0.f, 50.f, 0.f) + vLook*500.f, Engine::EFFECTTYPE::EFFECT_AIRTRAIL1, m_pPlayer->Get_BaseInfo()->eObjectID);
					if (pGameObject == nullptr)
						return CAi_Player::ID_SKILL;
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"DSBullet", pGameObject);
				}


				if (!isnan(fDegree))
				{
					if (fCrossy > 0.f)
						m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
				}
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AirComboLevel(2);
					m_pPlayer->Set_AnimationSpeed(1);
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::DS_AIRCOMBO_03)
			{
				_matrix matView = m_pPlayer->Get_ViewMatrix();
				_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vPos = m_pPlayer->GetPlayerPos();

				D3DXMatrixInverse(&matView, 0, &matView);
				D3DXVec3Normalize(&vLook, &vLook);

				_float fCrossy, fDegree;

				vPurpose = { matView.m[2][0],matView.m[2][1] ,matView.m[2][2] };

				CAi_PlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);


				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.25)
				{
					if (m_bDoubleEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_AIRTRAIL4, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bDoubleEffect = false;
						_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
						_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
						_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
						_vec3 test = *m_pTransform->Get_Angle();
						Engine::CGameObject* pGameObject = nullptr;
						pGameObject = CDsBullet::Create(Engine::Get_Device(), &(vPos + _vec3(0.f, 50.f, 0.f)), &test, vPos + _vec3(0.f, 50.f, 0.f) + vLook*500.f, Engine::EFFECTTYPE::EFFECT_AIRTRAIL4, m_pPlayer->Get_BaseInfo()->eObjectID);
						if (pGameObject == nullptr)
							return CAi_Player::ID_SKILL;
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"DSBullet", pGameObject);
					}
				}

				if (m_bEffect)
				{
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_AIRTRAIL3, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bEffect = false;
					_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
					_vec3 test = *m_pTransform->Get_Angle();
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = CDsBullet::Create(Engine::Get_Device(), &(vPos + _vec3(0.f, 50.f, 0.f)), &test, vPos + _vec3(0.f, 50.f, 0.f) + vLook*500.f, Engine::EFFECTTYPE::EFFECT_AIRTRAIL3, m_pPlayer->Get_BaseInfo()->eObjectID);
					if (pGameObject == nullptr)
						return CAi_Player::ID_SKILL;
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"DSBullet", pGameObject);
				}

				if (!isnan(fDegree))
				{
					if (fCrossy > 0.f)
						m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
				}
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AirComboLevel(3);
					m_pPlayer->Set_AnimationSpeed(1);
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::DS_AIRCOMBO_04)
			{
				_matrix matView = m_pPlayer->Get_ViewMatrix();
				_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vPos = m_pPlayer->GetPlayerPos();

				D3DXMatrixInverse(&matView, 0, &matView);
				D3DXVec3Normalize(&vLook, &vLook);

				_float fCrossy, fDegree;

				vPurpose = { matView.m[2][0],matView.m[2][1] ,matView.m[2][2] };

				CAi_PlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.1)
				{
					if (m_bDoubleEffect)
					{
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_AIRTRAIL5, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						m_bDoubleEffect = false;
						_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
						_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
						_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
						_vec3 test = *m_pTransform->Get_Angle();
						Engine::CGameObject* pGameObject = nullptr;
						pGameObject = CDsBullet::Create(Engine::Get_Device(), &(vPos + _vec3(0.f, 50.f, 0.f)), &test, vPos + _vec3(0.f, 50.f, 0.f) + vLook*500.f, Engine::EFFECTTYPE::EFFECT_AIRTRAIL5, m_pPlayer->Get_BaseInfo()->eObjectID);
						if (pGameObject == nullptr)
							return CAi_Player::ID_SKILL;
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"DSBullet", pGameObject);
					}
				}

				m_dbAcc += dTimeDelta;
				vPos = m_pPlayer->GetPlayerPos();
				vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK) * -1.f;
				_vec3 vUp = (*m_pTransform->Get_Info(Engine::INFO_UP) * -1.f);

				D3DXVec3Normalize(&vLook, &vLook);
				D3DXVec3Normalize(&vUp, &vUp);

				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 125.f*(_float)dTimeDelta * (_float)(1.5f - m_dbAcc)), m_dwNaviIndex, true)));
				vPos = m_pPlayer->GetPlayerPos();
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vUp * 375.f*(_float)(dTimeDelta*m_dbAcc)), m_dwNaviIndex, true)));

				if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
				{
					m_pNaviCom->Find_PosY(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex, vPos);
					m_pTransform->Set_Pos(&vPos);
					m_pPlayer->Set_Fall_End();
					m_pPlayer->Set_Jump_Finish();
					m_pPlayer->Set_AirComboLevel(0);
					m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_AIR_JUMPLANDING);
					return CAi_Player::ID_COMMON;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.15))
				{
					m_pPlayer->Set_AnimationSpeed(1);
					m_pPlayer->Set_AirComboLevel(0);
					return CAi_Player::ID_COMMON;
				}
				return CAi_Player::ID_AIRCOMBO;
			}
		}

		else if (m_eWeapon == CAi_Player::LONGBOW)
		{
			if (m_pPlayer->Get_CurState() == CAi_Player::LB_AIRCOMBO_01)
			{
				_matrix matView = m_pPlayer->Get_ViewMatrix();
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
						m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AirComboLevel(1);

					m_pPlayer->Set_AnimationSpeed(1);
					return CAi_Player::ID_COMMON;
				}
			}

			if (CAi_Player::LB_AIRCOMBOEX1_START == m_pPlayer->Get_CurState())
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_eReserveState = CAi_Player::LB_AIRCOMBOEX1_LOOP;
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBOEX1_LOOP);
					m_pPlayer->Set_State(CAi_Player::LB_AIRCOMBOEX1_LOOP);
					return CAi_Player::ID_AIRCOMBO;
				}
				return CAi_Player::ID_AIRCOMBO;
			}

			else if (CAi_Player::LB_AIRCOMBOEX1_END == m_pPlayer->Get_CurState())
			{
				if (m_pMeshCom->Get_TrackTime() <= m_pMeshCom->Get_Period())
				{
					_vec3 vPos = m_pPlayer->GetPlayerPos();
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					m_dbAcc += dTimeDelta * 3;
					vPos = m_pPlayer->GetPlayerPos();
					vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK) * -1.f;
					_vec3 vUp = (*m_pTransform->Get_Info(Engine::INFO_UP) * -1.f);

					D3DXVec3Normalize(&vLook, &vLook);
					D3DXVec3Normalize(&vUp, &vUp);

					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250.f*(_float)dTimeDelta * (_float)(1.5f - m_dbAcc)), m_dwNaviIndex, true)));
					vPos = m_pPlayer->GetPlayerPos();
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vUp * 375.f*(_float)(dTimeDelta*(m_dbAcc))), m_dwNaviIndex, true)));
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					return CAi_Player::ID_COMMON;
				}
				return CAi_Player::ID_AIRCOMBO;
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_AIRCOMBOEX1_LOOP)
			{
				if (m_eUp_Key == KEY_RB)
				{
					m_pPlayer->Set_State(CAi_Player::LB_AIRCOMBOEX1_END);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBOEX1_END);
					return CAi_Player::ID_AIRCOMBO;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					if (4 > m_iCount)
					{
						++m_iCount;
						m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBOEX1_END);
						m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBOEX1_LOOP);
						return CAi_Player::ID_AIRCOMBO;
					}
					else
					{
						m_pPlayer->Set_State(CAi_Player::LB_AIRCOMBOEX1_END);
						m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBOEX1_END);
						return CAi_Player::ID_AIRCOMBO;
					}
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_AIRCOMBOEX1_START)
			{
				m_pPlayer->Set_AirComboLevel(4);
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_State(CAi_Player::LB_AIRCOMBOEX1_LOOP);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBOEX1_LOOP);
					return CAi_Player::ID_AIRCOMBO;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_AIRCOMBO_02)
			{
				_matrix matView = m_pPlayer->Get_ViewMatrix();
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
						m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
				}

				if (m_eDown_Key == KEY_RB)
				{
					m_bChange = true;
				}

				if (m_bChange && m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
				{
					_matrix matView = m_pPlayer->Get_ViewMatrix();
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
							m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
						else
							m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
					}

					m_pPlayer->Set_State(CAi_Player::LB_AIRCOMBOEX1_START);
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBOEX1_START);
					return CAi_Player::ID_AIRCOMBO;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AirComboLevel(2);
					m_pPlayer->Set_AnimationSpeed(1);
					return CAi_Player::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CAi_Player::LB_AIRCOMBO_03)
			{
				_matrix matView = m_pPlayer->Get_ViewMatrix();
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
						m_pTransform->Rotation(Engine::ROT_Y, fDegree * (_float)dTimeDelta * 5.f);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree* (_float)dTimeDelta * 5.f);
				}

				m_dbAcc += dTimeDelta;
				vPos = m_pPlayer->GetPlayerPos();
				vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK) * -1.f;
				_vec3 vUp = (*m_pTransform->Get_Info(Engine::INFO_UP) * -1.f);

				D3DXVec3Normalize(&vLook, &vLook);
				D3DXVec3Normalize(&vUp, &vUp);

				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250.f*(_float)dTimeDelta * (_float)(1.5f - m_dbAcc)), m_dwNaviIndex, true)));
				vPos = m_pPlayer->GetPlayerPos();
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vUp * 375.f*(_float)(dTimeDelta*m_dbAcc)), m_dwNaviIndex, true)));

				if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
				{
					m_pNaviCom->Find_PosY(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex, vPos);
					m_pTransform->Set_Pos(&vPos);
					m_pPlayer->Set_Fall_End();
					m_pPlayer->Set_Jump_Finish();
					m_pPlayer->Set_AirComboLevel(0);
					m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_AIR_JUMPLANDING);
					return CAi_Player::ID_COMMON;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_AnimationSpeed(1);
					m_pPlayer->Set_AirComboLevel(0);
					return CAi_Player::ID_COMMON;
				}

			}

		}
		break;
	}
	default:
		break;
	}

	if (m_pMeshCom->Is_AnimationSetFinish(0.2))
	{
		m_pPlayer->Set_AnimationSpeed(1);
		m_pPlayer->Set_AirComboLevel(0);
		return CAi_Player::ID_COMMON;
	}

	if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
	{
		m_pNaviCom->Find_PosY(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex, vPos);
		m_pTransform->Set_Pos(&vPos);
		m_pPlayer->Set_Jump_Finish();
		m_pPlayer->Set_Fall_End();
		m_pPlayer->Set_AirComboLevel(0);
		m_pMeshCom->Set_AnimationSet(CAi_Player::COMMON_AIR_JUMPLANDING);

		return CAi_Player::ID_COMMON;
	}

	if (m_pPlayer->Get_Jump())
		m_pPlayer->Set_Fall_Start();

	return CAi_Player::ID_AIRCOMBO;
}

void CAi_AirCombo::TS_AirCombo(const _double dTimeDelta)
{
	if (m_bPass)
		return;
	switch (m_pPlayer->Get_AirComboLevel())
	{
	case CAi_Player::LEVEL1:
	{
		m_pPlayer->Set_AnimationSpeed(1.25);
		m_pMeshCom->Set_AnimationSet(CAi_Player::TS_AIR_COMBO_01);
		m_pPlayer->Set_State(CAi_Player::TS_AIR_COMBO_01);
		if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
		{
			_vec3 vPos = m_pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex, true)));
		}
		if (m_eDown_Key == KEY_LB)
		{
			m_bNextAtk = true;
			m_eEndType = TYPE_RESERVE;
			m_eReserveState = CAi_Player::TS_AIR_COMBO_02;
			//m_pPlayer->Set_AirComboLevel(1);
		}
		if (!m_bNextAtk)
			m_eEndType = TYPE_END;

	}
	break;
	case CAi_Player::LEVEL2:
	{
		m_pPlayer->Set_AnimationSpeed(1.25);
		m_pMeshCom->Set_AnimationSet(CAi_Player::TS_AIR_COMBO_02);
		m_pPlayer->Set_State(CAi_Player::TS_AIR_COMBO_02);

		if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
		{
			_vec3 vPos = m_pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex, true)));
		}
		if (m_eDown_Key == KEY_LB)
		{
			m_bNextAtk = true;
			m_eEndType = TYPE_RESERVE;
			m_eReserveState = CAi_Player::TS_AIR_COMBO_03;
			//m_pPlayer->Set_AirComboLevel(2);
		}
		if (!m_bNextAtk)
			m_eEndType = TYPE_END;
	}
	break;
	case CAi_Player::LEVEL3:
	{
		m_eEndType = TYPE_END;

		m_pPlayer->Set_AnimationSpeed(1.25);
		/*m_pPlayer->Set_Fall_End();
		m_pPlayer->Set_Jump_Finish();*/
		if (m_pNaviCom->Check_Landing(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex))
		{
			m_bCancel = true;
			return;
		}

		m_pMeshCom->Set_AnimationSet(CAi_Player::TS_AIR_COMBO_03);
		m_pPlayer->Set_State(CAi_Player::TS_AIR_COMBO_03);
		m_dbAcc = 0;
		if (m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.3)
		{
			_vec3 vPos = m_pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 250 * (_float)dTimeDelta), m_dwNaviIndex, true)));
		}
		m_bPass = true;
		//if (Engine::MouseDown(Engine::DIM_LB))
		//{
		//	m_bNextAtk = true;
		//	m_eEndType = TYPE_RESERVE;
		//	m_eReserveState = CAi_Player::TS_AIR_COMBO_04;
		//	//m_pPlayer->Set_AirComboLevel(3);
		//}
		//if (!m_bNextAtk)
		//	m_eEndType = TYPE_END;
	}
	break;
	case CAi_Player::LEVEL4:
	{
		/*_vec3 vPos = m_pPlayer->GetPlayerPos();
		_vec3 vUp = *m_pTransform->Get_Info(Engine::INFO_UP);
		_float fSpeed;
		m_pPlayer->Set_AnimationSpeed(0.45);

		m_dbJumpTime += dTimeDelta + m_pPlayer->Get_JumpTime();
		CAi_PlayerState::Calculate_FaillingSpeed(0.f, m_pPlayer->Get_Accelerate(), 150.f, m_dbJumpTime, 500, &fSpeed);

		if (!m_bFaill)
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vUp * 1.f*(_float)dTimeDelta), m_dwNaviIndex, true)));

		if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
		{
			m_bFaill = true;
			m_bPass = true;
			m_pNaviCom->Find_PosY(&m_pPlayer->GetPlayerPos(), m_dwNaviIndex, vPos);
			m_pTransform->Set_Pos(&vPos);
			m_eEndType = TYPE_END;
			m_pMeshCom->Set_AnimationSet(CAi_Player::TS_AIR_COMBO_04_LANDING);
			m_pPlayer->Set_State(CAi_Player::TS_AIR_COMBO_04_LANDING);
			return;
		}

		else
		{
			m_pMeshCom->Set_AnimationSet(CAi_Player::TS_AIR_COMBO_04);
			m_pPlayer->Set_State(CAi_Player::TS_AIR_COMBO_04);
		}
		m_eEndType = TYPE_END;*/
	}
	break;
	case CAi_Player::LEVEL5:
	{
		//m_pMeshCom->Set_AnimationSet(CAi_Player::DS_AIRCOMBO_04);
		//m_pPlayer->Set_State(CAi_Player::DS_AIRCOMBO_04);
		//m_eEndType = TYPE_END;
		//m_bPass = true;
	}
	break;
	case CAi_Player::LEVEL_END:
	{
		//m_pPlayer->Set_Fall_End();
		//m_pPlayer->Set_Jump_Finish();
		//m_pPlayer->Set_AirComboLevel(0);
		//m_eEndType = TYPE_RESERVE;
	}
	break;
	default:
		break;
	}
}

void CAi_AirCombo::DS_AirCombo(const _double dTimeDelta)
{
	Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_R", Engine::MESH, Engine::ID_STATIC));
	pWeaponCom->Set_AnimationSet(0);

	pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_L", Engine::MESH, Engine::ID_STATIC));
	pWeaponCom->Set_AnimationSet(0);

	if (m_bPass)
		return;

	switch (m_pPlayer->Get_AirComboLevel())
	{
	case CAi_Player::LEVEL1:
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::DS_AIRCOMBO_01);
		m_pPlayer->Set_State(CAi_Player::DS_AIRCOMBO_01);
		if (m_eDown_Key == KEY_LB)
		{
			m_bNextAtk = true;
			m_eEndType = TYPE_RESERVE;
			m_eReserveState = CAi_Player::DS_AIRCOMBO_02;
		}
		if (!m_bNextAtk)
			m_eEndType = TYPE_END;

	}
	break;
	case CAi_Player::LEVEL2:
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::DS_AIRCOMBO_02);
		m_pPlayer->Set_State(CAi_Player::DS_AIRCOMBO_02);
		if (m_eDown_Key == KEY_LB)
		{
			m_bNextAtk = true;
			m_eEndType = TYPE_RESERVE;
			m_eReserveState = CAi_Player::DS_AIRCOMBO_03;
		}
		if (!m_bNextAtk)
			m_eEndType = TYPE_END;
	}
	break;
	case CAi_Player::LEVEL3:
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::DS_AIRCOMBO_03);
		m_pPlayer->Set_State(CAi_Player::DS_AIRCOMBO_03);
		if (m_eDown_Key == KEY_LB)
		{
			m_bNextAtk = true;
			m_eEndType = TYPE_RESERVE;
			m_eReserveState = CAi_Player::DS_AIRCOMBO_04;
		}
		if (!m_bNextAtk)
			m_eEndType = TYPE_END;
	}
	break;
	case CAi_Player::LEVEL4:
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::DS_AIRCOMBO_04);
		m_pPlayer->Set_State(CAi_Player::DS_AIRCOMBO_04);
		m_eEndType = TYPE_END;
		m_bPass = true;
	}
	break;
	case CAi_Player::LEVEL_END:
	{
		//m_pPlayer->Set_Fall_End();
		//m_pPlayer->Set_Jump_Finish();
		//m_pPlayer->Set_AirComboLevel(0);
		//m_eEndType = TYPE_RESERVE;
	}
	break;
	default:
		break;
	}
}

void CAi_AirCombo::OB_AirCombo(const _double dTimeDelta)
{
	if (!m_bEnter)
	{
		switch (m_pPlayer->Get_AirComboLevel())
		{
		case CAi_Player::LEVEL1:
		{
			m_pPlayer->Set_AnimationSpeed(1.25);
			m_pMeshCom->Set_AnimationSet(CAi_Player::OB_AIR_COMBO_01);
			m_pPlayer->Set_State(CAi_Player::OB_AIR_COMBO_01);
			if (m_eDown_Key == KEY_LB)
			{
				m_bNextAtk = true;
				m_eEndType = TYPE_RESERVE;
				m_eReserveState = CAi_Player::OB_AIR_COMBO_02;
			}
			if (!m_bNextAtk)
				m_eEndType = TYPE_END;
		}
		break;
		case CAi_Player::LEVEL2:
		{
			m_pPlayer->Set_AnimationSpeed(1.25);
			m_dbAcc = 0;
			m_pMeshCom->Set_AnimationSet(CAi_Player::OB_AIR_COMBO_02);
			m_pPlayer->Set_State(CAi_Player::OB_AIR_COMBO_02);
			if (m_eDown_Key == KEY_LB)
			{
				m_bNextAtk = true;
				m_eEndType = TYPE_RESERVE;
				m_eReserveState = CAi_Player::OB_AIR_COMBO_03;
			}
			if (!m_bNextAtk)
				m_eEndType = TYPE_END;
		}
		break;
		case CAi_Player::LEVEL3:
		{
			m_pPlayer->Set_AnimationSpeed(1.25);
			m_pMeshCom->Set_AnimationSet(CAi_Player::OB_AIR_COMBO_03);
			m_pPlayer->Set_State(CAi_Player::OB_AIR_COMBO_03);

			if (m_eDown_Key == KEY_RB)
			{
				m_bNextAtk = true;
				m_eEndType = TYPE_END;
			}
			if (!m_bNextAtk)
				m_eEndType = TYPE_END;
		}

		break;
		case CAi_Player::LEVEL4:
		{
			m_pPlayer->Set_State(CAi_Player::OB_SPECIALMOVE);
			m_pMeshCom->Set_AnimationSet(CAi_Player::OB_SPECIALMOVE);
			m_iRepeat = 1;

			//if (Engine::MouseDown(Engine::DIM_RB))
			//{
			//	m_pPlayer->Set_AnimationSpeed(2);
			//	m_bNextAtk = true;
			//	m_eEndType = TYPE_RESERVE;
			//	m_eReserveState = CAi_Player::OB_SPECIALMOVE;
			//	//m_pPlayer->Set_AirComboLevel(4);
			//	m_iRepeat = 1;
			//}
			//if (!m_bNextAtk)
			//	m_eEndType = TYPE_END;
		}
		break;
		case CAi_Player::LEVEL_END:
		{
			m_pPlayer->Set_Fall_End();
			m_pPlayer->Set_Jump_Finish();
			m_pPlayer->Set_AirComboLevel(0);
			m_eEndType = TYPE_RESERVE;
		}
		break;
		default:
			break;
		}
	}
	else
	{
		if (CAi_Player::OB_AIR_COMBO_01 == m_pPlayer->Get_CurState())
		{
			if (m_eDown_Key == KEY_LB)
			{
				m_pPlayer->Set_AirComboLevel(1);
				m_eEndType = TYPE_RESERVE;
				m_eReserveState = CAi_Player::OB_AIR_COMBO_02;
			}
		}

		if (CAi_Player::OB_AIR_COMBO_02 == m_pPlayer->Get_CurState())
		{
			if (m_eDown_Key == KEY_LB)
			{
				m_pPlayer->Set_AirComboLevel(2);
				m_eEndType = TYPE_RESERVE;
				m_eReserveState = CAi_Player::OB_AIR_COMBO_03;
			}
		}

		if (CAi_Player::OB_AIR_COMBO_03 == m_pPlayer->Get_CurState())
		{
			//if (Engine::MouseDown(Engine::DIM_LB))
			//{
			//	m_pPlayer->Set_AirComboLevel(3);
			//	m_eEndType = TYPE_RESERVE;
			//	m_eReserveState = CAi_Player::OB_SPECIALMOVE;
			//}

			if (m_eDown_Key == KEY_RB)
			{
				m_eEndType = TYPE_RESERVE;
				//m_eReserveState = CAi_Player::OB_SPECIALMOVE;
				//m_pPlayer->Set_State(CAi_Player::OB_SPECIALMOVE);
				//m_pMeshCom->Set_AnimationSet(CAi_Player::OB_SPECIALMOVE);
				m_eReserveState = CAi_Player::OB_SPECIALMOVE;
				m_iRepeat = 1;
			}

		}
	}
}

void CAi_AirCombo::LB_AirCombo(const _double dTimeDelta)
{
	if (m_bPass)
		return;

	switch (m_pPlayer->Get_AirComboLevel())
	{
	case CAi_Player::LEVEL1:
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBO_01);
		m_pPlayer->Set_State(CAi_Player::LB_AIRCOMBO_01);
		if (m_eDown_Key == KEY_LB)
		{
			m_bNextAtk = true;
			m_bPass = true;
			m_eEndType = TYPE_RESERVE;
			m_eReserveState = CAi_Player::LB_AIRCOMBO_02;
			//m_pPlayer->Set_AirComboLevel(1);
		}
		if (!m_bNextAtk)
			m_eEndType = TYPE_END;

	}
	break;
	case CAi_Player::LEVEL2:
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBO_02);
		m_pPlayer->Set_State(CAi_Player::LB_AIRCOMBO_02);
		if (m_eDown_Key == KEY_LB)
		{
			m_bNextAtk = true;
			m_bPass = true;
			m_eEndType = TYPE_RESERVE;
			m_eReserveState = CAi_Player::LB_AIRCOMBO_03;
			//m_pPlayer->Set_AirComboLevel(2);
		}
		if (!m_bNextAtk)
			m_eEndType = TYPE_END;
	}
	break;
	case CAi_Player::LEVEL3:
	{
		m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBO_03);
		m_pPlayer->Set_State(CAi_Player::LB_AIRCOMBO_03);
		m_bPass = true;
		//if (Engine::MouseDown(Engine::DIM_RB))
		//{
		//	m_bNextAtk = true;
		//	m_eEndType = TYPE_RESERVE;
		//	m_eReserveState = CAi_Player::LB_AIRCOMBOEX1_START;
		//	//m_pPlayer->Set_AirComboLevel(3);
		//}
		if (!m_bNextAtk)
			m_eEndType = TYPE_END;
	}
	break;
	case CAi_Player::LEVEL4:
	{
		m_eEndType = TYPE_RESERVE;

		/*if (m_eWeapon == CAi_Player::LONGBOW)
		{
			if (CAi_Player::LB_AIRCOMBOEX1_START == m_pPlayer->Get_CurState())
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBOEX1_LOOP);
					m_pPlayer->Set_State(CAi_Player::LB_AIRCOMBOEX1_LOOP);
					m_eEndType = TYPE_RESERVE;
				}
			}

			else if (CAi_Player::LB_AIRCOMBOEX1_LOOP == m_pPlayer->Get_CurState())
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBOEX1_END);
					m_pPlayer->Set_State(CAi_Player::LB_AIRCOMBOEX1_END);
					m_eEndType = TYPE_RESERVE;
				}
			}
			if (CAi_Player::LB_AIRCOMBOEX1_END == m_pPlayer->Get_CurState())
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pMeshCom->Set_AnimationSet(CAi_Player::LB_AIRCOMBOEX1_END);
					m_pPlayer->Set_State(CAi_Player::LB_AIRCOMBOEX1_END);
					m_pPlayer->Set_AirComboLevel(4);
					m_eEndType = TYPE_END;
				}
			}
		}*/
	}
	break;
	case CAi_Player::LEVEL_END:
	{
		m_pPlayer->Set_Fall_End();
		m_pPlayer->Set_Jump_Finish();
		m_pPlayer->Set_AirComboLevel(0);
		m_eEndType = TYPE_RESERVE;
	}
	break;
	default:
		break;
	}
}

void CAi_AirCombo::Update_PlayerPattern(const _double dTimeDelta)
{
	switch (m_eWeapon)
	{
	case CAi_Player::COMMON:
		//ø©±‚ ø¿∏È ∏¡«‘
		break;
	case CAi_Player::DUALSWORD:
	{
		Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_R", Engine::MESH, Engine::ID_STATIC));
		pWeaponCom->Set_AnimationSet(0);

		pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_L", Engine::MESH, Engine::ID_STATIC));
		pWeaponCom->Set_AnimationSet(0);

		m_pPlayer->Set_AnimationSpeed(1.5);

		DS_AirCombo(dTimeDelta);
	}
	break;
	case CAi_Player::TWOHANDSWORD:
	{
		TS_AirCombo(dTimeDelta);
	}
	break;
	case CAi_Player::ORB:
	{
		OB_AirCombo(dTimeDelta);
	}
	break;

	case CAi_Player::LONGBOW:
		m_pPlayer->Set_AnimationSpeed(2);
		LB_AirCombo(dTimeDelta);
		break;
	default:
		break;
	}
}
