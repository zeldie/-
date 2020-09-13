#include "stdafx.h"

#include "Player.h"

#include "CS_State.h"
#include "BasicCombo.h"
#include "BasicComboEx.h"
#include "AirCombo.h"
#include "PlayerSkill.h"
#include "HeartState.h"

#include "PlayerObserver.h"
#include "PlayerState.h"

#include "Skill.h"
///// UI test ->희정 ////////////
//#include "FlagScorePopUp.h"
/////////////////////////////////
#include "DSBullet.h"
#include "KarmaOverHead.h"
#include "OrbCore.h"
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBasePlayer(pGraphicDev),
	m_eCollisionID(COLLISION_END),
	m_eReserveAtk(Engine::SM_END),
	m_pWeaponCom(nullptr),
	m_bLateInit(false),
	m_bIsJump(false),
	m_bIsFall(false),
	m_bIsAttacked(false),
	m_bReserveDash(false),
	m_bincibility(false),
	m_bTargetLock(false),
	m_bPlayerDeadFinish(false),
	m_bRender(true),
	m_bCutscene(false),
	m_bIncTime(false),
	m_bDeadSound(false),
	m_bEnterSound(false),
	m_fPlayerSpeed(250.f),
	m_dbAnimationSpeed(1),
	m_dbJumpTime(0),
	m_dbRecoveryTime(0),
	m_bTiredTime(0),
	m_dbDeadTime(0.0),
	m_dbFallTime(0.0),
	m_dbEnterTime(0.0),
	m_dbTiredTime(0.0),
	m_fAccelerate(75.f),
	m_fMaxSpeed(550.f),
	m_iStamina(100),
	m_iMaxStamina(100),
	m_iMp(1000.f),
	m_iMaxMp(1000.f),
	m_iCurHp(0),
	m_iDifHp(0),
	m_pUIMgr(CUIMgr::GetInstance()),
	m_bMissionMapCheck(true),
	m_bTransApostle(false),
	m_bIsTired(false),
	m_bCritical(false),
	m_bBreak(false),
	m_eDeadStep(DEAD1),
	m_bCreateRevivalUI(true),
	m_bCheat(false),
	m_dbHeartbeatSoundTime(0.0),
	m_dbHBSoundCycle(0.0),
	m_pTargetTransform(nullptr)
{
	ZeroMemory(m_vTargetPos, sizeof(_vec3));
	ZeroMemory(m_vFocus, sizeof(_vec3));
	ZeroMemory(vStartingPoint, sizeof(_vec3));
}


CPlayer::~CPlayer()
{
}

_vec3 CPlayer::GetPlayerPos()
{
	_vec3 vPos;
	memcpy(&vPos, m_pTransformCom->Get_Info(Engine::INFO_POS), sizeof(_vec3));
	return vPos;
}

HRESULT CPlayer::Ready_GameObject(_vec3* pPos, _vec3* pAngle, _vec3* pScale, PLAYER_ID eID)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_Speed(500.f);
	m_pTransformCom->Update_Component(0.1f);

	LoadOuterCollider(L"../../Data/Collider/HFPlayerOuter.dat");
	LoadHitCollider(L"../../Data/Collider/HFPlayerHit.dat");

	//m_eMainWeapon = PLAYER_WEAPONTYPE::TWOHANDSWORD;
	//m_eSubWeapon = PLAYER_WEAPONTYPE::LONGBOW;
	m_eMainWeapon = PLAYER_WEAPONTYPE(m_pUIMgr->Get_PlayerMainWeaponType());
	m_eSubWeapon = PLAYER_WEAPONTYPE(m_pUIMgr->Get_PlayerSubWeaponType());

	m_ePlayerID = ID_PLAYER;
	vStartingPoint = *pPos;
	// Test_Start
	m_pDynamicMeshCom->Set_AnimationSet(COMMON_WAIT);
	// Test_End

	// test start
	m_bBreak = false;
	m_iBasicSuperArmor = 600;
	// test end

	// Player Info
	m_tBaseInfo.iAtt = 100;
	m_tBaseInfo.iMaxHp = 18000;
	m_tBaseInfo.iHp = 18000;
	m_tBaseInfo.iPrevHp = 18000;
	m_tBaseInfo.iSuperAmmor = m_iBasicSuperArmor;
	m_tBaseInfo.iMaxSuperAmmor = 3000;
	m_tBaseInfo.eObjectID = OBJECT_PLAYER;
	m_tBaseInfo.vObjectPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	m_tBaseInfo.eEffectID = EFFECT_END;
	m_tBaseInfo.iCheatATK = 0;

	m_iCurHp = m_tBaseInfo.iHp;
	m_iDifHp = 0;

	m_iMaxMp = 1000.f;
	m_iMp = 1000.f;

	m_iStamina = 1000;
	m_iMaxStamina = 1000;

	m_dbRage = 0.f;
	m_dbMaxRage = 100.f;

	m_iPreHp = m_tBaseInfo.iPrevHp;
	m_iDifPreHp = 0;

	m_eFlagTeam = TEAM_BLUE;

	m_tBaseInfo.iCritical = 3;
	//	

	m_iAirComboLevel = 0;


	// Ai
	m_eFlagTeam = TEAM::TEAM_BLUE;
	m_eCtrlType = CONTROLTYPE::CTRL_PLAYER;


	//옵저버
	if (FAILED(Engine::AddSubject(Engine::PLAYER)))
		return E_FAIL;

	m_pRendererCom->Set_Monochrome(false);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _double & dTimeDelta)
{
	if (!m_bLateInit)
		LateInit();

	Link_NaviMesh();

	//Check Decal
	if (m_pDecal != nullptr)
	{
		if (m_pDecal->Get_NeedToReleaseDecal())
		{
			m_pDecal->Set_Dead();
			m_pDecal = nullptr;
		}
	}
	if (!m_bEnterSound && CUIMgr::GetInstance()->Get_UIType() != CUIMgr::UITYPE_MAIN)
	{
		/*m_dbEnterTime += dTimeDelta;
		if (1.0 <= m_dbEnterTime)
		{
			_uint i = rand() % 6 + 18;
			CSoundMgr::Get_Instance()->SoundOn(i);
			m_bEnterSound = true;
		}*/
	}

	Update_Status(dTimeDelta);

	if (m_bIncTime)
	{
		m_dbInvTime += dTimeDelta;
		////cout << "무적" << endl;
		if (0.5 <= m_dbInvTime)
		{
			m_dbInvTime = 0.0;
			m_bincibility = false;
			m_bIncTime = false;
		}
	}

	if (m_bIsDead)
	{
		Dead_Player(dTimeDelta);
	}

	if (m_bTargetLock)
	{
		if (200.f <= D3DXVec3Length(&(*m_pTransformCom->Get_Info(Engine::INFO_POS) - m_vFocus)))
			m_bTargetLock = false;
	}

	//if(!m_bIsAttacked)
	if (!m_pUIMgr->Get_UI_Working() && !m_bIsDead)		//UI가 꺼져 있을때만 상태변화일어남 -희정 추가
	{
		//if (m_bCutscene)
		//{
		//	m_eCurState = COMMON_WAIT_HABIT;
		//	m_pDynamicMeshCom->Set_AnimationSet(COMMON_WAIT_HABIT);
		//	m_dbAnimationSpeed = 1.5;
		//	if (m_pDynamicMeshCom->Is_AnimationSetFinish(0.2))
		//	{
		//		m_bCutscene = false;
		//		m_dbAnimationSpeed = 1;
		//	}
		//}
		//else
		//	m_pState->Update_State(dTimeDelta);

		if (m_pUIMgr->Get_UIType() == CUIMgr::UITYPE::UITYPE_CARTEL)
		{
			if (CAiMgr::GetInstance()->Get_CartelStart() == true)
				m_pState->Update_State(dTimeDelta);
		}
		else
		m_pState->Update_State(dTimeDelta);
	}
	else if (m_pUIMgr->Get_UI_Working())					//UI가 켜져있을 땐 일반 상태
	{
		m_eCurState = COMMON_COMBATWAIT;
		m_pDynamicMeshCom->Set_AnimationSet(COMMON_COMBATWAIT);
	}

	InGyunCodeTest(dTimeDelta);
	SiWonCodeTest(dTimeDelta);
	YongHoCodeTest(dTimeDelta);
	HeeJungCodeTest(dTimeDelta);
	YeongJoonCodeTest(dTimeDelta);

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

	CCollisionMgr::GetInstance()->Add_Collision(COLL_PLAYER, this);

	//_vec3* pPos = m_pTransformCom->Get_Info(Engine::INFO_POS);
	//_vec3 vLook = *(m_pTransformCom->Get_Info(Engine::INFO_LOOK));
	//vLightAt = *pPos;
	CBasePlayer::Update_GameObject(dTimeDelta);

	for (_uint i = SPHERE_FIRST; i < SPHERE_END; ++i)
	{
		for (auto& iter : m_vecCollSphere[i])
			iter->Update_Component(dTimeDelta);
	}

	/////////////////////UI ->희정 추가//////////////////
	//if (CUIMgr::UITYPE_READY == m_pUIMgr->Get_UIType())
	//{
	//	if (m_pUIMgr->Get_EraseSwapKarmaUIPossible())
	//	{
	////////////////???????
	//		m_pUIMgr->Set_EraseSwapKarmaUIPossible(false);
	//	}
	//}
	/////////////////////////////////////////////////////	

	ObserverPass();


	if (Engine::KeyDown(DIK_2))
	{
		m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_RUN_DUST, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 12.f, 0.f)));

		//m_pEffectMgr->Create_TextureEffect(TEXTURE_DUSTRUNSTOP, &(*pPos + _vec3(0.f, 100.f, 0.f)), &_vec3(0.f, 0.f, 0.f), &_vec3(100.f, 100.f, 100.f), 50.f, &_vec2(4.f, 0.f), &_vec2(4.f, 0.f), true, 20.f, 4);
	}

	////////////////////////임시 코드 ///////////////////////////////////////


	if (m_eBattleType == TYPE_IDLE && Engine::KeyDown(DIK_TAB) && !CUIMgr::GetInstance()->CheckSkillCoolDowning(Engine::KEYGUIDE_TAB) && m_eIdCurrent == ID_COMMON)
	{
		/////////////////////UI ->희정 추가//////////////////
		m_pUIMgr->SkillCoolDownCheck(Engine::KEYGUIDE_TAB);
		/////////////////////////////////////////////////////		

		Change_Weapon();

		//////////UI ->희정 추가(머리 위 무기표시)////////////
		Engine::CGameObject* pGameObject = CKarmaOverHead::Create(m_pGraphicDev, m_eMainWeapon, m_pTransformCom);
		if (pGameObject == nullptr)
			return E_FAIL;
		Engine::Add_GameObject(Engine::UI, L"KarmaOverHead", pGameObject);
		/////////////////////////////////////////////////////		
	}

	m_tBaseInfo.vObjectPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);

	return Engine::NO_EVENT;
}

_int CPlayer::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBasePlayer::LateUpdate_GameObject(dTimeDelta);
	if (!m_bTransApostle)	// 돼지천사 변신
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	if (!m_pUIMgr->Get_UI_Working())
	{
		if (m_pUIMgr->Get_UIType() == CUIMgr::UITYPE::UITYPE_CARTEL)
		{
			if (CAiMgr::GetInstance()->Get_CartelStart() == true)
				m_eIdNext = m_pState->LateUpadte_State(dTimeDelta);
		}
		else
			m_eIdNext = m_pState->LateUpadte_State(dTimeDelta);

		if (m_bIsAttacked && m_eIdCurrent != CBasePlayer::ID_SKILL)
		{
			Release_Decal();
			m_eIdNext = CBasePlayer::ID_HEART;
		}
		Change_State();
	}

	//////////////////// 희정 추가/////
	MissionEnterDistanceCheck();
	///////////////////////////////////

	if (0 >= m_iStamina && !m_bIsTired)
	{
		m_iStamina = 0;
		m_bIsTired = true;
		m_pUIMgr->UseableSkillCheck_All(false);
		m_pUIMgr->CreateTiredUI(m_pGraphicDev, m_pTransformCom);
	}

	_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	if (0.f > vPos.y || 1000.f < vPos.y)
	{
 		m_pTransformCom->Set_Pos(&m_pNaviMeshCom->CheckY(vPos, m_dwNaviIndex));
		int i = 0;
		if (m_bIsJump)
		{
			m_bIsJump = false;//int j = 0;//*/
			m_bIsFall = false;
		}
	}

	if (vPos.x >= 5000.f || -5000.f >= vPos.x || vPos.z >= 5000.f || -5000.f >= vPos.z || isnan(vPos.x) || isnan(vPos.y) || isnan(vPos.z))
	{
		m_pTransformCom->Set_Pos(&vStartingPoint);
		if (m_bIsJump)
		{
			m_bIsJump = false;//int j = 0;//*/
			m_bIsFall = false;
		}
	}

	return Engine::NO_EVENT;
}

void CPlayer::Render_Geometry(const _double & dTimeDelta)
{
	//if (!m_bRender)
	//	return;

	////cout << GetPlayerPos().x << " / " << GetPlayerPos().y << " / " << GetPlayerPos().z << endl;

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
			pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
			pEffect->BeginPass(0);
			pEffect->CommitChanges();
			m_pDynamicMeshCom->Render_Meshes(iter, i);
			pEffect->EndPass();
		}
		m_pDynamicMeshCom->Render_Meshes_End(iter);
	}
	pEffect->End();

	Engine::Safe_Release(pEffect);

	CDynamicMeshObject::Render_Geometry(dTimeDelta);
}

void CPlayer::Render_PostEffect(const _double & dTimeDelta)
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

void CPlayer::Render_Shadow(const _double & dTimeDelta)
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

void CPlayer::LateInit()
{
	m_bLateInit = true;

	Engine::CGameObject*		pGameObject = nullptr;



	switch (m_eMainWeapon)
	{
	case CBasePlayer::COMMON:
		break;
	case CBasePlayer::DUALSWORD:
	{
		pGameObject = m_pDualSword_R = CDualSword::Create(m_pGraphicDev, Engine::HAND::HAND_RIGHT, L"RealPlayer", L"DS_R");
		if (pGameObject == nullptr)
			return;
		if (FAILED(Engine::Add_GameObject(Engine::GAMEOBJECT, L"DS_R", pGameObject)))
			return;

		pGameObject = m_pDualSword_L = CDualSword::Create(m_pGraphicDev, Engine::HAND::HAND_LEFT, L"RealPlayer", L"DS_L");
		if (pGameObject == nullptr)
			return;
		if (FAILED(Engine::Add_GameObject(Engine::GAMEOBJECT, L"DS_L", pGameObject)))
			return;

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_R", Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_L", Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);
	}
	break;
	case CBasePlayer::TWOHANDSWORD:
	{
		pGameObject = m_pTwoHandSword = CTwohandSword::Create(m_pGraphicDev, L"RealPlayer", L"TS");
		if (pGameObject == nullptr)
			return;
		if (FAILED(Engine::Add_GameObject(Engine::GAMEOBJECT, L"TS", pGameObject)))
			return;

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"TS", Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);
	}
	break;
	case CBasePlayer::ORB:
	{
		pGameObject = m_pOrb = COrb::Create(m_pGraphicDev, L"RealPlayer", L"OB");
		if (pGameObject == nullptr)
			return;
		if (FAILED(Engine::Add_GameObject(Engine::GAMEOBJECT, L"OB", pGameObject)))
			return;

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"OB", Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);
	}
	break;
	case CBasePlayer::LONGBOW:
	{
		pGameObject = m_pLongBow = CLongBow::Create(m_pGraphicDev, L"RealPlayer", L"LB");
		if (pGameObject == nullptr)
			return;
		if (FAILED(Engine::Add_GameObject(Engine::GAMEOBJECT, L"LB", pGameObject)))
			return;

		m_pLongBow->Update_GameObject(0);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"LB", Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);
	}
	break;
	default:
		break;
	}

	m_pState = new CCsState();
	m_pState->Enter_State(this);

	if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_CARTEL)
	{
		m_bCutscene = true;
	}
}

const _matrix CPlayer::Get_ViewMatrix() const
{
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	return matView;
}

void CPlayer::ReSet_Incibility()
{
	m_bIncTime = true;
	m_bincibility = true;
	m_dbInvTime = 0.0;
}

void CPlayer::Set_Collision(CBaseObject * pObj, COLLIDER eColliderType)
{
	if (pObj == nullptr)
		return;

	const BASE_INFO*	pBaseInfo = pObj->Get_BaseInfo();

	// 공격에 맞았다

	if (eColliderType == COLLIDER::COLLIDER_ATT_HIT)
	{
		if (m_bCheat)
			return;
		if (pBaseInfo->eObjectID == OBJID::OBJECT_MONSTER)
		{
			Set_Collision_Effect(pObj);
		}
		else if (pBaseInfo->eObjectID == OBJID::OBJECT_ENEMY_1)
		{
			Set_Collision_Effect(pObj);
		}
		else if (pBaseInfo->eObjectID == OBJID::OBJECT_ENEMY_2)
		{
			Set_Collision_Effect(pObj);
		}
	}
	// 몸통끼리 부딪혔다
	else if (eColliderType == COLLIDER::COLLIDER_DEF_HIT)
	{
		//int i = 0;
		if (pObj->Get_BaseInfo()->eObjectID == ID_PLAYER)
			return;

		if (pObj->Get_BaseInfo()->eEffectID != EFFECT_END)
			return;

		_vec3 vFocus = pObj->Get_BaseInfo()->vObjectPos;

		if (!m_bTargetLock)
		{
			m_vFocus = vFocus;
			m_bTargetLock = true;
		}
		else
		{
			_float fNewDist = D3DXVec3Length(&(*m_pTransformCom->Get_Info(Engine::INFO_POS) - vFocus));
			_float fOldDist = D3DXVec3Length(&(*m_pTransformCom->Get_Info(Engine::INFO_POS) - m_vFocus));

			if (fNewDist < fOldDist)
				m_vFocus = vFocus;
		}
	}
}

void CPlayer::Set_Collision_Effect(CBaseObject * pObj)
{
	// 충돌 구현
	// 맵 만들어서 거기에 충돌한 이펙트들 넣어주고, 시간값 돌리면 된다.
	// 스테이지가 구성되면 스테이지 매니저를 통해 누적데미지,킬/데스 등 정보를 알려주면된다.
	// map(pobj, float)

	if (m_bIsDead)
		return;

	if (m_bincibility)
		return;

	// 피격 이펙트
	if (!m_bincibility && m_mapinvincibility.end() == m_mapinvincibility.find(pObj->Get_BaseInfo()->eEffectID))
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

		// 중복 충돌 방지
		m_mapinvincibility.emplace(pObj->Get_BaseInfo()->eEffectID, pObj->Get_BaseInfo()->fEffectHitTime);

		// 회복 시간 초기화
		m_dbRecoveryTime = 0;

		// 타깃 설정
		m_eCollisionID = pObj->Get_BaseInfo()->eCollisionID;
		
		OBJID t = pObj->Get_BaseInfo()->eObjectID;
		switch (t)
		{
		case OBJECT_PLAYER:
			// 올 일 없음
			break;
		case OBJECT_ALLIANCE:
			// 얘랑도 충돌 안함
			break;
		case OBJECT_ENEMY_1:
		{
			m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Enermy_1", Engine::TRANSFORM, Engine::ID_DYNAMIC));
			m_vTargetPos = *m_pTargetTransform->Get_Info(Engine::INFO_POS);
		}
			break;
		case OBJECT_ENEMY_2:
		{
			m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Enermy_2", Engine::TRANSFORM, Engine::ID_DYNAMIC));
			m_vTargetPos = *m_pTargetTransform->Get_Info(Engine::INFO_POS);
		}
			break;
		case OBJECT_MONSTER:
		{
			if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_BOSS)
			{
				m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Belatos", Engine::TRANSFORM, Engine::ID_DYNAMIC));
				m_vTargetPos = *m_pTargetTransform->Get_Info(Engine::INFO_POS);
			}
			else if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_SHOOTING)
			{
				m_pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(Engine::GAMEOBJECT, L"Apostle", Engine::TRANSFORM, Engine::ID_DYNAMIC));
				m_vTargetPos = *m_pTargetTransform->Get_Info(Engine::INFO_POS);
			}
		}
			break;
		case OBJECT_EFFECT:
			break;
		case OBJECT_END:
			break;
		default:
			break;
		}

		//cout << m_vTargetPos.x << ", " << m_vTargetPos.y << ", " << m_vTargetPos.z << endl;

		// 크리티컬 체크
		_int iCritical = rand() % 10;

		if (pObj->Get_BaseInfo()->iCritical >= iCritical)
			m_bCritical = true;
		else
			m_bCritical = false;

		// 임시 대미지 공식
		_int test = pObj->Get_BaseInfo()->iAtt >> 2;
		_int iDif = rand() % test + 1;

		_int iBasicDmg = (pObj->Get_BaseInfo()->iAtt + iDif);

		_int iHit = rand() % 12;
		_int iVoice = rand() % 4;

		if (m_bCritical)
		{
			// 카메라 쉐이킹
			CCameraMgr::GetInstance()->Set_ShakingCamera();
			// Critical Effect
			_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
			m_pEffectMgr->Create_Effect_Critical(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 125.f, 0.f) - vDir*15.f));

			m_iCurHp -= iBasicDmg * 1.5;
			m_iPreHp = (m_tBaseInfo.iHp - iBasicDmg*0.7*1.5);
			m_pUIMgr->CreateDamageBox(m_pGraphicDev, m_pTransformCom, CDamageBox::DAMAGEBOXTYPE::CRITICAL, iBasicDmg *1.5);

			//희정 추가(깃발전에서 필요한 누적 데미지 계산을 위해서 추가.(플레이어가 맞는 입장))
			m_pUIMgr->SetAccumulatedDamageForFlag(pObj->Get_BaseInfo()->eObjectID, iBasicDmg *1.5);

			if (0 == iVoice) // 25%확률로 플레이어 보이스 출력
			{
				if (0 < m_iCurHp && m_eIdCurrent != ID_SKILL)
				{
					// Critical Hit Sound
					_uint iSound = rand() % 8 + 34;
					CSoundMgr::Get_Instance()->SoundOn(iSound);
				}
			}
			iHit += 31;
			CSoundMgr::Get_Instance()->HoSoundOn(iHit);
		}
		else
		{
			m_iCurHp -= iBasicDmg;
			m_iPreHp = (_int)(m_tBaseInfo.iHp - iBasicDmg*0.7);
			m_pUIMgr->CreateDamageBox(m_pGraphicDev, m_pTransformCom, CDamageBox::DAMAGEBOXTYPE::BASIC, iBasicDmg);

			//희정 추가(깃발전에서 필요한 누적 데미지 계산을 위해서 추가.(플레이어가 맞는 입장))
			m_pUIMgr->SetAccumulatedDamageForFlag(pObj->Get_BaseInfo()->eObjectID, iBasicDmg);

			if (0 == iVoice) // 25%확률로 플레이어 보이스 출력
			{
				if (0 < m_iCurHp && m_eIdCurrent != ID_SKILL)
				{
					// Normal Hit Sound
					_uint iSound = rand() % 11 + 24;
					CSoundMgr::Get_Instance()->SoundOn(iSound);
				}
			}
			iHit += 19;
			CSoundMgr::Get_Instance()->HoSoundOn(iHit);
			
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

			if (CUIMgr::UITYPE_CARTEL == m_pUIMgr->Get_UIType())
			{
				// 희정 추가(깃발전에서 누가누구죽였는지 뜨는 UI)
				m_pUIMgr->CheckFlagScore(m_pGraphicDev, pObj->Get_BaseInfo()->eObjectID, m_tBaseInfo.eObjectID);
				//// 희정 추가(부활 몇초전 이라고 뜨는 UI)
				//m_pUIMgr->CreateRevivalPopUp(m_pGraphicDev);
				m_bCreateRevivalUI = true;
			}

			// Player Dead
			//m_bIsDead = true;
		}

		// 피격 시 분노 상승
		m_dbRage += 6;
		if (100 < m_dbRage)
			m_dbRage = 100;

		// 슈아 깎임
		_int iSuperArmorDamage = (_int)((pObj->Get_BaseInfo()->iAtt + iDif)*0.3); //희정 수정&추가
		m_tBaseInfo.iSuperAmmor -= iSuperArmorDamage;

		////cout << (_int)((pObj->Get_BaseInfo()->iAtt + iDif)*0.3) << endl;

		// 슈아 깨짐
		if (0 >= m_tBaseInfo.iSuperAmmor)
		{
			if (!m_bBreak)
			{
				// 카메라 쉐이킹
				CCameraMgr::GetInstance()->Set_ShakingCamera();
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
			if (CUIMgr::UITYPE_CARTEL == m_pUIMgr->Get_UIType())
			{
				m_pUIMgr->SetAccumulatedBreakForFlag(pObj->Get_BaseInfo()->eObjectID, iSuperArmorDamage);
			}
		}

		m_pState->Is_Attacked();
		m_bIsAttacked = true;

	}
}


HRESULT CPlayer::Clone_Component()
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

	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_RealPlayer"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	// Picking
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone(Engine::RESOURCE_STATIC, L"CalculatorCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::CALCULATOR, pComponent);
	return S_OK;
}

HRESULT CPlayer::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	pEffect->SetVector("vChangeColor", &_vec4(0.7f, 0.7f, 0.7f, 1.f));
	return S_OK;
}

void CPlayer::Change_State()
{
	if (m_eIdCurrent == CBasePlayer::ID_SKILL)
	{
		if (m_bIsAttacked)
			m_bIsAttacked = false;
	}

	if (m_eIdCurrent != m_eIdNext)
	{
		if (m_pState)
			delete m_pState;

		m_eIdCurrent = m_eIdNext;

		switch (m_eIdNext)
		{
		case CBasePlayer::ID_COMMON:
		{
			m_pState = new CCsState;
			m_pState->Enter_State(this);
			// Weapon Swap
			// Weapon의 1번 애니메이션은 Wait
			if (m_eMainWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
			{
				m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_R", Engine::MESH, Engine::ID_STATIC));
				m_pWeaponCom->Set_AnimationSet(1);

				m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_L", Engine::MESH, Engine::ID_STATIC));
				m_pWeaponCom->Set_AnimationSet(1);
			}
			else if (m_eMainWeapon == PLAYER_WEAPONTYPE::TWOHANDSWORD) //희정추가
			{
				m_pTwoHandSword->Set_CombatState(CTwohandSword::STATE_WAIT);
				m_pWeaponCom->Set_AnimationSet(1);
			}
			else if (m_eMainWeapon == PLAYER_WEAPONTYPE::LONGBOW)
			{
				m_pLongBow->Set_CombatState(CLongBow::STATE_WAIT);
				m_pWeaponCom->Set_AnimationSet(1);
			}
			else	// 듀얼소드는 무기 두개를 같이 Set시켜야한다.
			{
				m_pWeaponCom->Set_AnimationSet(1);
			}
			return;
		}
		break;
		case CBasePlayer::ID_BASICCOMBO:
			m_pState = new CBasicCombo;
			break;
		case CBasePlayer::ID_BASICCOMBOEX:
			m_pState = new CBasicComboEx;
			break;

		case CBasePlayer::ID_AIRCOMBO:
		{
			m_pState = new CAirCombo;
		}
		break;

		case CBasePlayer::ID_SKILL:
		{
			m_pState = new CPlayerSkill;
		}
		break;

		case CBasePlayer::ID_HEART:
			m_pState = new CHeartState;
			m_pState->Enter_State(this);

			// Weapon Swap
			// Weapon의 1번 애니메이션은 Wait
			if (m_eMainWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
			{
				m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_R", Engine::MESH, Engine::ID_STATIC));
				m_pWeaponCom->Set_AnimationSet(1);

				m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_L", Engine::MESH, Engine::ID_STATIC));
				m_pWeaponCom->Set_AnimationSet(1);
			}
			else if (m_eMainWeapon == PLAYER_WEAPONTYPE::TWOHANDSWORD) //희정추가
			{
				m_pTwoHandSword->Set_CombatState(CTwohandSword::STATE_WAIT);
				m_pWeaponCom->Set_AnimationSet(1);
			}
			else if (m_eMainWeapon == PLAYER_WEAPONTYPE::LONGBOW)
			{
				m_pLongBow->Set_CombatState(CLongBow::STATE_WAIT);
				m_pWeaponCom->Set_AnimationSet(1);
			}
			else	// 듀얼소드는 무기 두개를 같이 Set시켜야한다.
			{
				m_pWeaponCom->Set_AnimationSet(1);
			}
			return;
			break;
		case CBasePlayer::STATEID_END:
			m_pState = new CBasicCombo;

			break;
		default:
			m_pState = new CBasicCombo;

			break;
		}

		m_pState->Enter_State(this);

		// Weapon Swap
		// Weapon의 1번 애니메이션은 Wait
		if (m_eMainWeapon == PLAYER_WEAPONTYPE::DUALSWORD)
		{
			m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_R", Engine::MESH, Engine::ID_STATIC));
			m_pWeaponCom->Set_AnimationSet(0);

			m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"DS_L", Engine::MESH, Engine::ID_STATIC));
			m_pWeaponCom->Set_AnimationSet(0);
		}
		else if (m_eMainWeapon == PLAYER_WEAPONTYPE::TWOHANDSWORD) //희정추가
		{
			m_pTwoHandSword->Set_CombatState(CTwohandSword::STATE_COMBAT);
			m_pWeaponCom->Set_AnimationSet(1);
		}
		else if (m_eMainWeapon == PLAYER_WEAPONTYPE::LONGBOW)
		{
			m_pLongBow->Set_CombatState(CLongBow::STATE_COMBAT);
			m_pWeaponCom->Set_AnimationSet(1);
		}
		else	// 듀얼소드는 무기 두개를 같이 Set시켜야한다.
		{
			m_pWeaponCom->Set_AnimationSet(0);
		}

	}
}

void CPlayer::Update_Status(const _double dTimeDelta)
{
	if (!m_bIsDead && m_iCurHp <= m_tBaseInfo.iMaxHp *0.5)
	{
		m_dbHeartbeatSoundTime += dTimeDelta;
		if (m_iCurHp <= m_tBaseInfo.iMaxHp *0.5 && m_iCurHp > m_tBaseInfo.iMaxHp *0.25)
		{
			m_dbHBSoundCycle = 1.0;
			CSoundMgr::Get_Instance()->SetVolumeDown(0.5f);
			CSoundMgr::Get_Instance()->Set_DefaultVolume(0.05f);
		}
		else if (m_iCurHp <= m_tBaseInfo.iMaxHp *0.25 && m_iCurHp > m_tBaseInfo.iMaxHp *0.1)
		{
			m_dbHBSoundCycle = 0.75;
			CSoundMgr::Get_Instance()->SetVolumeDown(0.25f);
			CSoundMgr::Get_Instance()->Set_DefaultVolume(0.1f);
		}
		else if (m_iCurHp <= m_tBaseInfo.iMaxHp *0.1 && 0 < m_iCurHp)
		{
			m_dbHBSoundCycle = 0.5;
			CSoundMgr::Get_Instance()->SetVolumeDown(0.1f);
			CSoundMgr::Get_Instance()->Set_DefaultVolume(0.2f);
		}
		if (m_dbHBSoundCycle <= m_dbHeartbeatSoundTime)
		{
			m_dbHeartbeatSoundTime = 0.0;
			_int iSound = rand() % 2 + 17;
			CSoundMgr::Get_Instance()->HoSoundOn(iSound, 1.5f);
		}
	}

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

		if (2.0 <= m_dbRecoveryTime)
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
		m_pUIMgr->UseableSkillCheck_All(false);
		
		m_dbTiredTime += dTimeDelta;
		if (1.25 <= m_dbTiredTime)
		{
			_uint iSound = rand() % 2 + 84;
			CSoundMgr::Get_Instance()->SoundOn(iSound);
			m_dbTiredTime = 0;
		}

		if (3 <= m_bTiredTime)
		{
			m_bIsTired = false;
			m_pUIMgr->EraseTiredUI();
			m_bTiredTime = 0;
			m_pUIMgr->UseableSkillCheck_All(true);

		}
	}

	// Test yh
}

void CPlayer::Change_Weapon()
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
		pWeapon = dynamic_cast<CDynamicMeshObject*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"TS"));
		pWeapon->Set_Dead(true);
		m_pTwoHandSword = nullptr;
		Create_SubWeapon();
	}
	break;
	case CBasePlayer::ORB:
	{
		pWeapon = dynamic_cast<CDynamicMeshObject*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"OB"));
		pWeapon->Set_Dead(true);
		m_pOrb = nullptr;
		Create_SubWeapon();
	}
	break;
	case CBasePlayer::LONGBOW:
	{
		pWeapon = dynamic_cast<CDynamicMeshObject*>(Engine::Get_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"LB"));
		pWeapon->Set_Dead(true);
		m_pLongBow = nullptr;
		Create_SubWeapon();
	}
	break;
	default:
		break;
	}
}

void CPlayer::Create_SubWeapon()
{
	CDynamicMeshObject* pWeapon;
	switch (m_eSubWeapon)
	{
	case CBasePlayer::COMMON:
		break;
	case CBasePlayer::DUALSWORD:
	{
		swap(m_eMainWeapon, m_eSubWeapon);
		m_pDualSword_R = CDualSword::Create(m_pGraphicDev, Engine::HAND_RIGHT, L"RealPlayer", L"DS_R");
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_R", m_pDualSword_R);

		m_pDualSword_L = CDualSword::Create(m_pGraphicDev, Engine::HAND_LEFT, L"RealPlayer", L"DS_L");
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"DS_L", m_pDualSword_L);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(m_pDualSword_R->Get_Component(Engine::COMPONENTTYPE::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(m_pDualSword_L->Get_Component(Engine::COMPONENTTYPE::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);

		/////////////////////UI ->희정 추가//////////////////
		m_pUIMgr->SwitchSkill(m_pGraphicDev, L"Skill_DS", (CSkill::SKILLTYPE)m_eMainWeapon);
		/////////////////////////////////////////////////////
	}
	break;
	case CBasePlayer::TWOHANDSWORD:
	{
		swap(m_eMainWeapon, m_eSubWeapon);

		pWeapon = m_pTwoHandSword = CTwohandSword::Create(m_pGraphicDev, L"RealPlayer", L"TS");
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"TS", pWeapon);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"TS", Engine::MESH, Engine::ID_STATIC));

		if (m_eBattleType == TYPE_BATTLE)
			dynamic_cast<CTwohandSword*>(m_pTwoHandSword)->Change_WeaponState(CTwohandSword::STATE_WAIT);
		//m_pWeaponCom->Set_AnimationSet(1);

		/////////////////////UI ->희정 추가//////////////////
		m_pUIMgr->SwitchSkill(m_pGraphicDev, L"Skill_TS", (CSkill::SKILLTYPE)m_eMainWeapon);
		/////////////////////////////////////////////////////
	}
	break;
	case CBasePlayer::ORB:
	{
		swap(m_eMainWeapon, m_eSubWeapon);

		pWeapon = m_pOrb = COrb::Create(m_pGraphicDev, L"RealPlayer", L"OB");
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"OB", pWeapon);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"OB", Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);

		/////////////////////UI ->희정 추가//////////////////
		m_pUIMgr->SwitchSkill(m_pGraphicDev, L"Skill_ORB", (CSkill::SKILLTYPE)m_eMainWeapon);
		/////////////////////////////////////////////////////
	}
	break;
	case CBasePlayer::LONGBOW:
	{
		swap(m_eMainWeapon, m_eSubWeapon);

		pWeapon = m_pLongBow = CLongBow::Create(m_pGraphicDev, L"RealPlayer", L"LB");
		Engine::Add_GameObject(Engine::LAYERTYPE::GAMEOBJECT, L"LB", pWeapon);

		m_pWeaponCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(Engine::GAMEOBJECT, L"LB", Engine::MESH, Engine::ID_STATIC));
		m_pWeaponCom->Set_AnimationSet(1);

		/////////////////////UI ->희정 추가//////////////////
		m_pUIMgr->SwitchSkill(m_pGraphicDev, L"Skill_LB", (CSkill::SKILLTYPE)m_eMainWeapon);
		/////////////////////////////////////////////////////
	}
	break;
	default:
		break;
	}

	////////// 희정 추가 ////////////////////////
	//m_pUIMgr->Set_PlayerMainWeapon(m_eMainWeapon);
	//m_pUIMgr->Set_PlayerSubWeapon(m_eSubWeapon);
	//////////////////////////////////////////////////
}

_int CPlayer::Check_Enemy()
{
	if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_BOSS)
	{
		if (CUIMgr::GetInstance()->Get_BelatosTransformCom() == nullptr)
			return 0;
		_float fPlayerToBelatosDist = Engine::GetDist(*(CUIMgr::GetInstance()->Get_BelatosTransformCom()->Get_Info(Engine::INFO_POS)), GetPlayerPos());
		if (fPlayerToBelatosDist < 700.f)
			return 1;
		return 0;
	}
	else if (CUIMgr::GetInstance()->Get_UIType() == CUIMgr::UITYPE_CARTEL)
	{
		if (CUIMgr::GetInstance()->Get_AI1TransformCom() == nullptr)
			return 0;
		if (CUIMgr::GetInstance()->Get_AI2TransformCom() == nullptr)
			return 0;
		_float fPlayerToAI1Dist = Engine::GetDist(*(CUIMgr::GetInstance()->Get_AI1TransformCom()->Get_Info(Engine::INFO_POS)), GetPlayerPos());
		_float fPlayerToAI2Dist = Engine::GetDist(*(CUIMgr::GetInstance()->Get_AI2TransformCom()->Get_Info(Engine::INFO_POS)), GetPlayerPos());
		if (fPlayerToAI1Dist < 700.f)
		{
			if (fPlayerToAI2Dist < fPlayerToAI1Dist)
				return 3;
			else
				return 2;
		}
		if (fPlayerToAI2Dist < 700.f)
		{
			if (fPlayerToAI1Dist < fPlayerToAI2Dist)
				return 2;
			else
				return 3;
		}
		if (fPlayerToAI1Dist > 700.f && fPlayerToAI2Dist > 700.f)
			return 0;
	}
	return 0;
}

void CPlayer::Dead_Player(const _double & dTimeDelta)
{
	if (m_bIsJump)
	{
		//if (m_eCurState == COMMON_RTDOWN_AIR_LANDING_F_F)
		//{
		//	if (m_pDynamicMeshCom->Is_AnimationSetFinish(0.2))
		//	{

		//	}
		//	return;
		//}

		_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
		m_dbFallTime += dTimeDelta;

		m_eCurState = COMMON_RTSTAND_AIR_FALL_SMALL_F;
		m_pDynamicMeshCom->Set_AnimationSet(COMMON_RTSTAND_AIR_FALL_SMALL_F);

		m_pTransformCom->Set_Pos(&m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(*m_pTransformCom->Get_Info(Engine::INFO_LOOK) * 750.f * (_float)(1.0 - m_dbFallTime)*(_float)dTimeDelta), m_dwNaviIndex, true));
		m_pTransformCom->Set_Pos(&(m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(_vec3(0.f, -1.f, 0.f) * 250.f * (_float)(1.5 - m_dbFallTime) *(_float)dTimeDelta), m_dwNaviIndex, true)));

		if (m_pNaviMeshCom->Check_Landing(&vPos, m_dwNaviIndex))
		{
			_vec3 vPosY;
			m_pNaviMeshCom->Find_PosY(&vPos, m_dwNaviIndex, vPosY);
			m_pTransformCom->Set_Pos(&vPosY);
			m_pDynamicMeshCom->Set_AnimationSet(CPlayer::COMMON_RTDOWN_AIR_LANDING_F_F);
			m_eCurState = COMMON_RTDOWN_AIR_LANDING_F_F;
			m_bIsJump = false;
			m_bIsFall = false;
			//m_eDeadStep = DEAD1;
		}
	}
	else
	{

		switch (m_eDeadStep)
		{
		case CPlayer::DEAD1:
		{
			CCameraMgr::GetInstance()->ReSet_ActionZoom();
			m_eCurState = COMMON_DYING;
			m_pDynamicMeshCom->Set_AnimationSet(COMMON_DYING);
			m_pRendererCom->Set_Monochrome(true);
			if (!m_bDeadSound)
			{
				_uint i = rand() % 12 + 6;
				CSoundMgr::Get_Instance()->SoundOn(i);
				m_bDeadSound = true;
			}
			if (m_pDynamicMeshCom->Is_AnimationSetFinish(0.2))
				m_eDeadStep = DEAD2;
		}
		break;
		case CPlayer::DEAD2:
		{
			m_dbDeadTime += dTimeDelta;
			m_eCurState = COMMON_DOWN_B_B;
			m_pDynamicMeshCom->Set_AnimationSet(COMMON_DOWN_B_B);
			if (5 <= m_dbDeadTime)
			{
				m_dbDeadTime = 0.0;
				m_eDeadStep = DEAD3;
				CSoundMgr::Get_Instance()->SoundOn(79);
			}

			if (CUIMgr::UITYPE_CARTEL == m_pUIMgr->Get_UIType())
			{
				if (m_bCreateRevivalUI)
				{
					// 희정 추가(부활 몇초전 이라고 뜨는 UI)
					m_pUIMgr->CreateRevivalPopUp(m_pGraphicDev);

					m_bCreateRevivalUI = false;
				}
			}
		}
		break;
		case CPlayer::DEAD3:
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
				m_dbFallTime = 0.0;
				m_bIncTime = true;
				m_bDeadSound = false;
				m_pRendererCom->Set_Monochrome(false);
				m_bIsAttacked = false;
				m_eIdNext = ID_COMMON;
				Change_State();
				CSoundMgr::Get_Instance()->SetVolumeDown(0.35f);
				CSoundMgr::Get_Instance()->Set_DefaultVolume(0.0f);

				//희정 추가 //보스 결과창 UI에 필요한 플레이어 죽은 횟수 체크
				if (CUIMgr::UITYPE_BOSS == m_pUIMgr->Get_UIType())
					m_pUIMgr->SetAccumulatedDeath(1);

				_uint iSound = rand() % 3 + 80;
				CSoundMgr::Get_Instance()->SoundOn(iSound);
			}
		}
		break;
		case CPlayer::DEAD_END:
			break;
		default:
			break;
		}
	}
}


void CPlayer::InGyunCodeTest(const _double & dTimeDelta)
{
	_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	vPos += _vec3(0.f, 100.f, 0.f);
	_vec3 vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vAngle = *m_pTransformCom->Get_Angle();
	//if (Engine::KeyDown(DIK_F6))
	//{
	//	//_vec3 vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	//	//Engine::CGameObject* p = nullptr;
	//	//p = CPhoenix::Create(m_pGraphicDev, m_pTransformCom->Get_Info(Engine::INFO_POS), &vLook);
	//	//Engine::Add_GameObject(Engine::GAMEOBJECT, L"Phoenix", p);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_PHOENIX_WIND2, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_TRAIL, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_TRAIL2, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_FIREBALL_WIND, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE, &_vec3(100.f, 150.f, 20.f));
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE2, &_vec3(100.f, 150.f, 20.f));
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE3, &_vec3(100.f, 150.f, 20.f));
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_BLINK_SINGLE, &_vec3(100.f, 150.f, 20.f));
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE4, &_vec3(100.f, 150.f, 20.f));
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_HIT_GROUND, &_vec3(100.f, 150.f, 20.f));
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_FLARE5, &_vec3(100.f, 150.f, 20.f));
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_DUST, &_vec3(100.f, 150.f, 20.f), m_pTransformCom->Get_Angle());
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_R_BULLET, &_vec3(100.f, 500.f, 20.f));

	//	//Engine::CGameObject* pGameObject = nullptr;
	//	//pGameObject = COrbCore::Create(m_pGraphicDev, COrbCore::ORB_SPIN_FRONT_RIGHT, &vPos, m_pTransformCom->Get_Angle(), &vLook);
	//	//Engine::Add_GameObject(Engine::GAMEOBJECT, L"OrbCore", pGameObject);

	//}
	//if (Engine::KeyDown(DIK_F7))
	//{
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_PHOENIX_WIND1, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_PHOENIX_WIND2, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_PHOENIX_WIND3, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND2, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_ULT_GROUND2, &vPos);
	//	/*	m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB3, &_vec3(0.f, 150.f, 20.f));
	//	m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB1, &_vec3(0.f, 150.f, 20.f));
	//	m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB2, &_vec3(0.f, 150.f, 20.f));

	//	m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB3, &_vec3(100.f, 150.f, 20.f));
	//	m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB1, &_vec3(100.f, 150.f, 20.f));
	//	m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB2, &_vec3(100.f, 150.f, 20.f));*/
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_ULT_SWIRL, &_vec3(100.f, 150.f, 20.f));
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_FLARE_BLINK, &_vec3(100.f, 150.f, 20.f));
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_BLINK, &_vec3(100.f, 150.f, 20.f));
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_BEAM, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_R_PRE, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND3, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND2, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_WIND4, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_ORB_ULT_WIND, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_DUST, &vPos, m_pTransformCom->Get_Angle());
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_ULT_GROUND, m_pTransformCom->Get_Info(Engine::INFO_POS), m_pTransformCom->Get_Angle());
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_R_FLARE, &vPos);
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_DUST, &_vec3(100.f, 150.f, 20.f), m_pTransformCom->Get_Angle());
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_DUST_LARGE_YELLOW, &_vec3(100.f, 150.f, 20.f), m_pTransformCom->Get_Angle());
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_RING, &_vec3(100.f, 150.f, 20.f), m_pTransformCom->Get_Angle());
	//	vAngle += _vec3(0.f, 50.f, 0.f);
	//	m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR1, &vPos, &vAngle);
	//	m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR2, &vPos, &vAngle);
	//	/*vAngle += _vec3(0.f, -40.f, 0.f);
	//	m_pEffectMgr->Create_TextureEffect(TEXTURE_ORB_CREATOR1, &vPos, &vAngle);*/
	//}
}

void CPlayer::SiWonCodeTest(const _double & dTimeDelta)
{
	////이펙트 테스트용 코드
	//if (Engine::KeyDown(DIK_T))
	//{
	//	m_pEffectMgr->Create_Particle(PARTICLETYPE::PARTICLE_RADICAL, &(m_pTransformCom->m_vInfo[3]));AA
	//}
	//if (Engine::KeyDown(DIK_Y))
	//{
	//	m_pEffectMgr->Create_Particle(PARTICLETYPE::PARTICLE_LEAF, &(m_pTransformCom->m_vInfo[3]));
	//}
	//if (Engine::KeyDown(DIK_G))
	//{
	//	m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_99, m_pTransformCom->Get_WorldMatrix(), &(m_pTransformCom->m_vAngle), m_tBaseInfo);
	//}
	//if (Engine::KeyDown(DIK_H))
	//{	
	//	m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_BASICTRAIL5, m_pTransformCom->Get_WorldMatrix(), &(m_pTransformCom->m_vAngle), m_tBaseInfo);
	//}

	//if (Engine::KeyDown(DIK_J))
	//{
	//	m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_200, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_DUSTRUNSTOP, &(*pPos + _vec3(0.f, 100.f, 0.f)), &_vec3(0.f, 0.f, 0.f), &_vec3(100.f, 100.f, 100.f), 50.f, &_vec2(4.f, 0.f), &_vec2(4.f, 0.f), true, 20.f, 4);
	//}
	if (Engine::KeyDown(DIK_2)) // Cri
	{
		_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
		m_pEffectMgr->Create_Effect_Critical(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 125.f, 0.f) - vDir*70.f));
	}
	if (Engine::KeyDown(DIK_3)) // Break
	{
		_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
		m_pEffectMgr->Create_Effect_Break(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 125.f, 0.f) - vDir*70.f));
	}

	// =============================== 피격 이펙트 =======================================
	//int inum = rand() % 3;
	//if (inum == 0)
	//{
	//	_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
	//	m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_HIT_1, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 70.f, 0.f) - vDir*15.f));
	//	m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_HIT_3, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 70.f, 0.f) - vDir*15.f));
	//}
	//else if (inum == 1)
	//{
	//	_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
	//	m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_HIT_2, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 70.f, 0.f) - vDir*15.f));
	//}
	//else
	//{
	//	_vec3 vDir = Engine::GetDirNoY(m_pTransformCom->m_vInfo[Engine::INFO_POS], CCameraMgr::GetInstance()->Get_Pos());
	//	m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_HIT_1, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 70.f, 0.f) - vDir*15.f));
	//	m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_HIT_4, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 85.f, 0.f) - vDir*15.f));
	//}
	// ===================================================================================


	// 적용 완료
	// ==================== Q 스킬 사용가능 이펙트 =======================================
	//if (rand() % 13< 2)
	//{
	//	_vec3 vRanPos;
	//	vRanPos.x = (rand() % 40) - 20.f;
	//	vRanPos.y = (rand() % 70) + 30.f;
	//	vRanPos.z = (rand() % 40) - 20.f;
	//	m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_READY_ULT, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + vRanPos));
	//}
	//if (rand() % 20 < 2)
	//{
	//	_vec3 vRanPos;
	//	vRanPos.x = (rand() % 40) - 20.f;
	//	vRanPos.y = (rand() % 70) + 30.f;
	//	vRanPos.z = (rand() % 40) - 20.f;
	//	m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_READY_ULT2, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + vRanPos));
	//}
	// ===================================================================================
	//if (Engine::KeyDown(DIK_2))
	//{
	//	m_pEffectMgr->Create_TextureEffect(TEXTUREEFFECT::TEXTURE_RUN_DUST, &(m_pTransformCom->m_vInfo[Engine::INFO_POS] + _vec3(0.f, 12.f, 0.f)));

	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_DUSTRUNSTOP, &(*pPos + _vec3(0.f, 100.f, 0.f)), &_vec3(0.f, 0.f, 0.f), &_vec3(100.f, 100.f, 100.f), 50.f, &_vec2(4.f, 0.f), &_vec2(4.f, 0.f), true, 20.f, 4);
	//}
	//if (Engine::KeyDown(DIK_J))
	//{
	//	m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_200, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	//	//m_pEffectMgr->Create_TextureEffect(TEXTURE_DUSTRUNSTOP, &(*pPos + _vec3(0.f, 100.f, 0.f)), &_vec3(0.f, 0.f, 0.f), &_vec3(100.f, 100.f, 100.f), 50.f, &_vec2(4.f, 0.f), &_vec2(4.f, 0.f), true, 20.f, 4);
	//}
}

void CPlayer::YongHoCodeTest(const _double & dTimeDelta)
{
	if (Engine::KeyDown(DIK_B))
	{
		//m_tBaseInfo.iSuperAmmor = 1000;
		//CCameraMgr::GetInstance()->Change_Camera(CCameraMgr::DYNAMIC);
		//_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
		//_vec3 vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
		//_vec3 vRight = *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
		//_vec3 test = *m_pTransformCom->Get_Angle();wws
		//Engine::CGameObject* pGameObject = nullptr;
		//pGameObject = CDsBullet::Create(Engine::Get_Device(),  &(vPos + _vec3(0.f, 50.f, 0.f) + vRight*250.f), &test, vPos + _vec3(0.f, 50.f, 0.f) + vLook*500.f);
		//if (pGameObject == nullptr)
		//	return;// CPlayer::ID_SKILL;
		//Engine::Add_GameObject(Engine::GAMEOBJECT, L"DSBullet", pGameObject);

		//pGameObject = CDsBullet::Create(Engine::Get_Device(), &(vPos + _vec3(0.f, 50.f, 0.f) + -vRight*250.f), &test, vPos + _vec3(0.f, 50.f, 0.f) + vLook*500.f, false);
		//if (pGameObject == nullptr)
		//	return;// CPlayer::ID_SKILL;
		//Engine::Add_GameObject(Engine::GAMEOBJECT, L"DSBullet", pGameObject);
		//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_AIRBULLET1, m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_Angle(), m_tBaseInfo);
	}
	if (Engine::KeyDown(DIK_K))
	{
		//CCameraMgr::GetInstance()->Change_Camera(CCameraMgr::STATIC);
		m_tBaseInfo.iHp = m_tBaseInfo.iPrevHp = 18000;
		Reset_SuperAmmor();
		m_iCurHp = m_iPreHp = m_iDifPreHp = m_iDifHp = m_tBaseInfo.iHp;
	}
	if (Engine::KeyDown(DIK_P))
	{
		m_bCheat = true;
		m_tBaseInfo.iCheatATK = 999999;
	}

	if (Engine::KeyDown(DIK_G))
	{
		m_dbRage += 100;
	}

	if (Engine::KeyDown(DIK_T))
	{
		m_eSubWeapon = PLAYER_WEAPONTYPE::ORB;
	}

	if (Engine::KeyDown(DIK_Y))
	{
		m_eSubWeapon = PLAYER_WEAPONTYPE::LONGBOW;
	}

	if (Engine::KeyDown(DIK_U))
	{
		m_eSubWeapon = PLAYER_WEAPONTYPE::DUALSWORD;
	}

	if (Engine::KeyDown(DIK_H))
	{
		m_eSubWeapon = PLAYER_WEAPONTYPE::TWOHANDSWORD;
	}
}

void CPlayer::HeeJungCodeTest(const _double & dTimeDelta)
{
	if (Engine::KeyDown(DIK_PGUP))
	{
		//포탈미션
		CQuestMgr::GetInstance()->Set_YellowPotalQCompleted(true);
	}

	if (Engine::KeyDown(DIK_PGDN))
	{
		//노란머리NPC 의 미션을 클리어함 - 무기구해오기 돼지천사
		CQuestMgr::GetInstance()->Set_MissionCompleted(CQuestMgr::YELLOW, true);
	}

	if (Engine::KeyDown(DIK_INSERT))
	{
		//빨간머리NPC 의 미션을 클리어함 - 짝맞추기
		CQuestMgr::GetInstance()->Set_MissionCompleted(CQuestMgr::RED, true);
	}	
	if (Engine::KeyDown(DIK_DELETE))
	{
		//빨간머리NPC 의 미션을 클리어함 - 무기구해오기 벨라토스
		CQuestMgr::GetInstance()->Set_SecondMissionCompleted(CQuestMgr::RED, true);
	}
}

void CPlayer::YeongJoonCodeTest(const _double & dTimeDelta)
{
}

void CPlayer::ExChange_State(CPlayerState* pState, _bool bAtk)
{
	int i = 0;
}

void CPlayer::ObserverPass()
{
	Engine::Notify(Engine::PLAYER, CPlayerObserver::POS, (void*)(m_pTransformCom->Get_Info(Engine::INFO_POS)));
	Engine::Notify(Engine::PLAYER, CPlayerObserver::LOOK, (void*)(m_pTransformCom->Get_Info(Engine::INFO_LOOK)));

	Engine::Notify(Engine::PLAYER, CPlayerObserver::HP, (void*)(&m_tBaseInfo.iHp));
	Engine::Notify(Engine::PLAYER, CPlayerObserver::PREVHP, (void*)(&m_tBaseInfo.iPrevHp));
	Engine::Notify(Engine::PLAYER, CPlayerObserver::MAXHP, (void*)(&m_tBaseInfo.iMaxHp));
	Engine::Notify(Engine::PLAYER, CPlayerObserver::SUPERARMOR, (void*)(&m_tBaseInfo.iSuperAmmor));
	Engine::Notify(Engine::PLAYER, CPlayerObserver::MAXSUPERARMOR, (void*)(&m_tBaseInfo.iMaxSuperAmmor));
	//스테미나 mp 
	Engine::Notify(Engine::PLAYER, CPlayerObserver::STAMINA, (void*)(&m_iStamina));
	Engine::Notify(Engine::PLAYER, CPlayerObserver::MAXSTAMINA, (void*)(&m_iMaxStamina));
	Engine::Notify(Engine::PLAYER, CPlayerObserver::MP, (void*)(&m_iMp));
	Engine::Notify(Engine::PLAYER, CPlayerObserver::MAXMP, (void*)(&m_iMaxMp));
	//들고 있는 무기
	Engine::Notify(Engine::PLAYER, CPlayerObserver::MAINWEAPON, (void*)(&m_eMainWeapon));
	Engine::Notify(Engine::PLAYER, CPlayerObserver::SUBWEAPON, (void*)(&m_eSubWeapon));

	// 분노
	Engine::Notify(Engine::PLAYER, CPlayerObserver::RAGE, (void*)(&m_dbRage));
	Engine::Notify(Engine::PLAYER, CPlayerObserver::MAXRAGE, (void*)(&m_dbMaxRage));
}

void CPlayer::MissionEnterDistanceCheck()
{
	if (m_bMissionMapCheck)
	{
		_vec3 vPlayerPos;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPlayerPos);
		_float fDist = D3DXVec3Length(&(_vec3(-595.56f, vPlayerPos.y, -766.9f) - vPlayerPos)); // -595.56f, 65.38f, -766.9f

		if (500.f >= fDist)
		{
			if (Engine::KeyDown(DIK_M))
			{
				m_pUIMgr->ChangeRenderState(L"NoFuncObj", false);
				m_pUIMgr->CreateMissionUI(m_pGraphicDev);
				m_bMissionMapCheck = false;

				return;
			}

			m_pUIMgr->ChangeRenderState(L"NoFuncObj", true);
		}
		else
		{
			m_pUIMgr->ChangeRenderState(L"NoFuncObj", false);

		}
	}
	else // missionui 떠있는상황
	{
		if (Engine::KeyDown(DIK_M))
		{
			if (CUIMgr::UITYPE_MISSION == m_pUIMgr->Get_UIType())
			{
				m_pUIMgr->ClearUI();
				m_pUIMgr->Set_UIType(CUIMgr::UITYPE_MAIN);

				m_pUIMgr->CreateNPCMissionUI(m_pGraphicDev);
				m_pUIMgr->ChangeMouseType(CMouse::MOUSE_INVISIBLE);
				m_bMissionMapCheck = true;
			}
			else if (CUIMgr::UITYPE_READY == m_pUIMgr->Get_UIType())
			{
				m_pUIMgr->CreateMissionUI(m_pGraphicDev);
				m_bMissionMapCheck = false;
			}
		}
	}

}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale, PLAYER_ID eID)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale, eID)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free()
{
	if (m_pState != nullptr)
		delete m_pState;

	m_mapinvincibility.clear();

	CBasePlayer::Free();
}
