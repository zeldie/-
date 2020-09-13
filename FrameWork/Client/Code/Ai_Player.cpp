#include "stdafx.h"

#include "Ai_Player.h"

#include "Ai_CS_State.h"
#include "Ai_BasicCombo.h"
#include "Ai_BasicComboEx.h"
#include "Ai_AirCombo.h"
#include "Ai_PlayerSkill.h"
#include "Ai_HeartState.h"

#include "Ai_PlayerState.h"

#include "AI_Alliance_Observer.h"
#include "AI_Enemy1_Observer.h"
#include "AI_Enemy2_Observer.h"

#include "AIConditionBar.h"

CAi_Player::CAi_Player(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBasePlayer(pGraphicDev),
	m_eReserveAtk(Engine::SM_END),
	m_pWeaponCom(nullptr),
	m_bLateInit(false),
	m_bIsJump(false),
	m_bIsFall(false),
	m_bIsAttacked(false),
	m_fPlayerSpeed(250.f),
	m_dbAnimationSpeed(1),
	m_dbJumpTime(0),
	m_dbRecoveryTime(0),
	m_bTiredTime(0),
	m_dbDeadTime(0.0),
	m_fAccelerate(75.f),
	m_fMaxSpeed(400.f),
	m_iStamina(100),
	m_iMaxStamina(100),
	m_iMp(1000.f),
	m_iMaxMp(1000.f),
	m_iCurHp(0),
	m_iDifHp(0),
	m_bIsTired(false),
	m_eDeadStep(DEAD1)
{
	ZeroMemory(vStartingPoint, sizeof(_vec3));

}


CAi_Player::~CAi_Player()
{
}

_vec3 CAi_Player::GetPlayerPos()
{
	_vec3 vPos;
	memcpy(&vPos, m_pTransformCom->Get_Info(Engine::INFO_POS), sizeof(_vec3));
	return vPos;
}

HRESULT CAi_Player::Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, PLAYER_ID eID)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_Speed(500.f);

	LoadOuterCollider(L"../../Data/Collider/HFPlayerOuter.dat");
	LoadHitCollider(L"../../Data/Collider/HFPlayerHit.dat");

	m_ePlayerID = eID;

	// Test_Start
	m_pAiState = new CAi_CsState();
	m_pAiState->Enter_State(this);
	m_pDynamicMeshCom->Set_AnimationSet(COMMON_WAIT);
	// Test_End

	// test start
	m_bBreak = false;
	m_iBasicSuperArmor = 100;
	// test end

	// Player Info
	m_tBaseInfo.iAtt = 100;
	m_tBaseInfo.iMaxHp = 18000;
	m_tBaseInfo.iHp = 18000;
	m_tBaseInfo.iPrevHp = 18000;
	m_tBaseInfo.iSuperAmmor = m_iBasicSuperArmor;
	m_tBaseInfo.iMaxSuperAmmor = 1000;
	m_tBaseInfo.eEffectID = EFFECT_END;
	m_tBaseInfo.iCritical = 10;

	switch (m_eCtrlType)
	{
	case CTRL_AI_ALLIANCE:
	{
		m_tBaseInfo.eObjectID = OBJECT_ALLIANCE;
		if (FAILED(Engine::AddSubject(Engine::AI_ALLIANCE)))
			return E_FAIL;
		break;
	}
	case CTRL_AI_ENERMY_1:
	{
		m_tBaseInfo.eObjectID = OBJECT_ENEMY_1;
		if (FAILED(Engine::AddSubject(Engine::AI_ENEMY_1)))
			return E_FAIL;
		break;
	}
	case CTRL_AI_ENERMY_2:
	{
		m_tBaseInfo.eObjectID = OBJECT_ENEMY_2;
		if (FAILED(Engine::AddSubject(Engine::AI_ENEMY_2)))
			return E_FAIL;
		break;
	}
	}
	
	m_iCurHp = m_tBaseInfo.iHp;
	m_iDifHp = 0;

	m_iMaxMp = 1000.f;
	m_iMp = 1000.f;
	//
	
	m_iStamina = 1000;
	m_iMaxStamina = 1000;
	
	m_iPreHp = m_tBaseInfo.iPrevHp;
	m_iDifPreHp = 0;

	m_iAirComboLevel = 0;

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
	Set_WeaponType();
	Set_WeaponName();

	m_eCur_StateState = STATETYPE::STATE_COMMON;
	m_eNext_StateState = STATETYPE::STATE_COMMON;

	if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ALLIANCE)
		m_eFlagTeam = TEAM::TEAM_BLUE;
	else
		m_eFlagTeam = TEAM::TEAM_RED;


	return S_OK;
}

_int CAi_Player::Update_GameObject(const _double & dTimeDelta)
{
	// yh test code
	m_tBaseInfo.vObjectPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	// yh test end

	if (m_eCtrlType == CTRL_AI_ALLIANCE)
	{
		int i = 0;
	}
	else if (m_eCtrlType == CTRL_AI_ENERMY_1)
	{
		int i = 0;
	}
	else if (m_eCtrlType == CTRL_AI_ENERMY_2)
	{
		int i = 0;
	}

	if (!m_bLateInit) 
		LateInit();
	Link_NaviMesh();
	

	//// %%%%%%%%%%%%%%%%%%% Cartel 시작할때 컷씬 %%%%%%%%%%%%%%%%%%%%%%%%%%%
	if (m_eCtrlType == CTRL_AI_ALLIANCE)
	{
		if (m_eCurState == CBasePlayer::PLAYER_STATE::COMMON_START)
		{
			if (m_pDynamicMeshCom->Get_TrackTime() > m_pDynamicMeshCom->Get_Period() * 0.8)
				m_bCartelBlueStart = true;
		}
	}
	else if (m_eCtrlType == CTRL_AI_ENERMY_1)
	{
		if (m_eCurState == CBasePlayer::PLAYER_STATE::COMMON_START)
		{
			if (m_pDynamicMeshCom->Get_TrackTime() > m_pDynamicMeshCom->Get_Period() * 0.8)
				m_bCartelRedStart = true;
		}
	}
	//// %%%%%%%%%%%%%%%%%%% Cartel 시작할때 컷씬 %%%%%%%%%%%%%%%%%%%%%%%%%%%

	if (m_bIsDead)
		Dead_Player(dTimeDelta);

	if (!CUIMgr::GetInstance()->Get_UI_Working() && !m_bIsDead) //UI가 꺼져 있을때만 상태변화일어남 -희정 추가
	{
		if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ENERMY_1)
			int i = 0;

		Update_Status(dTimeDelta);
		if (CAiMgr::GetInstance()->Get_CartelStart() == true)
			m_pAiState->Update_State(dTimeDelta);
	}

	else if (CUIMgr::GetInstance()->Get_UI_Working()) 	//UI가 켜져있을 땐 일반 상태
	{
		m_eCurState = COMMON_COMBATWAIT;
		m_pDynamicMeshCom->Set_AnimationSet(COMMON_COMBATWAIT);
	}


	if (m_dbRage >= m_dbMaxRage)
	{
		if (rand() % 13 < 2)
		{
			_vec3 vRanPos;
			vRanPos.x = (rand() % 40) - 20.f;
			vRanPos.y = (rand() % 70) + 30.f;
			vRanPos.z = (rand() % 40) - 20.f;
			m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_READY_ULT, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + vRanPos));
		}
		if (rand() % 20 < 2)
		{
			_vec3 vRanPos;
			vRanPos.x = (rand() % 40) - 20.f;
			vRanPos.y = (rand() % 70) + 30.f;
			vRanPos.z = (rand() % 40) - 20.f;
			m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_READY_ULT2, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + vRanPos));
		}
	}


	for (auto iter = m_mapinvincibility.begin(); iter != m_mapinvincibility.end();)
	{
		iter->second -= dTimeDelta;
		if (iter->second <= 0)
		{
			m_mapinvincibility.erase(iter++);
		}
		else
		{
			iter++;
		}
	}


	// Collider 세팅
	if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ALLIANCE)
		CCollisionMgr::GetInstance()->Add_Collision(COLLLIST::COLL_PLAYER, this);
	else
		CCollisionMgr::GetInstance()->Add_Collision(COLLLIST::COLL_MONSTER, this);


	_vec3* pPos  = m_pTransformCom->Get_Info(Engine::INFO_POS);
	_vec3 vLook = *(m_pTransformCom->Get_Info(Engine::INFO_LOOK));
	//vLightAt = *pPos;



	CBasePlayer::Update_GameObject(dTimeDelta);

	ObserverPass();
	//if (m_eBattleType == TYPE_IDLE && Engine::KeyDown(DIK_TAB)&& !CUIMgr::GetInstance()->CheckSkillCoolDowning(Engine::KEYGUIDE_TAB))
	//{

	//	Change_Weapon();

	//}


	return Engine::NO_EVENT;
}

_int CAi_Player::LateUpdate_GameObject(const _double & dTimeDelta)
{
	// yh test code
	m_tBaseInfo.vObjectPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	// yh test end


	if (m_eCtrlType == CTRL_AI_ALLIANCE)
	{
		int i = 0;
	}
	else if (m_eCtrlType == CTRL_AI_ENERMY_1)
	{
		int i = 0;
	}
	else if (m_eCtrlType == CTRL_AI_ENERMY_2)
	{
		int i = 0;
	}

	//Check Decal
	if (m_pDecal != nullptr)
	{
		if (m_pDecal->Get_NeedToReleaseDecal())
		{
			m_pDecal->Set_Dead();
			m_pDecal = nullptr;
		}
	}


	CBasePlayer::LateUpdate_GameObject(dTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	

	if (CAiMgr::GetInstance()->Get_CartelStart() == true)
		m_eIdNext = m_pAiState->LateUpadte_State(dTimeDelta);

	if (m_bIsAttacked)
		m_eIdNext = CBasePlayer::ID_HEART;

	Change_State();
	Change_StateState();



	if (0 >= m_iStamina && !m_bIsTired)
	{
		m_iStamina = 0;
		m_bIsTired = true;
	}

	//_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	//if (0.f > vPos.y || 1000.f < vPos.y)
	//{
	//	m_pTransformCom->Set_Pos(&m_pNaviMeshCom->CheckY(vPos, m_dwNaviIndex));
	//	int i = 0;
	//	if (m_bIsJump)
	//	{
	//		m_bIsJump = false;//int j = 0;//*/
	//		m_bIsFall = false;
	//	}
	//}

	//if (vPos.x >= 5000.f || -5000.f >= vPos.x || vPos.z >= 5000.f || -5000.f >= vPos.z || isnan(vPos.x) || isnan(vPos.y) || isnan(vPos.z))
	//{
	//	m_pTransformCom->Set_Pos(&vStartingPoint);
	//	if (m_bIsJump)
	//	{
	//		m_bIsJump = false;//int j = 0;//*/
	//		m_bIsFall = false;
	//	}
	//}


	return Engine::NO_EVENT;
}

void CAi_Player::Render_Geometry(const _double & dTimeDelta)
{
	//Shader
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;

	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
			pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
			pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
			pEffect->SetTexture("g_EmmisiveTexture", nullptr);
			pEffect->BeginPass(0);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);

	}
	pEffect->End();
	Engine::Safe_Release(pEffect);


	for (_uint i = SPHERE_FIRST; i < SPHERE_END; ++i)
	{
		for (auto& iter : m_vecCollSphere[i])
			iter->Update_Component(dTimeDelta);
	}

	CDynamicMeshObject::Render_Geometry(dTimeDelta);
}

void CAi_Player::Render_PostEffect(const _double & dTimeDelta)
{
	//Shader
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->SetTexture("g_DiffuseTexture", nullptr);
			pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
			pEffect->BeginPass(1);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void CAi_Player::Render_Shadow(const _double & dTimeDelta)
{
	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta, m_dbAnimationSpeed);

	//Shader
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		m_pDynamicMeshCom->Render_Meshes_Begin(iter);
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			pEffect->BeginPass(6);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();

		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CAi_Player::LateInit()
{
	m_bLateInit = true;

	m_pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"RealPlayer", Engine::TRANSFORM, Engine::ID_DYNAMIC));

	Engine::CGameObject*		pGameObject = nullptr;

	switch (m_eMainWeapon)
	{
	case CBasePlayer::DUALSWORD:
		m_pDualSword_R = CDualSword::Create(m_pGraphicDev, Engine::HAND_RIGHT, m_wstrAiName, m_wstrName_MainWeapon);
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_MainWeapon, m_pDualSword_R);

		m_pDualSword_L = CDualSword::Create(m_pGraphicDev, Engine::HAND_LEFT, m_wstrAiName, m_wstrName_DualSword_L);
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_DualSword_L, m_pDualSword_L);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(m_pDualSword_R->Get_Component(Engine::COMPONENTTYPE::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(m_pDualSword_L->Get_Component(Engine::COMPONENTTYPE::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);
		break;
	case CBasePlayer::TWOHANDSWORD:
		pGameObject = m_pTwoHandSword = CTwohandSword::Create(m_pGraphicDev, m_wstrAiName, m_wstrName_MainWeapon);
		if (pGameObject == nullptr)
			return;
		if (FAILED(Engine::Add_GameObject(Engine::GAMEOBJECT, m_wstrName_MainWeapon, pGameObject)))
			return;
		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_MainWeapon, Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);
		break;
	case CBasePlayer::ORB:
		pGameObject = m_pOrb = COrb::Create(m_pGraphicDev, m_wstrAiName, m_wstrName_MainWeapon);
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_MainWeapon, pGameObject);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_MainWeapon, Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);
		break;
	case CBasePlayer::LONGBOW:
		pGameObject = m_pLongBow = CLongBow::Create(m_pGraphicDev, m_wstrAiName, m_wstrName_MainWeapon);
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_MainWeapon, pGameObject);

		m_pLongBow->Update_GameObject(0);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_MainWeapon, Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);

		break;
	}	


	////////////// 희정 추가 //////////////////// ->aimgr에서 컷씬끝나고 생성.
	//Engine::CGameObject*		pUIObject = nullptr;

	//pUIObject = CAIConditionBar::Create(m_pGraphicDev,m_eCtrlType, m_pTransformCom);
	//Engine::Add_GameObject(Engine::UI, L"AIAllCondition", pUIObject);

	/////////////////////////////////////////////
}

const _matrix CAi_Player::Get_ViewMatrix() const
{
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	return matView;
}

void CAi_Player::Set_WeaponType()
{
	if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ALLIANCE)
	{
		m_eMainWeapon = CAi_Player::PLAYER_WEAPONTYPE::LONGBOW;
		m_eSubWeapon = CAi_Player::PLAYER_WEAPONTYPE::DUALSWORD;
	}
	else if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ENERMY_1)
	{
		m_eMainWeapon = CAi_Player::PLAYER_WEAPONTYPE::TWOHANDSWORD;			// ORB , DUALSWORD
		m_eSubWeapon = CAi_Player::PLAYER_WEAPONTYPE::LONGBOW;
	}
	else if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ENERMY_2)
	{
		m_eMainWeapon = CAi_Player::PLAYER_WEAPONTYPE::ORB;
		m_eSubWeapon = CAi_Player::PLAYER_WEAPONTYPE::TWOHANDSWORD;
	}
}

void CAi_Player::Set_WeaponName()
{
	if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ALLIANCE)
	{
		// 메인
		if (m_eMainWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
		{
			m_wstrName_MainWeapon = L"Alliance_DualSword_R";
			m_wstrName_DualSword_L = L"Alliance_DualSword_L";
		}
		else if (m_eMainWeapon == PLAYER_WEAPONTYPE::TWOHANDSWORD)
			m_wstrName_MainWeapon = L"Alliance_TwohandSword";
		else if (m_eMainWeapon == PLAYER_WEAPONTYPE::LONGBOW)
			m_wstrName_MainWeapon = L"Alliance_LongBow";
		else if (m_eMainWeapon == PLAYER_WEAPONTYPE::ORB)
			m_wstrName_MainWeapon = L"Alliance_Orb";

		// 서브
		if (m_eSubWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
		{
			m_wstrName_SubWeapon = L"Alliance_DualSword_R";
			m_wstrName_DualSword_L = L"Alliance_DualSword_L";
		}
		else if (m_eSubWeapon == PLAYER_WEAPONTYPE::TWOHANDSWORD)
			m_wstrName_SubWeapon = L"Alliance_TwohandSword";
		else if (m_eSubWeapon == PLAYER_WEAPONTYPE::LONGBOW)
			m_wstrName_SubWeapon = L"Alliance_LongBow";
		else if (m_eSubWeapon == PLAYER_WEAPONTYPE::ORB)
			m_wstrName_SubWeapon = L"Alliance_Orb";
	}
	else if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ENERMY_1)
	{
		// 메인
		if (m_eMainWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
		{
			m_wstrName_MainWeapon = L"Enermy_1_DualSword_R";
			m_wstrName_DualSword_L = L"Enermy_1_DualSword_L";
		}
		else if (m_eMainWeapon == PLAYER_WEAPONTYPE::TWOHANDSWORD)
			m_wstrName_MainWeapon = L"Enermy_1_TwohandSword";
		else if (m_eMainWeapon == PLAYER_WEAPONTYPE::LONGBOW)
			m_wstrName_MainWeapon = L"Enermy_1_LongBow";
		else if (m_eMainWeapon == PLAYER_WEAPONTYPE::ORB)
			m_wstrName_MainWeapon = L"Enermy_1_Orb";

		// 서브
		if (m_eSubWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
		{
			m_wstrName_SubWeapon = L"Enermy_1_DualSword_R";
			m_wstrName_DualSword_L = L"Enermy_1_DualSword_L";
		}
		else if (m_eSubWeapon == PLAYER_WEAPONTYPE::TWOHANDSWORD)
			m_wstrName_SubWeapon = L"Enermy_1_TwohandSword";
		else if (m_eSubWeapon == PLAYER_WEAPONTYPE::LONGBOW)
			m_wstrName_SubWeapon = L"Enermy_1_LongBow";
		else if (m_eSubWeapon == PLAYER_WEAPONTYPE::ORB)
			m_wstrName_SubWeapon = L"Enermy_1_Orb";
	}
	else if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ENERMY_2)
	{
		// 메인
		if (m_eMainWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
		{
			m_wstrName_MainWeapon = L"Enermy_2_DualSword_R";
			m_wstrName_DualSword_L = L"Enermy_2_DualSword_L";
		}
		else if (m_eMainWeapon == PLAYER_WEAPONTYPE::TWOHANDSWORD)
			m_wstrName_MainWeapon = L"Enermy_2_TwohandSword";
		else if (m_eMainWeapon == PLAYER_WEAPONTYPE::LONGBOW)
			m_wstrName_MainWeapon = L"Enermy_2_LongBow";
		else if (m_eMainWeapon == PLAYER_WEAPONTYPE::ORB)
			m_wstrName_MainWeapon = L"Enermy_2_Orb";

		// 서브
		if (m_eSubWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
		{
			m_wstrName_SubWeapon = L"Enermy_2_DualSword_R";
			m_wstrName_DualSword_L = L"Enermy_2_DualSword_L";
		}
		else if (m_eSubWeapon == PLAYER_WEAPONTYPE::TWOHANDSWORD)
			m_wstrName_SubWeapon = L"Enermy_2_TwohandSword";
		else if (m_eSubWeapon == PLAYER_WEAPONTYPE::LONGBOW)
			m_wstrName_SubWeapon = L"Enermy_2_LongBow";
		else if (m_eSubWeapon == PLAYER_WEAPONTYPE::ORB)
			m_wstrName_SubWeapon = L"Enermy_2_Orb";
	}
}

void CAi_Player::Set_Collision(CBaseObject * pObj, COLLIDER eColliderType)
{
	if (pObj == nullptr)
		return;

	const BASE_INFO*	pBaseInfo = pObj->Get_BaseInfo();

	// 공격에 맞았다
	if (eColliderType == COLLIDER::COLLIDER_ATT_HIT)
	{
		if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ALLIANCE)
		{
			if (pBaseInfo->eObjectID == OBJID::OBJECT_ENEMY_1 || pBaseInfo->eObjectID == OBJID::OBJECT_ENEMY_2)
			{
				Set_Collision_Effect(pObj);
			}
		}
		else if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ENERMY_1 || m_eCtrlType == CONTROLTYPE::CTRL_AI_ENERMY_2)
		{
			if (pBaseInfo->eObjectID == OBJID::OBJECT_PLAYER || pBaseInfo->eObjectID == OBJID::OBJECT_ALLIANCE)
			{
				Set_Collision_Effect(pObj);
			}
		}
	}
	// 몸통끼리 부딪혔다
	else if (eColliderType == COLLIDER::COLLIDER_DEF_HIT)
	{

	}
}

void CAi_Player::Set_Collision_Effect(CBaseObject * pObj)
{
	if (m_bIsDead)
		return;

	if (m_mapinvincibility.end() == m_mapinvincibility.find(pObj->Get_BaseInfo()->eEffectID))
	{
		int inum = rand() % 3;
		if (inum == 0)
		{
			_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
			m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_HIT_1, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 70.f, 0.f) - vDir*15.f));
			m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_HIT_3, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 70.f, 0.f) - vDir*15.f));
		}
		else if (inum == 1)
		{
			_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
			m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_HIT_2, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 70.f, 0.f) - vDir*15.f));
		}
		else
		{
			_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
			m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_HIT_1, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 70.f, 0.f) - vDir*15.f));
			m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_HIT_4, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 85.f, 0.f) - vDir*15.f));
		}

		////cout << "Player Hit" << endl;
		m_mapinvincibility.emplace(pObj->Get_BaseInfo()->eEffectID, pObj->Get_BaseInfo()->fEffectHitTime);

		// 회복 시간 초기화
		m_dbRecoveryTime = 0;

		m_eCollisionID = pObj->Get_BaseInfo()->eCollisionID;

		if (pObj->Get_BaseInfo()->eObjectID == OBJID::OBJECT_PLAYER)
			m_pTargetPlayer = CAiMgr::GetInstance()->Get_Player();
		else if (pObj->Get_BaseInfo()->eObjectID == OBJID::OBJECT_ALLIANCE)
			m_pTargetPlayer = CAiMgr::GetInstance()->Get_Alliance();
		else if (pObj->Get_BaseInfo()->eObjectID == OBJID::OBJECT_ENEMY_1)
			m_pTargetPlayer = CAiMgr::GetInstance()->Get_Enemy_1();
		else if (pObj->Get_BaseInfo()->eObjectID == OBJID::OBJECT_ENEMY_2)
			m_pTargetPlayer = CAiMgr::GetInstance()->Get_Enemy_2();

		m_vTargetPos = pObj->Get_BaseInfo()->vObjectPos;
		_uint iCrit	= pObj->Get_BaseInfo()->iCritical;

		// 크리티컬 체크
		_int iTemp_Critical = Engine::RandomNumber(10);

		if (pObj->Get_BaseInfo()->iCritical >= iTemp_Critical)
			m_bCritical = true;
		else
			m_bCritical = false;

		// 임시 대미지 공식
		_int test = pObj->Get_BaseInfo()->iAtt >> 2;
		_int iDif = rand() % test + 1;

		_int iBasicDmg = (pObj->Get_BaseInfo()->iAtt + iDif);
		_int iHit = rand() % 12;

		if (m_bCritical)
		{
			// Critical Effect
			_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
			m_pEffectMgr->Create_Effect_Critical(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 125.f, 0.f) - vDir*15.f));

			m_iCurHp -= iBasicDmg * 1.5;
			m_iPreHp = (m_tBaseInfo.iHp - iBasicDmg*0.7*1.5);
			CUIMgr::GetInstance()->CreateDamageBox(m_pGraphicDev, m_pTransformCom, CDamageBox::DAMAGEBOXTYPE::CRITICAL, iBasicDmg *1.5);

			//희정 추가(깃발전에서 필요한 누적 데미지 계산을 위해서 추가.(플레이어가 맞는 입장))
			CUIMgr::GetInstance()->SetAccumulatedDamageForFlag(pObj->Get_BaseInfo()->eObjectID, iBasicDmg *1.5);

			if (pObj->Get_BaseInfo()->eObjectID == OBJECT_PLAYER)
			{
				// 카메라 쉐이킹
				CCameraMgr::GetInstance()->Set_ShakingCamera();
				iHit += 31;
				CSoundMgr::Get_Instance()->HoSoundOn(iHit);
			}
		}
		else
		{
			m_iCurHp -= iBasicDmg;
			m_iPreHp = (_int)(m_tBaseInfo.iHp - iBasicDmg*0.7);
			CUIMgr::GetInstance()->CreateDamageBox(m_pGraphicDev, m_pTransformCom, CDamageBox::DAMAGEBOXTYPE::BASIC, iBasicDmg);

			//희정 추가(깃발전에서 필요한 누적 데미지 계산을 위해서 추가.(플레이어가 맞는 입장))
			CUIMgr::GetInstance()->SetAccumulatedDamageForFlag(pObj->Get_BaseInfo()->eObjectID, iBasicDmg);

			if (pObj->Get_BaseInfo()->eObjectID == OBJECT_PLAYER)
			{
				iHit += 19;
				CSoundMgr::Get_Instance()->HoSoundOn(iHit);
			}
		}
	
		m_iDifHp = m_tBaseInfo.iHp - m_iCurHp;
		m_iDifPreHp = m_tBaseInfo.iPrevHp - m_iPreHp;

		if (0 >= m_iDifHp)
			m_iDifHp = 0;

		if (0 > m_iPreHp)
			m_iPreHp = 0;

		if (0 > m_iDifPreHp)
			m_iDifPreHp = 0;

		if (0 >= m_iCurHp && !m_bIsDead)
		{
			m_tBaseInfo.iHp = 0;
			m_tBaseInfo.iPrevHp = 0;

			if (CUIMgr::UITYPE_CARTEL == CUIMgr::GetInstance()->Get_UIType())
			{
				// 희정 추가(깃발전에서 누가누구죽였는지 뜨는 UI)
				CUIMgr::GetInstance()->CheckFlagScore(m_pGraphicDev, pObj->Get_BaseInfo()->eObjectID, m_tBaseInfo.eObjectID);

				if (OBJECT_PLAYER == pObj->Get_BaseInfo()->eObjectID || OBJECT_ALLIANCE == pObj->Get_BaseInfo()->eObjectID)
				{
					//kill voice
					CSoundMgr::Get_Instance()->HoSoundOn(rand() % 6 + 48, 1.f);
				}
				else if (OBJECT_ENEMY_1 == pObj->Get_BaseInfo()->eObjectID || OBJECT_ENEMY_2 == pObj->Get_BaseInfo()->eObjectID)
				{
					CSoundMgr::Get_Instance()->HoSoundOn(rand() % 2 + 54, 1.f);
				}
			}

			// Dead
			m_bIsDead = true;
			m_bisRunToFlag = false;
		}

		// 슈아 깎임
		_int iSuperArmorDamage = (_int)((pObj->Get_BaseInfo()->iAtt + iDif)*0.3); //희정 수정&추가
		m_tBaseInfo.iSuperAmmor -= iSuperArmorDamage;


		// 슈아 깨짐
		if (0 >= m_tBaseInfo.iSuperAmmor)
		{
			if (!m_bBreak)
			{
				// Break Effect
				_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
				m_pEffectMgr->Create_Effect_Break(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 125.f, 0.f) - vDir*15.f));
			}
			m_tBaseInfo.iSuperAmmor = 0;
			m_bBreak = true;
		}
		else// 희정 추가
		{
			//깃발전에서 필요한 누적 브레이크 계산을 위해서 추가.(플레이어가 맞는 입장)
			if (CUIMgr::UITYPE_CARTEL == CUIMgr::GetInstance()->Get_UIType())
			{
				CUIMgr::GetInstance()->SetAccumulatedBreakForFlag(pObj->Get_BaseInfo()->eObjectID, iSuperArmorDamage);
			}
		}

		m_pAiState->Is_Attacked();
		m_bIsAttacked = true;

		_int iHitSound = Engine::RandomNumber(3);
		_int iHitPer = Engine::RandomNumber(3);
		
		_vec3 vPlayer = *m_pPlayerTransform->Get_Info(Engine::INFO_POS);
		_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);

		_float fDistance = D3DXVec3Length(&(vPos - vPlayer));
		_float fVolume=0.f;
		//cout <<"dist :" << fDistance << endl;
		if (500.f <= fDistance)
			fVolume = 0.f;
		else
		{
			fVolume = fDistance*0.002f;
			fVolume *= 0.3f;
		}
		//cout << "fVolume :" << fVolume << endl;
		switch (m_eCtrlType)
		{
		case CTRL_AI_ALLIANCE:
			if (iHitPer == 0)
			{
				if (iHitSound == 0)
					CSoundMgr::Get_Instance()->AiSoundOn(15, fVolume);
				else if (iHitSound == 1)
					CSoundMgr::Get_Instance()->AiSoundOn(16, fVolume);
				else if (iHitSound == 2)
					CSoundMgr::Get_Instance()->AiSoundOn(17, fVolume);
			}
			break;
		case CTRL_AI_ENERMY_1:
			if (iHitPer == 0)
			{
				if (iHitSound == 0)
					CSoundMgr::Get_Instance()->AiSoundOn(32, fVolume);
				else if (iHitSound == 1)
					CSoundMgr::Get_Instance()->AiSoundOn(33, fVolume);
				else if (iHitSound == 2)
					CSoundMgr::Get_Instance()->AiSoundOn(34, fVolume);
			}
			break;
		case CTRL_AI_ENERMY_2:
			if (iHitPer == 0)
			{
				if (iHitSound == 0)
					CSoundMgr::Get_Instance()->AiSoundOn(53,fVolume);
				else if (iHitSound == 1)
					CSoundMgr::Get_Instance()->AiSoundOn(54,fVolume);
				else if (iHitSound == 2)
					CSoundMgr::Get_Instance()->AiSoundOn(55,fVolume);
			}
			break;
		}
	}
}

HRESULT CAi_Player::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	if (m_wstrAiName == L"Alliance")
	{
		pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_FinalPlayer"));
		if (pComponent == nullptr)
			return E_FAIL;
	}
	else if (m_wstrAiName == L"Enermy_1")
	{
		pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_HFPlayer"));
		if (pComponent == nullptr)
			return E_FAIL;
	}
	else if (m_wstrAiName == L"Enermy_2")
	{
		pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_Player_HM"));
		if (pComponent == nullptr)
			return E_FAIL;
	}
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent); 

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);
	return S_OK;
}

HRESULT CAi_Player::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	return S_OK;
}

void CAi_Player::Change_State()
{
	//m_pAiState->Update(this, dTimeDelta);

	////cout << m_eIdCurrent << endl;

	if (m_eIdCurrent != m_eIdNext)
	{
		if (m_eIdNext == CBasePlayer::ID_HEART)
		{
			if (m_pTargetPlayer == nullptr)
			{
				m_bIsAttacked = false;
				m_eIdNext = ID_COMMON;
				m_eCurState = CBasePlayer::PLAYER_STATE::COMMON_COMBATWAIT;
				return;
			}
		}

		if (m_pAiState)
			delete m_pAiState;

		m_eIdCurrent = m_eIdNext;

		switch (m_eIdNext)
		{
		case CBasePlayer::ID_COMMON:
		{
			m_pAiState = new CAi_CsState;
			m_pAiState->Enter_State(this);

			// Weapon Swap
			// Weapon의 1번 애니메이션은 Wait
			if (m_eMainWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
			{
				//m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_MainWeapon, Engine::MESH, Engine::ID_STATIC));
				//m_pWeaponCom->Set_AnimationSet(1);
				m_pDualSword_R->Set_CombatState(CDualSword::STATE_WAIT);

				//m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_DualSword_L, Engine::MESH, Engine::ID_STATIC));
				//m_pWeaponCom->Set_AnimationSet(1);
				m_pDualSword_L->Set_CombatState(CDualSword::STATE_WAIT);
			}
			else if (m_eMainWeapon == PLAYER_WEAPONTYPE::TWOHANDSWORD) //희정추가
			{
				m_pTwoHandSword->Set_CombatState(CTwohandSword::STATE_WAIT);
				//m_pWeaponCom->Set_AnimationSet(1);
			}
			else if (m_eMainWeapon == PLAYER_WEAPONTYPE::LONGBOW)
			{
				m_pLongBow->Set_CombatState(CLongBow::STATE_WAIT);
			}
			else if (m_eMainWeapon == PLAYER_WEAPONTYPE::ORB)
			{
				m_pWeaponCom->Set_AnimationSet(1);
			}
			return;
		}
		break;
		case CBasePlayer::ID_BASICCOMBO:
			m_pAiState = new CAi_BasicCombo;
			break;
		case CBasePlayer::ID_BASICCOMBOEX:
			m_pAiState = new CAi_BasicComboEx;
			break;
		case CBasePlayer::ID_AIRCOMBO:
			m_pAiState = new CAi_AirCombo;
			break;
		case CBasePlayer::ID_SKILL:
			m_pAiState = new CAi_PlayerSkill;
			break;
		case CBasePlayer::ID_HEART:
			m_pAiState = new CAi_HeartState;
			m_pAiState->Enter_State(this);

			// Weapon Swap
			// Weapon의 1번 애니메이션은 Wait
			if (m_eMainWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
			{
				m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_MainWeapon, Engine::MESH, Engine::ID_STATIC));
				m_pWeaponCom->Set_AnimationSet(1);

				m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_DualSword_L, Engine::MESH, Engine::ID_STATIC));
				m_pWeaponCom->Set_AnimationSet(1);
			}
			else if (m_eMainWeapon == PLAYER_WEAPONTYPE::TWOHANDSWORD) //희정추가
			{
				m_pTwoHandSword->Set_CombatState(CTwohandSword::STATE_WAIT);
				//m_pWeaponCom->Set_AnimationSet(1);
			}
			else if (m_eMainWeapon == PLAYER_WEAPONTYPE::LONGBOW)
			{
				m_pLongBow->Set_CombatState(CLongBow::STATE_WAIT);
				//m_pWeaponCom->Set_AnimationSet(1);
			}
			else	// 듀얼소드는 무기 두개를 같이 Set시켜야한다.
			{
				m_pWeaponCom->Set_AnimationSet(1);
			}
			return;
			break;
		case CBasePlayer::STATEID_END:
			m_pAiState = new CAi_BasicCombo;
			break;
		default:
			m_pAiState = new CAi_BasicCombo;
			break;
		}

		m_pAiState->Enter_State(this);
		
		// Weapon Swap
		// Weapon의 1번 애니메이션은 Wait
		if (m_eMainWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
		{
			m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_MainWeapon, Engine::MESH, Engine::ID_STATIC));
			m_pWeaponCom->Set_AnimationSet(0);

			m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_MainWeapon, Engine::MESH, Engine::ID_STATIC));
			m_pWeaponCom->Set_AnimationSet(0);
		}
		else	// 듀얼소드는 무기 두개를 같이 Set시켜야한다.
		{
			m_pWeaponCom->Set_AnimationSet(0);
		}

	}
}

void CAi_Player::Update_Status(const _double dTimeDelta)
{
	if (0 < m_iCurHp)
	{
		if (m_tBaseInfo.iHp > m_iCurHp)
		{
			m_tBaseInfo.iHp -= (_int)(m_iDifHp*dTimeDelta*2.0);
		}

		if (m_tBaseInfo.iPrevHp > m_iPreHp)
		{
			m_tBaseInfo.iPrevHp -= (_int)(m_iDifPreHp*dTimeDelta*0.75*2.0);
		}
	}
	if (m_eBattleType == TYPE_IDLE/* && m_eCurState == COMMON_WAIT*/)
	{
		m_dbRecoveryTime += dTimeDelta;

		if (2 <= m_dbRecoveryTime)			
		{
			if (m_tBaseInfo.iHp < m_tBaseInfo.iPrevHp)
			{
				m_tBaseInfo.iHp = m_iCurHp = m_tBaseInfo.iPrevHp;
				m_dbRecoveryTime = 0;
			}

			m_bBreak = false;
			if (m_iBasicSuperArmor > m_tBaseInfo.iSuperAmmor)
				m_tBaseInfo.iSuperAmmor += (_int)(m_iBasicSuperArmor * dTimeDelta);
			if (m_iBasicSuperArmor < m_tBaseInfo.iSuperAmmor)
				m_tBaseInfo.iSuperAmmor = m_iBasicSuperArmor;
		}
		
	}
	else
		m_dbRecoveryTime = 0;

	if (m_iStamina < m_iMaxStamina)
	{
		if (m_eCurState != COMMON_DASH)
		{
			m_iStamina += (_int)(200 * dTimeDelta);
		}
	}


	if (m_iMaxMp > m_iMp)
	{
		m_iMp += (20.f * (_float)dTimeDelta);
	}
	if (m_iMaxMp < m_iMp)
		m_iMp = m_iMaxMp;

	if (m_bIsTired)
	{
		m_bTiredTime += dTimeDelta;

		if (3 <= m_bTiredTime)
		{
			m_bIsTired = false;
			m_bTiredTime = 0;

		}
	}

	// Test yh
}

void CAi_Player::Change_Weapon()
{
	CDynamicMeshObject* pWeapon;

	// 사용중인 무기 삭제

	switch (m_eMainWeapon)
	{
	case CBasePlayer::COMMON:
		break;
	case CBasePlayer::DUALSWORD:
	{
		m_pDualSword_R->Set_Dead(true);
		m_pDualSword_L->Set_Dead(true);

		m_pDualSword_R = nullptr;
		m_pDualSword_L = nullptr;

		Create_SubWeapon();
	}
		break;
	case CBasePlayer::TWOHANDSWORD:
	{
		pWeapon = dynamic_cast<CDynamicMeshObject*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_MainWeapon));
		pWeapon->Set_Dead(true);
		m_pTwoHandSword = nullptr;
		Create_SubWeapon();
	}
		break;
	case CBasePlayer::ORB:
	{
		pWeapon = dynamic_cast<CDynamicMeshObject*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_MainWeapon));
		pWeapon->Set_Dead(true);
		m_pOrb = nullptr;
		Create_SubWeapon();
	}
		break;
	case CBasePlayer::LONGBOW:
	{
		pWeapon = dynamic_cast<CDynamicMeshObject*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_MainWeapon));
		pWeapon->Set_Dead(true);
		m_pLongBow = nullptr;
		Create_SubWeapon();
	}
		break;
	default:
		break;
	}
}

void CAi_Player::Create_SubWeapon()
{
	CDynamicMeshObject* pWeapon;
	switch (m_eSubWeapon)
	{
	case CBasePlayer::COMMON:
		break;
	case CBasePlayer::DUALSWORD:
	{
		swap(m_eMainWeapon, m_eSubWeapon);
		swap(m_wstrName_MainWeapon, m_wstrName_SubWeapon);

		m_pDualSword_R = CDualSword::Create(m_pGraphicDev, Engine::HAND_RIGHT, m_wstrAiName, m_wstrName_MainWeapon);
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_MainWeapon, m_pDualSword_R);

		m_pDualSword_L = CDualSword::Create(m_pGraphicDev, Engine::HAND_LEFT, m_wstrAiName, m_wstrName_DualSword_L);
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_DualSword_L, m_pDualSword_L);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(m_pDualSword_R->Get_Component(Engine::COMPONENTTYPE::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(m_pDualSword_L->Get_Component(Engine::COMPONENTTYPE::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);
	}
		break;
	case CBasePlayer::TWOHANDSWORD:
	{
		swap(m_eMainWeapon, m_eSubWeapon);
		swap(m_wstrName_MainWeapon, m_wstrName_SubWeapon);

		pWeapon = m_pTwoHandSword = CTwohandSword::Create(m_pGraphicDev, m_wstrAiName, m_wstrName_MainWeapon);
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_MainWeapon, pWeapon);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_MainWeapon, Engine::MESH, Engine::ID_STATIC));
		
		if(m_eBattleType == TYPE_BATTLE)
		dynamic_cast<CTwohandSword*>(m_pTwoHandSword)->Change_WeaponState(CTwohandSword::STATE_WAIT);
		m_pWeaponCom->Set_AnimationSet(1);
	}
		break;
	case CBasePlayer::ORB:
	{
		swap(m_eMainWeapon, m_eSubWeapon);
		swap(m_wstrName_MainWeapon, m_wstrName_SubWeapon);

		pWeapon = m_pOrb = COrb::Create(m_pGraphicDev, m_wstrAiName, m_wstrName_MainWeapon);
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_MainWeapon, pWeapon);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_MainWeapon, Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);
	}
		break;
	case CBasePlayer::LONGBOW:
	{
		swap(m_eMainWeapon, m_eSubWeapon);
		swap(m_wstrName_MainWeapon, m_wstrName_SubWeapon);

		pWeapon = m_pLongBow = CLongBow::Create(m_pGraphicDev, m_wstrAiName, m_wstrName_MainWeapon);
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, m_wstrName_MainWeapon, pWeapon);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, m_wstrName_MainWeapon, Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);
	}
		break;
	default:
		break;
	}
}

void CAi_Player::Dead_Player(const _double & dTimeDelta)
{
	switch (m_eDeadStep)
	{
	case CAi_Player::DEAD1:
	{
		CCameraMgr::GetInstance()->ReSet_ActionZoom();
		m_eCurState = COMMON_DYING;
		m_pDynamicMeshCom->Set_AnimationSet(COMMON_DYING);

		if (m_pDynamicMeshCom->Is_AnimationSetFinish(0.2))
			m_eDeadStep = DEAD2;
	}
	break;
	case CAi_Player::DEAD2:
	{
		m_dbDeadTime += dTimeDelta;
		m_eCurState = COMMON_DOWN_B_B;
		m_pDynamicMeshCom->Set_AnimationSet(COMMON_DOWN_B_B);
		if (5 <= m_dbDeadTime)
		{
			m_dbDeadTime = 0.0;
			m_eDeadStep = DEAD3;
		}
	}
	break;
	case CAi_Player::DEAD3:
	{
		m_eCurState = COMMON_RESPAWN;
		m_pDynamicMeshCom->Set_AnimationSet(COMMON_RESPAWN);
		if (m_pDynamicMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_tBaseInfo.iHp = m_tBaseInfo.iPrevHp = 18000;
			Reset_SuperAmmor();
			m_iCurHp = m_iPreHp = m_iDifPreHp = m_iDifHp = m_tBaseInfo.iHp;
			m_bIsDead = false;
			m_eDeadStep = DEAD1;
			m_bIsAttacked = false;
			m_pAiState->Set_AiState_isAttacked(false);
			m_eIdNext = ID_COMMON;
		}
	}
	break;
	case CPlayer::DEAD_END:
		break;
	default:
		break;
	}
}

void CAi_Player::ObserverPass()
{
	if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ALLIANCE)
	{
		Engine::Notify(Engine::AI_ALLIANCE, CAI_Alliance_Observer::HP, (void*)(&m_tBaseInfo.iHp));
		Engine::Notify(Engine::AI_ALLIANCE, CAI_Alliance_Observer::PREVHP, (void*)(&m_tBaseInfo.iPrevHp));
		Engine::Notify(Engine::AI_ALLIANCE, CAI_Alliance_Observer::MAXHP, (void*)(&m_tBaseInfo.iMaxHp));
		Engine::Notify(Engine::AI_ALLIANCE, CAI_Alliance_Observer::SUPERARMOR, (void*)(&m_tBaseInfo.iSuperAmmor));
		Engine::Notify(Engine::AI_ALLIANCE, CAI_Alliance_Observer::MAXSUPERARMOR, (void*)(&m_tBaseInfo.iMaxSuperAmmor));
		//들고 있는 무기
		Engine::Notify(Engine::AI_ALLIANCE, CAI_Alliance_Observer::MAINWEAPON, (void*)(&m_eMainWeapon));
		Engine::Notify(Engine::AI_ALLIANCE, CAI_Alliance_Observer::SUBWEAPON, (void*)(&m_eSubWeapon));
	}
	else if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ENERMY_1)
	{
		Engine::Notify(Engine::AI_ENEMY_1, CAI_Enemy1_Observer::HP, (void*)(&m_tBaseInfo.iHp));
		Engine::Notify(Engine::AI_ENEMY_1, CAI_Enemy1_Observer::PREVHP, (void*)(&m_tBaseInfo.iPrevHp));
		Engine::Notify(Engine::AI_ENEMY_1, CAI_Enemy1_Observer::MAXHP, (void*)(&m_tBaseInfo.iMaxHp));
		Engine::Notify(Engine::AI_ENEMY_1, CAI_Enemy1_Observer::SUPERARMOR, (void*)(&m_tBaseInfo.iSuperAmmor));
		Engine::Notify(Engine::AI_ENEMY_1, CAI_Enemy1_Observer::MAXSUPERARMOR, (void*)(&m_tBaseInfo.iMaxSuperAmmor));
		//들고 있는 무기
		Engine::Notify(Engine::AI_ENEMY_1, CAI_Enemy1_Observer::MAINWEAPON, (void*)(&m_eMainWeapon));
		Engine::Notify(Engine::AI_ENEMY_1, CAI_Enemy1_Observer::SUBWEAPON, (void*)(&m_eSubWeapon));
	}
	else if (m_eCtrlType == CONTROLTYPE::CTRL_AI_ENERMY_2)
	{
		Engine::Notify(Engine::AI_ENEMY_2, CAI_Enemy2_Observer::HP, (void*)(&m_tBaseInfo.iHp));
		Engine::Notify(Engine::AI_ENEMY_2, CAI_Enemy2_Observer::PREVHP, (void*)(&m_tBaseInfo.iPrevHp));
		Engine::Notify(Engine::AI_ENEMY_2, CAI_Enemy2_Observer::MAXHP, (void*)(&m_tBaseInfo.iMaxHp));
		Engine::Notify(Engine::AI_ENEMY_2, CAI_Enemy2_Observer::SUPERARMOR, (void*)(&m_tBaseInfo.iSuperAmmor));
		Engine::Notify(Engine::AI_ENEMY_2, CAI_Enemy2_Observer::MAXSUPERARMOR, (void*)(&m_tBaseInfo.iMaxSuperAmmor));
		//들고 있는 무기
		Engine::Notify(Engine::AI_ENEMY_2, CAI_Enemy2_Observer::MAINWEAPON, (void*)(&m_eMainWeapon));
		Engine::Notify(Engine::AI_ENEMY_2, CAI_Enemy2_Observer::SUBWEAPON, (void*)(&m_eSubWeapon));
	}

	CUIMgr::GetInstance()->SetWeapon(m_tBaseInfo.eObjectID,m_eMainWeapon,m_eSubWeapon);
}

void CAi_Player::ExChange_State(CAi_PlayerState* pState, _bool bAtk)
{
	int i = 0;
}

void CAi_Player::Change_StateState()
{
	if (m_eCur_StateState != m_eNext_StateState)
	{
		m_eCur_StateState = m_eNext_StateState;
	}
}

void CAi_Player::Check_State()
{
	switch (m_eCtrlType)
	{
	case CONTROLTYPE::CTRL_AI_ALLIANCE:
		if (m_pTargetPlayer == nullptr)
		{
			//cout << "[Alliance]는 타겟이 없습니다." << endl;
			break;
		}
		else
		{
			switch (m_pTargetPlayer->Get_ControlType())
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
		if (m_pTargetPlayer == nullptr)
		{
			//cout << "[Enermy_1]은 타겟이 없습니다." << endl;
			break;
		}
		else
		{
			switch (m_pTargetPlayer->Get_ControlType())
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
		if (m_pTargetPlayer == nullptr)
		{
			//cout << "[Enermy_2]는 타겟이 없습니다." << endl;
			break;
		}
		else
		{
			switch (m_pTargetPlayer->Get_ControlType())
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

CAi_Player * CAi_Player::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, PLAYER_ID eID, CONTROLTYPE eCtrlType, wstring wstrAiName)
{
	CAi_Player* pInstance = new CAi_Player(pGraphicDev);

	pInstance->Set_AiName(wstrAiName);
	pInstance->Set_CtrlType(eCtrlType);

	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale, eID)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CAi_Player::Free()
{
	if (m_pAiState != nullptr)
		delete m_pAiState;

	m_mapinvincibility.clear();

	CBasePlayer::Free();
}
