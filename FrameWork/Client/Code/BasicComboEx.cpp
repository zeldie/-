#include "stdafx.h"
#include "BasicComboEx.h"
#include "Decal.h"
CBasicComboEx::CBasicComboEx()
	:m_bEnter(false),
	m_eEndType(TYPE_END),
	m_eLevel(CPlayer::LEVEL_END),
	m_bReserveFinish(false),
	m_bPass(false),
	m_bJumpReserve(false),
	m_bLBDraw(false),
	m_dbHoldingTime(0),
	m_dbCharge(0),
	m_dbAcc(0),
	m_dbLoopTime(0.0),
	m_iCount(0),
	m_eStep(STEP1)
{
}

CBasicComboEx::~CBasicComboEx()
{
}

void CBasicComboEx::Enter(CPlayer * pPlayer)
{
}

void CBasicComboEx::Update(CPlayer * pPlayer, const _double dTimeDelta)
{
}

void CBasicComboEx::ChangeState(CPlayerState * pState)
{
}

void CBasicComboEx::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float * fCrossy, _vec3 * vDir, _float * fDegree)
{
}

void CBasicComboEx::Enter_State(CBasePlayer * pPlayer)
{
	CPlayerState::Enter_State(pPlayer);
	m_eLevel = m_pPlayer->Get_AtkLevel();
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
	}
}

void CBasicComboEx::Update_State(const _double dTimeDelta)
{
	Update_PlayerPattern(dTimeDelta);
}

CPlayer::STATE_ID CBasicComboEx::LateUpadte_State(const _double dTimeDelta)
{
	if (Engine::KeyDown(DIK_LSHIFT))
	{
		m_pPlayer->Set_DashReserve();
		return CPlayer::ID_COMMON;
	}

	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);
	_vec3 vPos = m_pPlayer->GetPlayerPos();
	
	switch (m_eEndType)
	{
	case CBasicComboEx::TYPE_LOOP:
	{
		return CPlayer::ID_BASICCOMBOEX;
	}
	break;
	case CBasicComboEx::TYPE_RESERVE:
	{
		if (!m_bEnter)
			m_bEnter = true;

		if (m_eWeapon == CPlayer::TWOHANDSWORD)
		{
			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				m_pPlayer->Set_State(m_eReserveState);
				m_eEndType = TYPE_END;
			}
		}
		else if (m_eWeapon == CPlayer::DUALSWORD)
		{
			if (m_pPlayer->Get_CurState() == CPlayer::DS_BASICCOMBOEX1)
			{
				if (m_bJumpReserve)
				{
					if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.35)
					{
						m_pPlayer->Set_Jump_Start();
						m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_AIR_START);
						m_pPlayer->Set_State(CPlayer::COMMON_AIR_START);
						WeaponDissolve();
						return CPlayer::ID_COMMON;
					}
				}
			}
			if (m_pMeshCom->Is_AnimationSetFinish(0.2))
			{
				m_pMeshCom->Set_AnimationSet(m_eReserveState);
				m_pPlayer->Set_State(m_eReserveState);
				m_eEndType = TYPE_END;
			}
		}
		else if (m_eWeapon == CPlayer::LONGBOW)
		{

		}

		else if (m_eWeapon == CPlayer::ORB)
		{

			if (m_pPlayer->Get_CurState() == CPlayer::COMMON_COMBATWAIT)
			{
				////cout << m_pMeshCom->Get_Period() << endl;
				if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.1)
				{
					WeaponDissolve();
					return CPlayer::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CPlayer::OB_BASICCOMBOEX2_01_F)
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

				CPlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

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
						m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR1, &vOrbCreatorPos1, &vOrbCreatorAngle1, nullptr, m_pPlayer->Get_ControlType());
						m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR2, &vOrbCreatorPos1, &vOrbCreatorAngle1, nullptr, m_pPlayer->Get_ControlType());
						m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR1, &vOrbCreatorPos2, &vOrbCreatorAngle2, nullptr, m_pPlayer->Get_ControlType());
						m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR2, &vOrbCreatorPos2, &vOrbCreatorAngle2, nullptr, m_pPlayer->Get_ControlType());
						m_bEffect = false;
						CSoundMgr::Get_Instance()->SoundOn(70);
					}
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vPurpose * 500 * (_float)dTimeDelta), m_dwNaviIndex)));
				}


				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pMeshCom->Set_AnimationSet(m_eReserveState);
					m_pPlayer->Set_State(m_eReserveState);
					m_eEndType = TYPE_RESERVE;
					return CPlayer::ID_BASICCOMBOEX;
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
	case CBasicComboEx::TYPE_END:
	{
		if (m_eWeapon == CPlayer::TWOHANDSWORD)
		{
			CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));

			if (m_pPlayer->Get_CurState() == CPlayer::TS_BASICCOMBOEX01)
			{
				if (m_bJumpReserve)
				{
					if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
					{
						m_pPlayer->Set_Jump_Start();
						m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_AIR_START);
						m_pPlayer->Set_State(CPlayer::COMMON_AIR_START);
						WeaponDissolve();
						return CPlayer::ID_COMMON;
					}
				}
			}

			if (m_pPlayer->Get_CurState() == CPlayer::TS_SPECIALCOMBO_CRASH)
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
						CSoundMgr::Get_Instance()->SoundOn(54);
					}
				}

			}
			else if (m_pPlayer->Get_CurState() == CPlayer::TS_SPECIALCOMBO_READY)
			{
				if(m_bActionZoom)
				{
					m_bActionZoom = false;
					_matrix matView = m_pPlayer->Get_ViewMatrix();
					_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					_vec3 vPos = m_pPlayer->GetPlayerPos();

					D3DXMatrixInverse(&matView, 0, &matView);
					D3DXVec3Normalize(&vLook, &vLook);

					_float fCrossy, fDegree;

					vPurpose = { matView.m[2][0],matView.m[2][1] ,matView.m[2][2] };

					CPlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);
					if (!isnan(fDegree))
					{
						if (fCrossy > 0.f)
							m_pTransform->Rotation(Engine::ROT_Y, fDegree);
						else
							m_pTransform->Rotation(Engine::ROT_Y, -fDegree);
					}
					CCameraMgr::GetInstance()->Set_ActionZoom();
				}
				//»Ò¡§ √ﬂ∞° - 2¡Ÿ
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));
				pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);

				if (Engine::MousePressing(Engine::DIM_RB))
					m_dbCharge += dTimeDelta;

				if (Engine::MouseUp(Engine::DIM_RB))
				{
					if (m_dbCharge >= m_pMeshCom->Get_Period()*0.5)
					{
						int i = 0;
						// full charge
					}
					CCameraMgr::GetInstance()->ReSet_ActionZoom();
					m_pPlayer->Set_State(CPlayer::TS_SPECIALCOMBO_CRASH);
					m_pMeshCom->Set_AnimationSet(CPlayer::TS_SPECIALCOMBO_CRASH);
					
					CSoundMgr::Get_Instance()->SoundOn(50);

					return CPlayer::ID_BASICCOMBOEX;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					CCameraMgr::GetInstance()->ReSet_ActionZoom();
					m_pPlayer->Set_State(CPlayer::TS_SPECIALCOMBO_CRASH);
					m_pMeshCom->Set_AnimationSet(CPlayer::TS_SPECIALCOMBO_CRASH);
					CSoundMgr::Get_Instance()->SoundOn(50);
					return CPlayer::ID_BASICCOMBOEX;
				}
			}

		}

		else if (m_eWeapon == CPlayer::DUALSWORD)
		{
			if (m_pPlayer->Get_CurState() == CPlayer::DS_BASICCOMBOEX1)
			{
				if (m_bJumpReserve)
				{
					if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.35)
					{
						m_pPlayer->Set_Jump_Start();
						m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_AIR_START);
						m_pPlayer->Set_State(CPlayer::COMMON_AIR_START);
						WeaponDissolve();
						return CPlayer::ID_COMMON;
					}
				}
			}

			if (m_pPlayer->Get_CurState() == CPlayer::DS_SPECIALCOMBO_END)
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
			if (m_pPlayer->Get_CurState() == CPlayer::DS_SPECIALCOMBO_CHARGE_LOOP)
			{
				if (Engine::MousePressing(Engine::DIM_RB))
					m_dbCharge += dTimeDelta;

				if (Engine::MouseUp(Engine::DIM_RB))
				{
					if (m_dbCharge >= 3)
					{
						int i = 0;
						// full charge
					}
					CCameraMgr::GetInstance()->ReSet_ActionZoom();
					m_pPlayer->Set_State(CPlayer::DS_SPECIALCOMBO_END);
					m_pMeshCom->Set_AnimationSet(CPlayer::DS_SPECIALCOMBO_END);
					CSoundMgr::Get_Instance()->SoundOn(43);
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
					return CPlayer::ID_BASICCOMBOEX;
				}

				if (m_dbCharge >= 5)
				{
					CCameraMgr::GetInstance()->ReSet_ActionZoom();
					m_pPlayer->Set_State(CPlayer::DS_SPECIALCOMBO_END);
					m_pMeshCom->Set_AnimationSet(CPlayer::DS_SPECIALCOMBO_END);
					CSoundMgr::Get_Instance()->SoundOn(43);
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
					return CPlayer::ID_BASICCOMBOEX;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pMeshCom->Set_AnimationSet(CPlayer::DS_SPECIALCOMBO_END);
					m_pMeshCom->Set_AnimationSet(CPlayer::DS_SPECIALCOMBO_CHARGE_LOOP);
				}
			}

			if (m_pPlayer->Get_CurState() == CPlayer::DS_SPECIALCOMBO_CHARGE)
			{
				if (Engine::MousePressing(Engine::DIM_RB))
					m_dbCharge += dTimeDelta;

				if (Engine::MouseUp(Engine::DIM_RB))
				{
					if (m_dbCharge >= 3)
					{
						int i = 0;
						// full charge
					}
					CCameraMgr::GetInstance()->ReSet_ActionZoom();
					m_pPlayer->Set_State(CPlayer::DS_SPECIALCOMBO_END);
					m_pMeshCom->Set_AnimationSet(CPlayer::DS_SPECIALCOMBO_END);
					CSoundMgr::Get_Instance()->SoundOn(43);
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
					return CPlayer::ID_BASICCOMBOEX;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					// full charge
					m_pPlayer->Set_State(CPlayer::DS_SPECIALCOMBO_CHARGE_LOOP);
					m_pMeshCom->Set_AnimationSet(CPlayer::DS_SPECIALCOMBO_CHARGE_LOOP);
					return CPlayer::ID_BASICCOMBOEX;
				}
			}

		}

		else if (m_eWeapon == CPlayer::ORB)
		{
			if (m_pPlayer->Get_CurState() == CPlayer::OB_SPECIALCOMBO_READY)
			{

				if (Engine::MousePressing(Engine::DIM_RB))
				{
					Engine::CGameObject* pTerrain = nullptr;
					pTerrain = Engine::Get_GameObject(Engine::GAMEOBJECT, L"Terrain");
					m_pCalculatorCom->PickDecalPos(g_hWnd, pTerrain, m_pPlayer->Get_DecalPos());
					//m_pPlayer->RotationToDecal();
					if (m_bEffect)
					{
						m_pPlayer->Create_Decal(CDecal::DECAL_SKILLTARGET, 100);
						m_bEffect = false;
					}
					m_dbCharge += dTimeDelta;
				}
					

				if (Engine::MouseUp(Engine::DIM_RB))
				{
					m_pPlayer->Release_Decal();
					m_pPlayer->Create_Decal(CDecal::DECAL_ORB_RIGHT, 2.7);
					if (m_dbCharge >= m_pMeshCom->Get_Period()*0.5)
					{
						int i = 0;
						// full charge
					}
					CCameraMgr::GetInstance()->ReSet_ActionZoom();
					m_pPlayer->Set_State(CPlayer::OB_DASHCOMBO);
					m_pMeshCom->Set_AnimationSet(CPlayer::OB_DASHCOMBO);
					CSoundMgr::Get_Instance()->SoundOn(73);
					return CPlayer::ID_BASICCOMBOEX;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					// full charge
					CCameraMgr::GetInstance()->ReSet_ActionZoom();
					m_pPlayer->Set_State(CPlayer::OB_DASHCOMBO);
					m_pMeshCom->Set_AnimationSet(CPlayer::OB_DASHCOMBO);
					CSoundMgr::Get_Instance()->SoundOn(73);
					return CPlayer::ID_BASICCOMBOEX;
				}
			}
			else if (m_pPlayer->Get_CurState() == CPlayer::OB_BASICCOMBOEX1)
			{
				//∆Ú∆Ú∆Ú∆Ú øÏ≈¨∏Ø
				if (m_bEffect)
				{
					Engine::CGameObject* pGameObject = nullptr;
					if (Get_FindTarget())
					{
						m_pPlayer->Set_DecalPos(&m_vTargetPos);
						m_pPlayer->Create_Decal(CDecal::DECAL_ORB_FLYING, 1.5);
					}
					else
					{
						m_pPlayer->Set_DecalPos(&vPos);
						m_pPlayer->Create_Decal(CDecal::DECAL_ORB_FLYING, 1.5);
					}
					CSoundMgr::Get_Instance()->SoundOn(71);
					m_bEffect = false;
				}

			}
		}


		else if (m_eWeapon == CPlayer::LONGBOW)
		{
			if (m_pPlayer->Get_CurState() == CPlayer::LB_BASICCOMBOEX1_END)
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
					return CPlayer::ID_COMMON;
				}
				else
					return CPlayer::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CPlayer::LB_BASICCOMBOEX1_START)
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_State(CPlayer::LB_BASICCOMBOEX1_LOOP);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX1_LOOP);
					return CPlayer::ID_BASICCOMBOEX;
				}
			}
			else if (m_pPlayer->Get_CurState() == CPlayer::LB_BASICCOMBOEX1_LOOP)
			{
				m_pPlayer->Set_AnimationSpeed(0.75);

				if (Engine::MouseUp(Engine::DIM_LB))
				{
					m_pPlayer->Set_AnimationSpeed(1);
					m_pPlayer->Set_State(CPlayer::LB_BASICCOMBOEX1_END);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX1_END);
					return CPlayer::ID_BASICCOMBOEX;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.015))
				{
					if (5 < m_iCount)
					{
						m_pPlayer->Set_AnimationSpeed(1);
						m_pPlayer->Set_State(CPlayer::LB_BASICCOMBOEX1_END);
						m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX1_END);
						Engine::CGameObject* pGameObject = nullptr;
						pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
						pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + (vLook + vRight) * 0.5f + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
						pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + (vLook - vRight) * 0.5f + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
						Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						
						_uint iSound = rand() % 6 + 57;
						CSoundMgr::Get_Instance()->SoundOn(iSound);

						return CPlayer::ID_BASICCOMBOEX;
					}
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					// full charge
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX1_END);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX1_LOOP);
					++m_iCount;
					_uint iSound = rand() % 6 + 57;
					CSoundMgr::Get_Instance()->SoundOn(iSound);
					return CPlayer::ID_BASICCOMBOEX;
				}
				return CPlayer::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CPlayer::LB_BASICCOMBOEX3_02_DOWN)
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
					return CPlayer::ID_COMMON;
				}
				else
					return CPlayer::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CPlayer::LB_BASICCOMBOEX3_02_CHARGESHOT)
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
					return CPlayer::ID_COMMON;
				}
				else
					return CPlayer::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CPlayer::LB_BASICCOMBOEX3_02_CHARGE)
			{
				//m_pPlayer->Set_AnimationSpeed(0.75);
				m_dbCharge += dTimeDelta;

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					// full charge
					m_pPlayer->Set_State(CPlayer::LB_BASICCOMBOEX3_02_CHARGESHOT);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX3_02_CHARGESHOT);
					return CPlayer::ID_BASICCOMBOEX;
				}

				if (Engine::MouseUp(Engine::DIM_RB))
				{
					if (m_dbCharge >= m_pMeshCom->Get_Period()*0.5)
						int i = 0; // full charge

					m_pPlayer->Set_State(CPlayer::LB_BASICCOMBOEX3_02_CHARGESHOT);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX3_02_CHARGESHOT);

					return CPlayer::ID_BASICCOMBOEX;
				}

				else
					return CPlayer::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CPlayer::LB_BASICCOMBOEX3_01)
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

						_uint iSound = rand() % 6 + 57;
						CSoundMgr::Get_Instance()->SoundOn(iSound);
					}
					m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vLook * 500 * (_float)((1.2f - m_dbAcc) * dTimeDelta)), m_dwNaviIndex)));
				}


				if (Engine::MouseDown(Engine::DIM_RB))
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

					CPlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

					if (!isnan(fDegree))
					{
						if (fCrossy > 0.f)
							m_pTransform->Rotation(Engine::ROT_Y, fDegree);
						else
							m_pTransform->Rotation(Engine::ROT_Y, -fDegree);
					}*/

					if (Engine::MousePressing(Engine::DIM_RB))
					{
						m_pPlayer->Set_State(CPlayer::LB_BASICCOMBOEX3_02_CHARGE);
						m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX3_02_CHARGE);
						CSoundMgr::Get_Instance()->SoundOn(63);
						return CPlayer::ID_BASICCOMBOEX;
					}

					m_pPlayer->Set_State(CPlayer::LB_BASICCOMBOEX3_02);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX3_02);
					return CPlayer::ID_BASICCOMBOEX;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					WeaponDissolve();
					return CPlayer::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CPlayer::LB_BASICCOMBOEX2_END)
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
					return CPlayer::ID_COMMON;
				}
			}

			else if (m_pPlayer->Get_CurState() == CPlayer::LB_BASICCOMBOEX2_START)
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_dbAcc = 0;
					m_pPlayer->Set_State(CPlayer::LB_BASICCOMBOEX2_END);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX2_END);
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

					_uint iSound = rand() % 6 + 57;
					CSoundMgr::Get_Instance()->SoundOn(iSound);

					return CPlayer::ID_BASICCOMBOEX;
				}
			}

			else if (m_pPlayer->Get_CurState() == CPlayer::LB_SPECIALCOMBO_CHARGESHOT)
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					if (Engine::MousePressing(Engine::DIM_RB))
					{
						m_pPlayer->Set_State(CPlayer::LB_SPECIALCOMBO_WAIT);
						m_pMeshCom->Set_AnimationSet(CPlayer::LB_SPECIALCOMBO_WAIT);
						return CPlayer::ID_BASICCOMBOEX;
					}
					else
					{
						CCameraMgr::GetInstance()->ReSet_ActionZoom();
						WeaponDissolve();
						return CPlayer::ID_COMMON;
					}
				}
				return CPlayer::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CPlayer::LB_SPECIALCOMBO_CHARGE)
			{
				m_dbCharge += dTimeDelta;

				if (0.25 <= m_dbCharge && !m_bLBDraw)
				{
					CSoundMgr::Get_Instance()->SoundOn(63);
					m_bLBDraw = true;
				}

				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					// full charge
					m_pPlayer->Set_State(CPlayer::LB_SPECIALCOMBO_CHARGESHOT);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_SPECIALCOMBO_CHARGESHOT);
					m_bLBDraw = false;
					m_dbCharge = 0.0;
					return CPlayer::ID_BASICCOMBOEX;
				}

				if (!Engine::MousePressing(Engine::DIM_LB))
				{
					if (m_dbCharge >= m_pMeshCom->Get_Period()*0.5)
						int i = 0; // full charge
					Engine::CGameObject* pGameObject = nullptr;
					pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
					Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_pPlayer->Set_State(CPlayer::LB_SPECIALCOMBO_CHARGESHOT);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_SPECIALCOMBO_CHARGESHOT);
					
					_uint iSound = rand() % 6 + 57;
					CSoundMgr::Get_Instance()->SoundOn(iSound);
					m_bLBDraw = false;
					m_dbCharge = 0.0;
					return CPlayer::ID_BASICCOMBOEX;
				}

				return CPlayer::ID_BASICCOMBOEX;
			}
			else if (m_pPlayer->Get_CurState() == CPlayer::LB_SPECIALCOMBO_WAIT)
			{
				if (!Engine::MousePressing(Engine::DIM_RB))
				{
					CCameraMgr::GetInstance()->ReSet_ActionZoom();
					m_pPlayer->Set_State(CPlayer::LB_SPECIALCOMBO_END);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_SPECIALCOMBO_END);
					return CPlayer::ID_BASICCOMBOEX;
				}

				if (Engine::MousePressing(Engine::DIM_LB))
				{
					m_pPlayer->Set_State(CPlayer::LB_SPECIALCOMBO_CHARGE);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_SPECIALCOMBO_CHARGE);
					//CSoundMgr::Get_Instance()->SoundOn(63);
					return CPlayer::ID_BASICCOMBOEX;
				}

				return CPlayer::ID_BASICCOMBOEX;
			}

			else if (m_pPlayer->Get_CurState() == CPlayer::LB_SPECIALCOMBO_READY)
			{
				if (m_pMeshCom->Is_AnimationSetFinish(0.2))
				{
					m_pPlayer->Set_State(CPlayer::LB_SPECIALCOMBO_WAIT);
					m_pMeshCom->Set_AnimationSet(CPlayer::LB_SPECIALCOMBO_WAIT);
					return CPlayer::ID_BASICCOMBOEX;
				}
				return CPlayer::ID_BASICCOMBOEX;
			}

		}

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}
	}
	break;
	default:
		break;
	}
	return CPlayer::ID_BASICCOMBOEX;
}

void CBasicComboEx::Update_PlayerPattern(const _double dTimeDelta)
{
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
			if (m_eWeapon == CPlayer::TWOHANDSWORD)
			{
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));
				pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);
				Set_Effect();
				m_bDoubleEffect = true;
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_BASICCOMBOEX01);
				m_pPlayer->Set_State(CPlayer::TS_BASICCOMBOEX01);
				m_eEndType = TYPE_RESERVE;
			}
			else if (m_eWeapon == CPlayer::DUALSWORD)
			{
				CDualSword* pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_L"));
				pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

				pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_R"));
				pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

				if (Engine::KeyDown(DIK_SPACE))
				{
					m_bJumpReserve = true;
				}
				if (m_bEffect)
				{
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_AIRUP, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_bEffect = false;
					CSoundMgr::Get_Instance()->SoundOn(48);
				}

				if (m_pMeshCom->Get_TrackTime() > m_pMeshCom->Get_Period()*0.2)
				{
					if (m_bDoubleEffect)
					{
						m_bDoubleEffect = false;
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_AIRUP2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
						CSoundMgr::Get_Instance()->SoundOn(48);
					}
				}

				m_pMeshCom->Set_AnimationSet(CPlayer::DS_BASICCOMBOEX1);
				m_pPlayer->Set_State(CPlayer::DS_BASICCOMBOEX1);

				m_eEndType = TYPE_RESERVE;
			}
			else if (m_eWeapon == CPlayer::LONGBOW)
			{
				CLongBow* pWeapon = dynamic_cast<CLongBow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"LB"));
				pWeapon->Set_CombatState(CLongBow::STATE_COMBAT);

				//ø©±‚∞° µ⁄∑Œ π–∑¡≥™∞®
				_matrix matView = m_pPlayer->Get_ViewMatrix();
				_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vPos = m_pPlayer->GetPlayerPos();

				D3DXMatrixInverse(&matView, 0, &matView);
				D3DXVec3Normalize(&vLook, &vLook);

				_float fCrossy, fDegree;

				vPurpose = { matView.m[2][0],matView.m[2][1] ,matView.m[2][2] };

				CPlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

				if (!isnan(fDegree))
				{
					if (fCrossy > 0.f)
						m_pTransform->Rotation(Engine::ROT_Y, fDegree);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree);
				}

				m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX3_01);
				m_pPlayer->Set_State(CPlayer::LB_BASICCOMBOEX3_01);
				m_eEndType = TYPE_END;
				m_bPass = true;
			}

		}
		break;
		case CBasePlayer::LEVEL2:
		{
			if (m_eWeapon == CPlayer::TWOHANDSWORD)
			{
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));
				pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);
				Set_Effect();
				m_bDoubleEffect = true;
				m_eEndType = TYPE_RESERVE;
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_BASICCOMBOEX02_START);
				m_pPlayer->Set_State(CPlayer::TS_BASICCOMBOEX02_START);
				CSoundMgr::Get_Instance()->SoundOn(5);
				m_pPlayer->Set_AnimationSpeed(2);
			}

			else if (m_eWeapon == CPlayer::DUALSWORD)
			{
				CDualSword* pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_L"));
				pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

				pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_R"));
				pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

				m_eEndType = TYPE_RESERVE;
				m_pMeshCom->Set_AnimationSet(CPlayer::DS_BASICCOMBOEX2_START);
				m_pPlayer->Set_State(CPlayer::DS_BASICCOMBOEX2_START);
			}

			else if (m_eWeapon == CPlayer::LONGBOW)
			{
				CLongBow* pWeapon = dynamic_cast<CLongBow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"LB"));
				pWeapon->Set_CombatState(CLongBow::STATE_COMBAT);

				//ø©±‚∞° ø¨πﬂ Ω√¿€
				m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX1_START);
				m_pPlayer->Set_State(CPlayer::LB_BASICCOMBOEX1_START);
				m_eEndType = TYPE_RESERVE;
				m_bPass = true;
			}

			else if (m_eWeapon == CPlayer::ORB)
			{
				//ø©±‚∞° ∞≠∞¯∞›
				if (m_bActionZoom)
				{
					m_bActionZoom = false;
					_matrix matView = m_pPlayer->Get_ViewMatrix();
					_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					_vec3 vPos = m_pPlayer->GetPlayerPos();

					D3DXMatrixInverse(&matView, 0, &matView);
					D3DXVec3Normalize(&vLook, &vLook);

					_float fCrossy, fDegree;

					vPurpose = { matView.m[2][0],matView.m[2][1] ,matView.m[2][2] };

					CPlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

					if (!isnan(fDegree))
					{
						if (fCrossy > 0.f)
							m_pTransform->Rotation(Engine::ROT_Y, fDegree);
						else
							m_pTransform->Rotation(Engine::ROT_Y, -fDegree);
					}
					CCameraMgr::GetInstance()->Set_ActionZoom();
				}
				m_eEndType = TYPE_END;
				m_bPass = true;
				m_pMeshCom->Set_AnimationSet(CPlayer::OB_SPECIALCOMBO_READY);
				m_pPlayer->Set_State(CPlayer::OB_SPECIALCOMBO_READY);
			}
		}
		break;
		case CBasePlayer::LEVEL3:
		{
			if (m_eWeapon == CPlayer::ORB)
			{
				//∆Ú∆Ú∆Ú øÏ≈¨∏Ø
				m_eEndType = TYPE_RESERVE;
				m_pMeshCom->Set_AnimationSet(CPlayer::OB_BASICCOMBOEX2_01_F);
				m_pPlayer->Set_State(CPlayer::OB_BASICCOMBOEX2_01_F);
				m_eReserveState = CPlayer::COMMON_COMBATWAIT;
				m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBOEX02);
			}

			else if (m_eWeapon == CPlayer::LONGBOW)
			{
				CLongBow* pWeapon = dynamic_cast<CLongBow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"LB"));
				pWeapon->Set_CombatState(CLongBow::STATE_COMBAT);

				//ø©±‚∞° ∞¯¡ﬂ
				_matrix matView = m_pPlayer->Get_ViewMatrix();
				_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vPos = m_pPlayer->GetPlayerPos();

				D3DXMatrixInverse(&matView, 0, &matView);
				D3DXVec3Normalize(&vLook, &vLook);

				_float fCrossy, fDegree;

				vPurpose = { matView.m[2][0],matView.m[2][1] ,matView.m[2][2] };

				CPlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

				if (!isnan(fDegree))
				{
					if (fCrossy > 0.f)
						m_pTransform->Rotation(Engine::ROT_Y, fDegree);
					else
						m_pTransform->Rotation(Engine::ROT_Y, -fDegree);
				}

				m_pMeshCom->Set_AnimationSet(CPlayer::LB_BASICCOMBOEX2_START);
				m_pPlayer->Set_State(CPlayer::LB_BASICCOMBOEX2_START);
				m_eEndType = TYPE_END;
				m_bPass = true;
			}
		}
		break;
		case CBasePlayer::LEVEL4:
		{
			if (m_eWeapon == CPlayer::ORB)
			{
				//∆Ú∆Ú∆Ú∆ÚøÏ≈¨∏Ø
				m_eEndType = TYPE_END;
				m_pMeshCom->Set_AnimationSet(CPlayer::OB_BASICCOMBOEX1);
				m_pPlayer->Set_State(CPlayer::OB_BASICCOMBOEX1);
				m_pPlayer->Set_OrbAnimation(COrb::BASICCOMBOEX01);
			}

			else if (m_eWeapon == CPlayer::TWOHANDSWORD)
			{
				Set_Effect();
				m_bDoubleEffect = true;
				m_eEndType = TYPE_END;
				m_bPass = true;
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_SPECIALCOMBO_READY);
				m_pPlayer->Set_State(CPlayer::TS_SPECIALCOMBO_READY);
			}

			else if (m_eWeapon == CPlayer::DUALSWORD)
			{
				CDualSword* pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_L"));
				pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

				pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_R"));
				pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

				if (m_bActionZoom)
				{
					m_bActionZoom = false;
					_matrix matView = m_pPlayer->Get_ViewMatrix();
					_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					_vec3 vPos = m_pPlayer->GetPlayerPos();

					D3DXMatrixInverse(&matView, 0, &matView);
					D3DXVec3Normalize(&vLook, &vLook);

					_float fCrossy, fDegree;

					vPurpose = { matView.m[2][0],matView.m[2][1] ,matView.m[2][2] };

					CPlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

					if (!isnan(fDegree))
					{
						if (fCrossy > 0.f)
							m_pTransform->Rotation(Engine::ROT_Y, fDegree);
						else
							m_pTransform->Rotation(Engine::ROT_Y, -fDegree);
					}
					CCameraMgr::GetInstance()->Set_ActionZoom();
				}

				m_eEndType = TYPE_END;
				m_bPass = true;
				m_pMeshCom->Set_AnimationSet(CPlayer::DS_SPECIALCOMBO_CHARGE);
				m_pPlayer->Set_State(CPlayer::DS_SPECIALCOMBO_CHARGE);
			}

			else if (m_eWeapon == CPlayer::LONGBOW)
			{
				CLongBow* pWeapon = dynamic_cast<CLongBow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"LB"));
				pWeapon->Set_CombatState(CLongBow::STATE_COMBAT);
				if (m_bActionZoom)
				{
					m_bActionZoom = false;
					_matrix matView = m_pPlayer->Get_ViewMatrix();
					_vec3 vPurpose = { 0.f,0.f,0.f }, vDir = { 0.f,0.f,0.f };
					_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
					_vec3 vPos = m_pPlayer->GetPlayerPos();

					D3DXMatrixInverse(&matView, 0, &matView);
					D3DXVec3Normalize(&vLook, &vLook);

					_float fCrossy, fDegree;

					vPurpose = { matView.m[2][0],matView.m[2][1] ,matView.m[2][2] };

					CPlayerState::Rotation_Direction(matView, vLook, vPurpose, &fCrossy, &vDir, &fDegree);

					if (!isnan(fDegree))
					{
						if (fCrossy > 0.f)
							m_pTransform->Rotation(Engine::ROT_Y, fDegree);
						else
							m_pTransform->Rotation(Engine::ROT_Y, -fDegree);
					}
					CCameraMgr::GetInstance()->Set_ActionZoom();
				}
				//ø©±‚∞° ¡‹
				m_eEndType = TYPE_END;
				m_bPass = true;
				m_pMeshCom->Set_AnimationSet(CPlayer::LB_SPECIALCOMBO_READY);
				m_pPlayer->Set_State(CPlayer::LB_SPECIALCOMBO_READY);
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
			if (m_eWeapon == CPlayer::TWOHANDSWORD)
			{
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));

				if (Engine::KeyDown(DIK_SPACE))
				{
 					m_bJumpReserve = true;
				}

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
					CSoundMgr::Get_Instance()->SoundOn(53);
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
			else if (m_eWeapon == CPlayer::DUALSWORD)
			{
				if (Engine::KeyDown(DIK_SPACE))
				{
					m_bJumpReserve = true;
				}
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
			if (CPlayer::TS_BASICCOMBOEX02_END != m_pPlayer->Get_CurState())
			{
				m_dbLoopTime += dTimeDelta;
				if (0.45 <= m_dbLoopTime)
				{
					m_dbLoopTime = 0.0;
					CSoundMgr::Get_Instance()->SoundOn(53);
				}
			}

			if (m_eWeapon == CPlayer::TWOHANDSWORD)
			{
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));

				if (CPlayer::TS_BASICCOMBOEX02_START == m_pPlayer->Get_CurState())
				{
					m_eReserveState = CPlayer::TS_BASICCOMBOEX02_LOOP;
					m_eEndType = TYPE_RESERVE;

					////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
					if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.3f)
					{
						if (m_bEffect)
						{							
							//π´±‚- ±√±ÿ±‚ off
							if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_PRESSING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
							//π´±‚- ±√±ÿ±‚ on
							else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_PRESSING_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

							m_bEffect = false;
							//CSoundMgr::Get_Instance()->SoundOn(53);
						}
					}
					///////////////////////////////

				}

				else if (CPlayer::TS_BASICCOMBOEX02_LOOP == m_pPlayer->Get_CurState())
				{
					m_pPlayer->Set_AnimationSpeed(2);
					m_dbHoldingTime += dTimeDelta;
					//m_eEndType = TYPE_RESERVE;

					////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
					if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.3f)
					{
						if (m_bEffect)
						{							
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
						//m_bPass = true;
						m_eEndType = TYPE_END;
						m_pMeshCom->Set_AnimationSet(CPlayer::TS_BASICCOMBOEX02_END);
						m_pPlayer->Set_State(CPlayer::TS_BASICCOMBOEX02_END);
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
						return;

						////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ////	
						if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.25f)
						{
							if (m_bDoubleEffect)
							{								
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

					if (Engine::MousePressing(Engine::DIM_LB))
					{
						if (2 <= m_dbHoldingTime)
						{
							m_eEndType = TYPE_RESERVE;
							m_eReserveState = CPlayer::TS_BASICCOMBOEX02_END;
						}
						else
						{
							m_eEndType = TYPE_LOOP;
							m_eReserveState = CPlayer::TS_BASICCOMBOEX02_LOOP;
						}
					}

					if (Engine::MouseUp(Engine::DIM_LB))
					{
						if (2 <= m_dbHoldingTime)
						{
							m_bPass = true;
							m_pMeshCom->Set_AnimationSet(CPlayer::TS_BASICCOMBOEX02_END);
							m_pPlayer->Set_State(CPlayer::TS_BASICCOMBOEX02_END);
							//m_eReserveState = CPlayer::TS_BASICCOMBOEX02_END;
						}
						else
						{
							m_bReserveFinish = true;
							m_eEndType = TYPE_LOOP;
							m_eReserveState = CPlayer::TS_BASICCOMBOEX02_LOOP;
						}
					}

					if (m_bReserveFinish && 2 <= m_dbHoldingTime)
					{
						m_bPass = true;
						m_pMeshCom->Set_AnimationSet(CPlayer::TS_BASICCOMBOEX02_END);
						m_pPlayer->Set_State(CPlayer::TS_BASICCOMBOEX02_END);
						m_dbHoldingTime += dTimeDelta;
						CSoundMgr::Get_Instance()->SoundOn(51);
					}
				}
			}

			else if (m_eWeapon == CPlayer::DUALSWORD)
			{
				if (CPlayer::DS_BASICCOMBOEX2_START == m_pPlayer->Get_CurState())
				{
					m_eReserveState = CPlayer::DS_BASICCOMBOEX2_LOOP;
					m_eEndType = TYPE_RESERVE;
				}

				else if (CPlayer::DS_BASICCOMBOEX2_LOOP == m_pPlayer->Get_CurState())
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
						m_eReserveState = CPlayer::DS_BASICCOMBOEX2_END;
					}

					if (Engine::MousePressing(Engine::DIM_LB))
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
								CSoundMgr::Get_Instance()->SoundOn(48);
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
								CSoundMgr::Get_Instance()->SoundOn(48);
							}
							else if (4 == m_iCount)
							{
								m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICSP5, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
								m_iCount = 0;
								CSoundMgr::Get_Instance()->SoundOn(48);
							}
						}

						if (3 <= m_dbHoldingTime)
						{
							m_eEndType = TYPE_RESERVE;
							m_eReserveState = CPlayer::DS_BASICCOMBOEX2_END;
						}
						else
						{
							m_eEndType = TYPE_LOOP;
							m_eReserveState = CPlayer::DS_BASICCOMBOEX2_LOOP;
						}
					}

					if (Engine::MouseUp(Engine::DIM_LB))
					{
						if (2 <= m_dbHoldingTime)
						{
							m_eEndType = TYPE_RESERVE;
							m_eReserveState = CPlayer::DS_BASICCOMBOEX2_END;
						}
						else
						{
							m_bReserveFinish = true;
							m_eEndType = TYPE_LOOP;
							m_eReserveState = CPlayer::DS_BASICCOMBOEX2_LOOP;
						}

					}
				}

				else if (CPlayer::DS_BASICCOMBOEX2_END == m_pPlayer->Get_CurState())
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
