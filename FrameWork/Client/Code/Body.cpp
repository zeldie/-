#include "stdafx.h"
#include "Body.h"


CBody::CBody(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicMeshObject(pGraphicDev),
	m_pTop(nullptr),
	m_pPants(nullptr),
	m_pFoot(nullptr),
	m_pHands(nullptr),
	m_pFrontHair(nullptr),
	m_pTailHair(nullptr),
	m_pBackHair(nullptr),
	m_pSideHair(nullptr),
	m_iAnimationNum(1),
	m_bIsHead(true),
	m_iBrowNum(0),
	m_iEyeNum(0),
	m_pNormalShaderCom(nullptr),
	m_bRenderNon(true)
{
	ZeroMemory(&m_vBodyColor, sizeof(_vec4));
	ZeroMemory(&m_vBrowUV, sizeof(_vec2));
}


CBody::~CBody()
{
}

_uint CBody::Get_FrontHairIndex()
{	
	return m_pFrontHair->Get_FrontHairIndex();	
}

_uint CBody::Get_TailHairIndex()
{
	return m_pTailHair->Get_TailHairIndex();
}

_uint CBody::Get_BackHairIndex()
{
	return m_pBackHair->Get_BackHairIndex();
}

_uint CBody::Get_SideHairIndex()
{
	return m_pSideHair->Get_SideHairIndex();
}

_uint CBody::Get_TopIndex()
{
	return m_pTop->Get_TopIndex();
}

_uint CBody::Get_PantsIndex()
{
	return m_pPants->Get_PantsIndex();
}

_uint CBody::Get_FootIndex()
{
	return m_pFoot->Get_FootIndex();
}

_uint CBody::Get_HandsIndex()
{
	return m_pHands->Get_HandsIndex();
}

HRESULT CBody::Link_Top(_uint iTopNum)
{
	m_pTop = CTop::Create(m_pGraphicDev, m_pTransformCom->Get_Info(Engine::INFO_POS), &_vec3(0.f, 0.f, 0.f), m_pTransformCom->Get_Scale(), iTopNum);
	return S_OK;
}

HRESULT CBody::Link_Pants(_uint iPantsNum)
{
	m_pPants= CPants::Create(m_pGraphicDev, m_pTransformCom->Get_Info(Engine::INFO_POS), &_vec3(0.f, 0.f, 0.f), m_pTransformCom->Get_Scale(), iPantsNum);
	return S_OK;
}

HRESULT CBody::Link_Foot(_uint iFootNum)
{
	m_pFoot = CFoot::Create(m_pGraphicDev, m_pTransformCom->Get_Info(Engine::INFO_POS), &_vec3(0.f, 0.f, 0.f), m_pTransformCom->Get_Scale(), iFootNum);
	return S_OK;
}

HRESULT CBody::Link_Hands(_uint iHandsNum)
{
	m_pHands = CHands::Create(m_pGraphicDev, m_pTransformCom->Get_Info(Engine::INFO_POS), &_vec3(0.f, 0.f, 0.f), m_pTransformCom->Get_Scale(), iHandsNum);
	return S_OK;
}

HRESULT CBody::Link_FrontHair(_uint iFrontHairNum)
{
	m_pFrontHair = CFrontHair::Create(m_pGraphicDev, m_pTransformCom->Get_Info(Engine::INFO_POS), &_vec3(0.f, 0.f, 0.f), m_pTransformCom->Get_Scale(), iFrontHairNum);
	return S_OK;
}

HRESULT CBody::Link_TailHair(_uint iTailHairNum)
{
	m_pTailHair = CTailHair::Create(m_pGraphicDev, m_pTransformCom->Get_Info(Engine::INFO_POS), &_vec3(0.f, 0.f, 0.f), m_pTransformCom->Get_Scale(), iTailHairNum);
	return S_OK;
}

HRESULT CBody::Link_BackHair(_uint iBackHairNum)
{
	m_pBackHair = CBackHair::Create(m_pGraphicDev, m_pTransformCom->Get_Info(Engine::INFO_POS), &_vec3(0.f, 0.f, 0.f), m_pTransformCom->Get_Scale(), iBackHairNum);
	return S_OK;
}

HRESULT CBody::Link_SideHair(_uint iSideHairNum)
{
	m_pSideHair = CSideHair::Create(m_pGraphicDev, m_pTransformCom->Get_Info(Engine::INFO_POS), &_vec3(0.f, 0.f, 0.f), m_pTransformCom->Get_Scale(), iSideHairNum);
	return S_OK;
}

HRESULT CBody::Change_Top(_uint iTopNum)
{
	if (m_pTop == nullptr)
		Link_Top(iTopNum);
	_double dTrackTime = m_pDynamicMeshCom->Get_TrackTime();
	_uint iCurrentTrack = m_pDynamicMeshCom->GetAniCtrl()->Get_CurrentTrack();
	_double dAccTime = m_pDynamicMeshCom->GetAniCtrl()->Get_AccTime();
	return m_pTop->Change_Top(iTopNum, iCurrentTrack, dTrackTime, m_iAnimationNum, dAccTime);
}

HRESULT CBody::Change_Pants(_uint iPantsNum)
{
	if (m_pPants == nullptr)
		Link_Pants(iPantsNum);
	_double dTrackTime = m_pDynamicMeshCom->Get_TrackTime();
	_uint iCurrentTrack = m_pDynamicMeshCom->GetAniCtrl()->Get_CurrentTrack();
	return m_pPants->Change_Pants(iPantsNum, iCurrentTrack, dTrackTime, m_iAnimationNum);
}

HRESULT CBody::Change_Foot(_uint iFootNum)
{
	if (m_pFoot == nullptr)
		Link_Foot(iFootNum);
	_double dTrackTime = m_pDynamicMeshCom->Get_TrackTime();
	_uint iCurrentTrack = m_pDynamicMeshCom->GetAniCtrl()->Get_CurrentTrack();
	return m_pFoot->Change_Foot(iFootNum, iCurrentTrack, dTrackTime, m_iAnimationNum);
}

HRESULT CBody::Change_Hands(_uint iHandsNum)
{
	if (m_pHands == nullptr)
		Link_Hands(iHandsNum);
	_double dTrackTime = m_pDynamicMeshCom->Get_TrackTime();
	_uint iCurrentTrack = m_pDynamicMeshCom->GetAniCtrl()->Get_CurrentTrack();
	return m_pHands->Change_Hands(iHandsNum, iCurrentTrack, dTrackTime, m_iAnimationNum);
}

HRESULT CBody::Change_FrontHair(_uint iFrontHairNum)
{
	if (m_pFrontHair == nullptr)
		Link_FrontHair(iFrontHairNum);
	_double dTrackTime = m_pDynamicMeshCom->Get_TrackTime();
	_uint iCurrentTrack = m_pDynamicMeshCom->GetAniCtrl()->Get_CurrentTrack();
	_double dAccTime = m_pDynamicMeshCom->GetAniCtrl()->Get_AccTime();
	return m_pFrontHair->Change_FrontHair(iFrontHairNum, iCurrentTrack, dTrackTime, m_iAnimationNum, dAccTime);
}

HRESULT CBody::Change_TailHair(_uint iTailHairNum)
{
	if (m_pTailHair == nullptr)
		Link_TailHair(iTailHairNum);
	_double dTrackTime = m_pDynamicMeshCom->Get_TrackTime();
	_uint iCurrentTrack = m_pDynamicMeshCom->GetAniCtrl()->Get_CurrentTrack();
	_double dAccTime = m_pDynamicMeshCom->GetAniCtrl()->Get_AccTime();
	return m_pTailHair->Change_TailHair(iTailHairNum, iCurrentTrack, dTrackTime, m_iAnimationNum, dAccTime);
}

HRESULT CBody::Change_BackHair(_uint iBackHairNum)
{
	if (m_pBackHair == nullptr)
		Link_TailHair(iBackHairNum);
	_double dTrackTime = m_pDynamicMeshCom->Get_TrackTime();
	_uint iCurrentTrack = m_pDynamicMeshCom->GetAniCtrl()->Get_CurrentTrack();
	_double dAccTime = m_pDynamicMeshCom->GetAniCtrl()->Get_AccTime();
	return m_pBackHair->Change_BackHair(iBackHairNum, iCurrentTrack, dTrackTime, m_iAnimationNum, dAccTime);
}

HRESULT CBody::Change_SideHair(_uint iSideHairNum)
{
	if (m_pSideHair == nullptr)
		Link_SideHair(iSideHairNum);
	_double dTrackTime = m_pDynamicMeshCom->Get_TrackTime();
	_uint iCurrentTrack = m_pDynamicMeshCom->GetAniCtrl()->Get_CurrentTrack();
	_double dAccTime = m_pDynamicMeshCom->GetAniCtrl()->Get_AccTime();
	return m_pSideHair->Change_SideHair(iSideHairNum, iCurrentTrack, dTrackTime, m_iAnimationNum, dAccTime);
}

void CBody::Change_HairColor(_vec4 tHairColor)
{
	if (m_pFrontHair != nullptr)
		m_pFrontHair->Change_HairColor(tHairColor);
	if (m_pTailHair != nullptr)
		m_pTailHair->Change_HairColor(tHairColor);
	if (m_pSideHair != nullptr)
		m_pSideHair->Change_HairColor(tHairColor);
	if (m_pBackHair != nullptr)
		m_pBackHair->Change_HairColor(tHairColor);
	return;
}

void CBody::Change_Brow(_uint iBroNum)
{
	if (iBroNum >= 4)
		return;
	switch (iBroNum)
	{
	case 0:
		m_vBrowUV = _vec2(0.f, 0.f);
		break;
	case 1:
		m_vBrowUV = _vec2(0.f, 0.125f);
		break;
	case 2:
		m_vBrowUV = _vec2(0.5f, 0.125f);
		break;
	case 3:
		m_vBrowUV = _vec2(0.5f, 0.f);
		break;
	default:
		break;
	}
	m_iBrowNum = iBroNum;
}

void CBody::Change_Eye(_uint iEyeNum)
{
	if (iEyeNum >= m_pTextureCom[EYE_D]->Get_TextureCnt())
		return;
	m_iEyeNum = iEyeNum;
}

void CBody::Release_Pants()
{
	Engine::Safe_Release(m_pPants);
	m_pPants = nullptr;
}

void CBody::Update_Parts(const _double & dTimeDelta, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iAnimationNum)
{
	if (m_pTop != nullptr)
		m_pTop->Update_GameObject(dTimeDelta, pPos, pAngle, pScale, iAnimationNum);
	if (m_pPants != nullptr)
		m_pPants->Update_GameObject(dTimeDelta, pPos, pAngle, pScale, iAnimationNum);
	if (m_pFoot != nullptr)
		m_pFoot->Update_GameObject(dTimeDelta, pPos, pAngle, pScale, iAnimationNum);
	if (m_pHands != nullptr)
		m_pHands->Update_GameObject(dTimeDelta, pPos, pAngle, pScale, iAnimationNum);
	if (m_pFrontHair != nullptr)
		m_pFrontHair->Update_GameObject(dTimeDelta, pPos, pAngle, pScale, iAnimationNum);
	if (m_pTailHair != nullptr)
		m_pTailHair->Update_GameObject(dTimeDelta, pPos, pAngle, pScale, iAnimationNum);
	if (m_pSideHair != nullptr)
		m_pSideHair->Update_GameObject(dTimeDelta, pPos, pAngle, pScale, iAnimationNum);
	if (m_pBackHair != nullptr)
		m_pBackHair->Update_GameObject(dTimeDelta, pPos, pAngle, pScale, iAnimationNum);
}

void CBody::LateUpdate_Parts(const _double & dTimeDelta)
{
	if (m_pTop != nullptr)
		m_pTop->LateUpdate_GameObject(dTimeDelta);
	if (m_pPants != nullptr)
		m_pPants->LateUpdate_GameObject(dTimeDelta);
	if (m_pFoot != nullptr)
		m_pFoot->LateUpdate_GameObject(dTimeDelta);
	if (m_pHands != nullptr)
		m_pHands->LateUpdate_GameObject(dTimeDelta);
	if (m_pFrontHair != nullptr)
		m_pFrontHair->LateUpdate_GameObject(dTimeDelta);
	if (m_pTailHair != nullptr)
		m_pTailHair->LateUpdate_GameObject(dTimeDelta);
	if (m_pSideHair != nullptr)
		m_pSideHair->LateUpdate_GameObject(dTimeDelta);
	if (m_pBackHair != nullptr)
		m_pBackHair->LateUpdate_GameObject(dTimeDelta);
}

HRESULT CBody::Ready_GameObject(SEX eSexType, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	if (FAILED(Clone_Component(eSexType)))
		return E_FAIL;
	m_pTransformCom->Set_Info(pPos, Engine::INFO_POS);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);
	m_pDynamicMeshCom->Set_AnimationSet(1);
	Link_Hands(0);
	Link_Top(0);
	Link_Foot(0);
	Link_Pants(0);
	Link_FrontHair(0);
	Link_BackHair(0);
	Link_TailHair(0);
	Link_SideHair(0);

	m_vBodyColor = _vec4(1.f, 0.f, 0.f, 1.f);
	Change_HairColor(_vec4(1.f, 1.f, 0.f, 1.f));
	return S_OK;
}

_int CBody::Update_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::Update_GameObject(dTimeDelta);
	if (Engine::KeyPressing(DIK_LMENU))
	{
		if (Engine::KeyDown(DIK_F6))
			Change_Brow(0);
		if (Engine::KeyDown(DIK_F7))
			Change_Brow(1);
		if (Engine::KeyDown(DIK_F8))
			Change_Brow(2);
		if (Engine::KeyDown(DIK_F9))
		{
			Release_Pants();
		}
			
		if (Engine::KeyDown(DIK_F10))
		{
			Change_Brow(2);
		}
	}
	m_pDynamicMeshCom->Set_AnimationSet(m_iAnimationNum);

	if (2 == m_iAnimationNum)
	{
		if (m_pDynamicMeshCom->Is_AnimationSetFinish(0.2))
		{
			m_pDynamicMeshCom->GetAniCtrl()->Set_Stop(true);
			CUIMgr::GetInstance()->Set_FinishCustomize();
		}
	}

	Update_Parts(dTimeDelta, m_pTransformCom->Get_Info(Engine::INFO_POS), m_pTransformCom->Get_Angle(), m_pTransformCom->Get_Scale(), m_iAnimationNum );
	return	Engine::NO_EVENT;
}

_int CBody::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CDynamicMeshObject::LateUpdate_GameObject(dTimeDelta);
	LateUpdate_Parts(dTimeDelta);


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	m_pDynamicMeshCom->Play_AnimationSet(dTimeDelta);
	return	Engine::NO_EVENT;
}

void CBody::Render_Geometry(const _double & dTimeDelta)
{
	
	LPD3DXEFFECT	pEffect = nullptr;
    if (m_bRenderNon)
	{
		pEffect = m_pShaderCom->Get_EffectHandle();

	}
	else
	{
		pEffect = m_pNormalShaderCom->Get_EffectHandle();
		
	}
		
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);
	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	m_bIsHead = true;
	for (auto& iter : *plistMeshContainer)
	{
		_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
		
		for (_ulong i = 0; i < dwSubsetNum; ++i)
		{
			if (m_bIsHead)
			{
				if (i == 0 && m_bRenderNon) // 倔奔
				{
					m_pDynamicMeshCom->Render_Meshes_Begin(iter);
					pEffect->SetVector("vChangeColor", &_vec4(1.f, 1.f, 1.f, 1.f));
					pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
					pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
					pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
					pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
					pEffect->BeginPass(0);
					pEffect->CommitChanges();
					m_pDynamicMeshCom->Render_Meshes(iter, i);
					pEffect->EndPass();
					m_pDynamicMeshCom->Render_Meshes_End(iter);
				}
				else if (i == 1 && !m_bRenderNon) // 传界
				{
					m_pDynamicMeshCom->Render_Meshes_Begin(iter);
					pEffect->SetValue("ChangeUV", (void*)&m_vBrowUV, sizeof(_vec2));
					m_pTextureCom[BROW]->SetTexture(pEffect, "g_DiffuseTexture", 0);
					pEffect->SetTexture("g_NormalTexture", nullptr);
					pEffect->SetTexture("g_SpecularTexture", nullptr);
					pEffect->SetTexture("g_EmmisiveTexture", nullptr);
					pEffect->BeginPass(28);
					pEffect->CommitChanges();
					m_pDynamicMeshCom->Render_Meshes(iter, i);
					pEffect->EndPass();
					m_pDynamicMeshCom->Render_Meshes_End(iter);
				}
				else if (i == 2 && m_bRenderNon) // 传 哭率
				{
					m_pDynamicMeshCom->Render_Meshes_Begin(iter);
					pEffect->SetVector("vChangeColor", &_vec4(1.f, 1.f, 1.f, 1.f));
					m_pTextureCom[EYE_D]->SetTexture(pEffect, "g_DiffuseTexture", m_iEyeNum);
					m_pTextureCom[EYE_N]->SetTexture(pEffect, "g_NormalTexture", m_iEyeNum);
					pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
					pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
					pEffect->BeginPass(0);
					pEffect->CommitChanges();
					m_pDynamicMeshCom->Render_Meshes(iter, i);
					pEffect->EndPass();
					m_pDynamicMeshCom->Render_Meshes_End(iter);
				}
				else if (i == 3 && m_bRenderNon) // 传 坷弗率
				{
					m_pDynamicMeshCom->Render_Meshes_Begin(iter);
					pEffect->SetVector("vChangeColor", &_vec4(1.f, 1.f, 1.f, 1.f));
					m_pTextureCom[EYE_D]->SetTexture(pEffect, "g_DiffuseTexture", m_iEyeNum);
					m_pTextureCom[EYE_N]->SetTexture(pEffect, "g_NormalTexture", m_iEyeNum);
					pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
					pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
					pEffect->BeginPass(0);
					pEffect->CommitChanges();
					m_pDynamicMeshCom->Render_Meshes(iter, i);
					pEffect->EndPass();
					m_pDynamicMeshCom->Render_Meshes_End(iter);
				}
			}
			else
			{
				if (i == 0 && m_bRenderNon) // 个
				{
					m_pDynamicMeshCom->Render_Meshes_Begin(iter);
					pEffect->SetVector("vDyeColor", &_vec4(m_vBodyColor));
					//pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
					pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
					pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
					pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
					pEffect->BeginPass(18);
					pEffect->CommitChanges();
					m_pDynamicMeshCom->Render_Meshes(iter, i);
					pEffect->EndPass();
					m_pDynamicMeshCom->Render_Meshes_End(iter);
				}
				else if (i == 1 && m_bRenderNon) // 加渴
				{
					m_pDynamicMeshCom->Render_Meshes_Begin(iter);
					pEffect->SetVector("vChangeColor", &_vec4(1.f, 1.f, 1.f, 1.f));
					pEffect->SetTexture("g_DiffuseTexture", iter->ppDiffuseTexture[i]);
					pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
					pEffect->SetTexture("g_SpecularTexture", iter->ppSpecularTexture[i]);
					pEffect->SetTexture("g_EmmisiveTexture", iter->ppEmmisiveTexture[i]);
					pEffect->BeginPass(0);
					pEffect->CommitChanges();
					m_pDynamicMeshCom->Render_Meshes(iter, i);
					pEffect->EndPass();
					m_pDynamicMeshCom->Render_Meshes_End(iter);
				}
			}
		}
		m_bIsHead = false;

	}
	pEffect->End();
	if (m_bRenderNon == true)
		m_bRenderNon = false;
	else
		m_bRenderNon = true;
	Engine::Safe_Release(pEffect);
}

void CBody::Render_PostEffect(const _double & dTimeDelta)
{
	////Shader
	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//if (pEffect == nullptr)
	//	return;
	//Engine::Safe_AddRef(pEffect);
	//if (FAILED(Setup_ShaderProps(pEffect)))
	//	return;
	//_uint iPassMax = 0;

	//pEffect->Begin(&iPassMax, 0);

	//list<Engine::D3DXMESHCONTAINER_DERIVED*>* plistMeshContainer = m_pDynamicMeshCom->Get_MeshContainerlist();
	//for (auto& iter : *plistMeshContainer)
	//{
	//	_ulong dwSubsetNum = m_pDynamicMeshCom->Get_SubsetNum(iter);
	//	m_pDynamicMeshCom->Render_Meshes_Begin(iter);
	//	for (_ulong i = 0; i < dwSubsetNum; ++i)
	//	{
	//		pEffect->SetTexture("g_DiffuseTexture", nullptr);
	//		pEffect->SetTexture("g_NormalTexture", iter->ppNormalTexture[i]);
	//		pEffect->BeginPass(1);
	//		pEffect->CommitChanges();
	//		m_pDynamicMeshCom->Render_Meshes(iter, i);
	//		pEffect->EndPass();
	//	}
	//	m_pDynamicMeshCom->Render_Meshes_End(iter);
	//}
	//pEffect->End();
	//Engine::Safe_Release(pEffect);
}

HRESULT CBody::Clone_Component(SEX eSexType)
{
	Engine::CComponent* pComponent = nullptr;
	wstring wstrBodyID = L"";
	if (eSexType == MALE)
		wstrBodyID = L"Mesh_Male";
	else
		wstrBodyID = L"Mesh_Female";
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_Body"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Mesh"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	//Shader2
	pComponent = m_pNormalShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER2, pComponent);

	//Texture
	m_pTextureCom[EYE_D] = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Customizing_Eye_D"));
	if (m_pTextureCom[EYE_D] == nullptr)
		return E_FAIL;
	m_pTextureCom[EYE_N] = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_Customizing_Eye_N"));
	if (m_pTextureCom[EYE_N] == nullptr)
		return E_FAIL;
	m_pTextureCom[BROW] = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_CustomizingBrow"));
	if (m_pTextureCom[BROW] == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CBody::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	return S_OK;
}

CBody * CBody::Create(LPDIRECT3DDEVICE9 pGraphicDev, SEX eSexType, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CBody* pInstance = new CBody(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(eSexType, pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CBody::Free()
{
	CDynamicMeshObject::Free();
	Engine::Safe_Release(m_pTop);
	Engine::Safe_Release(m_pPants);
	Engine::Safe_Release(m_pFoot);
	Engine::Safe_Release(m_pHands);
	Engine::Safe_Release(m_pFrontHair);
	Engine::Safe_Release(m_pBackHair);
	Engine::Safe_Release(m_pSideHair);
	Engine::Safe_Release(m_pTailHair);
	for (_uint i = 0; i < TEXTURE_CHANGE_END; ++i)
	{
		Engine::Safe_Release(m_pTextureCom[i]);
	}
}
