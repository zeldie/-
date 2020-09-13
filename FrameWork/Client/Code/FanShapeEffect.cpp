#include "stdafx.h"
#include "FanShapeEffect.h"

CFanShapeEffect::CFanShapeEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseEffect(pGraphicDev)
	,m_pBufferCom(nullptr)
	, m_iPass(0)
	, m_eTextureEffect(TEXTURE_END)
	, m_dbTime(0.)
	, m_fDiminishRate(1.f)
{
	ZeroMemory(m_vUVAnimation_U, sizeof(_vec2));
	ZeroMemory(m_vUVAnimation_V, sizeof(_vec2));
	ZeroMemory(m_vUVAnimation_UV_Sum, sizeof(_vec2));
}

CFanShapeEffect::~CFanShapeEffect()
{
}

HRESULT CFanShapeEffect::Ready_GameObject()
{
	if (FAILED(Clone_Component()))
		return E_FAIL;

	return S_OK;
}

_int CFanShapeEffect::Update_GameObject(const _double & dTimeDelta)
{
	CBaseEffect::Update_GameObject(dTimeDelta);

	//프레임증가 + 최대프레임도달시 객체반환
	//if ((m_dFrame += (dTimeDelta * m_dMaxFrame * m_dLifeTime)) >= m_dMaxFrame)
	//if ((m_dFrame += dTimeDelta) >= m_tEffectInfo.fDeadTime)
	if ((m_dFrame += dTimeDelta) >=m_tEffectInfo.fDeadTime)
	{
		CBaseEffect::Delete_Collider();
		return Engine::OBJ_DEAD;
	}

	// 수정하게 되면 사용할 부분
	//m_vDir = m_pTransformCom->m_vInfo[Engine::INFO_LOOK];
	//m_pTransformCom->Set_Info(&(m_pTransformCom->m_vInfo[Engine::INFO_POS] + (m_vDir*dTimeDelta * m_tEffectInfo.fSpeed)), Engine::INFO_POS);
	//m_pTransformCom->Set_Angle(&(m_pTransformCom->m_vAngle + (m_tEffectInfo.vMoveRot*dTimeDelta)));
	//m_pTransformCom->Set_Scale(&(m_pTransformCom->m_vScale + (m_tEffectInfo.vMoveScale*dTimeDelta)));
	// 충돌구체 크기 키우는함수
	//Set_ColliderScaling(dTimeDelta);

	if (m_tBaseInfo.eObjectID == OBJECT_PLAYER)
	{
		// 플레이어스킬에 부채꼴이 생긴다면 사용할 부분
		//if (m_vecCollSphere[0].size() >= 1)
		//	m_pCollisionMgr->Add_Collision(COLLLIST::COLL_PLAYER_EFFECT, this);
	}
	else if (m_tBaseInfo.eObjectID == OBJECT_MONSTER)
	{
		if (m_vecCollSphere[0].size() >= 1)
			m_pCollisionMgr->Add_Collision(COLLLIST::COLL_MONSTER_EFFECT_FAN, this);
	}


	return Engine::NO_EVENT;
}

_int CFanShapeEffect::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseEffect::LateUpdate_GameObject(dTimeDelta);
	if (m_tEffectInfo.iAlpha == 0)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	}
	else if (m_tEffectInfo.iAlpha == 1)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	}
	return Engine::NO_EVENT;
}

void CFanShapeEffect::Render_Geometry(const _double & dTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;

	_float	fU = ((_uint)m_dFrame % (_uint)(m_vUVAnimation_U.x)) / (m_vUVAnimation_U.x);
	_float	fV = ((_uint)m_dFrame / (_uint)(m_vUVAnimation_U.x)) / (m_vUVAnimation_V.x);
	m_vUVAnimation_UV_Sum.x += m_vUVAnimation_U.y * dTimeDelta;
	m_vUVAnimation_UV_Sum.y += m_vUVAnimation_V.y * dTimeDelta;
	pEffect->SetVector("g_vUV_U", &_vec4(fU, 1 / m_vUVAnimation_U.x, m_vUVAnimation_UV_Sum.x, 0.f));
	pEffect->SetVector("g_vUV_V", &_vec4(fV, 1 / m_vUVAnimation_V.x, m_vUVAnimation_UV_Sum.y, 0.f));

	switch (m_eTextureEffect)
	{
	case TEXTURE_DUSTRUNSTOP:
		break;
	case TEXTURE_FIRE:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 2);
		break;
	case TEXTURE_FAN_TRIANGLE:
	{
		Engine::SetTexture(pEffect, "g_MaskingTexture", 16);
		m_fDiminishRate -= _float(dTimeDelta * 1.f);
		pEffect->SetFloat("g_fDiminishColor", m_fDiminishRate);
		break;
	}
	case TEXTURE_FAN_HALF:
	{
		Engine::SetTexture(pEffect, "g_MaskingTexture", 14);
		m_fDiminishRate -= _float(dTimeDelta * 1.f);
		pEffect->SetFloat("g_fDiminishColor", m_fDiminishRate);
		break;
	}
	case TEXTURE_FAN_BIG:
		Engine::SetTexture(pEffect, "g_MaskingTexture", 6);
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

	Engine::Safe_Release(pEffect);

	m_pGraphicDev->SetTexture(0, nullptr);

	CBaseObject::Render_Geometry(dTimeDelta);
}

void CFanShapeEffect::Set_Info(TEXTUREEFFECT eType, _vec3* pPos, _vec3 * pAngle,  _vec3* pScale,BASE_INFO tBaseInfo)
{
	m_pTransformCom->Set_Pos(pPos);
	_vec3 vAngle = *pAngle;
	vAngle.x = 0.f;
	vAngle.z = 0.f;
	m_pTransformCom->Set_Angle(&vAngle);
	m_tEffectInfo.iAlpha = 0;
	_vec3 vScale = { 1000.f , 1000.f, 1000.f };
	vScale.x *= pScale->x;
	vScale.y *= pScale->y;
	vScale.z *= pScale->z;
	m_pTransformCom->Set_Scale(&vScale);
	m_tEffectInfo.fDeadTime = 0.5f;
	m_bLoop = true;
	m_iPass = 9;
	m_dFrame = 0.;
	m_eTextureEffect = eType;
	m_tBaseInfo = tBaseInfo;

	_matrix	matRotateY;
	D3DXMatrixIdentity(&matRotateY);
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(pAngle->y));
	m_pTransformCom->Rotation(Engine::ROTATION::ROT_X, 90.f);
	m_tBaseInfo.vFanLook = matRotateY.m[2];

	switch (eType)
	{
	case TEXTURE_DUSTRUNSTOP:
		break;
	case TEXTURE_FIRE:
		break;
	case TEXTURE_FAN_TRIANGLE:
	{
		m_vUVAnimation_U = _vec2(1.f, 0.f);
		m_vUVAnimation_V = _vec2(1.f, 0.f);
		m_tBaseInfo.fHalfAngle = 22.5f;
		m_tEffectInfo.fDeadTime = 0.8f;
		m_iPass = 17;
		break;
	}
	case TEXTURE_FAN_HALF:
	{
		m_vUVAnimation_U = _vec2(1.f, 0.f);
		m_vUVAnimation_V = _vec2(1.f, 0.f);
		m_tBaseInfo.fHalfAngle = 90.f;
		m_iPass = 17;
		break;
	}
	case TEXTURE_FAN_BIG:
	{
		m_vUVAnimation_U = _vec2(1.f, 0.f);
		m_vUVAnimation_V = _vec2(1.f, 0.f);
		m_tBaseInfo.fHalfAngle = 135.f;
		break;
	}
	case TEXTURE_END:
		break;
	default:
		break;
	}

	Clone_TextureCom();

	Engine::CSphereCollider* pCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::TYPE_STATIC, m_pTransformCom, 0.5f);
	m_vecCollSphere[COLLSPHERETYPE::SPHERE_FIRST].push_back(pCol);
}

HRESULT CFanShapeEffect::Clone_Component()
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

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);
	return S_OK;
}

HRESULT CFanShapeEffect::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture");
	// 객체의 기본 텍스쳐
	//m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", (_ulong)0);
	//m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_iTextureNum);
	//m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", stoi(m_tEffectInfo.wstrName));

	//_float	fU = ((_ulong)m_dFrame % 3) *0.33f;
	//_float	fV = ((_ulong)m_dFrame / 4) *0.25f;

	//pEffect->SetFloat("g_fU", fU);
	//pEffect->SetFloat("g_fV", fV);
	//pEffect->SetFloat("g_fUNum", 0.33f);
	//pEffect->SetFloat("g_fVNum", 0.25f);

	return S_OK;
}

wstring CFanShapeEffect::Find_Texture(TEXTUREEFFECT eEffectID)
{
	wstring wstrTexID = L"";
	switch (eEffectID)
	{
	case TEXTURE_FAN_TRIANGLE:
		wstrTexID = L"Texture_FanShapeTriangle";
		break;
	case TEXTURE_FAN_HALF:
		wstrTexID = L"Texture_FanShapeHalf";
		break;
	case TEXTURE_FAN_BIG:
		wstrTexID = L"Texture_Alpha";
		break;
	case TEXTURE_END:
		break;
	default:
		break;
	}
	return wstrTexID;
}

HRESULT CFanShapeEffect::Clone_TextureCom()
{
	Safe_Release(m_pTextureCom);
	m_mapComponent[Engine::ID_STATIC].erase(Engine::TEXTURE);

	Engine::CComponent* pComponent = nullptr;
	wstring wstrNewTextureID = Find_Texture(m_eTextureEffect);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, wstrNewTextureID));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	return S_OK;
}

CFanShapeEffect * CFanShapeEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFanShapeEffect* pInstance = new CFanShapeEffect(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFanShapeEffect::Free()
{
	CBaseEffect::Free();
}
