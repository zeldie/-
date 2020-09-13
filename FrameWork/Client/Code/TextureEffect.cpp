#include "stdafx.h"
#include "TextureEffect.h"
#include "OrbCore.h"
CTextureEffect::CTextureEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseEffect(pGraphicDev),
	m_pBufferCom(nullptr),
	m_eTextureEffect(TEXTURE_END),
	m_fTS_Alpha(1.f),
	m_fTS_Scale(1.f),
	m_iTextureNum(0),
	m_pParentTransformCom(nullptr),
	m_fRotation(0.f),
	m_dCountdown(0),
	m_bCreateEffect(false)
{
	ZeroMemory(m_vDir, sizeof(_vec3));
	ZeroMemory(m_vUVAnimation_UV_Sum, sizeof(_vec2));
}

CTextureEffect::~CTextureEffect()
{
}

HRESULT CTextureEffect::Ready_GameObject(TEXTUREEFFECT eEffectID, _vec3* pPos, _vec3* pAngle, BASE_INFO* tBaseInfo, CONTROLTYPE eControlType)
{
	if (FAILED(Clone_Component(eEffectID)))
		return E_FAIL;
	Set_Info(eEffectID, pPos, pAngle, tBaseInfo, eControlType);

	if (TEXTURE_TS_SMOKE0 == eEffectID) //희정
	{
		m_fTS_Alpha = 0.7f;
		m_fTS_Scale = 300.f;
	}
	else if (TEXTURE_TS_SMOKE1 == eEffectID)
	{
		m_fTS_Alpha = 0.8f;
		m_fTS_Scale = 200.f;
		m_iTextureNum = 1;
	}
	m_eControlType = eControlType;
	return S_OK;
}

_int CTextureEffect::Update_GameObject(const _double & dTimeDelta)
{
	////cout << (*m_pTransformCom->Get_Info(Engine::INFO_POS)).x << " , " << (*m_pTransformCom->Get_Info(Engine::INFO_POS)).y << " , " << (*m_pTransformCom->Get_Info(Engine::INFO_POS)).z << endl;
	//
	if (m_pParentTransformCom != nullptr)
	{
		m_pTransformCom->Set_Pos(&m_pParentTransformCom->Get_Info_byWorld(Engine::INFO_POS));
	}
	if (m_bLoop)
	{
		if (m_dFrame > m_dMaxFrame)
			m_dFrame = 0;
		if ((m_dbTime += dTimeDelta) > m_tEffectInfo.fDeadTime)
		{
			////cout << " 루프죽음 " << endl;
			DeadEvent();
			CBaseEffect::Delete_Collider();
			m_pEffectMgr->Return_TexturePool(this);
			ZeroMemory(&m_tBaseInfo,sizeof(BASE_INFO));
			return Engine::OBJ_POOL;
		}
		m_dFrame += dTimeDelta * m_fFrameSpeed;
	}
	else
	{
		if ((m_dFrame += dTimeDelta * m_fFrameSpeed) > m_dMaxFrame)
		{
			////cout << " 죽음 " << endl;
			DeadEvent();
			CBaseEffect::Delete_Collider();
			m_pEffectMgr->Return_TexturePool(this);
			ZeroMemory(&m_tBaseInfo, sizeof(BASE_INFO));
			return Engine::OBJ_POOL;
		}
	}

	/*m_vDir = m_pTransformCom->m_vInfo[Engine::INFO_LOOK];
	m_pTransformCom->Set_Info(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + (m_vDir*dTimeDelta * m_tEffectInfo.fSpeed)), Engine::INFO_POS);
	m_pTransformCom->Set_Angle(&(m_pTransformCom->m_vAngle + (m_tEffectInfo.vMoveRot*dTimeDelta)));
	m_pTransformCom->Set_Scale(&(m_pTransformCom->m_vScale + (m_tEffectInfo.vMoveScale*dTimeDelta)));*/

	//if (m_tBaseInfo.eObjectID == OBJECT_PLAYER)
	//{
	//	if (m_vecCollSphere[0].size() >= 1)
	//		m_pCollisionMgr->Add_Collision(COLLLIST::COLL_PLAYER, this);
	//}
	//else if (m_tBaseInfo.eObjectID == OBJECT_MONSTER)
	//{
	//	if (m_vecCollSphere[0].size() >= 1)
	//		m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER, this);
	//}

	
	/*if (m_tEffectInfo.iAlpha == 0)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	}
	else if (m_tEffectInfo.iAlpha == 1)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	}*/
	Engine::CGameObject::Compute_ViewZ(m_pTransformCom->Get_Info(Engine::INFO_POS));
	
	CBaseEffect::Update_GameObject(dTimeDelta);
	Transform(dTimeDelta);
	if (m_tTextureInfo.bIsBillboard)
		BillBoard();

	return Engine::NO_EVENT;
}

_int CTextureEffect::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseEffect::LateUpdate_GameObject(dTimeDelta);

	if (m_vecCollSphere[0].size() >= 1)
	{
		if (m_eControlType == CTRL_PLAYER || m_eControlType == CTRL_AI_ALLIANCE)
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_PLAYER_EFFECT, this);
		else
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER_EFFECT, this);
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_HDR_ALPHA, this);
	return Engine::NO_EVENT;
}

void CTextureEffect::Render_Geometry(const _double & dTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;

	_float	fU = ((_uint)m_dFrame % (_uint)(m_tTextureInfo.vAnimationU.x)) / (m_tTextureInfo.vAnimationU.x);
	_float	fV = ((_uint)m_dFrame / (_uint)(m_tTextureInfo.vAnimationU.x)) / (m_tTextureInfo.vAnimationV.x);
	m_vUVAnimation_UV_Sum.x += _float(m_tTextureInfo.vAnimationU.y * dTimeDelta);
	m_vUVAnimation_UV_Sum.y += _float(m_tTextureInfo.vAnimationV.y * dTimeDelta);
	pEffect->SetVector("g_vUV_U", &_vec4(fU, 1 / m_tTextureInfo.vAnimationU.x, m_vUVAnimation_UV_Sum.x, 0.f));
	pEffect->SetVector("g_vUV_V", &_vec4(fV, 1 / m_tTextureInfo.vAnimationV.x, m_vUVAnimation_UV_Sum.y, 0.f));
	//pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 1.f, 1.f));
	switch (m_eTextureEffect)
	{
	case TEXTURE_DUSTRUNSTOP:
		break;
	case TEXTURE_FIRE:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 2);
		break;
	case TEXTURE_FAN_TRIANGLE:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 5);
		break;
	case TEXTURE_FAN_HALF:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 5);
		break;
	case TEXTURE_FAN_BIG:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 5);
		break;
	case TEXTURE_SPHERE_RED:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 5);
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 0.94f, 0.8f, 1.f));
		break;
	case TEXTURE_SPHERE_RED2:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 5);
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 0.9f, 0.8f, 1.f));
		break;
	case TEXTURE_RUN_DUST:
		pEffect->SetVector("g_vRGBA", &_vec4(0.5f, 0.45f, 0.4f, 1.f));
		break;
	case TEXTURE_READY_ULT3:
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 0.3f, 0.3f, 1.f));
		break;
	case TEXTURE_HIT_1:
		pEffect->SetVector("g_vRGBA", &_vec4(0.95f, 0.55f, 0.f, 1.f));
		break;
	case TEXTURE_HIT_2:
		pEffect->SetVector("g_vRGBA", &_vec4(0.95f, 0.55f, 0.f, 1.f));
		break;
	case TEXTURE_HIT_3:
		pEffect->SetVector("g_vRGBA", &_vec4(0.95f, 0.55f, 0.f, 1.f));
		break;
	case TEXTURE_HIT_4:
		pEffect->SetVector("g_vRGBA", &_vec4(0.95f, 0.55f, 0.f, 1.f));
		break;
	case TEXTURE_HIT_5:
		pEffect->SetVector("g_vRGBA", &_vec4(0.95f, 0.55f, 0.f, 1.f));
		break;
	case TEXTURE_HIT_6:
		pEffect->SetVector("g_vRGBA", &_vec4(0.95f, 0.55f, 0.f, 1.f));
		break;
	case TEXTURE_SPHERE_GOLD:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 5);
		break;
	case TEXTURE_SPHERE_GOLD2:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 5);
		break;
	case TEXTURE_RAINARROWHIT:
		pEffect->SetVector("g_vRGBA", &_vec4(0.f, 1.f, 0.f, 1.f));
		break;
	case TEXTURE_ORB1:
		pEffect->SetVector("g_vRGBA", &_vec4(0.f, 1.f, 0.f, 1.f));
		break;
	case TEXTURE_ORB2:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 15);
		break;
	case TEXTURE_ORB3:
		pEffect->SetVector("g_vRGBA", &_vec4(0.48f, 0.8f, 0.69f, 1.f));
		break;
	case TEXTURE_ORB_ULT_SWIRL:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 23);
		break;
	case TEXTURE_TS_SMOKE0:
		pEffect->SetVector("g_vRGBA", &_vec4(m_fTS_Alpha, m_fTS_Alpha, m_fTS_Alpha, 0.8f));
		break;
	case TEXTURE_TS_SMOKE1:
		pEffect->SetVector("g_vRGBA", &_vec4(m_fTS_Alpha, m_fTS_Alpha, m_fTS_Alpha, 0.8f));
		break;
	case TEXTURE_WEAPONCHANGE_DS:
		pEffect->SetVector("g_vRGBA", &_vec4(0.52f, 0.31f, 1.f, 0.8f));
		break;
	case TEXTURE_WEAPONCHANGE_TS:
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 0.f, 0.8f));
		break;
	case TEXTURE_WEAPONCHANGE_ORB:
		pEffect->SetVector("g_vRGBA", &_vec4(0.039f, 1.f, 0.67f, 0.8f));
		break;
	case TEXTURE_WEAPONCHANGE_LB:
		pEffect->SetVector("g_vRGBA", &_vec4(0.30f, 1.f, 0.15f, 0.8f));
		break;
	case TEXTURE_BLINK:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 15);
		break;
	case TEXTURE_ORB_FLARE:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 15);
		break;
	case TEXTURE_ORB_FLARE2:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 23);
		break;
	case TEXTURE_ORB_FLARE3:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 23);
		break;
	case TEXTURE_BLINK_SINGLE:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 23);
		break;
	case TEXTURE_ORB_FLARE4:
		pEffect->SetVector("g_vRGBA", &_vec4(m_fTS_Alpha, m_fTS_Alpha, m_fTS_Alpha, 0.8f));
		break;
	case TEXTURE_ORB_ULT_GROUND:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 27);
		break;
	case TEXTURE_ORB_FLARE5:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 23);
		break;
	case TEXTURE_ORB_HIT_GROUND:
		pEffect->SetVector("g_vRGBA", &_vec4(m_fTS_Alpha, m_fTS_Alpha, m_fTS_Alpha, 0.8f));
		break;
	case TEXTURE_ORB_E_START:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 28);
		break;
	case TEXTURE_ORB_DUST:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 22);
		pEffect->SetVector("g_fDiminishColor", &_vec4(m_fTS_Alpha, m_fTS_Alpha, m_fTS_Alpha, 1.f));
		break;
	case TEXTURE_ORBBIG1:
		pEffect->SetVector("g_vRGBA", &_vec4(0.f, 1.f, 0.f, 1.f));
		break;
	case TEXTURE_ORBBIG2:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 15);
		break;
	case TEXTURE_ORBBIG3:
		pEffect->SetVector("g_vRGBA", &_vec4(0.48f, 0.8f, 0.69f, 1.f));
		break;
	case TEXTURE_ORB_DUST_LARGE_YELLOW:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 22);
		pEffect->SetVector("g_fDiminishColor", &_vec4(m_fTS_Alpha, m_fTS_Alpha, m_fTS_Alpha, 1.f));
		break;
	case TEXTURE_ORB_RING:
		pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 1.f, 1.f));
		break;
	case TEXTURE_ORBULT1:
		pEffect->SetVector("g_vRGBA", &_vec4(0.f, 1.f, 0.f, 1.f));
		break;
	case TEXTURE_ORBULT2:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 15);
		break;
	case TEXTURE_ORBULT3:
		pEffect->SetVector("g_vRGBA", &_vec4(0.48f, 0.8f, 0.69f, 1.f));
		break;
	case TEXTURE_ORB_CREATOR1:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 22);
		break;
	case TEXTURE_ORB_CREATOR2:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 23);
		break;
	case TEXTURE_END:
		break;
	default:
		break; 
	}

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(m_iPass);
	m_pBufferCom->Render_Buffer();
	pEffect->EndPass();
	pEffect->End();
	pEffect->SetVector("g_vUV_U", &_vec4(0.f, 1.f, 0.f, 0.f));
	pEffect->SetVector("g_vUV_V", &_vec4(0.f, 1.f, 0.f, 0.f));
	pEffect->SetTexture("g_MaskingTexture", nullptr);
	pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetFloat("g_fDiminishColor", 1.f);

	Engine::Safe_Release(pEffect);

	m_pGraphicDev->SetTexture(0, nullptr);

	CBaseObject::Render_Geometry(dTimeDelta);
}

void CTextureEffect::Set_Info(TEXTUREEFFECT eEffectID, _vec3* pPos, _vec3* pAngle, BASE_INFO* tBaseInfo, CONTROLTYPE eControlType)
{
	ResetTextureInfo();
	ResetEffectInfo();

	m_pTransformCom->Set_Pos(pPos);
	m_pTransformCom->Set_Angle(pAngle);
	m_eTextureEffect = eEffectID;
	this->Clone_TextureCom(eEffectID);
	m_dFrame = 0.;
	m_dbTime = 0.;
	if (tBaseInfo != nullptr)
		m_tBaseInfo = *tBaseInfo;
	m_pParentTransformCom = nullptr;
	m_eControlType = eControlType;
	//텍스쳐를 추가해야할때마다 아래 스위치에서 값들 추가해줘야함
	//추가해줘야할것들
	//Scale, AnimationUV, Billboard -> 얘내들은 m_tTextureInfo 안에 있음, 참고로 BillBoard는 기본이 true니 안쓸때만 false하면 됨
	//Loop(루프 돌건지), m_fFrameSpeed, m_iPass -> 얘내들은 BaseEffect에 있음, 그냥 변수로 초기화
	// 데드타임은 m_tEffectInfo.fDeadTime  이곳에 설정
	switch (eEffectID)
	{
	case TEXTURE_DUSTRUNSTOP:
		break;
	case TEXTURE_FIRE:
		break;
	case TEXTURE_FAN_TRIANGLE:
	{
		m_tEffectInfo.vRot.x = 90.f;
		//m_pTransformCom->Set_Scale(1000.f, 1000.f, 1000.f);
		m_pTransformCom->Set_Scale(3500.f, 3500.f, 3500.f);

		//
		//m_pTransformCom->Set_Scale(&_vec3(3.5f, 3.5f, 3.5f));
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_tTextureInfo.bIsBillboard = false;
		m_tEffectInfo.fDeadTime = 1.9f;
		m_fFrameSpeed = 1.f;
		m_iPass = 9;
		m_bLoop = true;
		//

		break;
	}
	case TEXTURE_FAN_HALF:
	{
		m_tEffectInfo.vRot.x = 90.f;
		//m_pTransformCom->Set_Scale(1000.f, 1000.f, 1000.f);
		m_pTransformCom->Set_Scale(2000.f, 2000.f, 2000.f);

		//m_pTransformCom->Set_Scale(&_vec3(1.5f, 1.5f, 1.5f));
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_tTextureInfo.bIsBillboard = false;
		m_tEffectInfo.fDeadTime = 0.9f;
		m_fFrameSpeed = 1.f;
		m_iPass = 9;
		m_bLoop = true;
		break;
	}
	case TEXTURE_FAN_BIG:
	{
		m_tEffectInfo.vRot.x = 90.f;
		//m_pTransformCom->Set_Scale(1000.f, 1000.f, 1000.f);
		m_pTransformCom->Set_Scale(2500.f, 2500.f, 2500.f);

		//m_pTransformCom->Set_Scale(&_vec3(2.5f, 2.5f, 2.5f));
		m_tEffectInfo.fDeadTime = 3.f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_tTextureInfo.bIsBillboard = false;
		m_fFrameSpeed = 1.f;
		m_iPass = 9;
		m_bLoop = true;

		break;
	}
	case TEXTURE_SPHERE_RED:
	{
		m_tEffectInfo.vRot.x = 90.f;
		//m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
		m_pTransformCom->Set_Scale(200.f, 200.f, 200.f);

		//m_pTransformCom->Set_Scale(&_vec3(2.f, 2.f, 2.f));
		m_tEffectInfo.fDeadTime = 1.9f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_tTextureInfo.bIsBillboard = false;
		m_fFrameSpeed = 1.f;
		m_iPass = 15;
		m_bLoop = true;
		break;
	}
	case TEXTURE_SPHERE_RED2:
	{
		m_tEffectInfo.vRot.x = 90.f;
		//m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
		m_pTransformCom->Set_Scale(400.f, 400.f, 400.f);

		//m_pTransformCom->Set_Scale(&_vec3(4.f, 4.f, 4.f));
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_tTextureInfo.bIsBillboard = false;
		m_fFrameSpeed = 1.f;
		m_iPass = 15;
		m_bLoop = true;
		break;
	}
	case TEXTURE_READY_ULT:
	{
		m_pTransformCom->Set_Scale(&_vec3(10.f, 10.f, 10.f));
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 24.f;
		m_iPass = 4;
		m_bLoop = false;
		break;
	}
	case TEXTURE_RUN_DUST:
	{
		m_pTransformCom->Set_Scale(&_vec3(30.f, 30.f, 30.f));
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 24.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_RUNGAME_TELEPORT:
	{
		m_pTransformCom->Set_Scale(&_vec3(80.f, 80.f, 1.f));
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(3.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(3.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 24.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_TRANS_APOSTLE:
	{
		m_pTransformCom->Set_Scale(&_vec3(500.f, 500.f, 1.f));
		m_tEffectInfo.fDeadTime = 3.f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 16.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_RUNGAME_ITEM_LOOT:
	{
		m_pTransformCom->Set_Scale(&_vec3(40.f, 40.f, 1.f));
		m_tEffectInfo.fDeadTime = 0.5f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 32.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_RUNGAME_HIT:
	{
		m_pTransformCom->Set_Scale(&_vec3(80.f, 80.f, 1.f));
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(3.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(3.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 18.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_RUNGAME_BIG_EFFECT:
	{
		m_pTransformCom->Set_Scale(&_vec3(200.f, 200.f, 1.f));
		m_tEffectInfo.fDeadTime = 5.f;
		m_tTextureInfo.vAnimationU = _vec2(3.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(3.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 18.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_READY_ULT2:
	{
		m_pTransformCom->Set_Scale(&_vec3(10.f, 10.f, 10.f));
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(2.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(2.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 8.f;
		m_iPass = 4;
		m_bLoop = false;
		break;
	}
	case TEXTURE_READY_ULT3:
	{
		m_pTransformCom->Set_Scale(&_vec3(70.f, 70.f, 70.f));
		m_tEffectInfo.fDeadTime = 5.f;
		m_tTextureInfo.vAnimationU = _vec2(8.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(8.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 32.f;
		m_iPass = 14;
		m_bLoop = true;
		break;
	}
	case TEXTURE_HIT_1:
	{
		m_pTransformCom->Set_Scale(&_vec3(70.f, 70.f, 70.f));
		m_tEffectInfo.fDeadTime = 5.f;
		m_tTextureInfo.vAnimationU = _vec2(3.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(3.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 32.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_HIT_2:
	{
		m_pTransformCom->Set_Scale(&_vec3(70.f, 70.f, 70.f));
		m_tEffectInfo.fDeadTime = 5.f;
		m_tTextureInfo.vAnimationU = _vec2(3.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(3.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 18.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_HIT_3:
	{
		m_pTransformCom->Set_Scale(&_vec3(70.f, 70.f, 70.f));
		m_tEffectInfo.fDeadTime = 5.f;
		m_tTextureInfo.vAnimationU = _vec2(2.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(2.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 8.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_HIT_4:
	{
		m_pTransformCom->Set_Scale(&_vec3(70.f, 70.f, 70.f));
		m_tEffectInfo.fDeadTime = 5.f;
		m_tTextureInfo.vAnimationU = _vec2(3.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(3.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 18.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_HIT_5:
	{
		m_pTransformCom->Set_Scale(&_vec3(70.f, 70.f, 70.f));
		m_tEffectInfo.fDeadTime = 5.f;
		m_tTextureInfo.vAnimationU = _vec2(3.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(3.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 18.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_HIT_6:
	{
		m_pTransformCom->Set_Scale(&_vec3(70.f, 70.f, 70.f));
		m_tEffectInfo.fDeadTime = 5.f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(2.f, 0.f);
		m_tTextureInfo.bIsBillboard = true;
		m_fFrameSpeed = 16.f;
		m_iPass = 14;
		m_bLoop = false;
		break;
	}
	case TEXTURE_SPHERE_GOLD:
	{
		m_tEffectInfo.vRot.x = 90.f;
		//m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
		m_pTransformCom->Set_Scale(200.f, 200.f, 200.f);

		//m_pTransformCom->Set_Scale(&_vec3(2.f, 2.f, 2.f));
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_tTextureInfo.bIsBillboard = false;
		m_fFrameSpeed = 1.f;
		m_iPass = 9;
		m_bLoop = true;
		break;
	}
	case TEXTURE_SPHERE_GOLD2:
	{
		m_tEffectInfo.vRot.x = 90.f;
		//m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
		m_pTransformCom->Set_Scale(400.f, 400.f, 400.f);

		//m_pTransformCom->Set_Scale(&_vec3(4.f, 4.f, 4.f));
		m_tEffectInfo.fDeadTime = 0.5f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_tTextureInfo.bIsBillboard = false;
		m_fFrameSpeed = 1.f;
		m_iPass = 9;
		m_bLoop = true;
		break;
	}
	case TEXTURE_RAINARROWHIT:
	{
		m_pTransformCom->Set_Scale(70.f, 70.f, 70.f);
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(3.f, 0.f);
		m_bLoop = false;
		m_fFrameSpeed = 24.f;
		m_iPass = 14;
		m_tTextureInfo.bIsBillboard = true;
		break;
	}
	case TEXTURE_ORB1:
	{
		//Scale, AnimationUV, Billboard -> 얘내들은 m_tTextureInfo 안에 있음, 참고로 BillBoard는 기본이 true니 안쓸때만 false하면 됨
		//Loop(루프 돌건지), m_fFrameSpeed, m_iPass -> 얘내들은 BaseEffect에 있음, 그냥 변수로 초기화
		m_pTransformCom->Set_Scale(50.f, 50.f, 50.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 16;
		break;
	}
	case TEXTURE_ORB2:
	{
		m_pTransformCom->Set_Scale(50.f, 50.f, 50.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_bLoop = true;
		m_fFrameSpeed = 24.f;
		m_iPass = 9;
		break;
	}
	case TEXTURE_ORB3:
	{
		m_pTransformCom->Set_Scale(50.f, 50.f, 50.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 14;
		break;
	}
	case TEXTURE_ORB_ULT_SWIRL:
	{
		m_pTransformCom->Set_Scale(300.f, 300.f, 300.f);
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_bLoop = false;
		m_fFrameSpeed = 24.f;
		m_iPass = 9;
		break;
	}
	case TEXTURE_TS_SMOKE0:
	{
		m_fTS_Scale = 300.f;
		m_pTransformCom->Set_Scale(m_fTS_Scale, m_fTS_Scale, m_fTS_Scale);
		m_tEffectInfo.fDeadTime = 5.f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 18;
		m_fTS_Alpha = 0.8f;
		break;
	}
	case TEXTURE_TS_SMOKE1:
	{
		m_fTS_Scale = 200.f;
		m_pTransformCom->Set_Scale(m_fTS_Scale, m_fTS_Scale, m_fTS_Scale);
		m_tEffectInfo.fDeadTime = 4.f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 18;
		m_fTS_Alpha = 0.7f;
		m_iTextureNum = 1;
		break;
	}
	case TEXTURE_WEAPONCHANGE_DS:
	{
		m_fTS_Scale = 100.f;
		m_pTransformCom->Set_Scale(m_fTS_Scale, m_fTS_Scale, m_fTS_Scale);
		m_tEffectInfo.fDeadTime = 0.3f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 23;
		break;
	}
	case TEXTURE_WEAPONCHANGE_TS:
	{
		m_fTS_Scale = 100.f;
		m_pTransformCom->Set_Scale(m_fTS_Scale , m_fTS_Scale, m_fTS_Scale);
		m_tEffectInfo.fDeadTime = 0.3f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 23;
		break;
	}
	case TEXTURE_WEAPONCHANGE_ORB:
	{
		m_fTS_Scale = 100.f;
		m_pTransformCom->Set_Scale(m_fTS_Scale, m_fTS_Scale, m_fTS_Scale);
		m_tEffectInfo.fDeadTime = 0.3f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 23;
		break;
	}
	case TEXTURE_WEAPONCHANGE_LB:
	{
		m_fTS_Scale = 100.f;
		m_pTransformCom->Set_Scale(m_fTS_Scale, m_fTS_Scale, m_fTS_Scale);
		m_tEffectInfo.fDeadTime = 0.3f;
		m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 23;
		break;
	}
	case TEXTURE_FLARE_BLINK:
	{
		m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(2.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_bLoop = false;
		m_fFrameSpeed = 8.f;
		m_iPass = 4;
		break;
	}
	case TEXTURE_BLINK:
	{
		m_pTransformCom->Set_Scale(50.f, 50.f, 50.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_bLoop = false;
		m_fFrameSpeed = 20.f;
		m_iPass = 9;
		break;
	}
	case TEXTURE_ORB_FLARE:
	{
		m_pTransformCom->Set_Scale(300.f, 300.f, 300.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_bLoop = false;
		m_fFrameSpeed = 30.f;
		m_iPass = 9;
		break;
	}
	case TEXTURE_ORB_FLARE2:
	{
		m_pTransformCom->Set_Scale(300.f, 300.f, 300.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_bLoop = false;
		m_fFrameSpeed = 30.f;
		m_iPass = 9;
		// 충돌구체 붙이기
		Engine::CComponent*		pFirstCol = nullptr;
		Engine::CComponent*		pAttCol = nullptr;

		pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 0.4);
		pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 0.4);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));
		BASE_INFO tBase;
		if (eControlType == CTRL_PLAYER)
			tBase.eObjectID = OBJECT_PLAYER;
		else if (eControlType == CTRL_AI_ALLIANCE)
			tBase.eObjectID = OBJECT_ALLIANCE;
		else if (eControlType == CTRL_AI_ENERMY_1)
			tBase.eObjectID = OBJECT_ENEMY_1;
		else if (eControlType == CTRL_AI_ENERMY_2)
			tBase.eObjectID = OBJECT_ENEMY_2;
		tBase.eCollisionID = COLLISIONID::COLLISION_AIR_UP;
		tBase.eEffectID = EFFECTID::EFFECT_ORB_FLARE_BOMB;
		tBase.fEffectHitTime = 2.f;
		tBase.iAtt = 1000;
		tBase.iCritical = 4;
		m_tBaseInfo = tBase;
		break;
	}
	case TEXTURE_ORB_FLARE3:
	{
		m_pTransformCom->Set_Scale(300.f, 300.f, 300.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(3.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(3.f, 0.f);
		m_bLoop = false;
		m_fFrameSpeed = 20.f;
		m_iPass = 9;
		break;
	}
	case TEXTURE_BLINK_SINGLE:
	{
		m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
		m_tEffectInfo.fDeadTime = 3.f;
		m_bLoop = true;
		m_iPass = 9;
		break;
	}
	case TEXTURE_ORB_FLARE4:
	{
		m_pTransformCom->Set_Scale(130.f, 130.f, 130.f);
		m_tEffectInfo.fDeadTime = 3.f;
		m_bLoop = true;
		m_fTS_Alpha = 0.7f;
		m_iPass = 20;
		break;
	}
	case TEXTURE_ORB_ULT_GROUND:
	{
		m_tTextureInfo.bIsBillboard = false;
		m_pTransformCom->Set_Scale(500.f, 500.f, 500.f);
		m_pTransformCom->Rotation(Engine::ROT_X, 90.f);
		m_tEffectInfo.fDeadTime = 5.f;
		m_bLoop = true;
		m_iPass = 16;
		break;
	}
	case TEXTURE_ORB_FLARE5:
	{
		m_pTransformCom->Set_Scale(200.f, 200.f, 200.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_bLoop = false;
		m_fFrameSpeed = 24.f;
		m_iPass = 9;
		break;
	}
	case TEXTURE_ORB_HIT_GROUND:
	{
		m_tTextureInfo.bIsBillboard = false;
		m_pTransformCom->Rotation(Engine::ROT_X, 90.f);
		m_pTransformCom->Set_Scale(200.f, 200.f, 200.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_bLoop = true;
		m_fTS_Alpha = 0.7f;
		m_iPass = 20;
		break;
	}
	case TEXTURE_ORB_E_START:
	{
		m_pTransformCom->Set_Scale(500.f, 500.f, 500.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_bLoop = true;
		m_iPass = 19;
		break;
	}
	case TEXTURE_ORB_DUST:
	{
		m_tTextureInfo.bIsBillboard = false;
		m_pTransformCom->Set_Scale(500.f, 500.f, 500.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_bLoop = true;
		m_iPass = 17;
		m_fTS_Alpha = 0.85f;
		break;
	}
	case TEXTURE_ORB_R_BULLET:
	{
		m_pTransformCom->Set_Scale(500.f, 500.f, 500.f);
		m_tEffectInfo.fDeadTime = 10.f;
		m_bLoop = true;
		m_iPass = 21;
		break;
	}
	case TEXTURE_ORB_R_FLARE:
	{
		//m_pTransformCom->Rotation(Engine::ROT_X, 90.f);
		//m_tTextureInfo.bIsBillboard = false;
		m_pTransformCom->Set_Scale(1000.f, 1000.f, 1000.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_bLoop = true;
		m_iPass = 22;
		break;
	}
	case TEXTURE_ORBBIG1:
	{
		m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
		m_tEffectInfo.fDeadTime = 5.f;
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 16;

		// 충돌구체 붙이기
		Engine::CComponent*		pFirstCol = nullptr;
		Engine::CComponent*		pAttCol = nullptr;

		pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 1);
		pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 1);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));
		BASE_INFO tBase;
		if (eControlType == CTRL_PLAYER)
			tBase.eObjectID = OBJECT_PLAYER;
		else if (eControlType == CTRL_AI_ALLIANCE)
			tBase.eObjectID = OBJECT_ALLIANCE;
		else if (eControlType == CTRL_AI_ENERMY_1)
			tBase.eObjectID = OBJECT_ENEMY_1;
		else if (eControlType == CTRL_AI_ENERMY_2)
			tBase.eObjectID = OBJECT_ENEMY_2;
		tBase.eCollisionID = COLLISIONID::COLLISION_HIT;
		tBase.eEffectID = EFFECTID::EFFECT_ORB_R_BIG;
		tBase.fEffectHitTime = 1.f;
		tBase.iAtt = 1000;
		tBase.iCritical = 4;
		m_tBaseInfo = tBase;
		break;
	}
	case TEXTURE_ORBBIG2:
	{
		m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
		m_tEffectInfo.fDeadTime = 5.f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_bLoop = true;
		m_fFrameSpeed = 24.f;
		m_iPass = 9;
		break;
	}
	case TEXTURE_ORBBIG3:
	{
		m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
		m_tEffectInfo.fDeadTime = 5.f;
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 14;
		break;
	}
	case TEXTURE_ORB_DUST_LARGE_YELLOW:
	{
		m_tTextureInfo.bIsBillboard = false;
		m_pTransformCom->Set_Scale(1000.f, 1000.f, 1000.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_bLoop = true;
		m_iPass = 17;
		m_fTS_Alpha = 0.5f;
		break;
	}
	case TEXTURE_ORB_RING:
	{
		m_tTextureInfo.bIsBillboard = false;
		m_pTransformCom->Set_Scale(500.f, 500.f, 500.f);
		m_tEffectInfo.fDeadTime = 1.f;
		m_bLoop = true;
		m_iPass = 20;
		break;
	}
	case TEXTURE_ORBULT1:
	{
		m_pTransformCom->Set_Scale(300.f, 300.f, 300.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 16;

		// 충돌구체 붙이기
		Engine::CComponent*		pFirstCol = nullptr;
		Engine::CComponent*		pAttCol = nullptr;

		pFirstCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 0.4);
		pAttCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::MESHTYPE::TYPE_STATIC, m_pTransformCom, 0.4);

		m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pFirstCol));
		m_vecCollSphere[COLLSPHERETYPE::SPHERE_ATT].emplace_back(dynamic_cast<Engine::CSphereCollider*>(pAttCol));
		BASE_INFO tBase;
		if (eControlType == CTRL_PLAYER)
			tBase.eObjectID = OBJECT_PLAYER;
		else if (eControlType == CTRL_AI_ALLIANCE)
			tBase.eObjectID = OBJECT_ALLIANCE;
		else if (eControlType == CTRL_AI_ENERMY_1)
			tBase.eObjectID = OBJECT_ENEMY_1;
		else if (eControlType == CTRL_AI_ENERMY_2)
			tBase.eObjectID = OBJECT_ENEMY_2;
		tBase.eCollisionID = COLLISIONID::COLLISION_HIT;
		tBase.eEffectID = EFFECTID::EFFECT_ORB_ULT_BIG;
		tBase.fEffectHitTime = 1.f;
		tBase.iAtt = 1000;
		tBase.iCritical = 4;
		m_tBaseInfo = tBase;
		break;
	}
	case TEXTURE_ORBULT2:
	{
		m_pTransformCom->Set_Scale(300.f, 300.f, 300.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_tTextureInfo.vAnimationU = _vec2(4.f, 0.f);
		m_tTextureInfo.vAnimationV = _vec2(4.f, 0.f);
		m_bLoop = true;
		m_fFrameSpeed = 24.f;
		m_iPass = 9;
		break;
	}
	case TEXTURE_ORBULT3:
	{
		m_pTransformCom->Set_Scale(300.f, 300.f, 300.f);
		m_tEffectInfo.fDeadTime = 2.f;
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 14;
		break;
	}
	case TEXTURE_ORB_CREATOR1:
	{
		m_tTextureInfo.bIsBillboard = false;
		m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
		m_tEffectInfo.fDeadTime = 0.5f;
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 22;
		break;
	}
	case TEXTURE_ORB_CREATOR2:
	{
		m_tTextureInfo.bIsBillboard = false;
		m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
		m_tEffectInfo.fDeadTime = 0.5f;
		m_bLoop = true;
		m_fFrameSpeed = 1.f;
		m_iPass = 22;
		break;
	}
	case TEXTURE_END:
		break;
	default:
		break;
	}
	m_dMaxFrame = m_tTextureInfo.vAnimationU.x *  m_tTextureInfo.vAnimationV.x;

	m_pTransformCom->Update_Component(0.f);
}

HRESULT CTextureEffect::Clone_TextureCom(TEXTUREEFFECT eEffectID)
{
	Safe_Release(m_pTextureCom);
	m_mapComponent[Engine::ID_STATIC].erase(Engine::TEXTURE);

	Engine::CComponent* pComponent = nullptr;
	wstring wstrNewTextureID = Find_Texture(eEffectID);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrNewTextureID));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	return S_OK;
}

HRESULT CTextureEffect::Clone_Component(TEXTUREEFFECT eEffectID)
{
	Engine::CComponent* pComponent = nullptr;
	wstring wstrTextureTag = Find_Texture(eEffectID);

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	//Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrTextureTag));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);
	return S_OK;
}

_vec3 CTextureEffect::Create_Angle()
{
	_vec3	vAngle;
	vAngle.x = ((rand() % 13) * 30.f) - 180.f;
	vAngle.y = ((rand() % 13) * 30.f) - 180.f;
	vAngle.z = ((rand() % 13) * 30.f) - 180.f;

	return vAngle;
}

HRESULT CTextureEffect::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	// 객체의 기본 텍스쳐
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture",m_iTextureNum);

	return S_OK;
}

wstring CTextureEffect::Find_Texture(TEXTUREEFFECT eEffectID)
{
	wstring wstrTexID = L"";
	switch (eEffectID)
	{
	case TEXTURE_DUSTRUNSTOP:
		wstrTexID = L"Texture_DustRunStop";
		break;
	case TEXTURE_FIRE:
		wstrTexID = L"Texture_Fire";
		break;
	case TEXTURE_FAN_TRIANGLE:
		wstrTexID = L"Texture_FanShapeTriangle";
		break;
	case TEXTURE_FAN_HALF:
		wstrTexID = L"Texture_FanShapeHalf";
		break;
	case TEXTURE_FAN_BIG:
		wstrTexID = L"Texture_FanShapeBig";
		break;
	case TEXTURE_SPHERE_RED:
		wstrTexID = L"Texture_RedSphere";
		break;
	case TEXTURE_SPHERE_RED2:
		wstrTexID = L"Texture_RedSphere";
		break;
	case TEXTURE_READY_ULT:
		wstrTexID = L"Texture_ReadyUlt";
		break;
	case TEXTURE_RUN_DUST:
		wstrTexID = L"Texture_RunDust";
		break;
	case TEXTURE_READY_ULT2:
		wstrTexID = L"Texture_ReadyUlt2";
		break;
	case TEXTURE_READY_ULT3:
		wstrTexID = L"Texture_ReadyUlt3";
		break;
	case TEXTURE_HIT_1:
		wstrTexID = L"Texture_Hit1";
		break;
	case TEXTURE_HIT_2:
		wstrTexID = L"Texture_Hit2";
		break;
	case TEXTURE_HIT_3:
		wstrTexID = L"Texture_Hit3";
		break;
	case TEXTURE_HIT_4:
		wstrTexID = L"Texture_Hit4";
		break;
	case TEXTURE_HIT_5:
		wstrTexID = L"Texture_Hit5";
		break;
	case TEXTURE_HIT_6:
		wstrTexID = L"Texture_Hit6";
		break;
	case TEXTURE_SPHERE_GOLD:
		wstrTexID = L"Texture_GoldSphere";
		break;
	case TEXTURE_SPHERE_GOLD2:
		wstrTexID = L"Texture_GoldSphere";
		break;
	case TEXTURE_RAINARROWHIT:
		wstrTexID = L"Texture_RainArrowHit";
		break;
	case TEXTURE_ORB1:
		wstrTexID = L"Texture_Orb1";
		break;
	case TEXTURE_ORB2:
		wstrTexID = L"Texture_Orb2";
		break;
	case TEXTURE_ORB3:
		wstrTexID = L"Texture_Orb3";
		break;
	case TEXTURE_ORB_ULT_SWIRL:
		wstrTexID = L"Texture_Orb_Ult_Swirl";
		break;
	case TEXTURE_TS_SMOKE0:
		wstrTexID = L"Texture_TS_Smoke";
		break;
	case TEXTURE_TS_SMOKE1:
		wstrTexID = L"Texture_TS_Smoke";
		break;
	case TEXTURE_WEAPONCHANGE_DS:
		wstrTexID = L"Texture_WeaponChange";
		break;
	case TEXTURE_WEAPONCHANGE_TS:
		wstrTexID = L"Texture_WeaponChange";
		break;
	case TEXTURE_WEAPONCHANGE_ORB:
		wstrTexID = L"Texture_WeaponChange";
		break;
	case TEXTURE_WEAPONCHANGE_LB:
		wstrTexID = L"Texture_WeaponChange";
		break;
	case TEXTURE_FLARE_BLINK:
		wstrTexID = L"Texture_FlareBlink";
		break;
	case TEXTURE_BLINK:
		wstrTexID = L"Texture_Blink";
		break;
	case TEXTURE_ORB_FLARE:
		wstrTexID = L"Texture_Orb_Flare";
		break;
	case TEXTURE_ORB_FLARE2:
		wstrTexID = L"Texture_Orb_Flare2";
		break;
	case TEXTURE_ORB_FLARE3:
		wstrTexID = L"Texture_Orb_Flare3";
		break;
	case TEXTURE_BLINK_SINGLE:
		wstrTexID = L"Texture_Blink_Single";
		break;
	case TEXTURE_ORB_FLARE4:
		wstrTexID = L"Texture_Orb_Flare4";
		break;
	case TEXTURE_ORB_ULT_GROUND:
		wstrTexID = L"Texture_Orb_Ult_Ground";
		break;
	case TEXTURE_ORB_FLARE5:
		wstrTexID = L"Texture_Orb_Flare5";
		break;
	case TEXTURE_ORB_HIT_GROUND:
		wstrTexID = L"Texture_Orb_Hit_Ground";
		break;
	case TEXTURE_ORB_E_START:
		wstrTexID = L"Texture_Orb_E_Start";
		break;
	case TEXTURE_ORB_DUST:
		wstrTexID = L"Texture_Orb_Dust";
		break;
	case TEXTURE_ORB_R_BULLET:
		wstrTexID = L"Texture_Orb_R_Bullet";
		break;
	case TEXTURE_ORB_R_FLARE:
		wstrTexID = L"Texture_Orb_R_Flare";
		break;
	case TEXTURE_ORBBIG1:
		wstrTexID = L"Texture_OrbBig1";
		break;
	case TEXTURE_ORBBIG2:
		wstrTexID = L"Texture_OrbBig2";
		break;
	case TEXTURE_ORBBIG3:
		wstrTexID = L"Texture_OrbBig3";
		break;
	case TEXTURE_ORB_DUST_LARGE_YELLOW:
		wstrTexID = L"Texture_Orb_Dust_Large_Yellow";
		break;
	case TEXTURE_ORB_RING:
		wstrTexID = L"Texture_Orb_Ring";
		break;
	case TEXTURE_ORBULT1:
		wstrTexID = L"Texture_OrbBig1";
		break;
	case TEXTURE_ORBULT2:
		wstrTexID = L"Texture_OrbBig2";
		break;
	case TEXTURE_ORBULT3:
		wstrTexID = L"Texture_OrbBig3";
		break;
	case TEXTURE_ORB_CREATOR1:
		wstrTexID = L"Texture_Orb_Creator1";
		break;
	case TEXTURE_ORB_CREATOR2:
		wstrTexID = L"Texture_Orb_Dust";
		break;
	case TEXTURE_RUNGAME_TELEPORT:
		wstrTexID = L"Texture_RunGame_Teleport";
		break;
	case TEXTURE_RUNGAME_ITEM_LOOT:
		wstrTexID = L"Texture_RunGame_ItemEffect";
		break;
	case TEXTURE_RUNGAME_HIT:
		wstrTexID = L"Texture_RunGame_Hit";
		break;
	case TEXTURE_RUNGAME_BIG_EFFECT:
		wstrTexID = L"Texture_RunGame_Big_Effect";
		break;
	case TEXTURE_TRANS_APOSTLE:
		wstrTexID = L"Texture_TransApostle";
		break;
	case TEXTURE_END:
		break;
	default:
		break;
	}
	return wstrTexID; 
}

void CTextureEffect::DeadEvent()
{
	switch (m_eTextureEffect)
	{
	case TEXTURE_DUSTRUNSTOP:
		break;
	case TEXTURE_FIRE:
		break;
	case TEXTURE_FAN_TRIANGLE:
	{
		m_pEffectMgr->Create_FanEffect(TEXTURE_FAN_TRIANGLE, &m_pTransformCom->m_vInfo[Engine::INFO_POS], &m_pTransformCom->m_vAngle, &_vec3(3.5f, 3.5f, 3.5f) , &m_tBaseInfo);
		break;
	}
	case TEXTURE_FAN_HALF:
	{
		m_pEffectMgr->Create_FanEffect(TEXTURE_FAN_HALF, &m_pTransformCom->m_vInfo[Engine::INFO_POS], &m_pTransformCom->m_vAngle, &_vec3(2.f, 2.f, 2.f), &m_tBaseInfo);
		break;
	}
	case TEXTURE_FAN_BIG:
	{
		m_pEffectMgr->Create_FanEffect(TEXTURE_FAN_BIG, &m_pTransformCom->m_vInfo[Engine::INFO_POS], &m_pTransformCom->m_vAngle, &_vec3(2.5f, 2.5f, 2.5f), &m_tBaseInfo);
		break;
	}
	case TEXTURE_SPHERE_RED:
	{
		_matrix mat = *m_pTransformCom->Get_WorldMatrix();
		mat.m[3][1] += 2000.f;
		m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_SPHERE_4, &mat, &(m_pTransformCom->m_vAngle), m_tBaseInfo);
		break;
	}
	case TEXTURE_SPHERE_RED2:
	{
		_matrix mat = *m_pTransformCom->Get_WorldMatrix();
		mat.m[3][1] += 2000.f;
		m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_SPHERE_5, &mat, &(m_pTransformCom->m_vAngle), m_tBaseInfo);
		break;
	}
	case TEXTURE_SPHERE_GOLD:
	{
		_matrix mat = *m_pTransformCom->Get_WorldMatrix();
		_vec3 vAngle = m_pTransformCom->m_vAngle;
		vAngle.x = 0.f;
		m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_LIGHT, &mat, &vAngle, m_tBaseInfo);
		m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_LIGHT_DOT, &mat, &vAngle, m_tBaseInfo);
		break;
	}
	case TEXTURE_SPHERE_GOLD2:
	{
		//_matrix mat = *m_pTransformCom->Get_WorldMatrix();
		//mat.m[3][1] += 2000.f;
		//m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_SPHERE_5, &mat, &(m_pTransformCom->m_vAngle), m_tBaseInfo);

		_matrix mat = *m_pTransformCom->Get_WorldMatrix();
		_vec3 vAngle = m_pTransformCom->m_vAngle;
		vAngle.x = 0.f;
		m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_LIGHT2, &mat, &vAngle, m_tBaseInfo);
		m_pEffectMgr->Create_Effect(Engine::EFFECTTYPE::EFFECT_APOSTLE_LIGHT2_DOT, &mat, &vAngle, m_tBaseInfo);
		break;
	}
	case TEXTURE_END:
		break;
	default:
		break;
	}
}

void CTextureEffect::Transform(const _double& dTimeDelta)
{
	if (TEXTURE_TS_SMOKE0 == m_eTextureEffect
		|| TEXTURE_TS_SMOKE1 == m_eTextureEffect)
	{
		m_fTS_Alpha -= _float(dTimeDelta*0.4);
		m_fTS_Scale += _float(dTimeDelta * 10);
		m_pTransformCom->Set_Scale(m_fTS_Scale, m_fTS_Scale, m_fTS_Scale);
	}
	else if (TEXTURE_WEAPONCHANGE_DS == m_eTextureEffect
		|| TEXTURE_WEAPONCHANGE_TS == m_eTextureEffect
		|| TEXTURE_WEAPONCHANGE_ORB == m_eTextureEffect
		|| TEXTURE_WEAPONCHANGE_LB == m_eTextureEffect)
	{
		_vec3 vTargetPos;
		vTargetPos = *dynamic_cast<Engine::CTransform*>(Engine::Get_GameObject(Engine::UI,L"KarmaOverHead")->Get_Component(Engine::TRANSFORM,Engine::ID_DYNAMIC))->Get_Info(Engine::INFO_POS);
		
		m_pTransformCom->Set_Pos(&vTargetPos);
		//m_pTransformCom->Set_Scale(m_fTS_Scale - dTimeDelta*100.f, m_fTS_Scale - dTimeDelta*100.f, m_fTS_Scale - dTimeDelta*100.f);
	}
	else if (m_eTextureEffect == TEXTURE_ORB_FLARE || m_eTextureEffect == TEXTURE_ORB_FLARE2)
	{
		_vec3 vScaling = _vec3(_float(dTimeDelta * 50), _float(dTimeDelta * 50), _float(dTimeDelta * 50));
		m_pTransformCom->Change_Scale(&vScaling);
	}
	else if (m_eTextureEffect == TEXTURE_ORB_FLARE3)
	{
		_vec3 vScaling = _vec3(_float(dTimeDelta * 400), _float(dTimeDelta * 400), _float(dTimeDelta * 400));
		m_pTransformCom->Change_Scale(&vScaling);
	}
	else if (m_eTextureEffect == TEXTURE_BLINK_SINGLE)
	{
		_vec3 vScaling = _vec3(-_float(dTimeDelta * 300), -_float(dTimeDelta * 300), -_float(dTimeDelta * 300));
		if (!m_pTransformCom->Change_Scale(&vScaling))
			m_dbTime = m_tEffectInfo.fDeadTime + 1.f; // 강제로 죽임
	}
	else if (m_eTextureEffect == TEXTURE_ORB_FLARE4)
	{
		m_fTS_Alpha -= _float(dTimeDelta);
		_vec3 vScaling = _vec3(_float(dTimeDelta * 300), _float(dTimeDelta * 300), _float(dTimeDelta * 300));
		m_pTransformCom->Change_Scale(&vScaling);
	}
	else if (m_eTextureEffect == TEXTURE_ORB_ULT_GROUND)
	{
		m_pTransformCom->Rotation(Engine::ROT_Y, 100 * dTimeDelta);
	}
	else if (m_eTextureEffect == TEXTURE_ORB_HIT_GROUND)
	{
		m_fTS_Alpha -= _float(dTimeDelta);
		_vec3 vScaling = _vec3(_float(dTimeDelta * 300), _float(dTimeDelta * 300), _float(dTimeDelta * 300));
		m_pTransformCom->Change_Scale(&vScaling);
	}
	else if (m_eTextureEffect == TEXTURE_ORB_E_START)
	{
		m_fTS_Alpha -= _float(dTimeDelta);
		_vec3 vScaling = _vec3(-_float(dTimeDelta * 100), -_float(dTimeDelta * 100), -_float(dTimeDelta * 100));
		if (!m_pTransformCom->Change_Scale(&vScaling))
			m_dbTime = m_tEffectInfo.fDeadTime + 1.f; // 강제로 죽임
		m_pTransformCom->Rotation(Engine::ROT_Y, 100 * dTimeDelta);
	}
	else if (m_eTextureEffect == TEXTURE_ORB_DUST)
	{
		m_fRotation += _float(dTimeDelta * 100);
		m_pTransformCom->RotationSelf(Engine::ROT_Z,m_fRotation);
	}
	else if (m_eTextureEffect == TEXTURE_ORB_R_BULLET)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.f, -1.f, 0.f), 100, dTimeDelta);
	}
	else if (m_eTextureEffect == TEXTURE_ORB_R_FLARE)
	{
		_vec3 vScaling = _vec3(-_float(dTimeDelta * 10000), -_float(dTimeDelta * 10000), -_float(dTimeDelta * 10000));
		if (!m_pTransformCom->Change_Scale(&vScaling))
			m_dbTime = m_tEffectInfo.fDeadTime + 1.f; // 강제로 죽임
	}
	else if (m_eTextureEffect == TEXTURE_ORB_DUST_LARGE_YELLOW)
	{
		m_fRotation += _float(dTimeDelta * 70);
		m_pTransformCom->RotationSelf(Engine::ROT_Z, m_fRotation);
	}
	else if (m_eTextureEffect == TEXTURE_ORB_CREATOR1)
	{
		m_dCountdown += dTimeDelta;
		m_fRotation += _float(dTimeDelta * 70);
		m_pTransformCom->RotationSelf(Engine::ROT_Z, m_fRotation);
		if (!m_bCreateEffect && m_dCountdown > 0.2)
		{
			_vec3 vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
			D3DXVec3Normalize(&vLook, &vLook);
			Engine::CGameObject* pGameObject = nullptr;
			pGameObject  = COrbCore::Create(m_pGraphicDev, COrbCore::ORB_NORMAL, m_pTransformCom->Get_Info(Engine::INFO_POS), m_pTransformCom->Get_Angle(), &vLook, 1,m_eControlType);
			Engine::Add_GameObject(Engine::GAMEOBJECT, L"Orb", pGameObject);
			m_bCreateEffect = true;
		}
	}
}

void CTextureEffect::ResetTextureInfo()
{
	m_tTextureInfo.fDeadTime = 1.f;
	m_tTextureInfo.bIsBillboard = true;
	m_tTextureInfo.vScale = _vec3(1.f, 1.f, 1.f);
	m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
	m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
	ZeroMemory(&m_vUVAnimation_UV_Sum,sizeof(_vec2));
}

CTextureEffect * CTextureEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREEFFECT eEffectID, _vec3* pPos, _vec3* pAngle, BASE_INFO* tBaseInfo, CONTROLTYPE eControlType)
{
	CTextureEffect* pInstance = new CTextureEffect(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eEffectID, pPos, pAngle, tBaseInfo, eControlType)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTextureEffect::Free()
{
	CBaseEffect::Free();
}
