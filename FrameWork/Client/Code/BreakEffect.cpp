#include "stdafx.h"
#include "BreakEffect.h"

CBreakEffect::CBreakEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBaseEffect(pGraphicDev),
	m_pBufferCom(nullptr),
	m_pParentTransformCom(nullptr),
	m_dDissolveCountDown(0),
	m_bDissolveStart(false)
{
	ZeroMemory(m_vDir, sizeof(_vec3));
	ZeroMemory(m_vUVAnimation_UV_Sum, sizeof(_vec2));
}

CBreakEffect::~CBreakEffect()
{
}

HRESULT CBreakEffect::Ready_GameObject(_vec3* pPos, _vec3* pAngle, BASE_INFO* pBaseInfo)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	Set_Info(pPos, pAngle, pBaseInfo);

	return S_OK;
}

_int CBreakEffect::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
		return Engine::OBJ_DEAD;
	if (m_pParentTransformCom != nullptr)
	{
		m_pTransformCom->Set_Pos(m_pParentTransformCom->Get_Info(Engine::INFO_POS));
	}
	if (m_bLoop)
	{
		if (m_dFrame > m_dMaxFrame)
			m_dFrame = 0;
		if ((m_dbTime += dTimeDelta) > m_tEffectInfo.fDeadTime)
		{
			m_bDissolveStart = true;
		}
		m_dFrame += dTimeDelta * m_fFrameSpeed;
	}
	else
	{
		if ((m_dFrame += dTimeDelta * m_fFrameSpeed) > m_dMaxFrame)
		{
			m_bDissolveStart = true;
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
	Transform(dTimeDelta);
	CBaseEffect::Update_GameObject(dTimeDelta);

	BillBoard();

	return Engine::NO_EVENT;
}

_int CBreakEffect::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CBaseEffect::LateUpdate_GameObject(dTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	return Engine::NO_EVENT;
}

void CBreakEffect::Render_Geometry(const _double & dTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	if (m_bDissolveStart)
	{
		Engine::SetTexture(pEffect, "g_DissolveTexture", 6);
		Engine::SetTexture(pEffect, "g_DissolveEdgeTexture", 2);
		m_dDissolveCountDown += dTimeDelta * 0.5;
		pEffect->SetFloat("fTime", m_dDissolveCountDown);
		m_iPass = 27;
		if (m_dDissolveCountDown > 1)
			m_bIsDead = true;
	}
	_uint iPassMax = 0;

	_float	fU = ((_uint)m_dFrame % (_uint)(m_tTextureInfo.vAnimationU.x)) / (m_tTextureInfo.vAnimationU.x);
	_float	fV = ((_uint)m_dFrame / (_uint)(m_tTextureInfo.vAnimationU.x)) / (m_tTextureInfo.vAnimationV.x);
	m_vUVAnimation_UV_Sum.x += _float(m_tTextureInfo.vAnimationU.y * dTimeDelta);
	m_vUVAnimation_UV_Sum.y += _float(m_tTextureInfo.vAnimationV.y * dTimeDelta);
	pEffect->SetVector("g_vUV_U", &_vec4(fU, 1 / m_tTextureInfo.vAnimationU.x, m_vUVAnimation_UV_Sum.x, 0.f));
	pEffect->SetVector("g_vUV_V", &_vec4(fV, 1 / m_tTextureInfo.vAnimationV.x, m_vUVAnimation_UV_Sum.y, 0.f));


	Engine::SetTexture(pEffect, "g_MaskingTexture", 23);

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(m_iPass);
	m_pBufferCom->Render_Buffer();
	pEffect->EndPass();
	pEffect->End();
	pEffect->SetVector("g_vUV_U", &_vec4(0.f, 1.f, 0.f, 0.f));
	pEffect->SetVector("g_vUV_V", &_vec4(0.f, 1.f, 0.f, 0.f));
	pEffect->SetTexture("g_MaskingTexture", nullptr);
	pEffect->SetVector("g_vRGBA", &_vec4(1.f, 1.f, 1.f, 1.f));

	Engine::Safe_Release(pEffect);

	m_pGraphicDev->SetTexture(0, nullptr);

	CBaseObject::Render_Geometry(dTimeDelta);
}

void CBreakEffect::Set_Info(_vec3* pPos, _vec3* pAngle, BASE_INFO* pBaseInfo)
{
	ResetTextureInfo();
	ResetEffectInfo();

	m_pTransformCom->Set_Pos(pPos);
	m_pTransformCom->Set_Angle(pAngle);
	m_dFrame = 0.;
	m_dbTime = 0.;
	if (pBaseInfo != nullptr)
		m_tBaseInfo = *pBaseInfo;
	m_pParentTransformCom = nullptr;

	m_pTransformCom->Set_Scale(100.f, 100.f, 100.f);
	m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
	m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
	m_tEffectInfo.fDeadTime = 0.5f;
	m_fFrameSpeed = 1.f;
	m_iPass = 9;
	m_bLoop = true;

	m_dMaxFrame = m_tTextureInfo.vAnimationU.x *  m_tTextureInfo.vAnimationV.x;
	m_pTransformCom->Update_Component(0.f);
	BillBoard();
}

HRESULT CBreakEffect::Clone_Component()
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

	//Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Break"));
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

HRESULT CBreakEffect::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	// 객체의 기본 텍스쳐
	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", 0);

	return S_OK;
}

void CBreakEffect::Transform(const _double& dTimeDelta)
{
	//if (TEXTURE_TS_SMOKE0 == m_eTextureEffect
	//	|| TEXTURE_TS_SMOKE1 == m_eTextureEffect)
	//{
	//	m_fTS_Alpha -= _float(dTimeDelta*0.4);
	//	m_fTS_Scale += _float(dTimeDelta * 10);
	//	m_pTransformCom->Set_Scale(m_fTS_Scale, m_fTS_Scale, m_fTS_Scale);
	//}
	//else if (m_eTextureEffect == TEXTURE_ORB_FLARE || m_eTextureEffect == TEXTURE_ORB_FLARE2 )
	//{
	//	_vec3 vScaling = _vec3(_float(dTimeDelta * 50), _float(dTimeDelta * 50), _float(dTimeDelta * 50));
	//	m_pTransformCom->Change_Scale(&vScaling);
	//}
	//else if (m_eTextureEffect == TEXTURE_ORB_FLARE3)
	//{
	//	_vec3 vScaling = _vec3(_float(dTimeDelta * 400), _float(dTimeDelta * 400), _float(dTimeDelta * 400));
	//	m_pTransformCom->Change_Scale(&vScaling);
	//}
	//else if (m_eTextureEffect == TEXTURE_BLINK_SINGLE)
	//{
	//	_vec3 vScaling = _vec3(-_float(dTimeDelta * 300), -_float(dTimeDelta * 300), -_float(dTimeDelta * 300));
	//	if (!m_pTransformCom->Change_Scale(&vScaling))
	//		m_dbTime = m_tEffectInfo.fDeadTime + 1.f; // 강제로 죽임
	//}
	//else if (m_eTextureEffect == TEXTURE_ORB_FLARE4)
	//{
	//	m_fTS_Alpha -= _float(dTimeDelta);
	//	_vec3 vScaling = _vec3(_float(dTimeDelta * 150), _float(dTimeDelta * 150), _float(dTimeDelta * 150));
	//	m_pTransformCom->Change_Scale(&vScaling);
	//}
}

void CBreakEffect::ResetTextureInfo()
{
	m_tTextureInfo.fDeadTime = 1.f;
	m_tTextureInfo.bIsBillboard = true;
	m_tTextureInfo.vScale = _vec3(1.f, 1.f, 1.f);
	m_tTextureInfo.vAnimationU = _vec2(1.f, 0.f);
	m_tTextureInfo.vAnimationV = _vec2(1.f, 0.f);
	ZeroMemory(&m_vUVAnimation_UV_Sum,sizeof(_vec2));
}

CBreakEffect * CBreakEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, BASE_INFO * pBaseInfo)
{
	CBreakEffect* pInstance = new CBreakEffect(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pBaseInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBreakEffect::Free()
{
	CBaseEffect::Free();
}
