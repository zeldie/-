#include "stdafx.h"
#include "PlayerSkill.h"
#include "Phoenix.h"
#include "FateShot.h"
#include "Genocide.h"
#include "DSBullet.h"

CPlayerSkill::CPlayerSkill()
	:m_bReserveFinish(false),
	m_bIsFinish(false),
	m_bPass(false),
	m_bLeapAtk(false),
	m_bSwapCam(true),
	m_bLBRage(true),
	m_eSequnce(SEQUENCE_END),
	m_dbPush(0),
	m_dbMinTime(0),
	m_dbLeapTime(0),
	m_dbKillingTime(0),
	m_dbBindingTime(0.0),
	m_iCount(0),
	fLength(0.f),
	m_dbTS_ESkill_Time_Right(0.113f),
	m_dbTS_ESkill_Time_Left(0.378f),
	m_dbTS_Time(0.15f),
	m_bTS_ESkill_CreateEffect_Right(false),
	m_bTS_ESkill_CreateEffect_Left(false),
	m_bTS_ESkill_Check_Right(false),
	m_bTS_ESkill_Check_Left(false),
	m_iTS_ESkill_Cnt(0),
	m_dArrowTerm(0.),
	m_dDustTime(0.),
	m_bDS_ESkill_Trail1(true),
	m_bDS_ESkill_Trail2(true),
	m_bDS_ESkill_Trail3(true),
	m_bDS_ESkill_Trail4(true),
	m_bDS_ESkill_Trail5(true),
	m_bDS_ESkill_Trail6(true)
{
}

CPlayerSkill::~CPlayerSkill()
{
}

void CPlayerSkill::Enter(CPlayer * pPlayer)
{
	CPlayerState::Enter_State(pPlayer);
	if (Engine::SM_F == m_pPlayer->Get_ReserveAction() && m_eWeapon == CPlayer::TWOHANDSWORD)
		m_eSequnce = SEQUENCE_1;
	else
		m_eSequnce = SEQUENCE_END;
}

void CPlayerSkill::Update(CPlayer * pPlayer, const _double dTimeDelta)
{
}

void CPlayerSkill::ChangeState(CPlayerState * pState)
{
}

void CPlayerSkill::Rotation_Direction(const _matrix matView, const _vec3 vPlayerLook, _float * fCrossy, _vec3 * vDir, _float * fDegree)
{
}

void CPlayerSkill::Enter_State(CBasePlayer * pPlayer)
{
	CPlayerState::Enter_State(pPlayer);
	m_pPlayer->Get_Renderer()->Set_RadialBlur(false);
	m_pPlayer->Set_BattleType(CPlayer::TYPE_BATTLE);
}

void CPlayerSkill::Update_State(const _double dTimeDelta)
{
	if (CPlayer::TS_OUTRAGE_START == m_pPlayer->Get_CurState())
	{
		if (m_bTS_ESkill_Check_Right)
		{
			m_dbTS_Time -= dTimeDelta;

			if (0.f >= m_dbTS_Time)
			{
				if (24 <= m_iTS_ESkill_Cnt)
					m_dbTS_Time = 0.08f;
				else
					m_dbTS_Time = 0.15f;

				m_bTS_ESkill_Check_Right = false;
			}
		}
		else
		{
			m_dbTS_ESkill_Time_Right -= dTimeDelta;

			if (0.f >= m_dbTS_ESkill_Time_Right)
			{
				m_bTS_ESkill_CreateEffect_Right = true;

				//
				if (12<= m_iTS_ESkill_Cnt && 14 > m_iTS_ESkill_Cnt) //13 15
					m_bTS_ESkill_CreateEffect_Right = false;
				//
				if (24 <= m_iTS_ESkill_Cnt)
					m_dbTS_ESkill_Time_Right = 0.083f;
				else
					m_dbTS_ESkill_Time_Right = 0.113f;

				m_bTS_ESkill_Check_Right = true;
				m_bTS_ESkill_Check_Left = true;
				++m_iTS_ESkill_Cnt;
			}
		}


		if (m_bTS_ESkill_Check_Left)
		{
			m_dbTS_ESkill_Time_Left -= dTimeDelta;

			if (0.f >= m_dbTS_ESkill_Time_Left)
			{
				m_bTS_ESkill_CreateEffect_Left = true;
				//
				if (12 <= m_iTS_ESkill_Cnt && 14> m_iTS_ESkill_Cnt)
					m_bTS_ESkill_CreateEffect_Left = false;
				//

				if (24 <= m_iTS_ESkill_Cnt)
					m_dbTS_ESkill_Time_Left = 0.078f;
				else
					m_dbTS_ESkill_Time_Left = 0.378f;

				m_bTS_ESkill_Check_Left = false;
				++m_iTS_ESkill_Cnt;
			}

		}
	}
	Update_PlayerPattern(dTimeDelta);

}

CPlayer::STATE_ID CPlayerSkill::LateUpadte_State(const _double dTimeDelta)
{
	if (0 >= m_pPlayer->Get_BaseInfo()->iHp)
		return CPlayer::ID_HEART;

	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vPos = m_pPlayer->GetPlayerPos();
	if (Engine::KeyDown(DIK_LSHIFT))
	{
		m_pPlayer->Set_DashReserve();
	}

	if (m_bIsFinish)
	{
		m_pPlayer->Set_State(CPlayer::COMMON_COMBATWAIT);
		m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_COMBATWAIT);
		m_pPlayer->Set_AnimationSpeed(1);
		WeaponDissolve();
		return CPlayer::ID_COMMON;
	}

	if (m_bLeapAtk)
	{
		m_dbLeapTime += dTimeDelta;

		_vec3 vNoYPos;
		_vec3 vPos = vNoYPos = m_pPlayer->GetPlayerPos();
		_vec3 vUp = *m_pTransform->Get_Info(Engine::INFO_UP);
		_vec3 vGoal = vTargetPos - vStartPos;
		vNoYPos.y = 0.f;
		_float fDist = D3DXVec3Length(&(vNoYPos - vTargetPos));

		//if (fDist <= fLength*0.5f)
		//	vUp *= -1.f;
		D3DXVec3Normalize(&vGoal, &vGoal);
		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vGoal * 750.f * (_float)(dTimeDelta)), m_dwNaviIndex, true)));

		vPos = m_pPlayer->GetPlayerPos();
		//m_pNaviCom->Find_Index(m_dwNaviIndex, &vPos);

		m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vUp *(fDist-fLength*0.5f) * (_float)(dTimeDelta)), m_dwNaviIndex, true)));

		if (m_dbLeapTime >= 0.25f && m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
		{
			_vec3 vPosY;
			m_pNaviCom->Find_PosY(&vPos, m_dwNaviIndex, vPosY);
			m_pTransform->Set_Pos(&vPosY);
			m_bLeapAtk = false;
			m_pPlayer->Set_State(CPlayer::TS_LEAP_ATTACK_LANDING);
			m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_LANDING);			
		}

		if (m_dbLeapTime >= 0.25f && m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
		{
			m_bLeapAtk = false;
		}

		if(25.f>=fDist)
			m_bLeapAtk = false;
		if (2.0 <= m_dbLeapTime)
			m_bLeapAtk = false;
	}

	if (CPlayer::OB_SKILL_CHAINWHIP_READY == m_pPlayer->Get_CurState())
	{
		m_dbMinTime += dTimeDelta;
		//EΩ∫≈≥
		Engine::CGameObject* pTerrain = nullptr;
		pTerrain = Engine::Get_GameObject(Engine::GAMEOBJECT, L"Terrain");
		m_pCalculatorCom->PickDecalPos(g_hWnd, pTerrain, m_pPlayer->Get_DecalPos());
		//m_pPlayer->RotationToDecal();
		if (m_bEffect)
		{
			m_pPlayer->Create_Decal(CDecal::DECAL_SKILLTARGET, 100);
			m_bEffect = false;
		}
		if (5.f <= m_dbMinTime)
		{
			CCameraMgr::GetInstance()->ReSet_ActionZoom();
			m_pPlayer->Set_State(CPlayer::COMMON_COMBATWAIT);
			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_COMBATWAIT);
			m_pPlayer->Set_AnimationSpeed(1);
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}


		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
		{
			m_pPlayer->Set_AnimationSpeed(0);
		}

		if (Engine::MouseDown(Engine::DIM_LB))
		{
			CCameraMgr::GetInstance()->ReSet_ActionZoom();
			m_pPlayer->Set_AnimationSpeed(1);
			m_pMeshCom->Set_AnimationSet(CPlayer::OB_SKILL_CHAINWHIP);
			m_pPlayer->Set_State(CPlayer::OB_SKILL_CHAINWHIP);
			m_pPlayer->Release_Decal();
			m_pPlayer->Create_Decal(CDecal::DECAL_ORB_E_BEAM, 5);

			CSoundMgr::Get_Instance()->SoundOn(75);

			return CPlayer::ID_SKILL;
		}
	}

	else if (CPlayer::OB_SKILL_CHAINWHIP == m_pPlayer->Get_CurState())
	{
		//EΩ∫≈≥
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_E);
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}
	}

	else if (CPlayer::OB_SKILL_SPINNINGCRUSHER == m_pPlayer->Get_CurState())
	{
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_F);
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}
	}


	else if (CPlayer::OB_SKILL_SPINNINGCRUSHER_START == m_pPlayer->Get_CurState())
	{
		//FΩ∫≈≥
		if (m_bEffect)
		{
			m_pPlayer->Set_DecalPos(&vPos);
			m_pPlayer->Create_Decal(CDecal::DECAL_ORB_F, 3.5,*m_pPlayer->Get_TransformCom()->Get_Angle());
			m_bEffect = false;
			CSoundMgr::Get_Instance()->SoundOn(73);
		}
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_pMeshCom->Set_AnimationSet(CPlayer::OB_SKILL_SPINNINGCRUSHER);
			m_pPlayer->Set_State(CPlayer::OB_SKILL_SPINNINGCRUSHER);
			return CPlayer::ID_SKILL;
		}
	}

	else if (CPlayer::OB_RAGESKILL_GIANTTWINKLE_READY == m_pPlayer->Get_CurState())
	{
		//±√
		Engine::CGameObject* pTerrain = nullptr;
		pTerrain = Engine::Get_GameObject(Engine::GAMEOBJECT, L"Terrain");
		m_pCalculatorCom->PickDecalPos(g_hWnd, pTerrain, m_pPlayer->Get_DecalPos());
		//m_pPlayer->RotationToDecal();
		if (m_bEffect)
		{
			m_pPlayer->Create_Decal(CDecal::DECAL_SKILLTARGET, 100);
			m_bEffect = false;
		}
		if (Engine::MouseDown(Engine::DIM_LB))
		{
			CCameraMgr::GetInstance()->ReSet_ActionZoom();
			CUIMgr::GetInstance()->Set_QSkillEnd();
			m_pPlayer->Release_Decal();
			m_pPlayer->Create_Decal(CDecal::DECAL_ORB_ULT, 5, *m_pPlayer->Get_TransformCom()->Get_Angle());

			m_pMeshCom->Set_AnimationSet(CPlayer::OB_RAGESKILL_GIANTTWINKLE);
			m_pPlayer->Set_State(CPlayer::OB_RAGESKILL_GIANTTWINKLE);
			
			_int iSound = rand() % 2 + 88;
			CSoundMgr::Get_Instance()->SoundOn(iSound);

			return CPlayer::ID_SKILL;
		}
		if(m_pMeshCom->Is_AnimationSetFinish(0.2))
			CCameraMgr::GetInstance()->ReSet_ActionZoom();
	}

	else if (CPlayer::OB_SKILL_GIANTCOFFIN == m_pPlayer->Get_CurState())
	{
		//RΩ∫≈≥
		if (m_pMeshCom->Is_AnimationSetFinish(0.2) && !m_bEffect)
		{
			if (Get_FindTarget())
			{
				m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE, &m_vTargetPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
				m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE, &m_vTargetPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
				m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE, &m_vTargetPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
			}
			else
			{
				_vec3 vUltPos = vPos + vLook * 200.f + _vec3(0.f, 100.f, 0.f);
				m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE, &vUltPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
				m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE2, &vUltPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
				m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE3, &vUltPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
			}
			m_bEffect = true;

		}
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_R);
		}
	}

	else if (CPlayer::OB_RAGESKILL_GIANTTWINKLE == m_pPlayer->Get_CurState())
	{
		m_pPlayer->Set_Incibility();
		if (m_bSwapCam)
		{
			CCameraMgr::GetInstance()->ReSet_ActionZoom();
			// rage cam
			CCameraMgr::GetInstance()->Set_RageCam(m_pPlayer->Get_MainWeapon());
			m_bSwapCam = false;
		}
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_pPlayer->ReSet_Incibility();

			CUIMgr::GetInstance()->Set_QSkillEnd();
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}
	}

	else if (CPlayer::LB_SKILL_BINDINGSHOT_READY == m_pPlayer->Get_CurState())
	{
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_BINDINGSHOT_WAIT);
			m_pPlayer->Set_State(CPlayer::LB_SKILL_BINDINGSHOT_WAIT);
			static_cast<CLongBow*>(m_pPlayer->Get_MainWeaponPointer())->Set_LBDissolve(true);
			return CPlayer::ID_SKILL;
		}
	}

	else if (CPlayer::LB_SKILL_BINDINGSHOT_WAIT == m_pPlayer->Get_CurState())
	{
		
		Engine::CGameObject* pTerrain = nullptr;
		pTerrain = Engine::Get_GameObject(Engine::GAMEOBJECT, L"Terrain");
		m_pCalculatorCom->PickDecalPos(g_hWnd, pTerrain, m_pPlayer->Get_DecalPos());
		if (m_bEffect)
		{
			m_pPlayer->Create_Decal(CDecal::DECAL_SKILLTARGET, 100);
			m_bEffect = false;
		}
		m_dbMinTime += dTimeDelta;

		if (Engine::MouseDown(Engine::DIM_LB))
		{
			CCameraMgr::GetInstance()->ReSet_ActionZoom();
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_E);
			m_pPlayer->Release_Decal();
			m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_BINDINGSHOT_END);
			m_pPlayer->Set_State(CPlayer::LB_SKILL_BINDINGSHOT_END);
			m_pPlayer->Create_Decal(CDecal::DECAL_BOW_FALL, 5);
			return CPlayer::ID_SKILL;
		}

		if (m_dbMinTime >= 5)
		{
			if (m_pMeshCom->Is_AnimationSetFinish(0.12))
			{
				CCameraMgr::GetInstance()->ReSet_ActionZoom();
				m_pPlayer->Release_Decal();
				m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_BINDINGSHOT_END);
				m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_BINDINGSHOT_WAIT);
				m_pPlayer->Set_State(CPlayer::LB_SKILL_BINDINGSHOT_WAIT);
				WeaponDissolve();
				static_cast<CLongBow*>(m_pPlayer->Get_MainWeaponPointer())->Reset_LBDissolve();
				return CPlayer::ID_COMMON;
			}
		}
		else
			return CPlayer::ID_SKILL;
	}

	else if (CPlayer::LB_SKILL_BINDINGSHOT_END == m_pPlayer->Get_CurState())
	{
		m_dbBindingTime += dTimeDelta;
		if (0.15 <= m_dbBindingTime)
		{
			_uint iSound = rand() % 6 + 57;
			CSoundMgr::Get_Instance()->SoundOn(iSound);
			m_dbBindingTime = 0.0;
		}

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			WeaponDissolve();
			static_cast<CLongBow*>(m_pPlayer->Get_MainWeaponPointer())->Reset_LBDissolve();
			return CPlayer::ID_COMMON;
		}

		return CPlayer::ID_SKILL;
	}

	else if (CPlayer::LB_SKILL_ARROWSTORM_READY == m_pPlayer->Get_CurState())
	{
		if (m_pMeshCom->Get_TrackTime()>=m_pMeshCom->Get_Period()*0.1)
		{
			m_dbPush = 0;
			m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_ARROWSTORM_SHOT);
			m_pPlayer->Set_State(CPlayer::LB_SKILL_ARROWSTORM_SHOT);
			return CPlayer::ID_SKILL;
		}
	}

	else if (CPlayer::LB_SKILL_ARROWSTORM_SHOT == m_pPlayer->Get_CurState())
	{
		m_dbPush += dTimeDelta;
		if (m_pMeshCom->Get_TrackTime() > m_pMeshCom->Get_Period()*0.16 && m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.6)
		{
			m_dArrowTerm += dTimeDelta;
			if (m_dArrowTerm > 0.15)
			{
				Engine::CGameObject* pGameObject = nullptr;
				pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
				Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_dArrowTerm = 0;

				_uint iSound = rand() % 6 + 57;
				CSoundMgr::Get_Instance()->SoundOn(iSound);
			}
			
			_vec3 vPos = m_pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vLook * 500.f * (_float)((1.5 - m_dbPush)*dTimeDelta)), m_dwNaviIndex)));
		}
		
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_R);
			WeaponDissolve();
			static_cast<CLongBow*>(m_pPlayer->Get_MainWeaponPointer())->Reset_LBDissolve();
			return CPlayer::ID_COMMON;
		}
		return CPlayer::ID_SKILL;
	}

	else if (CPlayer::LB_SKILL_SIEGEPOSITION_SHOT == m_pPlayer->Get_CurState())
	{
		m_dbPush += dTimeDelta*2.0;
		if (m_pMeshCom->Get_TrackTime() > m_pMeshCom->Get_Period()*0.16 && m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.6)
		{
			if (m_bEffect)
			{
				Engine::CGameObject* pGameObject = nullptr;
				pGameObject = CArrow::Create(Engine::Get_Device(), CArrow::BASIC_ARROW, &(vPos + _vec3(0.f, 50.f, 0.f)), &_vec3(0.f, 0.f, 0.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_ControlType());
				Engine::Add_GameObject(Engine::GAMEOBJECT, L"Arrow", pGameObject);
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BOW_RING, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bEffect = false;

				_uint iSound = rand() % 6 + 57;
				CSoundMgr::Get_Instance()->SoundOn(iSound);
			}
			
			_vec3 vPos = m_pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vLook * 500.f * (_float)((1.5 - m_dbPush)*dTimeDelta)), m_dwNaviIndex)));
		}

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			++m_iCount;
			if (3 <= m_iCount)
			{
				CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_F);
				CCameraMgr::GetInstance()->ReSet_ActionZoom();
				WeaponDissolve();
				static_cast<CLongBow*>(m_pPlayer->Get_MainWeaponPointer())->Reset_LBDissolve();
				return CPlayer::ID_COMMON;
			}	
			m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_SIEGEPOSITION_LOOP);
			m_pPlayer->Set_State(CPlayer::LB_SKILL_SIEGEPOSITION_LOOP);
			return CPlayer::ID_SKILL;
		}
	}

	else if (CPlayer::LB_SKILL_SIEGEPOSITION_LOOP == m_pPlayer->Get_CurState())
	{
		m_dbKillingTime += dTimeDelta;

		if (Engine::MouseDown(Engine::DIM_LB))
		{
			m_dbPush = 0.0;
			m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_SIEGEPOSITION_SHOT);
			m_pPlayer->Set_State(CPlayer::LB_SKILL_SIEGEPOSITION_SHOT);
			m_bEffect = true;
			return CPlayer::ID_SKILL;
		}
		
		if (5 <= m_dbKillingTime||3<=m_iCount)
		{
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_F);
			WeaponDissolve();

			CCameraMgr::GetInstance()->ReSet_ActionZoom();
			m_pMeshCom->Set_AnimationSet(CPlayer::COMMON_COMBATWAIT);
			m_pPlayer->Set_State(CPlayer::COMMON_COMBATWAIT);
			return CPlayer::ID_COMMON;
		}

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_SIEGEPOSITION_SHOT);
			m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_SIEGEPOSITION_LOOP);
			m_pPlayer->Set_State(CPlayer::LB_SKILL_SIEGEPOSITION_LOOP);
			return CPlayer::ID_SKILL;
		}
	}

	else if (CPlayer::LB_SKILL_SIEGEPOSITION_START == m_pPlayer->Get_CurState())
	{
		m_dDustTime += dTimeDelta;
		if (m_dDustTime > 0.4 && m_bEffect)
		{
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_WIND_DUST, m_pPlayer->Get_TransformCom()->Get_WorldMatrix(), m_pPlayer->Get_TransformCom()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_DUST, m_pPlayer->Get_TransformCom()->Get_WorldMatrix(), m_pPlayer->Get_TransformCom()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
			m_bEffect = false;
			CSoundMgr::Get_Instance()->SoundOn(64);
		}
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_SIEGEPOSITION_LOOP);
			m_pPlayer->Set_State(CPlayer::LB_SKILL_SIEGEPOSITION_LOOP);
			m_dDustTime = 0.;
			m_bEffect = true;
			static_cast<CLongBow*>(m_pPlayer->Get_MainWeaponPointer())->Set_LBDissolve(true);
			return CPlayer::ID_SKILL;
		}
	}	
	else if (CPlayer::LB_RAGESKILL_PHOENIXARROW == m_pPlayer->Get_CurState())
	{
		m_dDustTime += dTimeDelta;
		if (m_dDustTime < 0.1 && m_bEffect && m_dDustTime < 0.6)
		{
			static_cast<CLongBow*>(m_pPlayer->Get_MainWeaponPointer())->Set_LBDissolve(true);
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_PHOENIX_WIND1, m_pPlayer->Get_TransformCom()->Get_WorldMatrix(), m_pPlayer->Get_TransformCom()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
			m_bEffect = false;
		}
		if (m_dDustTime >= 0.6 && !m_bEffect && m_dDustTime < 1.1)
		{
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_PHOENIX_WIND2, m_pPlayer->Get_TransformCom()->Get_WorldMatrix(), m_pPlayer->Get_TransformCom()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
			m_bEffect = true;
		}
		if (m_dDustTime > 1.1 && m_bEffect)
		{
			Engine::CGameObject* pGameObject = nullptr;
			pGameObject = CPhoenix::Create(Engine::Get_Device(), &(vPos + vLook * 100.f), &vLook, m_pPlayer->Get_ControlType());
			Engine::Add_GameObject(Engine::GAMEOBJECT, L"Phoenix", pGameObject);
			m_bEffect = false;
			static_cast<CLongBow*>(m_pPlayer->Get_MainWeaponPointer())->Reset_LBDissolve();
		}

		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.15)
		{
			if (m_bLBRage)
			{
				_int iSound = rand() % 2 + 86;
				CSoundMgr::Get_Instance()->SoundOn(iSound);
				m_bLBRage = false;
			}
		}

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_pPlayer->ReSet_Incibility();
			return CPlayer::ID_COMMON;
		}
	}

	else if (CPlayer::DS_SKILL_FATEDCIRCLE_END == m_pPlayer->Get_CurState())
	{
		if (m_bDoubleEffect)
		{
			_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
			Engine::CGameObject* pGameObject = nullptr;
			 pGameObject = CFateShot::Create(Engine::Get_Device(), CFateShot::TYPE_END, &(vPos + _vec3(0.f, 25.f, 0.f) + vRight *100.f), &_vec3(0.f, 180.f, 45.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_BaseInfo()->eObjectID);
			if (pGameObject == nullptr)
				return CPlayer::ID_SKILL;
			Engine::Add_GameObject(Engine::GAMEOBJECT, L"FateR", pGameObject);

			//Engine::CGameObject* pGameObject = nullptr;
			pGameObject = CFateShot::Create(Engine::Get_Device(), CFateShot::TYPE_END, &(vPos + _vec3(0.f, 75.f, 0.f) + vRight *-100.f), &_vec3(0.f, 0.f, 30.f), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f), m_pPlayer->Get_BaseInfo()->eObjectID);
			if (pGameObject == nullptr)
				return CPlayer::ID_SKILL;
			Engine::Add_GameObject(Engine::GAMEOBJECT, L"FateL", pGameObject);
			m_bDoubleEffect = false;
		}

		if (m_pMeshCom->Get_TrackTime()>=m_pMeshCom->Get_Period()*0.5)
		{
			if (m_bEffect)
			{
				_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
				_vec3 vAngle = *m_pTransform->Get_Angle();
				Engine::CGameObject* pGameObject = nullptr;
				pGameObject = CFateShot::Create(Engine::Get_Device(), CFateShot::TYPE_MOVE, &(vPos + _vec3(0.f, 50.f, 0.f)+ vRight *50.f), &(_vec3(0.f, 0.f, 90.f) + vAngle), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f) + vRight *50.f, m_pPlayer->Get_BaseInfo()->eObjectID);
				if (pGameObject == nullptr)
					return CPlayer::ID_SKILL;
				Engine::Add_GameObject(Engine::GAMEOBJECT, L"FateR", pGameObject);

				//Engine::CGameObject* pGameObject = nullptr;
				pGameObject = CFateShot::Create(Engine::Get_Device(), CFateShot::TYPE_MOVE, &(vPos + _vec3(0.f, 50.f, 0.f) + vRight *50.f), &(_vec3(0.f, 0.f, 0.f)+vAngle), 100.f, vPos + vLook + _vec3(0.f, 50.f, 0.f) + vRight *50.f, m_pPlayer->Get_BaseInfo()->eObjectID);
				if (pGameObject == nullptr)
					return CPlayer::ID_SKILL;
				Engine::Add_GameObject(Engine::GAMEOBJECT, L"FateL", pGameObject);
				m_bEffect = false;
				CSoundMgr::Get_Instance()->SoundOn(45);
			}
		}

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_E);
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}
	}

	else if (CPlayer::DS_SKILL_CRITICALX == m_pPlayer->Get_CurState())
	{
		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.05)
		{
			if (m_bEffect)
			{
				m_bEffect = false;
				_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
				_vec3 test = *m_pTransform->Get_Angle();
				Engine::CGameObject* pGameObject = nullptr;
				pGameObject = CDsBullet::Create(Engine::Get_Device(), &(vPos + _vec3(0.f, 50.f, 0.f) + vRight*250.f), &test, vPos + _vec3(0.f, 50.f, 0.f) + vLook*500.f, Engine::EFFECTTYPE::EFFECT_BASICTRAIL8, m_pPlayer->Get_BaseInfo()->eObjectID);
				if (pGameObject == nullptr)
					return CPlayer::ID_SKILL;
				Engine::Add_GameObject(Engine::GAMEOBJECT, L"DSBullet", pGameObject);
				CSoundMgr::Get_Instance()->SoundOn(46);
			}
		}

		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.25)
		{
			if (m_bDoubleEffect)
			{
				m_bDoubleEffect = false;
				_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
				_vec3 test = *m_pTransform->Get_Angle();
				Engine::CGameObject* pGameObject = nullptr;

				pGameObject = CDsBullet::Create(Engine::Get_Device(), &(vPos + _vec3(0.f, 50.f, 0.f) + -vRight*250.f), &test, vPos + _vec3(0.f, 50.f, 0.f) + vLook*500.f, Engine::EFFECTTYPE::EFFECT_BASICTRAIL7, m_pPlayer->Get_BaseInfo()->eObjectID, false);
				if (pGameObject == nullptr)
					return CPlayer::ID_SKILL;
				Engine::Add_GameObject(Engine::GAMEOBJECT, L"DSBullet", pGameObject);
				CSoundMgr::Get_Instance()->SoundOn(44);
			}
		}
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_F);
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}
	}
	else if (CPlayer::DS_SKILL_SCART == m_pPlayer->Get_CurState())
	{
		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.25)
		{
			if (m_bEffect)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BLADE1, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bEffect = false;
				CSoundMgr::Get_Instance()->SoundOn(49);
			}
		}
		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.3)
		{
			if (m_bDoubleEffect)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_CROSS, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bDoubleEffect = false;
			}
		}
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_R);
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}
	}

	else if (CPlayer::DS_SKILL_FATEDCIRCLE_START == m_pPlayer->Get_CurState())
	{
		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.2)
		{
			if (m_bEffect)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_FATETRAIL1, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bEffect = false;
				CSoundMgr::Get_Instance()->SoundOn(43);
			}
		}

		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.25)
		{
			if (m_bDoubleEffect)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_FATETRAIL2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bDoubleEffect = false;
				CSoundMgr::Get_Instance()->SoundOn(44);
			}
		}

		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.35)
		{
			if (m_bDS_ESkill_Trail1)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_FATETRAIL3, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bDS_ESkill_Trail1 = false;
				CSoundMgr::Get_Instance()->SoundOn(45);
			}
		}

		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
		{
			if (m_bDS_ESkill_Trail2)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_FATETRAIL4, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bDS_ESkill_Trail2 = false;
				CSoundMgr::Get_Instance()->SoundOn(46);
			}
		}
		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.6)
		{
			if (m_bDS_ESkill_Trail3)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_FATETRAIL5, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bDS_ESkill_Trail3 = false;
				CSoundMgr::Get_Instance()->SoundOn(47);
			}
		}

		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.65)
		{
			if (m_bDS_ESkill_Trail4)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_FATETRAIL6, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bDS_ESkill_Trail4 = false;
				CSoundMgr::Get_Instance()->SoundOn(48);
			}
		}
		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.75)
		{
			if (m_bDS_ESkill_Trail5)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_FATETRAIL7, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bDS_ESkill_Trail5 = false;
				CSoundMgr::Get_Instance()->SoundOn(49);
			}
		}

		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.85)
		{
			if (m_bDS_ESkill_Trail6)
			{
				m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_FATETRAIL8, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				m_bDS_ESkill_Trail6 = false;
				CSoundMgr::Get_Instance()->SoundOn(43);
			}
		}
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_bEffect = true;
			m_bDoubleEffect = true;
			m_pMeshCom->Set_AnimationSet(CPlayer::DS_SKILL_FATEDCIRCLE_END);
			m_pPlayer->Set_State(CPlayer::DS_SKILL_FATEDCIRCLE_END);
			CSoundMgr::Get_Instance()->SoundOn(49);
			return CPlayer::ID_SKILL;
		}
	}

	else if (CPlayer::DS_RAGESKILL_GENOCIDE == m_pPlayer->Get_CurState())
	{
		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.6)
		{
			if (m_bEffect)
			{
				_vec3 vPos = *m_pTransform->Get_Info(Engine::INFO_POS);
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				_vec3 vRight = *m_pTransform->Get_Info(Engine::INFO_RIGHT);
				Engine::CGameObject* pGameObject = nullptr;
				pGameObject = CGenocide::Create(Engine::Get_Device(), &(vPos + _vec3(0.f, 25.f, 0.f)), &_vec3(0.f, 0.f, 0.f), vPos + vLook*1500.f, m_pPlayer->Get_BaseInfo()->eObjectID);
				if (pGameObject == nullptr)
					return CPlayer::ID_SKILL;
				Engine::Add_GameObject(Engine::GAMEOBJECT, L"Genocide", pGameObject);
				m_bEffect = false;
			}
		}

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_pPlayer->ReSet_Incibility();
			m_bEffect = true;
			m_bDoubleEffect = true;
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}
	}


	else if (m_pPlayer->Get_CurState() == CPlayer::TS_LEAP_ATTACK_LANDING)
	{

		m_pPlayer->Set_AnimationSpeed(1);
		/*_vec3 vPos = m_pPlayer->GetPlayerPos();
		m_pNaviCom->Find_Index(m_dwNaviIndex, &vPos);

		if (m_pNaviCom->Check_Landing(&vPos, m_dwNaviIndex))
		{
			m_pNaviCom->Find_PosY(&vPos, m_dwNaviIndex, vPos);
			m_pTransform->Set_Pos(&vPos);
		}*/

		////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ//////////	
		if (m_bEffect)
		{
			Engine::CTransform* pPlayerTransform = m_pPlayer->Get_PlayerTransform();

			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_R1, pPlayerTransform->Get_WorldMatrix(), pPlayerTransform->Get_Angle(), *m_pPlayer->Get_BaseInfo());
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_R2, pPlayerTransform->Get_WorldMatrix(), pPlayerTransform->Get_Angle(), *m_pPlayer->Get_BaseInfo());
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_R3, pPlayerTransform->Get_WorldMatrix(), pPlayerTransform->Get_Angle(), *m_pPlayer->Get_BaseInfo());
			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_R4, pPlayerTransform->Get_WorldMatrix(), pPlayerTransform->Get_Angle(), *m_pPlayer->Get_BaseInfo());

			m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LAY, pPlayerTransform->Get_WorldMatrix(), pPlayerTransform->Get_Angle(), *m_pPlayer->Get_BaseInfo());

			_vec3 vPos;
			_int iRandom;
			for (_uint i = 0; i < 10; ++i)
			{
				vPos = pPlayerTransform->m_vInfo[Engine::INFO_POS];
				iRandom = Engine::RandomNumber(2);
				if (0 == iRandom)
					iRandom = -1;

				vPos.x += Engine::RandomFloat(300.f * iRandom);
				vPos.y += 50.f;
				vPos.z += Engine::RandomFloat(300.f* iRandom);
				
				m_pEffectMgr->Create_TextureEffect(TEXTURE_TS_SMOKE0, &vPos);
			}
			CSoundMgr::Get_Instance()->SoundOn(55);
			m_bEffect = false;
		}
		///////////////////////////////////////

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_R);
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}

		return CPlayer::ID_SKILL;
	}

	else if (m_pPlayer->Get_CurState() == CPlayer::TS_LEAP_ATTACK_AIRDOWN)
	{
		//if (m_pMeshCom->Is_AnimationSetFinish(0.15))
		{
			_vec3 vPos = m_pPlayer->GetPlayerPos();
			vPos.y = 0.f;
			_float fDist = D3DXVec3Length(&(vPos - vTargetPos));

			if (25.f>=fDist || !m_bLeapAtk)
			{
				vPos = m_pPlayer->GetPlayerPos();
				m_pNaviCom->Find_PosY(&vPos, m_dwNaviIndex, vPos);
				m_pTransform->Set_Pos(&vPos);

				m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_LANDING);
				m_pPlayer->Set_State(CPlayer::TS_LEAP_ATTACK_LANDING);
				return CPlayer::ID_SKILL;
			}
			if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.75)
			{
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_START);
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_AIRDOWN);
				return CPlayer::ID_SKILL;
			}
		}
		return CPlayer::ID_SKILL;

	}

	else if (m_pPlayer->Get_CurState() == CPlayer::TS_LEAP_ATTACK_AIRUP)
	{
		//if (m_pMeshCom->Is_AnimationSetFinish(0.15))
		{
			_vec3 vPos = m_pPlayer->GetPlayerPos();
			vPos.y = 0.f;
			_float fDist = D3DXVec3Length(&(vPos - vTargetPos));

			if (fDist <= fLength*0.25f)
			{
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_AIRDOWN);
				m_pPlayer->Set_State(CPlayer::TS_LEAP_ATTACK_AIRDOWN);
				return CPlayer::ID_SKILL;
			}
			if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.75)
			{
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_START);
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_AIRUP);
				return CPlayer::ID_SKILL;
			}
		}
		return CPlayer::ID_SKILL;

	}

	else if (m_pPlayer->Get_CurState() == CPlayer::TS_LEAP_ATTACK_AIR)
	{
		m_pPlayer->Set_AnimationSpeed(0);
		//if (m_pMeshCom->Is_AnimationSetFinish(0.15))
		{
			_vec3 vPos = m_pPlayer->GetPlayerPos();
			vPos.y = 0.f;
			_float fDist = D3DXVec3Length(&(vPos - vTargetPos));

			if (fDist <= fLength*0.75f)
			{
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_AIRUP);
				m_pPlayer->Set_State(CPlayer::TS_LEAP_ATTACK_AIRUP);
				return CPlayer::ID_SKILL;
			}
			if(m_pMeshCom->Get_TrackTime()>=m_pMeshCom->Get_Period()*0.75)
			{
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_START);
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_AIR);
				return CPlayer::ID_SKILL;
			}
		}
		return CPlayer::ID_SKILL;

	}

	else if (m_pPlayer->Get_CurState() == CPlayer::TS_LEAP_ATTACK_START)
	{
		m_pPlayer->Set_AnimationSpeed(1);
		if (m_pMeshCom->Get_TrackTime()>=m_pMeshCom->Get_Period()*0.75)
		{
			m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_AIR);
			m_pPlayer->Set_State(CPlayer::TS_LEAP_ATTACK_AIR);
			CSoundMgr::Get_Instance()->SoundOn(5);
			return CPlayer::ID_SKILL;
		}
		return CPlayer::ID_SKILL;
	}

	else if (m_pPlayer->Get_CurState() == CPlayer::TS_ROCKSHOT)
	{
		CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));

		m_dbPush += dTimeDelta*1.5;
		{
			_vec3 vPos = m_pPlayer->GetPlayerPos();
			_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
			m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vLook * 500 * (_float)((2.5 - m_dbPush)*dTimeDelta)), m_dwNaviIndex)));

			////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ//////////	
			if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.2f)
			{
				if (m_bEffect)
				{
					//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_AIRUP, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					
					//π´±‚- ±√±ÿ±‚ off
					if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_AIRUP, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					//π´±‚- ±√±ÿ±‚ on
					else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
						m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_AIRUP_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

					m_bEffect = false;
				}

			}
			if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.4f)
			{				
				if (m_bDoubleEffect)
				{
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_F, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_F_ROCK, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

					_vec3 vPos;
					_int iRandom;
					for (_uint i = 0; i < 10; ++i)
					{
						vPos = m_pPlayer->Get_PlayerTransform()->m_vInfo[Engine::INFO_POS];
						vLook = m_pPlayer->Get_PlayerTransform()->m_vInfo[Engine::INFO_LOOK];
						D3DXVec3Normalize(&vLook, &vLook);
						vLook *= 40.f;

						iRandom = Engine::RandomNumber(2);
						if (0 == iRandom)
							iRandom = -1;

						vPos.x += Engine::RandomFloat(200.f * iRandom);
						vPos.y += 50.f;
						vPos.z += Engine::RandomFloat(200.f* iRandom);

						vPos += vLook;
						m_pEffectMgr->Create_TextureEffect(TEXTURE_TS_SMOKE0, &vPos);

						int itest = 0;
					}
					CSoundMgr::Get_Instance()->SoundOn(56);
					m_bDoubleEffect = false;
				}				
			}
			///////////////////////////////////////
		}

		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_F);
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}
	}
	else if (CPlayer::TS_OUTRAGE_START == m_pPlayer->Get_CurState())
	{
		CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));

		////»Ò¡§ √ﬂ∞°- ¥Î∞À ¿Ã∆Â∆Æ//////////	
		if (24 <= m_iTS_ESkill_Cnt)
		{
			if (m_bTS_ESkill_CreateEffect_Right)
			{
				//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_THIRD, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				//π´±‚- ±√±ÿ±‚ off
				if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_THIRD, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				//π´±‚- ±√±ÿ±‚ on
				else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_THIRD_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

				
				m_bTS_ESkill_CreateEffect_Right = false;
				CSoundMgr::Get_Instance()->SoundOn(52);
			}
			if (m_bTS_ESkill_CreateEffect_Left)
			{
				//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_FOURTH, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				//π´±‚- ±√±ÿ±‚ off
				if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_FOURTH, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				//π´±‚- ±√±ÿ±‚ on
				else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_FOURTH_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

				m_bTS_ESkill_CreateEffect_Left = false;
				//CSoundMgr::Get_Instance()->SoundOn(53);
			}
		}
		else
		{
			if (m_bTS_ESkill_CreateEffect_Right)
			{
				//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_FIRST, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				
				//π´±‚- ±√±ÿ±‚ off
				if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_FIRST, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				//π´±‚- ±√±ÿ±‚ on
				else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_FIRST_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());


				m_bTS_ESkill_CreateEffect_Right = false;
				CSoundMgr::Get_Instance()->SoundOn(53);
			}
			if (m_bTS_ESkill_CreateEffect_Left)
			{
				//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_SECOND, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				
				//π´±‚- ±√±ÿ±‚ off
				if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_SECOND, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				//π´±‚- ±√±ÿ±‚ on
				else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_E_SECOND_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

				
				m_bTS_ESkill_CreateEffect_Left = false;
				//CSoundMgr::Get_Instance()->SoundOn(53);
			}
		}

		if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.95f)
		{
			if (m_bEffect)
			{
				//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_THIRD2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				
				//π´±‚- ±√±ÿ±‚ off
				if (CTwohandSword::ULTIMATE_OFF == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_THIRD2, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());
				//π´±‚- ±√±ÿ±‚ on
				else if (CTwohandSword::ULTIMATE_ON == pWeapon->Get_UltimateState())
					m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_TS_LB_THIRD2_ULTIMATE, m_pPlayer->Get_PlayerTransform()->Get_WorldMatrix(), m_pPlayer->Get_PlayerTransform()->Get_Angle(), *m_pPlayer->Get_BaseInfo());

				m_bEffect = false;
				CSoundMgr::Get_Instance()->SoundOn(50);
			}
		}
		///////////////////////////////////////
	}

	else if (CPlayer::TS_RAGESKILL_ARMAGEDDONBLADE == m_pPlayer->Get_CurState())
	{
		if (m_pMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_pPlayer->ReSet_Incibility();
			return CPlayer::ID_COMMON;
		}
	}

	if (m_pMeshCom->Is_AnimationSetFinish(0.2))
	{
		if (CPlayer::TS_OUTRAGE_START == m_pPlayer->Get_CurState())
		{
			m_pPlayer->Set_AnimationSpeed(1);
			m_pMeshCom->Set_AnimationSet(CPlayer::TS_BASICCOMBO_01);
			m_pPlayer->Set_State(CPlayer::TS_BASICCOMBO_01);
			CUIMgr::GetInstance()->SkillCoolDownCheck(Engine::KEYGUIDE_E);
			return CPlayer::ID_SKILL;
		}

		else
		{
			WeaponDissolve();
			return CPlayer::ID_COMMON;
		}
	}
	
	return CPlayer::ID_SKILL;
}

void CPlayerSkill::Update_PlayerPattern(const _double dTimeDelta)
{
	_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vPos = m_pPlayer->GetPlayerPos();

	if (m_bPass)
		return;
	switch (m_eWeapon)
	{
	case CBasePlayer::COMMON:
		break;
	case CBasePlayer::DUALSWORD:
	{
		Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_R", Engine::MESH, Engine::ID_STATIC));
		pWeaponCom->Set_AnimationSet(0);

		pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_L", Engine::MESH, Engine::ID_STATIC));
		pWeaponCom->Set_AnimationSet(0);

		CDualSword* pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_L"));
		pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

		pWeapon = dynamic_cast<CDualSword*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_R"));
		pWeapon->Set_CombatState(CDualSword::COMBAT_STATE::STATE_COMBAT);

		switch (m_pPlayer->Get_ReserveAction())
		{
		case Engine::SM_E:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);

			m_pPlayer->Set_AnimationSpeed(2);
			if (!m_bReserveFinish)
			{
				m_pPlayer->Set_State(CPlayer::DS_SKILL_FATEDCIRCLE_START);
				m_pMeshCom->Set_AnimationSet(CPlayer::DS_SKILL_FATEDCIRCLE_START);
				m_eStartMotion = Engine::SM_END;
				m_bReserveFinish = true;
			}
			break;
		}
		case Engine::SM_Q:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);
			CUIMgr::GetInstance()->Set_QSkillEnd();
			CCameraMgr::GetInstance()->Set_RageCam(m_pPlayer->Get_MainWeapon());

			m_pPlayer->Set_Incibility();
			m_pPlayer->Set_State(CPlayer::DS_RAGESKILL_GENOCIDE);
			m_pMeshCom->Set_AnimationSet(CPlayer::DS_RAGESKILL_GENOCIDE);
			m_eStartMotion = Engine::SM_END;
			CUIMgr::GetInstance()->Set_QSkillEnd();
			m_bPass = true;

			_int iSound = rand() % 2 + 92;
			CSoundMgr::Get_Instance()->SoundOn(iSound);
			break;
		}

		case Engine::SM_F:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);

			m_pPlayer->Set_State(CPlayer::DS_SKILL_CRITICALX);
			m_pMeshCom->Set_AnimationSet(CPlayer::DS_SKILL_CRITICALX);
			m_eStartMotion = Engine::SM_END;
			break;
		}


		case Engine::SM_R:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);

			m_pPlayer->Set_State(CPlayer::DS_SKILL_SCART);
			m_pMeshCom->Set_AnimationSet(CPlayer::DS_SKILL_SCART);
			m_eStartMotion = Engine::SM_END;
			break;
		}

		}
	}
	break;
	case CBasePlayer::TWOHANDSWORD:
	{
		switch (m_pPlayer->Get_ReserveAction())
		{
		case Engine::SM_E:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);
			if (!m_bReserveFinish)
			{
				m_pPlayer->Set_AnimationSpeed(2);
				Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"TS", Engine::MESH, Engine::ID_STATIC));
				pWeaponCom->Set_AnimationSet(0);

				//»Ò¡§ √ﬂ∞° - 2¡Ÿ
				CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));
				pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);

				m_pPlayer->Set_State(CPlayer::TS_OUTRAGE_START);
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_OUTRAGE_START);
				m_eStartMotion = Engine::SM_END;

				Set_Effect();
				m_iTS_ESkill_Cnt = 0;
				m_bReserveFinish = true;
				CSoundMgr::Get_Instance()->SoundOn(5);
			}
			break;
		}
		case Engine::SM_Q:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);
			CUIMgr::GetInstance()->Set_QSkillEnd();
			CCameraMgr::GetInstance()->Set_RageCam(m_pPlayer->Get_MainWeapon());

			//»Ò¡§ √ﬂ∞° - 3¡Ÿ
			CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT,L"TS"));
			pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);
			pWeapon->Set_TSDecoRender(true);

			m_pPlayer->Set_Incibility();
			m_pPlayer->Set_State(CPlayer::TS_RAGESKILL_ARMAGEDDONBLADE);
			m_pMeshCom->Set_AnimationSet(CPlayer::TS_RAGESKILL_ARMAGEDDONBLADE);
			m_eStartMotion = Engine::SM_END;
			m_bPass = true;

			_int iSound = rand() % 2 + 90;
			CSoundMgr::Get_Instance()->SoundOn(iSound);

			break;
		}

		case Engine::SM_F:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);

			Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"TS", Engine::MESH, Engine::ID_STATIC));
			pWeaponCom->Set_AnimationSet(0);

			//»Ò¡§ √ﬂ∞° - 2¡Ÿ
			CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));
			pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);

			m_pPlayer->Set_State(CPlayer::TS_ROCKSHOT);
			m_pMeshCom->Set_AnimationSet(CPlayer::TS_ROCKSHOT);
			m_eStartMotion = Engine::SM_END;
			m_bPass = true;
			CSoundMgr::Get_Instance()->SoundOn(5);
			break;
		}


		case Engine::SM_R:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);

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

			//»Ò¡§ √ﬂ∞° - 2¡Ÿ
			CTwohandSword* pWeapon = dynamic_cast<CTwohandSword*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"TS"));
			pWeapon->Set_CombatState(CTwohandSword::STATE_COMBAT);

			m_pPlayer->Set_State(CPlayer::TS_LEAP_ATTACK_READY);
			m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_READY);
			m_eStartMotion = Engine::SM_END;

			m_dbMinTime += dTimeDelta;

			Engine::CGameObject* pTerrain = nullptr;
			pTerrain = Engine::Get_GameObject(Engine::GAMEOBJECT, L"Terrain");
			m_pCalculatorCom->PickDecalPos(g_hWnd, pTerrain, m_pPlayer->Get_DecalPos());
			if (m_bDoubleEffect)
			{
				m_pPlayer->Create_Decal(CDecal::DECAL_SKILLTARGET, 100);
				m_bDoubleEffect = false;
			}

			if (5.f <= m_dbMinTime)
			{
				m_pPlayer->Set_AnimationSpeed(1);
				m_bIsFinish = true;
				m_pPlayer->Release_Decal();
				CCameraMgr::GetInstance()->ReSet_ActionZoom();
				return;
			}

			if (Engine::MouseDown(Engine::DIM_LB))
			{
				_ulong dwIndex;

				vTargetPos = *m_pPlayer->Get_DecalPos();
				m_pNaviCom->Find_Index(dwIndex, &(vTargetPos + _vec3(0.f, 150.f, 0.f)));

				if (1000 < dwIndex)
					return;

				m_bPass = true;
				m_bLeapAtk = true;
				m_dbLeapTime = 0;
				// leap attack start
				m_pPlayer->Set_AnimationSpeed(1);
				m_pPlayer->Set_State(CPlayer::TS_LEAP_ATTACK_START);
				m_pMeshCom->Set_AnimationSet(CPlayer::TS_LEAP_ATTACK_START);

				CCameraMgr::GetInstance()->ReSet_ActionZoom(); 
				// ¿”Ω√ ≈∏∞Ÿ
				m_pPlayer->Release_Decal();
				
				//cout << "TS-R Skill Target Index: " << dwIndex << endl;
				vStartPos = m_pPlayer->GetPlayerPos();
				vTargetPos.y = vStartPos.y = 0.f;
				fLength = D3DXVec3Length(&(vTargetPos - vStartPos));
			}

			if (m_pMeshCom->Get_TrackTime() >= m_pMeshCom->Get_Period()*0.5)
			{
				m_pPlayer->Set_AnimationSpeed(0);
			}

			break;
		}

		}
	}
	break;
	case CBasePlayer::ORB:
	{
		switch (m_pPlayer->Get_ReserveAction())
		{
		case Engine::SM_E:
		{
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

			CUIMgr::GetInstance()->UseableSkillCheck_All(false);
			if (!m_bReserveFinish)
			{
				m_pPlayer->Set_State(CPlayer::OB_SKILL_CHAINWHIP_READY);
				m_pMeshCom->Set_AnimationSet(CPlayer::OB_SKILL_CHAINWHIP_READY);
				m_pPlayer->Set_ReserveAction(Engine::SM_END);
				m_bReserveFinish = true;
			}
			break;
		}
		case Engine::SM_Q:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);

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

			m_pPlayer->Set_State(CPlayer::OB_RAGESKILL_GIANTTWINKLE_READY);
			m_pMeshCom->Set_AnimationSet(CPlayer::OB_RAGESKILL_GIANTTWINKLE_READY);
			m_pPlayer->Set_ReserveAction(Engine::SM_END);



			break;
		}

		case Engine::SM_F:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);

			m_pPlayer->Set_State(CPlayer::OB_SKILL_SPINNINGCRUSHER_START);
			m_pMeshCom->Set_AnimationSet(CPlayer::OB_SKILL_SPINNINGCRUSHER_START);
			m_pPlayer->Set_ReserveAction(Engine::SM_END);
			CSoundMgr::Get_Instance()->SoundOn(5);
			m_bPass = true;
			break;
		}


		case Engine::SM_R:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);

			m_dbPush += dTimeDelta;
			m_pPlayer->Set_State(CPlayer::OB_SKILL_GIANTCOFFIN);
			m_pMeshCom->Set_AnimationSet(CPlayer::OB_SKILL_GIANTCOFFIN);
			//m_pPlayer->Set_ReserveAction(Engine::SM_END);

			if (m_bEffect)
			{
				_int iEnemyType = m_pPlayer->Check_Enemy();
				if (iEnemyType == 0)
				{
					_vec3 vUltPos = vPos + vLook* 40.f + _vec3(0.f, 50.f, 0.f);
					m_pEffectMgr->Create_TextureEffect(TEXTURE_ORBULT1, &vUltPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
					m_pEffectMgr->Create_TextureEffect(TEXTURE_ORBULT2, &vUltPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
					m_pEffectMgr->Create_TextureEffect(TEXTURE_ORBULT3, &vUltPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
				}
				else
				{
					RotationTarget_Range(iEnemyType);
					m_pEffectMgr->Create_TextureEffect(TEXTURE_ORBULT1, &m_vTargetPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
					m_pEffectMgr->Create_TextureEffect(TEXTURE_ORBULT2, &m_vTargetPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
					m_pEffectMgr->Create_TextureEffect(TEXTURE_ORBULT3, &m_vTargetPos, &_vec3(0.f, 0.f, 0.f), nullptr, m_pPlayer->Get_ControlType());
				}
				m_bEffect = false;
			}
			if (m_pMeshCom->Get_TrackTime() > m_pMeshCom->Get_Period()*0.09 && m_pMeshCom->Get_TrackTime() < m_pMeshCom->Get_Period()*0.25)
			{
				_vec3 vPos = m_pPlayer->GetPlayerPos();
				_vec3 vLook = *m_pTransform->Get_Info(Engine::INFO_LOOK);
				m_pTransform->Set_Pos(&(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vLook * 1750 * (_float)((1 - m_dbPush)*dTimeDelta)), m_dwNaviIndex)));
			}
			break;
		}

		break;
		}
		break;
	}
	case CBasePlayer::LONGBOW:
	{
		Engine::CDynamicMesh* pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"LB", Engine::MESH, Engine::ID_STATIC));
		pWeaponCom->Set_AnimationSet(1);
		CLongBow* pWeapon = dynamic_cast<CLongBow*>(Engine::Get_GameObject(Engine::GAMEOBJECT, L"LB"));
		pWeapon->Set_CombatState(CLongBow::STATE_COMBAT);
		switch (m_pPlayer->Get_ReserveAction())
		{
		case Engine::SM_E:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);
			if (!m_bReserveFinish)
			{
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

				m_pPlayer->Set_State(CPlayer::LB_SKILL_BINDINGSHOT_READY);
				m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_BINDINGSHOT_READY);
				CSoundMgr::Get_Instance()->SoundOn(5);
				m_pPlayer->Set_ReserveAction(Engine::SM_END);
				m_bReserveFinish = true;
			}
			break;
		}
		case Engine::SM_Q:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);
			CUIMgr::GetInstance()->Set_QSkillEnd();
			m_pPlayer->Set_State(CPlayer::LB_RAGESKILL_PHOENIXARROW);
			m_pMeshCom->Set_AnimationSet(CPlayer::LB_RAGESKILL_PHOENIXARROW);
			m_pPlayer->Set_ReserveAction(Engine::SM_END);
			m_pPlayer->Set_Incibility();
			CUIMgr::GetInstance()->Set_QSkillEnd();
			CCameraMgr::GetInstance()->Set_RageCam(m_pPlayer->Get_MainWeapon());
			m_bPass = true;
		
			break;
		}

		case Engine::SM_F:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);
			// Real Piercing
			//m_bIsR = true;
			//m_pPlayer->Set_State(CPlayer::LB_SKILL_PIERCINGARROW);
			//m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_PIERCINGARROW);
			//m_pPlayer->Set_ReserveAction(Engine::SM_END);

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

			m_pPlayer->Set_State(CPlayer::LB_SKILL_SIEGEPOSITION_START);
			m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_SIEGEPOSITION_START);
			m_pPlayer->Set_ReserveAction(Engine::SM_END);
			break;
		}

		case Engine::SM_R:
		{
			CUIMgr::GetInstance()->UseableSkillCheck_All(false);
			m_pPlayer->Set_State(CPlayer::LB_SKILL_ARROWSTORM_SHOT);
			m_pMeshCom->Set_AnimationSet(CPlayer::LB_SKILL_ARROWSTORM_SHOT);
			m_pPlayer->Set_ReserveAction(Engine::SM_END);
			CSoundMgr::Get_Instance()->SoundOn(5);
			static_cast<CLongBow*>(m_pPlayer->Get_MainWeaponPointer())->Set_LBDissolve(true);
			m_bPass = true;
			break;
		}
		default:
			break;
		}

	}

	}
}

